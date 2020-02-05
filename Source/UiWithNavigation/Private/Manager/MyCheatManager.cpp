// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/MyCheatManager.h"
#include "Mode/UiWithNavigationGameMode.h"
#include "UI/UIManager.h"

void UMyCheatManager::OpenUI(FName sUIID, int32 OpenWay)
{
    EUIOpenWay OpenWayEnum = EUIOpenWay(OpenWay);
    if (AUiWithNavigationGameMode::GetGameMode()->UIManager)
    {
        AUiWithNavigationGameMode::GetGameMode()->UIManager->OpenUI(sUIID, OpenWayEnum);
    }
}

void UMyCheatManager::ClearUI()
{
	if (AUiWithNavigationGameMode::GetGameMode()->UIManager)
	{
        AUiWithNavigationGameMode::GetGameMode()->UIManager->Clear();
	}
}