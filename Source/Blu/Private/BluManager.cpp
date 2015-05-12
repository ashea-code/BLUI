#include "BluPrivatePCH.h"

BluManager::BluManager()
{

}

void BluManager::OnBeforeCommandLineProcessing(const CefString& process_type,
	CefRefPtr< CefCommandLine > command_line)
{

	/////////////////
	/**
	* Used to pick command line switches
	* If set to "true": CEF will use less CPU, but rendering performance will be lower. CSS3 and WebGL may not be usable
	* If set to "false": CEF will use more CPU, but rendering will be better, CSS3 and WebGL will also be usable
	*/
	BluManager::CPURenderSettings = true;
	/////////////////

	command_line->AppendSwitch("off-screen-rendering-enabled");
	command_line->AppendSwitchWithValue("off-screen-frame-rate", "60");
	command_line->AppendSwitch("enable-font-antialiasing");
	command_line->AppendSwitch("enable-media-stream");

	// Should we use the render settings that use less CPU?
	if (CPURenderSettings)
	{
		command_line->AppendSwitch("disable-gpu");
		command_line->AppendSwitch("disable-gpu-compositing");
		command_line->AppendSwitch("enable-begin-frame-scheduling");
	}
	else
	{
		// Enables things like CSS3 and WebGL
		command_line->AppendSwitch("in-process-gpu");
		command_line->AppendSwitchWithValue("use-gl", "osmesa");
		command_line->AppendSwitch("enable-gpu-rasterization");
	}


	// Append more command line options here if you want
	// Visit Peter Beverloo's site: http://peter.sh/experiments/chromium-command-line-switches/ for more info on the switches

}

void BluManager::doBluMessageLoop()
{
	CefDoMessageLoopWork();
}

CefSettings BluManager::settings;
CefMainArgs BluManager::main_args;
bool BluManager::CPURenderSettings = true;