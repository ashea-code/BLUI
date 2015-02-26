#include "BluPrivatePCH.h"
#include "Json.h"

UBluJsonObj::UBluJsonObj(const class FObjectInitializer& PCIP)
: Super(PCIP)
{

}

void UBluJsonObj::init(const FString &StringData)
{

	StrData = *StringData;

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(StringData);
	doParseJson(JsonReader);

}

FString UBluJsonObj::getStringValue(const FString &index)
{

	return JsonParsed->GetStringField(index);

}

bool UBluJsonObj::getBooleanValue(const FString &index)
{

	return JsonParsed->GetBoolField(index);

}

float UBluJsonObj::getNumValue(const FString &index)
{

	return JsonParsed->GetNumberField(index);

}

UBluJsonObj* UBluJsonObj::getNestedObject(const FString &index)
{

	TSharedPtr<FJsonObject> newJson = JsonParsed->GetObjectField(index);

	if (!newJson.IsValid())
	{
		return NULL;
	}

	// Make our new temp obj
	UBluJsonObj* tempObj = NewObject<UBluJsonObj>(GetTransientPackage(), UBluJsonObj::StaticClass());
	tempObj->setJsonObj(newJson);

	// return it
	return tempObj;

}

void UBluJsonObj::setJsonObj(TSharedPtr<FJsonObject> NewJson)
{

	// Set our new stored JSON object
	JsonParsed = NewJson;

}

void UBluJsonObj::doParseJson(TSharedRef<TJsonReader<TCHAR>> JsonReader)
{

	if (!FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		UE_LOG(LogBlu, Warning, TEXT("JSON STRING FAILED TO PARSE! WILL DEFAULT TO EMPTY OBJECT {}"));

		// Make an empty json object to prevent crashing
		doParseJson(TJsonReaderFactory<TCHAR>::Create("{}"));
	}

}


