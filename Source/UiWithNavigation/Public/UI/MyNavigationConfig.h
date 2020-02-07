// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Application/NavigationConfig.h"
#include "MyNavigationConfig.generated.h"

UENUM(Blueprintable)
enum class EUIKeyControlType : uint8
{
	DirectionNavigation,//导航方式包括键盘wsad、上下左右小箭头以及手柄的十字键以及左摇杆
	ChangePage,//切换方式为键盘的某两个健以及对应手柄的LeftShouldder、RightShoulder
	ChangePage2,//同为键盘某两个键加手柄的LeftTrigger、RightTrigger
	ViewActor,//观察3D物品使用鼠标左键拖拽以及右摇杆
	Confirm,//确定功能
};

USTRUCT(Blueprintable)
struct UIWITHNAVIGATION_API FUINavigationKey
{
	GENERATED_BODY()

	FUINavigationKey(EUIKeyControlType inControlType, EUINavigation inUINavigation);
	FUINavigationKey() {};
	bool operator==(const FUINavigationKey& other)
	{
		return this->ControlType == other.ControlType && this->UINavigation == other.UINavigation;
	}

	UPROPERTY(EditAnywhere)
	EUIKeyControlType ControlType;
	
	UPROPERTY(EditAnywhere)
	EUINavigation UINavigation;	
};
inline uint32 GetTypeHash(const FUINavigationKey Key)
{
	return uint32(Key.ControlType) * 10 + uint32(Key.UINavigation);
}
inline bool operator==(const FUINavigationKey& A, const FUINavigationKey& B)
{
	return A.ControlType == B.ControlType && A.UINavigation == B.UINavigation;
}

class FNavigationConfigBase : public FNavigationConfig
{
public:
	bool IsProcessRepeatKeyEvent();
protected:
	bool bProcessRepeatKeyEvent;
};

class FNavigationDirectionConfig : public FNavigationConfigBase
{
public:
	FNavigationDirectionConfig();
};

class FNavigationPageConfig : public FNavigationConfigBase
{
public:
	FNavigationPageConfig();
};

class FNavigationPage2Config : public FNavigationConfigBase
{
public:
	FNavigationPage2Config();
};

class FNavigationViewActorConfig : public FNavigationConfigBase
{
public:
	FNavigationViewActorConfig();
};

class FNavigationConformConfig : public FNavigationConfigBase
{
public:
	FNavigationConformConfig();
};

