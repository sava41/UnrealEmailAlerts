#include "SNotifyOptionsWidget.h"
#include <SlateOptMacros.h>
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/SBoxPanel.h"
#include "Framework/Notifications/NotificationManager.h"

#define LOCTEXT_NAMESPACE "SNotifyOptionsWidget"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNotifyOptionsWidget::Construct(const FArguments& InArgs)
{

	EmailCallback = InArgs._EmailCallback;
	FiltersCallback = InArgs._FiltersCallback;
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot().AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::Format(LOCTEXT("MyWidgetName", "{0}"), FText::FromName("Enter Email Address")))
		]
		+SVerticalBox::Slot().AutoHeight()
		[
			SAssignNew(PhoneNumberTextBox, SEditableTextBox)
			.HintText(LOCTEXT("email", "myemail@example.com"))
			.OnTextCommitted(this, &SNotifyOptionsWidget::OnTextCommitted)
		]
		//+SVerticalBox::Slot().AutoHeight()
		//[
		//	SNew(SButton)
		//	.Text(LOCTEXT("SendTestSMS", "Send Test SMS"))
		//	.ToolTipText(LOCTEXT("SendTestSMS_Tooltip", "Sends a test message to the entered phone number"))
		//]
		+SVerticalBox::Slot().AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::Format(LOCTEXT("MyWidgetName", "{0}"), FText::FromName("Select Notification Types")))
		]
		+SVerticalBox::Slot().AutoHeight()
		[
			SNew(SCheckBox)
		]
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SCheckBox)
		]
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SCheckBox)
		]
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SCheckBox)
		]
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SCheckBox)
		]
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SCheckBox)
		]
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SButton)
			.Text(LOCTEXT("SpawnNotification", "Debug Notification"))
			.ToolTipText(LOCTEXT("SpawnNotification_Tooltip", "Creates a super cool Notification!"))
			.OnClicked(this, &SNotifyOptionsWidget::SpawnNotification)
		]
	];
}

void SNotifyOptionsWidget::OnTextCommitted(const FText& InText, ETextCommit::Type InCommitType)
{
	EmailCallback.ExecuteIfBound(InText, InCommitType);
}

FReply SNotifyOptionsWidget::SpawnNotification()
{
	FNotificationInfo Info(LOCTEXT("SpawnNotification_Notification", "This is a notification."));
	Info.ExpireDuration = 5.0f;
	FSlateNotificationManager::Get().AddNotification(Info);

	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE