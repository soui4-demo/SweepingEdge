#include "SMiniblink.h"
#include "tabctrlex/SBrowserTab.h"
#include "tabctrlex/SEditEx.h"
#include "tabctrlex/DropWindow.h"

//Soui 加载器
class CSouiLoader
{
	SApplication *theApp;
	SComMgr *pComMgr;
public:

	void RegisterWindows() const
	{	
#define RegWnd(wndClass) theApp->RegisterWindowClass<wndClass>();
		RegWnd(SBrowserTabCtrl)	
			RegWnd(CDropWnd)
			RegWnd(SRelTabCtrl)
			RegWnd(SWkeWebkit)
			RegWnd(SEditEx)
	}

	CSouiLoader(HINSTANCE hInst, LPCTSTR pszHostClassName = _T("SOUIHOST")) 
		:theApp(NULL),pComMgr(NULL)
	{		
		pComMgr = new SComMgr;
		CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
		CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
		BOOL bLoaded = pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
		SASSERT_FMT(bLoaded, _T("load interface [render] failed!"));
		bLoaded = pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
		SASSERT_FMT(bLoaded, _T("load interface [%s] failed!"), _T("imgdecoder"));
		pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
		theApp = new SApplication(pRenderFactory, hInst, pszHostClassName);

		RegisterWindows();
	}
	~CSouiLoader()
	{
		if (theApp)
			delete theApp;
		if (pComMgr)
			delete pComMgr;
	}
	SApplication *GetApp()
	{
		SASSERT(theApp);
		return theApp;
	}
	SComMgr *GetComMgr()
	{
		SASSERT(pComMgr);
		return pComMgr;
	}
};
//初使化资源加载路径
void InitDir(TCHAR *Path=NULL);

void InitSystemRes(SApplication *theApp, SComMgr *pComMgr);

void InitUserRes(SApplication * theApp, SComMgr *pComMgr);

template<class T>
int Run(SApplication *theApp)
{
	T dlgMain;
	dlgMain.Create(GetActiveWindow(),WS_POPUPWINDOW);
	dlgMain.SendMessage(WM_INITDIALOG);
	dlgMain.CenterWindow(dlgMain.m_hWnd);
	dlgMain.ShowWindow(SW_SHOWNORMAL);
	return theApp->Run(dlgMain.m_hWnd);
}

