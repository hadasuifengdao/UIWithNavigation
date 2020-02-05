// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Mode/UiWithNavigationGameMode.h"
#include "UI/UiWithNavigationHUD.h"
#include "Player/UiWithNavigationCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/UIManager.h"

AUiWithNavigationGameMode* AUiWithNavigationGameMode::GameModePtr = nullptr;
AUiWithNavigationGameMode::AUiWithNavigationGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUiWithNavigationHUD::StaticClass();
}
void AUiWithNavigationGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	GameModePtr = this;
	//UIπ‹¿Ì
	if (UIManagerClass)
	{
		UIManager = NewObject<UUIManager>(this, UIManagerClass);
		//UIManager->Init();
	}

}

AUiWithNavigationGameMode* AUiWithNavigationGameMode::GetGameMode()
{
	return GameModePtr;
}

class UUIManager* AUiWithNavigationGameMode::GetUIManager() 
{ 
	return GetGameMode()->UIManager;
}