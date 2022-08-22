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
			.HintText(LOCTEXT("email", "sava41@gmail.com"))
			.OnTextCommitted(this, &SNotifyOptionsWidget::VerifySMSNumber)
			.OnTextChanged(this, &SNotifyOptionsWidget::FormatSMSNumber)
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

void SNotifyOptionsWidget::VerifySMSNumber(const FText& InText, ETextCommit::Type InCommitType)
{
	//if (PhoneNumberTextBox.IsValid())
	//{
	//	PhoneNumberTextBox->SetText(FText::FromString("lol"));
	//}

	return;
}

void SNotifyOptionsWidget::FormatSMSNumber(const FText& InText)
{
	
	if (PhoneNumberTextBox.IsValid())
	{
		PhoneNumberTextBox->SetText(FText::FromString("lel"));
	}
	return;
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