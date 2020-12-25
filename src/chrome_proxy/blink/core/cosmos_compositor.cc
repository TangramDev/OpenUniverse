/********************************************************************************
 *             DOM Plus for Application - Version 1.1.8.202012250001
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

#include "cosmos.h"
#include "cosmos_xobj.h"
#include "cosmos_node.h"
#include "cosmos_event.h"
#include "cosmos_winform.h"
#include "cosmos_compositor.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/html/html_element.h"
#include "third_party/blink/renderer/core/html/html_head_element.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/public/web/web_local_frame_client.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"

namespace blink {

CosmosCompositor::CosmosCompositor(LocalFrame* frame) : DOMWindowClient(frame) {
	m_pRenderframeImpl = nullptr;
	id_ = WTF::CreateCanonicalUUIDString();
}

CosmosCompositor::~CosmosCompositor() {
}

CosmosCompositor* CosmosCompositor::Create(LocalFrame* frame, const String& strNodeXml) {
	return MakeGarbageCollected<CosmosCompositor>(frame, strNodeXml);
}

CosmosCompositor::CosmosCompositor(LocalFrame* frame, const String& strNodeXml) : DOMWindowClient(frame)
{
	m_strXobjXml = strNodeXml;
}

void CosmosCompositor::Trace(blink::Visitor* visitor) {
  EventTargetWithInlineData::Trace(visitor);
  ScriptWrappable::Trace(visitor);
  DOMWindowClient::Trace(visitor);
  visitor->Trace(innerXobj_);
}

void CosmosCompositor::AddedEventListener(const AtomicString& event_type,
                                 RegisteredEventListener& registered_listener) {
  EventTargetWithInlineData::AddedEventListener(event_type,
                                                registered_listener);
}

String CosmosCompositor::getid()
{
	return id_;
}

String CosmosCompositor::name() {
	return name_;
}

void CosmosCompositor::sendMessage(const String& id, const String& param1, const String& param2, const String& param3, const String& param4, const String& param5) {
	if (m_pRenderframeImpl) {
		WebString webstr = id;
		std::wstring u16_id = webstr.Utf16();
		webstr = param1;
		std::wstring u16_param1 = webstr.Utf16();
		webstr = param2;
		std::wstring u16_param2 = webstr.Utf16();
		webstr = param3;
		std::wstring u16_param3 = webstr.Utf16();
		webstr = param4;
		std::wstring u16_param4 = webstr.Utf16();
		webstr = param5;
		std::wstring u16_param5 = webstr.Utf16();
		m_pRenderframeImpl->SendCosmosMessage(u16_id, u16_param1, u16_param2, u16_param3, u16_param4, u16_param5);
	}
}

const AtomicString& CosmosCompositor::InterfaceName() const {
  return event_target_names::kCosmosCompositor;
}

ExecutionContext* CosmosCompositor::GetExecutionContext() const {
  return DomWindow()->document();
}

}  // namespace blink
