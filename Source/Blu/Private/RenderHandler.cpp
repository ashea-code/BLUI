#include "BluPrivatePCH.h"

RenderHandler::RenderHandler(int32 width, int32 height, UBluEye* ui)
{
	this->width = width;
	this->height = height;
	this->parentUI = ui;
}

bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	rect = CefRect(0, 0, width, height);
	return true;
}

void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
	// Trigger our parent UIs Texture to update
	parentUI->TextureUpdate(buffer);
}

bool BrowserClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	
	FString name = FString(UTF8_TO_TCHAR(message->GetArgumentList()->GetString(0).c_str()));
	FString data;
	FString type = FString(UTF8_TO_TCHAR(message->GetArgumentList()->GetString(2).c_str()));
	FString data_type = FString(UTF8_TO_TCHAR(message->GetArgumentList()->GetString(3).c_str()));

	if (type == "js_event")
	{

		// Check the datatype

		if (data_type == "bool")
			data = message->GetArgumentList()->GetBool(1) ? TEXT("true") : TEXT("false");
		else if (data_type == "int")
			data = FString::FromInt(message->GetArgumentList()->GetInt(1));
		else if (data_type == "string")
			data = FString(UTF8_TO_TCHAR(message->GetArgumentList()->GetString(1).c_str()));
		else if (data_type == "double")
			data = FString::SanitizeFloat(message->GetArgumentList()->GetDouble(1));

		event_emitter->Broadcast(name, data);
	}

	return true;

}

void BrowserClient::SetEventEmitter(FScriptEvent* emitter)
{
	this->event_emitter = emitter;
}