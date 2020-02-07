// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/VerticalBoxWithNavigation.h"

UVerticalBoxWithNavigation::UVerticalBoxWithNavigation()
{
	//bUniformGridPanel = true;
}

const EUIKeyControlType& UVerticalBoxWithNavigation::GetControlType()
{
	return ControlType;
}

TSharedRef<SWidget> UVerticalBoxWithNavigation::RebuildWidget()
{
	//ResetSelectedChildIndexBySlotNum();

	return Super::RebuildWidget();
}
