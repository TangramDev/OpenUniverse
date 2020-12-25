/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202012250002
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 * There are Three Key Features of Webruntime:
 * 1. Built-in Modern Web Browser;
 * 2. DOM Plus: DOMPlus is a natural extension of the standard DOM system.
 *    It allows the application system to support a kind of generalized web pages,
 *    which are composed of standard DOM elements and binary components supported
 *    by the application system;
 * 3. JavaScript for Application: Similar to VBA in MS office, JavaScript will
 *    become a built-in programmable language in the application system, so that
 *    the application system can be expanded and developed for the Internet based
 *    on modern javscript/Web technology.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_CONTROL_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_CONTROL_H_

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

class Cosmos;
class CosmosXobj;
class CosmosNode;
class CosmosGalaxy;
class CosmosWinform;
class CosmosControl;

class Document;
class ScriptState;
class ExceptionState;
class V8CosmosCallback;
class V8ApplicationCallback;

class WebLocalFrameClient;
class SerializedScriptValue;

class CORE_EXPORT CosmosControl final : public EventTargetWithInlineData,
									  public DOMWindowClient {
  DEFINE_WRAPPERTYPEINFO();
  USING_GARBAGE_COLLECTED_MIXIN(CosmosControl);

 public:
  static CosmosControl* Create(LocalFrame* frame) { return MakeGarbageCollected<CosmosControl>(frame); }
  static CosmosControl* Create(LocalFrame* frame, const String& strHandle);

  void Trace(blink::Visitor*) override;

  // Called when an event listener has been successfully added.
  void AddedEventListener(const AtomicString& event_type,
                          RegisteredEventListener&) override;

  int64_t handle();
  String name();
  String type();
  String getid();
  CosmosXobj* xobj();
  CosmosNode* parent();
  CosmosWinform* parentForm();
  CosmosControl* parentControl();

  // Message method
  void sendMessage(CosmosXobj* msg, V8ApplicationCallback* callback);
  void invokeCallback(wstring callbackid, CosmosXobj* callbackParam);

  DEFINE_ATTRIBUTE_EVENT_LISTENER(MessageReceived, kCosmoscontrol)

  // EventTarget overrides:
  const AtomicString& InterfaceName() const override;
  ExecutionContext* GetExecutionContext() const override;

  CosmosControl(LocalFrame*);
  CosmosControl(LocalFrame*, const String& strNodeXml);

  ~CosmosControl() override;

  // Binding Event:
  void BindCtrlEvents(const String& strEvents); 
  void addEventListener(const String& eventName, V8ApplicationCallback* callback);
  void fireEvent(const String& eventName, CosmosXobj* eventParam);
  void removeEventListener(const String& eventName);

  int64_t handle_ = 0;
  String name_;
  String type_;
  String events_;
  String webpageid_;

  mutable Member<Cosmos> cosmos_;
  mutable Member<CosmosXobj> innerXobj_;
  mutable Member<CosmosNode> m_pParentNode;
  mutable Member<CosmosWinform> m_pParentForm;
  mutable Member<CosmosControl> m_pParentControl;
  WebLocalFrameClient* m_pRenderframeImpl;
  map<std::wstring, int> m_mapEvent;
private:
  String id_;
  HeapHashMap<String, Member<V8ApplicationCallback>> mapCosmosEventCallback_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_CONTROL_H_
