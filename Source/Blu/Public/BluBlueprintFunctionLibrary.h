#pragma once

#include "../Public/BluEye.h"
#include "BluBlueprintFunctionLibrary.generated.h"

UCLASS(ClassGroup = Blu, Blueprintable)
class BLU_API UBluBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", FriendlyName = "Create BluEye", CompactNodeTitle = "BluEye", Keywords = "new create blu eye blui"), Category = Blu)
		static UBluEye* NewBluEye(UObject* WorldContextObject);

};
