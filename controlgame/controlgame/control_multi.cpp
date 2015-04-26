#include <Windows.h>
#include <time.h>
#include "Page.h"
#include "Button.h"

//������ ���ν����� ����
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//�������� ���ν��� ����

HINSTANCE g_hInst;//�ν��Ͻ� �ڵ��� �����ϱ� ���� ��������


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	TCHAR szClassName[] = L"winMain";//������ Ŭ���� �̸�
	TCHAR szTitle[] = L"now loading";//Ÿ��Ʋ �ٿ� ��µ� �̸�

	MSG msg;			//�޼��� ����ü
	HWND hWnd;		//������ �ڵ�
	WNDCLASSEX wc;	//������ Ŭ���� ����ü
	g_hInst = hInstance;	//�ν��� �ڵ� ����
	//------------------------------------------------------ :: ������ ����ü�� ���� �����Ѵ�. ::-----------------------------------
	wc.cbSize = sizeof(WNDCLASSEX);										//������ Ŭ���� ����ü�� ũ��
	wc.style = CS_HREDRAW | CS_VREDRAW;								//Ŭ���� ��Ÿ��

	//������ ���ν����� ������Ŭ���� ����ü�� ���

	wc.lpfnWndProc = WndProc;											//������ ���ν��� ����
	wc.cbClsExtra = 0;															//������ Ŭ���� ������ ����
	wc.cbWndExtra = 0;														//�������� ������ ����
	wc.hInstance = hInstance;												//�ν��Ͻ� �ڵ�
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);						//������ �ڵ�
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);						//����� Ŀ�� �ڵ�
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//������ �귯��
	wc.lpszMenuName = NULL;												//�޴� �̸�
	wc.lpszClassName = szClassName;										//������ Ŭ���� �̸�
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);					//���� ������ �ڵ�
	//------------------------------------------------------ :: ������ Ŭ������ ����Ѵ�.
	RegisterClassEx(&wc);							//   :: ������ Ŭ���� ���
	//------------------------------------------------------ :: ������ �����츦 ����
	hWnd = CreateWindow(
		szClassName,								//������ Ŭ���� �̸�
		szTitle,											//Ÿ��Ʋ �ٿ� ��µ� ���ڿ�, �տ��� ������
		WS_OVERLAPPEDWINDOW,				//������ ��Ÿ��, �տ��� ����
		CW_USEDEFAULT,							//������ ���� ����� X ��ǥ
		CW_USEDEFAULT,							//������ ���� ����� Y ��ǥ
		CLIENT_WIDTH,							//�������� ��
		CLIENT_HEIGHT,							//�������� ����
		NULL,											//�θ� �ڵ��� ����
		NULL,											//�޴�, Ȥ�� �ڽ� �������� �ĺ���
		hInstance,									//�����츦 ������ �ν��Ͻ� �ڵ�,  WinMain�� ù��° ����
		NULL											//CREATESTRUCT ����ü�� ���� ���޵Ǵ� ��
		);

	if (!hWnd)						//������ ������ ���� ��
		return(FALSE);			//���α׷� ����
	//------------------------------------------------------ :: ������ �����츦 ȭ�鿡 ���
	ShowWindow(hWnd, nCmdShow);				//�����츦 ȭ�鿡 ��Ÿ����.
	UpdateWindow(hWnd);								//�������� Ŭ���̾�Ʈ ������ ĥ�Ѵ�.
	//------------------------------------------------------ :: �޽��� ���� : ������ ���� �޼��� ť�κ��� �޼����� ���� ó���Ѵ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//	if(!TranslateAccelerator(hWnd,hAccel,&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static line l[3];
	HDC hdc;
	PAINTSTRUCT ps;
	HDC LayDC;
	HBITMAP Lay;
	static pButtonA button[3];
	static bool Gameover;
	switch (uMsg)
	{
	case WM_CREATE:
		srand((unsigned int)time(NULL));
		Gameover = false;
		for (auto& p : l)
		{
			p.SetFrequenct(rand() % 10);
			p.SetSpeed(rand() % 15 + 5);
		}

		for (int i = 0; i < 3; ++i)
		{
			button[i].Initialize(NULL, hWnd, { 0, 0 }, true, { 0, 0, 0, 0 },
				true, { 100 + i * 250, 300, 300 + i * 250, 350 });
			TCHAR str[5];
			wsprintf(str, L"%c", TCHAR(i + 'A'));
			button[i].SetName(str);
			button[i].SetLine(&l[i]);
		}

		SetTimer(hWnd, 0, 16, NULL);
		break;

	case WM_LBUTTONDOWN:
		if (Gameover)
		{
			Gameover = false;
			for (auto& p : l)
			{
				p.SetFrequenct(rand() % 10);
				p.SetSpeed(rand() % 15 + 5);
			}
			SetTimer(hWnd, 0, 16, NULL);
		}

		for (int i = 0; i < 3; ++i)
			button[i].OnProcessingMouseMessage(hWnd, uMsg, wParam, lParam);

		break;

	case WM_MOUSEMOVE:
		for (int i = 0; i < 3; ++i)
			button[i].OnProcessingMouseMessage(hWnd, uMsg, wParam, lParam);
		break;

	case WM_LBUTTONUP:
		for (int i = 0; i < 3; ++i)
			button[i].OnProcessingMouseMessage(hWnd, uMsg, wParam, lParam);
		break;

	case WM_KEYDOWN:
		if (wParam == 'Q' || wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 0:

			for (auto& p : l)
			{
				p.addLine();

				if (p.getLine() > CLIENT_WIDTH * 0.8)
				{
					Gameover = true;
					KillTimer(hWnd, 0);
				}
			}
			InvalidateRect(hWnd, NULL, false);
			break;
		}
		break;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		LayDC = CreateCompatibleDC(hdc);
		Lay	 = CreateCompatibleBitmap(hdc, 1000, 800);
		SelectObject(LayDC, Lay);
		Rectangle(LayDC, -1, -1, CLIENT_WIDTH, CLIENT_HEIGHT);
		//-------------------------------------------------

		for (int i = 0; i < 3; ++i)
		{
			TCHAR str[5];
			wsprintf(str, L"%c", TCHAR(i + 'A'));
			RECT rc = { 35, i * 70 + 50, 85, i * 70 + 100 };
			DrawText(LayDC, str, -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			rc = { 85, i * 70 + 50, 85 + l[i].length, i * 70 + 100 };
			SetDCBrushColor(LayDC, l[i].getColor());
			FillRect(LayDC, &rc, (HBRUSH)GetStockObject(DC_BRUSH));

		}

		for (int i = 0; i < 3; ++i)
			button[i].OnDraw(LayDC);

		if (Gameover)
		{
			RECT rc = { 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT };
			DrawText(LayDC, L"Game Over", -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		}

		//-------------------------------------------------

		BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT, LayDC, 0, 0, SRCCOPY);

		DeleteObject(Lay);
		DeleteDC(LayDC);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 0);
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}