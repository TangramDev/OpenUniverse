/********************************************************************************
 *           Web Runtime for Application - Version 1.0.1.202104260068           *
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

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_WINFORM_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_WINFORM_H_

#include <map>

#include "third_party/blink/renderer/bindings/core/v8/script_value.h"
#include "third_party/blink/renderer/bindings/core/v8/serialization/transferables.h"
#include "third_party/blink/renderer/core/execution_context/context_lifecycle_observer.h"
#include "third_party/blink/renderer/core/dom/events/event_target.h"
#include "third_party/blink/renderer/core/event_type_names.h"

#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/assertions.h"
#include "third_party/blink/renderer/platform/wtf/forward.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "cosmos_xobj.h"

namespace blink {

	using namespace std;

	class Cosmos;
	class DOMParser;
	class CosmosXobj;
	class CosmosNode;
	class CosmosGalaxy;
	class CosmosWinform;
	class CosmosControl;

	//for callback:
	class CallbackFunctionBase;
	class V8ApplicationCallback;

	class Document;
	class ScriptState;
	class ExceptionState;
	class DocumentFragment;
	class WebLocalFrameClient;
	class SerializedScriptValue;
	class V8ApplicationCallback;

	class CORE_EXPORT CosmosWinform final :
	public CosmosXobj{
  DEFINE_WRAPPERTYPEINFO();

 public:
  static CosmosWinform* Create() { return MakeGarbageCollected<CosmosWinform>(); }
  static CosmosWinform* Create(const String& strFormXml);

  void Trace(blink::Visitor*) override;

  int64_t handle();
  bool isReady();
  long formType();

  CosmosNode* mdibindgrid();
  CosmosWinform* mdiParent();
  CosmosWinform* activeMDIChild();
  CosmosNode* mdiwebbindgrid();
  CosmosNode* getXobj(Element* elem);
  CosmosNode* getXobj(const String& galaxyName, const String& clusterName, const String& gridName);
  CosmosGalaxy* getGalaxy(const String& galaxyName);
  void DispatchXobjEvent(Element* elem, const String& eventName);
  void ProcessFormMessage(const String& msgID);

  void InitWinForm();

	DEFINE_ATTRIBUTE_EVENT_LISTENER(MdiChildActivate, kMdichildactivate)
	DEFINE_ATTRIBUTE_EVENT_LISTENER(GalaxyCreated, kCosmosgalaxycreated)
	DEFINE_ATTRIBUTE_EVENT_LISTENER(AllMdiChildRemoved, kAllmdichildremoved)
	DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadMdiChildCloudForm, kLoadmdichildcloudform)
	DEFINE_ATTRIBUTE_EVENT_LISTENER(CloudMessageForCloudForm, kCloudmessageforcloudform)
	DEFINE_ATTRIBUTE_EVENT_LISTENER(MdiChildCloudFormReady, kMdichildcloudformready)
	DEFINE_ATTRIBUTE_EVENT_LISTENER(MdiCloudFormReady, kMdicloudformready)
	DEFINE_ATTRIBUTE_EVENT_LISTENER(CloudFormReady, kCloudformready)
	DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadCloudForm, kLoadcloudform)
	DEFINE_ATTRIBUTE_EVENT_LISTENER(CloudFormClosed, kCloudformclosed)

	  CosmosWinform();
	  CosmosWinform(const String& strNodeXml);

	  ~CosmosWinform() override;

	  bool isReady_ = false;
	  int64_t handle_ = 0;
	  int64_t m_nMdiwebbindgridhandle = 0;
	  mutable Member<Element> m_pContentElement;
	  mutable Member<CosmosNode> m_pBindMdiNode;
	  mutable Member<CosmosNode> m_pWebBindMdiNode;
	  mutable Member<CosmosWinform> m_pMDIParent;
	  mutable Member<CosmosWinform> m_pActiveMDIChild;

	  map<wstring, CosmosGalaxy*> m_mapCosmosGalaxy;
	  map<wstring, CosmosWinform*> m_mapControlBars;

	  map<int64_t, Member<CosmosControl>> m_mapChildControl;
	  map<wstring, Member<CosmosControl>> m_mapChildControl2;
	};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_WINFORM_H_
