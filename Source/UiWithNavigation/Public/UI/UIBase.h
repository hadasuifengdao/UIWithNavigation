﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIManager.h"
#include "UIBase.generated.h"

UENUM(Blueprintable)
enum class EUIAnimState : uint8
{
	Opening,
	Opened,
	Closing,
	Closed,
};

UCLASS(Abstract)
class UIWITHNAVIGATION_API UUIBase : public UUserWidget
{
	GENERATED_BODY()
	friend class UUIManager;
	
public:

	UUIBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void CloseUI();

	FName GetUIID() { return UIID; }

protected:
	virtual void AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder) override;

	/*DealKesyEvent*/
	virtual bool NativeSupportsKeyboardFocus() const override { return true; }
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	void DealNavigationEvent(EUIKeyControlType UIKeyControlType, const FNavigationEvent& NavigationEvent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Input")
	bool OnUIEvent(EUIKeyControlType UIKeyControlType, EUINavigation UINavigation);

	UFUNCTION(BlueprintImplementableEvent)
	UWidgetAnimation* OpenUIAnim(UUserWidget*& AnimWidget);

	UFUNCTION(BlueprintImplementableEvent)
	UWidgetAnimation* CloseUIAnim(UUserWidget*& AnimWidget);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnClosed();

	UFUNCTION(BlueprintImplementableEvent)
	bool K2_UIKeyDown(FKey Key);

private:
	UFUNCTION()
	void OnOpenUIAnimEndCall();

	void CloseUIByManager();

	void OnCloseUIAnimEndCall();
private:
	
	FName UIID;
	EUIAnimState AnimState;
};
