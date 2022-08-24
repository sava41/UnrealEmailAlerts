#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Containers/Ticker.h"

class FToolBarBuilder;
class FMenuBuilder;

class FNotifySMSModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();
	bool Tick(float DeltaTime);
	void SendEmail(const FString& NotificationMessage);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	void OnClosePluginTab(TSharedRef<SDockTab> TabBeingClosed);

	void SetEmail(const FText& InText, ETextCommit::Type InCommitType);
	void SetNotification(uint32 Index, bool IsEnabled);

	FTickerDelegate TickDelegate;
	FDelegateHandle TickDelegateHandle;

	FString EmailAddress = FString(TEXT("myemail@example.com"));

	TSet<int32> NotificationFilters;

	TSharedPtr<class FUICommandList> PluginCommands;
};
