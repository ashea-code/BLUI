#pragma once
#include "CEFPrivatePCH.h"

CEFInstance::CEFInstance()
{

	// checkout detailed settings options http://magpcss.org/ceforum/apidocs/projects/%28default%29/_cef_settings_t.html
	// nearly all the settings can be set via args too.
	// settings.multi_threaded_message_loop = true; // not supported, except windows
	// CefString(&settings.browser_subprocess_path).FromASCII("sub_proccess path, by default uses and starts this executeable as child");
	// CefString(&settings.cache_path).FromASCII("");
	// CefString(&settings.log_file).FromASCII("");
	// settings.log_severity = LOGSEVERITY_DEFAULT;
	// CefString(&settings.resources_dir_path).FromASCII("");
	// CefString(&settings.locales_dir_path).FromASCII("");

	info.SetAsWindowless(nullptr, true);

	settings.windowless_rendering_enabled = true;
	settings.log_severity = LOGSEVERITY_VERBOSE;
	CefString(&settings.log_file).FromASCII("./CEF.log");
	CefString(&settings.browser_subprocess_path).FromASCII("cef_ue4_process.exe");

	CefMainArgs main_args;
	CefExecuteProcess(main_args, NULL, NULL);
	CefInitialize(main_args, settings, NULL, NULL);

}

CEFInstance::~CEFInstance()
{
	UE_LOG(LogCEF, Warning, TEXT("CEF Instance Closing"));
}