#pragma once

#include "BluBlueprintFunctionLibrary.generated.h"

UCLASS(ClassGroup = Blu, Blueprintable)
class BLU_API UBluBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", DisplayName = "Create BluEye", CompactNodeTitle = "BluEye", Keywords = "new create blu eye blui"), Category = Blu)
		static UBluEye* NewBluEye(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", DisplayName = "Create BluJSON Obj", CompactNodeTitle = "JSON", Keywords = "new create blu eye blui json"), Category = Blu)
		static UBluJsonObj* NewBluJSONObj(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Run BLUI Tick", Keywords = "blui blu eye blui tick"), Category = Blu)
		static void RunBluEventLoop();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Parse JSON String", Keywords = "blui blu eye json parse"), Category = Blu)
		static UBluJsonObj* ParseJSON(const FString& JSONString);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "JSON To String", Keywords = "blui blu eye json parse string"), Category = Blu)
		static FString JSONToString(UBluJsonObj *ObjectToParse);

};
