// OpenATL + WTL 10.0 comprehensive compilation test
// Tests: SDI frame, dialogs, property sheets, all WTL controls,
//        GDI wrappers, message crackers, common dialogs, utility classes
//
// Build:
//   x86_64-w64-mingw32-clang++ -std=c++17 -DUNICODE -D_UNICODE
//     -I3rdparty/openatl/include -I3rdparty/wtl/include -c main.cpp
//     -lcomctl32 -lole32 -loleaut32 -luuid -luxtheme -lgdi32 -lcomdlg32
//     -lshell32 -lshlwapi

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

// ATL headers
#include <atlbase.h>
#include <atlwin.h>
#include <atlcom.h>
#include <atlstr.h>

// WTL headers
#include <atlapp.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atlcrack.h>
#include <atldlgs.h>
#include <atlgdi.h>
#include <atlmisc.h>
#include <atlscrl.h>
#include <atlsplit.h>
#include <atluser.h>
#include <atlwinx.h>

// Verify
static_assert(_ATL_VER >= 0x0800, "ATL version too old for WTL 10");
static_assert(WINVER >= 0x0601, "Minimum Windows 7 required");
static_assert(_WIN32_WINNT >= 0x0601, "Minimum Windows 7 required");

// Resource IDs
#include "resource.h"

///////////////////////////////////////////////////////////////////////////////
// WTL Application Module

WTL::CAppModule _Module;

///////////////////////////////////////////////////////////////////////////////
// Forward declarations

class CMainFrame;
class CView;
class CAboutDlg;
class CAllControlsDlg;

///////////////////////////////////////////////////////////////////////////////
// GDI wrapper compile test

[[maybe_unused]]
static void TestGdiWrappers()
{
    // DC handles
    WTL::CDCHandle dcHandle;
    (void)dcHandle;

    // Pen
    WTL::CPen pen;
    pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    WTL::CPenHandle penH(pen);
    (void)penH;

    // Brush
    WTL::CBrush brush;
    brush.CreateSolidBrush(RGB(255, 255, 255));
    brush.DeleteObject();
    brush.CreateHatchBrush(HS_CROSS, RGB(128, 128, 128));
    WTL::CBrushHandle brushH(brush);
    (void)brushH;

    // Font
    WTL::CFont font;
    font.CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Segoe UI"));
    WTL::CLogFont lf;
    font.GetLogFont(&lf);
    WTL::CFontHandle fontH(font);
    (void)fontH;

    // Bitmap
    WTL::CBitmap bmp;
    bmp.CreateBitmap(32, 32, 1, 32, NULL);
    WTL::CBitmapHandle bmpH(bmp);
    (void)bmpH;

    // Palette
    WTL::CPalette palette;
    WTL::CPaletteHandle palH;
    (void)palette;
    (void)palH;

    // Region
    WTL::CRgn rgn;
    rgn.CreateRectRgn(0, 0, 100, 100);
    WTL::CRgnHandle rgnH(rgn);
    (void)rgnH;

    // Image list
    WTL::CImageList il;
    il.Create(16, 16, ILC_COLOR32 | ILC_MASK, 4, 4);
    il.GetImageCount();
}

///////////////////////////////////////////////////////////////////////////////
// Utility class compile test
// Note: CPoint/CSize/CRect are in global namespace (from atltypes.h),
//       CWindow is in ATL namespace

[[maybe_unused]]
static void TestUtilityClasses()
{
    // CPoint (global namespace)
    CPoint pt1;
    CPoint pt2(10, 20);
    pt1.Offset(5, 5);
    BOOL b1 = (pt1 == pt2);
    (void)b1;

    // CSize (global namespace)
    CSize sz1;
    CSize sz2(100, 200);
    CSize sz3 = sz1 + sz2;
    (void)sz3;

    // CRect (global namespace)
    CRect rc1;
    CRect rc2(0, 0, 640, 480);
    CRect rc3(rc2);
    rc1.SetRectEmpty();
    rc1.InflateRect(10, 10);
    rc1.DeflateRect(5, 5);
    rc1.OffsetRect(10, 10);
    rc1.NormalizeRect();
    BOOL bEmpty = rc1.IsRectEmpty();
    BOOL bNull = rc1.IsRectNull();
    CPoint ptCenter = rc2.CenterPoint();
    int w = rc2.Width();
    int h = rc2.Height();
    (void)bEmpty; (void)bNull; (void)ptCenter; (void)w; (void)h;
    (void)rc3;

    // ATL::CString
    ATL::CString s1;
    ATL::CString s2(_T("Hello"));
    ATL::CString s3 = s2 + _T(" World");
    int len = s3.GetLength();
    int pos = s3.Find(_T('W'));
    ATL::CString upper = s3;
    upper.MakeUpper();
    ATL::CString mid = s3.Mid(6);
    ATL::CString left = s3.Left(5);
    ATL::CString right = s3.Right(5);
    s1.Format(_T("value=%d"), 42);
    s1.Replace(_T("value"), _T("result"));
    s1.Trim();
    LPTSTR buf = s1.GetBuffer(256);
    s1.ReleaseBuffer();
    (void)len; (void)pos; (void)mid; (void)left; (void)right; (void)buf;

    // CMenu
    WTL::CMenu menu;
    menu.CreatePopupMenu();
    menu.AppendMenu(MF_STRING, 1, _T("Item 1"));
    menu.AppendMenu(MF_SEPARATOR);
    menu.AppendMenu(MF_STRING, 2, _T("Item 2"));
    WTL::CMenuHandle menuH = menu.m_hMenu;
    (void)menuH;

    // CAccelerator
    WTL::CAccelerator accel;
    (void)accel;
}

///////////////////////////////////////////////////////////////////////////////
// Control wrapper compile test - exercises template instantiation

[[maybe_unused]]
static void TestAllControlWrappers()
{
    // --- Standard Windows Controls ---

    WTL::CStatic stat;
    stat.SetIcon((HICON)NULL);
    stat.GetIcon();

    WTL::CButton btn;
    btn.GetCheck();
    btn.SetCheck(BST_CHECKED);
    btn.GetButtonStyle();
    btn.SetButtonStyle(BS_PUSHBUTTON);
    btn.GetState();
    btn.SetState(TRUE);
    btn.GetBitmap();
    btn.SetBitmap((HBITMAP)NULL);

    WTL::CEdit edit;
    edit.GetLineCount();
    edit.SetSel(0, -1);
    edit.SetReadOnly(TRUE);
    edit.GetModify();
    edit.SetModify(FALSE);
    edit.GetLimitText();
    edit.SetLimitText(1000);

    WTL::CListBox lb;
    lb.GetCount();
    lb.AddString(_T("item"));
    lb.InsertString(0, _T("first"));
    lb.DeleteString(0);
    lb.GetCurSel();
    lb.SetCurSel(0);
    lb.ResetContent();
    lb.GetItemData(0);
    lb.SetItemData(0, 0);

    WTL::CComboBox cb;
    cb.GetCount();
    cb.AddString(_T("item"));
    cb.InsertString(0, _T("first"));
    cb.DeleteString(0);
    cb.GetCurSel();
    cb.SetCurSel(0);
    cb.ResetContent();
    cb.ShowDropDown(TRUE);
    cb.GetDroppedState();

    WTL::CScrollBar sb;
    sb.GetScrollPos();
    sb.SetScrollPos(0);
    sb.SetScrollRange(0, 100);
    sb.ShowScrollBar(TRUE);
    sb.EnableScrollBar(ESB_ENABLE_BOTH);

    // --- Common Controls ---

    WTL::CListViewCtrl lv;
    lv.GetItemCount();
    lv.GetSelectedCount();
    lv.DeleteAllItems();
    lv.InsertColumn(0, _T("Column"), LVCFMT_LEFT, 100);
    lv.DeleteColumn(0);
    lv.SetItemText(0, 0, _T("text"));
    lv.GetExtendedListViewStyle();
    lv.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);
    lv.GetCheckState(0);
    lv.SetCheckState(0, TRUE);
    lv.GetColumnWidth(0);
    lv.SetColumnWidth(0, 100);
    lv.EnsureVisible(0, FALSE);
    lv.GetNextItem(-1, LVNI_SELECTED);
    lv.SetImageList((HIMAGELIST)NULL, LVSIL_NORMAL);

    WTL::CTreeViewCtrl tv;
    tv.GetCount();
    tv.InsertItem(_T("Root"), TVI_ROOT, TVI_LAST);
    tv.DeleteAllItems();
    tv.GetSelectedItem();
    tv.SelectItem(TVI_ROOT);
    tv.Expand(TVI_ROOT, TVE_EXPAND);
    tv.SetImageList((HIMAGELIST)NULL, TVSIL_NORMAL);
    tv.GetChildItem(TVI_ROOT);
    tv.GetNextSiblingItem(TVI_ROOT);
    tv.GetParentItem(TVI_ROOT);
    tv.GetRootItem();
    tv.SortChildren(TVI_ROOT);

    WTL::CHeaderCtrl hdr;
    hdr.GetItemCount();

    WTL::CToolBarCtrl tb;
    tb.EnableButton(0, TRUE);
    tb.CheckButton(0, TRUE);
    tb.IsButtonEnabled(0);
    tb.IsButtonChecked(0);
    tb.GetButtonCount();
    tb.SetButtonSize(24, 24);
    tb.SetBitmapSize(16, 16);
    tb.AutoSize();

    WTL::CStatusBarCtrl stb;
    stb.SetText(0, _T("Ready"));
    stb.SetSimple(TRUE);
    stb.IsSimple();

    WTL::CTabCtrl tab;
    tab.GetItemCount();
    tab.GetCurSel();
    tab.SetCurSel(0);
    tab.DeleteAllItems();
    tab.SetImageList((HIMAGELIST)NULL);

    WTL::CToolTipCtrl tt;
    tt.GetToolCount();
    tt.Activate(TRUE);
    tt.SetDelayTime(TTDT_INITIAL, 500);
    tt.SetMaxTipWidth(300);

    WTL::CTrackBarCtrl track;
    track.GetPos();
    track.SetPos(50);
    track.GetRangeMin();
    track.GetRangeMax();
    track.SetRange(0, 100, TRUE);
    track.SetTicFreq(10);
    track.SetPageSize(10);
    track.SetLineSize(1);

    WTL::CUpDownCtrl ud;
    ud.GetPos();
    ud.SetPos(50);
    ud.SetRange(0, 100);
    ud.SetBuddy((HWND)NULL);
    ud.GetBuddy();
    ud.SetBase(10);

    WTL::CProgressBarCtrl prog;
    prog.SetPos(50);
    prog.GetPos();
    prog.SetRange(0, 100);
    prog.SetStep(1);
    prog.StepIt();
    prog.SetMarquee(TRUE, 30);
    prog.SetBarColor(RGB(0, 128, 0));

    WTL::CHotKeyCtrl hk;
    hk.GetHotKey();
    hk.SetHotKey(0, 0);
    hk.SetRules(0, 0);

    WTL::CAnimateCtrl anim;
    anim.Play(0, -1, -1);
    anim.Stop();
    anim.Close();
    anim.Seek(0);

    WTL::CRichEditCtrl re;
    re.GetLineCount();
    re.SetSel(0, -1);
    re.SetReadOnly(TRUE);
    re.GetTextLength();
    re.SetEventMask(ENM_CHANGE);
    re.GetEventMask();
    re.SetBackgroundColor(RGB(255, 255, 255));
    re.Undo();
    re.Redo();
    re.CanUndo();
    re.CanRedo();

    WTL::CReBarCtrl rebar;
    rebar.GetBandCount();
    rebar.DeleteBand(0);
    rebar.GetBarHeight();

    WTL::CComboBoxEx cbex;
    cbex.GetImageList();
    cbex.SetImageList((HIMAGELIST)NULL);
    cbex.DeleteItem(0);
    cbex.GetComboCtrl();
    cbex.GetEditCtrl();

    WTL::CDateTimePickerCtrl dtp;
    SYSTEMTIME st = {};
    dtp.GetSystemTime(&st);
    dtp.SetSystemTime(GDT_VALID, &st);
    dtp.SetFormat(_T("yyyy-MM-dd"));

    WTL::CMonthCalendarCtrl mc;
    mc.GetCurSel(&st);
    mc.SetCurSel(&st);
    mc.SetToday(&st);

    WTL::CIPAddressCtrl ip;
    ip.IsBlank();
    ip.ClearAddress();
    ip.SetAddress(MAKEIPADDRESS(192, 168, 1, 1));

    WTL::CPagerCtrl pager;
    pager.SetChild((HWND)NULL);
    pager.GetButtonSize();
    pager.SetButtonSize(20);
    pager.RecalcSize();

    WTL::CLinkCtrl link;
    (void)link;

    // --- Extended Controls (atlctrlx.h) ---

    WTL::CCheckListViewCtrl clv;
    clv.SetCheckState(0, TRUE);
    clv.GetCheckState(0);

    WTL::CMultiPaneStatusBarCtrl mpsb;
    (void)mpsb;

    {
        WTL::CWaitCursor wait;
        wait.Restore();
    }
}

///////////////////////////////////////////////////////////////////////////////
// Common dialog compile test

[[maybe_unused]]
static void TestCommonDialogs(HWND hWndParent)
{
    // File Open
    WTL::CFileDialog dlgOpen(TRUE, _T("txt"), NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"),
        hWndParent);

    // File Save
    WTL::CFileDialog dlgSave(FALSE, _T("txt"), _T("Untitled"),
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"),
        hWndParent);

    // Folder
    WTL::CFolderDialog dlgFolder(hWndParent, _T("Select folder"));

    // Font
    LOGFONT lf = {};
    WTL::CFontDialog dlgFont(&lf, CF_EFFECTS | CF_SCREENFONTS, NULL, hWndParent);

    // Color
    WTL::CColorDialog dlgColor(RGB(0, 0, 0), CC_FULLOPEN, hWndParent);

    // Page Setup
    WTL::CPageSetupDialog dlgPage(PSD_MARGINS, hWndParent);

    (void)dlgOpen; (void)dlgSave; (void)dlgFolder;
    (void)dlgFont; (void)dlgColor; (void)dlgPage;
}

///////////////////////////////////////////////////////////////////////////////
// CView - Scrollable view with message crackers from atlcrack.h

class CView : public WTL::CScrollWindowImpl<CView>
{
public:
    typedef WTL::CScrollWindowImpl<CView> _baseClass;

    DECLARE_WND_CLASS(NULL)

    WTL::CFont   m_fontText;
    WTL::CPen    m_penGrid;
    WTL::CBrush  m_brushBg;
    ATL::CString m_strText;

    CView() : m_strText(_T("OpenATL + WTL 10.0 SDI View")) {}

    // Use atlcrack.h message crackers to test their compilation
    BEGIN_MSG_MAP(CView)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
        MSG_WM_SIZE(OnSize)
        MSG_WM_SETFOCUS(OnSetFocus)
        MSG_WM_KILLFOCUS(OnKillFocus)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_LBUTTONUP(OnLButtonUp)
        MSG_WM_RBUTTONDOWN(OnRButtonDown)
        MSG_WM_MOUSEMOVE(OnMouseMove)
        MSG_WM_KEYDOWN(OnKeyDown)
        MSG_WM_CONTEXTMENU(OnContextMenu)
        CHAIN_MSG_MAP(_baseClass)
    END_MSG_MAP()

    int OnCreate(LPCREATESTRUCT /*lpcs*/)
    {
        m_fontText.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Consolas"));
        m_penGrid.CreatePen(PS_DOT, 1, RGB(200, 200, 200));
        m_brushBg.CreateSolidBrush(RGB(255, 255, 255));

        SetScrollSize(1000, 800, TRUE, false);
        SetScrollLine(10, 10);
        SetScrollPage(100, 100);
        return 0;
    }

    void OnDestroy()
    {
        SetMsgHandled(FALSE);
    }

    BOOL OnEraseBkgnd(WTL::CDCHandle /*dc*/)
    {
        return TRUE;
    }

    void OnSize(UINT /*nType*/, CSize /*size*/)
    {
        SetMsgHandled(FALSE);
    }

    void OnSetFocus(ATL::CWindow /*wndOld*/)
    {
        SetMsgHandled(FALSE);
    }

    void OnKillFocus(ATL::CWindow /*wndNew*/)
    {
        SetMsgHandled(FALSE);
    }

    void OnLButtonDown(UINT /*nFlags*/, CPoint /*pt*/)
    {
        SetFocus();
    }

    void OnLButtonUp(UINT /*nFlags*/, CPoint /*pt*/) {}
    void OnRButtonDown(UINT /*nFlags*/, CPoint /*pt*/) {}
    void OnMouseMove(UINT /*nFlags*/, CPoint /*pt*/) {}

    void OnKeyDown(UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/)
    {
        SetMsgHandled(FALSE);
    }

    void OnContextMenu(ATL::CWindow /*wnd*/, CPoint pt)
    {
        WTL::CMenu menu;
        menu.CreatePopupMenu();
        menu.AppendMenu(MF_STRING, 1, _T("Copy"));
        menu.AppendMenu(MF_STRING, 2, _T("Select All"));
        menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
            pt.x, pt.y, m_hWnd);
    }

    // Called by CScrollWindowImpl
    void DoPaint(WTL::CDCHandle dc)
    {
        CRect rcClient;
        GetClientRect(&rcClient);

        // Background
        dc.FillRect(&rcClient, m_brushBg);

        // Grid
        HPEN hOldPen = dc.SelectPen(m_penGrid);
        for (int x = 0; x < 1000; x += 50) {
            dc.MoveTo(x, 0);
            dc.LineTo(x, 800);
        }
        for (int y = 0; y < 800; y += 50) {
            dc.MoveTo(0, y);
            dc.LineTo(1000, y);
        }
        dc.SelectPen(hOldPen);

        // Text
        HFONT hOldFont = dc.SelectFont(m_fontText);
        dc.SetTextColor(RGB(0, 0, 128));
        dc.SetBkMode(TRANSPARENT);
        dc.TextOut(20, 20, m_strText, m_strText.GetLength());

        // Shapes
        dc.Rectangle(50, 60, 200, 120);
        dc.Ellipse(220, 60, 370, 120);
        dc.RoundRect(390, 60, 540, 120, 15, 15);

        dc.SelectFont(hOldFont);
    }
};

///////////////////////////////////////////////////////////////////////////////
// CAboutDlg - Simple about dialog (CDialogImpl test)

class CAboutDlg : public ATL::CDialogImpl<CAboutDlg>
{
public:
    enum { IDD = IDD_ABOUT };

    BEGIN_MSG_MAP(CAboutDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
        COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
    {
        CenterWindow(GetParent());
        return TRUE;
    }

    LRESULT OnCloseCmd(WORD, WORD wID, HWND, BOOL&)
    {
        EndDialog(wID);
        return 0;
    }
};

///////////////////////////////////////////////////////////////////////////////
// CAllControlsDlg - Dialog with ALL WTL control wrapper types
//   Tests CDialogImpl + CDialogResize + all control member declarations

class CAllControlsDlg : public ATL::CDialogImpl<CAllControlsDlg>,
                         public WTL::CDialogResize<CAllControlsDlg>
{
public:
    enum { IDD = IDD_CONTROLS };

    // --- Standard controls ---
    WTL::CStatic              m_static;
    WTL::CButton              m_btnPush;
    WTL::CButton              m_btnCheck;
    WTL::CButton              m_btnRadio;
    WTL::CEdit                m_edit;
    WTL::CListBox             m_listBox;
    WTL::CComboBox            m_comboBox;
    WTL::CScrollBar           m_scrollBar;

    // --- Common controls ---
    WTL::CListViewCtrl        m_listView;
    WTL::CTreeViewCtrl        m_treeView;
    WTL::CHeaderCtrl          m_header;
    WTL::CToolBarCtrl         m_toolbar;
    WTL::CStatusBarCtrl       m_statusBar;
    WTL::CTabCtrl             m_tabCtrl;
    WTL::CToolTipCtrl         m_toolTip;
    WTL::CTrackBarCtrl        m_trackBar;
    WTL::CUpDownCtrl          m_upDown;
    WTL::CProgressBarCtrl     m_progressBar;
    WTL::CHotKeyCtrl          m_hotKey;
    WTL::CAnimateCtrl         m_animate;
    WTL::CRichEditCtrl        m_richEdit;
    WTL::CReBarCtrl           m_rebar;
    WTL::CComboBoxEx          m_comboBoxEx;
    WTL::CDateTimePickerCtrl  m_datePicker;
    WTL::CMonthCalendarCtrl   m_monthCal;
    WTL::CIPAddressCtrl       m_ipAddress;
    WTL::CPagerCtrl           m_pager;
    WTL::CLinkCtrl            m_link;

    // --- Additional controls (Row 4) ---
    WTL::CButton              m_btnRadio2;
    WTL::CButton              m_btnRadio3;
    WTL::CButton              m_btnRadio4;
    WTL::CButton              m_btnCheck2;
    WTL::CButton              m_btnCheck3;
    WTL::CEdit                m_editPasswd;
    WTL::CEdit                m_editMulti;
    WTL::CProgressBarCtrl     m_progressBar2;
    WTL::CScrollBar           m_hScrollBar;
    WTL::CButton              m_btnReset;
    WTL::CStatic              m_staticStatus;

    // --- Extended controls (atlctrlx.h) ---
    WTL::CCheckListViewCtrl       m_checkListView;
    WTL::CMultiPaneStatusBarCtrl  m_multiStatusBar;

    // --- Image lists ---
    WTL::CImageList           m_imageListSmall;
    WTL::CImageList           m_imageListLarge;

    int m_nButtonClicks;
    int m_nScrollPos;

    BEGIN_MSG_MAP(CAllControlsDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
        MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
        COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
        COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
        COMMAND_ID_HANDLER(IDC_BUTTON1, OnPushButton)
        COMMAND_ID_HANDLER(IDC_BUTTON2, OnResetAll)
        COMMAND_ID_HANDLER(IDC_CHECK1, OnCheckBox)
        COMMAND_ID_HANDLER(IDC_CHECK3, OnReadOnlyCheck)
        COMMAND_ID_HANDLER(IDC_RADIO2, OnRadioGroup)
        COMMAND_ID_HANDLER(IDC_RADIO3, OnRadioGroup)
        COMMAND_ID_HANDLER(IDC_RADIO4, OnRadioGroup)
        COMMAND_CODE_HANDLER(EN_CHANGE, OnEnChange)
        COMMAND_CODE_HANDLER(LBN_SELCHANGE, OnLbnSelChange)
        COMMAND_CODE_HANDLER(CBN_SELCHANGE, OnCbnSelChange)
        NOTIFY_CODE_HANDLER(LVN_ITEMCHANGED, OnLvnItemChanged)
        NOTIFY_CODE_HANDLER(TVN_SELCHANGED, OnTvnSelChanged)
        NOTIFY_CODE_HANDLER(TCN_SELCHANGE, OnTcnSelChange)
        NOTIFY_CODE_HANDLER(NM_DBLCLK, OnNmDblClk)
        NOTIFY_HANDLER(IDC_LISTVIEW, LVN_COLUMNCLICK, OnLvnColumnClick)
        CHAIN_MSG_MAP(WTL::CDialogResize<CAllControlsDlg>)
    END_MSG_MAP()

    BEGIN_DLGRESIZE_MAP(CAllControlsDlg)
        DLGRESIZE_CONTROL(IDC_LISTVIEW, DLSZ_SIZE_X | DLSZ_SIZE_Y)
        DLGRESIZE_CONTROL(IDC_TREEVIEW, DLSZ_SIZE_Y)
        DLGRESIZE_CONTROL(IDC_EDIT1, DLSZ_SIZE_X)
        DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X | DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X | DLSZ_MOVE_Y)
    END_DLGRESIZE_MAP()

    LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
    {
        CenterWindow(GetParent());
        m_nButtonClicks = 0;
        m_nScrollPos = 0;
        m_imageListSmall.Create(16, 16, ILC_COLOR32 | ILC_MASK, 8, 8);
        m_imageListLarge.Create(32, 32, ILC_COLOR32 | ILC_MASK, 8, 8);

        // Populate ComboBox
        m_comboBox = GetDlgItem(IDC_COMBO1);
        m_comboBox.AddString(_T("Option Alpha"));
        m_comboBox.AddString(_T("Option Beta"));
        m_comboBox.AddString(_T("Option Gamma"));
        m_comboBox.AddString(_T("Option Delta"));
        m_comboBox.AddString(_T("Option Epsilon"));
        m_comboBox.SetCurSel(0);

        // Populate ListBox
        m_listBox = GetDlgItem(IDC_LISTBOX);
        m_listBox.AddString(_T("Apple"));
        m_listBox.AddString(_T("Banana"));
        m_listBox.AddString(_T("Cherry"));
        m_listBox.AddString(_T("Date"));
        m_listBox.AddString(_T("Fig"));
        m_listBox.AddString(_T("Grape"));
        m_listBox.AddString(_T("Kiwi"));
        m_listBox.AddString(_T("Lemon"));
        m_listBox.SetCurSel(0);

        // Populate ListView
        m_listView = GetDlgItem(IDC_LISTVIEW);
        m_listView.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
        m_listView.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 50);
        m_listView.InsertColumn(1, _T("Type"), LVCFMT_LEFT, 60);
        for (int i = 0; i < 10; i++) {
            ATL::CString s;
            s.Format(_T("Item %d"), i + 1);
            m_listView.InsertItem(i, s);
            s.Format(_T("Type %c"), _T('A') + i);
            m_listView.SetItemText(i, 1, s);
        }

        // Populate TreeView
        m_treeView = GetDlgItem(IDC_TREEVIEW);
        HTREEITEM hRoot = m_treeView.InsertItem(_T("Root"), TVI_ROOT, TVI_LAST);
        HTREEITEM hChild1 = m_treeView.InsertItem(_T("Branch A"), hRoot, TVI_LAST);
        m_treeView.InsertItem(_T("Leaf 1"), hChild1, TVI_LAST);
        m_treeView.InsertItem(_T("Leaf 2"), hChild1, TVI_LAST);
        HTREEITEM hChild2 = m_treeView.InsertItem(_T("Branch B"), hRoot, TVI_LAST);
        m_treeView.InsertItem(_T("Leaf 3"), hChild2, TVI_LAST);
        m_treeView.InsertItem(_T("Leaf 4"), hChild2, TVI_LAST);
        HTREEITEM hChild3 = m_treeView.InsertItem(_T("Branch C"), hRoot, TVI_LAST);
        m_treeView.InsertItem(_T("Leaf 5"), hChild3, TVI_LAST);
        m_treeView.Expand(hRoot, TVE_EXPAND);
        m_treeView.Expand(hChild1, TVE_EXPAND);

        // Set up TrackBar — synced with ProgressBar
        m_trackBar = GetDlgItem(IDC_SLIDER);
        m_trackBar.SetRange(0, 100);
        m_trackBar.SetPos(50);
        m_trackBar.SetTicFreq(10);

        // Set up ProgressBar — initial value matches trackbar
        m_progressBar = GetDlgItem(IDC_PROGRESS);
        m_progressBar.SetRange(0, 100);
        m_progressBar.SetPos(50);

        // Set up UpDown/Spin — synced with its buddy edit
        m_upDown = GetDlgItem(IDC_SPIN);
        m_upDown.SetRange(0, 100);
        m_upDown.SetPos(50);

        // Set up ScrollBar
        m_scrollBar = GetDlgItem(IDC_SCROLLBAR);
        m_scrollBar.SetScrollRange(0, 100);
        m_scrollBar.SetScrollPos(50);

        // Set up IP Address
        m_ipAddress = GetDlgItem(IDC_IPADDRESS);
        m_ipAddress.SetAddress(MAKEIPADDRESS(192, 168, 1, 100));

        // Set up Tab Control
        m_tabCtrl = GetDlgItem(IDC_TAB);
        TCITEM tci = {};
        tci.mask = TCIF_TEXT;
        tci.pszText = const_cast<LPTSTR>(_T("General"));
        m_tabCtrl.InsertItem(0, &tci);
        tci.pszText = const_cast<LPTSTR>(_T("Advanced"));
        m_tabCtrl.InsertItem(1, &tci);
        tci.pszText = const_cast<LPTSTR>(_T("Options"));
        m_tabCtrl.InsertItem(2, &tci);

        // Set up DateTimePicker (defaults to current date)
        m_datePicker = GetDlgItem(IDC_DATEPICKER);

        // Set up ComboBoxEx
        m_comboBoxEx = GetDlgItem(IDC_COMBOEX);
        COMBOBOXEXITEM cbei = {};
        cbei.mask = CBEIF_TEXT;
        cbei.iItem = 0;
        cbei.pszText = const_cast<LPTSTR>(_T("Debug Build"));
        m_comboBoxEx.InsertItem(&cbei);
        cbei.iItem = 1;
        cbei.pszText = const_cast<LPTSTR>(_T("Release Build"));
        m_comboBoxEx.InsertItem(&cbei);
        cbei.iItem = 2;
        cbei.pszText = const_cast<LPTSTR>(_T("Profile Build"));
        m_comboBoxEx.InsertItem(&cbei);
        m_comboBoxEx.SetCurSel(0);

        // Set up Edit
        m_edit = GetDlgItem(IDC_EDIT1);
        m_edit.SetWindowText(_T("Hello, WTL!"));

        // Set up RichEdit with formatted text
        m_richEdit = GetDlgItem(IDC_RICHEDIT);
        m_richEdit.SetWindowText(
            _T("OpenATL + WTL 10.0 Test\r\n\r\n")
            _T("This RichEdit control demonstrates multi-line text editing.\r\n\r\n")
            _T("Features tested:\r\n")
            _T("- Text input & selection\r\n")
            _T("- Scroll bars\r\n")
            _T("- Word wrap\r\n"));

        // Set up CheckBox and Radio
        m_btnCheck = GetDlgItem(IDC_CHECK1);
        m_btnCheck.SetCheck(BST_CHECKED);
        m_btnRadio = GetDlgItem(IDC_RADIO1);
        m_btnRadio.SetCheck(BST_CHECKED);

        // Set up Push Button
        m_btnPush = GetDlgItem(IDC_BUTTON1);

        // Set up HotKey
        m_hotKey = GetDlgItem(IDC_HOTKEY);

        // Set up MonthCalendar
        m_monthCal = GetDlgItem(IDC_MONTHCAL);

        // Set up Link
        m_link = GetDlgItem(IDC_LINK);

        // --- Row 4: Additional Controls ---

        // Radio button group
        m_btnRadio2 = GetDlgItem(IDC_RADIO2);
        m_btnRadio3 = GetDlgItem(IDC_RADIO3);
        m_btnRadio4 = GetDlgItem(IDC_RADIO4);
        m_btnRadio2.SetCheck(BST_CHECKED);  // Default selection

        // 3-state checkbox
        m_btnCheck2 = GetDlgItem(IDC_CHECK2);
        m_btnCheck2.SetCheck(BST_INDETERMINATE);  // Start in indeterminate state

        // Read-only checkbox
        m_btnCheck3 = GetDlgItem(IDC_CHECK3);

        // Password edit
        m_editPasswd = GetDlgItem(IDC_EDIT_PASSWD);
        m_editPasswd.SetWindowText(_T("secret"));

        // Multi-line standard edit
        m_editMulti = GetDlgItem(IDC_EDIT_MULTI);
        m_editMulti.SetWindowText(
            _T("Multi-line standard edit control.\r\n")
            _T("Line 2: supports scrolling.\r\n")
            _T("Line 3: unlike RichEdit, no formatting.\r\n")
            _T("Line 4: plain text only."));

        // Second progress bar (smooth style)
        m_progressBar2 = GetDlgItem(IDC_PROGRESS2);
        m_progressBar2.SetRange(0, 100);
        m_progressBar2.SetPos(30);

        // Horizontal scrollbar
        m_hScrollBar = GetDlgItem(IDC_HSCROLLBAR);
        m_hScrollBar.SetScrollRange(0, 100);
        m_hScrollBar.SetScrollPos(30);

        // Reset button
        m_btnReset = GetDlgItem(IDC_BUTTON2);

        // Status label
        m_staticStatus = GetDlgItem(IDC_STATIC_STATUS);

        DlgResize_Init();
        return TRUE;
    }

    LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
    { bHandled = FALSE; return 0; }

    LRESULT OnCloseCmd(WORD, WORD wID, HWND, BOOL&)
    { EndDialog(wID); return 0; }

    // TrackBar & HScrollBar (horizontal scroll) — sync with ProgressBars
    LRESULT OnHScroll(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if ((HWND)lParam == m_trackBar.m_hWnd) {
            int pos = m_trackBar.GetPos();
            m_progressBar.SetPos(pos);
            // Also update the spin edit to show the same value
            ATL::CString s;
            s.Format(_T("%d"), pos);
            ::SetDlgItemText(m_hWnd, IDC_SPIN_EDIT, s);
            return 0;
        }
        if ((HWND)lParam == m_hScrollBar.m_hWnd) {
            int nPos = m_hScrollBar.GetScrollPos();
            switch (LOWORD(wParam)) {
            case SB_LINELEFT:    nPos -= 1; break;
            case SB_LINERIGHT:   nPos += 1; break;
            case SB_PAGELEFT:    nPos -= 10; break;
            case SB_PAGERIGHT:   nPos += 10; break;
            case SB_THUMBTRACK:  nPos = HIWORD(wParam); break;
            case SB_THUMBPOSITION: nPos = HIWORD(wParam); break;
            }
            if (nPos < 0) nPos = 0;
            if (nPos > 100) nPos = 100;
            m_hScrollBar.SetScrollPos(nPos);
            // Sync with the smooth progress bar
            m_progressBar2.SetPos(nPos);
            // Update status label
            ATL::CString s;
            s.Format(_T("HScroll: %d"), nPos);
            m_staticStatus.SetWindowText(s);
            return 0;
        }
        bHandled = FALSE;
        return 0;
    }

    // ScrollBar (vertical scroll)
    LRESULT OnVScroll(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if ((HWND)lParam == m_scrollBar.m_hWnd) {
            int nPos = m_scrollBar.GetScrollPos();
            switch (LOWORD(wParam)) {
            case SB_LINEUP:      nPos -= 1; break;
            case SB_LINEDOWN:    nPos += 1; break;
            case SB_PAGEUP:      nPos -= 10; break;
            case SB_PAGEDOWN:    nPos += 10; break;
            case SB_THUMBTRACK:  nPos = HIWORD(wParam); break;
            case SB_THUMBPOSITION: nPos = HIWORD(wParam); break;
            }
            if (nPos < 0) nPos = 0;
            if (nPos > 100) nPos = 100;
            m_scrollBar.SetScrollPos(nPos);
            // Update the listbox selection to reflect scroll position
            int idx = (nPos * m_listBox.GetCount()) / 101;
            if (idx >= m_listBox.GetCount()) idx = m_listBox.GetCount() - 1;
            m_listBox.SetCurSel(idx);
            return 0;
        }
        bHandled = FALSE;
        return 0;
    }

    // Push Button — adds items to ListView and shows click count
    LRESULT OnPushButton(WORD, WORD, HWND, BOOL&)
    {
        m_nButtonClicks++;
        ATL::CString s;
        s.Format(_T("New %d"), m_nButtonClicks);
        int idx = m_listView.GetItemCount();
        m_listView.InsertItem(idx, s);
        s.Format(_T("Added"));
        m_listView.SetItemText(idx, 1, s);
        m_listView.EnsureVisible(idx, FALSE);

        // Also add to the listbox
        s.Format(_T("Item #%d"), m_nButtonClicks);
        m_listBox.AddString(s);

        // Update button text to show count
        s.Format(_T("Push (%d)"), m_nButtonClicks);
        m_btnPush.SetWindowText(s);
        return 0;
    }

    // CheckBox — toggles enable/disable of Edit, ComboBox, ListBox
    LRESULT OnCheckBox(WORD, WORD, HWND, BOOL&)
    {
        BOOL bEnable = (m_btnCheck.GetCheck() == BST_CHECKED);
        m_edit.EnableWindow(bEnable);
        m_comboBox.EnableWindow(bEnable);
        m_listBox.EnableWindow(bEnable);
        return 0;
    }

    // Edit change — mirror text to the dialog title
    LRESULT OnEnChange(WORD, WORD wID, HWND, BOOL& bHandled)
    {
        if (wID == IDC_EDIT1) {
            TCHAR buf[256] = {};
            m_edit.GetWindowText(buf, 256);
            ATL::CString title;
            title.Format(_T("All WTL Controls Showcase - \"%s\""), buf);
            SetWindowText(title);
        }
        bHandled = FALSE;
        return 0;
    }

    // ListBox selection — update edit with selected fruit name
    LRESULT OnLbnSelChange(WORD, WORD, HWND, BOOL& bHandled)
    {
        int sel = m_listBox.GetCurSel();
        if (sel != LB_ERR) {
            TCHAR buf[256] = {};
            m_listBox.GetText(sel, buf);
            // Append to richedit
            int len = m_richEdit.GetTextLength();
            m_richEdit.SetSel(len, len);
            ATL::CString s;
            s.Format(_T("\r\nSelected: %s"), buf);
            m_richEdit.ReplaceSel(s);
        }
        bHandled = FALSE;
        return 0;
    }

    // ComboBox selection — update progress bar color
    LRESULT OnCbnSelChange(WORD, WORD wID, HWND, BOOL& bHandled)
    {
        if (wID == IDC_COMBO1) {
            int sel = m_comboBox.GetCurSel();
            // Change progress bar color based on combo selection
            COLORREF colors[] = {
                RGB(0, 128, 0),    // Alpha - green
                RGB(0, 0, 200),    // Beta - blue
                RGB(200, 0, 0),    // Gamma - red
                RGB(200, 128, 0),  // Delta - orange
                RGB(128, 0, 200),  // Epsilon - purple
            };
            if (sel >= 0 && sel < 5)
                m_progressBar.SetBarColor(colors[sel]);
        }
        bHandled = FALSE;
        return 0;
    }

    // ListView item changed — show selected item info in status area
    LRESULT OnLvnItemChanged(int, LPNMHDR pnmh, BOOL& bHandled)
    {
        LPNMLISTVIEW pnmlv = (LPNMLISTVIEW)pnmh;
        if (pnmlv->uChanged & LVIF_STATE) {
            if (pnmlv->uNewState & LVIS_SELECTED) {
                TCHAR buf[256] = {};
                m_listView.GetItemText(pnmlv->iItem, 0, buf, 256);
                ATL::CString title;
                title.Format(_T("All WTL Controls Showcase - ListView: %s"), buf);
                SetWindowText(title);
            }
        }
        bHandled = FALSE;
        return 0;
    }

    // TreeView selection changed — show node path in edit
    LRESULT OnTvnSelChanged(int, LPNMHDR pnmh, BOOL& bHandled)
    {
        LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)pnmh;
        HTREEITEM hItem = pnmtv->itemNew.hItem;
        // Build path from root
        ATL::CString path;
        while (hItem) {
            TCHAR buf[256] = {};
            m_treeView.GetItemText(hItem, buf, 256);
            if (path.IsEmpty())
                path = buf;
            else
                path = ATL::CString(buf) + _T(" > ") + path;
            hItem = m_treeView.GetParentItem(hItem);
        }
        ATL::CString title;
        title.Format(_T("All WTL Controls Showcase - Tree: %s"), (LPCTSTR)path);
        SetWindowText(title);
        bHandled = FALSE;
        return 0;
    }

    // Tab selection changed — update richedit with tab info
    LRESULT OnTcnSelChange(int, LPNMHDR, BOOL& bHandled)
    {
        int sel = m_tabCtrl.GetCurSel();
        TCITEM tci = {};
        TCHAR buf[64] = {};
        tci.mask = TCIF_TEXT;
        tci.pszText = buf;
        tci.cchTextMax = 64;
        m_tabCtrl.GetItem(sel, &tci);
        ATL::CString s;
        s.Format(_T("\r\nTab selected: %s (index %d)"), buf, sel);
        int len = m_richEdit.GetTextLength();
        m_richEdit.SetSel(len, len);
        m_richEdit.ReplaceSel(s);
        bHandled = FALSE;
        return 0;
    }

    // ListView double-click — show message box with item details
    LRESULT OnNmDblClk(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
    {
        if (idCtrl == IDC_LISTVIEW) {
            LPNMITEMACTIVATE pnmia = (LPNMITEMACTIVATE)pnmh;
            if (pnmia->iItem >= 0) {
                TCHAR name[256] = {}, type[256] = {};
                m_listView.GetItemText(pnmia->iItem, 0, name, 256);
                m_listView.GetItemText(pnmia->iItem, 1, type, 256);
                ATL::CString msg;
                msg.Format(_T("Double-clicked:\n  Name: %s\n  Type: %s\n  Row: %d"),
                    name, type, pnmia->iItem);
                MessageBox(msg, _T("ListView Item"), MB_OK | MB_ICONINFORMATION);
            }
            return 0;
        }
        bHandled = FALSE;
        return 0;
    }

    // ListView column click — sort indication
    LRESULT OnLvnColumnClick(int, LPNMHDR pnmh, BOOL&)
    {
        LPNMLISTVIEW pnmlv = (LPNMLISTVIEW)pnmh;
        ATL::CString s;
        s.Format(_T("\r\nColumn %d header clicked"), pnmlv->iSubItem);
        int len = m_richEdit.GetTextLength();
        m_richEdit.SetSel(len, len);
        m_richEdit.ReplaceSel(s);
        return 0;
    }

    // Radio group selection — update status label and multi-line edit
    LRESULT OnRadioGroup(WORD, WORD wID, HWND, BOOL&)
    {
        LPCTSTR labels[] = { _T("Option A"), _T("Option B"), _T("Option C") };
        int idx = wID - IDC_RADIO2;
        if (idx >= 0 && idx < 3) {
            ATL::CString s;
            s.Format(_T("Radio: %s"), labels[idx]);
            m_staticStatus.SetWindowText(s);
            // Append to multi-line edit
            int len = m_editMulti.GetWindowTextLength();
            m_editMulti.SetSel(len, len);
            s.Format(_T("\r\nRadio changed to %s"), labels[idx]);
            m_editMulti.ReplaceSel(s);
        }
        return 0;
    }

    // Read-Only checkbox — toggles read-only state of edits
    LRESULT OnReadOnlyCheck(WORD, WORD, HWND, BOOL&)
    {
        BOOL bReadOnly = (m_btnCheck3.GetCheck() == BST_CHECKED);
        m_edit.SetReadOnly(bReadOnly);
        m_editMulti.SetReadOnly(bReadOnly);
        m_richEdit.SetReadOnly(bReadOnly);
        m_staticStatus.SetWindowText(bReadOnly ? _T("Read-Only ON") : _T("Read-Only OFF"));
        return 0;
    }

    // Reset All — restore all controls to initial state
    LRESULT OnResetAll(WORD, WORD, HWND, BOOL&)
    {
        // Reset edit fields
        m_edit.SetWindowText(_T("Hello, WTL!"));
        m_editPasswd.SetWindowText(_T("secret"));
        m_editMulti.SetWindowText(
            _T("Multi-line standard edit control.\r\n")
            _T("Line 2: supports scrolling.\r\n")
            _T("Line 3: unlike RichEdit, no formatting.\r\n")
            _T("Line 4: plain text only."));
        m_richEdit.SetWindowText(
            _T("OpenATL + WTL 10.0 Test\r\n\r\n")
            _T("Controls have been reset.\r\n"));

        // Reset combo
        m_comboBox.SetCurSel(0);
        m_comboBoxEx.SetCurSel(0);

        // Reset listbox
        m_listBox.SetCurSel(0);

        // Reset trackbar, progress, scrollbar
        m_trackBar.SetPos(50);
        m_progressBar.SetPos(50);
        m_progressBar.SetBarColor((COLORREF)-1);  // default color
        m_progressBar2.SetPos(30);
        m_scrollBar.SetScrollPos(50);
        m_hScrollBar.SetScrollPos(30);
        m_upDown.SetPos(50);

        // Reset checkboxes
        m_btnCheck.SetCheck(BST_CHECKED);
        m_btnCheck2.SetCheck(BST_INDETERMINATE);
        m_btnCheck3.SetCheck(BST_UNCHECKED);
        m_edit.SetReadOnly(FALSE);
        m_editMulti.SetReadOnly(FALSE);
        m_richEdit.SetReadOnly(FALSE);
        m_edit.EnableWindow(TRUE);
        m_comboBox.EnableWindow(TRUE);
        m_listBox.EnableWindow(TRUE);

        // Reset radios
        m_btnRadio.SetCheck(BST_CHECKED);
        m_btnRadio2.SetCheck(BST_CHECKED);
        m_btnRadio3.SetCheck(BST_UNCHECKED);
        m_btnRadio4.SetCheck(BST_UNCHECKED);

        // Reset IP
        m_ipAddress.SetAddress(MAKEIPADDRESS(192, 168, 1, 100));

        // Reset button click counter
        m_nButtonClicks = 0;
        m_btnPush.SetWindowText(_T("Push Button"));

        // Reset title
        SetWindowText(_T("All WTL Controls Showcase"));
        m_staticStatus.SetWindowText(_T("Reset done"));
        return 0;
    }
};

///////////////////////////////////////////////////////////////////////////////
// Property Pages (CPropertyPageImpl test)

class CGeneralPage : public WTL::CPropertyPageImpl<CGeneralPage>
{
public:
    enum { IDD = IDD_PROPPAGE1 };

    WTL::CEdit     m_editName;
    WTL::CComboBox m_comboType;
    WTL::CButton   m_btnEnabled;

    BEGIN_MSG_MAP(CGeneralPage)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_CODE_HANDLER(EN_CHANGE, OnFieldChanged)
        COMMAND_CODE_HANDLER(CBN_SELCHANGE, OnFieldChanged)
        COMMAND_CODE_HANDLER(BN_CLICKED, OnFieldChanged)
        CHAIN_MSG_MAP(WTL::CPropertyPageImpl<CGeneralPage>)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
    {
        m_editName = GetDlgItem(IDC_PP_EDIT_NAME);
        m_editName.SetWindowText(_T("My Project"));

        m_comboType = GetDlgItem(IDC_PP_COMBO_TYPE);
        m_comboType.AddString(_T("Application"));
        m_comboType.AddString(_T("Library (DLL)"));
        m_comboType.AddString(_T("Static Library"));
        m_comboType.AddString(_T("Console App"));
        m_comboType.AddString(_T("Driver"));
        m_comboType.SetCurSel(0);

        m_btnEnabled = GetDlgItem(IDC_PP_CHECK_EN);
        m_btnEnabled.SetCheck(BST_CHECKED);
        return TRUE;
    }

    LRESULT OnFieldChanged(WORD, WORD, HWND, BOOL& bHandled)
    {
        SetModified(TRUE);
        bHandled = FALSE;
        return 0;
    }

    int OnApply() { return PSNRET_NOERROR; }
};

class CAdvancedPage : public WTL::CPropertyPageImpl<CAdvancedPage>
{
public:
    enum { IDD = IDD_PROPPAGE2 };

    WTL::CListViewCtrl    m_listSettings;
    WTL::CTrackBarCtrl    m_sliderValue;
    WTL::CProgressBarCtrl m_progress;

    BEGIN_MSG_MAP(CAdvancedPage)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
        NOTIFY_CODE_HANDLER(LVN_ITEMCHANGED, OnListChanged)
        CHAIN_MSG_MAP(WTL::CPropertyPageImpl<CAdvancedPage>)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
    {
        m_listSettings = GetDlgItem(IDC_PP_LIST);
        m_listSettings.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
        m_listSettings.InsertColumn(0, _T("Setting"), LVCFMT_LEFT, 100);
        m_listSettings.InsertColumn(1, _T("Value"), LVCFMT_LEFT, 120);
        struct { LPCTSTR name; LPCTSTR val; } items[] = {
            { _T("Optimization"),   _T("O2") },
            { _T("Debug Info"),     _T("Full") },
            { _T("Warnings"),       _T("All (-Wall)") },
            { _T("C++ Standard"),   _T("C++17") },
            { _T("Link Type"),      _T("Static") },
            { _T("Architecture"),   _T("x86_64") },
            { _T("Charset"),        _T("Unicode") },
            { _T("Runtime"),        _T("UCRT") },
        };
        for (int i = 0; i < 8; i++) {
            m_listSettings.InsertItem(i, items[i].name);
            m_listSettings.SetItemText(i, 1, items[i].val);
        }

        m_sliderValue = GetDlgItem(IDC_PP_SLIDER);
        m_sliderValue.SetRange(0, 100);
        m_sliderValue.SetPos(65);
        m_sliderValue.SetTicFreq(10);

        m_progress = GetDlgItem(IDC_PP_PROGRESS);
        m_progress.SetRange(0, 100);
        m_progress.SetPos(65);
        return TRUE;
    }

    LRESULT OnHScroll(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
    {
        if ((HWND)lParam == m_sliderValue.m_hWnd) {
            int pos = m_sliderValue.GetPos();
            m_progress.SetPos(pos);
            SetModified(TRUE);
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnListChanged(int, LPNMHDR, BOOL& bHandled)
    {
        SetModified(TRUE);
        bHandled = FALSE;
        return 0;
    }

    int OnApply() { return PSNRET_NOERROR; }
};

///////////////////////////////////////////////////////////////////////////////
// CMainFrame - SDI Frame Window
//   Tests: CFrameWindowImpl, CUpdateUI, CMessageFilter, CIdleHandler,
//          CSplitterWindow, CPaneContainer, CMultiPaneStatusBarCtrl

class CMainFrame : public WTL::CFrameWindowImpl<CMainFrame>,
                   public WTL::CUpdateUI<CMainFrame>,
                   public WTL::CMessageFilter,
                   public WTL::CIdleHandler
{
public:
    DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

    // Splitter layout: tree pane | view
    WTL::CSplitterWindow  m_splitter;
    WTL::CPaneContainer   m_paneTree;
    WTL::CTreeViewCtrl    m_treeNav;
    CView                 m_view;

    // Multi-pane status bar
    WTL::CMultiPaneStatusBarCtrl m_statusBar;

    // GDI objects
    WTL::CFont            m_fontUI;
    WTL::CImageList       m_imageListTree;

    // State
    ATL::CString          m_strTitle;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
    BEGIN_MSG_MAP(CMainFrame)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
        COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
        COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
        COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
        COMMAND_ID_HANDLER(ID_FILE_SAVE, OnFileSave)
        COMMAND_ID_HANDLER(ID_TOOLS_OPTIONS, OnToolsOptions)
        COMMAND_ID_HANDLER(ID_VIEW_CONTROLS, OnViewControls)
        COMMAND_RANGE_HANDLER(ID_EDIT_CUT, ID_EDIT_PASTE, OnEditCmd)
        CHAIN_MSG_MAP(WTL::CUpdateUI<CMainFrame>)
        CHAIN_MSG_MAP(WTL::CFrameWindowImpl<CMainFrame>)
    END_MSG_MAP()
#pragma clang diagnostic pop

    BEGIN_UPDATE_UI_MAP(CMainFrame)
        UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
        UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
        UPDATE_ELEMENT(ID_FILE_SAVE, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
        UPDATE_ELEMENT(ID_EDIT_CUT, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
        UPDATE_ELEMENT(ID_EDIT_COPY, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
        UPDATE_ELEMENT(ID_EDIT_PASTE, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
    END_UPDATE_UI_MAP()

    virtual BOOL PreTranslateMessage(MSG* pMsg) override
    {
        return WTL::CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
    }

    virtual BOOL OnIdle() override
    {
        UIUpdateToolBar();
        return FALSE;
    }

    LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&)
    {

        // Create multi-pane status bar
        m_hWndStatusBar = m_statusBar.Create(m_hWnd);
        int arrPanes[] = { ID_DEFAULT_PANE, IDC_STATUSBAR, IDC_STATUSBAR + 1 };
        m_statusBar.SetPanes(arrPanes, 3, false);
        m_statusBar.SetPaneText(ID_DEFAULT_PANE, _T("Ready"));

        // Create splitter as client
        m_hWndClient = m_splitter.Create(m_hWnd, rcDefault, NULL,
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

        // Left pane: tree in a pane container
        m_paneTree.Create(m_splitter, _T("Navigation"),
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
        m_treeNav.Create(m_paneTree, rcDefault, NULL,
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
            TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
            WS_EX_CLIENTEDGE);
        m_paneTree.SetClient(m_treeNav);

        // Populate tree
        HTREEITEM hRoot = m_treeNav.InsertItem(_T("Project"), TVI_ROOT, TVI_LAST);
        m_treeNav.InsertItem(_T("Source Files"), hRoot, TVI_LAST);
        m_treeNav.InsertItem(_T("Header Files"), hRoot, TVI_LAST);
        m_treeNav.InsertItem(_T("Resources"), hRoot, TVI_LAST);
        m_treeNav.Expand(hRoot, TVE_EXPAND);

        // Right pane: scrollable view
        m_view.Create(m_splitter, rcDefault, NULL,
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
            WS_HSCROLL | WS_VSCROLL, WS_EX_CLIENTEDGE);

        // Set splitter panes
        m_splitter.SetSplitterPanes(m_paneTree, m_view);
        m_splitter.SetSplitterPos(200);

        // Message loop hooks
        WTL::CMessageLoop* pLoop = _Module.GetMessageLoop();
        ATLASSERT(pLoop != NULL);
        pLoop->AddMessageFilter(this);
        pLoop->AddIdleHandler(this);

        // UI state
        UISetCheck(ID_VIEW_TOOLBAR, 1);
        UISetCheck(ID_VIEW_STATUS_BAR, 1);
        UIEnable(ID_FILE_SAVE, FALSE);

        m_strTitle = _T("OpenATL + WTL Test Application");
        return 0;
    }

    LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
    {
        WTL::CMessageLoop* pLoop = _Module.GetMessageLoop();
        ATLASSERT(pLoop != NULL);
        pLoop->RemoveMessageFilter(this);
        pLoop->RemoveIdleHandler(this);
        bHandled = FALSE;
        return 1;
    }

    LRESULT OnFileNew(WORD, WORD, HWND, BOOL&)
    {
        ATL::CString str;
        str.Format(_T("New document"));
        m_statusBar.SetPaneText(ID_DEFAULT_PANE, str);
        return 0;
    }

    LRESULT OnFileOpen(WORD, WORD, HWND, BOOL&)
    {
        WTL::CFileDialog dlg(TRUE, _T("txt"), NULL,
            OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
            _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"));
        if (dlg.DoModal() == IDOK) {
            ATL::CString strFile = dlg.m_szFileName;
            m_statusBar.SetPaneText(ID_DEFAULT_PANE, strFile);
            UIEnable(ID_FILE_SAVE, TRUE);
        }
        return 0;
    }

    LRESULT OnFileSave(WORD, WORD, HWND, BOOL&)
    {
        WTL::CFileDialog dlg(FALSE, _T("txt"), _T("Untitled"),
            OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
            _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"));
        if (dlg.DoModal() == IDOK)
            m_statusBar.SetPaneText(ID_DEFAULT_PANE, _T("File saved"));
        return 0;
    }

    LRESULT OnFileExit(WORD, WORD, HWND, BOOL&)
    {
        PostMessage(WM_CLOSE);
        return 0;
    }

    LRESULT OnEditCmd(WORD, WORD wID, HWND, BOOL&)
    {
        (void)wID;
        return 0;
    }

    LRESULT OnToolsOptions(WORD, WORD, HWND, BOOL&)
    {
        CGeneralPage pageGeneral;
        CAdvancedPage pageAdvanced;
        WTL::CPropertySheet sheet(_T("Options"), 0, m_hWnd);
        sheet.AddPage(pageGeneral);
        sheet.AddPage(pageAdvanced);
        sheet.DoModal();
        return 0;
    }

    LRESULT OnViewControls(WORD, WORD, HWND, BOOL&)
    {
        CAllControlsDlg dlg;
        dlg.DoModal();
        return 0;
    }

    LRESULT OnAppAbout(WORD, WORD, HWND, BOOL&)
    {
        CAboutDlg dlg;
        dlg.DoModal();
        return 0;
    }
};

///////////////////////////////////////////////////////////////////////////////
// Horizontal splitter compile test

class CHSplitView : public WTL::CHorSplitterWindow
{
public:
    WTL::CListViewCtrl m_listTop;
    WTL::CEdit         m_editBottom;
};

///////////////////////////////////////////////////////////////////////////////
// wWinMain - Application entry point

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/,
                    LPWSTR /*lpCmdLine*/, int nCmdShow)
{
    // Initialize COM
    HRESULT hRes = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    ATLASSERT(SUCCEEDED(hRes));

    // Initialize ALL common control classes
    WTL::AtlInitCommonControls(
        ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_USEREX_CLASSES |
        ICC_DATE_CLASSES | ICC_INTERNET_CLASSES | ICC_PAGESCROLLER_CLASS |
        ICC_NATIVEFNTCTL_CLASS | ICC_STANDARD_CLASSES | ICC_LINK_CLASS |
        ICC_PROGRESS_CLASS | ICC_TREEVIEW_CLASSES | ICC_LISTVIEW_CLASSES |
        ICC_TAB_CLASSES | ICC_UPDOWN_CLASS | ICC_HOTKEY_CLASS |
        ICC_ANIMATE_CLASS);

    // Load RichEdit library (required before creating RICHEDIT50W controls)
    HMODULE hRichEdit = ::LoadLibrary(_T("msftedit.dll"));
    if (hRichEdit == NULL)
        hRichEdit = ::LoadLibrary(_T("riched20.dll"));  // fallback
    (void)hRichEdit;

    // Initialize application module
    hRes = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hRes));
    (void)hRes;

    // Create message loop
    WTL::CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    // Create and show main frame
    CMainFrame wndMain;
    if (wndMain.CreateEx() == NULL) {
        ATLTRACE(_T("Main window creation failed!\n"));
        return 0;
    }

    wndMain.ShowWindow(nCmdShow);
    wndMain.UpdateWindow();

    // Run message loop
    int nRet = theLoop.Run();

    // Cleanup
    _Module.RemoveMessageLoop();
    _Module.Term();
    ::CoUninitialize();

    return nRet;
}
