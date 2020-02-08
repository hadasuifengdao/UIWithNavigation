// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "MyCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class UIWITHNAVIGATION_API UMyCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
	//UICheat
	UFUNCTION(exec)
	void OpenUI(FName sUIID, int32 OpenWay);

	UFUNCTION(exec)
	void DebugUI(bool bNewValue);

	UFUNCTION(exec)
	void ClearUI();

public:
	static  bool bUIDebug;
};
