#pragma once

#include "CoreMinimal.h"

struct NotificationType {
	FString Description;
	FString SearchKeyword;
};

const TArray<NotificationType> Notifications = 
{
	{ "Shaders compiled", "shader"},
	{ "Code compiled",  "coding"},
	{ "Level compiled", "level"},
	{ "Test notification", "Test Notification"}
};