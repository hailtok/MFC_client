#pragma once
#include <afxwin.h>
#include <afxdlgs.h>
#include <afxdialogex.h>
#include"modal_windows.h"
#include"modeless_windows.h"
//此class用於展示modeless和modal視窗，主要為練習、測試用
#define MODAL_BUTTON 400
#define MODELESS_BUTTTON 401
class CMainDialog : public CDialog {
private:
    DLGTEMPLATE* CreateDialogTemplate();
protected:
    virtual BOOL OnInitDialog()override;
    afx_msg void OnModalButtonClicked();
    afx_msg void OnModelessButtonClicked();
    DECLARE_MESSAGE_MAP()
public:
    CMainDialog();
    CMainDialog(UINT, CWnd* pParent = nullptr);
    virtual ~CMainDialog();
    DLGTEMPLATE* pTemplate;
    CModelessWinsDialog* pModelessDialog;
    MyDialog* mydialog;
    CButton m_btnLeft;
    CButton m_btnRight;
};
