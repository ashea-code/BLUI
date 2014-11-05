/*
* (c) Aaron M. Shea 2014
*/
#pragma once
#include "ModuleManager.h"

#include "AllowWindowsPlatformTypes.h"
#include "include/cef_app.h"
#include "HideWindowsPlatformTypes.h"

class ICEF : public IModuleInterface
{
public:

	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline ICEF& Get()
	{
		return FModuleManager::LoadModuleChecked< ICEF >("CEF");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("CEF");
	}

	static void DoCEFUpdateLoop()
	{
		if (IsAvailable())
		{
			CefDoMessageLoopWork();
		}
	}

	


};