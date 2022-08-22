// Copyright Epic Games, Inc. All Rights Reserved.

#include "NotifySMSCommands.h"

#define LOCTEXT_NAMESPACE "FNotifySMSModule"

void FNotifySMSCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "NotifySMS", "Bring up NotifySMS window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
