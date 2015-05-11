#include "BluPrivatePCH.h"

BluManager::BluManager()
{

}

void BluManager::OnBeforeCommandLineProcessing(const CefString& process_type,
	CefRefPtr< CefCommandLine > command_line)
{
	command_line->AppendSwitch("off-screen-rendering-enabled");
	command_line->AppendSwitch("in-process-gpu");
	command_line->AppendSwitchWithValue("use-gl", "osmesa");
}

void BluManager::doBluMessageLoop()
{
	CefDoMessageLoopWork();
}

CefSettings BluManager::settings;
CefMainArgs BluManager::main_args;