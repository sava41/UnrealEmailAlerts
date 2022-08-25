#include "EmailAlerts.h"
#include "EmailAlertsStyle.h"
#include "EmailAlertsCommands.h"
#include "Twilio.h"

#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "HttpModule.h"
#include "ToolMenus.h"
#include "SOptionsWidget.h"
#include "NotificationFilters.h"

#include <regex>

static const FName EmailAlertsTabName("Email Alerts");

#define LOCTEXT_NAMESPACE "FEmailAlertsModule"

void FEmailAlertsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FEmailAlertsStyle::Initialize();
	FEmailAlertsStyle::ReloadTextures();

	FEmailAlertsCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FEmailAlertsCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FEmailAlertsModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FEmailAlertsModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(EmailAlertsTabName, FOnSpawnTab::CreateRaw(this, &FEmailAlertsModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FEmailAlertsTabTitle", "Email Alerts"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FEmailAlertsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FEmailAlertsStyle::Shutdown();

	FEmailAlertsCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(EmailAlertsTabName);

}

TSharedRef<SDockTab> FEmailAlertsModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	TickDelegate = FTickerDelegate::CreateRaw(this, &FEmailAlertsModule::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate, 1.0);

	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FEmailAlertsModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("EmailAlerts.cpp"))
	);

	TSharedRef<SDockTab> PluginTab = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.Padding(FMargin(5))
			[
				SNew(SNotifyOptionsWidget)
				.EmailCallback(FOnEmailChanged::CreateRaw(this, &FEmailAlertsModule::SetEmail))
				.FiltersCallback(FOnFilterStateChanged::CreateRaw(this, &FEmailAlertsModule::SetNotification))
			]
		];

	PluginTab->SetOnTabClosed(SDockTab::FOnTabClosedCallback::CreateRaw(this, &FEmailAlertsModule::OnClosePluginTab));

	EmailAddress.Reset();

	return PluginTab;
}

bool FEmailAlertsModule::SetEmail(const FText& InText)
{
	if (IsEmailValid(InText.ToString())) {
		EmailAddress = InText.ToString();
		return true;
	}
	else {
		EmailAddress.Reset();
		return false;
	}
}

void FEmailAlertsModule::SetNotification(uint32 Index, bool IsEnabled)
{
	if (Notifications.IsValidIndex(Index)) {
		if (IsEnabled) {
			NotificationFilters.Add(Index);
		}
		else {
			NotificationFilters.Remove(Index);
		}
	}
}

void FEmailAlertsModule::OnClosePluginTab(TSharedRef<SDockTab> TabBeingClosed)
{
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	TabBeingClosed->SetOnTabClosed(SDockTab::FOnTabClosedCallback());
}

void FEmailAlertsModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(EmailAlertsTabName);
}

void FEmailAlertsModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FEmailAlertsCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FEmailAlertsCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

bool FEmailAlertsModule::Tick(float DeltaTime)
{
	TArray< TSharedRef<SWindow> > NotificationWindows;
	FSlateNotificationManager::Get().GetWindows(NotificationWindows);
	for (TSharedRef<SWindow> Window : NotificationWindows)
	{
		if (Window->GetType() == EWindowType::Notification && Window->GetTitle().IsEmpty())
		{
			FString NotificationMessage = Window->GetContent()->GetAccessibleSummary().ToString();

			for (auto It = NotificationFilters.CreateConstIterator(); It; ++It)
			{
				if (NotificationMessage.Contains(Notifications[*It].SearchKeyword) && !EmailAddress.IsEmpty()) {
					SendEmail(NotificationMessage);
					UE_LOG(LogTemp, Display, TEXT("Notification: '%s' Sent to: '%s'"), *FString(Window->GetContent()->GetAccessibleSummary().ToString()), *FString(EmailAddress));
				}
			}
			
			Window->SetTitle(FText::FromString("ok"));
		}
	}
	return true;
}

void FEmailAlertsModule::SendEmail(const FString& NotificationMessage)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("https://api.sendgrid.com/v3/mail/send"));

	httpRequest->SetVerb(TEXT("POST"));
	httpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + FString(SENDGRID_API_KEY));
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	const FString Payload(TEXT("{\"personalizations\": [{ \
		\"to\": [{\"email\": \"" + EmailAddress + "\"}]}], \
		\"from\": {\"email\": \"" + FString(TWILIO_EMAIL) + "\"}, \
		\"subject\": \"Unreal Engine Did Something\", \
		\"content\": [{\"type\": \"text / plain\", \"value\": \"") 
		+ NotificationMessage.Replace(TEXT("\n"), TEXT("")) 
		+ TEXT("\"}]}"));

	httpRequest->SetContentAsString(Payload);

	httpRequest->ProcessRequest();
}

bool FEmailAlertsModule::IsEmailValid(const FString& Email)
{
	// define a regular expression
	const std::regex pattern
	("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	// try to match the string with the regular expression
	return std::regex_match(TCHAR_TO_UTF8(*Email), pattern);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEmailAlertsModule, EmailAlerts)