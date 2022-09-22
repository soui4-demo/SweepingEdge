#pragma once
#include <control/SCmnCtrl.h>

namespace SOUI
{
#define EVT_EDIT_KEYRETURN (EVT_EXTERNAL_BEGIN + 600)

	DEF_EVT_EXT(EventEditKeyReturn,EVT_EDIT_KEYRETURN,{int fake;});

	class SEditEx
		:public SEdit
	{
		DEF_SOBJECT(SEdit, L"editex")
	public:
		SEditEx();

	protected:

		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

		SOUI_MSG_MAP_BEGIN()
			MSG_WM_KEYDOWN(OnKeyDown)
		SOUI_MSG_MAP_END()
	};
}