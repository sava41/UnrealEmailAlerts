#pragma once

#include "CoreMinimal.h"

struct NotificationType {
	const FString Description;
	const FString SearchKeyword;
};

const TArray<NotificationType> Notifications = 
{
	{ "Shaders compiled", "shader"},
	{ "Code compiled",  "coding"},
	{ "Level compiled", "level"}
};