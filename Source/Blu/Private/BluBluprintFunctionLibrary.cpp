#include "BluPrivatePCH.h"
#include "BluManager.h"
#include "Json.h"

UBluBlueprintFunctionLibrary::UBluBlueprintFunctionLibrary(const class FObjectInitializer& PCIP)
: Super(PCIP)
{

}

UBluEye* UBluBlueprintFunctionLibrary::NewBluEye(UObject* WorldContextObject)
{

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	UBluEye* tempObject = Cast<UBluEye>(StaticConstructObject(UBluEye::StaticClass()));

	return tempObject;

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