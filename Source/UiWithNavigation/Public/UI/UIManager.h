// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/LatentActionManager.h"
#include "Engine/DataTable.h"
#include "UIManager.generated.h"

UENUM(BlueprintType)
enum class EUIProperty : uint8
{
	MainUI,
	FunctionUI,
	MessageBox,
	LoadingUI,
};

UENUM(Blueprintable)
enum class EUIOpenWay : uint8
{
	/*直接将新的UI覆盖在当前UI之上*/
	CoverOnCurrent,

	/*关闭当前UI 然后打开新的UI*/
	JustCloseCurrent,

	/*关闭当前UI后将其压栈 然后打开新的UI*/
	PushCurrentToStack,
};

struct FOpenUIStruct
{
	FName UIID;
	EUIOpenWay OpenWay;

	FOpenUIStruct() {}
	FOpenUIStruct(FName InUIID, EUIOpenWay InOpenWay)
	{
		UIID = InUIID;
		OpenWay = InOpenWay;
	}
	void Reset()
	{
		UIID = "";
	}
	bool IsValidUI()
	{
		return !UIID.IsNone();
	}
	bool operator== (const FOpenUIStruct& other)
	{
		return UIID == other.UIID;
	}
};
USTRUCT(Blueprintable)
struct UIWITHNAVIGATION_API FUIData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<class UUIBase> UIClass;

	UPROPERTY(EditDefaultsOnly)
	bool PauseGame = false;
};


UCLASS(Blueprintable, BlueprintType)
class  UIWITHNAVIGATION_API UUIManager : public UObject
{
	GENERATED_BODY()
	friend class UUIBase;
	
public:	

	UUIManager();
	//枚举->FName
	template<typename TEnum>
	static FORCEINLINE FName GetEnumValueAsName(const FString& Name, TEnum Value)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
		if (!enumPtr)
		{
			return TEXT("Invalid");
		}
		return enumPtr->GetNameByValue((int64)Value);
	}

	void OpenUI(FName UIID, EUIOpenWay OpenWay);

	UFUNCTION(BlueprintCallable)
	void Clear();

	UFUNCTION(BlueprintCallable)
	UUIBase* GetCurrentUI();

	UFUNCTION(BlueprintCallable)
	void PrintDebug();
private:
	UUIBase* Internal_OpenUI(FName UIID);

	void Internal_EnterUIMode(FName UIID);

	void Internal_ExitUIMode(FName UIID);

	void UIClosedCallBack(FName UIID);

protected:

	UPROPERTY()
	class UDataTable* UIDataTable;

	UPROPERTY()
	TMap<FName, class UUIBase*> UICache;

	FOpenUIStruct WaitingOpenUI;
	TArray<FName> UIStack;

	bool bdrawDebug = true;
};
