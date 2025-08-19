#include"modeless_windows.h"
BEGIN_MESSAGE_MAP(CModelessWinsDialog, CDialog)
    ON_WM_DESTROY()
END_MESSAGE_MAP()
CModelessWinsDialog::CModelessWinsDialog(CWnd* pParent):CDialog(nullptr,pParent) {
    pTemplate=CreateDialogTemplate();
}
DLGTEMPLATE* CModelessWinsDialog::CreateDialogTemplate() {
    // �t�m�O����
    BYTE* pMem = new BYTE[1024];
    ZeroMemory(pMem, 1024);

    DLGTEMPLATE* pDlg = (DLGTEMPLATE*)pMem;

    // �򥻹�ܮؼ˦�
    pDlg->style = DS_SETFONT | DS_CENTER | WS_VISIBLE | WS_CAPTION | WS_SYSMENU;
    pDlg->cx = 205;
    pDlg->cy = 150;

    BYTE* p = (BYTE*)(pDlg + 1);

    // �L menu
    *(WORD*)p = 0; p += sizeof(WORD);

    // �ϥιw�] dialog class
    *(WORD*)p = 0; p += sizeof(WORD);

    // Caption �r��
    wcscpy_s((WCHAR*)p, 32, L"MODELESS");
    p += (wcslen(L"MODELESS") + 1) * sizeof(WCHAR);

    // Font �]�w
    *(WORD*)p = 9;  p += sizeof(WORD); // �r��j�p
    wcscpy_s((WCHAR*)p, 32, L"MS Shell Dlg");

    return pDlg;
}
BOOL CModelessWinsDialog::OnInitDialog()
{
    return CDialog::OnInitDialog();
}
void CModelessWinsDialog::OnCancel() {
    DestroyWindow();
}
void CModelessWinsDialog::OnDestroy() {
    CDialog::OnDestroy();
}
void CModelessWinsDialog::PostNcDestroy() {
    delete this;
}
CModelessWinsDialog::~CModelessWinsDialog() {
    delete pTemplate;
}