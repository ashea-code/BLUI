#pragma once
#include "../Private/BluPrivatePCH.h"
#include "BluViewPort.generated.h"

UCLASS()
class BLU_API UBluViewPort : public UGameViewportClient
{
	GENERATED_UCLASS_BODY()

	/** Register UI component as the HUD */
	void RegisterHudUI(UBluUIComponent* BluView);

	/** Unregister HUD */
	void UnregisterHudUI(UBluUIComponent* BluView);

	/** Register UI component as a Scene UI */
	void RegisterSceneUI(UBluUIComponent* BluView);

	/** Unregister Scene UI component */
	void UnregisterSceneUI(UBluUIComponent* BluView);

	// FViewportClient interface.
	//virtual bool InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent EventType, float AmountDepressed = 1.f, bool bGamepad = false) override;
	// End of FViewportClient interface.

private:
	/** List of all HUD web views (UI of this kind can be operated in 2D mode). High priority to grab input. */
	TArray<UBluUIComponent*> HudViews;

	/** List of all scene web views. Low priority to grab input. */
	TArray<UBluUIComponent*> SceneViews;
};