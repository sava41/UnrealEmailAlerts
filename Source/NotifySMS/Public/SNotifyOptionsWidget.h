#pragma once

#include "CoreMinimal.h"
#include "NotificationFilters.h"

DECLARE_DELEGATE_RetVal_TwoParams(void, FOnFilterStateChanged, uint32 /* Index */, bool /* IsEnabled */);
DECLARE_DELEGATE_RetVal_OneParam(bool, FOnEmailChanged, const FText& /* InText */);

/**
 * A CompoundWidget is the base from which most non-primitive widgets should be built.
 * CompoundWidgets have a protected member named ChildSlot.
 */
class SNotifyOptionsWidget : public SCompoundWidget
{

public:

	SLATE_BEGIN_ARGS(SNotifyOptionsWidget)
	{}
		SLATE_EVENT(FOnFilterStateChanged, FiltersCallback)
		SLATE_EVENT(FOnEmailChanged, EmailCallback)

	SLATE_END_ARGS()

	/**
	* Construct this widget. Called by the SNew() Slate macro.
	*
	* @param  InArgs	Declaration used by the SNew() macro to construct this widget
	*/
	void Construct(const FArguments& InArgs);

	FReply SpawnNotification();

private:

	FOnFilterStateChanged FiltersCallback;
	FOnEmailChanged EmailCallback;

	TSharedPtr<STextBlock> EmailWarning;

	void OnTextCommitted(const FText& InText, ETextCommit::Type InCommitType);
	void OnCheckStateChanged(ECheckBoxState InState);

	TSharedPtr<SEditableTextBox> PhoneNumberTextBox;

	TWeakPtr<SNotificationItem> NotificationPtr;

	TArray<TSharedPtr<SCheckBox>> FilterCheckboxes;
};
