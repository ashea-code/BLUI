#include "BluPrivatePCH.h"

class FBlu : public IBlu
{

	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		CefString GameDirCef = *FPaths::ConvertRelativePathToFull(FPaths::GameDir() + "BluCache");
		FString ExecutablePath = FPaths::ConvertRelativePathToFull(FPaths::GameDir() + "Plugins/BLUI/ThirdParty/cef/");

		// Setup the default settings for BluManager
		BluManager::settings.windowless_rendering_enabled = true;
		BluManager::settings.no_sandbox = true;
		BluManager::settings.remote_debugging_port = 7777;

	#if PLATFORM_LINUX
		ExecutablePath = "./blu_ue4_process";
	#endif
	#if PLATFORM_MAC
		ExecutablePath += "Mac/shipping/blu_ue4_process.app/Contents/MacOS/blu_ue4_process";
	#endif
	#if PLATFORM_WINDOWS
		ExecutablePath += "Win/shipping/blu_ue4_process.exe";
	#endif

		CefString realExePath = *ExecutablePath;

		// Set the sub-process path
		CefString(&BluManager::settings.browser_subprocess_path).FromString(realExePath);

		// Set the cache path
		CefString(&BluManager::settings.cache_path).FromString(GameDirCef);

		// Make a new manager instance
		CefRefPtr<BluManager> BluApp = new BluManager();

		//CefExecuteProcess(BluManager::main_args, BluApp, NULL);
		CefInitialize(BluManager::main_args, BluManager::settings, BluApp, NULL);

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