// HookBHO.h : Declaration of the CHookBHO

#pragma once
#include "resource.h"       // main symbols

#include <ShlGuid.h>
#include <ExDispid.h>

#include "Hook_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CHookBHO

class ATL_NO_VTABLE CHookBHO :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHookBHO, &CLSID_HookBHO>,
	public IObjectWithSiteImpl<CHookBHO>,
	public IDispatchImpl<IHookBHO, &IID_IHookBHO, &LIBID_HookLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispEventImpl<1, CHookBHO, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 1>
{
public:
	CHookBHO()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HOOKBHO)

DECLARE_NOT_AGGREGATABLE(CHookBHO)

BEGIN_COM_MAP(CHookBHO)
	COM_INTERFACE_ENTRY(IHookBHO)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(SetSite)(IUnknown *pUnkSite);

BEGIN_SINK_MAP(CHookBHO)
	//SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
	SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOWNLOADCOMPLETE, OnDownloadComplete)
END_SINK_MAP()

	void STDMETHODCALLTYPE OnDocumentComplete(IDispatch *pDisp, VARIANT *pvarURL);
	void STDMETHODCALLTYPE OnDownloadComplete(IDispatch *pDisp, VARIANT *pvarURL);

private: 
	CComPtr<IWebBrowser2> m_spWebBrowser;
	BOOL m_fAdvised;
};

OBJECT_ENTRY_AUTO(__uuidof(HookBHO), CHookBHO)
