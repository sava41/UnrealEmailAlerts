#include "NotificationFilters.h"
#include "SOptionsWidget.h"
#include <SlateOptMacros.h>
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Math/Color.h"

#define LOCTEXT_NAMESPACE "SNotifyOptionsWidget"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNotifyOptionsWidget::Construct(const FArguments& InArgs)
{

	EmailCallback = InArgs._EmailCallback;
	FiltersCallback = InArgs._FiltersCallback;

	TSharedPtr<SScrollBox> Container = SNew(SScrollBox);

	Container->AddSlot()
	[
		SNew(STextBlock)
		.Font(DEFAULT_FONT("Regular", 12))
		.LineHeightPercentage(1.4)
		.Text(FText::Format(LOCTEXT("MyWidgetName", "{0}"), FText::FromName("Enter Email Address")))

	];
	
	Container->AddSlot()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.MaxWidth(220.0f)
		[
			SAssignNew(PhoneNumberTextBox, SEditableTextBox)
			.HintText(LOCTEXT("email", "myemail@example.com"))
			.OnTextCommitted(this, &SNotifyOptionsWidget::OnTextCommitted)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SAssignNew(EmailWarning, STextBlock)
			.ColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.0f, 0.0f))
			.Text(FText::Format(LOCTEXT("MyWidgetName", "{0}"), FText::FromName("	Invalid Email")))
		]
	];

	Container->AddSlot()
	.Padding(FMargin(0, 5, 0, 5))
	[
		SNew(SSeparator)
	];

	Container->AddSlot()
	[
		SNew(STextBlock)
		.Font(DEFAULT_FONT("Regular", 12))
		.LineHeightPercentage(1.4)
		.Text(FText::Format(LOCTEXT("MyWidgetName", "{0}"), FText::FromName("Select Notification Types")))
	];

	for (int Index = 0; Index < Notifications.Num(); Index++) {
		TSharedPtr<SCheckBox> Checkbox;

		Container->AddSlot()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.MaxWidth(200.0f)
			[
				SNew(STextBlock)
				.Text(FText::Format(LOCTEXT("MyWidgetName", "{0}"), FText::FromString(Notifications[Index].Description)))
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(Checkbox, SCheckBox)
				.OnCheckStateChanged(this, &SNotifyOptionsWidget::OnCheckStateChanged)
			]
		];

		FilterCheckboxes.Add(Checkbox);
	}
	
	Container->AddSlot()
	.Padding(FMargin(0, 5, 0, 5))
	[
		SNew(SSeparator)
	];

	Container->AddSlot()
	[
		SNew(STextBlock)
		.Font(DEFAULT_FONT("Regular", 12))
		.LineHeightPercentage(1.4)
		.Text(FText::Format(LOCTEXT("MyWidgetName", "{0}"), FText::FromName("Spawn Test Notification")))
	];

	Container->AddSlot()
	[
		SNew(SButton)
		.Text(LOCTEXT("SpawnNotification", "Spawn"))
		.ToolTipText(LOCTEXT("SpawnNotification_Tooltip", "Debug notification"))
		.OnClicked(this, &SNotifyOptionsWidget::SpawnNotification)
	];

	Container->AddSlot()
	.Padding(FMargin(0, 5, 0, 5))
	[
		SNew(SSeparator)

	];

	Container->AddSlot()
	[
		SNew(STextBlock)
		.Font(DEFAULT_FONT("Regular", 12))
		.LineHeightPercentage(1.4)
		.Text(FText::Format(LOCTEXT("MyWidgetName", "{0}"), FText::FromName("Notes")))
	];

	Container->AddSlot()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(STextBlock)
			.Text(FText::Format(LOCTEXT("MyWidgetName", "{0}"), FText::FromName(
				"Plugin will only monitor notifcations when plugin tab is open\nEmails may deliver to the spam folder")))
		]
	];
	
	ChildSlot[ Container->AsShared() ];
}

void SNotifyOptionsWidget::OnTextCommitted(const FText& InText, ETextCommit::Type InCommitType)
{	
	if (!EmailCallback.IsBound()) return;

	if (EmailCallback.Execute(InText)) {
		EmailWarning->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.0f, 0.0f));
	} 
	else {
		EmailWarning->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

void SNotifyOptionsWidget::OnCheckStateChanged(ECheckBoxState InState)
{
	for (int Index = 0; Index < FilterCheckboxes.Num(); Index++) {
		FiltersCallback.ExecuteIfBound(Index, FilterCheckboxes[Index]->IsChecked());
	}
}

FReply SNotifyOptionsWidget::SpawnNotification()
{
	FNotificationInfo Info(LOCTEXT("SpawnNotification_Notification", "Test Notification"));
	Info.ExpireDuration = 5.0f;
	FSlateNotificationManager::Get().AddNotification(Info);

	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE