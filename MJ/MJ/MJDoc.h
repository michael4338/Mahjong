// MJDoc.h : interface of the CMJDoc class
//


#pragma once


class CMJDoc : public CDocument
{
protected: // create from serialization only
	CMJDoc();
	DECLARE_DYNCREATE(CMJDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CMJDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


