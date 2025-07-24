#pragma once
#include "afxdialogex.h"

#define MAX_CLICKCOUNT 3


class ImageDrawDig : public CDialogEx
{
	DECLARE_DYNAMIC(ImageDrawDig)

public:
	CWnd* Parent;
	CImage Image;

public:
	ImageDrawDig(CWnd* pParent = nullptr);   
	virtual ~ImageDrawDig();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ImageDrawDig };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ImageInit();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	int ClickCount=0;

	CPoint Points[3];
};
