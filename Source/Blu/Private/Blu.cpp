#include "BluPrivatePCH.h"

class FBlu : public IBlu
{
	CefSettings settings;
	CefMainArgs main_args;

	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{

		FString GameDir = FPaths::ConvertRelativePathToFull(FPaths::GameDir() + "BluCache");
		CefString GameDirCef = *GameDir;

		// Setup the default settings for BluManager
		settings.windowless_rendering_enabled = false;
		settings.no_sandbox = true;

		settings.remote_debugging_port = 7777;
		
#if PLATFORM_LINUX
		CefString(&settings.browser_subprocess_path).FromASCII("./blu_ue4_process");
#endif
#if PLATFORM_WINDOWS
		CefString(&settings.browser_subprocess_path).FromASCII("./blu_ue4_process.exe");
#endif
		CefString(&settings.cache_path).FromString(GameDirCef);

		CefExecuteProcess(main_args, NULL, NULL);
		CefInitialize(main_args, settings, NULL, NULL);

		UBluEye::StaticClass();

		UE_LOG(LogBlu, Log, TEXT(" STATUS: Loaded"));
	}

	virtual void ShutdownModule() override
	{
		UE_LOG(LogBlu, Log, TEXT(" STATUS: Shutdown"));
		CefShutdown();
	}

};




IMPLEMENT_MODULE( FBlu, Blu )
DEFINE_LOG_CATEGORY(LogBlu);