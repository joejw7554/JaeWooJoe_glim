#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "ImageDrawDig.h"



IMPLEMENT_DYNAMIC(ImageDrawDig, CDialogEx)

ImageDrawDig::ImageDrawDig(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ImageDrawDig, pParent)
{
	Parent = pParent;
}

ImageDrawDig::~ImageDrawDig()
{
}

void ImageDrawDig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ImageDrawDig, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



BOOL ImageDrawDig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	MoveWindow(0, 0, 640, 480);
	ImageInit();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void ImageDrawDig::ImageInit()
{
	int Width = 640;
	int Height = 480;
	int Bpp = 8;
	Image.Create(Width, -Height, Bpp);

	static RGBQUAD rgb[256];
	for (int i = 0; i < 256; i++)
	{
		rgb[i].rgbRed = i;
		rgb[i].rgbBlue = i;
		rgb[i].rgbGreen = i;
	}

	Image.SetColorTable(0, 255, rgb);

	unsigned char* fm = (unsigned char*)Image.GetBits();
	memset(fm, 255, Width * Height);
}

void ImageDrawDig::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	Image.Draw(dc, 0, 0);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.


}


#include <iostream>
void ImageDrawDig::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (ClickCount < MAX_CLICKCOUNT)
	{
		Points[ClickCount] = point;
		ClickCount++;
		std::cout << point.x << ", " << point.y;
	}

	if (ClickCount == MAX_CLICKCOUNT)
	{
		//Request Draw Circle
	}


	CDialogEx::OnLButtonDown(nFlags, point);
}
