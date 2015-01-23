#include "BluPrivatePCH.h"

class FBlu : public IBlu
{

	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		// Setup the default settings for BluManager
		BluManager::settings.windowless_rendering_enabled = true;

		BluManager::settings.remote_debugging_port = 7889;

		CefString(&BluManager::settings.browser_subprocess_path).FromASCII("blu_ue4_process.exe");

		CefExecuteProcess(BluManager::main_args, NULL, NULL);
		CefInitialize(BluManager::main_args, BluManager::settings, NULL, NULL);

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