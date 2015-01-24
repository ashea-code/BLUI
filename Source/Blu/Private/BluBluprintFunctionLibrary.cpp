#include "BluPrivatePCH.h"

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