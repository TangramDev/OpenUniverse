

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0613 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for TangramWinRT.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0613 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
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

#ifndef __TangramWinRTProxy_h__
#define __TangramWinRTProxy_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IHubbleWinRT_FWD_DEFINED__
#define __IHubbleWinRT_FWD_DEFINED__
typedef interface IHubbleWinRT IHubbleWinRT;

#endif 	/* __IHubbleWinRT_FWD_DEFINED__ */


#ifndef __ISmartAppliance_FWD_DEFINED__
#define __ISmartAppliance_FWD_DEFINED__
typedef interface ISmartAppliance ISmartAppliance;

#endif 	/* __ISmartAppliance_FWD_DEFINED__ */


#ifndef __TangramWinRT_FWD_DEFINED__
#define __TangramWinRT_FWD_DEFINED__

#ifdef __cplusplus
typedef class TangramWinRT TangramWinRT;
#else
typedef struct TangramWinRT TangramWinRT;
#endif /* __cplusplus */

#endif 	/* __TangramWinRT_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IHubbleWinRT_INTERFACE_DEFINED__
#define __IHubbleWinRT_INTERFACE_DEFINED__

/* interface IHubbleWinRT */
/* [object][version][uuid] */ 


EXTERN_C const IID IID_IHubbleWinRT;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-0601198253EC")
    IHubbleWinRT : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateJSRuntime( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TurnOff( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IHubbleWinRTVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHubbleWinRT * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHubbleWinRT * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHubbleWinRT * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateJSRuntime )( 
            IHubbleWinRT * This);
        
        HRESULT ( STDMETHODCALLTYPE *TurnOff )( 
            IHubbleWinRT * This);
        
        END_INTERFACE
    } IHubbleWinRTVtbl;

    interface IHubbleWinRT
    {
        CONST_VTBL struct IHubbleWinRTVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHubbleWinRT_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHubbleWinRT_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHubbleWinRT_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHubbleWinRT_CreateJSRuntime(This)	\
    ( (This)->lpVtbl -> CreateJSRuntime(This) ) 

#define IHubbleWinRT_TurnOff(This)	\
    ( (This)->lpVtbl -> TurnOff(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHubbleWinRT_INTERFACE_DEFINED__ */


#ifndef __ISmartAppliance_INTERFACE_DEFINED__
#define __ISmartAppliance_INTERFACE_DEFINED__

/* interface ISmartAppliance */
/* [object][version][uuid] */ 


EXTERN_C const IID IID_ISmartAppliance;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19631222-1992-0612-1965-0601198224D7")
    ISmartAppliance : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RemoteTurnOn( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoteTurnOff( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISmartApplianceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISmartAppliance * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISmartAppliance * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISmartAppliance * This);
        
        HRESULT ( STDMETHODCALLTYPE *RemoteTurnOn )( 
            ISmartAppliance * This);
        
        HRESULT ( STDMETHODCALLTYPE *RemoteTurnOff )( 
            ISmartAppliance * This);
        
        END_INTERFACE
    } ISmartApplianceVtbl;

    interface ISmartAppliance
    {
        CONST_VTBL struct ISmartApplianceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISmartAppliance_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISmartAppliance_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISmartAppliance_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISmartAppliance_RemoteTurnOn(This)	\
    ( (This)->lpVtbl -> RemoteTurnOn(This) ) 

#define ISmartAppliance_RemoteTurnOff(This)	\
    ( (This)->lpVtbl -> RemoteTurnOff(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISmartAppliance_INTERFACE_DEFINED__ */



#ifndef __TangramWinRTLib_LIBRARY_DEFINED__
#define __TangramWinRTLib_LIBRARY_DEFINED__

/* library TangramWinRTLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_UniverseWinRTLib;

EXTERN_C const CLSID CLSID_TangramWinRT;

#ifdef __cplusplus

class DECLSPEC_UUID("19631222-1992-0612-1965-060119822008")
TangramWinRT;
#endif
#endif /* __TangramWinRTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


