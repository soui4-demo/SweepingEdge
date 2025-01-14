// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "SMiniblink.h"
#include "tabctrlex/SBrowserTab.h"
#include "tabctrlex/SEditEx.h"
#include "tabctrlex/DropWindow.h"
class CMainDlg : public SHostWnd, public IHostInterface
{
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, CSize size);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
// 通过 IHostInterface 继承
	virtual void SetAutoNewCreateTab(bool) override;
	virtual void InsertTab(SBrowserTab * tab, STabPage * page) override;
	virtual bool CreateHostWnd() override;
	virtual void InitDialog() override;
	virtual void SetPos(CPoint &) override;
	void OnUpdateWebBrowserTab();
protected:
	void            OnWebBrowserTabNew(EventArgs *pEvt);
	void            OnWebBrowserTabDel(EventArgs *pEvt);
	void            OnWebBrowserTabSel(EventArgs *pEvt);
	void			OnWkeTitleChanged(EventArgs *pEvt);
	void			OnWkeUrlChanged(EventArgs *pEvt);
	void			OnWkeNewNav(EventArgs *pEvt);
	void			OnGoAddress();
	void			OnGoAddress(EventArgs *pEvt);
	void			OnRefresh();
	void			OnGoBack();
	void			OnGoForward();
	inline	SWkeWebkit*	GetCurMB();
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_HANDLER(L"WebBrowserTab", EVT_WEBTAB_NEW, OnWebBrowserTabNew)
		EVENT_NAME_HANDLER(L"WebBrowserTab", EVT_WEBTAB_CLOSE, OnWebBrowserTabDel)
		EVENT_NAME_HANDLER(L"WebBrowserTab", EVT_WEBTAB_SELCHANGED, OnWebBrowserTabSel)
		EVENT_HANDLER(EVT_WKE_UPDATATITLE,OnWkeTitleChanged)
		EVENT_HANDLER(EVT_WKE_UPDATAURL, OnWkeUrlChanged)
		EVENT_HANDLER(EVT_WKE_NEW_NAV, OnWkeNewNav)
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		EVENT_NAME_COMMAND(L"btn_goaddress", OnGoAddress)
		EVENT_NAME_HANDLER(L"edt_address", EVT_EDIT_KEYRETURN, OnGoAddress)
		EVENT_NAME_COMMAND(L"btn_refresh", OnRefresh)
		EVENT_NAME_COMMAND(L"btn_webnav_back", OnGoBack)
		EVENT_NAME_COMMAND(L"btn_webnav_forward", OnGoForward)

	EVENT_MAP_END()
		
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
private:
	BOOL			m_bLayoutInited;

	SBrowserTabCtrl *m_pWebTab;
	SRelTabCtrl     *m_pTabWebPage;
	SEdit			*m_pAddEt;
	int				iPage;
	bool			m_bAutoCreate;
};
