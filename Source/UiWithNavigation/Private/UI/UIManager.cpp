// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UIManager.h"
#include "UI/UIBase.h"
#include "Mode/UiWithNavigationGameMode.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine.h"
#include "Slate/SObjectWidget.h"


// Sets default values for this component's properties
UUIManager::UUIManager()
{
	ConstructorHelpers::FObjectFinder<UDataTable> Finder(TEXT("/Game/DataTable/UiDataTable"));
	UIDataTable = Finder.Object;
	check(UIDataTable);
}

void UUIManager::OpenUI(FName UIID, EUIOpenWay OpenWay)
{
	//不会处理栈中已经存在的UI
	if (UIStack.Contains(UIID) || WaitingOpenUI.UIID == UIID)
	{
		return;
	}

	FUIData* UIData = UIDataTable->FindRow<FUIData>(UIID, TEXT("UIDataTable->FindRow"));
	if (!UIData)
	{
		return;
	}

	if (UIStack.Num() == 0 || OpenWay ==EUIOpenWay::CoverOnCurrent)
	{
		UUIBase* NewUI = Internal_OpenUI(UIID);
		if (NewUI)
		{
			UIStack.Push(UIID);

			if (UIStack.Num() == 1)
			{
				Internal_EnterUIMode(UIID);
			}
		}
	}
	else
	{
		UUIBase* CurrentUI = GetCurrentUI();
		if (CurrentUI && IsValid(CurrentUI))
		{
			WaitingOpenUI.UIID = UIID;
			WaitingOpenUI.OpenWay = OpenWay;

			CurrentUI->CloseUIByManager();
		}
		else
		{
			//error 尝试关闭当前UI 但该UI非法
		}
	}

	FLatentActionManager& LatentActionManager = GetWorld()->GetLatentActionManager();
	LatentActionManager.ProcessLatentActions(this, GetWorld()->GetDeltaSeconds());

}

void UUIManager::Clear()
{
	if (UIStack.Num() == 0)
	{
		return;
	}

	FName BottomUIID = UIStack[0];

	WaitingOpenUI.Reset();

	for (auto it : UIStack)
	{
		UUIBase** pUI = UICache.Find(it);
		if (pUI && IsValid(*pUI) && (*pUI)->IsInViewport() && (*pUI)->AnimState != EUIAnimState::Closed)
		{
			(*pUI)->AnimState = EUIAnimState::Closed;
			(*pUI)->RemoveFromParent();
		}
	}
	UIStack.Empty();

	Internal_ExitUIMode(BottomUIID);
}

UUIBase* UUIManager::Internal_OpenUI(FName UIID)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	check(PC);

	UUIBase* UI = nullptr;
	UUIBase** pUI = UICache.Find(UIID);
	if (pUI && IsValid(*pUI))
	{
		UI = *pUI;
	}
	else
	{
		FUIData* UIData = UIDataTable->FindRow<FUIData>(UIID, TEXT("UIDataTable->FindRow"));
		if (UIData)
		{
			UClass* UIUClass = UIData->UIClass.Get();
			if (!UIUClass)
			{
				UIUClass = Cast<UClass>(UIData->UIClass.ToSoftObjectPath().TryLoad());
			}
			if (UIUClass)
			{
				UI = Cast<UUIBase>(UWidgetBlueprintLibrary::Create(GetWorld(), UIUClass, PC));
				if (UI)
				{
					UI->UIID = UIID;
					UICache.Add(UIID, UI);
				}
			}
		}
	}

	if (UI)
	{
		if (!UI->IsInViewport())
		{
			UI->AddToViewport(int32(EUIProperty::FunctionUI));
		}

		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, UI, EMouseLockMode::LockAlways);
	}

	return UI;
}
void UUIManager::Internal_EnterUIMode(FName UIID)
{
	FUIData* UIData = UIDataTable->FindRow<FUIData>(UIID, TEXT("UIDataTable->FindRow"));
	if (UIData && UIData->PauseGame)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	int32 vx, vy;
	PC->GetViewportSize(vx, vy);
	if (vx > 0 && vy > 0)
	{
		PC->SetMouseLocation(vx / 2, vy / 2);
	}
}

UUIBase* UUIManager::GetCurrentUI()
{
	if (UIStack.Num() > 0)
	{
		UUIBase** pUI = UICache.Find(UIStack.Top());
		if (pUI)
		{
			return *pUI;
		}
	}
	return nullptr;
}
void UUIManager::UIClosedCallBack(FName UIID)
{
	check(UIStack.Num() > 0);

	if (UIStack.Top() != UIID)
	{
		UE_LOG(LogTemp, Error, TEXT("UUIManager::UIClosedCallBack Error. StackTop:%s Clo	sed:%s"), *UIStack.Top().ToString(), *UIID.ToString());
		return;
	}

	if (WaitingOpenUI.IsValidUI())
	{
		UUIBase* NewUI = Internal_OpenUI(WaitingOpenUI.UIID);
		if (NewUI)
		{
			if (WaitingOpenUI.OpenWay == EUIOpenWay::JustCloseCurrent)
			{
				UIStack.Pop();
			}
			UIStack.Push(WaitingOpenUI.UIID);
		}

		WaitingOpenUI.Reset();
	}
	else
	{
		UIStack.Pop();
		if (UIStack.Num() == 0)
		{
			Internal_ExitUIMode(UIID);
		}
		else
		{
			Internal_OpenUI(UIStack.Top());
		}
	}
	FLatentActionManager& LatentActionManager = GetWorld()->GetLatentActionManager();
	LatentActionManager.ProcessLatentActions(this, GetWorld()->GetDeltaSeconds());
}
void UUIManager::Internal_ExitUIMode(FName UIID)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);

	FUIData* UIData = UIDataTable->FindRow<FUIData>(UIID, TEXT("UIDataTable->FindRow"));
	if (UIData && UIData->PauseGame)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}
void UUIManager::PrintDebug()
{
	FColor DebugTextColor=FColor::Green;
	if (bdrawDebug)
	{
		GEngine->AddOnScreenDebugMessage((uint64)-1, 0, DebugTextColor, TEXT("----------------------"));

		FString OpenedUI = TEXT("UICache:");
		for (auto it : UICache)
		{
			FString UIName = it.Key.ToString();
			UUIBase* UI = it.Value;
			if (IsValid(UI))
			{
				FString UIAnimState = GetEnumValueAsName(TEXT("EPaladinUIAnimState"), UI->AnimState).ToString();

				UIName = FString::Printf(TEXT("%s(%s)  "), *UIName, *UIAnimState);
				OpenedUI += UIName;
			}
			else
			{
				UIName += TEXT("(NotValid)  ");
				OpenedUI += UIName;
			}
		}
		GEngine->AddOnScreenDebugMessage((uint64)-1, 0, DebugTextColor, OpenedUI);

		FString StackUI = TEXT("UIStack:");
		for (int32 i = 0; i < UIStack.Num(); ++i)
		{
			FString UIName = UIStack[i].ToString();
			StackUI += UIName;
			StackUI += TEXT("  ");
		}
		GEngine->AddOnScreenDebugMessage((uint64)-1, 0, DebugTextColor, StackUI);

		FString WaitingOpenUIIDInfo = TEXT("WaitingOpenUIID:") + WaitingOpenUI.UIID.ToString();
		GEngine->AddOnScreenDebugMessage((uint64)-1, 0, DebugTextColor, WaitingOpenUIIDInfo);

		FString FoucsUWidgetName = TEXT("");
		SWidget* FocusSWidget = FSlateApplicationBase::Get().GetKeyboardFocusedWidget().Get();
		if (FocusSWidget && FocusSWidget->GetType() == "SObjectWidget")
		{
			SObjectWidget* FocusWidget = static_cast<SObjectWidget*>(FocusSWidget);
			if (FocusWidget)
			{
				UUserWidget* FocusUWidget = FocusWidget->GetWidgetObject();
				if (FocusUWidget)
				{
					UUIBase* FocusPaladinUWidget = Cast<UUIBase>(FocusUWidget);
					if (FocusPaladinUWidget)
					{
						FoucsUWidgetName = FocusPaladinUWidget->GetUIID().ToString();
					}
				}
			}
		}
		FString FoucsWidgetInfo = TEXT("FoucsPaladinUserWidget:") + FoucsUWidgetName;
		GEngine->AddOnScreenDebugMessage((uint64)-1, 0, DebugTextColor, FoucsWidgetInfo);

		//APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		//GEngine->AddOnScreenDebugMessage((uint64)-1, 0, DebugTextColor, bLastControlDeviceIsGamepad ? TEXT("Last Control Device Is Gamepad.") : TEXT("Last Control Device Is NotGamepad."));
	}
	if (AGameModeBase* GMInstance = Cast<AGameModeBase>(GetOuter()))
	{
		GMInstance->GetWorldTimerManager().SetTimerForNextTick(this, &UUIManager::PrintDebug);
	}
}