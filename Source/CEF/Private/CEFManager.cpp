#include "CEFPrivatePCH.h"

void CEFManager::doCEFMessageLoop()
{
	CefDoMessageLoopWork();
}

CefSettings CEFManager::settings;
CefWindowInfo CEFManager::info;
CefMainArgs CEFManager::main_args;