#pragma once

#include "CoreMinimal.h"

/**
 * A CompoundWidget is the base from which most non-primitive widgets should be built.
 * CompoundWidgets have a protected member named ChildSlot.
 */
class SNotifyOptionsWidget : public SCompoundWidget
{

public:

	SLATE_BEGIN_ARGS(SNotifyOptionsWidget)
		{}
		SLATE_EVENT(FOnTextCommitted, EmailCallback)
		SLATE_EVENT(FOnClicked, FiltersCallback)

	SLATE_END_ARGS()

	/**
	* Construct this widget. Called by the SNew() Slate macro.
	*
	* @param  InArgs	Declaration used by the SNew() macro to construct this widget
	*/
	void Construct(const FArguments& InArgs);

	FReply SpawnNotification();

protected:

	FOnTextCommitted EmailCallback;
	FOnClicked FiltersCallback;

private:

	void OnTextCommitted(const FText& InText, ETextCommit::Type InCommitType);

	TSharedPtr<SEditableTextBox> PhoneNumberTextBox;
	FText PhoneNumber;

	TWeakPtr<SNotificationItem> NotificationPtr;


};
