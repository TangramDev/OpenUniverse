#include "hubble.h"
#include "hubble_node.h"
#include "hubble_xobj.h"
#include "hubble_event.h"
#include "hubble_winform.h"
#include "hubble_control.h"
#include "hubble_galaxy.h"
#include "hubble_compositor.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/document_fragment.h"
#include "third_party/blink/renderer/core/dom/node.h"
#include "third_party/blink/renderer/core/dom/node_list.h"
#include "third_party/blink/renderer/core/html/html_element.h"
#include "third_party/blink/renderer/core/html/html_head_element.h"
#include "third_party/blink/renderer/core/dom/class_collection.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/public/web/web_local_frame_client.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_application_callback.h"
#include "third_party/blink/renderer/platform/wtf/uuid.h"
#include "third_party/blink/renderer/core/xml/dom_parser.h"
#include "third_party/blink/renderer/bindings/core/v8/string_or_trusted_html.h"

namespace blink {

	HubbleWinform::HubbleWinform(LocalFrame* frame) : DOMWindowClient(frame) {
		m_pBindMdiNode = nullptr;
		m_pWebBindMdiNode = nullptr;
		m_pRenderframeImpl = nullptr;
		DocumentFragment_ = nullptr;
		m_pContentElement = nullptr;
		messageElem_ = nullptr;
		m_pMDIParent = nullptr;
		m_pActiveMDIChild = nullptr;
		formElem_ = nullptr;
		propertyElem_ = nullptr;
		eventElem_ = nullptr;
		uiElem_ = nullptr;
		id_ = WTF::CreateCanonicalUUIDString();
	}

	HubbleWinform::~HubbleWinform() {
		Hubble* pHubble = hubble_.Get();
		if (pHubble)
		{
			auto it = pHubble->m_mapHubbleWinformforEvent.find(handle_);
			if (it != pHubble->m_mapHubbleWinformforEvent.end())
				pHubble->m_mapHubbleWinformforEvent.erase(it);

			auto it2 = pHubble->m_mapWinForm.find(handle_);
			if (it2 != pHubble->m_mapWinForm.end())
				pHubble->m_mapWinForm.erase(it2);
		}
		mapHubbleEventCallback_.clear();
	}

	HubbleWinform* HubbleWinform::Create(LocalFrame* frame, const String& strNodeName) {
		return MakeGarbageCollected<HubbleWinform>(frame, strNodeName);
	}

	HubbleWinform::HubbleWinform(LocalFrame* frame, const String& strNodeName) : DOMWindowClient(frame)
	{
		name_ = strNodeName;
	}

	void HubbleWinform::Trace(blink::Visitor* visitor) {
		EventTargetWithInlineData::Trace(visitor);
		ScriptWrappable::Trace(visitor);
		DOMWindowClient::Trace(visitor);
		visitor->Trace(hubble_);
		visitor->Trace(innerXobj_);
		visitor->Trace(m_pBindMdiNode);
		visitor->Trace(eventElem_);
		visitor->Trace(formElem_);
		visitor->Trace(uiElem_);
		visitor->Trace(m_pMDIParent);
		visitor->Trace(m_pActiveMDIChild);
		visitor->Trace(messageElem_);
		visitor->Trace(propertyElem_);
		visitor->Trace(DocumentFragment_);
		visitor->Trace(m_pContentElement);
		visitor->Trace(m_pWebBindMdiNode);
		visitor->Trace(mapHubbleEventCallback_);
	}

	void HubbleWinform::AddedEventListener(const AtomicString& event_type,
		RegisteredEventListener& registered_listener) {
		EventTargetWithInlineData::AddedEventListener(event_type,
			registered_listener);
	}

	String HubbleWinform::name() {
		long nType = innerXobj_->getLong(L"WinFormType");
		String strname = "";
		switch (nType)
		{
		case 2:
			strname = innerXobj_->getStr(L"tagName");
			break;
		default:
		{
			strname = innerXobj_->getStr(L"id");
			if(strname=="")
				strname = innerXobj_->getStr(L"formname");
			if(strname=="")
				strname = innerXobj_->getStr(L"tagName");
		}
		break;
		}
		return strname;
	}

	DocumentFragment* HubbleWinform::docFragment()
	{
		return DocumentFragment_.Get();
	}

	HubbleXobj* HubbleWinform::xobj()
	{
		return innerXobj_;
	}

	HubbleNode* HubbleWinform::mdibindgrid()
	{
		if (m_pBindMdiNode)
			return m_pBindMdiNode;
		return nullptr;
	}

	HubbleNode* HubbleWinform::mdiwebbindgrid()
	{
		if (m_nMdiwebbindgridhandle)
		{
			if (m_pWebBindMdiNode == nullptr)
			{
				Hubble* pHubble = hubble_.Get();
				auto it1 = pHubble->m_mapHubbleNode.find(m_nMdiwebbindgridhandle);
				if (it1 != pHubble->m_mapHubbleNode.end())
				{
					m_pWebBindMdiNode = it1->value.Get();
				}
			}
		}
		if (m_pWebBindMdiNode)
			return m_pWebBindMdiNode;
		return nullptr;
	}

	HubbleWinform* HubbleWinform::mdiParent()
	{
		if (m_pMDIParent == nullptr)
		{
			Hubble* pHubble = hubble_.Get();
			int64_t nHandle = innerXobj_->getInt64("mdiformhandle");
			if (nHandle&&pHubble)
			{
				pHubble->m_mapWinForm.find(nHandle);
				auto it1 = pHubble->m_mapWinForm.find(nHandle);
				if (it1 != pHubble->m_mapWinForm.end())
				{
					m_pMDIParent = it1->value;
					return it1->value.Get();
				}
			}

		}
		return m_pMDIParent.Get();
	}

	Element* HubbleWinform::eventElement()
	{
		return eventElem_;
	}

	Element* HubbleWinform::messageElement()
	{
		return messageElem_;
	}

	Element* HubbleWinform::formElement()
	{
		return formElem_;
	}

	Element* HubbleWinform::uiElement()
	{
		return uiElem_;
	}

	Element* HubbleWinform::propertyElement()
	{
		return propertyElem_;

	}

	String HubbleWinform::getid()
	{
		return id_;
	}

	void HubbleWinform::setStr(const String& strKey, const String& value)
	{
		WebString str = strKey;
		WebString val = value;
		innerXobj_->session_.m_mapString[str.Utf16()] = val.Utf16();
		auto it = innerXobj_->session_.m_mapint64.find(WebString(strKey).Utf16());
		if (it != innerXobj_->session_.m_mapint64.end())
		{
			setStr(L"msgID", L"MODIFY_CTRL_VALUE");
			setStr(L"currentsubobjformodify", strKey);
			m_pRenderframeImpl->SendHubbleMessageEx(innerXobj_->session_);
		}
	}

	String HubbleWinform::getStr(const String& strKey)
	{
		WebString str = strKey;
		auto it = innerXobj_->session_.m_mapString.find(str.Utf16());
		if (it != innerXobj_->session_.m_mapString.end())
		{
			return String(it->second.c_str());
		}
		return L"";
	}

	void HubbleWinform::setLong(const String& strKey, long value)
	{
		WebString str = strKey;
		innerXobj_->session_.m_mapLong[str.Utf16()] = value;
	}

	long HubbleWinform::getLong(const String& strKey)
	{
		WebString str = strKey;
		auto it = innerXobj_->session_.m_mapLong.find(str.Utf16());
		if (it != innerXobj_->session_.m_mapLong.end())
		{
			return it->second;
		}
		return 0;
	}

	void HubbleWinform::setInt64(const String& strKey, int64_t value)
	{
		WebString str = strKey;
		auto it = innerXobj_->session_.m_mapint64.find(str.Utf16());
		if (it != innerXobj_->session_.m_mapint64.end())
		{
			innerXobj_->session_.m_mapint64.erase(it);
		}
		innerXobj_->session_.m_mapint64[str.Utf16()] = value;
	}

	int64_t HubbleWinform::getInt64(const String& strKey)
	{
		WebString str = strKey;
		auto it = innerXobj_->session_.m_mapint64.find(str.Utf16());
		if (it != innerXobj_->session_.m_mapint64.end())
		{
			return it->second;
		}
		return 0;
	}

	void HubbleWinform::setFloat(const String& strKey, float value)
	{
		WebString str = strKey;
		innerXobj_->session_.m_mapFloat[str.Utf16()] = value;
	}

	float HubbleWinform::getFloat(const String& strKey)
	{
		WebString str = strKey;
		auto it = innerXobj_->session_.m_mapFloat.find(str.Utf16());
		if (it != innerXobj_->session_.m_mapFloat.end())
			return it->second;
		return 0;
	}

	void HubbleWinform::setMsgID(const String& value)
	{
		WebString str = "msgID";
		WebString val = value;
		innerXobj_->session_.m_mapString[str.Utf16()] = val.Utf16();
	}

	String HubbleWinform::msgID()
	{
		WebString str = "msgID";
		auto it = innerXobj_->session_.m_mapString.find(str.Utf16());
		if (it != innerXobj_->session_.m_mapString.end())
		{
			return String(it->second.c_str());
		}
		return L"";
	}

	HubbleGalaxy* HubbleWinform::getGalaxy(const String& galaxyName)
	{
		auto it = m_mapHubbleGalaxy.find(WebString(galaxyName).Utf16());
		if (it != m_mapHubbleGalaxy.end())
			return it->second;
		return nullptr;
	}

	HubbleNode* HubbleWinform::getGrid(const String& galaxyName, const String& clusterName, const String& gridName)
	{
		String clusterName_ = clusterName;
		if (clusterName == "undefined" || clusterName == "" || clusterName.IsNull() == true)
			clusterName_ = "default";
		String gridName_ = gridName;
		if (gridName == "undefined" || gridName == "" || gridName.IsNull() == true)
			gridName_ = "default";
		auto it = m_mapHubbleGalaxy.find(WebString(galaxyName).Utf16());
		if (it != m_mapHubbleGalaxy.end())
		{
			clusterName_ = clusterName_ + "__" + gridName_;
			auto it2 = it->second->m_mapHubbleNode2.find(WebString(clusterName_).Utf16());
			if (it2 != it->second->m_mapHubbleNode2.end())
				return it2->second;
		}
		return nullptr;
	}

	HubbleNode* HubbleWinform::getGrid(Element* elem)
	{
		if (elem)
		{
			Node* pNode = elem;
			if (pNode->getNodeType() == 1) {
				AtomicString target = elem->getAttribute("target");
				AtomicString galaxy = elem->getAttribute("galaxy");
				AtomicString cluster = elem->getAttribute("cluster");
				if (galaxy == "" || galaxy.IsNull() == true)
					galaxy = "default";
				if (cluster == "" || cluster.IsNull() == true)
					cluster = "default";
				if (target.IsNull() == true || target == "")
				{
					auto it = m_mapHubbleGalaxy.find(WebString(galaxy).Utf16());
					if (it != m_mapHubbleGalaxy.end())
					{
						auto it2 = it->second->m_mapRootNode.find(WebString(cluster).Utf16());
						if (it2 != it->second->m_mapRootNode.end())
							return it2->second;
						return nullptr;
					}
				}
				else
				{
					HubbleNode* gridfortarget = getGrid(galaxy, cluster, target);
					if (!!gridfortarget) {
						return gridfortarget;
					}
				}
			}
		}
		return nullptr;
	}

	long HubbleWinform::formType()
	{
		long nFormType = innerXobj_->getLong(L"WinFormType");
		
		return nFormType;
	}

	bool HubbleWinform::isReady()
	{
		if (isReady_)
			return true;
		return false;
	}

	int64_t HubbleWinform::handle() {
		return handle_;
	}

	void HubbleWinform::SyncCtrlTextChange(const String& strcontrols, V8ApplicationCallback* callback)
	{
		if (callback)
		{
			innerXobj_->setStr(L"eventtype", L"SyncCtrlTextChange");
			innerXobj_->setStr(L"ctrls", strcontrols);
			innerXobj_->addEventListener(L"SyncCtrlTextChange", L"OnTextChanged", callback);
		}
	}

	void HubbleWinform::addEventListener(const String& eventName, V8ApplicationCallback* callback)
	{
		blink::Hubble* pHubble = (blink::Hubble*)DomWindow()->cosmos();
		if (callback)
		{
			auto it = innerXobj_->session_.m_mapString.find(L"objID");
			if (it != innerXobj_->session_.m_mapString.end())
			{
				pHubble->m_mapHubbleWinformforEvent[handle_] = this;
				mapHubbleEventCallback_.insert(eventName, callback);
				innerXobj_->session_.m_mapString[L"listenerType"] = WebString(eventName).Utf16();
				innerXobj_->session_.m_mapString[L"SenderType"] = L"HubbleForm";
				innerXobj_->session_.m_mapString[L"Sender"] = WebString(id_).Utf16();
				sendMessage(innerXobj_, nullptr);
			}
		}
	}

	void HubbleWinform::addEventListener(const String& subObjName, const String& eventName, V8ApplicationCallback* callback)
	{
		if (callback)
		{
			innerXobj_->addEventListener(subObjName, eventName, callback);
		}
	}

	void HubbleWinform::fireEvent(const String& eventName, HubbleXobj* eventParam)
	{
		auto itcallback = mapHubbleEventCallback_.find(eventName);
		if (itcallback != mapHubbleEventCallback_.end())
		{
			blink::V8ApplicationCallback* callback = (blink::V8ApplicationCallback*)itcallback->value;
			ScriptState* callback_relevant_script_state = callback->CallbackRelevantScriptState();
			ScriptState::Scope callback_relevant_context_scope(callback_relevant_script_state);
			callback->InvokeAndReportException(nullptr, eventParam);
		}
	}

	const AtomicString& HubbleWinform::InterfaceName() const {
		return event_target_names::kHubbleWinForm;
	}

	ExecutionContext* HubbleWinform::GetExecutionContext() const {
		return DomWindow()->document();
	}

	void HubbleWinform::removeEventListener(const String& eventName)
	{
		auto it = mapHubbleEventCallback_.find(eventName);
		if (it != mapHubbleEventCallback_.end())
			mapHubbleEventCallback_.erase(it);
	}

	void HubbleWinform::disConnect()
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

	void HubbleWinform::DispatchGridEvent(Element* e, const String& eventName)
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
									cluster = "default";
								HubbleNode* gridfortarget = getGrid(galaxy, cluster, target);
								if (gridfortarget == nullptr)
									gridfortarget = hubble_.Get()->getGrid(galaxy, cluster, target);
								if (!!gridfortarget) {
									//gridfortarget->setWorkElement(nullptr);
									gridfortarget->setWorkElement(elem);
									gridfortarget->setMsgID(e->GetIdAttribute() + "_" + eventName);
									gridfortarget->DispatchEvent(*blink::HubbleEvent::Create(blink::event_type_names::kCloudmessageforgrid, gridfortarget->xobj()));
									//gridfortarget->setWorkElement(nullptr);
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

	//void HubbleWinform::DispatchGridEvent(HubbleXobj* xObj, const String& ctrlName, const String& eventName)
	//{
	//	String ctrlName_ = ctrlName;
	//	if (ctrlName.IsNull() || ctrlName == "")
	//	{
	//		ctrlName_ = xObj->getStr(L"name@page");
	//		if (ctrlName_.IsNull() || ctrlName_ == "")
	//		{
	//			ctrlName_ = xObj->getStr(L"formname");
	//		}
	//	}
	//	if (DOMParser_ == nullptr)
	//	{
	//		DOMParser_ = DOMParser::Create(*(DomWindow()->document()));
	//	}
	//	if (DOMParser_)
	//	{
	//		ExceptionState exception_state(nullptr,
	//			ExceptionState::kExecutionContext,
	//			"DOMParser",
	//			"");
	//		Document* doc = DOMParser_->parseFromString(blink::StringOrTrustedHTML::FromString(xObj->getStr(eventName + "Xml")), "application/xml", exception_state);
	//		if (doc)
	//		{
	//			String eventName_ = eventName.LowerASCII();
	//			AtomicString name = AtomicString(eventName_);
	//			ContainerNode* pContainerNode = (ContainerNode*)doc->firstChild();
	//			HTMLCollection* list = pContainerNode->getElementsByTagName(name);
	//			if (list->length())
	//			{
	//				for (unsigned int index = 0; index < list->length(); index++)
	//				{
	//					Element* workItem = list->item(index);
	//					for (unsigned int i = 1; i < workItem->childNodes()->length(); i++)
	//					{
	//						Element* elem = (Element*)workItem->childNodes()->item(i);
	//						Node* pNode = elem;
	//						AtomicString target = "";
	//						if (pNode->getNodeType() == 1) {
	//							target = elem->getAttribute("target");
	//							if (target != "") {
	//								AtomicString galaxy = elem->getAttribute("galaxy");
	//								if (galaxy == "")
	//									galaxy = "default";

	//								AtomicString cluster = elem->getAttribute("cluster");
	//								if (cluster == "")
	//									cluster = "default";
	//								HubbleNode* gridfortarget = getGrid(galaxy, cluster, target);
	//								if (!!gridfortarget) {
	//									gridfortarget->element_ = elem;
	//									gridfortarget->setMsgID(ctrlName_ + "_" + eventName);
	//									gridfortarget->DispatchEvent(*blink::HubbleEvent::Create(blink::event_type_names::kCloudmessageforgrid, gridfortarget->xobj()));
	//									//gridfortarget->setWorkElement(nullptr);
	//								}
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//	//var domparser = new DOMParser();
	//	//var doc = domparser.parseFromString(e.getStr("OnAfterSelectXml"), "application/xml");
	//	//if (!!doc) {
	//	//    var webcontent = e.getStr("webcontent");
	//	//    var selectCluster = e.getStr(eventName+"_key");
	//	//    var list = doc.firstChild.getElementsByTagName(eventName.toLowerCase());
	//	//    if (list.length) {
	//	//        for (var index = 0; index < list.length;index++) {
	//	//            var onafterselect = list[index];
	//	//            for (var i = 1; i < onafterselect.childNodes.length; i++) {
	//	//                var elem = onafterselect.childNodes[i];
	//	//                if (elem.nodeType == 1) {
	//	//                    var target = elem.getAttribute("target");
	//	//                    var galaxy = elem.getAttribute("galaxy");
	//	//                    var cluster = elem.getAttribute("cluster");
	//	//                    if (!!target&&!!galaxy && !!cluster) {
	//	//                        var gridfortarget = apppage.getGrid(galaxy, cluster, target);
	//	//                        if (!!gridfortarget) {
	//	//                            gridfortarget.workElement = elem;
	//	//                            gridfortarget.setStr("targetCluster", selectCluster);
	//	//                            gridfortarget.msgID = "treeView1_" + eventName;
	//	//                            if (gridfortarget.objtype == "nucleus") {
	//	//                                gridfortarget.setStr("content_show", webcontent);
	//	//                                gridfortarget.setStr("content_parent", "contents");
	//	//                            }
	//	//                            gridfortarget.sendMessageToGrid(gridfortarget);
	//	//                        }
	//	//                    }
	//	//                }
	//	//            }
	//	//        }
	//	//    }
	//	//}
	//}

	void HubbleWinform::sendMessage(HubbleXobj* msg, V8ApplicationCallback* callback)
	{
		if (m_pRenderframeImpl)
		{
			if (msg == nullptr)
				msg = innerXobj_;
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

	void HubbleWinform::invokeCallback(wstring callbackid, HubbleXobj* callbackParam)
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

	void HubbleWinform::InitWinForm()
	{
		String strMessageXml = innerXobj_->getStr(L"hubblexml");
		if (strMessageXml.IsNull() == false && strMessageXml != "")
		{
			DocumentFragment_ = DomWindow()->document()->createDocumentFragment();
			if (DocumentFragment_)
			{
				ExceptionState exception_state(V8PerIsolateData::MainThreadIsolate(), ExceptionState::kSetterContext,
					"Element", "outerHTML");
				hubble_->helperElem_->SetInnerHTMLFromString(strMessageXml, exception_state);
				DocumentFragment_->appendChild(hubble_->helperElem_->firstChild());

				if (DocumentFragment_->Children()->length())
				{
					formElem_ = DocumentFragment_->Children()->item(0);
					HTMLCollection* list_ = formElem_->Children();
					for (unsigned int i = 0; i < list_->length(); i++)
					{
						Element* elem = list_->item(i);
						Node* pNode = elem;
						if (pNode->getNodeType() == 1)
						{
							String name = elem->tagName().LowerASCII();
							if (name == "message")
							{
								messageElem_ = elem;
							}
							else
							{
								HTMLCollection* list = elem->getElementsByTagName("event");
								for (unsigned int index = 0; index < list->length(); index++)
								{
									HTMLCollection* list2 = list->item(index)->Children();
									if (list2->length())
									{
										for (unsigned int i = 0; i < list2->length(); i++)
										{
											Element* e = list2->item(i);
											Node* pNode = e;
											Element* pPNode = (Element*)e->parentNode()->parentNode();
											if (pPNode && pPNode == elem && pNode->getNodeType() == 1)
											{
												String name = e->tagName().LowerASCII();
												String parentname = pPNode->tagName().LowerASCII();
												String strIndex = name + "@" + parentname;
												wstring key = WebString(strIndex).Utf16();
												auto it = m_mapEventInfo.find(key);
												if (it == m_mapEventInfo.end())
												{
													innerXobj_->setStr(L"msgID", L"BIND_NATIVEOBJ_IPC_MSG");
													innerXobj_->setStr(L"BindObj", parentname);
													innerXobj_->setStr(L"Bindevent", name);
													m_mapEventInfo[key] = e;
													m_pRenderframeImpl->SendHubbleMessageEx(innerXobj_->session_);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		innerXobj_->setStr(L"hubblexml", L"");
	}

	HubbleWinform* HubbleWinform::activeMDIChild()
	{
		if(m_pActiveMDIChild)
			return m_pActiveMDIChild.Get();
		return nullptr;
	}

	void HubbleWinform::ProcessFormMessage(const String& msgID)
	{
		if (messageElem_ && msgID.IsNull() == false && msgID != "")
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
					}
					else {
						AtomicString galaxy = elem->getAttribute("galaxy");
						AtomicString cluster = elem->getAttribute("cluster");
						if (galaxy == "")
							galaxy = "default";
						if (cluster == "")
							cluster = "default";

						gridfortarget = hubble_->getGrid(galaxy, cluster, target);
						if (gridfortarget == nullptr) {
							gridfortarget = getGrid(galaxy, cluster, target);
						}
						if (gridfortarget == nullptr) {
							HubbleWinform* Parentform = mdiParent();
							if(Parentform)
								gridfortarget = Parentform->getGrid(galaxy, cluster, target);
						}
						if (gridfortarget == nullptr) {
							long nFormType = innerXobj_->getLong(L"WinFormType");
							if (nFormType == 2)
							{
								HubbleWinform* childform = activeMDIChild();
								if(childform)
									gridfortarget = childform->getGrid(galaxy, cluster, target);
							}
						}
					}
					if (!!gridfortarget) {
						gridfortarget->setWorkElement(elem);
						gridfortarget->setMsgID(msgID);
						gridfortarget->xobj()->setSender(innerXobj_);
						gridfortarget->DispatchEvent(*blink::HubbleEvent::Create(blink::event_type_names::kCloudmessageforgrid, gridfortarget->xobj()));
					}
				}
			}
		}
	}
}  // namespace blink
