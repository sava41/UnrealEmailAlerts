// Copyright Epic Games, Inc. All Rights Reserved.

#include "EmailAlertsStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FEmailAlertsStyle::StyleInstance = nullptr;

void FEmailAlertsStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FEmailAlertsStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FEmailAlertsStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("EmailAlertsStyle"));
	return StyleSetName;
}

void FEmailAlertsStyle::SetTabIcon(TSharedRef<SDockTab> PluginTab)
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("EmailAlertsStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("EmailAlerts")->GetBaseDir() / TEXT("Resources"));

	PluginTab->SetTabIcon(new IMAGE_BRUSH_SVG(TEXT("MailIcon"), FVector2D(20.0f, 20.0f)));
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FEmailAlertsStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("EmailAlertsStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("EmailAlerts")->GetBaseDir() / TEXT("Resources"));

	Style->Set("EmailAlerts.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("MailIcon"), Icon20x20));

	return Style;
}

void FEmailAlertsStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FEmailAlertsStyle::Get()
{
	return *StyleInstance;
}
