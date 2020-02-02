#pragma once
#include "CDropWnd.h"
#include "SBrowserTab.h"
class CDropWindow :
	public SHostWnd
{
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	
	SPrintWindow *m_PrintWnd;
public:
	void CopyDropWindBkgndFormWindow(SPrintWindow *FormWindow)
	{
		/*m_PrintWnd = FormWindow;
		IRenderTarget *pRt = GetRenderTarget(NULL,0,FALSE);
		m_PrintWnd->Print(pRt);
		ReleaseRenderTarget(pRt);*/
		//UpdateLayerFromRenderTarget(m_memRT, m_hostAttr.m_byAlpha, GetWindowRect());
		FindChildByName2<CDropWnd>(L"dropwindow")->CopyDropWindBkgndFormWindow(FormWindow);
	}
	void close()
	{
		SNativeWnd::DestroyWindow();
 	}

	CDropWindow();
	~CDropWindow(); 
	BEGIN_MSG_MAP_EX(CDropWindow)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_INITDIALOG(OnInitDialog)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
private:
	BOOL			m_bLayoutInited;
};

