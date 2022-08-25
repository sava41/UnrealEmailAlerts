// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EmailAlertsStyle.h"

class FEmailAlertsCommands : public TCommands<FEmailAlertsCommands>
{
public:

	FEmailAlertsCommands()
		: TCommands<FEmailAlertsCommands>(TEXT("EmailAlerts"), NSLOCTEXT("Contexts", "EmailAlerts", "EmailAlerts Plugin"), NAME_None, FEmailAlertsStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};