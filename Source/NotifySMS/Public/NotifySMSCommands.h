// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "NotifySMSStyle.h"

class FNotifySMSCommands : public TCommands<FNotifySMSCommands>
{
public:

	FNotifySMSCommands()
		: TCommands<FNotifySMSCommands>(TEXT("NotifySMS"), NSLOCTEXT("Contexts", "NotifySMS", "NotifySMS Plugin"), NAME_None, FNotifySMSStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};