// AviCapTestDoc.cpp : implementation of the CAviCapTestDoc class
//

#include "stdafx.h"
#include "AviCapTest.h"
#include "AviCapTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAviCapTestDoc

IMPLEMENT_DYNCREATE(CAviCapTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CAviCapTestDoc, CDocument)
	//{{AFX_MSG_MAP(CAviCapTestDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAviCapTestDoc construction/destruction

CAviCapTestDoc::CAviCapTestDoc()
{
	// TODO: add one-time construction code here

}

CAviCapTestDoc::~CAviCapTestDoc()
{
}

BOOL CAviCapTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAviCapTestDoc serialization

void CAviCapTestDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CAviCapTestDoc diagnostics

#ifdef _DEBUG
void CAviCapTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAviCapTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAviCapTestDoc commands
