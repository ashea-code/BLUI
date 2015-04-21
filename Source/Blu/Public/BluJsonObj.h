#pragma once

#include "BluJsonObj.generated.h"

UCLASS(ClassGroup = Blu, Blueprintable)
class BLU_API UBluJsonObj : public UObject
{

	GENERATED_UCLASS_BODY()

public:

	//// Get Values ////

	/* Gets a String value for the key given */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		FString getStringValue(const FString &index);

	/* Gets a Numerical value for the key given */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		float getNumValue(const FString &index);

	/* Gets a Boolean value for the key given */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		bool getBooleanValue(const FString &index);

	/* Gets a Nested JSON Object value for the key given */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		UBluJsonObj* getNestedObject(const FString &index);

	//// Get Array Values ////

	/* Gets an Array of floats or numbers for the key given */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		TArray<float> getNumArray(const FString &index);

	/* Gets an Array of booleans for the key given */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		TArray<bool> getBooleanArray(const FString &index);

	/* Gets an Array of strings for the key given */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		TArray<FString> getStringArray(const FString &index);

	//// Set Values ////

	/* Sets or Adds a String value to this JSON object */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void setStringValue(const FString &value, const FString &index);

	/* Sets or Adds a Numerical value to this JSON object */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void setNumValue(const float value, const FString &index);

	/* Sets or Adds a Boolean value to this JSON object */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void setBooleanValue(const bool value, const FString &index);

	/* Sets or Adds a Nested JSON Object value to this JSON object */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void setNestedObject(UBluJsonObj *value, const FString &index);

	void init(const FString &dataString);
	void setJsonObj(TSharedPtr<FJsonObject> NewJson);
	
	TSharedPtr<FJsonObject> getJsonObj();

private:

	FString StrData;
	TSharedPtr<FJsonObject> JsonParsed;

	void doParseJson(TSharedRef<TJsonReader<TCHAR>> JsonReader);


};