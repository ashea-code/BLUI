#include "BluLoaderPrivatePCH.h"

class FBluLoader : public IBluLoader
{

	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		FString LibPath = FPaths::ConvertRelativePathToFull(FPaths::GameDir() + "Plugins/BLUI/ThirdParty/cef/");

		// If we're on Windows in Editor we need to add a new path to load DLLs from
		#if PLATFORM_WINDOWS && WITH_EDITOR
			LibPath += "Win/shipping/";
			SetDllDirectory(*LibPath);
		#endif

		UE_LOG(LogBluLoader, Log, TEXT("STATUS: BLUI Ready to Load!"));
	}

	virtual void ShutdownModule() override
	{
		UE_LOG(LogBluLoader, Log, TEXT("STATUS: BLUI Has Shutdown"));
	}

};

IMPLEMENT_MODULE( FBluLoader, BluLoader )
DEFINE_LOG_CATEGORY(LogBluLoader);