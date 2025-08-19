#pragma once
#include <afxwin.h>
#include <afxdlgs.h>
#include <afxdialogex.h>
#include<afxmt.h>
#include<afxsock.h>
#include<fstream>
#include<sstream>
#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include"ini_manager.h"
#include"SharedMemory.h"
#include"sockets.h"
//�o��class�D�n�O�Ω�B�z�Ϥ���T�������A�ñN���G�^�Ǧܭ쥻��������
//UI���ƶq�MID
const int listbox_count = 1;
const int combobox_count = 2;
const int static_count = 12;
const int edit_count = 6;
const int string_count = 8;
const int button_count = 8;
#define ID_LISTBOX 200
#define LIGHTSETTING_COMBOBOX 201
#define INSPECTIONSETTING_COMBOBOX 202
#define LANGUAGE_SETTING_COMBOBOX 203

#define BUTTON_BASE 300
#define SIZE_FILTER_BUTTON 300
#define DEFECT_TYPE_LABEL_BUTTON 301
#define DEFECT_TYPE_REJECTOR_BUTTON 302
#define RESERVED_BUTTON 303
#define INPUT_BUTTON 304
#define CANCEL_BUTTON 305
#define SAVE_BUTTON 306
#define CLOSE_BUTTON 307

#define STATIC_BASE 400
#define RECIPE_ID_STATIC 400
#define REF_RECIPE_ID_STATIC 401
#define RECIPE_NAME_STATIC 402
#define MATERIAL_WIDTH_STATIC 403
#define SHEET_LENGTH_STATIC 404
#define OFFSET_STATIC 405
#define LIGHT_SETTING_STATIC 406
#define INSPECTION_SETTING_STATIC 407
#define SIZE_FILTER_STATIC 408
#define DEFECT_TYPE_LABEL_STATIC 409
#define DEFECT_TYPE_REJECTOR_STATIC 410
#define RESERVED_STATIC 411
#define BMP_STATIC 412

#define EDIT_BASE 500
#define RECIPE_ID_EDIT 500
#define REF_RECIPE_ID_EDIT 501
#define RECIPE_NAME_EDIT 502
#define MATERIAL_WIDTH_EDIT 503
#define SHEET_LENGTH_EDIT 504
#define OFFSET_EDIT 505
//�Ω�q��MyDialog�@�ɰO�Хi�HŪ��
#define WM_SOCKET_NOTIFY (WM_USER + 1)
//�Ω��קK�t�@�䪺�{���b�o��|���B�z���A�æA��Ū���@�ɰO����
static CCriticalSection cs;
class CMyDialog : public CDialogEx
{
private:
	//�Ыع�ܮؼҪO
    DLGTEMPLATE* CreateDialogTemplate();
	//Ū��ini�ɮפ�k�A�U����Ӥ�k�D�n�t�bŪ�����Pini�ɮת��榡�A���O��UTF16�MUTF8
    void Read_ini_file_method1();//UTF16�榡
    void Read_ini_file_method2();//UTF8�榡
	//�g�Jini�ɮפ�k�A�U����Ӥ�k�D�n�t�b�g�J���Pini�ɮת��榡�A���O��UTF16�MUTF8
    void Write_ini_file_method1();//UTF16�榡
    void Write_ini_file_method2();//UTF8�榡
	//�qini�ɮסAŪ��UI���y��
    void Read_UI_language_ini_file();
    //�H�U�|�ӳ����eUI��function
    void DrawListBoxs(int, LPDRAWITEMSTRUCT);
    void DrawStatics(int, LPDRAWITEMSTRUCT);
    void DrawButtons(int, LPDRAWITEMSTRUCT);
    void DrawEdits(int, LPDRAWITEMSTRUCT);
	//�M��listbox�A�í��s�N��ƶ�J
    void fill_empty_listbox();
public:
    CMyDialog();//constrctor
    CMyDialog(UINT,CWnd* pParent = nullptr);//constrctor
    virtual ~CMyDialog();//deconstructor
protected:
    virtual BOOL OnInitDialog() override;
	void OnLbnSelChangeList();//�B�zlistbox����ܧ��ܨƥ�
    afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);//�B�zlistbox�I���C��P���
    afx_msg void ButtonsClicked_Event(UINT);
	//�b�B�z����N�B�z������T�Ǭ��^�ܭ쥻�������A�o�䪽���N�^�ǫ��s�g�bInput���s��
    afx_msg void Input_Button_Event();
    afx_msg void Cancel_Button_Event();
    //�Ω�q��MyDialog�@�ɰO�Хi�HŪ�����ƥ�
    afx_msg LRESULT OnSocketNotify(WPARAM wParam, LPARAM lParam);
    //�m��WM_COPYDATA
    afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
    DECLARE_MESSAGE_MAP()
private:
    HANDLE shared_file;
    std::vector<CString>keys = { _T("RECIPE_ID_EDIT"), _T("RECIPE_NAME_EDIT"),
        _T("REF_RECIPE_ID_EDIT"),_T("MATERIAL_WIDTH_EDIT"), _T("SHEET_LENGTH_EDIT"),
        _T("OFFSET_EDIT"),_T("LIGHTSETTING_COMBOBOX"), _T("INSPECTIONSETTING_COMBOBOX") };
    CFont *m_Font;
    CListBox* m_ID_Recipe_NameList;
    CComboBox* Light_Setting_ComboBox,* Inspection_Setting_ComboBox;
    CEdit** Edits;
	CStatic** Statics;
    std::vector<CString>UI_string;
    CButton** Buttons;
    std::unordered_map<int,std::vector<CString>>lists;
    std::vector<char> imgBuffer;
    CClientSocket  *Client_Socket;
    Shared_Memory* shared_memory;
    CStatic* share_memory_static;
};