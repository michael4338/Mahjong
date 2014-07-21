// MJ.h : main header file for the MJ application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMJApp:
// See MJ.cpp for the implementation of this class
//

class CMJApp : public CWinApp
{
public:
	CMJApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMJApp theApp;