// Copyright Epic Games, Inc. All Rights Reserved.

#include "EmailAlertsCommands.h"

#define LOCTEXT_NAMESPACE "FEmailAlertsModule"

void FEmailAlertsCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "EmailAlerts", "Bring up EmailAlerts window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
