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

	// Append more command line options here if you want
	// Visit Peter Beverloo's site: http://peter.sh/experiments/chromium-command-line-switches/ for more info on the switches

}

void BluManager::doBluMessageLoop()
{
	CefDoMessageLoopWork();
}

CefSettings BluManager::settings;
CefMainArgs BluManager::main_args;