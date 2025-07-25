#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "ImageDrawDig.h"
#include "MFCApplication1Dlg.h"




IMPLEMENT_DYNAMIC(ImageDrawDig, CDialogEx)

ImageDrawDig::ImageDrawDig(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ImageDrawDig, pParent)
{
	Parent = (CMFCApplication1Dlg*)pParent;
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
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



BOOL ImageDrawDig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	MoveWindow(0, 0, 640, 480);
	CreateImage();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void ImageDrawDig::ResetProcess()
{
	MakeImageWhite();

	for (CPoint& point : Points)
	{
		point.x = 0;
		point.y = 0;
	}
	ClickCount = 0;

	for (int i = 0; i < MAX_CLICKCOUNT + 1; ++i)
	{
		CString str;
		str.Format(_T("Dot%d Coordinate:", ), i + 1);
		int id = IDC_STATIC_DOT1 + i; // 연속된 ID라면 가능
		Parent->SetDlgItemTextW(id, str);
	}

}

void ImageDrawDig::GenerateRandomDots()
{
	if (ClickCount < 3) return;

	EraseCircle();

	int Width = Image.GetWidth();
	int Height = Image.GetHeight();

	for (CPoint& point : Points)
	{
		point.x = rand() % Width;
		point.y = rand() % Height;
		DrawSmallCircle(point, BLACK);
	}

	DrawResultCircle(BLACK);
}

void ImageDrawDig::CreateImage()
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
	MakeImageWhite();
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
		DrawSmallCircle(point, BLACK);


		Invalidate();
		std::cout << point.x << ", " << point.y;
		std::cout << std::endl;



		if (ClickCount == MAX_CLICKCOUNT) //3번쨰 클릭때 점3개를 지나는 원 그려
		{
			DrawResultCircle(BLACK);
			std::cout << "Draw Complete" << std::endl;
			bDotMoveEnable = true;
			std::cout << bDotMoveEnable << std::endl;
		}
		return;
	}

	if (ClickCount == MAX_CLICKCOUNT)
	{
		bDotMoveEnable = true;
	}


	if (bDotMoveEnable)
	{
		for (int i = 0; i < MAX_DOTS; i++)
		{
			int DebugResult = GetDistance(point.x, Points[i].x, point.y, Points[i].y);

			if (GetDistance(point.x, Points[i].x, point.y, Points[i].y) < SelectTolerance)
			{
				SelectedDotIndex = i;
				std::cout << "you Selected: " << Points[SelectedDotIndex].x << ", " << Points[SelectedDotIndex].y << " Dot" << std::endl;
				break;
			}
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);


}

void ImageDrawDig::UpdateUI(const CPoint& point)
{
	if (Parent)
	{
		for (int i = 0; i < ClickCount; ++i)
		{
			CString str;
			str.Format(_T("Dot%d Coordinate: (%d, %d)"), i + 1, Points[i].x, Points[i].y);
			int id = IDC_STATIC_DOT1 + i; // 연속된 ID라면 가능
			Parent->SetDlgItemTextW(id, str);
		}
	}
}

void ImageDrawDig::DrawSmallCircle(const CPoint& point, int InColor)
{

	UpdateUI(point);

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
				if (IsValidBit(j, i))
				{
					fm[i * Pitch + j] = InColor;
				}
			}
		}
	}

}

void ImageDrawDig::DrawResultCircle(int InColor)
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
		for (int j = 0; j < Width; j++)
		{
			int Distance = GetDistance(j, CircleCenter.x, i, CircleCenter.y);

			if (abs(Distance - CircleRadius) < LineThickness)
			{
				if (IsValidBit(j, i))
				{
					fm[i * Pitch + j] = InColor;
				}
			}
		}
	}
}

void ImageDrawDig::AdjustDotLoction(const CPoint& CursorLocation)
{
	Points[SelectedDotIndex].SetPoint(CursorLocation.x, CursorLocation.y);
	DrawResultCircle(BLACK);
}

bool ImageDrawDig::IsValidBit(int x, int y)
{
	int Width = Image.GetWidth();
	int Height = Image.GetHeight();

	CRect rect(0, 0, Width, Height);

	return rect.PtInRect(CPoint(x, y));
}

CPoint ImageDrawDig::GetCircleCenterCoordinate()
{
	int x1 = Points[0].x, y1 = Points[0].y;
	int x2 = Points[1].x, y2 = Points[1].y;
	int x3 = Points[2].x, y3 = Points[2].y;

	int A = x1 - x2;
	int B = y1 - y2;
	int C = x1 - x3;
	int D = y1 - y3;

	int E = (x1 * x1 - x2 * x2) + (y1 * y1 - y2 * y2);
	int F = (x1 * x1 - x3 * x3) + (y1 * y1 - y3 * y3);

	int Det = 2 * (A * D - B * C);

	int cx = (E * D - F * B) / Det;
	int cy = (F * A - E * C) / Det;

	CircleRadius = GetDistance(cx, x1, cy, y1);

	return CPoint(cx, cy);
}

int ImageDrawDig::GetDistance(int x1, int x2, int y1, int y2)
{
	return (int)sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void ImageDrawDig::OnMouseMove(UINT nFlags, CPoint point)
{

	if (bDotMoveEnable && SelectedDotIndex != -1) 
	{
		//원으로 그렸던 부분만 다시 하양색으로 지우기
		EraseCircle();

		//선택한 점위치 갱신후 다시 그리기
		Points[SelectedDotIndex] = point;

		RedrawCircle();

		Invalidate();
	}


	CDialogEx::OnMouseMove(nFlags, point);
}

void ImageDrawDig::RedrawCircle()
{
	for (int i = 0; i < MAX_CLICKCOUNT; i++) //작은 원 그리기
	{
		DrawSmallCircle(Points[i], BLACK);
	}

	DrawResultCircle(BLACK); //큰 원 그리기
}

void ImageDrawDig::EraseCircle()
{
	DrawResultCircle(WHITE); //큰 원 없애기

	for (int i = 0; i < MAX_CLICKCOUNT; i++) //작은 원 없애기
	{
		DrawSmallCircle(Points[i], WHITE);
	}
}

void ImageDrawDig::MakeImageWhite()
{
	int Width = Image.GetWidth();
	int Height = Image.GetHeight();

	unsigned char* fm = (unsigned char*)Image.GetBits();

	memset(fm, WHITE, Width * Height);
}


void ImageDrawDig::OnLButtonUp(UINT nFlags, CPoint point)
{
	DrawSmallCircle(Points[SelectedDotIndex], BLACK);

	bDotMoveEnable = false;
	SelectedDotIndex = -1;

	CDialogEx::OnLButtonUp(nFlags, point);
}
