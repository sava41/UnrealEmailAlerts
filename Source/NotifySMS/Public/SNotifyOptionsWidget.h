// Fill out your copyright notice in the Description page of Project Settings.

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

	SLATE_END_ARGS()

	/**
	* Construct this widget. Called by the SNew() Slate macro.
	*
	* @param  InArgs	Declaration used by the SNew() macro to construct this widget
	*/
	void Construct(const FArguments& InArgs);

	FReply SpawnNotification();

private:

	void VerifySMSNumber(const FText& InText, ETextCommit::Type InCommitType);
	void FormatSMSNumber(const FText& InText);

	TSharedPtr<SEditableTextBox> PhoneNumberTextBox;
	FText PhoneNumber;

	TWeakPtr<SNotificationItem> NotificationPtr;


};
