#include "BluPrivatePCH.h"
#include "BluManager.h"
#include "Json.h"

UBluBlueprintFunctionLibrary::UBluBlueprintFunctionLibrary(const class FObjectInitializer& PCIP)
: Super(PCIP)
{

}

UBluEye* UBluBlueprintFunctionLibrary::NewBluEye(UObject* WorldContextObject)
{

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	UBluEye* tempObject = Cast<UBluEye>(StaticConstructObject_Internal(UBluEye::StaticClass()));

	return tempObject;

}

UBluJsonObj* UBluBlueprintFunctionLibrary::NewBluJSONObj(UObject* WorldContextObject)
{

	UBluJsonObj* tempObj = NewObject<UBluJsonObj>(GetTransientPackage(), UBluJsonObj::StaticClass());
	tempObj->init("{}");
	
	return tempObj;

}

void UBluBlueprintFunctionLibrary::RunBluEventLoop()
{
	BluManager::doBluMessageLoop();
}

UBluJsonObj* UBluBlueprintFunctionLibrary::ParseJSON(const FString& JSONString)
{

	UBluJsonObj* tempObj = NewObject<UBluJsonObj>(GetTransientPackage(), UBluJsonObj::StaticClass());
	tempObj->init(JSONString);

	return tempObj;

}

FString UBluBlueprintFunctionLibrary::JSONToString(UBluJsonObj *ObjectToParse)
{

	// Create the JSON reader
	FString returnString;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&returnString);

	// Convert the JSON object to an FString
	FJsonSerializer::Serialize(ObjectToParse->getJsonObj().ToSharedRef(), writer);

	return returnString;

}


FString UBluBlueprintFunctionLibrary::GameRootDirectory()
{
	return FPaths::ConvertRelativePathToFull(FPaths::GameDir());
}


bool UBluBlueprintFunctionLibrary::HasSubstring(const FString& SearchIn, const FString& Substring, ESearchCase::Type SearchCase /*= ESearchCase::IgnoreCase*/, ESearchDir::Type SearchDir /*= ESearchDir::FromStart*/)
{
	return SearchIn.Contains(Substring, SearchCase, SearchDir);
}