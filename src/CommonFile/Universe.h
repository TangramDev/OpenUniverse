

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for ..\CommonFile\Universe.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Universe_h__
#define __Universe_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IHubbleExtender_FWD_DEFINED__
#define __IHubbleExtender_FWD_DEFINED__
typedef interface IHubbleExtender IHubbleExtender;

#endif 	/* __IHubbleExtender_FWD_DEFINED__ */


#ifndef __IEclipseExtender_FWD_DEFINED__
#define __IEclipseExtender_FWD_DEFINED__
typedef interface IEclipseExtender IEclipseExtender;

#endif 	/* __IEclipseExtender_FWD_DEFINED__ */


#ifndef __IOfficeExtender_FWD_DEFINED__
#define __IOfficeExtender_FWD_DEFINED__
typedef interface IOfficeExtender IOfficeExtender;

#endif 	/* __IOfficeExtender_FWD_DEFINED__ */


#ifndef __IHubbleTreeNode_FWD_DEFINED__
#define __IHubbleTreeNode_FWD_DEFINED__
typedef interface IHubbleTreeNode IHubbleTreeNode;

#endif 	/* __IHubbleTreeNode_FWD_DEFINED__ */


#ifndef __IHubbleTreeViewCallBack_FWD_DEFINED__
#define __IHubbleTreeViewCallBack_FWD_DEFINED__
typedef interface IHubbleTreeViewCallBack IHubbleTreeViewCallBack;

#endif 	/* __IHubbleTreeViewCallBack_FWD_DEFINED__ */


#ifndef __IHubbleTreeView_FWD_DEFINED__
#define __IHubbleTreeView_FWD_DEFINED__
typedef interface IHubbleTreeView IHubbleTreeView;

#endif 	/* __IHubbleTreeView_FWD_DEFINED__ */


#ifndef __IHubbleEventObj_FWD_DEFINED__
#define __IHubbleEventObj_FWD_DEFINED__
typedef interface IHubbleEventObj IHubbleEventObj;

#endif 	/* __IHubbleEventObj_FWD_DEFINED__ */


#ifndef __IGrid_FWD_DEFINED__
#define __IGrid_FWD_DEFINED__
typedef interface IGrid IGrid;

#endif 	/* __IGrid_FWD_DEFINED__ */


#ifndef __IAppExtender_FWD_DEFINED__
#define __IAppExtender_FWD_DEFINED__
typedef interface IAppExtender IAppExtender;

#endif 	/* __IAppExtender_FWD_DEFINED__ */


#ifndef __IHubbleDoc_FWD_DEFINED__
#define __IHubbleDoc_FWD_DEFINED__
typedef interface IHubbleDoc IHubbleDoc;

#endif 	/* __IHubbleDoc_FWD_DEFINED__ */


#ifndef __IHubbleDocTemplate_FWD_DEFINED__
#define __IHubbleDocTemplate_FWD_DEFINED__
typedef interface IHubbleDocTemplate IHubbleDocTemplate;

#endif 	/* __IHubbleDocTemplate_FWD_DEFINED__ */


#ifndef __IHubble_FWD_DEFINED__
#define __IHubble_FWD_DEFINED__
typedef interface IHubble IHubble;

#endif 	/* __IHubble_FWD_DEFINED__ */


#ifndef __IGalaxy_FWD_DEFINED__
#define __IGalaxy_FWD_DEFINED__
typedef interface IGalaxy IGalaxy;

#endif 	/* __IGalaxy_FWD_DEFINED__ */


#ifndef __IGridCollection_FWD_DEFINED__
#define __IGridCollection_FWD_DEFINED__
typedef interface IGridCollection IGridCollection;

#endif 	/* __IGridCollection_FWD_DEFINED__ */


#ifndef __IGalaxyCluster_FWD_DEFINED__
#define __IGalaxyCluster_FWD_DEFINED__
typedef interface IGalaxyCluster IGalaxyCluster;

#endif 	/* __IGalaxyCluster_FWD_DEFINED__ */


#ifndef __IWorkBenchWindow_FWD_DEFINED__
#define __IWorkBenchWindow_FWD_DEFINED__
typedef interface IWorkBenchWindow IWorkBenchWindow;

#endif 	/* __IWorkBenchWindow_FWD_DEFINED__ */


#ifndef __IBrowser_FWD_DEFINED__
#define __IBrowser_FWD_DEFINED__
typedef interface IBrowser IBrowser;

#endif 	/* __IBrowser_FWD_DEFINED__ */


#ifndef __IWebPage_FWD_DEFINED__
#define __IWebPage_FWD_DEFINED__
typedef interface IWebPage IWebPage;

#endif 	/* __IWebPage_FWD_DEFINED__ */


#ifndef __IOfficeObject_FWD_DEFINED__
#define __IOfficeObject_FWD_DEFINED__
typedef interface IOfficeObject IOfficeObject;

#endif 	/* __IOfficeObject_FWD_DEFINED__ */


#ifndef __IHubbleCtrl_FWD_DEFINED__
#define __IHubbleCtrl_FWD_DEFINED__
typedef interface IHubbleCtrl IHubbleCtrl;

#endif 	/* __IHubbleCtrl_FWD_DEFINED__ */


#ifndef __IHubbleAppCtrl_FWD_DEFINED__
#define __IHubbleAppCtrl_FWD_DEFINED__
typedef interface IHubbleAppCtrl IHubbleAppCtrl;

#endif 	/* __IHubbleAppCtrl_FWD_DEFINED__ */


#ifndef __IEclipseCtrl_FWD_DEFINED__
#define __IEclipseCtrl_FWD_DEFINED__
typedef interface IEclipseCtrl IEclipseCtrl;

#endif 	/* __IEclipseCtrl_FWD_DEFINED__ */


#ifndef ___IHubble_FWD_DEFINED__
#define ___IHubble_FWD_DEFINED__
typedef interface _IHubble _IHubble;

#endif 	/* ___IHubble_FWD_DEFINED__ */


#ifndef __Universe_FWD_DEFINED__
#define __Universe_FWD_DEFINED__

#ifdef __cplusplus
typedef class Universe Universe;
#else
typedef struct Universe Universe;
#endif /* __cplusplus */

#endif 	/* __Universe_FWD_DEFINED__ */


#ifndef ___IGridEvents_FWD_DEFINED__
#define ___IGridEvents_FWD_DEFINED__
typedef interface _IGridEvents _IGridEvents;

#endif 	/* ___IGridEvents_FWD_DEFINED__ */


#ifndef ___IHubbleObjEvents_FWD_DEFINED__
#define ___IHubbleObjEvents_FWD_DEFINED__
typedef interface _IHubbleObjEvents _IHubbleObjEvents;

#endif 	/* ___IHubbleObjEvents_FWD_DEFINED__ */


#ifndef ___IHubbleAppEvents_FWD_DEFINED__
#define ___IHubbleAppEvents_FWD_DEFINED__
typedef interface _IHubbleAppEvents _IHubbleAppEvents;

#endif 	/* ___IHubbleAppEvents_FWD_DEFINED__ */


#ifndef __HubbleCtrl_FWD_DEFINED__
#define __HubbleCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class HubbleCtrl HubbleCtrl;
#else
typedef struct HubbleCtrl HubbleCtrl;
#endif /* __cplusplus */

#endif 	/* __HubbleCtrl_FWD_DEFINED__ */


#ifndef __UniverseExtender_FWD_DEFINED__
#define __UniverseExtender_FWD_DEFINED__

#ifdef __cplusplus
typedef class UniverseExtender UniverseExtender;
#else
typedef struct UniverseExtender UniverseExtender;
#endif /* __cplusplus */

#endif 	/* __UniverseExtender_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_Universe_0000_0000 */
/* [local] */ 













typedef /* [helpstring] */ 
enum BrowserWndOpenDisposition
    {
        UNKNOWN	= 0,
        CURRENT_TAB	= 0x1,
        SINGLETON_TAB	= 0x2,
        NEW_FOREGROUND_TAB	= 0x3,
        NEW_BACKGROUND_TAB	= 0x4,
        NEW_POPUP	= 0x5,
        NEW_WINDOW	= 0x6,
        SAVE_TO_DISK	= 0x7,
        OFF_THE_RECORD	= 0x8,
        IGNORE_ACTION	= 0x9,
        SWITCH_TO_TAB	= 0xa
    } 	BrowserWndOpenDisposition;

typedef /* [helpstring] */ 
enum GalaxyType
    {
        MDIClientGalaxy	= 0,
        MDIChildGalaxy	= 0x1,
        SDIGalaxy	= 0x2,
        CtrlBarGalaxy	= 0x3,
        WinFormMDIClientGalaxy	= 0x4,
        WinFormMDIChildGalaxy	= 0x5,
        WinFormGalaxy	= 0x6,
        EclipseWorkBenchGalaxy	= 0x7,
        EclipseViewGalaxy	= 0x8,
        EclipseSWTGalaxy	= 0x9,
        WinFormControlGalaxy	= 0xa,
        TabbedMDIClientGalaxy	= 0xb,
        NOGalaxy	= 0x10a
    } 	GalaxyType;

typedef /* [helpstring] */ 
enum ObjEventType
    {
        TangramNode	= 0,
        HubbleDocEvent	= 0x1,
        TangramNodeAllChildNode	= 0x2,
        HubbleDocAllGalaxyAllChildNode	= 0x3,
        HubbleDocAllGalaxyAllTopGrid	= 0x4,
        TangramGalaxyAllTopGridAllChildNode	= 0x5,
        TangramGalaxyAllTopGrid	= 0x6,
        GalaxyClusterAllGalaxyAllTopGridAllChildNode	= 0x7,
        GalaxyClusterAllGalaxyAllTopGrid	= 0x8,
        GalaxyClusterCtrlBarGalaxyAllTopGridAllChildNode	= 0x9,
        GalaxyClusterCtrlBarGalaxyAllTopGrid	= 0xa,
        GalaxyClusterNotCtrlBarGalaxyAllTopGridAllChildNode	= 0xb,
        GalaxyClusterNotCtrlBarGalaxyAllTopGrid	= 0xc,
        HubbleDocAllCtrlBarGalaxyAllChildNode	= 0xd,
        HubbleDocAllCtrlBarGalaxy	= 0xe
    } 	ObjEventType;

typedef /* [helpstring] */ 
enum GridType
    {
        BlankView	= 0x1,
        ActiveX	= 0x2,
        Grid	= 0x4,
        TabGrid	= 0x8,
        CLRCtrl	= 0x10,
        CLRForm	= 0x20,
        CLRWnd	= 0x40,
        TangramView	= 0x80,
        TangramTreeView	= 0x100,
        TangramListView	= 0x200,
        TabCtrl	= 0x400,
        TangramWPFCtrl	= 0x800
    } 	GridType;



extern RPC_IF_HANDLE __MIDL_itf_Universe_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Universe_0000_0000_v0_0_s_ifspec;

#ifndef __IHubbleExtender_INTERFACE_DEFINED__
#define __IHubbleExtender_INTERFACE_DEFINED__

/* interface IHubbleExtender */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHubbleExtender;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060120160101")
    IHubbleExtender : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveWorkBenchWindow( 
            BSTR bstrID,
            /* [retval][out] */ IWorkBenchWindow **pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IHubbleExtenderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHubbleExtender * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHubbleExtender * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHubbleExtender * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHubbleExtender * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHubbleExtender * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHubbleExtender * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHubbleExtender * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IHubbleExtender * This);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveWorkBenchWindow )( 
            IHubbleExtender * This,
            BSTR bstrID,
            /* [retval][out] */ IWorkBenchWindow **pVal);
        
        END_INTERFACE
    } IHubbleExtenderVtbl;

    interface IHubbleExtender
    {
        CONST_VTBL struct IHubbleExtenderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHubbleExtender_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHubbleExtender_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHubbleExtender_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHubbleExtender_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IHubbleExtender_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IHubbleExtender_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IHubbleExtender_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IHubbleExtender_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#define IHubbleExtender_get_ActiveWorkBenchWindow(This,bstrID,pVal)	\
    ( (This)->lpVtbl -> get_ActiveWorkBenchWindow(This,bstrID,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHubbleExtender_INTERFACE_DEFINED__ */


#ifndef __IEclipseExtender_INTERFACE_DEFINED__
#define __IEclipseExtender_INTERFACE_DEFINED__

/* interface IEclipseExtender */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IEclipseExtender;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060120160918")
    IEclipseExtender : public IHubbleExtender
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct IEclipseExtenderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEclipseExtender * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEclipseExtender * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEclipseExtender * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEclipseExtender * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEclipseExtender * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEclipseExtender * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEclipseExtender * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IEclipseExtender * This);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveWorkBenchWindow )( 
            IEclipseExtender * This,
            BSTR bstrID,
            /* [retval][out] */ IWorkBenchWindow **pVal);
        
        END_INTERFACE
    } IEclipseExtenderVtbl;

    interface IEclipseExtender
    {
        CONST_VTBL struct IEclipseExtenderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEclipseExtender_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEclipseExtender_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEclipseExtender_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEclipseExtender_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IEclipseExtender_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IEclipseExtender_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IEclipseExtender_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IEclipseExtender_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#define IEclipseExtender_get_ActiveWorkBenchWindow(This,bstrID,pVal)	\
    ( (This)->lpVtbl -> get_ActiveWorkBenchWindow(This,bstrID,pVal) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEclipseExtender_INTERFACE_DEFINED__ */


#ifndef __IOfficeExtender_INTERFACE_DEFINED__
#define __IOfficeExtender_INTERFACE_DEFINED__

/* interface IOfficeExtender */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IOfficeExtender;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060119631222")
    IOfficeExtender : public IHubbleExtender
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddVBAFormsScript( 
            IDispatch *OfficeObject,
            BSTR bstrKey,
            BSTR bstrXml) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE InitVBAForm( 
            /* [in] */ IDispatch *newVal,
            /* [in] */ long nStyle,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGalaxyFromVBAForm( 
            IDispatch *pForm,
            /* [retval][out] */ IGalaxy **ppGalaxy) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetActiveTopGrid( 
            IDispatch *pForm,
            /* [retval][out] */ IGrid **WndGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetObjectFromWnd( 
            LONG hWnd,
            /* [retval][out] */ IDispatch **ppObjFromWnd) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IOfficeExtenderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOfficeExtender * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOfficeExtender * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOfficeExtender * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IOfficeExtender * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IOfficeExtender * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IOfficeExtender * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IOfficeExtender * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IOfficeExtender * This);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveWorkBenchWindow )( 
            IOfficeExtender * This,
            BSTR bstrID,
            /* [retval][out] */ IWorkBenchWindow **pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddVBAFormsScript )( 
            IOfficeExtender * This,
            IDispatch *OfficeObject,
            BSTR bstrKey,
            BSTR bstrXml);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *InitVBAForm )( 
            IOfficeExtender * This,
            /* [in] */ IDispatch *newVal,
            /* [in] */ long nStyle,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGalaxyFromVBAForm )( 
            IOfficeExtender * This,
            IDispatch *pForm,
            /* [retval][out] */ IGalaxy **ppGalaxy);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetActiveTopGrid )( 
            IOfficeExtender * This,
            IDispatch *pForm,
            /* [retval][out] */ IGrid **WndGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetObjectFromWnd )( 
            IOfficeExtender * This,
            LONG hWnd,
            /* [retval][out] */ IDispatch **ppObjFromWnd);
        
        END_INTERFACE
    } IOfficeExtenderVtbl;

    interface IOfficeExtender
    {
        CONST_VTBL struct IOfficeExtenderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOfficeExtender_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOfficeExtender_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOfficeExtender_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOfficeExtender_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IOfficeExtender_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IOfficeExtender_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IOfficeExtender_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IOfficeExtender_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#define IOfficeExtender_get_ActiveWorkBenchWindow(This,bstrID,pVal)	\
    ( (This)->lpVtbl -> get_ActiveWorkBenchWindow(This,bstrID,pVal) ) 


#define IOfficeExtender_AddVBAFormsScript(This,OfficeObject,bstrKey,bstrXml)	\
    ( (This)->lpVtbl -> AddVBAFormsScript(This,OfficeObject,bstrKey,bstrXml) ) 

#define IOfficeExtender_InitVBAForm(This,newVal,nStyle,bstrXml,ppGrid)	\
    ( (This)->lpVtbl -> InitVBAForm(This,newVal,nStyle,bstrXml,ppGrid) ) 

#define IOfficeExtender_GetGalaxyFromVBAForm(This,pForm,ppGalaxy)	\
    ( (This)->lpVtbl -> GetGalaxyFromVBAForm(This,pForm,ppGalaxy) ) 

#define IOfficeExtender_GetActiveTopGrid(This,pForm,WndGrid)	\
    ( (This)->lpVtbl -> GetActiveTopGrid(This,pForm,WndGrid) ) 

#define IOfficeExtender_GetObjectFromWnd(This,hWnd,ppObjFromWnd)	\
    ( (This)->lpVtbl -> GetObjectFromWnd(This,hWnd,ppObjFromWnd) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOfficeExtender_INTERFACE_DEFINED__ */


#ifndef __IHubbleTreeNode_INTERFACE_DEFINED__
#define __IHubbleTreeNode_INTERFACE_DEFINED__

/* interface IHubbleTreeNode */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHubbleTreeNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-06011982C951")
    IHubbleTreeNode : public IDispatch
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct IHubbleTreeNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHubbleTreeNode * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHubbleTreeNode * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHubbleTreeNode * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHubbleTreeNode * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHubbleTreeNode * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHubbleTreeNode * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHubbleTreeNode * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IHubbleTreeNodeVtbl;

    interface IHubbleTreeNode
    {
        CONST_VTBL struct IHubbleTreeNodeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHubbleTreeNode_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHubbleTreeNode_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHubbleTreeNode_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHubbleTreeNode_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IHubbleTreeNode_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IHubbleTreeNode_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IHubbleTreeNode_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHubbleTreeNode_INTERFACE_DEFINED__ */


#ifndef __IHubbleTreeViewCallBack_INTERFACE_DEFINED__
#define __IHubbleTreeViewCallBack_INTERFACE_DEFINED__

/* interface IHubbleTreeViewCallBack */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHubbleTreeViewCallBack;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-0601198283A6")
    IHubbleTreeViewCallBack : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_WndGrid( 
            IGrid *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Pages( 
            long *retVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnInitTreeView( 
            IHubbleTreeView *pTangramTreeView,
            BSTR bstrXml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnClick( 
            BSTR bstrXml,
            BSTR bstrXmlData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnInit( 
            BSTR bstrXml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnNewPage( 
            int nNewPage) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE HubbleAction( 
            BSTR bstrXml,
            /* [retval][out] */ BSTR *bstrRetXml) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IHubbleTreeViewCallBackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHubbleTreeViewCallBack * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHubbleTreeViewCallBack * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHubbleTreeViewCallBack * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHubbleTreeViewCallBack * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHubbleTreeViewCallBack * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHubbleTreeViewCallBack * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHubbleTreeViewCallBack * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_WndGrid )( 
            IHubbleTreeViewCallBack * This,
            IGrid *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Pages )( 
            IHubbleTreeViewCallBack * This,
            long *retVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnInitTreeView )( 
            IHubbleTreeViewCallBack * This,
            IHubbleTreeView *pTangramTreeView,
            BSTR bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnClick )( 
            IHubbleTreeViewCallBack * This,
            BSTR bstrXml,
            BSTR bstrXmlData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnInit )( 
            IHubbleTreeViewCallBack * This,
            BSTR bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnNewPage )( 
            IHubbleTreeViewCallBack * This,
            int nNewPage);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *HubbleAction )( 
            IHubbleTreeViewCallBack * This,
            BSTR bstrXml,
            /* [retval][out] */ BSTR *bstrRetXml);
        
        END_INTERFACE
    } IHubbleTreeViewCallBackVtbl;

    interface IHubbleTreeViewCallBack
    {
        CONST_VTBL struct IHubbleTreeViewCallBackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHubbleTreeViewCallBack_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHubbleTreeViewCallBack_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHubbleTreeViewCallBack_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHubbleTreeViewCallBack_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IHubbleTreeViewCallBack_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IHubbleTreeViewCallBack_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IHubbleTreeViewCallBack_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IHubbleTreeViewCallBack_put_WndGrid(This,newVal)	\
    ( (This)->lpVtbl -> put_WndGrid(This,newVal) ) 

#define IHubbleTreeViewCallBack_get_Pages(This,retVal)	\
    ( (This)->lpVtbl -> get_Pages(This,retVal) ) 

#define IHubbleTreeViewCallBack_OnInitTreeView(This,pTangramTreeView,bstrXml)	\
    ( (This)->lpVtbl -> OnInitTreeView(This,pTangramTreeView,bstrXml) ) 

#define IHubbleTreeViewCallBack_OnClick(This,bstrXml,bstrXmlData)	\
    ( (This)->lpVtbl -> OnClick(This,bstrXml,bstrXmlData) ) 

#define IHubbleTreeViewCallBack_OnInit(This,bstrXml)	\
    ( (This)->lpVtbl -> OnInit(This,bstrXml) ) 

#define IHubbleTreeViewCallBack_OnNewPage(This,nNewPage)	\
    ( (This)->lpVtbl -> OnNewPage(This,nNewPage) ) 

#define IHubbleTreeViewCallBack_HubbleAction(This,bstrXml,bstrRetXml)	\
    ( (This)->lpVtbl -> HubbleAction(This,bstrXml,bstrRetXml) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHubbleTreeViewCallBack_INTERFACE_DEFINED__ */


#ifndef __IHubbleTreeView_INTERFACE_DEFINED__
#define __IHubbleTreeView_INTERFACE_DEFINED__

/* interface IHubbleTreeView */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHubbleTreeView;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-0601198283A5")
    IHubbleTreeView : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TangramTreeViewCallBack( 
            BSTR bstrKey,
            /* [in] */ IHubbleTreeViewCallBack *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FirstRoot( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddTreeNode( 
            long hItem,
            BSTR bstrXml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InsertNode( 
            BSTR bstrXml,
            /* [retval][out] */ long *hItem) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IHubbleTreeViewVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHubbleTreeView * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHubbleTreeView * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHubbleTreeView * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHubbleTreeView * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHubbleTreeView * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHubbleTreeView * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHubbleTreeView * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TangramTreeViewCallBack )( 
            IHubbleTreeView * This,
            BSTR bstrKey,
            /* [in] */ IHubbleTreeViewCallBack *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FirstRoot )( 
            IHubbleTreeView * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddTreeNode )( 
            IHubbleTreeView * This,
            long hItem,
            BSTR bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InsertNode )( 
            IHubbleTreeView * This,
            BSTR bstrXml,
            /* [retval][out] */ long *hItem);
        
        END_INTERFACE
    } IHubbleTreeViewVtbl;

    interface IHubbleTreeView
    {
        CONST_VTBL struct IHubbleTreeViewVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHubbleTreeView_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHubbleTreeView_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHubbleTreeView_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHubbleTreeView_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IHubbleTreeView_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IHubbleTreeView_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IHubbleTreeView_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IHubbleTreeView_put_TangramTreeViewCallBack(This,bstrKey,newVal)	\
    ( (This)->lpVtbl -> put_TangramTreeViewCallBack(This,bstrKey,newVal) ) 

#define IHubbleTreeView_get_FirstRoot(This,pVal)	\
    ( (This)->lpVtbl -> get_FirstRoot(This,pVal) ) 

#define IHubbleTreeView_AddTreeNode(This,hItem,bstrXml)	\
    ( (This)->lpVtbl -> AddTreeNode(This,hItem,bstrXml) ) 

#define IHubbleTreeView_InsertNode(This,bstrXml,hItem)	\
    ( (This)->lpVtbl -> InsertNode(This,bstrXml,hItem) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHubbleTreeView_INTERFACE_DEFINED__ */


#ifndef __IHubbleEventObj_INTERFACE_DEFINED__
#define __IHubbleEventObj_INTERFACE_DEFINED__

/* interface IHubbleEventObj */
/* [unique][helpstring][nonextensible][hidden][dual][uuid][object] */ 


EXTERN_C const IID IID_IHubbleEventObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060120160928")
    IHubbleEventObj : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_eventSource( 
            /* [retval][out] */ IDispatch **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_eventSource( 
            /* [in] */ IDispatch *eventSource) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Index( 
            /* [retval][out] */ int *nVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Index( 
            int newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_EventName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_EventName( 
            BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Object( 
            int nIndex,
            /* [retval][out] */ IDispatch **pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Object( 
            int nIndex,
            IDispatch *newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            int nIndex,
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            int nIndex,
            VARIANT newVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IHubbleEventObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHubbleEventObj * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHubbleEventObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHubbleEventObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHubbleEventObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHubbleEventObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHubbleEventObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHubbleEventObj * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_eventSource )( 
            IHubbleEventObj * This,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_eventSource )( 
            IHubbleEventObj * This,
            /* [in] */ IDispatch *eventSource);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Index )( 
            IHubbleEventObj * This,
            /* [retval][out] */ int *nVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Index )( 
            IHubbleEventObj * This,
            int newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_EventName )( 
            IHubbleEventObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_EventName )( 
            IHubbleEventObj * This,
            BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Object )( 
            IHubbleEventObj * This,
            int nIndex,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Object )( 
            IHubbleEventObj * This,
            int nIndex,
            IDispatch *newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            IHubbleEventObj * This,
            int nIndex,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Value )( 
            IHubbleEventObj * This,
            int nIndex,
            VARIANT newVal);
        
        END_INTERFACE
    } IHubbleEventObjVtbl;

    interface IHubbleEventObj
    {
        CONST_VTBL struct IHubbleEventObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHubbleEventObj_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHubbleEventObj_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHubbleEventObj_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHubbleEventObj_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IHubbleEventObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IHubbleEventObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IHubbleEventObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IHubbleEventObj_get_eventSource(This,pVal)	\
    ( (This)->lpVtbl -> get_eventSource(This,pVal) ) 

#define IHubbleEventObj_put_eventSource(This,eventSource)	\
    ( (This)->lpVtbl -> put_eventSource(This,eventSource) ) 

#define IHubbleEventObj_get_Index(This,nVal)	\
    ( (This)->lpVtbl -> get_Index(This,nVal) ) 

#define IHubbleEventObj_put_Index(This,newVal)	\
    ( (This)->lpVtbl -> put_Index(This,newVal) ) 

#define IHubbleEventObj_get_EventName(This,pVal)	\
    ( (This)->lpVtbl -> get_EventName(This,pVal) ) 

#define IHubbleEventObj_put_EventName(This,newVal)	\
    ( (This)->lpVtbl -> put_EventName(This,newVal) ) 

#define IHubbleEventObj_get_Object(This,nIndex,pVal)	\
    ( (This)->lpVtbl -> get_Object(This,nIndex,pVal) ) 

#define IHubbleEventObj_put_Object(This,nIndex,newVal)	\
    ( (This)->lpVtbl -> put_Object(This,nIndex,newVal) ) 

#define IHubbleEventObj_get_Value(This,nIndex,pVal)	\
    ( (This)->lpVtbl -> get_Value(This,nIndex,pVal) ) 

#define IHubbleEventObj_put_Value(This,nIndex,newVal)	\
    ( (This)->lpVtbl -> put_Value(This,nIndex,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHubbleEventObj_INTERFACE_DEFINED__ */


#ifndef __IGrid_INTERFACE_DEFINED__
#define __IGrid_INTERFACE_DEFINED__

/* interface IGrid */
/* [object][unique][helpstring][uuid] */ 


EXTERN_C const IID IID_IGrid;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060119820004")
    IGrid : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ChildNodes( 
            /* [retval][out] */ IGridCollection **ppGridColletion) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Rows( 
            /* [retval][out] */ long *nRows) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Cols( 
            /* [retval][out] */ long *nCols) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Row( 
            /* [retval][out] */ long *nRow) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Col( 
            /* [retval][out] */ long *nCol) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GridType( 
            /* [retval][out] */ GridType *nType) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ParentGrid( 
            /* [retval][out] */ IGrid **ppGrid) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_URL( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_URL( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_GalaxyCluster( 
            /* [retval][out] */ IGalaxyCluster **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RootGrid( 
            /* [retval][out] */ IGrid **ppGrid) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_XObject( 
            /* [retval][out] */ VARIANT *pVar) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AxPlugIn( 
            /* [in] */ BSTR bstrPlugInName,
            /* [retval][out] */ IDispatch **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Caption( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Caption( 
            /* [in] */ BSTR bstrCaption) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Objects( 
            /* [in] */ long nType,
            /* [retval][out] */ IGridCollection **ppGridColletion) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Handle( 
            /* [retval][out] */ LONGLONG *hWnd) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Attribute( 
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Attribute( 
            /* [in] */ BSTR bstrKey,
            /* [in] */ BSTR bstrVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Tag( 
            /* [retval][out] */ VARIANT *pVar) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Tag( 
            /* [in] */ VARIANT vVar) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_OuterXml( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Key( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DockObj( 
            BSTR bstrName,
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DockObj( 
            BSTR bstrName,
            /* [in] */ LONGLONG newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_NameAtWindowPage( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Galaxy( 
            /* [retval][out] */ IGalaxy **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_XML( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Extender( 
            /* [retval][out] */ IDispatch **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Extender( 
            /* [in] */ IDispatch *newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DocXml( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_rgbMiddle( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_rgbMiddle( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_rgbRightBottom( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_rgbRightBottom( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_rgbLeftTop( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_rgbLeftTop( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Hmin( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Hmin( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Hmax( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Hmax( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Vmin( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Vmin( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Vmax( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Vmax( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HostGrid( 
            /* [retval][out] */ IGrid **pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_HostGrid( 
            /* [in] */ IGrid *newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ActivePage( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ActivePage( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HostGalaxy( 
            /* [retval][out] */ IGalaxy **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_MasterRow( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_MasterRow( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_MasterCol( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_MasterCol( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_OfficeObj( 
            /* [retval][out] */ IDispatch **pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_SaveToConfigFile( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_WebPage( 
            /* [retval][out] */ IWebPage **pVal) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE ActiveTabPage( 
            IGrid *pGrid) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetGrids( 
            /* [in] */ BSTR bstrName,
            /* [out] */ IGrid **ppGrid,
            /* [out] */ IGridCollection **ppGrids,
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetGrid( 
            /* [in] */ long nRow,
            /* [in] */ long nCol,
            /* [retval][out] */ IGrid **ppGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCtrlByName( 
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            /* [retval][out] */ IDispatch **ppCtrlDisp) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendIPCMessage( 
            BSTR bstrTo,
            BSTR bstrPayload,
            BSTR bstrExtra,
            BSTR bstrMsgId,
            /* [retval][out] */ BSTR *bstrRes) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Observe( 
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppRetGrid) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadXML( 
            int nType,
            BSTR bstrXML) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ObserveEx( 
            int nRow,
            int nCol,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppRetGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGridByName( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IGridCollection **pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCtrlValueByName( 
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            /* [retval][out] */ BSTR *bstrVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetCtrlValueByName( 
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            BSTR bstrVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE NavigateURL( 
            BSTR bstrURL,
            IDispatch *dispObjforScript) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetChildGridByName( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IGrid **pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IGridVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGrid * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGrid * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGrid * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IGrid * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IGrid * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IGrid * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IGrid * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ChildNodes )( 
            IGrid * This,
            /* [retval][out] */ IGridCollection **ppGridColletion);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Rows )( 
            IGrid * This,
            /* [retval][out] */ long *nRows);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Cols )( 
            IGrid * This,
            /* [retval][out] */ long *nCols);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Row )( 
            IGrid * This,
            /* [retval][out] */ long *nRow);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Col )( 
            IGrid * This,
            /* [retval][out] */ long *nCol);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GridType )( 
            IGrid * This,
            /* [retval][out] */ GridType *nType);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ParentGrid )( 
            IGrid * This,
            /* [retval][out] */ IGrid **ppGrid);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_URL )( 
            IGrid * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_URL )( 
            IGrid * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GalaxyCluster )( 
            IGrid * This,
            /* [retval][out] */ IGalaxyCluster **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RootGrid )( 
            IGrid * This,
            /* [retval][out] */ IGrid **ppGrid);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_XObject )( 
            IGrid * This,
            /* [retval][out] */ VARIANT *pVar);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AxPlugIn )( 
            IGrid * This,
            /* [in] */ BSTR bstrPlugInName,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Caption )( 
            IGrid * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Caption )( 
            IGrid * This,
            /* [in] */ BSTR bstrCaption);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IGrid * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            IGrid * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Objects )( 
            IGrid * This,
            /* [in] */ long nType,
            /* [retval][out] */ IGridCollection **ppGridColletion);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Handle )( 
            IGrid * This,
            /* [retval][out] */ LONGLONG *hWnd);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Attribute )( 
            IGrid * This,
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Attribute )( 
            IGrid * This,
            /* [in] */ BSTR bstrKey,
            /* [in] */ BSTR bstrVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Tag )( 
            IGrid * This,
            /* [retval][out] */ VARIANT *pVar);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Tag )( 
            IGrid * This,
            /* [in] */ VARIANT vVar);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_OuterXml )( 
            IGrid * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Key )( 
            IGrid * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DockObj )( 
            IGrid * This,
            BSTR bstrName,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DockObj )( 
            IGrid * This,
            BSTR bstrName,
            /* [in] */ LONGLONG newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NameAtWindowPage )( 
            IGrid * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IGrid * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IGrid * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Galaxy )( 
            IGrid * This,
            /* [retval][out] */ IGalaxy **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_XML )( 
            IGrid * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Extender )( 
            IGrid * This,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Extender )( 
            IGrid * This,
            /* [in] */ IDispatch *newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DocXml )( 
            IGrid * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_rgbMiddle )( 
            IGrid * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_rgbMiddle )( 
            IGrid * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_rgbRightBottom )( 
            IGrid * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_rgbRightBottom )( 
            IGrid * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_rgbLeftTop )( 
            IGrid * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_rgbLeftTop )( 
            IGrid * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Hmin )( 
            IGrid * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Hmin )( 
            IGrid * This,
            /* [in] */ int newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Hmax )( 
            IGrid * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Hmax )( 
            IGrid * This,
            /* [in] */ int newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Vmin )( 
            IGrid * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Vmin )( 
            IGrid * This,
            /* [in] */ int newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Vmax )( 
            IGrid * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Vmax )( 
            IGrid * This,
            /* [in] */ int newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HostGrid )( 
            IGrid * This,
            /* [retval][out] */ IGrid **pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HostGrid )( 
            IGrid * This,
            /* [in] */ IGrid *newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActivePage )( 
            IGrid * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ActivePage )( 
            IGrid * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HostGalaxy )( 
            IGrid * This,
            /* [retval][out] */ IGalaxy **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MasterRow )( 
            IGrid * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MasterRow )( 
            IGrid * This,
            /* [in] */ int newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MasterCol )( 
            IGrid * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MasterCol )( 
            IGrid * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_OfficeObj )( 
            IGrid * This,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SaveToConfigFile )( 
            IGrid * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WebPage )( 
            IGrid * This,
            /* [retval][out] */ IWebPage **pVal);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *ActiveTabPage )( 
            IGrid * This,
            IGrid *pGrid);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetGrids )( 
            IGrid * This,
            /* [in] */ BSTR bstrName,
            /* [out] */ IGrid **ppGrid,
            /* [out] */ IGridCollection **ppGrids,
            /* [retval][out] */ long *pCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetGrid )( 
            IGrid * This,
            /* [in] */ long nRow,
            /* [in] */ long nCol,
            /* [retval][out] */ IGrid **ppGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCtrlByName )( 
            IGrid * This,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            /* [retval][out] */ IDispatch **ppCtrlDisp);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SendIPCMessage )( 
            IGrid * This,
            BSTR bstrTo,
            BSTR bstrPayload,
            BSTR bstrExtra,
            BSTR bstrMsgId,
            /* [retval][out] */ BSTR *bstrRes);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Observe )( 
            IGrid * This,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppRetGrid);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadXML )( 
            IGrid * This,
            int nType,
            BSTR bstrXML);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ObserveEx )( 
            IGrid * This,
            int nRow,
            int nCol,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppRetGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGridByName )( 
            IGrid * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IGridCollection **pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCtrlValueByName )( 
            IGrid * This,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            /* [retval][out] */ BSTR *bstrVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetCtrlValueByName )( 
            IGrid * This,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            BSTR bstrVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *NavigateURL )( 
            IGrid * This,
            BSTR bstrURL,
            IDispatch *dispObjforScript);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetChildGridByName )( 
            IGrid * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IGrid **pVal);
        
        END_INTERFACE
    } IGridVtbl;

    interface IGrid
    {
        CONST_VTBL struct IGridVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGrid_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGrid_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGrid_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGrid_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IGrid_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IGrid_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IGrid_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IGrid_get_ChildNodes(This,ppGridColletion)	\
    ( (This)->lpVtbl -> get_ChildNodes(This,ppGridColletion) ) 

#define IGrid_get_Rows(This,nRows)	\
    ( (This)->lpVtbl -> get_Rows(This,nRows) ) 

#define IGrid_get_Cols(This,nCols)	\
    ( (This)->lpVtbl -> get_Cols(This,nCols) ) 

#define IGrid_get_Row(This,nRow)	\
    ( (This)->lpVtbl -> get_Row(This,nRow) ) 

#define IGrid_get_Col(This,nCol)	\
    ( (This)->lpVtbl -> get_Col(This,nCol) ) 

#define IGrid_get_GridType(This,nType)	\
    ( (This)->lpVtbl -> get_GridType(This,nType) ) 

#define IGrid_get_ParentGrid(This,ppGrid)	\
    ( (This)->lpVtbl -> get_ParentGrid(This,ppGrid) ) 

#define IGrid_get_URL(This,pVal)	\
    ( (This)->lpVtbl -> get_URL(This,pVal) ) 

#define IGrid_put_URL(This,newVal)	\
    ( (This)->lpVtbl -> put_URL(This,newVal) ) 

#define IGrid_get_GalaxyCluster(This,pVal)	\
    ( (This)->lpVtbl -> get_GalaxyCluster(This,pVal) ) 

#define IGrid_get_RootGrid(This,ppGrid)	\
    ( (This)->lpVtbl -> get_RootGrid(This,ppGrid) ) 

#define IGrid_get_XObject(This,pVar)	\
    ( (This)->lpVtbl -> get_XObject(This,pVar) ) 

#define IGrid_get_AxPlugIn(This,bstrPlugInName,pVal)	\
    ( (This)->lpVtbl -> get_AxPlugIn(This,bstrPlugInName,pVal) ) 

#define IGrid_get_Caption(This,pVal)	\
    ( (This)->lpVtbl -> get_Caption(This,pVal) ) 

#define IGrid_put_Caption(This,bstrCaption)	\
    ( (This)->lpVtbl -> put_Caption(This,bstrCaption) ) 

#define IGrid_get_Name(This,pVal)	\
    ( (This)->lpVtbl -> get_Name(This,pVal) ) 

#define IGrid_put_Name(This,bstrName)	\
    ( (This)->lpVtbl -> put_Name(This,bstrName) ) 

#define IGrid_get_Objects(This,nType,ppGridColletion)	\
    ( (This)->lpVtbl -> get_Objects(This,nType,ppGridColletion) ) 

#define IGrid_get_Handle(This,hWnd)	\
    ( (This)->lpVtbl -> get_Handle(This,hWnd) ) 

#define IGrid_get_Attribute(This,bstrKey,pVal)	\
    ( (This)->lpVtbl -> get_Attribute(This,bstrKey,pVal) ) 

#define IGrid_put_Attribute(This,bstrKey,bstrVal)	\
    ( (This)->lpVtbl -> put_Attribute(This,bstrKey,bstrVal) ) 

#define IGrid_get_Tag(This,pVar)	\
    ( (This)->lpVtbl -> get_Tag(This,pVar) ) 

#define IGrid_put_Tag(This,vVar)	\
    ( (This)->lpVtbl -> put_Tag(This,vVar) ) 

#define IGrid_get_OuterXml(This,pVal)	\
    ( (This)->lpVtbl -> get_OuterXml(This,pVal) ) 

#define IGrid_get_Key(This,pVal)	\
    ( (This)->lpVtbl -> get_Key(This,pVal) ) 

#define IGrid_get_DockObj(This,bstrName,pVal)	\
    ( (This)->lpVtbl -> get_DockObj(This,bstrName,pVal) ) 

#define IGrid_put_DockObj(This,bstrName,newVal)	\
    ( (This)->lpVtbl -> put_DockObj(This,bstrName,newVal) ) 

#define IGrid_get_NameAtWindowPage(This,pVal)	\
    ( (This)->lpVtbl -> get_NameAtWindowPage(This,pVal) ) 

#define IGrid_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IGrid_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IGrid_get_Galaxy(This,pVal)	\
    ( (This)->lpVtbl -> get_Galaxy(This,pVal) ) 

#define IGrid_get_XML(This,pVal)	\
    ( (This)->lpVtbl -> get_XML(This,pVal) ) 

#define IGrid_get_Extender(This,pVal)	\
    ( (This)->lpVtbl -> get_Extender(This,pVal) ) 

#define IGrid_put_Extender(This,newVal)	\
    ( (This)->lpVtbl -> put_Extender(This,newVal) ) 

#define IGrid_get_DocXml(This,pVal)	\
    ( (This)->lpVtbl -> get_DocXml(This,pVal) ) 

#define IGrid_get_rgbMiddle(This,pVal)	\
    ( (This)->lpVtbl -> get_rgbMiddle(This,pVal) ) 

#define IGrid_put_rgbMiddle(This,newVal)	\
    ( (This)->lpVtbl -> put_rgbMiddle(This,newVal) ) 

#define IGrid_get_rgbRightBottom(This,pVal)	\
    ( (This)->lpVtbl -> get_rgbRightBottom(This,pVal) ) 

#define IGrid_put_rgbRightBottom(This,newVal)	\
    ( (This)->lpVtbl -> put_rgbRightBottom(This,newVal) ) 

#define IGrid_get_rgbLeftTop(This,pVal)	\
    ( (This)->lpVtbl -> get_rgbLeftTop(This,pVal) ) 

#define IGrid_put_rgbLeftTop(This,newVal)	\
    ( (This)->lpVtbl -> put_rgbLeftTop(This,newVal) ) 

#define IGrid_get_Hmin(This,pVal)	\
    ( (This)->lpVtbl -> get_Hmin(This,pVal) ) 

#define IGrid_put_Hmin(This,newVal)	\
    ( (This)->lpVtbl -> put_Hmin(This,newVal) ) 

#define IGrid_get_Hmax(This,pVal)	\
    ( (This)->lpVtbl -> get_Hmax(This,pVal) ) 

#define IGrid_put_Hmax(This,newVal)	\
    ( (This)->lpVtbl -> put_Hmax(This,newVal) ) 

#define IGrid_get_Vmin(This,pVal)	\
    ( (This)->lpVtbl -> get_Vmin(This,pVal) ) 

#define IGrid_put_Vmin(This,newVal)	\
    ( (This)->lpVtbl -> put_Vmin(This,newVal) ) 

#define IGrid_get_Vmax(This,pVal)	\
    ( (This)->lpVtbl -> get_Vmax(This,pVal) ) 

#define IGrid_put_Vmax(This,newVal)	\
    ( (This)->lpVtbl -> put_Vmax(This,newVal) ) 

#define IGrid_get_HostGrid(This,pVal)	\
    ( (This)->lpVtbl -> get_HostGrid(This,pVal) ) 

#define IGrid_put_HostGrid(This,newVal)	\
    ( (This)->lpVtbl -> put_HostGrid(This,newVal) ) 

#define IGrid_get_ActivePage(This,pVal)	\
    ( (This)->lpVtbl -> get_ActivePage(This,pVal) ) 

#define IGrid_put_ActivePage(This,newVal)	\
    ( (This)->lpVtbl -> put_ActivePage(This,newVal) ) 

#define IGrid_get_HostGalaxy(This,pVal)	\
    ( (This)->lpVtbl -> get_HostGalaxy(This,pVal) ) 

#define IGrid_get_MasterRow(This,pVal)	\
    ( (This)->lpVtbl -> get_MasterRow(This,pVal) ) 

#define IGrid_put_MasterRow(This,newVal)	\
    ( (This)->lpVtbl -> put_MasterRow(This,newVal) ) 

#define IGrid_get_MasterCol(This,pVal)	\
    ( (This)->lpVtbl -> get_MasterCol(This,pVal) ) 

#define IGrid_put_MasterCol(This,newVal)	\
    ( (This)->lpVtbl -> put_MasterCol(This,newVal) ) 

#define IGrid_get_OfficeObj(This,pVal)	\
    ( (This)->lpVtbl -> get_OfficeObj(This,pVal) ) 

#define IGrid_put_SaveToConfigFile(This,newVal)	\
    ( (This)->lpVtbl -> put_SaveToConfigFile(This,newVal) ) 

#define IGrid_get_WebPage(This,pVal)	\
    ( (This)->lpVtbl -> get_WebPage(This,pVal) ) 

#define IGrid_ActiveTabPage(This,pGrid)	\
    ( (This)->lpVtbl -> ActiveTabPage(This,pGrid) ) 

#define IGrid_GetGrids(This,bstrName,ppGrid,ppGrids,pCount)	\
    ( (This)->lpVtbl -> GetGrids(This,bstrName,ppGrid,ppGrids,pCount) ) 

#define IGrid_GetGrid(This,nRow,nCol,ppGrid)	\
    ( (This)->lpVtbl -> GetGrid(This,nRow,nCol,ppGrid) ) 

#define IGrid_GetCtrlByName(This,bstrName,bFindInChild,ppCtrlDisp)	\
    ( (This)->lpVtbl -> GetCtrlByName(This,bstrName,bFindInChild,ppCtrlDisp) ) 

#define IGrid_SendIPCMessage(This,bstrTo,bstrPayload,bstrExtra,bstrMsgId,bstrRes)	\
    ( (This)->lpVtbl -> SendIPCMessage(This,bstrTo,bstrPayload,bstrExtra,bstrMsgId,bstrRes) ) 

#define IGrid_Observe(This,bstrKey,bstrXml,ppRetGrid)	\
    ( (This)->lpVtbl -> Observe(This,bstrKey,bstrXml,ppRetGrid) ) 

#define IGrid_LoadXML(This,nType,bstrXML)	\
    ( (This)->lpVtbl -> LoadXML(This,nType,bstrXML) ) 

#define IGrid_ObserveEx(This,nRow,nCol,bstrKey,bstrXml,ppRetGrid)	\
    ( (This)->lpVtbl -> ObserveEx(This,nRow,nCol,bstrKey,bstrXml,ppRetGrid) ) 

#define IGrid_GetGridByName(This,bstrName,pVal)	\
    ( (This)->lpVtbl -> GetGridByName(This,bstrName,pVal) ) 

#define IGrid_GetCtrlValueByName(This,bstrName,bFindInChild,bstrVal)	\
    ( (This)->lpVtbl -> GetCtrlValueByName(This,bstrName,bFindInChild,bstrVal) ) 

#define IGrid_SetCtrlValueByName(This,bstrName,bFindInChild,bstrVal)	\
    ( (This)->lpVtbl -> SetCtrlValueByName(This,bstrName,bFindInChild,bstrVal) ) 

#define IGrid_NavigateURL(This,bstrURL,dispObjforScript)	\
    ( (This)->lpVtbl -> NavigateURL(This,bstrURL,dispObjforScript) ) 

#define IGrid_GetChildGridByName(This,bstrName,pVal)	\
    ( (This)->lpVtbl -> GetChildGridByName(This,bstrName,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGrid_INTERFACE_DEFINED__ */


#ifndef __IAppExtender_INTERFACE_DEFINED__
#define __IAppExtender_INTERFACE_DEFINED__

/* interface IAppExtender */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IAppExtender;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060119822007")
    IAppExtender : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ProcessNotify( 
            /* [in] */ BSTR bstrXmlNotify) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAppExtenderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAppExtender * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAppExtender * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAppExtender * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAppExtender * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAppExtender * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAppExtender * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAppExtender * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ProcessNotify )( 
            IAppExtender * This,
            /* [in] */ BSTR bstrXmlNotify);
        
        END_INTERFACE
    } IAppExtenderVtbl;

    interface IAppExtender
    {
        CONST_VTBL struct IAppExtenderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAppExtender_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAppExtender_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAppExtender_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAppExtender_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAppExtender_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAppExtender_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAppExtender_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IAppExtender_ProcessNotify(This,bstrXmlNotify)	\
    ( (This)->lpVtbl -> ProcessNotify(This,bstrXmlNotify) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAppExtender_INTERFACE_DEFINED__ */


#ifndef __IHubbleDoc_INTERFACE_DEFINED__
#define __IHubbleDoc_INTERFACE_DEFINED__

/* interface IHubbleDoc */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHubbleDoc;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060120161101")
    IHubbleDoc : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_TemplateXml( 
            /* [retval][out] */ BSTR *bstrVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_TemplateXml( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DocType( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DocID( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DocID( 
            /* [in] */ LONGLONG newVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGalaxyWndXml( 
            BSTR bstrWndID,
            /* [retval][out] */ BSTR *bstrWndScriptVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IHubbleDocVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHubbleDoc * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHubbleDoc * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHubbleDoc * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHubbleDoc * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHubbleDoc * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHubbleDoc * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHubbleDoc * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TemplateXml )( 
            IHubbleDoc * This,
            /* [retval][out] */ BSTR *bstrVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TemplateXml )( 
            IHubbleDoc * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DocType )( 
            IHubbleDoc * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DocID )( 
            IHubbleDoc * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DocID )( 
            IHubbleDoc * This,
            /* [in] */ LONGLONG newVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGalaxyWndXml )( 
            IHubbleDoc * This,
            BSTR bstrWndID,
            /* [retval][out] */ BSTR *bstrWndScriptVal);
        
        END_INTERFACE
    } IHubbleDocVtbl;

    interface IHubbleDoc
    {
        CONST_VTBL struct IHubbleDocVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHubbleDoc_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHubbleDoc_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHubbleDoc_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHubbleDoc_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IHubbleDoc_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IHubbleDoc_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IHubbleDoc_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IHubbleDoc_get_TemplateXml(This,bstrVal)	\
    ( (This)->lpVtbl -> get_TemplateXml(This,bstrVal) ) 

#define IHubbleDoc_put_TemplateXml(This,newVal)	\
    ( (This)->lpVtbl -> put_TemplateXml(This,newVal) ) 

#define IHubbleDoc_put_DocType(This,newVal)	\
    ( (This)->lpVtbl -> put_DocType(This,newVal) ) 

#define IHubbleDoc_get_DocID(This,pVal)	\
    ( (This)->lpVtbl -> get_DocID(This,pVal) ) 

#define IHubbleDoc_put_DocID(This,newVal)	\
    ( (This)->lpVtbl -> put_DocID(This,newVal) ) 

#define IHubbleDoc_GetGalaxyWndXml(This,bstrWndID,bstrWndScriptVal)	\
    ( (This)->lpVtbl -> GetGalaxyWndXml(This,bstrWndID,bstrWndScriptVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHubbleDoc_INTERFACE_DEFINED__ */


#ifndef __IHubbleDocTemplate_INTERFACE_DEFINED__
#define __IHubbleDocTemplate_INTERFACE_DEFINED__

/* interface IHubbleDocTemplate */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHubbleDocTemplate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060120170707")
    IHubbleDocTemplate : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_TemplateXml( 
            /* [retval][out] */ BSTR *bstrVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_TemplateXml( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DocType( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DocID( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DocID( 
            /* [in] */ LONGLONG newVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGalaxyWndXml( 
            BSTR bstrWndID,
            /* [retval][out] */ BSTR *bstrWndScriptVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IHubbleDocTemplateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHubbleDocTemplate * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHubbleDocTemplate * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHubbleDocTemplate * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHubbleDocTemplate * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHubbleDocTemplate * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHubbleDocTemplate * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHubbleDocTemplate * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TemplateXml )( 
            IHubbleDocTemplate * This,
            /* [retval][out] */ BSTR *bstrVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TemplateXml )( 
            IHubbleDocTemplate * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DocType )( 
            IHubbleDocTemplate * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DocID )( 
            IHubbleDocTemplate * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DocID )( 
            IHubbleDocTemplate * This,
            /* [in] */ LONGLONG newVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGalaxyWndXml )( 
            IHubbleDocTemplate * This,
            BSTR bstrWndID,
            /* [retval][out] */ BSTR *bstrWndScriptVal);
        
        END_INTERFACE
    } IHubbleDocTemplateVtbl;

    interface IHubbleDocTemplate
    {
        CONST_VTBL struct IHubbleDocTemplateVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHubbleDocTemplate_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHubbleDocTemplate_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHubbleDocTemplate_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHubbleDocTemplate_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IHubbleDocTemplate_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IHubbleDocTemplate_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IHubbleDocTemplate_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IHubbleDocTemplate_get_TemplateXml(This,bstrVal)	\
    ( (This)->lpVtbl -> get_TemplateXml(This,bstrVal) ) 

#define IHubbleDocTemplate_put_TemplateXml(This,newVal)	\
    ( (This)->lpVtbl -> put_TemplateXml(This,newVal) ) 

#define IHubbleDocTemplate_put_DocType(This,newVal)	\
    ( (This)->lpVtbl -> put_DocType(This,newVal) ) 

#define IHubbleDocTemplate_get_DocID(This,pVal)	\
    ( (This)->lpVtbl -> get_DocID(This,pVal) ) 

#define IHubbleDocTemplate_put_DocID(This,newVal)	\
    ( (This)->lpVtbl -> put_DocID(This,newVal) ) 

#define IHubbleDocTemplate_GetGalaxyWndXml(This,bstrWndID,bstrWndScriptVal)	\
    ( (This)->lpVtbl -> GetGalaxyWndXml(This,bstrWndID,bstrWndScriptVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHubbleDocTemplate_INTERFACE_DEFINED__ */


#ifndef __IHubble_INTERFACE_DEFINED__
#define __IHubble_INTERFACE_DEFINED__

/* interface IHubble */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHubble;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060119820001")
    IHubble : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HostChromeBrowserWnd( 
            /* [retval][out] */ IBrowser **ppChromeWebBrowser) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AppKeyValue( 
            BSTR bstrKey,
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AppKeyValue( 
            BSTR bstrKey,
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AppExtender( 
            BSTR bstrKey,
            /* [retval][out] */ IDispatch **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AppExtender( 
            BSTR bstrKey,
            /* [in] */ IDispatch *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RootNodes( 
            /* [retval][out] */ IGridCollection **ppGridColletion) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HostWnd( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentActiveGrid( 
            /* [retval][out] */ IGrid **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CreatingGrid( 
            /* [retval][out] */ IGrid **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_RemoteHubble( 
            BSTR bstrID,
            /* [retval][out] */ IHubble **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Extender( 
            /* [retval][out] */ IHubbleExtender **pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Extender( 
            /* [in] */ IHubbleExtender *newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_RemoteHelperHWND( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_RemoteHelperHWND( 
            /* [in] */ LONGLONG newVal) = 0;
        
        virtual /* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_DesignNode( 
            /* [retval][out] */ IGrid **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HubbleDoc( 
            LONGLONG AppProxy,
            LONGLONG nDocID,
            /* [retval][out] */ IHubbleDoc **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DocTemplate( 
            BSTR bstrID,
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveChromeBrowserWnd( 
            /* [retval][out] */ IBrowser **ppChromeWebBrowser) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE NavigateNode( 
            IGrid *pGrid,
            BSTR bstrBrowserID,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateGalaxyCluster( 
            LONGLONG hWnd,
            /* [retval][out] */ IGalaxyCluster **ppGalaxyCluster) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateOfficeDocument( 
            BSTR bstrXml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateCLRObj( 
            BSTR bstrObjID,
            /* [retval][out] */ IDispatch **ppDisp) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE StartApplication( 
            BSTR bstrAppID,
            BSTR bstrXml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetGalaxy( 
            LONGLONG hHostWnd,
            /* [retval][out] */ IGalaxy **ppGalaxy) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetItemText( 
            IGrid *pGrid,
            long nCtrlID,
            LONG nMaxLengeh,
            /* [retval][out] */ BSTR *bstrRet) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetItemText( 
            IGrid *pGrid,
            long nCtrlID,
            BSTR bstrText) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCLRControl( 
            IDispatch *CtrlDisp,
            BSTR bstrName,
            /* [retval][out] */ IDispatch **ppRetDisp) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MessageBox( 
            LONGLONG hWnd,
            BSTR bstrContext,
            BSTR bstrCaption,
            long nStyle,
            /* [retval][out] */ int *nRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Encode( 
            BSTR bstrSRC,
            VARIANT_BOOL bEncode,
            /* [retval][out] */ BSTR *bstrRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetHostFocus( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UpdateGrid( 
            IGrid *pGrid) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NewGUID( 
            /* [retval][out] */ BSTR *retVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DownLoadFile( 
            BSTR strFileURL,
            BSTR bstrTargetFile,
            BSTR bstrActionXml) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCtrlByName( 
            IDispatch *pCtrl,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            /* [retval][out] */ IDispatch **ppCtrlDisp) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGridFromHandle( 
            LONGLONG hWnd,
            /* [retval][out] */ IGrid **ppRetGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCtrlValueByName( 
            IDispatch *pCtrl,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            /* [retval][out] */ BSTR *bstrVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetCtrlValueByName( 
            IDispatch *pCtrl,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            BSTR bstrVal) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE CreateHubbleCtrl( 
            BSTR bstrAppID,
            /* [retval][out] */ IHubbleCtrl **ppRetCtrl) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetWindowClientDefaultNode( 
            IDispatch *pAddDisp,
            LONGLONG hParent,
            BSTR bstrWndClaName,
            BSTR bstrGalaxyClusterName,
            /* [retval][out] */ IGrid **ppGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDocTemplateXml( 
            BSTR bstrCaption,
            BSTR bstrPath,
            BSTR bstrFilter,
            /* [retval][out] */ BSTR *bstrTemplatePath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE OpenTangramFile( 
            /* [retval][out] */ IHubbleDoc **ppDoc) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateHubbleEventObj( 
            /* [retval][out] */ IHubbleEventObj **ppHubbleEventObj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FireHubbleEventObj( 
            IHubbleEventObj *pHubbleEventObj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ObserveGalaxys( 
            LONGLONG hWnd,
            BSTR bstrGalaxys,
            BSTR bstrKey,
            BSTR bstrXml,
            VARIANT_BOOL bSaveToConfigFile) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DeletePage( 
            LONGLONG PageHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LoadDocComponent( 
            BSTR bstrLib,
            /* [retval][out] */ LONGLONG *llAppProxy) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE OpenHubbleDocFile( 
            BSTR bstrFilePath,
            /* [retval][out] */ IHubbleDoc **ppDoc) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE InitCLRApp( 
            BSTR strInitXml,
            /* [retval][out] */ LONGLONG *llHandle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateOutLookObj( 
            BSTR bstrObjType,
            int nType,
            BSTR bstrURL,
            /* [retval][out] */ IDispatch **ppRetDisp) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE NewWorkBench( 
            BSTR bstrHubbleDoc,
            /* [retval][out] */ IWorkBenchWindow **ppWorkBenchWindow) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateBrowser( 
            ULONGLONG hParentWnd,
            BSTR strUrls,
            /* [retval][out] */ IBrowser **ppRet) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE HubbleNotify( 
            BSTR strXml1,
            BSTR strXml2,
            LONGLONG wParam,
            LONGLONG lParam) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DeleteGalaxy( 
            IGalaxy *pGalaxy) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE InitEclipseApp( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SelectVSObj( 
            BSTR bstrData,
            IDispatch *pVSObj,
            LONGLONG nHandle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE HubbleCommand( 
            IDispatch *RibbonControl) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE HubbleGetImage( 
            BSTR strValue,
            /* [retval][out] */ IPictureDisp **ppdispImage) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE HubbleGetVisible( 
            IDispatch *RibbonControl,
            /* [retval][out] */ VARIANT *varVisible) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE HubbleOnLoad( 
            IDispatch *RibbonControl) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE HubbleGetItemCount( 
            IDispatch *RibbonControl,
            /* [retval][out] */ long *nCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE HubbleGetItemLabel( 
            IDispatch *RibbonControl,
            long nIndex,
            /* [retval][out] */ BSTR *bstrLabel) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE HubbleGetItemID( 
            IDispatch *RibbonControl,
            long nIndex,
            /* [retval][out] */ BSTR *bstrID) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IHubbleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHubble * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHubble * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHubble * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHubble * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHubble * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHubble * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHubble * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HostChromeBrowserWnd )( 
            IHubble * This,
            /* [retval][out] */ IBrowser **ppChromeWebBrowser);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AppKeyValue )( 
            IHubble * This,
            BSTR bstrKey,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AppKeyValue )( 
            IHubble * This,
            BSTR bstrKey,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AppExtender )( 
            IHubble * This,
            BSTR bstrKey,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AppExtender )( 
            IHubble * This,
            BSTR bstrKey,
            /* [in] */ IDispatch *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RootNodes )( 
            IHubble * This,
            /* [retval][out] */ IGridCollection **ppGridColletion);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HostWnd )( 
            IHubble * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentActiveGrid )( 
            IHubble * This,
            /* [retval][out] */ IGrid **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CreatingGrid )( 
            IHubble * This,
            /* [retval][out] */ IGrid **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RemoteHubble )( 
            IHubble * This,
            BSTR bstrID,
            /* [retval][out] */ IHubble **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Extender )( 
            IHubble * This,
            /* [retval][out] */ IHubbleExtender **pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Extender )( 
            IHubble * This,
            /* [in] */ IHubbleExtender *newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RemoteHelperHWND )( 
            IHubble * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RemoteHelperHWND )( 
            IHubble * This,
            /* [in] */ LONGLONG newVal);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DesignNode )( 
            IHubble * This,
            /* [retval][out] */ IGrid **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HubbleDoc )( 
            IHubble * This,
            LONGLONG AppProxy,
            LONGLONG nDocID,
            /* [retval][out] */ IHubbleDoc **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DocTemplate )( 
            IHubble * This,
            BSTR bstrID,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveChromeBrowserWnd )( 
            IHubble * This,
            /* [retval][out] */ IBrowser **ppChromeWebBrowser);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *NavigateNode )( 
            IHubble * This,
            IGrid *pGrid,
            BSTR bstrBrowserID,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateGalaxyCluster )( 
            IHubble * This,
            LONGLONG hWnd,
            /* [retval][out] */ IGalaxyCluster **ppGalaxyCluster);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateOfficeDocument )( 
            IHubble * This,
            BSTR bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateCLRObj )( 
            IHubble * This,
            BSTR bstrObjID,
            /* [retval][out] */ IDispatch **ppDisp);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StartApplication )( 
            IHubble * This,
            BSTR bstrAppID,
            BSTR bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetGalaxy )( 
            IHubble * This,
            LONGLONG hHostWnd,
            /* [retval][out] */ IGalaxy **ppGalaxy);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetItemText )( 
            IHubble * This,
            IGrid *pGrid,
            long nCtrlID,
            LONG nMaxLengeh,
            /* [retval][out] */ BSTR *bstrRet);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetItemText )( 
            IHubble * This,
            IGrid *pGrid,
            long nCtrlID,
            BSTR bstrText);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCLRControl )( 
            IHubble * This,
            IDispatch *CtrlDisp,
            BSTR bstrName,
            /* [retval][out] */ IDispatch **ppRetDisp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MessageBox )( 
            IHubble * This,
            LONGLONG hWnd,
            BSTR bstrContext,
            BSTR bstrCaption,
            long nStyle,
            /* [retval][out] */ int *nRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Encode )( 
            IHubble * This,
            BSTR bstrSRC,
            VARIANT_BOOL bEncode,
            /* [retval][out] */ BSTR *bstrRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetHostFocus )( 
            IHubble * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UpdateGrid )( 
            IHubble * This,
            IGrid *pGrid);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NewGUID )( 
            IHubble * This,
            /* [retval][out] */ BSTR *retVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DownLoadFile )( 
            IHubble * This,
            BSTR strFileURL,
            BSTR bstrTargetFile,
            BSTR bstrActionXml);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCtrlByName )( 
            IHubble * This,
            IDispatch *pCtrl,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            /* [retval][out] */ IDispatch **ppCtrlDisp);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGridFromHandle )( 
            IHubble * This,
            LONGLONG hWnd,
            /* [retval][out] */ IGrid **ppRetGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCtrlValueByName )( 
            IHubble * This,
            IDispatch *pCtrl,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            /* [retval][out] */ BSTR *bstrVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetCtrlValueByName )( 
            IHubble * This,
            IDispatch *pCtrl,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            BSTR bstrVal);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *CreateHubbleCtrl )( 
            IHubble * This,
            BSTR bstrAppID,
            /* [retval][out] */ IHubbleCtrl **ppRetCtrl);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetWindowClientDefaultNode )( 
            IHubble * This,
            IDispatch *pAddDisp,
            LONGLONG hParent,
            BSTR bstrWndClaName,
            BSTR bstrGalaxyClusterName,
            /* [retval][out] */ IGrid **ppGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDocTemplateXml )( 
            IHubble * This,
            BSTR bstrCaption,
            BSTR bstrPath,
            BSTR bstrFilter,
            /* [retval][out] */ BSTR *bstrTemplatePath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *OpenTangramFile )( 
            IHubble * This,
            /* [retval][out] */ IHubbleDoc **ppDoc);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateHubbleEventObj )( 
            IHubble * This,
            /* [retval][out] */ IHubbleEventObj **ppHubbleEventObj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FireHubbleEventObj )( 
            IHubble * This,
            IHubbleEventObj *pHubbleEventObj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ObserveGalaxys )( 
            IHubble * This,
            LONGLONG hWnd,
            BSTR bstrGalaxys,
            BSTR bstrKey,
            BSTR bstrXml,
            VARIANT_BOOL bSaveToConfigFile);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DeletePage )( 
            IHubble * This,
            LONGLONG PageHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LoadDocComponent )( 
            IHubble * This,
            BSTR bstrLib,
            /* [retval][out] */ LONGLONG *llAppProxy);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *OpenHubbleDocFile )( 
            IHubble * This,
            BSTR bstrFilePath,
            /* [retval][out] */ IHubbleDoc **ppDoc);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *InitCLRApp )( 
            IHubble * This,
            BSTR strInitXml,
            /* [retval][out] */ LONGLONG *llHandle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateOutLookObj )( 
            IHubble * This,
            BSTR bstrObjType,
            int nType,
            BSTR bstrURL,
            /* [retval][out] */ IDispatch **ppRetDisp);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *NewWorkBench )( 
            IHubble * This,
            BSTR bstrHubbleDoc,
            /* [retval][out] */ IWorkBenchWindow **ppWorkBenchWindow);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateBrowser )( 
            IHubble * This,
            ULONGLONG hParentWnd,
            BSTR strUrls,
            /* [retval][out] */ IBrowser **ppRet);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *HubbleNotify )( 
            IHubble * This,
            BSTR strXml1,
            BSTR strXml2,
            LONGLONG wParam,
            LONGLONG lParam);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DeleteGalaxy )( 
            IHubble * This,
            IGalaxy *pGalaxy);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *InitEclipseApp )( 
            IHubble * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SelectVSObj )( 
            IHubble * This,
            BSTR bstrData,
            IDispatch *pVSObj,
            LONGLONG nHandle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *HubbleCommand )( 
            IHubble * This,
            IDispatch *RibbonControl);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *HubbleGetImage )( 
            IHubble * This,
            BSTR strValue,
            /* [retval][out] */ IPictureDisp **ppdispImage);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *HubbleGetVisible )( 
            IHubble * This,
            IDispatch *RibbonControl,
            /* [retval][out] */ VARIANT *varVisible);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *HubbleOnLoad )( 
            IHubble * This,
            IDispatch *RibbonControl);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *HubbleGetItemCount )( 
            IHubble * This,
            IDispatch *RibbonControl,
            /* [retval][out] */ long *nCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *HubbleGetItemLabel )( 
            IHubble * This,
            IDispatch *RibbonControl,
            long nIndex,
            /* [retval][out] */ BSTR *bstrLabel);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *HubbleGetItemID )( 
            IHubble * This,
            IDispatch *RibbonControl,
            long nIndex,
            /* [retval][out] */ BSTR *bstrID);
        
        END_INTERFACE
    } IHubbleVtbl;

    interface IHubble
    {
        CONST_VTBL struct IHubbleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHubble_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHubble_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHubble_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHubble_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IHubble_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IHubble_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IHubble_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IHubble_get_HostChromeBrowserWnd(This,ppChromeWebBrowser)	\
    ( (This)->lpVtbl -> get_HostChromeBrowserWnd(This,ppChromeWebBrowser) ) 

#define IHubble_get_AppKeyValue(This,bstrKey,pVal)	\
    ( (This)->lpVtbl -> get_AppKeyValue(This,bstrKey,pVal) ) 

#define IHubble_put_AppKeyValue(This,bstrKey,newVal)	\
    ( (This)->lpVtbl -> put_AppKeyValue(This,bstrKey,newVal) ) 

#define IHubble_get_AppExtender(This,bstrKey,pVal)	\
    ( (This)->lpVtbl -> get_AppExtender(This,bstrKey,pVal) ) 

#define IHubble_put_AppExtender(This,bstrKey,newVal)	\
    ( (This)->lpVtbl -> put_AppExtender(This,bstrKey,newVal) ) 

#define IHubble_get_RootNodes(This,ppGridColletion)	\
    ( (This)->lpVtbl -> get_RootNodes(This,ppGridColletion) ) 

#define IHubble_get_HostWnd(This,pVal)	\
    ( (This)->lpVtbl -> get_HostWnd(This,pVal) ) 

#define IHubble_get_CurrentActiveGrid(This,pVal)	\
    ( (This)->lpVtbl -> get_CurrentActiveGrid(This,pVal) ) 

#define IHubble_get_CreatingGrid(This,pVal)	\
    ( (This)->lpVtbl -> get_CreatingGrid(This,pVal) ) 

#define IHubble_get_RemoteHubble(This,bstrID,pVal)	\
    ( (This)->lpVtbl -> get_RemoteHubble(This,bstrID,pVal) ) 

#define IHubble_get_Extender(This,pVal)	\
    ( (This)->lpVtbl -> get_Extender(This,pVal) ) 

#define IHubble_put_Extender(This,newVal)	\
    ( (This)->lpVtbl -> put_Extender(This,newVal) ) 

#define IHubble_get_RemoteHelperHWND(This,pVal)	\
    ( (This)->lpVtbl -> get_RemoteHelperHWND(This,pVal) ) 

#define IHubble_put_RemoteHelperHWND(This,newVal)	\
    ( (This)->lpVtbl -> put_RemoteHelperHWND(This,newVal) ) 

#define IHubble_get_DesignNode(This,pVal)	\
    ( (This)->lpVtbl -> get_DesignNode(This,pVal) ) 

#define IHubble_get_HubbleDoc(This,AppProxy,nDocID,pVal)	\
    ( (This)->lpVtbl -> get_HubbleDoc(This,AppProxy,nDocID,pVal) ) 

#define IHubble_get_DocTemplate(This,bstrID,pVal)	\
    ( (This)->lpVtbl -> get_DocTemplate(This,bstrID,pVal) ) 

#define IHubble_get_ActiveChromeBrowserWnd(This,ppChromeWebBrowser)	\
    ( (This)->lpVtbl -> get_ActiveChromeBrowserWnd(This,ppChromeWebBrowser) ) 

#define IHubble_NavigateNode(This,pGrid,bstrBrowserID,bstrXml,pVal)	\
    ( (This)->lpVtbl -> NavigateNode(This,pGrid,bstrBrowserID,bstrXml,pVal) ) 

#define IHubble_CreateGalaxyCluster(This,hWnd,ppGalaxyCluster)	\
    ( (This)->lpVtbl -> CreateGalaxyCluster(This,hWnd,ppGalaxyCluster) ) 

#define IHubble_CreateOfficeDocument(This,bstrXml)	\
    ( (This)->lpVtbl -> CreateOfficeDocument(This,bstrXml) ) 

#define IHubble_CreateCLRObj(This,bstrObjID,ppDisp)	\
    ( (This)->lpVtbl -> CreateCLRObj(This,bstrObjID,ppDisp) ) 

#define IHubble_StartApplication(This,bstrAppID,bstrXml)	\
    ( (This)->lpVtbl -> StartApplication(This,bstrAppID,bstrXml) ) 

#define IHubble_GetGalaxy(This,hHostWnd,ppGalaxy)	\
    ( (This)->lpVtbl -> GetGalaxy(This,hHostWnd,ppGalaxy) ) 

#define IHubble_GetItemText(This,pGrid,nCtrlID,nMaxLengeh,bstrRet)	\
    ( (This)->lpVtbl -> GetItemText(This,pGrid,nCtrlID,nMaxLengeh,bstrRet) ) 

#define IHubble_SetItemText(This,pGrid,nCtrlID,bstrText)	\
    ( (This)->lpVtbl -> SetItemText(This,pGrid,nCtrlID,bstrText) ) 

#define IHubble_GetCLRControl(This,CtrlDisp,bstrName,ppRetDisp)	\
    ( (This)->lpVtbl -> GetCLRControl(This,CtrlDisp,bstrName,ppRetDisp) ) 

#define IHubble_MessageBox(This,hWnd,bstrContext,bstrCaption,nStyle,nRet)	\
    ( (This)->lpVtbl -> MessageBox(This,hWnd,bstrContext,bstrCaption,nStyle,nRet) ) 

#define IHubble_Encode(This,bstrSRC,bEncode,bstrRet)	\
    ( (This)->lpVtbl -> Encode(This,bstrSRC,bEncode,bstrRet) ) 

#define IHubble_SetHostFocus(This)	\
    ( (This)->lpVtbl -> SetHostFocus(This) ) 

#define IHubble_UpdateGrid(This,pGrid)	\
    ( (This)->lpVtbl -> UpdateGrid(This,pGrid) ) 

#define IHubble_NewGUID(This,retVal)	\
    ( (This)->lpVtbl -> NewGUID(This,retVal) ) 

#define IHubble_DownLoadFile(This,strFileURL,bstrTargetFile,bstrActionXml)	\
    ( (This)->lpVtbl -> DownLoadFile(This,strFileURL,bstrTargetFile,bstrActionXml) ) 

#define IHubble_GetCtrlByName(This,pCtrl,bstrName,bFindInChild,ppCtrlDisp)	\
    ( (This)->lpVtbl -> GetCtrlByName(This,pCtrl,bstrName,bFindInChild,ppCtrlDisp) ) 

#define IHubble_GetGridFromHandle(This,hWnd,ppRetGrid)	\
    ( (This)->lpVtbl -> GetGridFromHandle(This,hWnd,ppRetGrid) ) 

#define IHubble_GetCtrlValueByName(This,pCtrl,bstrName,bFindInChild,bstrVal)	\
    ( (This)->lpVtbl -> GetCtrlValueByName(This,pCtrl,bstrName,bFindInChild,bstrVal) ) 

#define IHubble_SetCtrlValueByName(This,pCtrl,bstrName,bFindInChild,bstrVal)	\
    ( (This)->lpVtbl -> SetCtrlValueByName(This,pCtrl,bstrName,bFindInChild,bstrVal) ) 

#define IHubble_CreateHubbleCtrl(This,bstrAppID,ppRetCtrl)	\
    ( (This)->lpVtbl -> CreateHubbleCtrl(This,bstrAppID,ppRetCtrl) ) 

#define IHubble_GetWindowClientDefaultNode(This,pAddDisp,hParent,bstrWndClaName,bstrGalaxyClusterName,ppGrid)	\
    ( (This)->lpVtbl -> GetWindowClientDefaultNode(This,pAddDisp,hParent,bstrWndClaName,bstrGalaxyClusterName,ppGrid) ) 

#define IHubble_GetDocTemplateXml(This,bstrCaption,bstrPath,bstrFilter,bstrTemplatePath)	\
    ( (This)->lpVtbl -> GetDocTemplateXml(This,bstrCaption,bstrPath,bstrFilter,bstrTemplatePath) ) 

#define IHubble_OpenTangramFile(This,ppDoc)	\
    ( (This)->lpVtbl -> OpenTangramFile(This,ppDoc) ) 

#define IHubble_CreateHubbleEventObj(This,ppHubbleEventObj)	\
    ( (This)->lpVtbl -> CreateHubbleEventObj(This,ppHubbleEventObj) ) 

#define IHubble_FireHubbleEventObj(This,pHubbleEventObj)	\
    ( (This)->lpVtbl -> FireHubbleEventObj(This,pHubbleEventObj) ) 

#define IHubble_ObserveGalaxys(This,hWnd,bstrGalaxys,bstrKey,bstrXml,bSaveToConfigFile)	\
    ( (This)->lpVtbl -> ObserveGalaxys(This,hWnd,bstrGalaxys,bstrKey,bstrXml,bSaveToConfigFile) ) 

#define IHubble_DeletePage(This,PageHandle)	\
    ( (This)->lpVtbl -> DeletePage(This,PageHandle) ) 

#define IHubble_LoadDocComponent(This,bstrLib,llAppProxy)	\
    ( (This)->lpVtbl -> LoadDocComponent(This,bstrLib,llAppProxy) ) 

#define IHubble_OpenHubbleDocFile(This,bstrFilePath,ppDoc)	\
    ( (This)->lpVtbl -> OpenHubbleDocFile(This,bstrFilePath,ppDoc) ) 

#define IHubble_InitCLRApp(This,strInitXml,llHandle)	\
    ( (This)->lpVtbl -> InitCLRApp(This,strInitXml,llHandle) ) 

#define IHubble_CreateOutLookObj(This,bstrObjType,nType,bstrURL,ppRetDisp)	\
    ( (This)->lpVtbl -> CreateOutLookObj(This,bstrObjType,nType,bstrURL,ppRetDisp) ) 

#define IHubble_NewWorkBench(This,bstrHubbleDoc,ppWorkBenchWindow)	\
    ( (This)->lpVtbl -> NewWorkBench(This,bstrHubbleDoc,ppWorkBenchWindow) ) 

#define IHubble_CreateBrowser(This,hParentWnd,strUrls,ppRet)	\
    ( (This)->lpVtbl -> CreateBrowser(This,hParentWnd,strUrls,ppRet) ) 

#define IHubble_HubbleNotify(This,strXml1,strXml2,wParam,lParam)	\
    ( (This)->lpVtbl -> HubbleNotify(This,strXml1,strXml2,wParam,lParam) ) 

#define IHubble_DeleteGalaxy(This,pGalaxy)	\
    ( (This)->lpVtbl -> DeleteGalaxy(This,pGalaxy) ) 

#define IHubble_InitEclipseApp(This)	\
    ( (This)->lpVtbl -> InitEclipseApp(This) ) 

#define IHubble_SelectVSObj(This,bstrData,pVSObj,nHandle)	\
    ( (This)->lpVtbl -> SelectVSObj(This,bstrData,pVSObj,nHandle) ) 

#define IHubble_HubbleCommand(This,RibbonControl)	\
    ( (This)->lpVtbl -> HubbleCommand(This,RibbonControl) ) 

#define IHubble_HubbleGetImage(This,strValue,ppdispImage)	\
    ( (This)->lpVtbl -> HubbleGetImage(This,strValue,ppdispImage) ) 

#define IHubble_HubbleGetVisible(This,RibbonControl,varVisible)	\
    ( (This)->lpVtbl -> HubbleGetVisible(This,RibbonControl,varVisible) ) 

#define IHubble_HubbleOnLoad(This,RibbonControl)	\
    ( (This)->lpVtbl -> HubbleOnLoad(This,RibbonControl) ) 

#define IHubble_HubbleGetItemCount(This,RibbonControl,nCount)	\
    ( (This)->lpVtbl -> HubbleGetItemCount(This,RibbonControl,nCount) ) 

#define IHubble_HubbleGetItemLabel(This,RibbonControl,nIndex,bstrLabel)	\
    ( (This)->lpVtbl -> HubbleGetItemLabel(This,RibbonControl,nIndex,bstrLabel) ) 

#define IHubble_HubbleGetItemID(This,RibbonControl,nIndex,bstrID)	\
    ( (This)->lpVtbl -> HubbleGetItemID(This,RibbonControl,nIndex,bstrID) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHubble_INTERFACE_DEFINED__ */


#ifndef __IGalaxy_INTERFACE_DEFINED__
#define __IGalaxy_INTERFACE_DEFINED__

/* interface IGalaxy */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IGalaxy;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060119820003")
    IGalaxy : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Grid( 
            /* [in] */ VARIANT vIndex,
            /* [retval][out] */ IGrid **ppGrid) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HWND( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_VisibleGrid( 
            /* [retval][out] */ IGrid **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentNavigateKey( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_GalaxyCluster( 
            /* [retval][out] */ IGalaxyCluster **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_GalaxyData( 
            BSTR bstrKey,
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_GalaxyData( 
            BSTR bstrKey,
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_DesignerState( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [hidden][id][propput] */ HRESULT STDMETHODCALLTYPE put_DesignerState( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RootGrids( 
            /* [retval][out] */ IGridCollection **ppGridColletion) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_GalaxyXML( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HubbleDoc( 
            /* [retval][out] */ IHubbleDoc **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_GalaxyType( 
            /* [retval][out] */ GalaxyType *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HostBrowser( 
            /* [retval][out] */ IBrowser **ppChromeWebBrowser) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HostWebPage( 
            /* [retval][out] */ IWebPage **ppChromeWebPage) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Detach( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Attach( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ModifyHost( 
            LONGLONG hHostWnd) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Observe( 
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppRetGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetXml( 
            BSTR bstrRootName,
            /* [retval][out] */ BSTR *bstrRet) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IGalaxyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGalaxy * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGalaxy * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGalaxy * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IGalaxy * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IGalaxy * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IGalaxy * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IGalaxy * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Grid )( 
            IGalaxy * This,
            /* [in] */ VARIANT vIndex,
            /* [retval][out] */ IGrid **ppGrid);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IGalaxy * This,
            /* [retval][out] */ IUnknown **ppVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWND )( 
            IGalaxy * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VisibleGrid )( 
            IGalaxy * This,
            /* [retval][out] */ IGrid **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentNavigateKey )( 
            IGalaxy * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GalaxyCluster )( 
            IGalaxy * This,
            /* [retval][out] */ IGalaxyCluster **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GalaxyData )( 
            IGalaxy * This,
            BSTR bstrKey,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_GalaxyData )( 
            IGalaxy * This,
            BSTR bstrKey,
            /* [in] */ VARIANT newVal);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DesignerState )( 
            IGalaxy * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [hidden][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DesignerState )( 
            IGalaxy * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IGalaxy * This,
            /* [retval][out] */ long *pCount);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RootGrids )( 
            IGalaxy * This,
            /* [retval][out] */ IGridCollection **ppGridColletion);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GalaxyXML )( 
            IGalaxy * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HubbleDoc )( 
            IGalaxy * This,
            /* [retval][out] */ IHubbleDoc **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GalaxyType )( 
            IGalaxy * This,
            /* [retval][out] */ GalaxyType *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IGalaxy * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HostBrowser )( 
            IGalaxy * This,
            /* [retval][out] */ IBrowser **ppChromeWebBrowser);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HostWebPage )( 
            IGalaxy * This,
            /* [retval][out] */ IWebPage **ppChromeWebPage);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Detach )( 
            IGalaxy * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Attach )( 
            IGalaxy * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ModifyHost )( 
            IGalaxy * This,
            LONGLONG hHostWnd);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Observe )( 
            IGalaxy * This,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppRetGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetXml )( 
            IGalaxy * This,
            BSTR bstrRootName,
            /* [retval][out] */ BSTR *bstrRet);
        
        END_INTERFACE
    } IGalaxyVtbl;

    interface IGalaxy
    {
        CONST_VTBL struct IGalaxyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGalaxy_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGalaxy_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGalaxy_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGalaxy_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IGalaxy_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IGalaxy_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IGalaxy_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IGalaxy_get_Grid(This,vIndex,ppGrid)	\
    ( (This)->lpVtbl -> get_Grid(This,vIndex,ppGrid) ) 

#define IGalaxy_get__NewEnum(This,ppVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppVal) ) 

#define IGalaxy_get_HWND(This,pVal)	\
    ( (This)->lpVtbl -> get_HWND(This,pVal) ) 

#define IGalaxy_get_VisibleGrid(This,pVal)	\
    ( (This)->lpVtbl -> get_VisibleGrid(This,pVal) ) 

#define IGalaxy_get_CurrentNavigateKey(This,pVal)	\
    ( (This)->lpVtbl -> get_CurrentNavigateKey(This,pVal) ) 

#define IGalaxy_get_GalaxyCluster(This,pVal)	\
    ( (This)->lpVtbl -> get_GalaxyCluster(This,pVal) ) 

#define IGalaxy_get_GalaxyData(This,bstrKey,pVal)	\
    ( (This)->lpVtbl -> get_GalaxyData(This,bstrKey,pVal) ) 

#define IGalaxy_put_GalaxyData(This,bstrKey,newVal)	\
    ( (This)->lpVtbl -> put_GalaxyData(This,bstrKey,newVal) ) 

#define IGalaxy_get_DesignerState(This,pVal)	\
    ( (This)->lpVtbl -> get_DesignerState(This,pVal) ) 

#define IGalaxy_put_DesignerState(This,newVal)	\
    ( (This)->lpVtbl -> put_DesignerState(This,newVal) ) 

#define IGalaxy_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define IGalaxy_get_RootGrids(This,ppGridColletion)	\
    ( (This)->lpVtbl -> get_RootGrids(This,ppGridColletion) ) 

#define IGalaxy_get_GalaxyXML(This,pVal)	\
    ( (This)->lpVtbl -> get_GalaxyXML(This,pVal) ) 

#define IGalaxy_get_HubbleDoc(This,pVal)	\
    ( (This)->lpVtbl -> get_HubbleDoc(This,pVal) ) 

#define IGalaxy_get_GalaxyType(This,pVal)	\
    ( (This)->lpVtbl -> get_GalaxyType(This,pVal) ) 

#define IGalaxy_get_Name(This,pVal)	\
    ( (This)->lpVtbl -> get_Name(This,pVal) ) 

#define IGalaxy_get_HostBrowser(This,ppChromeWebBrowser)	\
    ( (This)->lpVtbl -> get_HostBrowser(This,ppChromeWebBrowser) ) 

#define IGalaxy_get_HostWebPage(This,ppChromeWebPage)	\
    ( (This)->lpVtbl -> get_HostWebPage(This,ppChromeWebPage) ) 

#define IGalaxy_Detach(This)	\
    ( (This)->lpVtbl -> Detach(This) ) 

#define IGalaxy_Attach(This)	\
    ( (This)->lpVtbl -> Attach(This) ) 

#define IGalaxy_ModifyHost(This,hHostWnd)	\
    ( (This)->lpVtbl -> ModifyHost(This,hHostWnd) ) 

#define IGalaxy_Observe(This,bstrKey,bstrXml,ppRetGrid)	\
    ( (This)->lpVtbl -> Observe(This,bstrKey,bstrXml,ppRetGrid) ) 

#define IGalaxy_GetXml(This,bstrRootName,bstrRet)	\
    ( (This)->lpVtbl -> GetXml(This,bstrRootName,bstrRet) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGalaxy_INTERFACE_DEFINED__ */


#ifndef __IGridCollection_INTERFACE_DEFINED__
#define __IGridCollection_INTERFACE_DEFINED__

/* interface IGridCollection */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IGridCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060119820017")
    IGridCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GridCount( 
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long iIndex,
            /* [retval][out] */ IGrid **ppTopWindow) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IGridCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGridCollection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGridCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGridCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IGridCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IGridCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IGridCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IGridCollection * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GridCount )( 
            IGridCollection * This,
            /* [retval][out] */ long *pCount);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IGridCollection * This,
            /* [in] */ long iIndex,
            /* [retval][out] */ IGrid **ppTopWindow);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IGridCollection * This,
            /* [retval][out] */ IUnknown **ppVal);
        
        END_INTERFACE
    } IGridCollectionVtbl;

    interface IGridCollection
    {
        CONST_VTBL struct IGridCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGridCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGridCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGridCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGridCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IGridCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IGridCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IGridCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IGridCollection_get_GridCount(This,pCount)	\
    ( (This)->lpVtbl -> get_GridCount(This,pCount) ) 

#define IGridCollection_get_Item(This,iIndex,ppTopWindow)	\
    ( (This)->lpVtbl -> get_Item(This,iIndex,ppTopWindow) ) 

#define IGridCollection_get__NewEnum(This,ppVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGridCollection_INTERFACE_DEFINED__ */


#ifndef __IGalaxyCluster_INTERFACE_DEFINED__
#define __IGalaxyCluster_INTERFACE_DEFINED__

/* interface IGalaxyCluster */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IGalaxyCluster;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060119820002")
    IGalaxyCluster : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Galaxy( 
            /* [in] */ VARIANT vIndex,
            /* [retval][out] */ IGalaxy **ppGalaxy) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_xtml( 
            BSTR strKey,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_xtml( 
            BSTR strKey,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Extender( 
            BSTR bstrExtenderName,
            /* [retval][out] */ IDispatch **pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Extender( 
            BSTR bstrExtenderName,
            /* [in] */ IDispatch *newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Grid( 
            BSTR bstrNodeName,
            /* [retval][out] */ IGrid **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_XObject( 
            BSTR bstrName,
            /* [retval][out] */ IDispatch **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_GalaxyName( 
            LONGLONG hHwnd,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Handle( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Width( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Height( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_GridNames( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Parent( 
            /* [retval][out] */ IGalaxyCluster **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_GalaxyClusterXML( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ConfigName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentDesignGalaxyType( 
            /* [retval][out] */ GalaxyType *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentDesignNode( 
            /* [retval][out] */ IGrid **pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateGalaxy( 
            VARIANT ParentObj,
            VARIANT HostWnd,
            BSTR bstrGalaxyName,
            /* [retval][out] */ IGalaxy **pRetGalaxy) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Observe( 
            IDispatch *Parent,
            BSTR CtrlName,
            BSTR GalaxyName,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppRetGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ObserveCtrl( 
            VARIANT Ctrl,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppRetGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGrid( 
            BSTR bstrGalaxyName,
            BSTR bstrNodeName,
            /* [retval][out] */ IGrid **pRetNode) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCtrlInGrid( 
            BSTR NodeName,
            BSTR CtrlName,
            /* [retval][out] */ IDispatch **ppCtrl) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGalaxyFromCtrl( 
            IDispatch *ctrl,
            /* [retval][out] */ IGalaxy **ppGalaxy) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ConnectHubbleCtrl( 
            IHubbleCtrl *eventSource) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateGalaxyWithDefaultNode( 
            ULONGLONG hGalaxyWnd,
            BSTR bstrGalaxyName,
            BSTR bstrDefaultNodeKey,
            BSTR bstrXml,
            VARIANT_BOOL bSaveToConfig,
            /* [retval][out] */ IGrid **ppGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ObserveGalaxys( 
            BSTR bstrGalaxys,
            BSTR bstrKey,
            BSTR bstrXml,
            VARIANT_BOOL bSaveToConfigFile) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IGalaxyClusterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGalaxyCluster * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGalaxyCluster * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGalaxyCluster * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IGalaxyCluster * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IGalaxyCluster * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IGalaxyCluster * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IGalaxyCluster * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Galaxy )( 
            IGalaxyCluster * This,
            /* [in] */ VARIANT vIndex,
            /* [retval][out] */ IGalaxy **ppGalaxy);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IGalaxyCluster * This,
            /* [retval][out] */ IUnknown **ppVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IGalaxyCluster * This,
            /* [retval][out] */ long *pCount);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_xtml )( 
            IGalaxyCluster * This,
            BSTR strKey,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_xtml )( 
            IGalaxyCluster * This,
            BSTR strKey,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Extender )( 
            IGalaxyCluster * This,
            BSTR bstrExtenderName,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Extender )( 
            IGalaxyCluster * This,
            BSTR bstrExtenderName,
            /* [in] */ IDispatch *newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Grid )( 
            IGalaxyCluster * This,
            BSTR bstrNodeName,
            /* [retval][out] */ IGrid **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_XObject )( 
            IGalaxyCluster * This,
            BSTR bstrName,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GalaxyName )( 
            IGalaxyCluster * This,
            LONGLONG hHwnd,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Handle )( 
            IGalaxyCluster * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IGalaxyCluster * This,
            /* [retval][out] */ long *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IGalaxyCluster * This,
            /* [in] */ long newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IGalaxyCluster * This,
            /* [retval][out] */ long *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IGalaxyCluster * This,
            /* [in] */ long newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GridNames )( 
            IGalaxyCluster * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Parent )( 
            IGalaxyCluster * This,
            /* [retval][out] */ IGalaxyCluster **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GalaxyClusterXML )( 
            IGalaxyCluster * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ConfigName )( 
            IGalaxyCluster * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentDesignGalaxyType )( 
            IGalaxyCluster * This,
            /* [retval][out] */ GalaxyType *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentDesignNode )( 
            IGalaxyCluster * This,
            /* [retval][out] */ IGrid **pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateGalaxy )( 
            IGalaxyCluster * This,
            VARIANT ParentObj,
            VARIANT HostWnd,
            BSTR bstrGalaxyName,
            /* [retval][out] */ IGalaxy **pRetGalaxy);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Observe )( 
            IGalaxyCluster * This,
            IDispatch *Parent,
            BSTR CtrlName,
            BSTR GalaxyName,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppRetGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ObserveCtrl )( 
            IGalaxyCluster * This,
            VARIANT Ctrl,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppRetGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGrid )( 
            IGalaxyCluster * This,
            BSTR bstrGalaxyName,
            BSTR bstrNodeName,
            /* [retval][out] */ IGrid **pRetNode);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCtrlInGrid )( 
            IGalaxyCluster * This,
            BSTR NodeName,
            BSTR CtrlName,
            /* [retval][out] */ IDispatch **ppCtrl);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGalaxyFromCtrl )( 
            IGalaxyCluster * This,
            IDispatch *ctrl,
            /* [retval][out] */ IGalaxy **ppGalaxy);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ConnectHubbleCtrl )( 
            IGalaxyCluster * This,
            IHubbleCtrl *eventSource);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateGalaxyWithDefaultNode )( 
            IGalaxyCluster * This,
            ULONGLONG hGalaxyWnd,
            BSTR bstrGalaxyName,
            BSTR bstrDefaultNodeKey,
            BSTR bstrXml,
            VARIANT_BOOL bSaveToConfig,
            /* [retval][out] */ IGrid **ppGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ObserveGalaxys )( 
            IGalaxyCluster * This,
            BSTR bstrGalaxys,
            BSTR bstrKey,
            BSTR bstrXml,
            VARIANT_BOOL bSaveToConfigFile);
        
        END_INTERFACE
    } IGalaxyClusterVtbl;

    interface IGalaxyCluster
    {
        CONST_VTBL struct IGalaxyClusterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGalaxyCluster_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGalaxyCluster_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGalaxyCluster_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGalaxyCluster_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IGalaxyCluster_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IGalaxyCluster_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IGalaxyCluster_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IGalaxyCluster_get_Galaxy(This,vIndex,ppGalaxy)	\
    ( (This)->lpVtbl -> get_Galaxy(This,vIndex,ppGalaxy) ) 

#define IGalaxyCluster_get__NewEnum(This,ppVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppVal) ) 

#define IGalaxyCluster_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define IGalaxyCluster_get_xtml(This,strKey,pVal)	\
    ( (This)->lpVtbl -> get_xtml(This,strKey,pVal) ) 

#define IGalaxyCluster_put_xtml(This,strKey,newVal)	\
    ( (This)->lpVtbl -> put_xtml(This,strKey,newVal) ) 

#define IGalaxyCluster_get_Extender(This,bstrExtenderName,pVal)	\
    ( (This)->lpVtbl -> get_Extender(This,bstrExtenderName,pVal) ) 

#define IGalaxyCluster_put_Extender(This,bstrExtenderName,newVal)	\
    ( (This)->lpVtbl -> put_Extender(This,bstrExtenderName,newVal) ) 

#define IGalaxyCluster_get_Grid(This,bstrNodeName,pVal)	\
    ( (This)->lpVtbl -> get_Grid(This,bstrNodeName,pVal) ) 

#define IGalaxyCluster_get_XObject(This,bstrName,pVal)	\
    ( (This)->lpVtbl -> get_XObject(This,bstrName,pVal) ) 

#define IGalaxyCluster_get_GalaxyName(This,hHwnd,pVal)	\
    ( (This)->lpVtbl -> get_GalaxyName(This,hHwnd,pVal) ) 

#define IGalaxyCluster_get_Handle(This,pVal)	\
    ( (This)->lpVtbl -> get_Handle(This,pVal) ) 

#define IGalaxyCluster_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IGalaxyCluster_put_Width(This,newVal)	\
    ( (This)->lpVtbl -> put_Width(This,newVal) ) 

#define IGalaxyCluster_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IGalaxyCluster_put_Height(This,newVal)	\
    ( (This)->lpVtbl -> put_Height(This,newVal) ) 

#define IGalaxyCluster_get_GridNames(This,pVal)	\
    ( (This)->lpVtbl -> get_GridNames(This,pVal) ) 

#define IGalaxyCluster_get_Parent(This,pVal)	\
    ( (This)->lpVtbl -> get_Parent(This,pVal) ) 

#define IGalaxyCluster_get_GalaxyClusterXML(This,pVal)	\
    ( (This)->lpVtbl -> get_GalaxyClusterXML(This,pVal) ) 

#define IGalaxyCluster_put_ConfigName(This,newVal)	\
    ( (This)->lpVtbl -> put_ConfigName(This,newVal) ) 

#define IGalaxyCluster_get_CurrentDesignGalaxyType(This,pVal)	\
    ( (This)->lpVtbl -> get_CurrentDesignGalaxyType(This,pVal) ) 

#define IGalaxyCluster_get_CurrentDesignNode(This,pVal)	\
    ( (This)->lpVtbl -> get_CurrentDesignNode(This,pVal) ) 

#define IGalaxyCluster_CreateGalaxy(This,ParentObj,HostWnd,bstrGalaxyName,pRetGalaxy)	\
    ( (This)->lpVtbl -> CreateGalaxy(This,ParentObj,HostWnd,bstrGalaxyName,pRetGalaxy) ) 

#define IGalaxyCluster_Observe(This,Parent,CtrlName,GalaxyName,bstrKey,bstrXml,ppRetGrid)	\
    ( (This)->lpVtbl -> Observe(This,Parent,CtrlName,GalaxyName,bstrKey,bstrXml,ppRetGrid) ) 

#define IGalaxyCluster_ObserveCtrl(This,Ctrl,bstrKey,bstrXml,ppRetGrid)	\
    ( (This)->lpVtbl -> ObserveCtrl(This,Ctrl,bstrKey,bstrXml,ppRetGrid) ) 

#define IGalaxyCluster_GetGrid(This,bstrGalaxyName,bstrNodeName,pRetNode)	\
    ( (This)->lpVtbl -> GetGrid(This,bstrGalaxyName,bstrNodeName,pRetNode) ) 

#define IGalaxyCluster_GetCtrlInGrid(This,NodeName,CtrlName,ppCtrl)	\
    ( (This)->lpVtbl -> GetCtrlInGrid(This,NodeName,CtrlName,ppCtrl) ) 

#define IGalaxyCluster_GetGalaxyFromCtrl(This,ctrl,ppGalaxy)	\
    ( (This)->lpVtbl -> GetGalaxyFromCtrl(This,ctrl,ppGalaxy) ) 

#define IGalaxyCluster_ConnectHubbleCtrl(This,eventSource)	\
    ( (This)->lpVtbl -> ConnectHubbleCtrl(This,eventSource) ) 

#define IGalaxyCluster_CreateGalaxyWithDefaultNode(This,hGalaxyWnd,bstrGalaxyName,bstrDefaultNodeKey,bstrXml,bSaveToConfig,ppGrid)	\
    ( (This)->lpVtbl -> CreateGalaxyWithDefaultNode(This,hGalaxyWnd,bstrGalaxyName,bstrDefaultNodeKey,bstrXml,bSaveToConfig,ppGrid) ) 

#define IGalaxyCluster_ObserveGalaxys(This,bstrGalaxys,bstrKey,bstrXml,bSaveToConfigFile)	\
    ( (This)->lpVtbl -> ObserveGalaxys(This,bstrGalaxys,bstrKey,bstrXml,bSaveToConfigFile) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGalaxyCluster_INTERFACE_DEFINED__ */


#ifndef __IWorkBenchWindow_INTERFACE_DEFINED__
#define __IWorkBenchWindow_INTERFACE_DEFINED__

/* interface IWorkBenchWindow */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IWorkBenchWindow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060119825D34")
    IWorkBenchWindow : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Ctrl( 
            /* [in] */ VARIANT vIndex,
            /* [retval][out] */ IEclipseCtrl **ppCtrl) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Handle( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HubbleCtrl( 
            LONGLONG hWnd,
            /* [retval][out] */ IEclipseCtrl **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_GalaxyCluster( 
            /* [retval][out] */ IGalaxyCluster **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Galaxy( 
            /* [retval][out] */ IGalaxy **pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Active( void) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE CloseTangramUI( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Observe( 
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ObserveEx( 
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ObserveInView( 
            int nIndex,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppRetGrid) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IWorkBenchWindowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWorkBenchWindow * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWorkBenchWindow * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWorkBenchWindow * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWorkBenchWindow * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWorkBenchWindow * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWorkBenchWindow * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWorkBenchWindow * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Ctrl )( 
            IWorkBenchWindow * This,
            /* [in] */ VARIANT vIndex,
            /* [retval][out] */ IEclipseCtrl **ppCtrl);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IWorkBenchWindow * This,
            /* [retval][out] */ IUnknown **ppVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IWorkBenchWindow * This,
            /* [retval][out] */ long *pCount);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Handle )( 
            IWorkBenchWindow * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HubbleCtrl )( 
            IWorkBenchWindow * This,
            LONGLONG hWnd,
            /* [retval][out] */ IEclipseCtrl **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GalaxyCluster )( 
            IWorkBenchWindow * This,
            /* [retval][out] */ IGalaxyCluster **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Galaxy )( 
            IWorkBenchWindow * This,
            /* [retval][out] */ IGalaxy **pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Active )( 
            IWorkBenchWindow * This);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *CloseTangramUI )( 
            IWorkBenchWindow * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Observe )( 
            IWorkBenchWindow * This,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ObserveEx )( 
            IWorkBenchWindow * This,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ObserveInView )( 
            IWorkBenchWindow * This,
            int nIndex,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppRetGrid);
        
        END_INTERFACE
    } IWorkBenchWindowVtbl;

    interface IWorkBenchWindow
    {
        CONST_VTBL struct IWorkBenchWindowVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWorkBenchWindow_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWorkBenchWindow_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWorkBenchWindow_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWorkBenchWindow_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWorkBenchWindow_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWorkBenchWindow_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWorkBenchWindow_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IWorkBenchWindow_get_Ctrl(This,vIndex,ppCtrl)	\
    ( (This)->lpVtbl -> get_Ctrl(This,vIndex,ppCtrl) ) 

#define IWorkBenchWindow_get__NewEnum(This,ppVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppVal) ) 

#define IWorkBenchWindow_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define IWorkBenchWindow_get_Handle(This,pVal)	\
    ( (This)->lpVtbl -> get_Handle(This,pVal) ) 

#define IWorkBenchWindow_get_HubbleCtrl(This,hWnd,pVal)	\
    ( (This)->lpVtbl -> get_HubbleCtrl(This,hWnd,pVal) ) 

#define IWorkBenchWindow_get_GalaxyCluster(This,pVal)	\
    ( (This)->lpVtbl -> get_GalaxyCluster(This,pVal) ) 

#define IWorkBenchWindow_get_Galaxy(This,pVal)	\
    ( (This)->lpVtbl -> get_Galaxy(This,pVal) ) 

#define IWorkBenchWindow_Active(This)	\
    ( (This)->lpVtbl -> Active(This) ) 

#define IWorkBenchWindow_CloseTangramUI(This)	\
    ( (This)->lpVtbl -> CloseTangramUI(This) ) 

#define IWorkBenchWindow_Observe(This,bstrKey,bstrXml,ppGrid)	\
    ( (This)->lpVtbl -> Observe(This,bstrKey,bstrXml,ppGrid) ) 

#define IWorkBenchWindow_ObserveEx(This,bstrKey,bstrXml,ppGrid)	\
    ( (This)->lpVtbl -> ObserveEx(This,bstrKey,bstrXml,ppGrid) ) 

#define IWorkBenchWindow_ObserveInView(This,nIndex,bstrKey,bstrXml,ppRetGrid)	\
    ( (This)->lpVtbl -> ObserveInView(This,nIndex,bstrKey,bstrXml,ppRetGrid) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWorkBenchWindow_INTERFACE_DEFINED__ */


#ifndef __IBrowser_INTERFACE_DEFINED__
#define __IBrowser_INTERFACE_DEFINED__

/* interface IBrowser */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IBrowser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060120180828")
    IBrowser : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_RemoteGrid( 
            /* [retval][out] */ IGrid **pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_RemoteGrid( 
            /* [in] */ IGrid *newVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE OpenURL( 
            BSTR bstrURL,
            BrowserWndOpenDisposition nDisposition,
            BSTR bstrKey,
            BSTR bstrXml) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddURLs( 
            BSTR bstrURLs) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBrowserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBrowser * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBrowser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBrowser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBrowser * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBrowser * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBrowser * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBrowser * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RemoteGrid )( 
            IBrowser * This,
            /* [retval][out] */ IGrid **pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RemoteGrid )( 
            IBrowser * This,
            /* [in] */ IGrid *newVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *OpenURL )( 
            IBrowser * This,
            BSTR bstrURL,
            BrowserWndOpenDisposition nDisposition,
            BSTR bstrKey,
            BSTR bstrXml);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddURLs )( 
            IBrowser * This,
            BSTR bstrURLs);
        
        END_INTERFACE
    } IBrowserVtbl;

    interface IBrowser
    {
        CONST_VTBL struct IBrowserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBrowser_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBrowser_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBrowser_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBrowser_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBrowser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBrowser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBrowser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBrowser_get_RemoteGrid(This,pVal)	\
    ( (This)->lpVtbl -> get_RemoteGrid(This,pVal) ) 

#define IBrowser_put_RemoteGrid(This,newVal)	\
    ( (This)->lpVtbl -> put_RemoteGrid(This,newVal) ) 

#define IBrowser_OpenURL(This,bstrURL,nDisposition,bstrKey,bstrXml)	\
    ( (This)->lpVtbl -> OpenURL(This,bstrURL,nDisposition,bstrKey,bstrXml) ) 

#define IBrowser_AddURLs(This,bstrURLs)	\
    ( (This)->lpVtbl -> AddURLs(This,bstrURLs) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBrowser_INTERFACE_DEFINED__ */


#ifndef __IWebPage_INTERFACE_DEFINED__
#define __IWebPage_INTERFACE_DEFINED__

/* interface IWebPage */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IWebPage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060120180903")
    IWebPage : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HostWnd( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_HostWnd( 
            /* [in] */ LONGLONG newVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateForm( 
            BSTR bstrKey,
            LONGLONG hParent,
            /* [retval][out] */ IDispatch **pRetForm) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Observe( 
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **pRetGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendXmlMessage( 
            IGrid *sender,
            BSTR bstrXml) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IWebPageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWebPage * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWebPage * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWebPage * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWebPage * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWebPage * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWebPage * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWebPage * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HostWnd )( 
            IWebPage * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HostWnd )( 
            IWebPage * This,
            /* [in] */ LONGLONG newVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateForm )( 
            IWebPage * This,
            BSTR bstrKey,
            LONGLONG hParent,
            /* [retval][out] */ IDispatch **pRetForm);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Observe )( 
            IWebPage * This,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **pRetGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SendXmlMessage )( 
            IWebPage * This,
            IGrid *sender,
            BSTR bstrXml);
        
        END_INTERFACE
    } IWebPageVtbl;

    interface IWebPage
    {
        CONST_VTBL struct IWebPageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWebPage_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWebPage_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWebPage_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWebPage_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWebPage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWebPage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWebPage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IWebPage_get_HostWnd(This,pVal)	\
    ( (This)->lpVtbl -> get_HostWnd(This,pVal) ) 

#define IWebPage_put_HostWnd(This,newVal)	\
    ( (This)->lpVtbl -> put_HostWnd(This,newVal) ) 

#define IWebPage_CreateForm(This,bstrKey,hParent,pRetForm)	\
    ( (This)->lpVtbl -> CreateForm(This,bstrKey,hParent,pRetForm) ) 

#define IWebPage_Observe(This,bstrKey,bstrXml,pRetGrid)	\
    ( (This)->lpVtbl -> Observe(This,bstrKey,bstrXml,pRetGrid) ) 

#define IWebPage_SendXmlMessage(This,sender,bstrXml)	\
    ( (This)->lpVtbl -> SendXmlMessage(This,sender,bstrXml) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWebPage_INTERFACE_DEFINED__ */


#ifndef __IOfficeObject_INTERFACE_DEFINED__
#define __IOfficeObject_INTERFACE_DEFINED__

/* interface IOfficeObject */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IOfficeObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060119650606")
    IOfficeObject : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Show( 
            VARIANT_BOOL bShow) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Observe( 
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnloadHubble( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IOfficeObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOfficeObject * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOfficeObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOfficeObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IOfficeObject * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IOfficeObject * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IOfficeObject * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IOfficeObject * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Show )( 
            IOfficeObject * This,
            VARIANT_BOOL bShow);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Observe )( 
            IOfficeObject * This,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnloadHubble )( 
            IOfficeObject * This);
        
        END_INTERFACE
    } IOfficeObjectVtbl;

    interface IOfficeObject
    {
        CONST_VTBL struct IOfficeObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOfficeObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOfficeObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOfficeObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOfficeObject_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IOfficeObject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IOfficeObject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IOfficeObject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IOfficeObject_Show(This,bShow)	\
    ( (This)->lpVtbl -> Show(This,bShow) ) 

#define IOfficeObject_Observe(This,bstrKey,bstrXml,ppGrid)	\
    ( (This)->lpVtbl -> Observe(This,bstrKey,bstrXml,ppGrid) ) 

#define IOfficeObject_UnloadHubble(This)	\
    ( (This)->lpVtbl -> UnloadHubble(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOfficeObject_INTERFACE_DEFINED__ */


#ifndef __IHubbleCtrl_INTERFACE_DEFINED__
#define __IHubbleCtrl_INTERFACE_DEFINED__

/* interface IHubbleCtrl */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHubbleCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-0601198231DC")
    IHubbleCtrl : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HWND( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Hubble( 
            /* [retval][out] */ IHubble **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_tag( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_tag( 
            /* [in] */ VARIANT newVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IHubbleCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHubbleCtrl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHubbleCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHubbleCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHubbleCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHubbleCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHubbleCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHubbleCtrl * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWND )( 
            IHubbleCtrl * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Hubble )( 
            IHubbleCtrl * This,
            /* [retval][out] */ IHubble **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_tag )( 
            IHubbleCtrl * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_tag )( 
            IHubbleCtrl * This,
            /* [in] */ VARIANT newVal);
        
        END_INTERFACE
    } IHubbleCtrlVtbl;

    interface IHubbleCtrl
    {
        CONST_VTBL struct IHubbleCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHubbleCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHubbleCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHubbleCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHubbleCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IHubbleCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IHubbleCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IHubbleCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IHubbleCtrl_get_HWND(This,pVal)	\
    ( (This)->lpVtbl -> get_HWND(This,pVal) ) 

#define IHubbleCtrl_get_Hubble(This,pVal)	\
    ( (This)->lpVtbl -> get_Hubble(This,pVal) ) 

#define IHubbleCtrl_get_tag(This,pVal)	\
    ( (This)->lpVtbl -> get_tag(This,pVal) ) 

#define IHubbleCtrl_put_tag(This,newVal)	\
    ( (This)->lpVtbl -> put_tag(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHubbleCtrl_INTERFACE_DEFINED__ */


#ifndef __IHubbleAppCtrl_INTERFACE_DEFINED__
#define __IHubbleAppCtrl_INTERFACE_DEFINED__

/* interface IHubbleAppCtrl */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHubbleAppCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060120161001")
    IHubbleAppCtrl : public IHubbleCtrl
    {
    public:
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_AppCtrl( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IHubbleAppCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHubbleAppCtrl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHubbleAppCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHubbleAppCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHubbleAppCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHubbleAppCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHubbleAppCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHubbleAppCtrl * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWND )( 
            IHubbleAppCtrl * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Hubble )( 
            IHubbleAppCtrl * This,
            /* [retval][out] */ IHubble **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_tag )( 
            IHubbleAppCtrl * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_tag )( 
            IHubbleAppCtrl * This,
            /* [in] */ VARIANT newVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AppCtrl )( 
            IHubbleAppCtrl * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        END_INTERFACE
    } IHubbleAppCtrlVtbl;

    interface IHubbleAppCtrl
    {
        CONST_VTBL struct IHubbleAppCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHubbleAppCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHubbleAppCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHubbleAppCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHubbleAppCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IHubbleAppCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IHubbleAppCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IHubbleAppCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IHubbleAppCtrl_get_HWND(This,pVal)	\
    ( (This)->lpVtbl -> get_HWND(This,pVal) ) 

#define IHubbleAppCtrl_get_Hubble(This,pVal)	\
    ( (This)->lpVtbl -> get_Hubble(This,pVal) ) 

#define IHubbleAppCtrl_get_tag(This,pVal)	\
    ( (This)->lpVtbl -> get_tag(This,pVal) ) 

#define IHubbleAppCtrl_put_tag(This,newVal)	\
    ( (This)->lpVtbl -> put_tag(This,newVal) ) 


#define IHubbleAppCtrl_put_AppCtrl(This,newVal)	\
    ( (This)->lpVtbl -> put_AppCtrl(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHubbleAppCtrl_INTERFACE_DEFINED__ */


#ifndef __IEclipseCtrl_INTERFACE_DEFINED__
#define __IEclipseCtrl_INTERFACE_DEFINED__

/* interface IEclipseCtrl */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IEclipseCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060120160912")
    IEclipseCtrl : public IHubbleCtrl
    {
    public:
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Handle( 
            BSTR bstrHandleName,
            /* [in] */ LONGLONG newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_EclipseViewHandle( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_GalaxyCluster( 
            /* [retval][out] */ IGalaxyCluster **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_WorkBenchWindow( 
            /* [retval][out] */ IWorkBenchWindow **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_TopGalaxyCluster( 
            /* [retval][out] */ IGalaxyCluster **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_TopGalaxy( 
            /* [retval][out] */ IGalaxy **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveTopGrid( 
            /* [retval][out] */ IGrid **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AppKeyValue( 
            BSTR bstrKey,
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AppKeyValue( 
            BSTR bstrKey,
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Observe( 
            BSTR bstrGalaxyName,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ObserveEx( 
            BSTR bstrGalaxyName,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppGrid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE InitCtrl( 
            BSTR bstrXml) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IEclipseCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEclipseCtrl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEclipseCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEclipseCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEclipseCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEclipseCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEclipseCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEclipseCtrl * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWND )( 
            IEclipseCtrl * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Hubble )( 
            IEclipseCtrl * This,
            /* [retval][out] */ IHubble **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_tag )( 
            IEclipseCtrl * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_tag )( 
            IEclipseCtrl * This,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Handle )( 
            IEclipseCtrl * This,
            BSTR bstrHandleName,
            /* [in] */ LONGLONG newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_EclipseViewHandle )( 
            IEclipseCtrl * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GalaxyCluster )( 
            IEclipseCtrl * This,
            /* [retval][out] */ IGalaxyCluster **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WorkBenchWindow )( 
            IEclipseCtrl * This,
            /* [retval][out] */ IWorkBenchWindow **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TopGalaxyCluster )( 
            IEclipseCtrl * This,
            /* [retval][out] */ IGalaxyCluster **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TopGalaxy )( 
            IEclipseCtrl * This,
            /* [retval][out] */ IGalaxy **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveTopGrid )( 
            IEclipseCtrl * This,
            /* [retval][out] */ IGrid **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AppKeyValue )( 
            IEclipseCtrl * This,
            BSTR bstrKey,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AppKeyValue )( 
            IEclipseCtrl * This,
            BSTR bstrKey,
            /* [in] */ VARIANT newVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Observe )( 
            IEclipseCtrl * This,
            BSTR bstrGalaxyName,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ObserveEx )( 
            IEclipseCtrl * This,
            BSTR bstrGalaxyName,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IGrid **ppGrid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *InitCtrl )( 
            IEclipseCtrl * This,
            BSTR bstrXml);
        
        END_INTERFACE
    } IEclipseCtrlVtbl;

    interface IEclipseCtrl
    {
        CONST_VTBL struct IEclipseCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEclipseCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEclipseCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEclipseCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEclipseCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IEclipseCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IEclipseCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IEclipseCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IEclipseCtrl_get_HWND(This,pVal)	\
    ( (This)->lpVtbl -> get_HWND(This,pVal) ) 

#define IEclipseCtrl_get_Hubble(This,pVal)	\
    ( (This)->lpVtbl -> get_Hubble(This,pVal) ) 

#define IEclipseCtrl_get_tag(This,pVal)	\
    ( (This)->lpVtbl -> get_tag(This,pVal) ) 

#define IEclipseCtrl_put_tag(This,newVal)	\
    ( (This)->lpVtbl -> put_tag(This,newVal) ) 


#define IEclipseCtrl_put_Handle(This,bstrHandleName,newVal)	\
    ( (This)->lpVtbl -> put_Handle(This,bstrHandleName,newVal) ) 

#define IEclipseCtrl_get_EclipseViewHandle(This,pVal)	\
    ( (This)->lpVtbl -> get_EclipseViewHandle(This,pVal) ) 

#define IEclipseCtrl_get_GalaxyCluster(This,pVal)	\
    ( (This)->lpVtbl -> get_GalaxyCluster(This,pVal) ) 

#define IEclipseCtrl_get_WorkBenchWindow(This,pVal)	\
    ( (This)->lpVtbl -> get_WorkBenchWindow(This,pVal) ) 

#define IEclipseCtrl_get_TopGalaxyCluster(This,pVal)	\
    ( (This)->lpVtbl -> get_TopGalaxyCluster(This,pVal) ) 

#define IEclipseCtrl_get_TopGalaxy(This,pVal)	\
    ( (This)->lpVtbl -> get_TopGalaxy(This,pVal) ) 

#define IEclipseCtrl_get_ActiveTopGrid(This,pVal)	\
    ( (This)->lpVtbl -> get_ActiveTopGrid(This,pVal) ) 

#define IEclipseCtrl_get_AppKeyValue(This,bstrKey,pVal)	\
    ( (This)->lpVtbl -> get_AppKeyValue(This,bstrKey,pVal) ) 

#define IEclipseCtrl_put_AppKeyValue(This,bstrKey,newVal)	\
    ( (This)->lpVtbl -> put_AppKeyValue(This,bstrKey,newVal) ) 

#define IEclipseCtrl_Observe(This,bstrGalaxyName,bstrKey,bstrXml,ppGrid)	\
    ( (This)->lpVtbl -> Observe(This,bstrGalaxyName,bstrKey,bstrXml,ppGrid) ) 

#define IEclipseCtrl_ObserveEx(This,bstrGalaxyName,bstrKey,bstrXml,ppGrid)	\
    ( (This)->lpVtbl -> ObserveEx(This,bstrGalaxyName,bstrKey,bstrXml,ppGrid) ) 

#define IEclipseCtrl_InitCtrl(This,bstrXml)	\
    ( (This)->lpVtbl -> InitCtrl(This,bstrXml) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEclipseCtrl_INTERFACE_DEFINED__ */



#ifndef __Universe_LIBRARY_DEFINED__
#define __Universe_LIBRARY_DEFINED__

/* library Universe */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_Universe;

#ifndef ___IHubble_DISPINTERFACE_DEFINED__
#define ___IHubble_DISPINTERFACE_DEFINED__

/* dispinterface _IHubble */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IHubble;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("19631222-1992-0612-1965-060119821002")
    _IHubble : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IHubbleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IHubble * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IHubble * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IHubble * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IHubble * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IHubble * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IHubble * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IHubble * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _IHubbleVtbl;

    interface _IHubble
    {
        CONST_VTBL struct _IHubbleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IHubble_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IHubble_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IHubble_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IHubble_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IHubble_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IHubble_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IHubble_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IHubble_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Universe;

#ifdef __cplusplus

class DECLSPEC_UUID("19631222-1992-0612-1965-060119990606")
Universe;
#endif

#ifndef ___IGridEvents_DISPINTERFACE_DEFINED__
#define ___IGridEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IGridEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IGridEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("19631222-1992-0612-1965-060119821001")
    _IGridEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IGridEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IGridEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IGridEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IGridEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IGridEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IGridEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IGridEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IGridEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _IGridEventsVtbl;

    interface _IGridEvents
    {
        CONST_VTBL struct _IGridEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IGridEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IGridEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IGridEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IGridEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IGridEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IGridEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IGridEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IGridEvents_DISPINTERFACE_DEFINED__ */


#ifndef ___IHubbleObjEvents_DISPINTERFACE_DEFINED__
#define ___IHubbleObjEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IHubbleObjEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IHubbleObjEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("19631222-1992-0612-1965-060119821992")
    _IHubbleObjEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IHubbleObjEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IHubbleObjEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IHubbleObjEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IHubbleObjEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IHubbleObjEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IHubbleObjEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IHubbleObjEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IHubbleObjEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _IHubbleObjEventsVtbl;

    interface _IHubbleObjEvents
    {
        CONST_VTBL struct _IHubbleObjEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IHubbleObjEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IHubbleObjEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IHubbleObjEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IHubbleObjEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IHubbleObjEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IHubbleObjEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IHubbleObjEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IHubbleObjEvents_DISPINTERFACE_DEFINED__ */


#ifndef ___IHubbleAppEvents_DISPINTERFACE_DEFINED__
#define ___IHubbleAppEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IHubbleAppEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IHubbleAppEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("19631222-1992-0612-1965-060120160930")
    _IHubbleAppEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IHubbleAppEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IHubbleAppEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IHubbleAppEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IHubbleAppEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IHubbleAppEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IHubbleAppEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IHubbleAppEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IHubbleAppEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _IHubbleAppEventsVtbl;

    interface _IHubbleAppEvents
    {
        CONST_VTBL struct _IHubbleAppEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IHubbleAppEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IHubbleAppEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IHubbleAppEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IHubbleAppEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IHubbleAppEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IHubbleAppEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IHubbleAppEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IHubbleAppEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_HubbleCtrl;

#ifdef __cplusplus

class DECLSPEC_UUID("19631222-1992-0612-1965-060120071001")
HubbleCtrl;
#endif

EXTERN_C const CLSID CLSID_UniverseExtender;

#ifdef __cplusplus

class DECLSPEC_UUID("19631222-1992-0612-1965-060100000000")
UniverseExtender;
#endif
#endif /* __Universe_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize64(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal64(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal64(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree64(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


