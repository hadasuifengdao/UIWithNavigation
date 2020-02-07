// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UiWithNavigationGameMode.generated.h"

UCLASS(minimalapi)
class AUiWithNavigationGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUiWithNavigationGameMode();

public:
	UPROPERTY()
	class UUIManager* UIManager;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	static class AUiWithNavigationGameMode* GetGameMode();

	UFUNCTION(BlueprintCallable)
	static class UUIManager* GetUIManager();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUIManager> UIManagerClass;

private:
	static AUiWithNavigationGameMode* GameModePtr;
};



