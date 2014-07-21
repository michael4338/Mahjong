// MJView.h : interface of the CMJView class
//


#pragma once


class CMJView : public CFormView
{
protected: // create from serialization only
	CMJView();
	DECLARE_DYNCREATE(CMJView)

public:
	enum{ IDD = IDD_MJ_FORM };

// Attributes
public:
	CMJDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CMJView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MJView.cpp
inline CMJDoc* CMJView::GetDocument() const
   { return reinterpret_cast<CMJDoc*>(m_pDocument); }
#endif

