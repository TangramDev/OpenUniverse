/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202102250037           *
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 * There are Three Key Features of Webruntime:
 * 1. Built-in Modern Web Browser: Independent Browser Window and Browser Window
 *    as sub windows of other windows are supported in the application process;
 * 2. DOM Plus: DOMPlus is a natural extension of the standard DOM system.
 *    It allows the application system to support a kind of generalized web pages,
 *    which are composed of standard DOM elements and binary components supported
 *    by the application system;
 * 3. JavaScript for Application: Similar to VBA in MS office, JavaScript will
 *    become a built-in programmable language in the application system, so that
 *    the application system can be expanded and developed for the Internet based
 *    on modern javscript/Web technology.
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
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

class CORE_EXPORT CosmosControl final : public CosmosXobj{
  DEFINE_WRAPPERTYPEINFO();

 public:
  static CosmosControl* Create() { return MakeGarbageCollected<CosmosControl>(); }
  static CosmosControl* Create(const String& strHandle);

  void Trace(blink::Visitor*) override;

  int64_t handle();
  String type();
  CosmosNode* parent();
  CosmosWinform* parentForm();
  CosmosControl* parentControl();

  DEFINE_ATTRIBUTE_EVENT_LISTENER(MessageReceived, kCosmoscontrol)

  CosmosControl();
  CosmosControl(const String& strNodeXml);

  ~CosmosControl() override;

  // Binding Event:
  void BindCtrlEvents(const String& strEvents); 

  int64_t handle_ = 0;
  String type_;
  String events_;
  String webpageid_;

  mutable Member<CosmosNode> m_pParentNode;
  mutable Member<CosmosWinform> m_pParentForm;
  mutable Member<CosmosControl> m_pParentControl;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_CONTROL_H_
