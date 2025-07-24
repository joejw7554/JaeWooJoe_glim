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

	Image.SetColorTable(0, 256, rgb);
	unsigned char* fm = (unsigned char*)Image.GetBits();
	memset(fm, WHITE, Width * Height);
}

void ImageDrawDig::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	Image.Draw(dc, 0, 0);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}


void ImageDrawDig::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (ClickCount < MAX_CLICKCOUNT)
	{
		Points[ClickCount] = point;
		ClickCount++;
		std::cout << point.x << ", " << point.y;
		std::cout << std::endl;

		DrawSmallCircle(point);
		std::cout << "Radius: " << CircleRadius << std::endl;
		std::cout << "Thickness: " << LineThickness << std::endl;

	}

	if (ClickCount == MAX_CLICKCOUNT)
	{

		DrawResultCircle();
		std::cout << "Draw Complete" << std::endl;
	}


	CDialogEx::OnLButtonDown(nFlags, point);
}

void ImageDrawDig::DrawSmallCircle(const CPoint& point)
{
	int Width = Image.GetWidth();
	int Height = Image.GetHeight();
	int Pitch = Image.GetPitch();


	unsigned char* fm = (unsigned char*)Image.GetBits();

	for (int i = point.y - SmallCircleRadius; i <= point.y + SmallCircleRadius; i++)
	{
		for (int j = point.x - SmallCircleRadius; j <= point.x + SmallCircleRadius; j++)
		{
			int dx = j - point.x;
			int dy = i - point.y;
			if (dx * dx + dy * dy <= SmallCircleRadius * SmallCircleRadius)
			{
				fm[i * Pitch + j] = BLACK;
			}
		}
	}

	Invalidate();
}

void ImageDrawDig::DrawResultCircle()
{
	// 3개 점을 지나는 원의 중심 구하기
	CPoint CircleCenter = GetCircleCenterCoordinate();


	//원 그리는 과정
	int Width = Image.GetWidth();
	int Height = Image.GetHeight();
	int Pitch = Image.GetPitch();

	unsigned char* fm = (unsigned char*)Image.GetBits();


	for (int i = 0; i < Height; i++)
	{
		for (int j = 0;  j < Width;  j++)
		{
			int Distance = sqrt(pow(j - CircleCenter.x, 2) + pow(i - CircleCenter.y, 2));

			if (abs(Distance - CircleRadius) < LineThickness)
			{
				if (IsValidBit(j,i))
				{
					fm[i * Pitch + j] = BLACK;
				}
			}
		}
	}

	Invalidate();
}

bool ImageDrawDig::IsValidBit(int x, int y)
{
	int Width = Image.GetWidth();
	int Height = Image.GetHeight();

	CRect rect(0, 0, Width, Height);

	return rect.PtInRect(CPoint(x,y));
}

CPoint ImageDrawDig::GetCircleCenterCoordinate()
{
	int x1 = Points[0].x, y1 = Points[0].y;
	int x2 = Points[1].x, y2 = Points[1].y;
	int x3 = Points[2].x, y3 = Points[2].y;

	float A = (float)x1 - x2;
	float B = (float)y1 - y2;
	float C = (float)x1 - x3;
	float D = (float)y1 - y3;

	float E =(float) (x1 * x1 - x2 * x2) + (y1 * y1 - y2 * y2);
	float F =(float) (x1 * x1 - x3 * x3) + (y1 * y1 - y3 * y3);

	float Det = 2.0 * (A * D - B * C);

	float cx = (E * D - F * B) / Det;
	float cy = (F * A - E * C) / Det;

	CircleRadius = sqrt(pow(cx - x1, 2) + pow(cy - y1, 2));

	return CPoint(cx, cy);
}
