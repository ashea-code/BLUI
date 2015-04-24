#include "BluPrivatePCH.h"

void BluManager::doBluMessageLoop()
{
	CefDoMessageLoopWork();
}

CefSettings BluManager::settings;
CefMainArgs BluManager::main_args;