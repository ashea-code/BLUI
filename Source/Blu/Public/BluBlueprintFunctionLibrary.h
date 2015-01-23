#pragma once

#include "../Public/BluEye.h"
#include "BluBlueprintFunctionLibrary.generated.h"

UCLASS(ClassGroup = UI)
class BLU_API UBluBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", FriendlyName = "Create BluEye", CompactNodeTitle = "BluEye", Keywords = "new create blu eye blui"), Category = Game)
		static UBluEye* NewBluEye(UObject* WorldContextObject);

};
