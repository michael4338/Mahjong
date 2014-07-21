// MJDoc.cpp : implementation of the CMJDoc class
//

#include "stdafx.h"
#include "MJ.h"

#include "MJDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMJDoc

IMPLEMENT_DYNCREATE(CMJDoc, CDocument)

BEGIN_MESSAGE_MAP(CMJDoc, CDocument)
END_MESSAGE_MAP()


// CMJDoc construction/destruction

CMJDoc::CMJDoc()
{
	// TODO: add one-time construction code here

}

CMJDoc::~CMJDoc()
{
}

BOOL CMJDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMJDoc serialization

void CMJDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CMJDoc diagnostics

#ifdef _DEBUG
void CMJDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMJDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMJDoc commands
