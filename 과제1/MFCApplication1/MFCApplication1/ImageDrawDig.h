#pragma once
#include "afxdialogex.h"
#include <iostream>



#define MAX_CLICKCOUNT 3
#define MAX_DOTS 3
#define BLACK 0
#define WHITE 255


class ImageDrawDig : public CDialogEx
{
	DECLARE_DYNAMIC(ImageDrawDig)

public:
	class CMFCApplication1Dlg* Parent;
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
	void SetLineThickness(int InValue) { LineThickness = InValue;  SmallCircleRadius = LineThickness + 20; }
	void ResetProcess();
	void GenerateRandomDots();

private:
	void CreateImage();

	//Editable Properties
	int SmallCircleRadius = 10;
	int SelectTolerance = 10;


	//NON-EDITABLE 
	int LineThickness = 0;
	int CircleRadius = 0;
	int SelectedDotIndex = -1;

	//초기화해줘야하는 변수들?
	bool bDotMoveEnable = false;
	int ClickCount = 0;
	CPoint Points[3];

	void DrawSmallCircle(const CPoint& point, int InColor);
	void DrawResultCircle(int InColor);
	void AdjustDotLoction(const CPoint& CursorLocation);

	bool IsValidBit(int x, int y);

	CPoint GetCircleCenterCoordinate();

	int GetDistance(int cx, int x1, int cy, int y1);
	void MakeImageWhite();
	void RedrawCircle();
	void EraseCircle();



	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void UpdateUI(const CPoint& point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
