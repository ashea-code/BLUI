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
		return nullptr;
	}

	// Make our new temp obj
	UBluJsonObj* tempObj = NewObject<UBluJsonObj>(GetTransientPackage(), UBluJsonObj::StaticClass());
	tempObj->setJsonObj(newJson);

	// return it
	return tempObj;

}

TArray<float> UBluJsonObj::getNumArray(const FString &index)
{

	TArray<float> temp;

	for (TSharedPtr<FJsonValue> val : JsonParsed->GetArrayField(index))
	{

		temp.Add(val->AsNumber());

	}

	return temp;

}

TArray<bool> UBluJsonObj::getBooleanArray(const FString &index)
{

	TArray<bool> temp;

	for (TSharedPtr<FJsonValue> val : JsonParsed->GetArrayField(index))
	{

		temp.Add(val->AsBool());

	}

	return temp;

}

TArray<FString> UBluJsonObj::getStringArray(const FString &index)
{

	TArray<FString> temp;

	for (TSharedPtr<FJsonValue> val : JsonParsed->GetArrayField(index))
	{

		temp.Add(val->AsString());

	}

	return temp;

}


void UBluJsonObj::setStringValue(const FString &value, const FString &index)
{

	JsonParsed->SetStringField(index, value);

}

void UBluJsonObj::setNumValue(const float value, const FString &index)
{

	JsonParsed->SetNumberField(index, value);

}

void UBluJsonObj::setBooleanValue(const bool value, const FString &index)
{

	JsonParsed->SetBoolField(index, value);

}

void UBluJsonObj::setNestedObject(UBluJsonObj *value, const FString &index)
{

	JsonParsed->SetObjectField(index, value->getJsonObj());

}

void UBluJsonObj::setJsonObj(TSharedPtr<FJsonObject> NewJson)
{

	// Set our new stored JSON object
	JsonParsed = NewJson;

}

TSharedPtr<FJsonObject> UBluJsonObj::getJsonObj()
{

	return JsonParsed;

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


