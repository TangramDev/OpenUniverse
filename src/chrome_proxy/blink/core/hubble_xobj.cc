// begin Add by HubbleTeam
#include "hubble.h"
#include "hubble_xobj.h"
#include "hubble_node.h"
#include "hubble_event.h"
#include "hubble_winform.h"
#include "base/strings/string_split.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/dom_token_list.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/dom/document_fragment.h"
#include "third_party/blink/renderer/core/dom/node.h"
#include "third_party/blink/renderer/core/dom/node_list.h"
#include "third_party/blink/renderer/core/html/html_element.h"
#include "third_party/blink/renderer/core/html/html_head_element.h"
#include "third_party/blink/renderer/core/dom/class_collection.h"
#include "third_party/blink/renderer/core/html/html_element.h"
#include "third_party/blink/renderer/core/html/html_head_element.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/public/web/web_local_frame_client.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_application_callback.h"
#include "third_party/blink/renderer/bindings/core/v8/string_or_trusted_html.h"

namespace blink {

	HubbleXobj::HubbleXobj() {
		uiElem_ = nullptr;
		sender_ = nullptr;
		hostElem_ = nullptr;
		eventElem_ = nullptr;
		messageElem_ = nullptr;
		propertyElem_ = nullptr;
		DocumentFragment_ = nullptr;
		m_pRenderframeImpl = nullptr;
		m_pVisibleContentElement = nullptr;
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
		visitor->Trace(element_);
		visitor->Trace(eventElem_);
		visitor->Trace(hostElem_);
		visitor->Trace(uiElem_);
		visitor->Trace(DocumentFragment_);
		visitor->Trace(messageElem_);
		visitor->Trace(propertyElem_);
		visitor->Trace(m_pVisibleContentElement);
	}

	void HubbleXobj::AddedEventListener(const AtomicString& event_type,
		RegisteredEventListener& registered_listener) {
		EventTargetWithInlineData::AddedEventListener(event_type,
			registered_listener);
	}

	const AtomicString& HubbleXobj::InterfaceName() const {
		//int64_t nHandle = getInt64(L"gridhandle");
		//if(nHandle)
		//	return event_target_names::kGrid;
		//nHandle = getInt64(L"formhandle");
		//if (nHandle)
		//	return event_target_names::kHubbleWinForm;
		return event_target_names::kHubbleXobj;
	}

	ExecutionContext* HubbleXobj::GetExecutionContext() const {
		return hubble_->GetExecutionContext();
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
		if (nHandle)
			return (HubbleWinform*)this;
		//auto it = hubble_->m_mapWinForm.find(nHandle);
		//if (it != hubble_->m_mapWinForm.end())
		//	return it->value.Get();
		return nullptr;
	}

	HubbleNode* HubbleXobj::grid()
	{
		__int64 nHandle = getInt64(L"gridhandle");
		if (nHandle)
			return (HubbleNode*)this;
		//auto it = hubble_->m_mapHubbleNode.find(nHandle);
		//if (it != hubble_->m_mapHubbleNode.end())
		//	return it->value.Get();
		return nullptr;
	}

	HubbleGalaxy* HubbleXobj::galaxy()
	{
		__int64 nHandle = getInt64(L"gridhandle");
		if (nHandle)
		{
			return ((HubbleNode*)this)->parentGalaxy();
		}
		//auto it = hubble_->m_mapHubbleNode.find(nHandle);
		//if (it != hubble_->m_mapHubbleNode.end())
		//{
		//	return it->value->parentGalaxy();
		//}
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

	Element* HubbleXobj::element() {
		return  hostElem_.Get();
	}

	Element* HubbleXobj::workElement() {
		return  element_.Get();
	}

	void HubbleXobj::setWorkElement(Element* elem) {
		element_ = elem;
	}

	Element* HubbleXobj::eventElement()
	{
		return eventElem_;
	}

	Element* HubbleXobj::messageElement()
	{
		return messageElem_;
	}

	Element* HubbleXobj::uiElement()
	{
		return uiElem_;
	}

	Element* HubbleXobj::propertyElement()
	{
		return propertyElem_;
	}

	DocumentFragment* HubbleXobj::docFragment()
	{
		return  DocumentFragment_.Get();
	}

	void HubbleXobj::SyncCtrlTextChange(const String& strcontrols, V8ApplicationCallback* callback)
	{
		if (callback)
		{
			setStr(L"eventtype", L"SyncCtrlTextChange");
			setStr(L"ctrls", strcontrols);
			addEventListener(L"SyncCtrlTextChange", L"OnTextChanged", callback);
		}
	}

	void HubbleXobj::setStr(const String& strKey, const String& value)
	{
		WebString str = strKey;
		WebString val = value;
		session_.m_mapString[str.Utf16()] = val.Utf16();
		auto it = session_.m_mapint64.find(WebString(strKey).Utf16());
		if (it != session_.m_mapint64.end())
		{
			setStr(L"msgID", L"MODIFY_CTRL_VALUE");
			setStr(L"currentsubobjformodify", strKey);
			m_pRenderframeImpl->SendHubbleMessageEx(session_);
		}
		if (value == "")
		{
			auto it = session_.m_mapString.find(WebString(strKey).Utf16());
			if (it != session_.m_mapString.end())
				session_.m_mapString.erase(it);
		}
	}

	String HubbleXobj::getStr(const String& strKey)
	{
		WebString str = strKey;
		auto it = session_.m_mapString.find(str.Utf16());
		if (it != session_.m_mapString.end())
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
		session_.m_mapFloat[str.Utf16()] = value;
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
				if (id_ == nullptr)
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
			if (msg == nullptr)
				msg = this;
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
			callback->InvokeAndReportException(this, callbackParam);
		}
	}

	void HubbleXobj::ProcessNodeMessage(const String& msgID)
	{
		HubbleNode* thisGrid = grid();
		wstring strHandles = L"";
		if (thisGrid && messageElem_ && msgID.IsNull() == false && msgID != "")
		{
			wstring _strID = WebString(msgID).Utf16();
			auto it = m_mapMsgInfo.find(_strID);
			if (it == m_mapMsgInfo.end())
			{
				HubbleNode* gridfortarget = nullptr;
				HTMLCollection* list = messageElem_->getElementsByTagName(AtomicString(msgID.LowerASCII()));
				for (unsigned int i = 0; i < list->length(); i++)
				{
					HTMLCollection* plist = list->item(i)->Children();
					for (unsigned int i = 0; i < plist->length(); i++)
					{
						Element* elem = plist->item(i);
						AtomicString target = "";
						target = elem->getAttribute("target");
						if (target.IsNull() || target == "")
						{
							gridfortarget = thisGrid;
						}
						else {
							AtomicString galaxy = elem->getAttribute("galaxy");
							AtomicString cluster = elem->getAttribute("cluster");
							if (galaxy == "")
								galaxy = "default";
							if (cluster == "")
								cluster = "__viewport_default__";

							gridfortarget = hubble_->getGrid(galaxy, cluster, target);
							if (gridfortarget == nullptr) {
								HubbleWinform* form = thisGrid->parentForm();
								if (form)
								{
									gridfortarget = form->getGrid(galaxy, cluster, target);
								}
							}
						}
						if (!!gridfortarget) {
							__int64 nHandle = gridfortarget->handle();
							wstring strHandle = std::to_wstring(nHandle);
							strHandles += strHandle + L"|";
							m_mapElement[_strID + L"__" + strHandle] = elem;
							gridfortarget->setWorkElement(elem);
							gridfortarget->setMsgID(msgID);
							gridfortarget->setSender(gridfortarget);
							gridfortarget->DispatchEvent(*blink::HubbleEvent::Create(blink::event_type_names::kCloudmessageforgrid, gridfortarget));
						}
					}
				}
				if (strHandles != L"")
				{
					m_mapMsgInfo[_strID] = strHandles;
				}
			}
			else
			{
				wstring strHandles = it->second;
				const std::vector<std::wstring> strVecHandles = base::SplitString(
					strHandles, L"|", base::TRIM_WHITESPACE, base::SPLIT_WANT_NONEMPTY);
				int nCount = strVecHandles.size();
				for (int i = 0; i < nCount; i++)
				{
					__int64 nHandle = std::stoll(strVecHandles[i]);
					if (nHandle)
					{
						auto it = hubble_->m_mapHubbleNode.find(nHandle);
						if (it != hubble_->m_mapHubbleNode.end())
						{
							Element* elem = nullptr;
							auto it1 = m_mapElement.find(_strID + L"__" + std::to_wstring(nHandle));
							if (it1 != m_mapElement.end())
								elem = it1->second;
							HubbleNode* gridfortarget = it->value;
							if (!!gridfortarget) {
								gridfortarget->setWorkElement(elem);
								gridfortarget->setMsgID(msgID);
								gridfortarget->setSender(gridfortarget);
								gridfortarget->DispatchEvent(*blink::HubbleEvent::Create(blink::event_type_names::kCloudmessageforgrid, gridfortarget));
							}
						}
					}
				}
			}
		}
	}

	void HubbleXobj::DispatchGridEvent(Element* e, const String& eventName)
	{
		Element* element = static_cast<Element*>(e->childNodes()->item(1));
		if (!!element) {
			for (unsigned int i = 1; i < element->childNodes()->length(); i++) {
				Element* elem = static_cast<Element*>(element->childNodes()->item(i));
				if (elem)
				{
					Node* pNode = elem;
					if (pNode->getNodeType() == 1) {
						AtomicString target = "";
						if (elem->hasAttribute("target"))
						{
							target = elem->getAttribute("target");
							if (target != "") {
								AtomicString galaxy = elem->getAttribute("galaxy");
								AtomicString cluster = elem->getAttribute("cluster");
								if (galaxy == "")
									galaxy = "default";
								if (cluster == "")
									cluster = "__viewport_default__";

								HubbleNode* gridfortarget = hubble_->getGrid(galaxy, cluster, target);
								if (gridfortarget == nullptr)
								{
									HubbleNode* thisGrid = grid();
									if (thisGrid && thisGrid->m_pParentForm)
										gridfortarget = thisGrid->m_pParentForm->getGrid(galaxy, cluster, target);
									else
									{
										HubbleWinform* form_ = form();
										if (form_)
											gridfortarget = form_->getGrid(galaxy, cluster, target);
									}
								}
								if (!!gridfortarget) {
									gridfortarget->setWorkElement(elem);
									gridfortarget->setMsgID(e->GetIdAttribute() + "_" + eventName);
									gridfortarget->DispatchEvent(*blink::HubbleEvent::Create(blink::event_type_names::kCloudmessageforgrid, gridfortarget));
								}
							}
						}
					}
				}
			}
		}
		//var element = e.childNodes[1];
		//if (!!element) {
		//    alert(element.childNodes.length);
		//    for (var i = 1; i < element.childNodes.length; i++) {
		//        var elem = element.childNodes[i];
		//        alert(elem.nodeType);
		//        if (elem.nodeType == 1) {
		//           var target = elem.getAttribute("target");
		//           var galaxy = elem.getAttribute("galaxy");
		//           var cluster = elem.getAttribute("cluster");
		//            if (!!target && !!galaxy && !!cluster) {
		//                var gridfortarget = apppage.getGrid(galaxy, cluster, target);
		//                if (!!gridfortarget) {
		//                    gridfortarget.workElement = elem;
		//                    gridfortarget.msgID = e.id + "_OnClick";
		//                    gridfortarget.sendMessageToGrid(gridfortarget);
		//                }
		//            }
		//        }
		//    }
		//}
	}
}  // namespace blink
// end Add by HubbleTeam
