#include "BluLoaderPrivatePCH.h"

class FBluLoader : public IBluLoader
{

	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		UE_LOG(LogBluLoader, Log, TEXT("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! STATUS: Loaded"));
	}

	virtual void ShutdownModule() override
	{
		UE_LOG(LogBluLoader, Log, TEXT("STATUS: Shutdown"));
	}

};




IMPLEMENT_MODULE( FBluLoader, BluLoader )
DEFINE_LOG_CATEGORY(LogBluLoader);