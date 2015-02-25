#pragma once

#include "BluJsonObj.generated.h"

UCLASS(ClassGroup = Blu, Blueprintable)
class BLU_API UBluJsonObj : public UObject
{

	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Blu")
		FString getStringValue(const FString &index);

	UFUNCTION(BlueprintCallable, Category = "Blu")
		float getNumValue(const FString &index);

	UFUNCTION(BlueprintCallable, Category = "Blu")
		bool getBooleanValue(const FString &index);

	//UFUNCTION(BlueprintCallable, Category = "Blu")
		//UBluJsonObj* getNestedObject(const FString &index);

	void init(const FString &dataString);

private:

	FString StrData;
	TSharedPtr<FJsonObject> JsonParsed;

	void doParseJson(TSharedRef<TJsonReader<TCHAR>> JsonReader);


};