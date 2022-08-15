
// KeyboardVolumnDlg.h : header file
//

#pragma once

#include <endpointvolume.h>
#include <mmdeviceapi.h>

#define WM_MYMESSAGE            WM_USER +1
#define MY_ID_HOTKEY			1
#define MY_ID_ICONTASK			1
// CKeyboardVolumnDlg dialog
class CKeyboardVolumnDlg : public CDialogEx
{
// Construction
public:
	CKeyboardVolumnDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KEYBOARDVOLUMN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg LRESULT OnMSGIconTaskCallBack(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	IAudioEndpointVolume* audio = NULL;
	afx_msg void OnBnClickedOk();
};
