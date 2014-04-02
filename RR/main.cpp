#include <windows.h> 
#include <scrnsave.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include<sstream>
#pragma comment(lib,"ScrnSave.lib")
#pragma comment(lib,"Comctl32.lib")
#define ID_TIMER 32767
#define RAD 57.295779513082320876798154814105
int wx,wy;
int width,height;
std::ostringstream str;
HDC bmphdc;
HBITMAP bmp[16];
HDC backhdc;
int** field;
static HDC hdc,hdc2;
RECT rc;
static HINSTANCE hInst;

int drawScreen(int,int,float);
int XdrawScreen(int,int,int,int);
int YdrawScreen(int,int,int,int);
int fieldReset(int,int);

HDC Back_Init_Surface(HWND hwnd){
	HDC hdc,BackDC;
	HBITMAP hBackBMP;

	hdc = GetDC(hwnd);
	
	BackDC = CreateCompatibleDC(hdc);
	//âÊñ ÇÃëÂÇ´Ç≥ÇæÇØBMPÇ¡Ç€Ç¢óÃàÊÇçÏÇÈ
	hBackBMP = CreateCompatibleBitmap(hdc,wx,wy);
	//è„ÇÃìÒÇ¬ÇçáëÃ
	SelectObject(BackDC,hBackBMP);
	//çÏÇ¡ÇΩóÃàÊÇçïÇ≈ìhÇËÇ¬Ç‘Ç∑
	PatBlt(BackDC,0,0,wx,wy,BLACKNESS);
	//BMPÇ¡Ç€Ç¢óÃàÊÇÕÇ‡Ç§Ç¢ÇÁÇ»Ç¢Ç©ÇÁâï˙
	DeleteObject(hBackBMP);
	//GetDCÇµÇΩÇ»ÇÁReleaseDC
	ReleaseDC(hwnd,hdc);
	//ó†âÊñ Çï‘Ç∑
	return BackDC;
}

LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	int a;
	static int plus=0;
	static int xflag,yflag;
	switch(msg){   
		case WM_CREATE:
			
			GetClientRect(hWnd,&rc);
			wx = rc.right-rc.left;
			wy = rc.bottom-rc.top;
			//--------------------------------
			width=(wx/175)+2;
			height=(wy/175)+2;
			//--------------------------------
			srand((unsigned int)time(NULL));
			field=new int*[width];
			for(int i=0;i<width;i++){
				field[i]=new int[height];
					for(int j=0;j<height;j++){
						field[i][j]=rand()%8;
				}
			}
			
			yflag=(rand()%width);
			xflag=(rand()%height);

			hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
			bmp[0] = LoadBitmap(hInst, "IMAGEg1");
			bmp[1] = LoadBitmap(hInst, "IMAGEg2");
			bmp[2] = LoadBitmap(hInst, "IMAGEg3");
			bmp[3] = LoadBitmap(hInst, "IMAGEg4");

			bmp[4] = LoadBitmap(hInst, "IMAGE1");
			bmp[5] = LoadBitmap(hInst, "IMAGE2");
			bmp[6] = LoadBitmap(hInst, "IMAGE3");
			bmp[7] = LoadBitmap(hInst, "IMAGE4");
			
			bmphdc = CreateCompatibleDC(NULL);
			SelectObject(bmphdc, bmp[0]);
			hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
			backhdc = Back_Init_Surface(hWnd);
			SetTimer(hWnd,ID_TIMER,8,NULL);
			break;
		case WM_TIMER:
			
			hdc = GetDC(hWnd);
			PatBlt(backhdc,0,0,wx,wy,BLACKNESS);
			//=====================================================================
			for(int i=0;i<width;i++){
				for(int j=0;j<height;j++){
					drawScreen(i,j,plus);
				}
			}
			plus+=5;
			if(plus>=175){
			plus=0;
			fieldReset(width,height);
			}			
			//=====================================================================
			BitBlt(hdc,0,0,wx,wy,backhdc,0,0,SRCCOPY);
			ReleaseDC(hWnd,hdc);
			break;
		case WM_DESTROY:
			KillTimer(hWnd,ID_TIMER);
			PostQuitMessage(0);
			return 0;
		default:
			break;
	}
	return DefScreenSaverProc(hWnd, msg, wParam, lParam);
}

int drawScreen(int i,int j,float plus){
	int x,y;
	SelectObject(bmphdc, bmp[field[i][j]]);
	x=(i-1)*175+plus;
	y=(j-1)*175+plus;
	BitBlt(backhdc,x,y,x+2*175,y+2*175,bmphdc, 0, 0, SRCCOPY);
	return 0;
}

int XdrawScreen(int width,int i,int j,int gas){
	int x,y;
	SelectObject(bmphdc, bmp[field[i][j]]);
	x=(i-1)*175;
	y=(j-1)*175;
	BitBlt(backhdc,x+gas,y,x+2*175+gas,y+2*175,bmphdc, 0, 0, SRCCOPY);
	return 0;
}

int YdrawScreen(int height,int i,int j,int gas){
	int x,y;
	SelectObject(bmphdc, bmp[field[i][j]]);
	x=(i-1)*175;
	y=(j-1)*175;
	BitBlt(backhdc,x,y+gas,x+2*175,y+2*175+gas,bmphdc, 0, 0, SRCCOPY);
	return 0;
}

int fieldReset(int width,int height){
	int *yoko,*tate;
	yoko=new int[width];
	tate=new int[height];
	for(int i=0;i<width;i++){
		yoko[i]=field[i][height-1];
	}
	for(int j=0;j<height;j++){
		tate[j]=field[width-1][j];
	}
	for(int i=width-1;i>0;i--){
		for(int j=height-1;j>0;j--){
			field[i][j]=field[i-1][j-1];
		}
	}
	for(int i=0;i<width;i++){
		field[i][0]=yoko[i];
	}
	for(int j=0;j<height;j++){
		field[0][j]=tate[j];
	}
	return 0;
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
	return TRUE;
}
BOOL WINAPI RegisterDialogClasses(HANDLE hInst){
	return TRUE;
}