// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/UIBase.h"
#include "UI/UIManager.h"
#include "Mode/UiWithNavigationGameMode.h"
#include "Application/SlateApplicationBase.h"
#include "UI/NavigationWidgetPanelInterface.h"
#include "Manager/MyCheatManager.h"
#include "Engine.h"
#include "Components/WidgetSwitcher.h"
#include "Blueprint/WidgetTree.h"


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
FReply UUIBase::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
#if (WITH_EDITOR)
	if (InKeyEvent.GetKey() == EKeys::Tilde)
	{
		return FReply::Unhandled();
	}
#endif

	if (K2_UIKeyDown(InKeyEvent.GetKey()))
	{
		return FReply::Handled();
	}

	UUIManager* UIManager = AUiWithNavigationGameMode::GetGameMode()->UIManager;
	if (UIManager)
	{
		EUIKeyControlType UIKeyControlType;
		EUINavigation UINavigation = UIManager->GetNavigationDirectionFromKey(InKeyEvent, UIKeyControlType);

		if (UINavigation != EUINavigation::Invalid)
		{
			const ENavigationGenesis Genesis = InKeyEvent.GetKey().IsGamepadKey() ? ENavigationGenesis::Controller : ENavigationGenesis::Keyboard;
			FNavigationEvent NavigationEvent(FSlateApplicationBase::Get().GetPlatformApplication()->GetModifierKeys(), 0, UINavigation, Genesis);

			DealNavigationEvent(UIKeyControlType, NavigationEvent);
		}
		//else
		//{
		//	APaladinPlayerControllerBase* PaladinPlayerController = UPaladinBlueprintLibrary::GetPalPlayerController();
		//	FPaladinDefaultKeySetting* KeySetting = PaladinPlayerController->FindKetSettingByPredicate([&](const FPaladinDefaultKeySetting* setting) {
		//		return (setting->UMGName == UIID);
		//		});

		//	if (KeySetting && InKeyEvent.GetKey() == KeySetting->KeyBoardKey)
		//	{
		//		//当使用键盘操作 再次按下触发开启UI的按键时 关闭该UI
		//		//由于该键位玩家可自定义 所以在这里特殊处理
		//		CloseUI();
		//	}
		//}
	}

	return FReply::Handled();
}
bool ShouldGoOnTravel(UWidget* CheckWidget)
{
	bool GoOn = true;
	if (!CheckWidget || !CheckWidget->IsVisible())
	{
		GoOn = false;
	}
	else if (INavigationWidgetPanelInterface* NavigationWidgetPanel = Cast<INavigationWidgetPanelInterface>(CheckWidget))
	{
		GoOn = NavigationWidgetPanel->IsNavigationActive();
	}

	return GoOn;
}
void ForVisibleNavigationWidgetAndChildren(UWidget* Widget, bool& IsHandled, TFunctionRef<void(UWidget*, bool&)> Predicate)
{
	if (!Widget)
	{
		return;
	}

	auto ChildPredicate = [&](UWidget* ChildWidget)
	{
		if (!IsHandled && ShouldGoOnTravel(ChildWidget))
		{
			Predicate(ChildWidget, IsHandled);
			if (IsHandled)
			{
				return;
			}
			ForVisibleNavigationWidgetAndChildren(ChildWidget, IsHandled, Predicate);
		}
	};

	// Search for any named slot with content that we need to dive into.
	if (INamedSlotInterface* NamedSlotHost = Cast<INamedSlotInterface>(Widget))
	{
		TArray<FName> SlotNames;
		NamedSlotHost->GetSlotNames(SlotNames);

		for (FName SlotName : SlotNames)
		{
			if (UWidget* SlotContent = NamedSlotHost->GetContentForSlot(SlotName))
			{
				ChildPredicate(SlotContent);
			}
		}
	}

	// Search standard children.
	if (UPanelWidget* PanelParent = Cast<UPanelWidget>(Widget))
	{
		if (INavigationWidgetPanelInterface* NavigationWidgetPanel = Cast<INavigationWidgetPanelInterface>(PanelParent))
		{
			ChildPredicate(NavigationWidgetPanel->GetSelectedChild());
		}
		else if (UWidgetSwitcher* Switcher = Cast<UWidgetSwitcher>(PanelParent))
		{
			ChildPredicate(Switcher->GetActiveWidget());
		}
		else
		{
			for (int32 ChildIndex = 0; ChildIndex < PanelParent->GetChildrenCount(); ChildIndex++)
			{
				if (UWidget* ChildWidget = PanelParent->GetChildAt(ChildIndex))
				{
					ChildPredicate(ChildWidget);
				}
			}
		}
	}

	if (UUserWidget* UserWidget = Cast<UUserWidget>(Widget))
	{
		Predicate(UserWidget, IsHandled);

		if (UserWidget->WidgetTree)
		{
			ChildPredicate(UserWidget->WidgetTree->RootWidget);
		}
	}
}

void UUIBase::CloseUI()
{
	CloseUIByManager();
}
void UUIBase::DealNavigationEvent(EUIKeyControlType UIKeyControlType, const FNavigationEvent& NavigationEvent)
{
	UUIManager* UIManager  = AUiWithNavigationGameMode::GetUIManager();;

	auto CheckNavigationWidgetPredicate = [&](UWidget* TravelWidget, bool& inIsHandled)
	{
		static FColor DebugTextColor;

		if (INavigationWidgetPanelInterface* NavigationWidgetPanel = Cast<INavigationWidgetPanelInterface>(TravelWidget))
		{
			if (NavigationWidgetPanel->GetControlType() == UIKeyControlType)
			{
				UPanelWidget* PanelWidget = CastChecked<UPanelWidget>(TravelWidget);

				if (NavigationWidgetPanel->PanelNavigation(NavigationEvent))
				{
					inIsHandled = true;

					//Navigation sound effect
					UIManager->PlayPanelNavigationSound(PanelWidget);
					if (UMyCheatManager::bUIDebug)
					{
						GEngine->AddOnScreenDebugMessage((uint64)-1, 3.0f,FColor::Green, FString::Printf(TEXT("UIDebug:Panel(%s) Consume NavigationEvent."), *PanelWidget->GetName()));
					}
				}
			}
		}
		else if (UNavigationUserWidgetBase* NavigationUserWidgetBase = Cast<UNavigationUserWidgetBase>(TravelWidget))
		{
			bool bActive = false;

			if (UNavigationUserWidget* NavigationUserWidget = Cast<UNavigationUserWidget>(TravelWidget))
			{
				bActive = NavigationUserWidgetBase->IsNavigationActive() && NavigationUserWidget->IsSelected();
			}
			else
			{
				bActive = NavigationUserWidgetBase->IsNavigationActive();
			}

			if (bActive)
			{
				if (NavigationUserWidgetBase->GetControlTypes().Contains(UIKeyControlType))
				{
					if (NavigationUserWidgetBase->UIEvent(UIKeyControlType, NavigationEvent.GetNavigationType()))
					{
						inIsHandled = true;

						if (UMyCheatManager::bUIDebug)
						{
							GEngine->AddOnScreenDebugMessage((uint64)-1, 3.0f, FColor::Green, FString::Printf(TEXT("UIDebug:Widget(%s) Consume NavigationEvent."), *NavigationUserWidgetBase->GetName()));
						}
					}
				}
			}
		}
	};

	bool bHandled = false;
	ForVisibleNavigationWidgetAndChildren(WidgetTree->RootWidget, bHandled, CheckNavigationWidgetPredicate);

	if (!bHandled)
	{
		if (!OnUIEvent(UIKeyControlType, NavigationEvent.GetNavigationType()))
		{
			if (UIKeyControlType == EUIKeyControlType::Confirm && NavigationEvent.GetNavigationType() == EUINavigation::Previous)
			{
				CloseUI();
			}
		}
	}
}

