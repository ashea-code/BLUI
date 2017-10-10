#pragma once

#include "Containers/Map.h"
#include "JSMessageDataVariant.h"
#include "JSMessageData.generated.h"

USTRUCT(Blueprintable)
struct BLU_API FJSMessageData {

	GENERATED_BODY()

	UPROPERTY()
	FString EventName;

	UPROPERTY()
	TArray<FJSMessageDataVariant> Parameters;
};