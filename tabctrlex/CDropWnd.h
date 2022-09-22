#pragma once
#include "core\Swnd.h"
#include "SBrowserTab.h"

class CDropWnd : public SWindow
{
	DEF_SOBJECT(SWindow, L"dropwindow")
	SAutoRefPtr<IRenderTarget> m_memRT;
protected:
	void OnPaint(IRenderTarget *pRT);
public:
	void CopyDropWindBkgndFormWindow(SPrintWindow *FormWindow);
	SOUI_MSG_MAP_BEGIN()
		MSG_WM_PAINT_EX(OnPaint)
	SOUI_MSG_MAP_END()
};