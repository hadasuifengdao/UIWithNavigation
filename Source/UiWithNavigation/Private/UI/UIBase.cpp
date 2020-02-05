// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/UIBase.h"
#include "UI/UIManager.h"
#include "Mode/UiWithNavigationGameMode.h"


UUIBase::UUIBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUIBase::AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder)
{
	Super::AddToScreen(LocalPlayer, ZOrder);

	UUserWidget* AnimWidget = nullptr;
	UWidgetAnimation* Anim = OpenUIAnim(AnimWidget);
	if (Anim && AnimWidget)
	{
		AnimWidget->UnbindAllFromAnimationFinished(Anim);
		FWidgetAnimationDynamicEvent FinishDelegate;
		FinishDelegate.BindUFunction(this, FName("OnOpenUIAnimEndCall"));
		AnimWidget->BindToAnimationFinished(Anim, FinishDelegate);
		AnimWidget->PlayAnimation(Anim);

		AnimState = EUIAnimState::Opening;
	}
	else
	{
		OnOpenUIAnimEndCall();
	}
}
void UUIBase::OnOpenUIAnimEndCall()
{
	AnimState = EUIAnimState::Opened;
}

void UUIBase::CloseUIByManager()
{
	if (AnimState == EUIAnimState::Closing || AnimState == EUIAnimState::Closed)
	{
		return;
	}
	UUserWidget* AnimWidget = nullptr;
	UWidgetAnimation* Anim = CloseUIAnim(AnimWidget);
	if (Anim && AnimWidget)
	{
		AnimWidget->UnbindAllFromAnimationFinished(Anim);
		FWidgetAnimationDynamicEvent FinishDelegate;
		FinishDelegate.BindUFunction(this, FName("OnCloseUIAnimEndCall"));
		AnimWidget->BindToAnimationFinished(Anim, FinishDelegate);
		AnimWidget->PlayAnimation(Anim);

		AnimState = EUIAnimState::Closing;
	}
	else
	{
		OnCloseUIAnimEndCall();
	}
}

void UUIBase::OnCloseUIAnimEndCall()
{
	AnimState = EUIAnimState::Closed;

	UUIManager* UIManager = AUiWithNavigationGameMode::GetGameMode()->UIManager;

	Super::RemoveFromParent();

	if (UIManager)
	{
		UIManager->UIClosedCallBack(UIID);
	}

	if (!this->IsPendingKill())
	{
		K2_OnClosed();
	}
}



