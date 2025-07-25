
// MFCApplication1Dlg.h: 헤더 파일
//
#include "ImageDrawDig.h"
#pragma once


// CMFCApplication1Dlg 대화 상자
class CMFCApplication1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	ImageDrawDig* ResultImage;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	int ClickCount = 0;

public:
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnEnChangeEditboxRadius();

	int CircleRadiusInputVal;
	afx_msg void OnEnChangeEditThickness();
	int CircleThicknessInputVal;

	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonRandom();
};
