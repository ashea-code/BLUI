#include "../Private/BluPrivatePCH.h"

RenderHandler::RenderHandler(int32 width, int32 height, UBluWidget* ui)
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
	
	FString name = message->GetArgumentList()->GetString(0).c_str();
	FString data = message->GetArgumentList()->GetString(1).c_str();
	FString type = message->GetArgumentList()->GetString(2).c_str();

	if (type == "js_event")
	{
		event_emitter->Broadcast(name, data);
	}

	return true;

}

void BrowserClient::SetEventEmitter(FScriptEvent* emitter)
{
	this->event_emitter = emitter;
}