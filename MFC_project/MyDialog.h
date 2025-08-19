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
//這個class主要是用於處理圖片資訊的視窗，並將結果回傳至原本的視窗中
//UI的數量和ID
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
//用於通知MyDialog共享記憶可以讀取
#define WM_SOCKET_NOTIFY (WM_USER + 1)
//用於避免另一邊的程式在這邊尚未處理完，並再次讀取共享記憶體
static CCriticalSection cs;
class CMyDialog : public CDialogEx
{
private:
	//創建對話框模板
    DLGTEMPLATE* CreateDialogTemplate();
	//讀取ini檔案方法，下面兩個方法主要差在讀取不同ini檔案的格式，分別為UTF16和UTF8
    void Read_ini_file_method1();//UTF16格式
    void Read_ini_file_method2();//UTF8格式
	//寫入ini檔案方法，下面兩個方法主要差在寫入不同ini檔案的格式，分別為UTF16和UTF8
    void Write_ini_file_method1();//UTF16格式
    void Write_ini_file_method2();//UTF8格式
	//從ini檔案，讀取UI的語言
    void Read_UI_language_ini_file();
    //以下四個都為畫UI的function
    void DrawListBoxs(int, LPDRAWITEMSTRUCT);
    void DrawStatics(int, LPDRAWITEMSTRUCT);
    void DrawButtons(int, LPDRAWITEMSTRUCT);
    void DrawEdits(int, LPDRAWITEMSTRUCT);
	//清除listbox，並重新將資料填入
    void fill_empty_listbox();
public:
    CMyDialog();//constrctor
    CMyDialog(UINT,CWnd* pParent = nullptr);//constrctor
    virtual ~CMyDialog();//deconstructor
protected:
    virtual BOOL OnInitDialog() override;
	void OnLbnSelChangeList();//處理listbox的選擇改變事件
    afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);//處理listbox背景顏色與邊框
    afx_msg void ButtonsClicked_Event(UINT);
	//在處理完後將處理完的資訊傳為回至原本的視窗，這邊直接將回傳按鈕寫在Input按鈕中
    afx_msg void Input_Button_Event();
    afx_msg void Cancel_Button_Event();
    //用於通知MyDialog共享記憶可以讀取的事件
    afx_msg LRESULT OnSocketNotify(WPARAM wParam, LPARAM lParam);
    //練習WM_COPYDATA
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