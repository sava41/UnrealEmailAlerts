#include "SNotifyOptionsWidget.h"
#include <SlateOptMacros.h>
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SBorder.h"
#include "Framework/Notifications/NotificationManager.h"
#include "NotificationFilters.h"
#include "Math/Color.h"

#define LOCTEXT_NAMESPACE "SNotifyOptionsWidget"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNotifyOptionsWidget::Construct(const FArguments& InArgs)
{

	EmailCallback = InArgs._EmailCallback;
	FiltersCallback = InArgs._FiltersCallback;

	TSharedPtr<SVerticalBox> Container = SNew(SVerticalBox);

	Container->AddSlot().AutoHeight()
	[
		SNew(STextBlock)
		.Font(DEFAULT_FONT("Regular", 14))
		.LineHeightPercentage(1.2)
		.Text(FText::Format(LOCTEXT("MyWidgetName", "{0}"), FText::FromName("Enter Email Address")))

	];
	
	Container->AddSlot().AutoHeight()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SAssignNew(PhoneNumberTextBox, SEditableTextBox)
			.HintText(LOCTEXT("email", "myemail@example.com"))
			.OnTextCommitted(this, &SNotifyOptionsWidget::OnTextCommitted)
		]
		+ SHorizontalBox::Slot()
		[
			SNew(STextBlock)
			.Justification(ETextJustify::Right)
			.Text(FText::Format(LOCTEXT("MyWidgetName", "{0}"), FText::FromName("Invalid Email")))
		]
	];

	Container->AddSlot().AutoHeight()
	[
		SNew(SBorder)
		.Padding(FMargin(5.0f))
		.BorderBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f))
		[
			SNew(SSeparator)
		]
	];

	Container->AddSlot().AutoHeight()
	[
		SNew(STextBlock)
		.Font(DEFAULT_FONT("Regular", 14))
		.LineHeightPercentage(1.2)
		.Text(FText::Format(LOCTEXT("MyWidgetName", "{0}"), FText::FromName("Select Notification Types")))
	];

	for (int Index = 0; Index < Notifications.Num(); Index++) {
		TSharedPtr<SCheckBox> Checkbox;

		Container->AddSlot().AutoHeight()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::Format(LOCTEXT("MyWidgetName", "{0}"), FText::FromString(Notifications[Index].Description)))
			]
			+SHorizontalBox::Slot()
			[
				SAssignNew(Checkbox, SCheckBox)
				.OnCheckStateChanged(this, &SNotifyOptionsWidget::OnCheckStateChanged)
			]
		];

		FilterCheckboxes.Add(Checkbox);
	}
	
	Container->AddSlot().AutoHeight()
	[
		SNew(SBorder)
		.Padding(FMargin(5.0f))
		.BorderBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f))
		[
			SNew(SSeparator)
		]
	];

	Container->AddSlot().AutoHeight()
	[
		SNew(SButton)
		.Text(LOCTEXT("SpawnNotification", "Debug Notification"))
		.ToolTipText(LOCTEXT("SpawnNotification_Tooltip", "Creates a super cool Notification!"))
		.OnClicked(this, &SNotifyOptionsWidget::SpawnNotification)
	];
	
	ChildSlot[ Container->AsShared() ];
}

void SNotifyOptionsWidget::OnTextCommitted(const FText& InText, ETextCommit::Type InCommitType)
{
	EmailCallback.ExecuteIfBound(InText, InCommitType);
}

void SNotifyOptionsWidget::OnCheckStateChanged(ECheckBoxState InState)
{
	for (int Index = 0; Index < FilterCheckboxes.Num(); Index++) {
		FiltersCallback.ExecuteIfBound(Index, FilterCheckboxes[Index]->IsChecked());
	}
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