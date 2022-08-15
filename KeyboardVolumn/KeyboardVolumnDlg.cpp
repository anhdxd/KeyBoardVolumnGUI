
// KeyboardVolumnDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "KeyboardVolumn.h"
#include "KeyboardVolumnDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKeyboardVolumnDlg dialog



CKeyboardVolumnDlg::CKeyboardVolumnDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KEYBOARDVOLUMN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeyboardVolumnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKeyboardVolumnDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HOTKEY()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_MYMESSAGE, OnMSGIconTaskCallBack)
	ON_BN_CLICKED(IDOK, &OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CKeyboardVolumnDlg::OnBnClickedCancel)
//	ON_WM_ACTIVATE()
//ON_WM_CREATE()
//ON_WM_ACTIVATEAPP()
END_MESSAGE_MAP()


// CKeyboardVolumnDlg message handlers
BOOL CKeyboardVolumnDlg::m_bShowWindow = false;
BOOL CKeyboardVolumnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	HRESULT hr;

	IMMDevice* defaultDevice = NULL;
	IMMDeviceEnumerator* deviceEnumerator = NULL;
	BOOL bMute, bRes;
	HKEY hkey;
	WCHAR AutoStartPath[260];
	bRes = RegisterHotKey(GetSafeHwnd(), MY_ID_HOTKEY, MOD_ALT, 0x31);
	bRes = RegisterHotKey(GetSafeHwnd(), MY_ID_HOTKEY, MOD_ALT, 0x32);
	bRes = RegisterHotKey(GetSafeHwnd(), MY_ID_HOTKEY, MOD_ALT, 0x33);

	// Auto Start Windows
#ifdef DEBUG
#else
	GetModuleFileNameW(0, AutoStartPath, MAX_PATH);
	RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hkey);
	int i = RegSetValueExW(hkey, L"VolumnKeyboardAnhdz", NULL, REG_SZ, (const BYTE*)AutoStartPath, lstrlen(AutoStartPath) * sizeof(WCHAR) + 1);
	RegCloseKey(hkey);
#endif // DEBUG


	// Init
	hr = CoInitialize(NULL);
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&audio);
	defaultDevice->Release();
	defaultDevice = NULL;

	NOTIFYICONDATA nid;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = GetSafeHwnd();
	nid.uID = MY_ID_ICONTASK;
	nid.uVersion = NOTIFYICON_VERSION;
	nid.uCallbackMessage = WM_MYMESSAGE;
	nid.hIcon = m_hIcon;
	wcscpy_s(nid.szTip, L"Keyboard Volumn");
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

	//if(!Shell_NotifyIconW(NIM_SETVERSION, &nid))
	Shell_NotifyIconW(NIM_ADD, &nid);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKeyboardVolumnDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	if (!m_bShowWindow)
	{
		ShowWindow(0);
		m_bShowWindow = true;
	}
}

void CKeyboardVolumnDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{


	float pfLevel = 0;
	BOOL bMute;
	audio->GetMasterVolumeLevelScalar(&pfLevel);
	if (nKey2 == 0x33)
	{
		audio->SetMute(0, 0);
		audio->VolumeStepUp(0);
	}
	if (nKey2 == 0x32)
	{
		audio->SetMute(0, 0);
		audio->VolumeStepDown(0);
	}
	if (nKey2 == 0x31)
	{
		audio->GetMute(&bMute);
		if (bMute)
			audio->SetMute(0, 0);
		else
			audio->SetMute(1, 0);
	}
	
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKeyboardVolumnDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKeyboardVolumnDlg::OnClose()
{
	NOTIFYICONDATA nid;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = GetSafeHwnd();
	nid.uID = MY_ID_ICONTASK;

	UnregisterHotKey(GetSafeHwnd(), MY_ID_HOTKEY);
	Shell_NotifyIconW(NIM_DELETE, &nid);
}

LRESULT CKeyboardVolumnDlg::OnMSGIconTaskCallBack(WPARAM wParam, LPARAM lParam)
{
	switch ((int)lParam)
	{
	case WM_LBUTTONDBLCLK:
		ShowWindow(1);
		UpdateWindow();
		break;

	default:
		break;
	}

	
	return LRESULT();
}

void CKeyboardVolumnDlg::OnBnClickedOk()
{

	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
	return;
}


void CKeyboardVolumnDlg::OnBnClickedCancel()
{
	NOTIFYICONDATA nid;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = m_hWnd;
	nid.uID = MY_ID_ICONTASK;

	UnregisterHotKey(GetSafeHwnd(), MY_ID_HOTKEY);
	Shell_NotifyIconW(NIM_DELETE, &nid);

	CDialogEx::OnCancel();
}


//void CKeyboardVolumnDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
//
//	// TODO: Add your message handler code here
//}


//int CKeyboardVolumnDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  Add your specialized creation code here
//
//	return 0;
//}


//void CKeyboardVolumnDlg::OnActivateApp(BOOL bActive, DWORD dwThreadID)
//{
//	CDialogEx::OnActivateApp(bActive, dwThreadID);
//	ShowWindow(0);
//	UpdateWindow();
//	// TODO: Add your message handler code here
//}
