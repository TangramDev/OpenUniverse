// begin Add by HubbleTeam
#include "hubble.h"
#include "hubble_xobj.h"
#include "hubble_node.h"
#include "hubble_event.h"
#include "hubble_winform.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/html/html_element.h"
#include "third_party/blink/renderer/core/html/html_head_element.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/public/web/web_local_frame_client.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_application_callback.h"

namespace blink {

	HubbleXobj::HubbleXobj() {
		sender_ = nullptr;
		m_pRenderframeImpl = nullptr;
		id_ = WTF::CreateCanonicalUUIDString();
		session_.m_mapString[L"sessionid"] = WebString(id_).Utf16();
	}

	HubbleXobj::~HubbleXobj() {
		session_.m_mapString.clear();
		session_.m_mapLong.clear();
		session_.m_mapFloat.clear();
		mapHubbleEventCallback_.clear();
	}

	HubbleXobj* HubbleXobj::Create(const String& strNodeXml) {
		return MakeGarbageCollected<HubbleXobj>(strNodeXml);
	}

	HubbleXobj::HubbleXobj(const String& strName)
	{
		name_ = strName;
	}

	void HubbleXobj::Trace(blink::Visitor* visitor) {
		ScriptWrappable::Trace(visitor);
		visitor->Trace(mapVisibleElem);
		visitor->Trace(mapHubbleEventCallback_);
		visitor->Trace(sender_);
		visitor->Trace(hubble_);
	}

	String HubbleXobj::name() {
		return getStr(L"name@page");
	}

	String HubbleXobj::getid()
	{
		return id_;
	}

	HubbleWinform* HubbleXobj::form()
	{
		__int64 nHandle = getInt64(L"formhandle");
		auto it = hubble_->m_mapWinForm.find(nHandle);
		if (it != hubble_->m_mapWinForm.end())
			return it->value.Get();
		return nullptr;
	}

	HubbleNode* HubbleXobj::grid()
	{
		__int64 nHandle = getInt64(L"gridobjhandle");
		auto it = hubble_->m_mapHubbleNode.find(nHandle);
		if (it != hubble_->m_mapHubbleNode.end())
			return it->value.Get();
		return nullptr;
	}

	HubbleGalaxy* HubbleXobj::galaxy()
	{
		__int64 nHandle = getInt64(L"gridobjhandle");
		auto it = hubble_->m_mapHubbleNode.find(nHandle);
		if (it != hubble_->m_mapHubbleNode.end())
		{
			return it->value->parentGalaxy();
		}
		return nullptr;
	}

	void HubbleXobj::setSender(HubbleXobj* xobj)
	{
		sender_ = xobj;
	}

	HubbleXobj* HubbleXobj::sender()
	{
		return sender_.Get();
	}

	void HubbleXobj::setStr(const String& strKey, const String& value)
	{
		WebString str = strKey;
		WebString val = value;
		session_.m_mapString[str.Utf16()] = val.Utf16();
		auto it = session_.m_mapint64.find(WebString(strKey).Utf16());
		if(it!= session_.m_mapint64.end())
		{
			setStr(L"msgID", L"MODIFY_CTRL_VALUE");
			setStr(L"currentsubobjformodify", strKey);
			m_pRenderframeImpl->SendHubbleMessageEx(session_);
		}
	}

	String HubbleXobj::getStr(const String& strKey)
	{
		WebString str = strKey;
		auto it = session_.m_mapString.find(str.Utf16());
		if(it!= session_.m_mapString.end())
		{
			return String(it->second.c_str());
		}
		return L"";
	}

	void HubbleXobj::setLong(const String& strKey, long value)
	{
		WebString str = strKey;
		session_.m_mapLong[str.Utf16()] = value;
	}

	long HubbleXobj::getLong(const String& strKey)
	{
		WebString str = strKey;
		auto it = session_.m_mapLong.find(str.Utf16());
		if (it != session_.m_mapLong.end())
		{
			return it->second;
		}
		return 0;
	}

	void HubbleXobj::setInt64(const String& strKey, int64_t value)
	{
		WebString str = strKey;
		auto it = session_.m_mapint64.find(str.Utf16());
		if (it != session_.m_mapint64.end())
		{
			session_.m_mapint64.erase(it);
		}
		session_.m_mapint64[str.Utf16()] = value;
	}

	int64_t HubbleXobj::getInt64(const String& strKey)
	{
		WebString str = strKey;
		auto it = session_.m_mapint64.find(str.Utf16());
		if (it != session_.m_mapint64.end())
		{
			return it->second;
		}
		return 0;
	}

	void HubbleXobj::setFloat(const String& strKey, float value)
	{
		WebString str = strKey;
		session_.m_mapFloat [str.Utf16()] = value;
	}

	float HubbleXobj::getFloat(const String& strKey)
	{
		WebString str = strKey;
		auto it = session_.m_mapFloat.find(str.Utf16());
		if (it != session_.m_mapFloat.end())
			return it->second;
		return 0;
	}

	void HubbleXobj::setMsgID(const String& value)
	{
		WebString str = "msgID";
		WebString val = value;
		session_.m_mapString[str.Utf16()] = val.Utf16();
	}

	String HubbleXobj::msgID()
	{
		WebString str = "msgID";
		auto it = session_.m_mapString.find(str.Utf16());
		if (it != session_.m_mapString.end())
		{
			return String(it->second.c_str());
		}
		return L"";
	}

	void HubbleXobj::setVisibleElement(const String& strKey, Element* value)
	{
		auto it = mapVisibleElem.find(strKey);
		if (it == mapVisibleElem.end())
		{
			mapVisibleElem.insert(strKey, value);
		}
		else {
			mapVisibleElem.erase(it);
			mapVisibleElem.insert(strKey, value);
		}
	}

	Element* HubbleXobj::getVisibleElement(const String& strKey)
	{
		auto it = mapVisibleElem.find(strKey);
		if (it != mapVisibleElem.end())
		{			
			return it->value.Get();
		}
		return nullptr;
	}

	void HubbleXobj::addEventListener(const String& eventName, V8ApplicationCallback* callback)
	{
		if (callback)
		{
			auto it = session_.m_mapString.find(L"objID");
			if (it != session_.m_mapString.end())
			{
				if(id_==nullptr)
					id_ = WTF::CreateCanonicalUUIDString();
				hubble_->mapCloudSession_.insert(id_, this);
				//插入callbackID:
				String callbackid_ = WTF::CreateCanonicalUUIDString();
				std::wstring strID = WebString(callbackid_).Utf16();
				setStr(L"callbackid", callbackid_);

				mapHubbleEventCallback_.insert(eventName, callback);
				//绑定事件名称与callbackid建立对应关系：
				session_.m_mapString[strID] = WebString(eventName).Utf16();
				session_.m_mapString[L"currentsubobjforlistener"] = L"";

				//允许RenderFrameImpl根据回调id查找对应的session：
				m_pRenderframeImpl->m_mapHubbleSession[strID] = this;
				//通知客户端建立监听连接：
				m_pRenderframeImpl->SendHubbleMessageEx(session_);
			}
		}
	}

	void HubbleXobj::addEventListener(const String& SubObjName, const String& eventName, V8ApplicationCallback* callback)
	{
		if (callback)
		{
			auto it = session_.m_mapString.find(L"objID");
			if (it != session_.m_mapString.end())
			{
				hubble_->mapCloudSession_.insert(id_, this);
				String eventName_ = eventName + "@" + SubObjName;
				auto it = mapHubbleEventCallback_.find(eventName_);
				if (it == mapHubbleEventCallback_.end())
				{
					mapHubbleEventCallback_.insert(eventName_, callback);
					//插入callbackID:
					String callbackid_ = WTF::CreateCanonicalUUIDString();
					std::wstring strID = WebString(callbackid_).Utf16();
					setStr(L"callbackid", callbackid_);
					//绑定事件名称与callbackid建立对应关系：
					session_.m_mapString[strID] = WebString(eventName_).Utf16();

					//允许RenderFrameImpl根据回调id查找对应的session：
					m_pRenderframeImpl->m_mapHubbleSession[strID] = this;
				}
				else {
					mapHubbleEventCallback_.erase(it);
					mapHubbleEventCallback_.insert(eventName_, callback);
				}
				//通知客户端建立监听连接：
				m_pRenderframeImpl->SendHubbleMessageEx(session_);
			}
		}
	}

	void HubbleXobj::removeEventListener(const String& eventName)
	{
		auto it = mapHubbleEventCallback_.find(eventName);
		if (it != mapHubbleEventCallback_.end())
			mapHubbleEventCallback_.erase(it);
	}

	void HubbleXobj::disConnect()
	{
		int nSize = mapHubbleEventCallback_.size();
		if (nSize)
		{
			while (mapHubbleEventCallback_.size())
			{
				auto it = mapHubbleEventCallback_.begin();
				mapHubbleEventCallback_.erase(it);
			}
		}
	}

	void HubbleXobj::fireEvent(const String& eventName, HubbleXobj* eventParam)
	{
		auto itcallback = mapHubbleEventCallback_.find(eventName);
		if (itcallback != mapHubbleEventCallback_.end())
		{
			setStr(L"callbackid", L"");
			blink::V8ApplicationCallback* callback = (blink::V8ApplicationCallback*)itcallback->value.Get();
			ScriptState* callback_relevant_script_state = callback->CallbackRelevantScriptState();
			ScriptState::Scope callback_relevant_context_scope(callback_relevant_script_state);
			callback->InvokeAndReportException(nullptr, eventParam);
		}
	}

	void HubbleXobj::sendMessage(HubbleXobj* msg, V8ApplicationCallback* callback)
	{
		if (m_pRenderframeImpl)
		{
			msg->setStr(L"senderid", id_);
			if (callback)
			{
				String callbackid_ = WTF::CreateCanonicalUUIDString();
				msg->setStr(L"callbackid", callbackid_);
				mapHubbleEventCallback_.insert(callbackid_, callback);
				WebString strID = callbackid_;
				m_pRenderframeImpl->m_mapHubbleSession[strID.Utf16()] = this;
			}
			m_pRenderframeImpl->SendHubbleMessageEx(msg->session_);
		}
	}

	void HubbleXobj::invokeCallback(wstring callbackid, HubbleXobj* callbackParam)
	{
		auto itcallback = mapHubbleEventCallback_.find(String(callbackid.c_str()));
		if (itcallback != mapHubbleEventCallback_.end())
		{
			blink::V8ApplicationCallback* callback = (blink::V8ApplicationCallback*)itcallback->value.Get();
			mapHubbleEventCallback_.erase(itcallback);
			ScriptState* callback_relevant_script_state = callback->CallbackRelevantScriptState();
			ScriptState::Scope callback_relevant_context_scope(callback_relevant_script_state);
			callback->InvokeAndReportException(nullptr, callbackParam);
		}
	}

}  // namespace blink
// end Add by HubbleTeam
