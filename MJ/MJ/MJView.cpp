// MJView.cpp : implementation of the CMJView class
//

#include "stdafx.h"
#include "MJ.h"

#include "MJDoc.h"
#include "MJView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMJView

IMPLEMENT_DYNCREATE(CMJView, CFormView)

BEGIN_MESSAGE_MAP(CMJView, CFormView)
END_MESSAGE_MAP()

// CMJView construction/destruction

CMJView::CMJView()
	: CFormView(CMJView::IDD)
{
	// TODO: add construction code here

}

CMJView::~CMJView()
{
}

void CMJView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CMJView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CMJView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}


// CMJView diagnostics

#ifdef _DEBUG
void CMJView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMJView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMJDoc* CMJView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMJDoc)));
	return (CMJDoc*)m_pDocument;
}
#endif //_DEBUG


// CMJView message handlers
