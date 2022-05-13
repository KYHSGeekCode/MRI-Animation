//quick GDI MRI animator
//양현서 

#include <windows.h>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <algorithm>
#include <vector>
using namespace std;

#include "resource.h"
#include "Classes.h"
//enum bitname
//{
//	IDB_BITLIGHT=101,
//	B_END
//};
void TransBlt(HBITMAP hbitmap);

HDC hdc; 
HINSTANCE g_hinst;
HWND hWnd;

HBITMAP bitmaps[NUMBIT];

XFORM ident={1,0,0,1,0,0};

//몇 초부터 몇 초까지 이 행동을해라
//Object.ChangeUpdater(AnimationUpdater *au);
//void update(DWORD dt){animUpdater->Update(dt);HDC hdc;//CS_OWNDC
AnimUpdate::AnimUpdate()
	{
		life=0;
		dx=0;
		dy=0;
		createTime=GetTickCount();
		deg=ddeg=0;
		ZeroMemory(&trans,sizeof(trans));
	}
AnimUpdate::AnimUpdate(float dx,float dy,degree _ddeg):dx(dx),dy(dy),ddeg(_ddeg){ZeroMemory(&trans,sizeof(trans));}
AnimUpdate::AnimUpdate(int x, int y, degree deg, float dx,float dy,degree _ddeg):deg(deg),dx(dx),dy(dy),ddeg(_ddeg)
	{
		ZeroMemory(&trans,sizeof(trans));
		trans.eDx=x;
		trans.eDy=y;
	}
	
	//이번 소스에서는충돌위험은없다
void AnimUpdate::Update(DWORD dt)
	{
		float deltaTime=(float)dt/1000.0f;
		deg=ddeg*dt;
		while(deg>360)deg-=360;
		while(deg<0)deg+=360;
		//deg=fmod(deg,360.0f);
		trans.eDx+=dx;
		trans.eDy+=dy;
		trans.eM11=(float)cos(deg*3.14/180);
		trans.eM12=(float)sin(deg*3.14/180);
		trans.eM21=(float)-sin(deg*3.14/180);
		trans.eM22=(float)cos(deg*3.14/180);
	}
void AnimUpdate::Apply()
	{
		SetWorldTransform(hdc,&trans);
	}

AnimObject::AnimObject(HBITMAP img, AnimUpdate *a):img(img),updater(a){}
void AnimObject::Update(DWORD dt)
		{
			updater->Update(dt);
		}
void AnimObject::Draw()
		{
			updater->Apply();
			TransBlt(img);
			SetWorldTransform(hdc,&ident);
			//MessageBox(0,"draw","draw",0);
		}

vector<AnimObject> objects;

void OnUpdate(DWORD dt)
{
	//hdc=BeginPaint(hWnd,&ps);
	RECT rect;
	GetClientRect(hWnd,&rect);
	FillRect(hdc,&rect,(HBRUSH)(COLOR_WINDOW+1));
	int siz=objects.size();
	//for(vector<AnimObject>::iterator it=objects.begin();it!=objects.end();++it)
	for(int i=0;i<siz;i++)
	{
		objects[i].Update(dt);
	}
	//for(vector<AnimObject>::iterator it=objects.begin();it!=objects.end();++it)
	for(int i=0;i<siz;i++)
	{
		objects[i].Draw();
	}	
//	for_each(objects.begin(),objects.end(),std::mem_fun(&AnimObject::Update(dt));
//	for_each(objects.begin(),objects.end(),std::mem_fun(&AnimObject::Draw);
}

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			if(hdc!=NULL)
			{
				ReleaseDC(hwnd,hdc);
				hdc=NULL;
			}
			for(int i=101;i<B_END;i++)
			{
				DeleteObject(bitmaps[i]);
			}
			PostQuitMessage(0);
			break;
		}
		case WM_CREATE:
			hdc=GetDC(hwnd);
			SetGraphicsMode(hdc,GM_ADVANCED);
			for(int i=101;i<B_END;i++)
			{
				bitmaps[i]=LoadBitmap(g_hinst,MAKEINTRESOURCE(i));
				//#ifdef DEBUG
				if(bitmaps[i]==NULL)
				{
					char buf[10];
					sprintf(buf,"%d",i);
					MessageBox(0,"failed",buf,0);
					return -1;
				}
				//#endif
			}
			MakeObjects();
			//GEtSome REsources
			return 0;
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	g_hinst=hInstance;
	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.style		 = CS_OWNDC;
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Animation",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	hWnd=hwnd;
	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
//	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
//		TranslateMessage(&msg); /* Translate key codes to chars if present */
//		DispatchMessage(&msg); /* Send it to WndProc */
//	}
	DWORD starttick=GetTickCount();
	DWORD lastTick=starttick;
	DWORD nowTick=lastTick;
	DWORD dt=1;
	BOOL done=FALSE;
	while(!done)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			nowTick=GetTickCount();
			dt=nowTick-lastTick;
			OnUpdate(dt);							// Update and Draw The Scene
			UpdateObjects(nowTick-starttick);
			SwapBuffers(hdc);					// Swap Buffers (Double Buffering)		
		}
	}
	return msg.wParam;
}

void TransBlt(HBITMAP hbitmap)
{
	COLORREF clrMask=RGB(0xff,0,0);
	BITMAP bm;
	COLORREF cColor;
	HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
	HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
	HDC		hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
	POINT	ptSize;

	hdcTemp = CreateCompatibleDC(hdc);
	SelectObject(hdcTemp, hbitmap);
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	DPtoLP(hdcTemp, &ptSize,1);

	hdcBack   = CreateCompatibleDC(hdc);
	hdcObject = CreateCompatibleDC(hdc);
	hdcMem    = CreateCompatibleDC(hdc);
	hdcSave   = CreateCompatibleDC(hdc);

	bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
	bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

	bmBackOld   = (HBITMAP) SelectObject(hdcBack, bmAndBack);
	bmObjectOld = (HBITMAP) SelectObject(hdcObject, bmAndObject);
	bmMemOld    = (HBITMAP) SelectObject(hdcMem, bmAndMem);
	bmSaveOld   = (HBITMAP) SelectObject(hdcSave, bmSave);

	SetMapMode(hdcTemp, GetMapMode(hdc));

	BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	cColor = SetBkColor(hdcTemp, clrMask);

	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	SetBkColor(hdcTemp, cColor);

	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, NOTSRCCOPY);
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdc      , 0/*x*/,0/*y*/, SRCCOPY);
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack  , 0, 0, SRCAND);
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdcTemp  , 0, 0, SRCPAINT);
	BitBlt(hdc    , 0/*x*/, 0/*y*/, ptSize.x, ptSize.y, hdcMem   , 0, 0, SRCCOPY);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave  , 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hdcBack, bmBackOld));
	DeleteObject(SelectObject(hdcObject, bmObjectOld));
	DeleteObject(SelectObject(hdcMem, bmMemOld));
	DeleteObject(SelectObject(hdcSave, bmSaveOld));
	
	DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcSave);
	DeleteDC(hdcTemp);
}


