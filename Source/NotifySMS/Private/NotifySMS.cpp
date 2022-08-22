#include "NotifySMS.h"
#include "NotifySMSStyle.h"
#include "NotifySMSCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "HttpModule.h"
#include "ToolMenus.h"
#include "SNotifyOptionsWidget.h"


static const FName NotifySMSTabName("NotifySMS");

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
		.SetDisplayName(LOCTEXT("FNotifySMSTabTitle", "NotifySMS"))
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
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);

	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FNotifySMSModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("NotifySMS.cpp"))
		);

	TSharedRef<SDockTab> PluginTab = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			[
				SNew(SNotifyOptionsWidget)
			]
		];

	PluginTab->SetOnTabClosed(SDockTab::FOnTabClosedCallback::CreateRaw(this, &FNotifySMSModule::OnClosePluginTab));

	return PluginTab;
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
	//UE_LOG(LogTemp, Warning, TEXT("Lel"));
	TArray< TSharedRef<SWindow> > NotificationWindows;
	FSlateNotificationManager::Get().GetWindows(NotificationWindows);
	for (TSharedRef<SWindow> Window : NotificationWindows)
	{
		TSharedRef<const SNotificationList> NotificationList = StaticCastSharedRef<const SNotificationList>(Window->GetContent());

		if (Window->GetType() == EWindowType::Notification && Window->GetTitle().IsEmpty())
		{
			SendEmail();
			Window->SetTitle(FText::FromString("lol"));
			UE_LOG(LogTemp, Warning, TEXT("lol %s"), *FString(Window->GetContent()->GetChildren()->GetChildAt(0)->ToString()));
		}
	}
	return true;
}

void FNotifySMSModule::SendEmail()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpRequest = FHttpModule::Get().CreateRequest();
	httpRequest->SetURL(TEXT("https://api.sendgrid.com/v3/mail/send"));

	httpRequest->SetVerb(TEXT("POST"));
	httpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer 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"));
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	httpRequest->SetContentAsString(TEXT("{\"personalizations\": [{\"to\": [{\"email\": \"sava41@gmail.com\"}]}],\"from\": {\"email\": \"sava41@gmail.com\"},\"subject\": \"Unreal Engine Did Something\",\"content\": [{\"type\": \"text / plain\", \"value\": \"lol\"}]}"));

	httpRequest->ProcessRequest();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNotifySMSModule, NotifySMS)