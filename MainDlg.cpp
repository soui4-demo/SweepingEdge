// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"	
#include "tabctrlex/SBrowserTab.h"	
#include "SMiniblink.h"

LONG wndCount = 0;

#define MAX_BROWSER_TABS 10

CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND")),m_pWebTab(NULL),m_pTabWebPage(NULL),m_pAddEt(NULL),iPage(0),m_bAutoCreate(true)
{
	m_bLayoutInited = FALSE;
}

CMainDlg::~CMainDlg()
{
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	LONG count = InterlockedIncrement(&wndCount);

	ModifyStyle(WS_BORDER, 0);
	HRESULT hr = ::RegisterDragDrop(m_hWnd, GetDropTarget());
	m_pWebTab = FindChildByName2<SBrowserTabCtrl>(L"WebBrowserTab");
	m_pWebTab->RegisterHost(SHostFactory<CMainDlg>());
	m_pTabWebPage = FindChildByName2<SRelTabCtrl>(L"tab_webpage");
	SASSERT(m_pTabWebPage);
	m_pWebTab->SetTabCtrl(m_pTabWebPage);	
	m_pAddEt = FindChildByName2<SEdit>(L"edt_address");

	if(m_bAutoCreate)
		m_pWebTab->InsertTab(_T("csdn"), _T("https://blog.csdn.net/weolar/article/details/80458523"), 0);	
	return 0;
}

void CMainDlg::SetAutoNewCreateTab(bool bAutoCreate)
{
	m_bAutoCreate=bAutoCreate;
}

void CMainDlg::InsertTab(SBrowserTab * tab, STabPage * page)
{
	if ((tab != NULL) && m_pWebTab != NULL)
	{	
		if (m_pWebTab->InsertTabEx(tab, page))
			++iPage;
		OnUpdateWebBrowserTab();
	}
}

bool CMainDlg::CreateHostWnd()
{
	return ::IsWindow(Create(NULL)) == TRUE;
}

void CMainDlg::InitDialog()
{
	SendMessage(WM_INITDIALOG);
}

void CMainDlg::SetPos(CPoint &curPos)
{
	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rc, 0);
	if (curPos.y > rc.bottom - 100)
	{
		curPos.y = rc.bottom - 100;
	}
	if (curPos.x > rc.right - 300)
	{
		curPos.x = rc.right - 300;
	}
	SetWindowPos(NULL, curPos.x - 100, curPos.y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
}

void CMainDlg::OnUpdateWebBrowserTab()
{
	if (m_pWebTab) {
		CRect rcMain = GetWindowRect();
		SStringW strPos;
		if ((iPage * 200 + 36) < (rcMain.Width() - 10)) {
			strPos.Format(_T("5,-30,%d,@30"), iPage * 200 + 36 + 75);
			m_pWebTab->SetAttribute(_T("pos"), strPos);
		}
		else {
			m_pWebTab->SetAttribute(_T("pos"), _T("5,-30,-5,@30"));
		}		
		GetRoot()->RequestRelayout(m_pWebTab->GetSwnd(),TRUE);
	}
}

void CMainDlg::OnWebBrowserTabNew(EventArgs * pEvt)
{
		EventBrowserTabNew *pEvtTabNew = (EventBrowserTabNew*)pEvt;
		iPage++;
		SStringT strTitle;
		if (iPage > MAX_BROWSER_TABS) {
			m_pWebTab->RemoveTab(iPage - 1);
			iPage--;
			return;
		}

		SBrowserTab *pEdgeTab = (SBrowserTab*)pEvtTabNew->pNewTab;

		if (pEvtTabNew->pszTitle == NULL) {
			strTitle = SStringT().Format(_T("新建标签页-%d"), iPage);
		}
		else {
			strTitle = SStringT().Format(_T("%s"), pEvtTabNew->pszTitle);
		}
		pEdgeTab->SetTitle(strTitle);
		pEvtTabNew->pNewTab->SetAttribute(L"tip", S_CT2W(strTitle));

		if (m_pWebTab == NULL)
			return;

		int nPageCount = m_pWebTab->GetTabCount();
		//m_pWebTab->SetCurSel(iPage - 1);
		SStringT strPage;
		strPage.Format(_T("(<page title=\"InsertItem\" id=\"%d\"><wkeWebkit size=\"full,full\" colorBkgnd=\"#ffffff\" id=\"%d\"/></page>)"), pEdgeTab->GetTabID(),pEdgeTab->GetTabID());
		if (m_pTabWebPage == NULL)
			return;

		int nIndex = m_pTabWebPage->InsertItem(strPage);
		if (nIndex != -1)
		{
			pEvtTabNew->pData;
			SStringT strTitle;
			
			SWkeWebkit  *pWebHostWnd = (m_pTabWebPage->FindChildByID(pEdgeTab->GetTabID()))->FindChildByID2<SWkeWebkit>(pEdgeTab->GetTabID());

			SASSERT(pWebHostWnd);
			pWebHostWnd->LoadUrl((LPCTSTR)pEvtTabNew->pData);
			m_pTabWebPage->SetCurSel(nIndex);
		}	
		OnUpdateWebBrowserTab();
}

void CMainDlg::OnWebBrowserTabDel(EventArgs * pEvt)
{
	EventBrowserTabClose *pEvtTabClose = (EventBrowserTabClose*)pEvt;

	if (m_pWebTab) {		
		m_pTabWebPage->RemoveItem(m_pTabWebPage->GetIndexById(pEvtTabClose->iCloseTabId));
	}
}

void CMainDlg::OnWebBrowserTabSel(EventArgs * pEvt)
{	
	EventBrowserTabSelChanged *pEvtTabChange = (EventBrowserTabSelChanged*)pEvt;
	
	m_pTabWebPage->SetCurSelById(m_pWebTab->GetTabIdByIndex(pEvtTabChange->iNewSel));

	SWkeWebkit  *pWebHostWnd =GetCurMB();
	m_pAddEt->SetWindowText(pWebHostWnd->GetUrl());
}

void CMainDlg::OnWkeTitleChanged(EventArgs * pEvt)
{
	EventBrowserTitleChanged* pEv = (EventBrowserTitleChanged*)pEvt;
	m_pWebTab->UpdateTabTitle(pEv->iId, pEv->pszTitle);
}

void CMainDlg::OnWkeUrlChanged(EventArgs * pEvt)
{
	EventBrowserUrlChanged* pEv = (EventBrowserUrlChanged*)pEvt;

	if (m_pWebTab->GetSelTabId()== pEv->iId)
	{
		//m_pAddEt->SetWindowText(pEv->pszUrl);
	}
}

void CMainDlg::OnWkeNewNav(EventArgs * pEvt)
{
	EventBrowserNewNav *pEv = (EventBrowserNewNav*)pEvt;
	m_pWebTab->InsertTab(_T("new"), (void*)pEv->pszUrl, -1);
	pEv->pRetView = GetCurMB()->GetWebView();
}

void CMainDlg::OnGoAddress()
{	
	SWkeWebkit  *pWebHostWnd = GetCurMB();
	pWebHostWnd->StopLoading();
	pWebHostWnd->LoadUrl(m_pAddEt->GetWindowText());
}

void CMainDlg::OnGoAddress(EventArgs * pEvt)
{
	OnGoAddress();
}

void CMainDlg::OnRefresh()
{	
	SWkeWebkit  *pWebHostWnd = GetCurMB();
	pWebHostWnd->ReLoad();
}

void CMainDlg::OnGoBack()
{
	SWkeWebkit  *pWebHostWnd = GetCurMB();
	pWebHostWnd->GoBack();
}

void CMainDlg::OnGoForward()
{
	SWkeWebkit  *pWebHostWnd = GetCurMB();
	pWebHostWnd->GoForward();
}

inline SWkeWebkit * CMainDlg::GetCurMB()
{
	SWindow* pPage = (STabPage *)m_pTabWebPage->GetPage(m_pTabWebPage->GetCurSel());	
	return  pPage->FindChildByID2<SWkeWebkit>(pPage->GetID());;
}
//*/

//TODO:消息映射
void CMainDlg::OnClose()
{

	if (m_pWebTab->GetTabCount() > 1) {		
		if (SMessageBox(GetHostHwnd(), L"还有其它标签未关闭，确定要退出吗？",L"提示",MB_OKCANCEL) != IDOK)
			return;
	}
	m_pWebTab->CloseAllTab();

	SNativeWnd::DestroyWindow();
	if (InterlockedDecrement(&wndCount) == 0)
	{
		PostQuitMessage(0);
	}
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}

void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}

void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
	OnUpdateWebBrowserTab();
}

