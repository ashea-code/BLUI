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

void UBluJsonObj::doParseJson(TSharedRef<TJsonReader<TCHAR>> JsonReader)
{

	if (!FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		UE_LOG(LogBlu, Warning, TEXT("JSON OBJECT FAILED TO PARSE PROPERLY!"));
	}

}


