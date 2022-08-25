#include "NotifySMS.h"
#include "NotifySMSStyle.h"
#include "NotifySMSCommands.h"
#include "Twilio.h"

#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "HttpModule.h"
#include "ToolMenus.h"
#include "SNotifyOptionsWidget.h"
#include "NotificationFilters.h"

#include <regex>

static const FName NotifySMSTabName("Email Alerts");

#define LOCTEXT_NAMESPACE "FNotifySMSModule"

void FNotifySMSModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FNotifySMSStyle::Initialize();
	FNotifySMSStyle::ReloadTextures();

	FNotifySMSCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FNotifySMSCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FNotifySMSModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FNotifySMSModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(NotifySMSTabName, FOnSpawnTab::CreateRaw(this, &FNotifySMSModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FNotifySMSTabTitle", "Email Alerts"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FNotifySMSModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FNotifySMSStyle::Shutdown();

	FNotifySMSCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(NotifySMSTabName);

}

TSharedRef<SDockTab> FNotifySMSModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	TickDelegate = FTickerDelegate::CreateRaw(this, &FNotifySMSModule::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate, 1.0);

	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FNotifySMSModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("NotifySMS.cpp"))
	);

	TSharedRef<SDockTab> PluginTab = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			[
				SNew(SBorder)
				.Padding(FMargin(5.0f))
				.BorderBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f))
				[
					SNew(SNotifyOptionsWidget)
					.EmailCallback(FOnEmailChanged::CreateRaw(this, &FNotifySMSModule::SetEmail))
					.FiltersCallback(FOnFilterStateChanged::CreateRaw(this, &FNotifySMSModule::SetNotification))
				]
			]
		];

	PluginTab->SetOnTabClosed(SDockTab::FOnTabClosedCallback::CreateRaw(this, &FNotifySMSModule::OnClosePluginTab));

	EmailAddress.Reset();

	return PluginTab;
}

bool FNotifySMSModule::SetEmail(const FText& InText)
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

void FNotifySMSModule::SetNotification(uint32 Index, bool IsEnabled)
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

void FNotifySMSModule::OnClosePluginTab(TSharedRef<SDockTab> TabBeingClosed)
{
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	TabBeingClosed->SetOnTabClosed(SDockTab::FOnTabClosedCallback());
}

void FNotifySMSModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(NotifySMSTabName);
}

void FNotifySMSModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FNotifySMSCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FNotifySMSCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

bool FNotifySMSModule::Tick(float DeltaTime)
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
					//SendEmail(NotificationMessage);
					UE_LOG(LogTemp, Display, TEXT("Notification: '%s' Sent to: '%s'"), *FString(Window->GetContent()->GetAccessibleSummary().ToString()), *FString(EmailAddress));
				}
			}
			
			Window->SetTitle(FText::FromString("ok"));
		}
	}
	return true;
}

void FNotifySMSModule::SendEmail(const FString& NotificationMessage)
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

bool FNotifySMSModule::IsEmailValid(const FString& Email)
{
	// define a regular expression
	const std::regex pattern
	("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	// try to match the string with the regular expression
	return std::regex_match(TCHAR_TO_UTF8(*Email), pattern);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNotifySMSModule, NotifySMS)