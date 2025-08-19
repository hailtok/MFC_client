#pragma once
#include <afxwin.h>
#include <afxdlgs.h>
#include <afxdialogex.h>
//¥Î©ó½m²ßmodeless dialog
class CModelessWinsDialog : public CDialog {
private:
    DLGTEMPLATE* CreateDialogTemplate();
protected:
    virtual BOOL OnInitDialog();
    virtual void OnCancel()override;
    
    virtual void PostNcDestroy()override;
    afx_msg void OnDestroy();
    DECLARE_MESSAGE_MAP()
public:
    CModelessWinsDialog(CWnd* pParent=nullptr);
    virtual ~CModelessWinsDialog();
    DLGTEMPLATE* pTemplate;
};