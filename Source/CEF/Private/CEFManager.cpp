#include "CEFPrivatePCH.h"

void CEFManager::doCEFMessageLoop()
{
	CefDoMessageLoopWork();
}

CefSettings CEFManager::settings;
CefMainArgs CEFManager::main_args;