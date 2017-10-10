#pragma once

#include "JSMessageDataVariant.generated.h"

USTRUCT(Blueprintable)
struct BLU_API FJSMessageDataVariant
{
	GENERATED_BODY()

	UPROPERTY()
	FString Type;

	UPROPERTY()
	FString Value;

};