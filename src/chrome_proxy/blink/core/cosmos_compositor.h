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

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_COMPOSITOR_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_COMPOSITOR_H_

#include <map>

#include "third_party/blink/renderer/bindings/core/v8/script_value.h"
#include "third_party/blink/renderer/bindings/core/v8/serialization/transferables.h"
#include "third_party/blink/renderer/core/execution_context/context_lifecycle_observer.h"
#include "third_party/blink/renderer/core/dom/events/event_target.h"
#include "third_party/blink/renderer/core/event_type_names.h"

#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/assertions.h"
#include "third_party/blink/renderer/platform/wtf/forward.h"

namespace blink {

using namespace std;

class Document;
class CosmosXobj;
class ScriptState;
class ExceptionState;
class WebLocalFrameClient;
class SerializedScriptValue;

class CORE_EXPORT CosmosCompositor final : public EventTargetWithInlineData,
									  public DOMWindowClient {
  DEFINE_WRAPPERTYPEINFO();
  USING_GARBAGE_COLLECTED_MIXIN(CosmosCompositor);

 public:
  static CosmosCompositor* Create(LocalFrame* frame) { return MakeGarbageCollected<CosmosCompositor>(frame); }
  static CosmosCompositor* Create(LocalFrame* frame, const String& strHandle);

  void Trace(blink::Visitor*) override;

  // Called when an event listener has been successfully added.
  void AddedEventListener(const AtomicString& event_type,
                          RegisteredEventListener&) override;

  String name();
  String getid();

  // Message method
  void sendMessage(const String& id, const String& param1, const String& param2, const String& param3, const String& param4, const String& param5);
  

  DEFINE_ATTRIBUTE_EVENT_LISTENER(MessageReceived, kCosmoscompositor)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(XobjCreated, kXobjcreated)

  // EventTarget overrides:
  const AtomicString& InterfaceName() const override;
  ExecutionContext* GetExecutionContext() const override;

  CosmosCompositor(LocalFrame*);
  CosmosCompositor(LocalFrame*, const String& strNodeXml);

  ~CosmosCompositor() override;

  WebLocalFrameClient* m_pRenderframeImpl;
  mutable Member<CosmosXobj> innerXobj_;
private:
  String id_;
  String name_;
  String m_strXobjXml;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_COMPOSITOR_H_
