#include "stdafx.h"
#include "SouiInit.h"
#include "tabctrlex/SBrowserTab.h"
#include "tabctrlex/CDropWnd.h"
#include "tabctrlex/SEditEx.h"
//��PE�ļ����أ�ע����ļ�����·��λ��
#define RES_TYPE 0
//#define SYSRES_TYPE 0
// #define RES_TYPE 0   //PE
// #define RES_TYPE 1   //ZIP
// #define RES_TYPE 2   //7z
// #define RES_TYPE 2   //�ļ�
//ȥ������֧�֣���������Եû���
#if (RES_TYPE==1)
#include "resprovider-zip\zipresprovider-param.h"
#else 
#if (RES_TYPE==2)
#include "resprovider-7zip\zip7resprovider-param.h"
#endif
#endif
#ifdef _DEBUG
#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
#else
#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#endif
#include "SMiniblink.h"
#include <SouiFactory.h>

//����Ψһ��һ��R,UIRES����,ROBJ_IN_CPP��resource.h�ж���ĺꡣ
ROBJ_IN_CPP

void InitDir(TCHAR *Path)
{
	if (Path == NULL)
	{
		TCHAR szCurrentDir[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szCurrentDir, sizeof(szCurrentDir));

		LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
#ifdef _DEBUG
		_tcscpy(lpInsertPos + 1, _T("..\\SweepingEdge"));
#else
		_tcscpy(lpInsertPos + 1, _T("\0"));
#endif
		SetCurrentDirectory(szCurrentDir);
	}
	else
		SetCurrentDirectory(Path);
}

void InitSystemRes(SApplication * theApp,SComMgr *pComMgr)
{
	SouiFactory souiFac;
#ifdef _DEBUG
	//ѡ���˽���Release�汾�����Դ��ϵͳ��Դ��DEBUG��ʼ��ʹ��DLL����
	{
		HMODULE hModSysResource = LoadLibrary(SYS_NAMED_RESOURCE);
		if (hModSysResource)
		{
			SAutoRefPtr<IResProvider> sysResProvider;
			sysResProvider.Attach(souiFac.CreateResProvider(RES_PE));
			sysResProvider->Init((WPARAM)hModSysResource, 0);
			theApp->LoadSystemNamedResource(sysResProvider);
			FreeLibrary(hModSysResource);
		}
		else
		{
			SASSERT(0);
		}
	}
#else		
	//��DLL����ϵͳ��Դ
	{
		HMODULE hModSysResource = LoadLibrary(SYS_NAMED_RESOURCE);
		if (hModSysResource)
		{
			CAutoRefPtr<IResProvider> sysResProvider;
			sysResProvider.Attach(souiFac.CreateResProvider(RES_PE));
			sysResProvider->Init((WPARAM)hModSysResource, 0);
			theApp->LoadSystemNamedResource(sysResProvider);
			FreeLibrary(hModSysResource);
		}
		else
		{
			SASSERT(0);
		}
	}
#endif
}

void InitUserRes(SApplication * theApp, SComMgr *pComMgr)
{
	SouiFactory souiFac;
	CAutoRefPtr<IResProvider>   pResProvider;

#ifdef _DEBUG		
	//ѡ���˽���Release�汾�����Դ����DEBUG��ʼ��ʹ���ļ�����
	{
		pResProvider.Attach(souiFac.CreateResProvider(RES_FILE));
		BOOL bLoaded = pResProvider->Init((LPARAM)_T("uires"), 0);
		SASSERT(bLoaded);
	}
#else
	{
		pResProvider.Attach(souiFac.CreateResProvider(RES_PE));
		BOOL bLoaded = pResProvider->Init((WPARAM)theApp->GetModule(), 0);
		SASSERT(bLoaded);
	}
#endif
	theApp->InitXmlNamedID(namedXmlID, ARRAYSIZE(namedXmlID), TRUE);
	theApp->AddResProvider(pResProvider);
}
