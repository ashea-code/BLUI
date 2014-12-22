#include "BluPrivatePCH.h"

UBluViewPort::UBluViewPort(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	
}

void UBluViewPort::RegisterHudUI(UBluUIComponent* BluView)
{
	UE_LOG(LogBlu, Warning, TEXT("HUD UI has been registered"));
	HudViews.AddUnique(BluView);
}

void UBluViewPort::UnregisterHudUI(UBluUIComponent* BluView)
{
	UE_LOG(LogBlu, Log, TEXT("HUD UI unregistered"));
	HudViews.Remove(BluView);
}

void UBluViewPort::RegisterSceneUI(UBluUIComponent* BluView)
{
	UE_LOG(LogBlu, Log, TEXT("Scene UI registered"));
	SceneViews.AddUnique(BluView);
}

void UBluViewPort::UnregisterSceneUI(UBluUIComponent* BluView)
{
	UE_LOG(LogBlu, Log, TEXT("Scene UI unregistered"));
	SceneViews.Remove(BluView);
}
