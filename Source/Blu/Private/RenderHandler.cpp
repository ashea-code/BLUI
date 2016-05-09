#include "BluPrivatePCH.h"

RenderHandler::RenderHandler(int32 width, int32 height, UBluEye* ui)
{
	this->Width = width;
	this->Height = height;
	this->parentUI = ui;
}

bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	rect = CefRect(0, 0, Width, Height);
	return true;
}

void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
	FUpdateTextureRegion2D *updateRegions = static_cast<FUpdateTextureRegion2D*>(FMemory::Malloc(sizeof(FUpdateTextureRegion2D) * dirtyRects.size()));

	int current = 0;
	for (auto dirtyRect : dirtyRects)
	{
		updateRegions[current].DestX = updateRegions[current].SrcX = dirtyRect.x;
		updateRegions[current].DestY = updateRegions[current].SrcY = dirtyRect.y;
		updateRegions[current].Height = dirtyRect.height;
		updateRegions[current].Width = dirtyRect.width;

		current++;
	}

	// Trigger our parent UIs Texture to update
	parentUI->TextureUpdate(buffer, updateRegions, dirtyRects.size());
}

void BrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	if (!m_Browser.get())
	{
		// Keep a reference to the main browser.
		m_Browser = browser;
		m_BrowserId = browser->GetIdentifier();
	}
}

void BrowserClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	if (m_BrowserId == browser->GetIdentifier())
	{
		m_Browser = NULL;
	}
}

bool BrowserClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	
	FString data;
	FString name = FString(UTF8_TO_TCHAR(message->GetArgumentList()->GetString(0).ToString().c_str()));
	FString type = FString(UTF8_TO_TCHAR(message->GetArgumentList()->GetString(2).ToString().c_str()));
	FString data_type = FString(UTF8_TO_TCHAR(message->GetArgumentList()->GetString(3).ToString().c_str()));
	
	if (type == "js_event")
	{
		
		// Check the datatype
		
		if (data_type == "bool")
			data = message->GetArgumentList()->GetBool(1) ? TEXT("true") : TEXT("false");
		else if (data_type == "int")
			data = FString::FromInt(message->GetArgumentList()->GetInt(1));
		else if (data_type == "string")
			data = FString(UTF8_TO_TCHAR(message->GetArgumentList()->GetString(1).ToString().c_str()));
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