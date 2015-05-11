#include "BluPrivatePCH.h"

BluManager::BluManager()
{

}

void BluManager::OnBeforeCommandLineProcessing(const CefString& process_type,
	CefRefPtr< CefCommandLine > command_line)
{
	command_line->AppendSwitch("enable-pinch-virtual-viewport");
	command_line->AppendSwitch("enable-delegated-renderer");
	command_line->AppendSwitchWithValue("num-raster-threads", "2");
	command_line->AppendSwitch("enable-gpu-rasterization");
	command_line->AppendSwitch("disable-gpu-watchdog");
	command_line->AppendSwitchWithValue("supports-dual-gpus", "false");
}

void BluManager::doBluMessageLoop()
{
	CefDoMessageLoopWork();
}

CefSettings BluManager::settings;
CefMainArgs BluManager::main_args;