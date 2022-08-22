// Copyright Epic Games, Inc. All Rights Reserved.

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
	void SendEmail(const FString& NotificationString);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	void OnClosePluginTab(TSharedRef<SDockTab> TabBeingClosed);

	FTickerDelegate TickDelegate;
	FDelegateHandle TickDelegateHandle;

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
