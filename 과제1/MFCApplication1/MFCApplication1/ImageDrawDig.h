#pragma once
#include "afxdialogex.h"
#include <iostream>


#define MAX_CLICKCOUNT 3
#define BLACK 0
#define WHITE 255


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
	afx_msg void OnPaint();

	void SetCircleRadius(int InValue) { CircleRadius = InValue; }
	void SetLineThickness(int InValue) { LineThickness = InValue; }

private:
	void ImageInit();

	//Editable Properties
	int SmallCircleRadius = 10;
	int LineThickness = 0;



	int ClickCount=0;
	int CircleRadius = 0;

	CPoint Points[3];

	void DrawSmallCircle(const CPoint& point);
	void DrawResultCircle();

	bool IsValidBit(int x, int y);

	CPoint GetCircleCenterCoordinate();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
