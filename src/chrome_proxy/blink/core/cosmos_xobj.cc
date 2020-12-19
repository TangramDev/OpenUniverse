// begin Add by CosmosTeam
#include "cosmos.h"
#include "cosmos_xobj.h"
#include "cosmos_node.h"
#include "cosmos_event.h"
#include "cosmos_winform.h"
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

	CosmosXobj::CosmosXobj() {
		uiElem_ = nullptr;
		refElem_ = nullptr;
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

	CosmosXobj::~CosmosXobj() {
		session_.m_mapString.clear();
		session_.m_mapLong.clear();
		session_.m_mapFloat.clear();
		mapCosmosEventCallback_.clear();
	}

	CosmosXobj* CosmosXobj::Create(const String& strNodeXml) {
		return MakeGarbageCollected<CosmosXobj>(strNodeXml);
	}

	CosmosXobj::CosmosXobj(const String& strName)
	{
		name_ = strName;
	}

	void CosmosXobj::Trace(blink::Visitor* visitor) {
		ScriptWrappable::Trace(visitor);
		visitor->Trace(mapVisibleElem);
		visitor->Trace(mapCosmosEventCallback_);
		visitor->Trace(sender_);
		visitor->Trace(cosmos_);
		visitor->Trace(element_);
		visitor->Trace(eventElem_);
		visitor->Trace(hostElem_);
		visitor->Trace(uiElem_);
		visitor->Trace(refElem_);
		visitor->Trace(DocumentFragment_);
		visitor->Trace(messageElem_);
		visitor->Trace(propertyElem_);
		visitor->Trace(m_pVisibleContentElement);
	}

	void CosmosXobj::AddedEventListener(const AtomicString& event_type,
		RegisteredEventListener& registered_listener) {
		EventTargetWithInlineData::AddedEventListener(event_type,
			registered_listener);
	}

	const AtomicString& CosmosXobj::InterfaceName() const {
		return event_target_names::kCosmosXobj;
	}

	ExecutionContext* CosmosXobj::GetExecutionContext() const {
		return cosmos_->GetExecutionContext();
	}

	String CosmosXobj::name() {
		String name = getStr(L"name@page");
		if (name == "")
		{
			if (hostElem_)
			{
				name = hostElem_->GetIdAttribute();
				if (name == ""||name.IsNull())
					name = hostElem_->tagName().LowerASCII();
			}
		}
		return name;
	}

	String CosmosXobj::getid()
	{
		return id_;
	}

	CosmosWinform* CosmosXobj::form()
	{
		__int64 nHandle = getInt64(L"formhandle");
		if (nHandle)
			return (CosmosWinform*)this;
		//auto it = cosmos_->m_mapWinForm.find(nHandle);
		//if (it != cosmos_->m_mapWinForm.end())
		//	return it->value.Get();
		return nullptr;
	}

	CosmosNode* CosmosXobj::grid()
	{
		__int64 nHandle = getInt64(L"gridhandle");
		if (nHandle)
			return (CosmosNode*)this;
		return nullptr;
	}

	CosmosGalaxy* CosmosXobj::galaxy()
	{
		__int64 nHandle = getInt64(L"gridhandle");
		if (nHandle)
		{
			return ((CosmosNode*)this)->parentGalaxy();
		}
		return nullptr;
	}

	void CosmosXobj::setSender(CosmosXobj* xobj)
	{
		sender_ = xobj;
	}

	CosmosXobj* CosmosXobj::sender()
	{
		return sender_.Get();
	}

	Element* CosmosXobj::element() {
		return  hostElem_.Get();
	}

	Element* CosmosXobj::workElement() {
		return  element_.Get();
	}

	void CosmosXobj::setWorkElement(Element* elem) {
		element_ = elem;
	}

	Element* CosmosXobj::eventElement()
	{
		return eventElem_;
	}

	Element* CosmosXobj::messageElement()
	{
		return messageElem_;
	}

	Element* CosmosXobj::uiElement()
	{
		return uiElem_;
	}

	Element* CosmosXobj::refElement()
	{
		return refElem_;
	}

	Element* CosmosXobj::propertyElement()
	{
		return propertyElem_;
	}

	DocumentFragment* CosmosXobj::docFragment()
	{
		return  DocumentFragment_.Get();
	}

	void CosmosXobj::BindCtrlValue(const String& strcontrols, V8ApplicationCallback* callback)
	{
		if (callback)
		{
			setStr(L"eventtype", L"BindCtrlValue");
			setStr(L"ctrls", strcontrols);
			addEventListener(L"BindCtrlValue", L"OnTextChanged", callback);
		}
	}

	void CosmosXobj::setStr(const String& strKey, const String& value)
	{
		WebString str = strKey;
		WebString val = value;
		session_.m_mapString[str.Utf16()] = val.Utf16();
		auto it = session_.m_mapint64.find(WebString(strKey).Utf16());
		if (it != session_.m_mapint64.end())
		{
			setStr(L"msgID", L"MODIFY_CTRL_VALUE");
			setStr(L"currentsubobjformodify", strKey);
			m_pRenderframeImpl->SendCosmosMessageEx(session_);
		}
		if (value == "")
		{
			auto it = session_.m_mapString.find(WebString(strKey).Utf16());
			if (it != session_.m_mapString.end())
				session_.m_mapString.erase(it);
		}
	}

	String CosmosXobj::getStr(const String& strKey)
	{
		WebString str = strKey;
		auto it = session_.m_mapString.find(str.Utf16());
		if (it != session_.m_mapString.end())
		{
			return String(it->second.c_str());
		}
		return L"";
	}

	void CosmosXobj::setLong(const String& strKey, long value)
	{
		WebString str = strKey;
		session_.m_mapLong[str.Utf16()] = value;
	}

	long CosmosXobj::getLong(const String& strKey)
	{
		WebString str = strKey;
		auto it = session_.m_mapLong.find(str.Utf16());
		if (it != session_.m_mapLong.end())
		{
			return it->second;
		}
		return 0;
	}

	void CosmosXobj::setInt64(const String& strKey, int64_t value)
	{
		WebString str = strKey;
		auto it = session_.m_mapint64.find(str.Utf16());
		if (it != session_.m_mapint64.end())
		{
			session_.m_mapint64.erase(it);
		}
		session_.m_mapint64[str.Utf16()] = value;
	}

	int64_t CosmosXobj::getInt64(const String& strKey)
	{
		WebString str = strKey;
		auto it = session_.m_mapint64.find(str.Utf16());
		if (it != session_.m_mapint64.end())
		{
			return it->second;
		}
		return 0;
	}

	void CosmosXobj::setFloat(const String& strKey, float value)
	{
		WebString str = strKey;
		session_.m_mapFloat[str.Utf16()] = value;
	}

	float CosmosXobj::getFloat(const String& strKey)
	{
		WebString str = strKey;
		auto it = session_.m_mapFloat.find(str.Utf16());
		if (it != session_.m_mapFloat.end())
			return it->second;
		return 0;
	}

	void CosmosXobj::setMsgID(const String& value)
	{
		WebString str = "msgID";
		WebString val = value;
		session_.m_mapString[str.Utf16()] = val.Utf16();
	}

	String CosmosXobj::msgID()
	{
		WebString str = "msgID";
		auto it = session_.m_mapString.find(str.Utf16());
		if (it != session_.m_mapString.end())
		{
			return String(it->second.c_str());
		}
		return L"";
	}

	void CosmosXobj::setVisibleElement(const String& strKey, Element* value)
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

	Element* CosmosXobj::getVisibleElement(const String& strKey)
	{
		auto it = mapVisibleElem.find(strKey);
		if (it != mapVisibleElem.end())
		{
			return it->value.Get();
		}
		return nullptr;
	}

	void CosmosXobj::addEventListener(const String& eventName, V8ApplicationCallback* callback)
	{
		if (callback)
		{
			auto it = session_.m_mapString.find(L"objID");
			if (it != session_.m_mapString.end())
			{
				if (id_ == nullptr)
					id_ = WTF::CreateCanonicalUUIDString();
				cosmos_->mapCloudSession_.insert(id_, this);
				//插入callbackID:
				String callbackid_ = WTF::CreateCanonicalUUIDString();
				std::wstring strID = WebString(callbackid_).Utf16();
				setStr(L"callbackid", callbackid_);

				mapCosmosEventCallback_.insert(eventName, callback);
				//绑定事件名称与callbackid建立对应关系：
				session_.m_mapString[strID] = WebString(eventName).Utf16();
				session_.m_mapString[L"currentsubobjforlistener"] = L"";

				//允许RenderFrameImpl根据回调id查找对应的session：
				m_pRenderframeImpl->m_mapCosmosSession[strID] = this;
				//通知客户端建立监听连接：
				m_pRenderframeImpl->SendCosmosMessageEx(session_);
			}
		}
	}

	void CosmosXobj::addEventListener(const String& SubObjName, const String& eventName, V8ApplicationCallback* callback)
	{
		if (callback)
		{
			auto it = session_.m_mapString.find(L"objID");
			if (it != session_.m_mapString.end())
			{
				cosmos_->mapCloudSession_.insert(id_, this);
				String eventName_ = eventName + "@" + SubObjName;
				auto it = mapCosmosEventCallback_.find(eventName_);
				if (it == mapCosmosEventCallback_.end())
				{
					mapCosmosEventCallback_.insert(eventName_, callback);
					//插入callbackID:
					String callbackid_ = WTF::CreateCanonicalUUIDString();
					std::wstring strID = WebString(callbackid_).Utf16();
					setStr(L"callbackid", callbackid_);
					//绑定事件名称与callbackid建立对应关系：
					session_.m_mapString[strID] = WebString(eventName_).Utf16();

					//允许RenderFrameImpl根据回调id查找对应的session：
					m_pRenderframeImpl->m_mapCosmosSession[strID] = this;
				}
				else {
					mapCosmosEventCallback_.erase(it);
					mapCosmosEventCallback_.insert(eventName_, callback);
				}
				//通知客户端建立监听连接：
				setStr(L"msgID", "WINFORM_CREATED");
				m_pRenderframeImpl->SendCosmosMessageEx(session_);
			}
		}
	}

	void CosmosXobj::removeEventListener(const String& eventName)
	{
		auto it = mapCosmosEventCallback_.find(eventName);
		if (it != mapCosmosEventCallback_.end())
			mapCosmosEventCallback_.erase(it);
	}

	void CosmosXobj::disConnect()
	{
		int nSize = mapCosmosEventCallback_.size();
		if (nSize)
		{
			while (mapCosmosEventCallback_.size())
			{
				auto it = mapCosmosEventCallback_.begin();
				mapCosmosEventCallback_.erase(it);
			}
		}
	}

	void CosmosXobj::fireEvent(const String& eventName, CosmosXobj* eventParam)
	{
		auto itcallback = mapCosmosEventCallback_.find(eventName);
		if (itcallback != mapCosmosEventCallback_.end())
		{
			setStr(L"callbackid", L"");
			blink::V8ApplicationCallback* callback = (blink::V8ApplicationCallback*)itcallback->value.Get();
			ScriptState* callback_relevant_script_state = callback->CallbackRelevantScriptState();
			ScriptState::Scope callback_relevant_context_scope(callback_relevant_script_state);
			callback->InvokeAndReportException(nullptr, eventParam);
		}
	}

	void CosmosXobj::sendMessage(CosmosXobj* msg, V8ApplicationCallback* callback)
	{
		if (m_pRenderframeImpl)
		{
			if (msg == nullptr)
				msg = this;
			msg->setStr(L"senderid", id_);
			__int64 nHandle = getInt64(L"gridhandle");
			if(nHandle==0)
				nHandle = getInt64(L"formhandle");
			msg->setInt64(L"sender", nHandle);
			if (callback)
			{
				String callbackid_ = WTF::CreateCanonicalUUIDString();
				msg->setStr(L"callbackid", callbackid_);
				mapCosmosEventCallback_.insert(callbackid_, callback);
				WebString strID = callbackid_;
				m_pRenderframeImpl->m_mapCosmosSession[strID.Utf16()] = this;
			}
			m_pRenderframeImpl->SendCosmosMessageEx(msg->session_);
		}
	}

	void CosmosXobj::invokeCallback(wstring callbackid, CosmosXobj* callbackParam)
	{
		auto itcallback = mapCosmosEventCallback_.find(String(callbackid.c_str()));
		if (itcallback != mapCosmosEventCallback_.end())
		{
			blink::V8ApplicationCallback* callback = (blink::V8ApplicationCallback*)itcallback->value.Get();
			mapCosmosEventCallback_.erase(itcallback);
			ScriptState* callback_relevant_script_state = callback->CallbackRelevantScriptState();
			ScriptState::Scope callback_relevant_context_scope(callback_relevant_script_state);
			callback->InvokeAndReportException(this, callbackParam);
		}
	}

	void CosmosXobj::ProcessNodeMessage(const String& msgID)
	{
		CosmosNode* thisGrid = grid();
		wstring strHandles = L"";
		if (thisGrid && messageElem_ && msgID.IsNull() == false && msgID != "")
		{
			wstring _strID = WebString(msgID).Utf16();
			CosmosNode* gridfortarget = nullptr;
			HTMLCollection* list = messageElem_->getElementsByTagName(AtomicString(msgID.LowerASCII()));
			if (list->length())
			{
				Element* msg = list->item(0);
				AtomicString _strHandles = msg->getAttribute("gridhandles");
				if (_strHandles.IsNull() == false && _strHandles != "")
				{
					String s = _strHandles;
					strHandles = WebString(s).Utf16();
					const std::vector<std::wstring> strVecHandles = base::SplitString(
						strHandles, L"|", base::TRIM_WHITESPACE, base::SPLIT_WANT_NONEMPTY);
					int nCount = strVecHandles.size();
					for (int i = 0; i < nCount; i++)
					{
						__int64 nHandle = std::stoll(strVecHandles[i]);
						if (nHandle)
						{
							auto it = cosmos_->m_mapCosmosNode.find(nHandle);
							if (it != cosmos_->m_mapCosmosNode.end())
							{
								Element* elem = nullptr;
								auto it1 = m_mapElement.find(_strID + L"__" + std::to_wstring(nHandle));
								if (it1 != m_mapElement.end())
									elem = it1->second;
								CosmosNode* gridfortarget = it->value;
								if (!!gridfortarget) {
									gridfortarget->setWorkElement(elem);
									gridfortarget->setMsgID(msgID);
									gridfortarget->setSender(gridfortarget);
									gridfortarget->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCloudmessageforgrid, gridfortarget));
								}
							}
						}
					}
				}
				else {
					HTMLCollection* plist = msg->Children();
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

							gridfortarget = cosmos_->getGrid(galaxy, cluster, target);
							if (gridfortarget == nullptr) {
								CosmosWinform* form = thisGrid->parentForm();
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
							gridfortarget->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCloudmessageforgrid, gridfortarget));
						}
					}
					if (strHandles != L"")
					{
						msg->setAttribute("gridhandles", AtomicString(String(strHandles.c_str())));
					}
				}
			}
		}
	}

	void CosmosXobj::DispatchGridEvent(Element* e, const String& eventName)
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

								CosmosNode* gridfortarget = cosmos_->getGrid(galaxy, cluster, target);
								if (gridfortarget == nullptr)
								{
									CosmosNode* thisGrid = grid();
									if (thisGrid && thisGrid->m_pParentForm)
										gridfortarget = thisGrid->m_pParentForm->getGrid(galaxy, cluster, target);
									else
									{
										CosmosWinform* form_ = form();
										if (form_)
											gridfortarget = form_->getGrid(galaxy, cluster, target);
									}
								}
								if (!!gridfortarget) {
									gridfortarget->setWorkElement(elem);
									gridfortarget->setMsgID(e->GetIdAttribute() + "_" + eventName);
									gridfortarget->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCloudmessageforgrid, gridfortarget));
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

	Element* CosmosXobj::getElementById(const String& strID)
	{
		return DocumentFragment_->getElementById(AtomicString(strID));
	}

	NameNodeList* CosmosXobj::getElementsByName(const String& localName)
	{
		return DocumentFragment_->getElementsByName(AtomicString(localName));
	}

	HTMLCollection* CosmosXobj::getElementsByTagName(const String& localName)
	{
		return DocumentFragment_->getElementsByTagName(AtomicString(localName));
	}
}  // namespace blink
// end Add by CosmosTeam
