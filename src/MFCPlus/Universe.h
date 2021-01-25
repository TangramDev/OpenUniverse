

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

#ifndef __ICosmosExtender_FWD_DEFINED__
#define __ICosmosExtender_FWD_DEFINED__
typedef interface ICosmosExtender ICosmosExtender;

#endif 	/* __ICosmosExtender_FWD_DEFINED__ */


#ifndef __IEclipseExtender_FWD_DEFINED__
#define __IEclipseExtender_FWD_DEFINED__
typedef interface IEclipseExtender IEclipseExtender;

#endif 	/* __IEclipseExtender_FWD_DEFINED__ */


#ifndef __IOfficeExtender_FWD_DEFINED__
#define __IOfficeExtender_FWD_DEFINED__
typedef interface IOfficeExtender IOfficeExtender;

#endif 	/* __IOfficeExtender_FWD_DEFINED__ */


#ifndef __ICosmosTreeNode_FWD_DEFINED__
#define __ICosmosTreeNode_FWD_DEFINED__
typedef interface ICosmosTreeNode ICosmosTreeNode;

#endif 	/* __ICosmosTreeNode_FWD_DEFINED__ */


#ifndef __ICosmosTreeViewCallBack_FWD_DEFINED__
#define __ICosmosTreeViewCallBack_FWD_DEFINED__
typedef interface ICosmosTreeViewCallBack ICosmosTreeViewCallBack;

#endif 	/* __ICosmosTreeViewCallBack_FWD_DEFINED__ */


#ifndef __ICosmosTreeView_FWD_DEFINED__
#define __ICosmosTreeView_FWD_DEFINED__
typedef interface ICosmosTreeView ICosmosTreeView;

#endif 	/* __ICosmosTreeView_FWD_DEFINED__ */


#ifndef __ICosmosEventObj_FWD_DEFINED__
#define __ICosmosEventObj_FWD_DEFINED__
typedef interface ICosmosEventObj ICosmosEventObj;

#endif 	/* __ICosmosEventObj_FWD_DEFINED__ */


#ifndef __IXobj_FWD_DEFINED__
#define __IXobj_FWD_DEFINED__
typedef interface IXobj IXobj;

#endif 	/* __IXobj_FWD_DEFINED__ */


#ifndef __IAppExtender_FWD_DEFINED__
#define __IAppExtender_FWD_DEFINED__
typedef interface IAppExtender IAppExtender;

#endif 	/* __IAppExtender_FWD_DEFINED__ */


#ifndef __ICosmos_FWD_DEFINED__
#define __ICosmos_FWD_DEFINED__
typedef interface ICosmos ICosmos;

#endif 	/* __ICosmos_FWD_DEFINED__ */


#ifndef __IGalaxy_FWD_DEFINED__
#define __IGalaxy_FWD_DEFINED__
typedef interface IGalaxy IGalaxy;

#endif 	/* __IGalaxy_FWD_DEFINED__ */


#ifndef __IXobjCollection_FWD_DEFINED__
#define __IXobjCollection_FWD_DEFINED__
typedef interface IXobjCollection IXobjCollection;

#endif 	/* __IXobjCollection_FWD_DEFINED__ */


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


#ifndef __ICosmosCtrl_FWD_DEFINED__
#define __ICosmosCtrl_FWD_DEFINED__
typedef interface ICosmosCtrl ICosmosCtrl;

#endif 	/* __ICosmosCtrl_FWD_DEFINED__ */


#ifndef __ICosmosAppCtrl_FWD_DEFINED__
#define __ICosmosAppCtrl_FWD_DEFINED__
typedef interface ICosmosAppCtrl ICosmosAppCtrl;

#endif 	/* __ICosmosAppCtrl_FWD_DEFINED__ */


#ifndef __IEclipseCtrl_FWD_DEFINED__
#define __IEclipseCtrl_FWD_DEFINED__
typedef interface IEclipseCtrl IEclipseCtrl;

#endif 	/* __IEclipseCtrl_FWD_DEFINED__ */


#ifndef ___ICosmos_FWD_DEFINED__
#define ___ICosmos_FWD_DEFINED__
typedef interface _ICosmos _ICosmos;

#endif 	/* ___ICosmos_FWD_DEFINED__ */


#ifndef __Cosmos_FWD_DEFINED__
#define __Cosmos_FWD_DEFINED__

#ifdef __cplusplus
typedef class Cosmos Cosmos;
#else
typedef struct Cosmos Cosmos;
#endif /* __cplusplus */

#endif 	/* __Cosmos_FWD_DEFINED__ */


#ifndef ___IXobjEvents_FWD_DEFINED__
#define ___IXobjEvents_FWD_DEFINED__
typedef interface _IXobjEvents _IXobjEvents;

#endif 	/* ___IXobjEvents_FWD_DEFINED__ */


#ifndef ___ICosmosObjEvents_FWD_DEFINED__
#define ___ICosmosObjEvents_FWD_DEFINED__
typedef interface _ICosmosObjEvents _ICosmosObjEvents;

#endif 	/* ___ICosmosObjEvents_FWD_DEFINED__ */


#ifndef ___ICosmosAppEvents_FWD_DEFINED__
#define ___ICosmosAppEvents_FWD_DEFINED__
typedef interface _ICosmosAppEvents _ICosmosAppEvents;

#endif 	/* ___ICosmosAppEvents_FWD_DEFINED__ */


#ifndef __CosmosCtrl_FWD_DEFINED__
#define __CosmosCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class CosmosCtrl CosmosCtrl;
#else
typedef struct CosmosCtrl CosmosCtrl;
#endif /* __cplusplus */

#endif 	/* __CosmosCtrl_FWD_DEFINED__ */


#ifndef __CosmosExtender_FWD_DEFINED__
#define __CosmosExtender_FWD_DEFINED__

#ifdef __cplusplus
typedef class CosmosExtender CosmosExtender;
#else
typedef struct CosmosExtender CosmosExtender;
#endif /* __cplusplus */

#endif 	/* __CosmosExtender_FWD_DEFINED__ */


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
        CosmosDocEvent	= 0x1,
        TangramNodeAllChildNode	= 0x2,
        CosmosDocAllGalaxyAllChildNode	= 0x3,
        CosmosDocAllGalaxyAllTopXobj	= 0x4,
        TangramGalaxyAllTopXobjAllChildNode	= 0x5,
        TangramGalaxyAllTopXobj	= 0x6,
        GalaxyClusterAllGalaxyAllTopXobjAllChildNode	= 0x7,
        GalaxyClusterAllGalaxyAllTopXobj	= 0x8,
        GalaxyClusterCtrlBarGalaxyAllTopXobjAllChildNode	= 0x9,
        GalaxyClusterCtrlBarGalaxyAllTopXobj	= 0xa,
        GalaxyClusterNotCtrlBarGalaxyAllTopXobjAllChildNode	= 0xb,
        GalaxyClusterNotCtrlBarGalaxyAllTopXobj	= 0xc,
        CosmosDocAllCtrlBarGalaxyAllChildNode	= 0xd,
        CosmosDocAllCtrlBarGalaxy	= 0xe
    } 	ObjEventType;

typedef /* [helpstring] */ 
enum XobjType
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
    } 	XobjType;



extern RPC_IF_HANDLE __MIDL_itf_Universe_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Universe_0000_0000_v0_0_s_ifspec;

#ifndef __ICosmosExtender_INTERFACE_DEFINED__
#define __ICosmosExtender_INTERFACE_DEFINED__

/* interface ICosmosExtender */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICosmosExtender;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060120160101")
    ICosmosExtender : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveWorkBenchWindow( 
            BSTR bstrID,
            /* [retval][out] */ IWorkBenchWindow **pVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICosmosExtenderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICosmosExtender * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICosmosExtender * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICosmosExtender * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICosmosExtender * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICosmosExtender * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICosmosExtender * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICosmosExtender * This,
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
            ICosmosExtender * This);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveWorkBenchWindow )( 
            ICosmosExtender * This,
            BSTR bstrID,
            /* [retval][out] */ IWorkBenchWindow **pVal);
        
        END_INTERFACE
    } ICosmosExtenderVtbl;

    interface ICosmosExtender
    {
        CONST_VTBL struct ICosmosExtenderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICosmosExtender_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICosmosExtender_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICosmosExtender_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICosmosExtender_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICosmosExtender_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICosmosExtender_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICosmosExtender_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICosmosExtender_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#define ICosmosExtender_get_ActiveWorkBenchWindow(This,bstrID,pVal)	\
    ( (This)->lpVtbl -> get_ActiveWorkBenchWindow(This,bstrID,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICosmosExtender_INTERFACE_DEFINED__ */


#ifndef __IEclipseExtender_INTERFACE_DEFINED__
#define __IEclipseExtender_INTERFACE_DEFINED__

/* interface IEclipseExtender */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IEclipseExtender;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060120160918")
    IEclipseExtender : public ICosmosExtender
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
    IOfficeExtender : public ICosmosExtender
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
            /* [retval][out] */ IXobj **ppXobj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGalaxyFromVBAForm( 
            IDispatch *pForm,
            /* [retval][out] */ IGalaxy **ppGalaxy) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetActiveTopXobj( 
            IDispatch *pForm,
            /* [retval][out] */ IXobj **WndXobj) = 0;
        
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
            /* [retval][out] */ IXobj **ppXobj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGalaxyFromVBAForm )( 
            IOfficeExtender * This,
            IDispatch *pForm,
            /* [retval][out] */ IGalaxy **ppGalaxy);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetActiveTopXobj )( 
            IOfficeExtender * This,
            IDispatch *pForm,
            /* [retval][out] */ IXobj **WndXobj);
        
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

#define IOfficeExtender_InitVBAForm(This,newVal,nStyle,bstrXml,ppXobj)	\
    ( (This)->lpVtbl -> InitVBAForm(This,newVal,nStyle,bstrXml,ppXobj) ) 

#define IOfficeExtender_GetGalaxyFromVBAForm(This,pForm,ppGalaxy)	\
    ( (This)->lpVtbl -> GetGalaxyFromVBAForm(This,pForm,ppGalaxy) ) 

#define IOfficeExtender_GetActiveTopXobj(This,pForm,WndXobj)	\
    ( (This)->lpVtbl -> GetActiveTopXobj(This,pForm,WndXobj) ) 

#define IOfficeExtender_GetObjectFromWnd(This,hWnd,ppObjFromWnd)	\
    ( (This)->lpVtbl -> GetObjectFromWnd(This,hWnd,ppObjFromWnd) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOfficeExtender_INTERFACE_DEFINED__ */


#ifndef __ICosmosTreeNode_INTERFACE_DEFINED__
#define __ICosmosTreeNode_INTERFACE_DEFINED__

/* interface ICosmosTreeNode */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICosmosTreeNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-06011982C951")
    ICosmosTreeNode : public IDispatch
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct ICosmosTreeNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICosmosTreeNode * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICosmosTreeNode * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICosmosTreeNode * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICosmosTreeNode * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICosmosTreeNode * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICosmosTreeNode * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICosmosTreeNode * This,
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
    } ICosmosTreeNodeVtbl;

    interface ICosmosTreeNode
    {
        CONST_VTBL struct ICosmosTreeNodeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICosmosTreeNode_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICosmosTreeNode_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICosmosTreeNode_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICosmosTreeNode_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICosmosTreeNode_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICosmosTreeNode_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICosmosTreeNode_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICosmosTreeNode_INTERFACE_DEFINED__ */


#ifndef __ICosmosTreeViewCallBack_INTERFACE_DEFINED__
#define __ICosmosTreeViewCallBack_INTERFACE_DEFINED__

/* interface ICosmosTreeViewCallBack */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICosmosTreeViewCallBack;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-0601198283A6")
    ICosmosTreeViewCallBack : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_WndXobj( 
            IXobj *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Pages( 
            long *retVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnInitTreeView( 
            ICosmosTreeView *pTangramTreeView,
            BSTR bstrXml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnClick( 
            BSTR bstrXml,
            BSTR bstrXmlData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnInit( 
            BSTR bstrXml) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnNewPage( 
            int nNewPage) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CosmosAction( 
            BSTR bstrXml,
            /* [retval][out] */ BSTR *bstrRetXml) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICosmosTreeViewCallBackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICosmosTreeViewCallBack * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICosmosTreeViewCallBack * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICosmosTreeViewCallBack * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICosmosTreeViewCallBack * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICosmosTreeViewCallBack * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICosmosTreeViewCallBack * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICosmosTreeViewCallBack * This,
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
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_WndXobj )( 
            ICosmosTreeViewCallBack * This,
            IXobj *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Pages )( 
            ICosmosTreeViewCallBack * This,
            long *retVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnInitTreeView )( 
            ICosmosTreeViewCallBack * This,
            ICosmosTreeView *pTangramTreeView,
            BSTR bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnClick )( 
            ICosmosTreeViewCallBack * This,
            BSTR bstrXml,
            BSTR bstrXmlData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnInit )( 
            ICosmosTreeViewCallBack * This,
            BSTR bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnNewPage )( 
            ICosmosTreeViewCallBack * This,
            int nNewPage);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CosmosAction )( 
            ICosmosTreeViewCallBack * This,
            BSTR bstrXml,
            /* [retval][out] */ BSTR *bstrRetXml);
        
        END_INTERFACE
    } ICosmosTreeViewCallBackVtbl;

    interface ICosmosTreeViewCallBack
    {
        CONST_VTBL struct ICosmosTreeViewCallBackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICosmosTreeViewCallBack_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICosmosTreeViewCallBack_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICosmosTreeViewCallBack_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICosmosTreeViewCallBack_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICosmosTreeViewCallBack_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICosmosTreeViewCallBack_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICosmosTreeViewCallBack_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICosmosTreeViewCallBack_put_WndXobj(This,newVal)	\
    ( (This)->lpVtbl -> put_WndXobj(This,newVal) ) 

#define ICosmosTreeViewCallBack_get_Pages(This,retVal)	\
    ( (This)->lpVtbl -> get_Pages(This,retVal) ) 

#define ICosmosTreeViewCallBack_OnInitTreeView(This,pTangramTreeView,bstrXml)	\
    ( (This)->lpVtbl -> OnInitTreeView(This,pTangramTreeView,bstrXml) ) 

#define ICosmosTreeViewCallBack_OnClick(This,bstrXml,bstrXmlData)	\
    ( (This)->lpVtbl -> OnClick(This,bstrXml,bstrXmlData) ) 

#define ICosmosTreeViewCallBack_OnInit(This,bstrXml)	\
    ( (This)->lpVtbl -> OnInit(This,bstrXml) ) 

#define ICosmosTreeViewCallBack_OnNewPage(This,nNewPage)	\
    ( (This)->lpVtbl -> OnNewPage(This,nNewPage) ) 

#define ICosmosTreeViewCallBack_CosmosAction(This,bstrXml,bstrRetXml)	\
    ( (This)->lpVtbl -> CosmosAction(This,bstrXml,bstrRetXml) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICosmosTreeViewCallBack_INTERFACE_DEFINED__ */


#ifndef __ICosmosTreeView_INTERFACE_DEFINED__
#define __ICosmosTreeView_INTERFACE_DEFINED__

/* interface ICosmosTreeView */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICosmosTreeView;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-0601198283A5")
    ICosmosTreeView : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TangramTreeViewCallBack( 
            BSTR bstrKey,
            /* [in] */ ICosmosTreeViewCallBack *newVal) = 0;
        
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

    typedef struct ICosmosTreeViewVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICosmosTreeView * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICosmosTreeView * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICosmosTreeView * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICosmosTreeView * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICosmosTreeView * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICosmosTreeView * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICosmosTreeView * This,
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
            ICosmosTreeView * This,
            BSTR bstrKey,
            /* [in] */ ICosmosTreeViewCallBack *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FirstRoot )( 
            ICosmosTreeView * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddTreeNode )( 
            ICosmosTreeView * This,
            long hItem,
            BSTR bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InsertNode )( 
            ICosmosTreeView * This,
            BSTR bstrXml,
            /* [retval][out] */ long *hItem);
        
        END_INTERFACE
    } ICosmosTreeViewVtbl;

    interface ICosmosTreeView
    {
        CONST_VTBL struct ICosmosTreeViewVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICosmosTreeView_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICosmosTreeView_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICosmosTreeView_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICosmosTreeView_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICosmosTreeView_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICosmosTreeView_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICosmosTreeView_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICosmosTreeView_put_TangramTreeViewCallBack(This,bstrKey,newVal)	\
    ( (This)->lpVtbl -> put_TangramTreeViewCallBack(This,bstrKey,newVal) ) 

#define ICosmosTreeView_get_FirstRoot(This,pVal)	\
    ( (This)->lpVtbl -> get_FirstRoot(This,pVal) ) 

#define ICosmosTreeView_AddTreeNode(This,hItem,bstrXml)	\
    ( (This)->lpVtbl -> AddTreeNode(This,hItem,bstrXml) ) 

#define ICosmosTreeView_InsertNode(This,bstrXml,hItem)	\
    ( (This)->lpVtbl -> InsertNode(This,bstrXml,hItem) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICosmosTreeView_INTERFACE_DEFINED__ */


#ifndef __ICosmosEventObj_INTERFACE_DEFINED__
#define __ICosmosEventObj_INTERFACE_DEFINED__

/* interface ICosmosEventObj */
/* [unique][helpstring][nonextensible][hidden][dual][uuid][object] */ 


EXTERN_C const IID IID_ICosmosEventObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060120160928")
    ICosmosEventObj : public IDispatch
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

    typedef struct ICosmosEventObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICosmosEventObj * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICosmosEventObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICosmosEventObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICosmosEventObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICosmosEventObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICosmosEventObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICosmosEventObj * This,
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
            ICosmosEventObj * This,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_eventSource )( 
            ICosmosEventObj * This,
            /* [in] */ IDispatch *eventSource);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Index )( 
            ICosmosEventObj * This,
            /* [retval][out] */ int *nVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Index )( 
            ICosmosEventObj * This,
            int newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_EventName )( 
            ICosmosEventObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_EventName )( 
            ICosmosEventObj * This,
            BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Object )( 
            ICosmosEventObj * This,
            int nIndex,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Object )( 
            ICosmosEventObj * This,
            int nIndex,
            IDispatch *newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            ICosmosEventObj * This,
            int nIndex,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Value )( 
            ICosmosEventObj * This,
            int nIndex,
            VARIANT newVal);
        
        END_INTERFACE
    } ICosmosEventObjVtbl;

    interface ICosmosEventObj
    {
        CONST_VTBL struct ICosmosEventObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICosmosEventObj_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICosmosEventObj_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICosmosEventObj_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICosmosEventObj_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICosmosEventObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICosmosEventObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICosmosEventObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICosmosEventObj_get_eventSource(This,pVal)	\
    ( (This)->lpVtbl -> get_eventSource(This,pVal) ) 

#define ICosmosEventObj_put_eventSource(This,eventSource)	\
    ( (This)->lpVtbl -> put_eventSource(This,eventSource) ) 

#define ICosmosEventObj_get_Index(This,nVal)	\
    ( (This)->lpVtbl -> get_Index(This,nVal) ) 

#define ICosmosEventObj_put_Index(This,newVal)	\
    ( (This)->lpVtbl -> put_Index(This,newVal) ) 

#define ICosmosEventObj_get_EventName(This,pVal)	\
    ( (This)->lpVtbl -> get_EventName(This,pVal) ) 

#define ICosmosEventObj_put_EventName(This,newVal)	\
    ( (This)->lpVtbl -> put_EventName(This,newVal) ) 

#define ICosmosEventObj_get_Object(This,nIndex,pVal)	\
    ( (This)->lpVtbl -> get_Object(This,nIndex,pVal) ) 

#define ICosmosEventObj_put_Object(This,nIndex,newVal)	\
    ( (This)->lpVtbl -> put_Object(This,nIndex,newVal) ) 

#define ICosmosEventObj_get_Value(This,nIndex,pVal)	\
    ( (This)->lpVtbl -> get_Value(This,nIndex,pVal) ) 

#define ICosmosEventObj_put_Value(This,nIndex,newVal)	\
    ( (This)->lpVtbl -> put_Value(This,nIndex,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICosmosEventObj_INTERFACE_DEFINED__ */


#ifndef __IXobj_INTERFACE_DEFINED__
#define __IXobj_INTERFACE_DEFINED__

/* interface IXobj */
/* [object][unique][helpstring][uuid] */ 


EXTERN_C const IID IID_IXobj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060119820004")
    IXobj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ChildNodes( 
            /* [retval][out] */ IXobjCollection **ppXobjColletion) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Rows( 
            /* [retval][out] */ long *nRows) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Cols( 
            /* [retval][out] */ long *nCols) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Row( 
            /* [retval][out] */ long *nRow) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Col( 
            /* [retval][out] */ long *nCol) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_XobjType( 
            /* [retval][out] */ XobjType *nType) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ParentXobj( 
            /* [retval][out] */ IXobj **ppXobj) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_URL( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_URL( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_GalaxyCluster( 
            /* [retval][out] */ IGalaxyCluster **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RootXobj( 
            /* [retval][out] */ IXobj **ppXobj) = 0;
        
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
            /* [retval][out] */ IXobjCollection **ppXobjColletion) = 0;
        
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
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HostXobj( 
            /* [retval][out] */ IXobj **pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_HostXobj( 
            /* [in] */ IXobj *newVal) = 0;
        
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
            IXobj *pXobj) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetXobjs( 
            /* [in] */ BSTR bstrName,
            /* [out] */ IXobj **ppXobj,
            /* [out] */ IXobjCollection **ppXobjs,
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetXobj( 
            /* [in] */ long nRow,
            /* [in] */ long nCol,
            /* [retval][out] */ IXobj **ppXobj) = 0;
        
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
            /* [retval][out] */ IXobj **ppRetXobj) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadXML( 
            int nType,
            BSTR bstrXML) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ObserveEx( 
            int nRow,
            int nCol,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IXobj **ppRetXobj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetXobjByName( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IXobjCollection **pVal) = 0;
        
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
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetChildXobjByName( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IXobj **pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetUIScript( 
            /* [in] */ BSTR bstrCtrlName,
            /* [retval][out] */ BSTR *bstrVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXobjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IXobj * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IXobj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IXobj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IXobj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IXobj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IXobj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IXobj * This,
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
            IXobj * This,
            /* [retval][out] */ IXobjCollection **ppXobjColletion);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Rows )( 
            IXobj * This,
            /* [retval][out] */ long *nRows);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Cols )( 
            IXobj * This,
            /* [retval][out] */ long *nCols);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Row )( 
            IXobj * This,
            /* [retval][out] */ long *nRow);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Col )( 
            IXobj * This,
            /* [retval][out] */ long *nCol);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_XobjType )( 
            IXobj * This,
            /* [retval][out] */ XobjType *nType);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ParentXobj )( 
            IXobj * This,
            /* [retval][out] */ IXobj **ppXobj);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_URL )( 
            IXobj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_URL )( 
            IXobj * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GalaxyCluster )( 
            IXobj * This,
            /* [retval][out] */ IGalaxyCluster **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RootXobj )( 
            IXobj * This,
            /* [retval][out] */ IXobj **ppXobj);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_XObject )( 
            IXobj * This,
            /* [retval][out] */ VARIANT *pVar);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AxPlugIn )( 
            IXobj * This,
            /* [in] */ BSTR bstrPlugInName,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Caption )( 
            IXobj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Caption )( 
            IXobj * This,
            /* [in] */ BSTR bstrCaption);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IXobj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            IXobj * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Objects )( 
            IXobj * This,
            /* [in] */ long nType,
            /* [retval][out] */ IXobjCollection **ppXobjColletion);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Handle )( 
            IXobj * This,
            /* [retval][out] */ LONGLONG *hWnd);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Attribute )( 
            IXobj * This,
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Attribute )( 
            IXobj * This,
            /* [in] */ BSTR bstrKey,
            /* [in] */ BSTR bstrVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Tag )( 
            IXobj * This,
            /* [retval][out] */ VARIANT *pVar);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Tag )( 
            IXobj * This,
            /* [in] */ VARIANT vVar);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_OuterXml )( 
            IXobj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Key )( 
            IXobj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DockObj )( 
            IXobj * This,
            BSTR bstrName,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DockObj )( 
            IXobj * This,
            BSTR bstrName,
            /* [in] */ LONGLONG newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NameAtWindowPage )( 
            IXobj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IXobj * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IXobj * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Galaxy )( 
            IXobj * This,
            /* [retval][out] */ IGalaxy **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_XML )( 
            IXobj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Extender )( 
            IXobj * This,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Extender )( 
            IXobj * This,
            /* [in] */ IDispatch *newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DocXml )( 
            IXobj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_rgbMiddle )( 
            IXobj * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_rgbMiddle )( 
            IXobj * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_rgbRightBottom )( 
            IXobj * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_rgbRightBottom )( 
            IXobj * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_rgbLeftTop )( 
            IXobj * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_rgbLeftTop )( 
            IXobj * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Hmin )( 
            IXobj * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Hmin )( 
            IXobj * This,
            /* [in] */ int newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Hmax )( 
            IXobj * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Hmax )( 
            IXobj * This,
            /* [in] */ int newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Vmin )( 
            IXobj * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Vmin )( 
            IXobj * This,
            /* [in] */ int newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Vmax )( 
            IXobj * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Vmax )( 
            IXobj * This,
            /* [in] */ int newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HostXobj )( 
            IXobj * This,
            /* [retval][out] */ IXobj **pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HostXobj )( 
            IXobj * This,
            /* [in] */ IXobj *newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActivePage )( 
            IXobj * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ActivePage )( 
            IXobj * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HostGalaxy )( 
            IXobj * This,
            /* [retval][out] */ IGalaxy **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MasterRow )( 
            IXobj * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MasterRow )( 
            IXobj * This,
            /* [in] */ int newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MasterCol )( 
            IXobj * This,
            /* [retval][out] */ int *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MasterCol )( 
            IXobj * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_OfficeObj )( 
            IXobj * This,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SaveToConfigFile )( 
            IXobj * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WebPage )( 
            IXobj * This,
            /* [retval][out] */ IWebPage **pVal);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *ActiveTabPage )( 
            IXobj * This,
            IXobj *pXobj);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetXobjs )( 
            IXobj * This,
            /* [in] */ BSTR bstrName,
            /* [out] */ IXobj **ppXobj,
            /* [out] */ IXobjCollection **ppXobjs,
            /* [retval][out] */ long *pCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetXobj )( 
            IXobj * This,
            /* [in] */ long nRow,
            /* [in] */ long nCol,
            /* [retval][out] */ IXobj **ppXobj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCtrlByName )( 
            IXobj * This,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            /* [retval][out] */ IDispatch **ppCtrlDisp);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SendIPCMessage )( 
            IXobj * This,
            BSTR bstrTo,
            BSTR bstrPayload,
            BSTR bstrExtra,
            BSTR bstrMsgId,
            /* [retval][out] */ BSTR *bstrRes);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Observe )( 
            IXobj * This,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IXobj **ppRetXobj);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadXML )( 
            IXobj * This,
            int nType,
            BSTR bstrXML);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ObserveEx )( 
            IXobj * This,
            int nRow,
            int nCol,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IXobj **ppRetXobj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetXobjByName )( 
            IXobj * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IXobjCollection **pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCtrlValueByName )( 
            IXobj * This,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            /* [retval][out] */ BSTR *bstrVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetCtrlValueByName )( 
            IXobj * This,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            BSTR bstrVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *NavigateURL )( 
            IXobj * This,
            BSTR bstrURL,
            IDispatch *dispObjforScript);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetChildXobjByName )( 
            IXobj * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IXobj **pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetUIScript )( 
            IXobj * This,
            /* [in] */ BSTR bstrCtrlName,
            /* [retval][out] */ BSTR *bstrVal);
        
        END_INTERFACE
    } IXobjVtbl;

    interface IXobj
    {
        CONST_VTBL struct IXobjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXobj_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXobj_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXobj_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXobj_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IXobj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IXobj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IXobj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IXobj_get_ChildNodes(This,ppXobjColletion)	\
    ( (This)->lpVtbl -> get_ChildNodes(This,ppXobjColletion) ) 

#define IXobj_get_Rows(This,nRows)	\
    ( (This)->lpVtbl -> get_Rows(This,nRows) ) 

#define IXobj_get_Cols(This,nCols)	\
    ( (This)->lpVtbl -> get_Cols(This,nCols) ) 

#define IXobj_get_Row(This,nRow)	\
    ( (This)->lpVtbl -> get_Row(This,nRow) ) 

#define IXobj_get_Col(This,nCol)	\
    ( (This)->lpVtbl -> get_Col(This,nCol) ) 

#define IXobj_get_XobjType(This,nType)	\
    ( (This)->lpVtbl -> get_XobjType(This,nType) ) 

#define IXobj_get_ParentXobj(This,ppXobj)	\
    ( (This)->lpVtbl -> get_ParentXobj(This,ppXobj) ) 

#define IXobj_get_URL(This,pVal)	\
    ( (This)->lpVtbl -> get_URL(This,pVal) ) 

#define IXobj_put_URL(This,newVal)	\
    ( (This)->lpVtbl -> put_URL(This,newVal) ) 

#define IXobj_get_GalaxyCluster(This,pVal)	\
    ( (This)->lpVtbl -> get_GalaxyCluster(This,pVal) ) 

#define IXobj_get_RootXobj(This,ppXobj)	\
    ( (This)->lpVtbl -> get_RootXobj(This,ppXobj) ) 

#define IXobj_get_XObject(This,pVar)	\
    ( (This)->lpVtbl -> get_XObject(This,pVar) ) 

#define IXobj_get_AxPlugIn(This,bstrPlugInName,pVal)	\
    ( (This)->lpVtbl -> get_AxPlugIn(This,bstrPlugInName,pVal) ) 

#define IXobj_get_Caption(This,pVal)	\
    ( (This)->lpVtbl -> get_Caption(This,pVal) ) 

#define IXobj_put_Caption(This,bstrCaption)	\
    ( (This)->lpVtbl -> put_Caption(This,bstrCaption) ) 

#define IXobj_get_Name(This,pVal)	\
    ( (This)->lpVtbl -> get_Name(This,pVal) ) 

#define IXobj_put_Name(This,bstrName)	\
    ( (This)->lpVtbl -> put_Name(This,bstrName) ) 

#define IXobj_get_Objects(This,nType,ppXobjColletion)	\
    ( (This)->lpVtbl -> get_Objects(This,nType,ppXobjColletion) ) 

#define IXobj_get_Handle(This,hWnd)	\
    ( (This)->lpVtbl -> get_Handle(This,hWnd) ) 

#define IXobj_get_Attribute(This,bstrKey,pVal)	\
    ( (This)->lpVtbl -> get_Attribute(This,bstrKey,pVal) ) 

#define IXobj_put_Attribute(This,bstrKey,bstrVal)	\
    ( (This)->lpVtbl -> put_Attribute(This,bstrKey,bstrVal) ) 

#define IXobj_get_Tag(This,pVar)	\
    ( (This)->lpVtbl -> get_Tag(This,pVar) ) 

#define IXobj_put_Tag(This,vVar)	\
    ( (This)->lpVtbl -> put_Tag(This,vVar) ) 

#define IXobj_get_OuterXml(This,pVal)	\
    ( (This)->lpVtbl -> get_OuterXml(This,pVal) ) 

#define IXobj_get_Key(This,pVal)	\
    ( (This)->lpVtbl -> get_Key(This,pVal) ) 

#define IXobj_get_DockObj(This,bstrName,pVal)	\
    ( (This)->lpVtbl -> get_DockObj(This,bstrName,pVal) ) 

#define IXobj_put_DockObj(This,bstrName,newVal)	\
    ( (This)->lpVtbl -> put_DockObj(This,bstrName,newVal) ) 

#define IXobj_get_NameAtWindowPage(This,pVal)	\
    ( (This)->lpVtbl -> get_NameAtWindowPage(This,pVal) ) 

#define IXobj_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IXobj_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IXobj_get_Galaxy(This,pVal)	\
    ( (This)->lpVtbl -> get_Galaxy(This,pVal) ) 

#define IXobj_get_XML(This,pVal)	\
    ( (This)->lpVtbl -> get_XML(This,pVal) ) 

#define IXobj_get_Extender(This,pVal)	\
    ( (This)->lpVtbl -> get_Extender(This,pVal) ) 

#define IXobj_put_Extender(This,newVal)	\
    ( (This)->lpVtbl -> put_Extender(This,newVal) ) 

#define IXobj_get_DocXml(This,pVal)	\
    ( (This)->lpVtbl -> get_DocXml(This,pVal) ) 

#define IXobj_get_rgbMiddle(This,pVal)	\
    ( (This)->lpVtbl -> get_rgbMiddle(This,pVal) ) 

#define IXobj_put_rgbMiddle(This,newVal)	\
    ( (This)->lpVtbl -> put_rgbMiddle(This,newVal) ) 

#define IXobj_get_rgbRightBottom(This,pVal)	\
    ( (This)->lpVtbl -> get_rgbRightBottom(This,pVal) ) 

#define IXobj_put_rgbRightBottom(This,newVal)	\
    ( (This)->lpVtbl -> put_rgbRightBottom(This,newVal) ) 

#define IXobj_get_rgbLeftTop(This,pVal)	\
    ( (This)->lpVtbl -> get_rgbLeftTop(This,pVal) ) 

#define IXobj_put_rgbLeftTop(This,newVal)	\
    ( (This)->lpVtbl -> put_rgbLeftTop(This,newVal) ) 

#define IXobj_get_Hmin(This,pVal)	\
    ( (This)->lpVtbl -> get_Hmin(This,pVal) ) 

#define IXobj_put_Hmin(This,newVal)	\
    ( (This)->lpVtbl -> put_Hmin(This,newVal) ) 

#define IXobj_get_Hmax(This,pVal)	\
    ( (This)->lpVtbl -> get_Hmax(This,pVal) ) 

#define IXobj_put_Hmax(This,newVal)	\
    ( (This)->lpVtbl -> put_Hmax(This,newVal) ) 

#define IXobj_get_Vmin(This,pVal)	\
    ( (This)->lpVtbl -> get_Vmin(This,pVal) ) 

#define IXobj_put_Vmin(This,newVal)	\
    ( (This)->lpVtbl -> put_Vmin(This,newVal) ) 

#define IXobj_get_Vmax(This,pVal)	\
    ( (This)->lpVtbl -> get_Vmax(This,pVal) ) 

#define IXobj_put_Vmax(This,newVal)	\
    ( (This)->lpVtbl -> put_Vmax(This,newVal) ) 

#define IXobj_get_HostXobj(This,pVal)	\
    ( (This)->lpVtbl -> get_HostXobj(This,pVal) ) 

#define IXobj_put_HostXobj(This,newVal)	\
    ( (This)->lpVtbl -> put_HostXobj(This,newVal) ) 

#define IXobj_get_ActivePage(This,pVal)	\
    ( (This)->lpVtbl -> get_ActivePage(This,pVal) ) 

#define IXobj_put_ActivePage(This,newVal)	\
    ( (This)->lpVtbl -> put_ActivePage(This,newVal) ) 

#define IXobj_get_HostGalaxy(This,pVal)	\
    ( (This)->lpVtbl -> get_HostGalaxy(This,pVal) ) 

#define IXobj_get_MasterRow(This,pVal)	\
    ( (This)->lpVtbl -> get_MasterRow(This,pVal) ) 

#define IXobj_put_MasterRow(This,newVal)	\
    ( (This)->lpVtbl -> put_MasterRow(This,newVal) ) 

#define IXobj_get_MasterCol(This,pVal)	\
    ( (This)->lpVtbl -> get_MasterCol(This,pVal) ) 

#define IXobj_put_MasterCol(This,newVal)	\
    ( (This)->lpVtbl -> put_MasterCol(This,newVal) ) 

#define IXobj_get_OfficeObj(This,pVal)	\
    ( (This)->lpVtbl -> get_OfficeObj(This,pVal) ) 

#define IXobj_put_SaveToConfigFile(This,newVal)	\
    ( (This)->lpVtbl -> put_SaveToConfigFile(This,newVal) ) 

#define IXobj_get_WebPage(This,pVal)	\
    ( (This)->lpVtbl -> get_WebPage(This,pVal) ) 

#define IXobj_ActiveTabPage(This,pXobj)	\
    ( (This)->lpVtbl -> ActiveTabPage(This,pXobj) ) 

#define IXobj_GetXobjs(This,bstrName,ppXobj,ppXobjs,pCount)	\
    ( (This)->lpVtbl -> GetXobjs(This,bstrName,ppXobj,ppXobjs,pCount) ) 

#define IXobj_GetXobj(This,nRow,nCol,ppXobj)	\
    ( (This)->lpVtbl -> GetXobj(This,nRow,nCol,ppXobj) ) 

#define IXobj_GetCtrlByName(This,bstrName,bFindInChild,ppCtrlDisp)	\
    ( (This)->lpVtbl -> GetCtrlByName(This,bstrName,bFindInChild,ppCtrlDisp) ) 

#define IXobj_SendIPCMessage(This,bstrTo,bstrPayload,bstrExtra,bstrMsgId,bstrRes)	\
    ( (This)->lpVtbl -> SendIPCMessage(This,bstrTo,bstrPayload,bstrExtra,bstrMsgId,bstrRes) ) 

#define IXobj_Observe(This,bstrKey,bstrXml,ppRetXobj)	\
    ( (This)->lpVtbl -> Observe(This,bstrKey,bstrXml,ppRetXobj) ) 

#define IXobj_LoadXML(This,nType,bstrXML)	\
    ( (This)->lpVtbl -> LoadXML(This,nType,bstrXML) ) 

#define IXobj_ObserveEx(This,nRow,nCol,bstrKey,bstrXml,ppRetXobj)	\
    ( (This)->lpVtbl -> ObserveEx(This,nRow,nCol,bstrKey,bstrXml,ppRetXobj) ) 

#define IXobj_GetXobjByName(This,bstrName,pVal)	\
    ( (This)->lpVtbl -> GetXobjByName(This,bstrName,pVal) ) 

#define IXobj_GetCtrlValueByName(This,bstrName,bFindInChild,bstrVal)	\
    ( (This)->lpVtbl -> GetCtrlValueByName(This,bstrName,bFindInChild,bstrVal) ) 

#define IXobj_SetCtrlValueByName(This,bstrName,bFindInChild,bstrVal)	\
    ( (This)->lpVtbl -> SetCtrlValueByName(This,bstrName,bFindInChild,bstrVal) ) 

#define IXobj_NavigateURL(This,bstrURL,dispObjforScript)	\
    ( (This)->lpVtbl -> NavigateURL(This,bstrURL,dispObjforScript) ) 

#define IXobj_GetChildXobjByName(This,bstrName,pVal)	\
    ( (This)->lpVtbl -> GetChildXobjByName(This,bstrName,pVal) ) 

#define IXobj_GetUIScript(This,bstrCtrlName,bstrVal)	\
    ( (This)->lpVtbl -> GetUIScript(This,bstrCtrlName,bstrVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXobj_INTERFACE_DEFINED__ */


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


#ifndef __ICosmos_INTERFACE_DEFINED__
#define __ICosmos_INTERFACE_DEFINED__

/* interface ICosmos */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICosmos;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060119820001")
    ICosmos : public IDispatch
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
            /* [retval][out] */ IXobjCollection **ppXobjColletion) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HostWnd( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentActiveXobj( 
            /* [retval][out] */ IXobj **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CreatingXobj( 
            /* [retval][out] */ IXobj **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_RemoteCosmos( 
            BSTR bstrID,
            /* [retval][out] */ ICosmos **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Extender( 
            /* [retval][out] */ ICosmosExtender **pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Extender( 
            /* [in] */ ICosmosExtender *newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_RemoteHelperHWND( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_RemoteHelperHWND( 
            /* [in] */ LONGLONG newVal) = 0;
        
        virtual /* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_DesignNode( 
            /* [retval][out] */ IXobj **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DocTemplate( 
            BSTR bstrID,
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveChromeBrowserWnd( 
            /* [retval][out] */ IBrowser **ppChromeWebBrowser) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE NavigateNode( 
            IXobj *pXobj,
            BSTR bstrBrowserID,
            BSTR bstrXml,
            /* [retval][out] */ IXobj **pVal) = 0;
        
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
            IXobj *pXobj,
            long nCtrlID,
            LONG nMaxLengeh,
            /* [retval][out] */ BSTR *bstrRet) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetItemText( 
            IXobj *pXobj,
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
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UpdateXobj( 
            IXobj *pXobj) = 0;
        
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
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetXobjFromHandle( 
            LONGLONG hWnd,
            /* [retval][out] */ IXobj **ppRetXobj) = 0;
        
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
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE CreateCosmosCtrl( 
            BSTR bstrAppID,
            /* [retval][out] */ ICosmosCtrl **ppRetCtrl) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetWindowClientDefaultNode( 
            IDispatch *pAddDisp,
            LONGLONG hParent,
            BSTR bstrWndClaName,
            BSTR bstrGalaxyClusterName,
            /* [retval][out] */ IXobj **ppXobj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDocTemplateXml( 
            BSTR bstrCaption,
            BSTR bstrPath,
            BSTR bstrFilter,
            /* [retval][out] */ BSTR *bstrTemplatePath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateCosmosEventObj( 
            /* [retval][out] */ ICosmosEventObj **ppCosmosEventObj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FireCosmosEventObj( 
            ICosmosEventObj *pCosmosEventObj) = 0;
        
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
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE InitCLRApp( 
            BSTR strInitXml,
            /* [retval][out] */ LONGLONG *llHandle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateOutLookObj( 
            BSTR bstrObjType,
            int nType,
            BSTR bstrURL,
            /* [retval][out] */ IDispatch **ppRetDisp) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE NewWorkBench( 
            BSTR bstrCosmosDoc,
            /* [retval][out] */ IWorkBenchWindow **ppWorkBenchWindow) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateBrowser( 
            ULONGLONG hParentWnd,
            BSTR strUrls,
            /* [retval][out] */ IBrowser **ppRet) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CosmosNotify( 
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
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CosmosCommand( 
            IDispatch *RibbonControl) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CosmosGetImage( 
            BSTR strValue,
            /* [retval][out] */ IPictureDisp **ppdispImage) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CosmosGetVisible( 
            IDispatch *RibbonControl,
            /* [retval][out] */ VARIANT *varVisible) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CosmosOnLoad( 
            IDispatch *RibbonControl) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CosmosGetItemCount( 
            IDispatch *RibbonControl,
            /* [retval][out] */ long *nCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CosmosGetItemLabel( 
            IDispatch *RibbonControl,
            long nIndex,
            /* [retval][out] */ BSTR *bstrLabel) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CosmosGetItemID( 
            IDispatch *RibbonControl,
            long nIndex,
            /* [retval][out] */ BSTR *bstrID) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICosmosVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICosmos * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICosmos * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICosmos * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICosmos * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICosmos * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICosmos * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICosmos * This,
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
            ICosmos * This,
            /* [retval][out] */ IBrowser **ppChromeWebBrowser);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AppKeyValue )( 
            ICosmos * This,
            BSTR bstrKey,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AppKeyValue )( 
            ICosmos * This,
            BSTR bstrKey,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AppExtender )( 
            ICosmos * This,
            BSTR bstrKey,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AppExtender )( 
            ICosmos * This,
            BSTR bstrKey,
            /* [in] */ IDispatch *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RootNodes )( 
            ICosmos * This,
            /* [retval][out] */ IXobjCollection **ppXobjColletion);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HostWnd )( 
            ICosmos * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentActiveXobj )( 
            ICosmos * This,
            /* [retval][out] */ IXobj **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CreatingXobj )( 
            ICosmos * This,
            /* [retval][out] */ IXobj **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RemoteCosmos )( 
            ICosmos * This,
            BSTR bstrID,
            /* [retval][out] */ ICosmos **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Extender )( 
            ICosmos * This,
            /* [retval][out] */ ICosmosExtender **pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Extender )( 
            ICosmos * This,
            /* [in] */ ICosmosExtender *newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RemoteHelperHWND )( 
            ICosmos * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RemoteHelperHWND )( 
            ICosmos * This,
            /* [in] */ LONGLONG newVal);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DesignNode )( 
            ICosmos * This,
            /* [retval][out] */ IXobj **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DocTemplate )( 
            ICosmos * This,
            BSTR bstrID,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveChromeBrowserWnd )( 
            ICosmos * This,
            /* [retval][out] */ IBrowser **ppChromeWebBrowser);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *NavigateNode )( 
            ICosmos * This,
            IXobj *pXobj,
            BSTR bstrBrowserID,
            BSTR bstrXml,
            /* [retval][out] */ IXobj **pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateGalaxyCluster )( 
            ICosmos * This,
            LONGLONG hWnd,
            /* [retval][out] */ IGalaxyCluster **ppGalaxyCluster);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateOfficeDocument )( 
            ICosmos * This,
            BSTR bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateCLRObj )( 
            ICosmos * This,
            BSTR bstrObjID,
            /* [retval][out] */ IDispatch **ppDisp);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StartApplication )( 
            ICosmos * This,
            BSTR bstrAppID,
            BSTR bstrXml);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetGalaxy )( 
            ICosmos * This,
            LONGLONG hHostWnd,
            /* [retval][out] */ IGalaxy **ppGalaxy);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetItemText )( 
            ICosmos * This,
            IXobj *pXobj,
            long nCtrlID,
            LONG nMaxLengeh,
            /* [retval][out] */ BSTR *bstrRet);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetItemText )( 
            ICosmos * This,
            IXobj *pXobj,
            long nCtrlID,
            BSTR bstrText);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCLRControl )( 
            ICosmos * This,
            IDispatch *CtrlDisp,
            BSTR bstrName,
            /* [retval][out] */ IDispatch **ppRetDisp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MessageBox )( 
            ICosmos * This,
            LONGLONG hWnd,
            BSTR bstrContext,
            BSTR bstrCaption,
            long nStyle,
            /* [retval][out] */ int *nRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Encode )( 
            ICosmos * This,
            BSTR bstrSRC,
            VARIANT_BOOL bEncode,
            /* [retval][out] */ BSTR *bstrRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetHostFocus )( 
            ICosmos * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UpdateXobj )( 
            ICosmos * This,
            IXobj *pXobj);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NewGUID )( 
            ICosmos * This,
            /* [retval][out] */ BSTR *retVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DownLoadFile )( 
            ICosmos * This,
            BSTR strFileURL,
            BSTR bstrTargetFile,
            BSTR bstrActionXml);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCtrlByName )( 
            ICosmos * This,
            IDispatch *pCtrl,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            /* [retval][out] */ IDispatch **ppCtrlDisp);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetXobjFromHandle )( 
            ICosmos * This,
            LONGLONG hWnd,
            /* [retval][out] */ IXobj **ppRetXobj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCtrlValueByName )( 
            ICosmos * This,
            IDispatch *pCtrl,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            /* [retval][out] */ BSTR *bstrVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetCtrlValueByName )( 
            ICosmos * This,
            IDispatch *pCtrl,
            BSTR bstrName,
            VARIANT_BOOL bFindInChild,
            BSTR bstrVal);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *CreateCosmosCtrl )( 
            ICosmos * This,
            BSTR bstrAppID,
            /* [retval][out] */ ICosmosCtrl **ppRetCtrl);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetWindowClientDefaultNode )( 
            ICosmos * This,
            IDispatch *pAddDisp,
            LONGLONG hParent,
            BSTR bstrWndClaName,
            BSTR bstrGalaxyClusterName,
            /* [retval][out] */ IXobj **ppXobj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDocTemplateXml )( 
            ICosmos * This,
            BSTR bstrCaption,
            BSTR bstrPath,
            BSTR bstrFilter,
            /* [retval][out] */ BSTR *bstrTemplatePath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateCosmosEventObj )( 
            ICosmos * This,
            /* [retval][out] */ ICosmosEventObj **ppCosmosEventObj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FireCosmosEventObj )( 
            ICosmos * This,
            ICosmosEventObj *pCosmosEventObj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ObserveGalaxys )( 
            ICosmos * This,
            LONGLONG hWnd,
            BSTR bstrGalaxys,
            BSTR bstrKey,
            BSTR bstrXml,
            VARIANT_BOOL bSaveToConfigFile);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DeletePage )( 
            ICosmos * This,
            LONGLONG PageHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LoadDocComponent )( 
            ICosmos * This,
            BSTR bstrLib,
            /* [retval][out] */ LONGLONG *llAppProxy);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *InitCLRApp )( 
            ICosmos * This,
            BSTR strInitXml,
            /* [retval][out] */ LONGLONG *llHandle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateOutLookObj )( 
            ICosmos * This,
            BSTR bstrObjType,
            int nType,
            BSTR bstrURL,
            /* [retval][out] */ IDispatch **ppRetDisp);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *NewWorkBench )( 
            ICosmos * This,
            BSTR bstrCosmosDoc,
            /* [retval][out] */ IWorkBenchWindow **ppWorkBenchWindow);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateBrowser )( 
            ICosmos * This,
            ULONGLONG hParentWnd,
            BSTR strUrls,
            /* [retval][out] */ IBrowser **ppRet);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CosmosNotify )( 
            ICosmos * This,
            BSTR strXml1,
            BSTR strXml2,
            LONGLONG wParam,
            LONGLONG lParam);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DeleteGalaxy )( 
            ICosmos * This,
            IGalaxy *pGalaxy);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *InitEclipseApp )( 
            ICosmos * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SelectVSObj )( 
            ICosmos * This,
            BSTR bstrData,
            IDispatch *pVSObj,
            LONGLONG nHandle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CosmosCommand )( 
            ICosmos * This,
            IDispatch *RibbonControl);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CosmosGetImage )( 
            ICosmos * This,
            BSTR strValue,
            /* [retval][out] */ IPictureDisp **ppdispImage);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CosmosGetVisible )( 
            ICosmos * This,
            IDispatch *RibbonControl,
            /* [retval][out] */ VARIANT *varVisible);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CosmosOnLoad )( 
            ICosmos * This,
            IDispatch *RibbonControl);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CosmosGetItemCount )( 
            ICosmos * This,
            IDispatch *RibbonControl,
            /* [retval][out] */ long *nCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CosmosGetItemLabel )( 
            ICosmos * This,
            IDispatch *RibbonControl,
            long nIndex,
            /* [retval][out] */ BSTR *bstrLabel);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CosmosGetItemID )( 
            ICosmos * This,
            IDispatch *RibbonControl,
            long nIndex,
            /* [retval][out] */ BSTR *bstrID);
        
        END_INTERFACE
    } ICosmosVtbl;

    interface ICosmos
    {
        CONST_VTBL struct ICosmosVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICosmos_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICosmos_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICosmos_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICosmos_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICosmos_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICosmos_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICosmos_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICosmos_get_HostChromeBrowserWnd(This,ppChromeWebBrowser)	\
    ( (This)->lpVtbl -> get_HostChromeBrowserWnd(This,ppChromeWebBrowser) ) 

#define ICosmos_get_AppKeyValue(This,bstrKey,pVal)	\
    ( (This)->lpVtbl -> get_AppKeyValue(This,bstrKey,pVal) ) 

#define ICosmos_put_AppKeyValue(This,bstrKey,newVal)	\
    ( (This)->lpVtbl -> put_AppKeyValue(This,bstrKey,newVal) ) 

#define ICosmos_get_AppExtender(This,bstrKey,pVal)	\
    ( (This)->lpVtbl -> get_AppExtender(This,bstrKey,pVal) ) 

#define ICosmos_put_AppExtender(This,bstrKey,newVal)	\
    ( (This)->lpVtbl -> put_AppExtender(This,bstrKey,newVal) ) 

#define ICosmos_get_RootNodes(This,ppXobjColletion)	\
    ( (This)->lpVtbl -> get_RootNodes(This,ppXobjColletion) ) 

#define ICosmos_get_HostWnd(This,pVal)	\
    ( (This)->lpVtbl -> get_HostWnd(This,pVal) ) 

#define ICosmos_get_CurrentActiveXobj(This,pVal)	\
    ( (This)->lpVtbl -> get_CurrentActiveXobj(This,pVal) ) 

#define ICosmos_get_CreatingXobj(This,pVal)	\
    ( (This)->lpVtbl -> get_CreatingXobj(This,pVal) ) 

#define ICosmos_get_RemoteCosmos(This,bstrID,pVal)	\
    ( (This)->lpVtbl -> get_RemoteCosmos(This,bstrID,pVal) ) 

#define ICosmos_get_Extender(This,pVal)	\
    ( (This)->lpVtbl -> get_Extender(This,pVal) ) 

#define ICosmos_put_Extender(This,newVal)	\
    ( (This)->lpVtbl -> put_Extender(This,newVal) ) 

#define ICosmos_get_RemoteHelperHWND(This,pVal)	\
    ( (This)->lpVtbl -> get_RemoteHelperHWND(This,pVal) ) 

#define ICosmos_put_RemoteHelperHWND(This,newVal)	\
    ( (This)->lpVtbl -> put_RemoteHelperHWND(This,newVal) ) 

#define ICosmos_get_DesignNode(This,pVal)	\
    ( (This)->lpVtbl -> get_DesignNode(This,pVal) ) 

#define ICosmos_get_DocTemplate(This,bstrID,pVal)	\
    ( (This)->lpVtbl -> get_DocTemplate(This,bstrID,pVal) ) 

#define ICosmos_get_ActiveChromeBrowserWnd(This,ppChromeWebBrowser)	\
    ( (This)->lpVtbl -> get_ActiveChromeBrowserWnd(This,ppChromeWebBrowser) ) 

#define ICosmos_NavigateNode(This,pXobj,bstrBrowserID,bstrXml,pVal)	\
    ( (This)->lpVtbl -> NavigateNode(This,pXobj,bstrBrowserID,bstrXml,pVal) ) 

#define ICosmos_CreateGalaxyCluster(This,hWnd,ppGalaxyCluster)	\
    ( (This)->lpVtbl -> CreateGalaxyCluster(This,hWnd,ppGalaxyCluster) ) 

#define ICosmos_CreateOfficeDocument(This,bstrXml)	\
    ( (This)->lpVtbl -> CreateOfficeDocument(This,bstrXml) ) 

#define ICosmos_CreateCLRObj(This,bstrObjID,ppDisp)	\
    ( (This)->lpVtbl -> CreateCLRObj(This,bstrObjID,ppDisp) ) 

#define ICosmos_StartApplication(This,bstrAppID,bstrXml)	\
    ( (This)->lpVtbl -> StartApplication(This,bstrAppID,bstrXml) ) 

#define ICosmos_GetGalaxy(This,hHostWnd,ppGalaxy)	\
    ( (This)->lpVtbl -> GetGalaxy(This,hHostWnd,ppGalaxy) ) 

#define ICosmos_GetItemText(This,pXobj,nCtrlID,nMaxLengeh,bstrRet)	\
    ( (This)->lpVtbl -> GetItemText(This,pXobj,nCtrlID,nMaxLengeh,bstrRet) ) 

#define ICosmos_SetItemText(This,pXobj,nCtrlID,bstrText)	\
    ( (This)->lpVtbl -> SetItemText(This,pXobj,nCtrlID,bstrText) ) 

#define ICosmos_GetCLRControl(This,CtrlDisp,bstrName,ppRetDisp)	\
    ( (This)->lpVtbl -> GetCLRControl(This,CtrlDisp,bstrName,ppRetDisp) ) 

#define ICosmos_MessageBox(This,hWnd,bstrContext,bstrCaption,nStyle,nRet)	\
    ( (This)->lpVtbl -> MessageBox(This,hWnd,bstrContext,bstrCaption,nStyle,nRet) ) 

#define ICosmos_Encode(This,bstrSRC,bEncode,bstrRet)	\
    ( (This)->lpVtbl -> Encode(This,bstrSRC,bEncode,bstrRet) ) 

#define ICosmos_SetHostFocus(This)	\
    ( (This)->lpVtbl -> SetHostFocus(This) ) 

#define ICosmos_UpdateXobj(This,pXobj)	\
    ( (This)->lpVtbl -> UpdateXobj(This,pXobj) ) 

#define ICosmos_NewGUID(This,retVal)	\
    ( (This)->lpVtbl -> NewGUID(This,retVal) ) 

#define ICosmos_DownLoadFile(This,strFileURL,bstrTargetFile,bstrActionXml)	\
    ( (This)->lpVtbl -> DownLoadFile(This,strFileURL,bstrTargetFile,bstrActionXml) ) 

#define ICosmos_GetCtrlByName(This,pCtrl,bstrName,bFindInChild,ppCtrlDisp)	\
    ( (This)->lpVtbl -> GetCtrlByName(This,pCtrl,bstrName,bFindInChild,ppCtrlDisp) ) 

#define ICosmos_GetXobjFromHandle(This,hWnd,ppRetXobj)	\
    ( (This)->lpVtbl -> GetXobjFromHandle(This,hWnd,ppRetXobj) ) 

#define ICosmos_GetCtrlValueByName(This,pCtrl,bstrName,bFindInChild,bstrVal)	\
    ( (This)->lpVtbl -> GetCtrlValueByName(This,pCtrl,bstrName,bFindInChild,bstrVal) ) 

#define ICosmos_SetCtrlValueByName(This,pCtrl,bstrName,bFindInChild,bstrVal)	\
    ( (This)->lpVtbl -> SetCtrlValueByName(This,pCtrl,bstrName,bFindInChild,bstrVal) ) 

#define ICosmos_CreateCosmosCtrl(This,bstrAppID,ppRetCtrl)	\
    ( (This)->lpVtbl -> CreateCosmosCtrl(This,bstrAppID,ppRetCtrl) ) 

#define ICosmos_GetWindowClientDefaultNode(This,pAddDisp,hParent,bstrWndClaName,bstrGalaxyClusterName,ppXobj)	\
    ( (This)->lpVtbl -> GetWindowClientDefaultNode(This,pAddDisp,hParent,bstrWndClaName,bstrGalaxyClusterName,ppXobj) ) 

#define ICosmos_GetDocTemplateXml(This,bstrCaption,bstrPath,bstrFilter,bstrTemplatePath)	\
    ( (This)->lpVtbl -> GetDocTemplateXml(This,bstrCaption,bstrPath,bstrFilter,bstrTemplatePath) ) 

#define ICosmos_CreateCosmosEventObj(This,ppCosmosEventObj)	\
    ( (This)->lpVtbl -> CreateCosmosEventObj(This,ppCosmosEventObj) ) 

#define ICosmos_FireCosmosEventObj(This,pCosmosEventObj)	\
    ( (This)->lpVtbl -> FireCosmosEventObj(This,pCosmosEventObj) ) 

#define ICosmos_ObserveGalaxys(This,hWnd,bstrGalaxys,bstrKey,bstrXml,bSaveToConfigFile)	\
    ( (This)->lpVtbl -> ObserveGalaxys(This,hWnd,bstrGalaxys,bstrKey,bstrXml,bSaveToConfigFile) ) 

#define ICosmos_DeletePage(This,PageHandle)	\
    ( (This)->lpVtbl -> DeletePage(This,PageHandle) ) 

#define ICosmos_LoadDocComponent(This,bstrLib,llAppProxy)	\
    ( (This)->lpVtbl -> LoadDocComponent(This,bstrLib,llAppProxy) ) 

#define ICosmos_InitCLRApp(This,strInitXml,llHandle)	\
    ( (This)->lpVtbl -> InitCLRApp(This,strInitXml,llHandle) ) 

#define ICosmos_CreateOutLookObj(This,bstrObjType,nType,bstrURL,ppRetDisp)	\
    ( (This)->lpVtbl -> CreateOutLookObj(This,bstrObjType,nType,bstrURL,ppRetDisp) ) 

#define ICosmos_NewWorkBench(This,bstrCosmosDoc,ppWorkBenchWindow)	\
    ( (This)->lpVtbl -> NewWorkBench(This,bstrCosmosDoc,ppWorkBenchWindow) ) 

#define ICosmos_CreateBrowser(This,hParentWnd,strUrls,ppRet)	\
    ( (This)->lpVtbl -> CreateBrowser(This,hParentWnd,strUrls,ppRet) ) 

#define ICosmos_CosmosNotify(This,strXml1,strXml2,wParam,lParam)	\
    ( (This)->lpVtbl -> CosmosNotify(This,strXml1,strXml2,wParam,lParam) ) 

#define ICosmos_DeleteGalaxy(This,pGalaxy)	\
    ( (This)->lpVtbl -> DeleteGalaxy(This,pGalaxy) ) 

#define ICosmos_InitEclipseApp(This)	\
    ( (This)->lpVtbl -> InitEclipseApp(This) ) 

#define ICosmos_SelectVSObj(This,bstrData,pVSObj,nHandle)	\
    ( (This)->lpVtbl -> SelectVSObj(This,bstrData,pVSObj,nHandle) ) 

#define ICosmos_CosmosCommand(This,RibbonControl)	\
    ( (This)->lpVtbl -> CosmosCommand(This,RibbonControl) ) 

#define ICosmos_CosmosGetImage(This,strValue,ppdispImage)	\
    ( (This)->lpVtbl -> CosmosGetImage(This,strValue,ppdispImage) ) 

#define ICosmos_CosmosGetVisible(This,RibbonControl,varVisible)	\
    ( (This)->lpVtbl -> CosmosGetVisible(This,RibbonControl,varVisible) ) 

#define ICosmos_CosmosOnLoad(This,RibbonControl)	\
    ( (This)->lpVtbl -> CosmosOnLoad(This,RibbonControl) ) 

#define ICosmos_CosmosGetItemCount(This,RibbonControl,nCount)	\
    ( (This)->lpVtbl -> CosmosGetItemCount(This,RibbonControl,nCount) ) 

#define ICosmos_CosmosGetItemLabel(This,RibbonControl,nIndex,bstrLabel)	\
    ( (This)->lpVtbl -> CosmosGetItemLabel(This,RibbonControl,nIndex,bstrLabel) ) 

#define ICosmos_CosmosGetItemID(This,RibbonControl,nIndex,bstrID)	\
    ( (This)->lpVtbl -> CosmosGetItemID(This,RibbonControl,nIndex,bstrID) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICosmos_INTERFACE_DEFINED__ */


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
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Xobj( 
            /* [in] */ VARIANT vIndex,
            /* [retval][out] */ IXobj **ppXobj) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HWND( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_VisibleXobj( 
            /* [retval][out] */ IXobj **pVal) = 0;
        
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
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RootXobjs( 
            /* [retval][out] */ IXobjCollection **ppXobjColletion) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_GalaxyXML( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
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
            /* [retval][out] */ IXobj **ppRetXobj) = 0;
        
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
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Xobj )( 
            IGalaxy * This,
            /* [in] */ VARIANT vIndex,
            /* [retval][out] */ IXobj **ppXobj);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IGalaxy * This,
            /* [retval][out] */ IUnknown **ppVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWND )( 
            IGalaxy * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VisibleXobj )( 
            IGalaxy * This,
            /* [retval][out] */ IXobj **pVal);
        
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
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RootXobjs )( 
            IGalaxy * This,
            /* [retval][out] */ IXobjCollection **ppXobjColletion);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GalaxyXML )( 
            IGalaxy * This,
            /* [retval][out] */ BSTR *pVal);
        
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
            /* [retval][out] */ IXobj **ppRetXobj);
        
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


#define IGalaxy_get_Xobj(This,vIndex,ppXobj)	\
    ( (This)->lpVtbl -> get_Xobj(This,vIndex,ppXobj) ) 

#define IGalaxy_get__NewEnum(This,ppVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppVal) ) 

#define IGalaxy_get_HWND(This,pVal)	\
    ( (This)->lpVtbl -> get_HWND(This,pVal) ) 

#define IGalaxy_get_VisibleXobj(This,pVal)	\
    ( (This)->lpVtbl -> get_VisibleXobj(This,pVal) ) 

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

#define IGalaxy_get_RootXobjs(This,ppXobjColletion)	\
    ( (This)->lpVtbl -> get_RootXobjs(This,ppXobjColletion) ) 

#define IGalaxy_get_GalaxyXML(This,pVal)	\
    ( (This)->lpVtbl -> get_GalaxyXML(This,pVal) ) 

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

#define IGalaxy_Observe(This,bstrKey,bstrXml,ppRetXobj)	\
    ( (This)->lpVtbl -> Observe(This,bstrKey,bstrXml,ppRetXobj) ) 

#define IGalaxy_GetXml(This,bstrRootName,bstrRet)	\
    ( (This)->lpVtbl -> GetXml(This,bstrRootName,bstrRet) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGalaxy_INTERFACE_DEFINED__ */


#ifndef __IXobjCollection_INTERFACE_DEFINED__
#define __IXobjCollection_INTERFACE_DEFINED__

/* interface IXobjCollection */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IXobjCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060119820017")
    IXobjCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_XobjCount( 
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long iIndex,
            /* [retval][out] */ IXobj **ppTopWindow) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXobjCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IXobjCollection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IXobjCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IXobjCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IXobjCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IXobjCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IXobjCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IXobjCollection * This,
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
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_XobjCount )( 
            IXobjCollection * This,
            /* [retval][out] */ long *pCount);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IXobjCollection * This,
            /* [in] */ long iIndex,
            /* [retval][out] */ IXobj **ppTopWindow);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IXobjCollection * This,
            /* [retval][out] */ IUnknown **ppVal);
        
        END_INTERFACE
    } IXobjCollectionVtbl;

    interface IXobjCollection
    {
        CONST_VTBL struct IXobjCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXobjCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXobjCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXobjCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXobjCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IXobjCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IXobjCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IXobjCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IXobjCollection_get_XobjCount(This,pCount)	\
    ( (This)->lpVtbl -> get_XobjCount(This,pCount) ) 

#define IXobjCollection_get_Item(This,iIndex,ppTopWindow)	\
    ( (This)->lpVtbl -> get_Item(This,iIndex,ppTopWindow) ) 

#define IXobjCollection_get__NewEnum(This,ppVal)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXobjCollection_INTERFACE_DEFINED__ */


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
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Xobj( 
            BSTR bstrNodeName,
            /* [retval][out] */ IXobj **pVal) = 0;
        
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
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_XobjNames( 
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
            /* [retval][out] */ IXobj **pVal) = 0;
        
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
            /* [retval][out] */ IXobj **ppRetXobj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ObserveCtrl( 
            VARIANT Ctrl,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IXobj **ppRetXobj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetXobj( 
            BSTR bstrGalaxyName,
            BSTR bstrNodeName,
            /* [retval][out] */ IXobj **pRetNode) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCtrlInXobj( 
            BSTR NodeName,
            BSTR CtrlName,
            /* [retval][out] */ IDispatch **ppCtrl) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGalaxyFromCtrl( 
            IDispatch *ctrl,
            /* [retval][out] */ IGalaxy **ppGalaxy) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ConnectCosmosCtrl( 
            ICosmosCtrl *eventSource) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateGalaxyWithDefaultNode( 
            ULONGLONG hGalaxyWnd,
            BSTR bstrGalaxyName,
            BSTR bstrDefaultNodeKey,
            BSTR bstrXml,
            VARIANT_BOOL bSaveToConfig,
            /* [retval][out] */ IXobj **ppXobj) = 0;
        
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
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Xobj )( 
            IGalaxyCluster * This,
            BSTR bstrNodeName,
            /* [retval][out] */ IXobj **pVal);
        
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
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_XobjNames )( 
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
            /* [retval][out] */ IXobj **pVal);
        
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
            /* [retval][out] */ IXobj **ppRetXobj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ObserveCtrl )( 
            IGalaxyCluster * This,
            VARIANT Ctrl,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IXobj **ppRetXobj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetXobj )( 
            IGalaxyCluster * This,
            BSTR bstrGalaxyName,
            BSTR bstrNodeName,
            /* [retval][out] */ IXobj **pRetNode);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCtrlInXobj )( 
            IGalaxyCluster * This,
            BSTR NodeName,
            BSTR CtrlName,
            /* [retval][out] */ IDispatch **ppCtrl);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGalaxyFromCtrl )( 
            IGalaxyCluster * This,
            IDispatch *ctrl,
            /* [retval][out] */ IGalaxy **ppGalaxy);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ConnectCosmosCtrl )( 
            IGalaxyCluster * This,
            ICosmosCtrl *eventSource);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateGalaxyWithDefaultNode )( 
            IGalaxyCluster * This,
            ULONGLONG hGalaxyWnd,
            BSTR bstrGalaxyName,
            BSTR bstrDefaultNodeKey,
            BSTR bstrXml,
            VARIANT_BOOL bSaveToConfig,
            /* [retval][out] */ IXobj **ppXobj);
        
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

#define IGalaxyCluster_get_Xobj(This,bstrNodeName,pVal)	\
    ( (This)->lpVtbl -> get_Xobj(This,bstrNodeName,pVal) ) 

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

#define IGalaxyCluster_get_XobjNames(This,pVal)	\
    ( (This)->lpVtbl -> get_XobjNames(This,pVal) ) 

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

#define IGalaxyCluster_Observe(This,Parent,CtrlName,GalaxyName,bstrKey,bstrXml,ppRetXobj)	\
    ( (This)->lpVtbl -> Observe(This,Parent,CtrlName,GalaxyName,bstrKey,bstrXml,ppRetXobj) ) 

#define IGalaxyCluster_ObserveCtrl(This,Ctrl,bstrKey,bstrXml,ppRetXobj)	\
    ( (This)->lpVtbl -> ObserveCtrl(This,Ctrl,bstrKey,bstrXml,ppRetXobj) ) 

#define IGalaxyCluster_GetXobj(This,bstrGalaxyName,bstrNodeName,pRetNode)	\
    ( (This)->lpVtbl -> GetXobj(This,bstrGalaxyName,bstrNodeName,pRetNode) ) 

#define IGalaxyCluster_GetCtrlInXobj(This,NodeName,CtrlName,ppCtrl)	\
    ( (This)->lpVtbl -> GetCtrlInXobj(This,NodeName,CtrlName,ppCtrl) ) 

#define IGalaxyCluster_GetGalaxyFromCtrl(This,ctrl,ppGalaxy)	\
    ( (This)->lpVtbl -> GetGalaxyFromCtrl(This,ctrl,ppGalaxy) ) 

#define IGalaxyCluster_ConnectCosmosCtrl(This,eventSource)	\
    ( (This)->lpVtbl -> ConnectCosmosCtrl(This,eventSource) ) 

#define IGalaxyCluster_CreateGalaxyWithDefaultNode(This,hGalaxyWnd,bstrGalaxyName,bstrDefaultNodeKey,bstrXml,bSaveToConfig,ppXobj)	\
    ( (This)->lpVtbl -> CreateGalaxyWithDefaultNode(This,hGalaxyWnd,bstrGalaxyName,bstrDefaultNodeKey,bstrXml,bSaveToConfig,ppXobj) ) 

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
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_CosmosCtrl( 
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
            /* [retval][out] */ IXobj **ppXobj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ObserveEx( 
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IXobj **ppXobj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ObserveInView( 
            int nIndex,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IXobj **ppRetXobj) = 0;
        
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
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CosmosCtrl )( 
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
            /* [retval][out] */ IXobj **ppXobj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ObserveEx )( 
            IWorkBenchWindow * This,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IXobj **ppXobj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ObserveInView )( 
            IWorkBenchWindow * This,
            int nIndex,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IXobj **ppRetXobj);
        
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

#define IWorkBenchWindow_get_CosmosCtrl(This,hWnd,pVal)	\
    ( (This)->lpVtbl -> get_CosmosCtrl(This,hWnd,pVal) ) 

#define IWorkBenchWindow_get_GalaxyCluster(This,pVal)	\
    ( (This)->lpVtbl -> get_GalaxyCluster(This,pVal) ) 

#define IWorkBenchWindow_get_Galaxy(This,pVal)	\
    ( (This)->lpVtbl -> get_Galaxy(This,pVal) ) 

#define IWorkBenchWindow_Active(This)	\
    ( (This)->lpVtbl -> Active(This) ) 

#define IWorkBenchWindow_CloseTangramUI(This)	\
    ( (This)->lpVtbl -> CloseTangramUI(This) ) 

#define IWorkBenchWindow_Observe(This,bstrKey,bstrXml,ppXobj)	\
    ( (This)->lpVtbl -> Observe(This,bstrKey,bstrXml,ppXobj) ) 

#define IWorkBenchWindow_ObserveEx(This,bstrKey,bstrXml,ppXobj)	\
    ( (This)->lpVtbl -> ObserveEx(This,bstrKey,bstrXml,ppXobj) ) 

#define IWorkBenchWindow_ObserveInView(This,nIndex,bstrKey,bstrXml,ppRetXobj)	\
    ( (This)->lpVtbl -> ObserveInView(This,nIndex,bstrKey,bstrXml,ppRetXobj) ) 

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
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_RemoteXobj( 
            /* [retval][out] */ IXobj **pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_RemoteXobj( 
            /* [in] */ IXobj *newVal) = 0;
        
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
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RemoteXobj )( 
            IBrowser * This,
            /* [retval][out] */ IXobj **pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RemoteXobj )( 
            IBrowser * This,
            /* [in] */ IXobj *newVal);
        
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


#define IBrowser_get_RemoteXobj(This,pVal)	\
    ( (This)->lpVtbl -> get_RemoteXobj(This,pVal) ) 

#define IBrowser_put_RemoteXobj(This,newVal)	\
    ( (This)->lpVtbl -> put_RemoteXobj(This,newVal) ) 

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
            /* [retval][out] */ IXobj **pRetXobj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendXmlMessage( 
            IXobj *sender,
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
            /* [retval][out] */ IXobj **pRetXobj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SendXmlMessage )( 
            IWebPage * This,
            IXobj *sender,
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

#define IWebPage_Observe(This,bstrKey,bstrXml,pRetXobj)	\
    ( (This)->lpVtbl -> Observe(This,bstrKey,bstrXml,pRetXobj) ) 

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
            /* [retval][out] */ IXobj **ppXobj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnloadCosmos( void) = 0;
        
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
            /* [retval][out] */ IXobj **ppXobj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnloadCosmos )( 
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

#define IOfficeObject_Observe(This,bstrKey,bstrXml,ppXobj)	\
    ( (This)->lpVtbl -> Observe(This,bstrKey,bstrXml,ppXobj) ) 

#define IOfficeObject_UnloadCosmos(This)	\
    ( (This)->lpVtbl -> UnloadCosmos(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOfficeObject_INTERFACE_DEFINED__ */


#ifndef __ICosmosCtrl_INTERFACE_DEFINED__
#define __ICosmosCtrl_INTERFACE_DEFINED__

/* interface ICosmosCtrl */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICosmosCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-0601198231DC")
    ICosmosCtrl : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HWND( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Cosmos( 
            /* [retval][out] */ ICosmos **pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_tag( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_tag( 
            /* [in] */ VARIANT newVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICosmosCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICosmosCtrl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICosmosCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICosmosCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICosmosCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICosmosCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICosmosCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICosmosCtrl * This,
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
            ICosmosCtrl * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Cosmos )( 
            ICosmosCtrl * This,
            /* [retval][out] */ ICosmos **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_tag )( 
            ICosmosCtrl * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_tag )( 
            ICosmosCtrl * This,
            /* [in] */ VARIANT newVal);
        
        END_INTERFACE
    } ICosmosCtrlVtbl;

    interface ICosmosCtrl
    {
        CONST_VTBL struct ICosmosCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICosmosCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICosmosCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICosmosCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICosmosCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICosmosCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICosmosCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICosmosCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICosmosCtrl_get_HWND(This,pVal)	\
    ( (This)->lpVtbl -> get_HWND(This,pVal) ) 

#define ICosmosCtrl_get_Cosmos(This,pVal)	\
    ( (This)->lpVtbl -> get_Cosmos(This,pVal) ) 

#define ICosmosCtrl_get_tag(This,pVal)	\
    ( (This)->lpVtbl -> get_tag(This,pVal) ) 

#define ICosmosCtrl_put_tag(This,newVal)	\
    ( (This)->lpVtbl -> put_tag(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICosmosCtrl_INTERFACE_DEFINED__ */


#ifndef __ICosmosAppCtrl_INTERFACE_DEFINED__
#define __ICosmosAppCtrl_INTERFACE_DEFINED__

/* interface ICosmosAppCtrl */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICosmosAppCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060120161001")
    ICosmosAppCtrl : public ICosmosCtrl
    {
    public:
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_AppCtrl( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICosmosAppCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICosmosAppCtrl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICosmosAppCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICosmosAppCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICosmosAppCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICosmosAppCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICosmosAppCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICosmosAppCtrl * This,
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
            ICosmosAppCtrl * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Cosmos )( 
            ICosmosAppCtrl * This,
            /* [retval][out] */ ICosmos **pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_tag )( 
            ICosmosAppCtrl * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_tag )( 
            ICosmosAppCtrl * This,
            /* [in] */ VARIANT newVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AppCtrl )( 
            ICosmosAppCtrl * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        END_INTERFACE
    } ICosmosAppCtrlVtbl;

    interface ICosmosAppCtrl
    {
        CONST_VTBL struct ICosmosAppCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICosmosAppCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICosmosAppCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICosmosAppCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICosmosAppCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICosmosAppCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICosmosAppCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICosmosAppCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICosmosAppCtrl_get_HWND(This,pVal)	\
    ( (This)->lpVtbl -> get_HWND(This,pVal) ) 

#define ICosmosAppCtrl_get_Cosmos(This,pVal)	\
    ( (This)->lpVtbl -> get_Cosmos(This,pVal) ) 

#define ICosmosAppCtrl_get_tag(This,pVal)	\
    ( (This)->lpVtbl -> get_tag(This,pVal) ) 

#define ICosmosAppCtrl_put_tag(This,newVal)	\
    ( (This)->lpVtbl -> put_tag(This,newVal) ) 


#define ICosmosAppCtrl_put_AppCtrl(This,newVal)	\
    ( (This)->lpVtbl -> put_AppCtrl(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICosmosAppCtrl_INTERFACE_DEFINED__ */


#ifndef __IEclipseCtrl_INTERFACE_DEFINED__
#define __IEclipseCtrl_INTERFACE_DEFINED__

/* interface IEclipseCtrl */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IEclipseCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-060120160912")
    IEclipseCtrl : public ICosmosCtrl
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
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveTopXobj( 
            /* [retval][out] */ IXobj **pVal) = 0;
        
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
            /* [retval][out] */ IXobj **ppXobj) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ObserveEx( 
            BSTR bstrGalaxyName,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IXobj **ppXobj) = 0;
        
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
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Cosmos )( 
            IEclipseCtrl * This,
            /* [retval][out] */ ICosmos **pVal);
        
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
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveTopXobj )( 
            IEclipseCtrl * This,
            /* [retval][out] */ IXobj **pVal);
        
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
            /* [retval][out] */ IXobj **ppXobj);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ObserveEx )( 
            IEclipseCtrl * This,
            BSTR bstrGalaxyName,
            BSTR bstrKey,
            BSTR bstrXml,
            /* [retval][out] */ IXobj **ppXobj);
        
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

#define IEclipseCtrl_get_Cosmos(This,pVal)	\
    ( (This)->lpVtbl -> get_Cosmos(This,pVal) ) 

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

#define IEclipseCtrl_get_ActiveTopXobj(This,pVal)	\
    ( (This)->lpVtbl -> get_ActiveTopXobj(This,pVal) ) 

#define IEclipseCtrl_get_AppKeyValue(This,bstrKey,pVal)	\
    ( (This)->lpVtbl -> get_AppKeyValue(This,bstrKey,pVal) ) 

#define IEclipseCtrl_put_AppKeyValue(This,bstrKey,newVal)	\
    ( (This)->lpVtbl -> put_AppKeyValue(This,bstrKey,newVal) ) 

#define IEclipseCtrl_Observe(This,bstrGalaxyName,bstrKey,bstrXml,ppXobj)	\
    ( (This)->lpVtbl -> Observe(This,bstrGalaxyName,bstrKey,bstrXml,ppXobj) ) 

#define IEclipseCtrl_ObserveEx(This,bstrGalaxyName,bstrKey,bstrXml,ppXobj)	\
    ( (This)->lpVtbl -> ObserveEx(This,bstrGalaxyName,bstrKey,bstrXml,ppXobj) ) 

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

#ifndef ___ICosmos_DISPINTERFACE_DEFINED__
#define ___ICosmos_DISPINTERFACE_DEFINED__

/* dispinterface _ICosmos */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ICosmos;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("19631222-1992-0612-1965-060119821002")
    _ICosmos : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ICosmosVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ICosmos * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ICosmos * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ICosmos * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ICosmos * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ICosmos * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ICosmos * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ICosmos * This,
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
    } _ICosmosVtbl;

    interface _ICosmos
    {
        CONST_VTBL struct _ICosmosVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ICosmos_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _ICosmos_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _ICosmos_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _ICosmos_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _ICosmos_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _ICosmos_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _ICosmos_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ICosmos_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Cosmos;

#ifdef __cplusplus

class DECLSPEC_UUID("19631222-1992-0612-1965-060119990606")
Cosmos;
#endif

#ifndef ___IXobjEvents_DISPINTERFACE_DEFINED__
#define ___IXobjEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IXobjEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IXobjEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("19631222-1992-0612-1965-060119821001")
    _IXobjEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IXobjEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IXobjEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IXobjEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IXobjEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IXobjEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IXobjEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IXobjEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IXobjEvents * This,
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
    } _IXobjEventsVtbl;

    interface _IXobjEvents
    {
        CONST_VTBL struct _IXobjEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IXobjEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IXobjEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IXobjEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IXobjEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IXobjEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IXobjEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IXobjEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IXobjEvents_DISPINTERFACE_DEFINED__ */


#ifndef ___ICosmosObjEvents_DISPINTERFACE_DEFINED__
#define ___ICosmosObjEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ICosmosObjEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ICosmosObjEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("19631222-1992-0612-1965-060119821992")
    _ICosmosObjEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ICosmosObjEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ICosmosObjEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ICosmosObjEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ICosmosObjEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ICosmosObjEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ICosmosObjEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ICosmosObjEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ICosmosObjEvents * This,
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
    } _ICosmosObjEventsVtbl;

    interface _ICosmosObjEvents
    {
        CONST_VTBL struct _ICosmosObjEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ICosmosObjEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _ICosmosObjEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _ICosmosObjEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _ICosmosObjEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _ICosmosObjEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _ICosmosObjEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _ICosmosObjEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ICosmosObjEvents_DISPINTERFACE_DEFINED__ */


#ifndef ___ICosmosAppEvents_DISPINTERFACE_DEFINED__
#define ___ICosmosAppEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ICosmosAppEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ICosmosAppEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("19631222-1992-0612-1965-060120160930")
    _ICosmosAppEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ICosmosAppEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ICosmosAppEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ICosmosAppEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ICosmosAppEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ICosmosAppEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ICosmosAppEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ICosmosAppEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ICosmosAppEvents * This,
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
    } _ICosmosAppEventsVtbl;

    interface _ICosmosAppEvents
    {
        CONST_VTBL struct _ICosmosAppEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ICosmosAppEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _ICosmosAppEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _ICosmosAppEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _ICosmosAppEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _ICosmosAppEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _ICosmosAppEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _ICosmosAppEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ICosmosAppEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CosmosCtrl;

#ifdef __cplusplus

class DECLSPEC_UUID("19631222-1992-0612-1965-060120071001")
CosmosCtrl;
#endif

EXTERN_C const CLSID CLSID_CosmosExtender;

#ifdef __cplusplus

class DECLSPEC_UUID("19631222-1992-0612-1965-060100000000")
CosmosExtender;
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


