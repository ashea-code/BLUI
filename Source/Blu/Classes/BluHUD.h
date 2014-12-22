#pragma once
#include "../Private/BluPrivatePCH.h"
#include "BluHUD.generated.h"

UCLASS(ClassGroup = Blu, editinlinenew, meta = (BlueprintSpawnableComponent))
class BLU_API UBluHUD : public UBluUIComponent
{

	GENERATED_UCLASS_BODY()

	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void BeginDestroy() override;

	/** Screen position */
	UPROPERTY(EditAnywhere, Category = "HUD")
	FVector2D ScreenPosition;

	/** Screen size */
	UPROPERTY(EditAnywhere, Category = "HUD")
	FVector2D ScreenSize;

	/** Current screen position of HUD widget */
	UFUNCTION(BlueprintCallable, Category = "UI|Blu|HUD")
	FVector2D GetScreenPosition() const;

	/** Set screen position to process mouse events for HUD widgets properly */
	UFUNCTION(BlueprintCallable, Category = "UI|Blu|HUD")
	void SetScreenPosition(float ScreenX, float ScreenY);

	/** Current screen size of HUD widget */
	UFUNCTION(BlueprintCallable, Category = "UI|Blu|HUD")
	FVector2D GetScreenSize() const;

	/** Set screen size to process mouse events for HUD widgets properly */
	UFUNCTION(BlueprintCallable, Category = "UI|Blu|HUD")
	void SetScreenSize(float ScreenWidth, float ScreenHeight);

};