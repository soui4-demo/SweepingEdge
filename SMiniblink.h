#pragma once
#include "../mblib/wke.h"
SNSBEGIN

class SWkeLoader
    {    
    public:
        SWkeLoader();
        ~SWkeLoader();
	};

	//class jsFun<class>

	class SWkeJsFunBinder {
		/*jsValue js_msgBox(jsExecState es,void *param)
		{
			SWkeWebkit *_THIS = (SWkeWebkit*)param;
		}*/
	};

#define EVT_WKE_BEGIN (EVT_EXTERNAL_BEGIN + 500)
#define EVT_WKE_UPDATATITLE (EVT_WKE_BEGIN + 1)
#define EVT_WKE_UPDATAURL (EVT_WKE_BEGIN + 2)
#define EVT_WKE_NEW_NAV (EVT_WKE_BEGIN + 3)

	DEF_EVT_EXT(EventBrowserTitleChanged,EVT_WKE_UPDATATITLE,{
		LPCTSTR		pszTitle;
		int       iId;
	});

	DEF_EVT_EXT(EventBrowserUrlChanged,EVT_WKE_UPDATAURL,{
		LPCTSTR		pszUrl;
		int			iId;
	});


	DEF_EVT_EXT(EventBrowserNewNav,EVT_WKE_NEW_NAV,{
		LPCTSTR		pszUrl;
		int			iId;
		wkeWebView pRetView;
	});

    class SWkeWebkit : public SWindow
    {
        DEF_SOBJECT(SWindow, L"wkeWebkit")
    public:
        SWkeWebkit(void);
        ~SWkeWebkit(void);
		wkeWebView	GetWebView() { return m_pWebView; }
		//必须在创建之前调用
		static void BindJsToWke(const char* jsFunName, wkeJsNativeFunction nativeFun, int argc, void *evtbinder);
		SStringW RunJSW(SStringT strValue);
		SStringA RunJSA(SStringT strValue);
		void LoadUrl(LPCTSTR url);
		SStringT GetUrl();
		void ReLoad();
		bool DownloadCallback(wkeWebView webView, const char* url);
		void URLChangedCallback(wkeWebView webView, const wkeString url);
		void TitleChangedCallback(wkeWebView webView, const wkeString url);
		wkeWebView CreateViewCallback(wkeWebView webView, wkeNavigationType navigationType, const wkeString url, const wkeWindowFeatures* windowFeatures);
		void StopLoading();
		bool GoBack();
		bool GoForward();
		bool GoCanBack();
		bool GoCanForward();
    protected:
		void BindCallBackFunToWeb();

		LPCTSTR wkeCursor(int wekInfId);
        int OnCreate(void *);
        void OnDestroy();
        void OnPaint(IRenderTarget *pRT);
        void OnSize(UINT nType, CSize size);
        LRESULT OnMouseEvent(UINT uMsg, WPARAM wParam,LPARAM lParam); 
        LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam,LPARAM lParam); 
        LRESULT OnKeyDown(UINT uMsg, WPARAM wParam,LPARAM lParam);
        LRESULT OnKeyUp(UINT uMsg, WPARAM wParam,LPARAM lParam);
        LRESULT OnChar(UINT uMsg, WPARAM wParam,LPARAM lParam);
        LRESULT OnImeStartComposition(UINT uMsg, WPARAM wParam,LPARAM lParam);
        void OnSetFocus(SWND wndOld);
        void OnKillFocus(SWND wndFocus);        
        virtual BOOL OnSetCursor(const CPoint &pt);
        virtual UINT OnGetDlgCode(){return SC_WANTALLKEYS;}
        BOOL OnAttrUrl(SStringW strValue, BOOL bLoading);
        SOUI_ATTRS_BEGIN()
            ATTR_CUSTOM(L"url",OnAttrUrl)
        SOUI_ATTRS_END()

        SOUI_MSG_MAP_BEGIN()
            MSG_WM_PAINT_EX(OnPaint)
            MSG_WM_CREATE(OnCreate)
            MSG_WM_DESTROY(OnDestroy)
            MSG_WM_SIZE(OnSize)
            MSG_WM_SETFOCUS_EX(OnSetFocus)
            MSG_WM_KILLFOCUS_EX(OnKillFocus)
            MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST,0x209,OnMouseEvent)
            MESSAGE_HANDLER_EX(WM_MOUSEWHEEL,OnMouseWheel)
            MESSAGE_HANDLER_EX(WM_KEYDOWN,OnKeyDown)
            MESSAGE_HANDLER_EX(WM_KEYUP,OnKeyUp)
            MESSAGE_HANDLER_EX(WM_CHAR,OnChar)
            MESSAGE_HANDLER_EX(WM_IME_STARTCOMPOSITION,OnImeStartComposition)
        SOUI_MSG_MAP_END()
    protected:
		wkeWebView m_pWebView;
        SStringW m_strUrl;
		CSize m_szRtSize;
    };

SNSEND