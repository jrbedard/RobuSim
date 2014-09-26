

/* SOus-SDK de DirectX

DirectX/Win32 encapsulation


 NOTEZ bien qu'il faut ajouter ces librairies dans le menu "link" de setting
  dxguid.lib ddraw.lib dsound.lib dinput.lib wsock32.lib winmm.lib

 Une version de directX supérieure à 7.0 est requise
  www.microsoft.com/directx




///////////////////////////////////////////////////
DIRECT DRAW
DIRECT SOUND

*/


#include "resource.h"
#include "SDH.h"


/********** objets / écran ******************/

MasterScreen::~MasterScreen()
{
	if(lpBack)
	{
		lpBack->Release();
		lpBack = NULL;
	}

	if(lpFront)
	{
		lpFront->Release();
		lpFront = NULL;
	}

	if(lpDD)
	{
		lpDD->Release();
		lpDD = NULL;
	}
}

// ******************  Objects windows 

// initialisation / parametres
bool MasterXHWND::CreateMasterX(char* mytitle,int width, int height,DisplayColor color,WNDPROC proc,HINSTANCE hInst,HICON icon,HCURSOR cursor)
{

	WNDCLASS wc;
	
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = hInst;
	wc.hIcon = icon;
	wc.hCursor = LoadCursor(NULL,IDC_ARROW); // souris pour path
	wc.lpfnWndProc = (WNDPROC) proc;
	wc.lpszClassName = mytitle;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	hwnd = CreateWindow(mytitle,mytitle, WS_OVERLAPPEDWINDOW,
								0,0,1024,760,NULL,NULL,hInst,NULL);
	
	

	if(hwnd == 0)
	{
		return false;
	}

	ShowWindow(hwnd,SW_SHOW);
	UpdateWindow(hwnd);

	int bpp;


	switch(color)
	{
	case COLOR_DEFAULT:
    HDC hdc;

	
   
	hdc = GetDC(NULL);
  
    bpp = GetDeviceCaps(hdc, BITSPIXEL);
  
	ReleaseDC(NULL, hdc);
		break;

	case COLOR_8:
		bpp = 8;
		break;
	case COLOR_16:
		bpp = 16;
		break;
	case COLOR_24:
		bpp = 24;
		break;
	case COLOR_32:
		bpp = 32;
		break;
	}

	
	HRESULT ddrval;
	
	// direct-draw objet principal
	
	ddrval = DirectDrawCreate(NULL, &scr.lpDD,NULL);
	
	if(ddrval != DD_OK)
	{
		MessageBox(NULL,"Erreur lors de création objetDX--assurez vous d'avoir DirectX 7.0 ou supérieur ","Erreur",MB_ICONERROR);
		Kill();
		return false;
	}
	
	ddrval = scr.lpDD->SetCooperativeLevel(NULL, DDSCL_NORMAL);
	if(ddrval != DD_OK)
	{
		scr.lpDD->Release();
		MessageBox(0,"Couldnt Set Cooperative Level, Fatal Error Aborting","Erreur",MB_ICONERROR);		
	}
	
	ddrval = scr.lpDD->SetDisplayMode(width,height,bpp);
	
	

	
	
	
	if(ddrval != DD_OK)
	{
		MessageBox(0,"Error Setting Display Mode",0,MB_ICONERROR);
		scr.lpDD->Release();
		return false;
	}
	

	if(setprimarysurface())
	{
		sound_ok = initSound();
	}
	else
	{
		MessageBox(0,"DirectX non fonctionnel","Broken Software :(",MB_ICONERROR);
		return false;
	}

	initInput();
	text.init((MasterScreen*)&scr);
	paint.init(&scr);
	
	alive = true;

	return true;
}

// Boucle windows messages proc 
WPARAM MasterXHWND::InitLoop(void(* update)(MASTERSCREEN xscr))
{
	
	MSG msg;

	while(1)
    {
		
        int t = PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);
        if (t)
        {
            if (!GetMessage (& msg, NULL, 0, 0))
            return msg.wParam;
            TranslateMessage (&msg);
            DispatchMessage (&msg);
        }
        
        else if (activeapp)
		{	
			clear();
			update(mscr);
			mx_update();
		}
        else if (!activeapp)
		{
         WaitMessage();
		}
    }

 
	Destroy();
	return msg.wParam;
	
}

// fin ,effacer
void MasterXHWND::Destroy()
{

	pKeyboard->Unacquire();
	pKeyboard->Release();
	pDI->Release();
	pDI = NULL;

	if(pDS)
	{
		pDS->Release();
	}

	
}


void MasterXHWND::Kill()
{
	alive = false;
	SendMessage(hwnd,WM_CLOSE,0,0);
}


// écran courante
MASTERSCREEN MasterXHWND::GetScreen()
{
	return mscr;
}

// appliquer l'écran
void MasterXHWND::SetScreen(MASTERSCREEN xmscr)
{
	mscr = xmscr;
}


// messages -> init
LRESULT MasterXHWND::SendMasterMessage(UINT msg,WPARAM wParam,LPARAM lParam)
{
	return SendMessage(hwnd,msg,wParam,lParam);
}

//  clear screen
void MasterXHWND::clear()
{
	ZeroMemory(&ddbltfx,sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = 0;
	scr.lpBack->Blt(NULL,NULL,NULL,DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
}


//"buffer-flip" de manière manuelle
void MasterXHWND::mx_update()
{
	
	HRESULT ddrval;

	
	ddrval = scr.lpFront->Blt(NULL,scr.lpBack , NULL, DDBLT_WAIT, NULL);

//	ddrval = lpDDSPrimary->Blt( &rcRectDest, lpDDSBack, &rcRectSrc, DDBLT_WAIT, NULL);
	
	if( ddrval == DDERR_SURFACELOST )
	{
      restoreAll();
	}

	/*
	ddrval = lpDDSPrimary->Flip( NULL, DDFLIP_WAIT);

	if( ddrval == DDERR_SURFACELOST )
	{
    ddrval = restoreAll();
	}
	*/
}



void MasterXHWND::restoreAll()
{
    HRESULT ddrval;
	// for each surface your app has created you should do the following:
    ddrval = scr.lpDD->RestoreDisplayMode(); // this reattaches the video memory to the surface
    
	if( ddrval == DD_OK )
    {
    //  scr.lpDD->ReLoadBitmap(); // this will be the same as the function above
                                         // that was originally used to load the bitmap
                                         // with the exception that the surface is already
                                         // created and ready to
	}
}




bool MasterXHWND::openfile(char* cFile, char* type, char* name)
{
	char szFile[200];
	
	
	OPENFILENAME ofn;       // common dialog box structure
	
	strcpy(szFile, type);
	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "\0*.des\0ALL\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	
	if (GetOpenFileName(&ofn)==TRUE) 
	{
		strcpy(cFile, ofn.lpstrFile);
		return true;
	}
	else
	{
		return false;
	}
	
}




// initialisation des entrée
void MasterXHWND::initInput()
{
	DirectInputCreate(GetModuleHandle(NULL),DIRECTINPUT_VERSION,(struct IDirectInputA**)&pDI,NULL);

	pDI->CreateDevice(GUID_SysKeyboard,&pKeyboard,NULL);
	pKeyboard->SetDataFormat(&c_dfDIKeyboard);

	pKeyboard->SetCooperativeLevel(hwnd, 
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
	pKeyboard->Acquire();
}


// surfaces primaires
bool MasterXHWND::setprimarysurface()
{
	
	DDSURFACEDESC ddsd;
	//DDSCAPS ddsCaps;
	HRESULT ddrval;
	
	
	memset(&ddsd,0,sizeof(ddsd)); 
	ddsd.dwSize = sizeof(ddsd); 


	ddsd.dwFlags = DDSD_CAPS; //| DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE; //| DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	//ddsd.dwBackBufferCount = 1;
	
	ddrval = scr.lpDD->CreateSurface(&ddsd,&scr.lpFront,NULL);
	if(ddrval != DD_OK)
	{
        scr.lpDD->Release();
		return false;
	}
	
	ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwWidth = 1024;
		ddsd.dwHeight = 768;


	// create the back buffer
		ddrval = scr.lpDD->CreateSurface(&ddsd, &scr.lpBack , NULL);

		if( FAILED( ddrval ) )
			throw "Failed to create the back buffer";

	//ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	//ddrval = scr.lpFront->GetAttachedSurface(&ddscaps, &scr.lpBack);
	//ddrval = scr.lpDD->CreateSurface(&ddsd, lpBack, NULL);
	
	if(ddrval != DD_OK)
	{
		
		scr.lpFront->Release();
		scr.lpDD->Release();
		return false;
	}
	
	// create the clipper
		ddrval = scr.lpDD->CreateClipper(0, &scr.g_lpClipper , NULL);

		if( FAILED( ddrval ) )
			throw "Failed to create the clipper";

		ddrval = scr.g_lpClipper->SetHWnd(0, hwnd);

		if( FAILED( ddrval ) )
			throw "Failed to set the clipper's HWND";

		ddrval = scr.lpFront->SetClipper(scr.g_lpClipper);

		if( FAILED( ddrval ) )
			throw "Failed to set the primary surface's clippe";
	return true;
}

	




void MasterXHWND::processgfx()
{
	// clear the back buffer before drawing
	DDBLTFX ddbltfx;
	ZeroMemory(&ddbltfx, sizeof( ddbltfx ));

	ddbltfx.dwSize = sizeof( ddbltfx );
	ddbltfx.dwFillColor = RGB(0, 0, 0);

	scr.lpFront->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);

	// draw pixels to the back buffer -- notice this doesn't change
	// between exclusive mode and windowed mode!
	//DrawPixels(scr.lpBack);


		// windowed code

		// calculate the client rect in screen coordinates
		RECT rect;
		ZeroMemory(&rect, sizeof( rect ));

		// get the client area
		GetClientRect(hwnd, &rect);

		// copy the rect's data into two points
		POINT p1;
		POINT p2;

		p1.x = rect.left;
		p1.y = rect.top;
		p2.x = rect.right;
		p2.y = rect.bottom;

		// convert it to screen coordinates (like DirectDraw uses)
		ClientToScreen(hwnd, &p1);
		ClientToScreen(hwnd, &p2);

		// copy the two points' data back into the rect
		rect.left   = p1.x;
		rect.top    = p1.y;
		rect.right  = p2.x;
		rect.bottom = p2.y;

		// blit the back buffer to our window's position
		scr.lpFront->Blt(&rect, scr.lpBack, NULL, DDBLT_WAIT, NULL);
}




void MasterXHWND::DrawPixels(LPDIRECTDRAWSURFACE lpSurface)
{
	//HRESULT hr = 0;
	HRESULT ddrval;
	// Get the bit-depth of the surface, so we draw in different
	// bit-depths correctly
	
	DDPIXELFORMAT ddpf;
	ZeroMemory(&ddpf, sizeof( ddpf ));

	ddpf.dwSize = sizeof( ddpf );

	lpSurface->GetPixelFormat(&ddpf);

	// Get the surface's description and lock it.
	DDSURFACEDESC ddsd;
	ZeroMemory(&ddsd, sizeof( ddsd ));

	ddsd.dwSize = sizeof( ddsd );

	ddrval = lpSurface->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	{
		if( FAILED( ddrval ) )
		{
			lpSurface->Unlock(NULL);
			throw "DrawPixels: Failed to lock the specified surface.";
		}

		// Plot the pixels
		int x = 0;
		int y = 0;
		unsigned char nColor = 0;

		const int nBytesPerPixel = ddpf.dwRGBBitCount / 8;
		const int nPitch = (int) (ddsd.lPitch / nBytesPerPixel);
		unsigned char* pVideoMemory = (unsigned char*) ddsd.lpSurface;

		for( int i=0; i < 10000; i++ )
		{
			x = (rand() % (ddsd.dwWidth  * nBytesPerPixel));
			y = (rand() % (ddsd.dwHeight * nBytesPerPixel));

			nColor = (rand() % 256);

			pVideoMemory[x + y * nPitch] = nColor;
		}
	}
	ddrval = lpSurface->Unlock(NULL);

	if( FAILED( ddrval ) )
		throw "DrawPixels: Failed to unlock the specified surface";
}









// init du son
bool MasterXHWND::initSound()
{
	HRESULT h;
	
	h = DirectSoundCreate(NULL, &pDS, NULL);
	if (h != DS_OK)
		return FALSE;
	
	HWND hWnd = GetForegroundWindow();
	if (hWnd == NULL)
		hWnd = GetDesktopWindow();
	
	h = pDS->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if (h != DS_OK)
	{	pDS->Release();
	pDS = NULL;
	return FALSE;
	}
	
	IDirectSoundBuffer *lpDsb;
	DSBUFFERDESC		dsbdesc;
	
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat = NULL;
	
	if (pDS->CreateSoundBuffer(&dsbdesc, &lpDsb, NULL) != DS_OK)
	{	pDS->Release();
	pDS = NULL;
	return FALSE;
	}
	lpDsb->Play(0, 0, DSBPLAY_LOOPING);
	
	return TRUE;
}


// charger un bitmap 
bool MasterXHWND::LoadGraphic(MasterGraphic* g,char* filename)
{
	return g->LoadGraphic(filename,&scr);
}


// Charger un son
bool MasterXHWND::LoadSound(MasterSound* s,LPCSTR resourceID)
{
	return s->LoadSound(pDS,resourceID);
}

// état des entrée avec direct input
bool MasterXHWND::KeyCheck(int key)
{
	BYTE	state[256];
	pKeyboard->GetDeviceState(sizeof(state), &state);

	if(state[key] & 0x80)
	{
		return true;
	}
	else
	{
		return false;
	}
}


// trouver la position de la souris par WIN32 API
void MasterXHWND::GetMousePosWin(long& x, long& y)
{
	POINT point;
	GetCursorPos(&point);
	x = point.x;
	y = point.y;
}




// état du clavier par Windows
bool MasterXHWND::AsyncKeyCheck(int key)
{
  if(GetAsyncKeyState(key))
  {
	  return true;
  }
  else
  {
	  return false;
  }
}

// création de police FONT
HFONT MasterXHWND::CreateMasterFont(char* font_name,int size)
{
	return CreateFont(size,0,0,0,FW_DONTCARE,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,font_name);
}


// timer
void MasterXHWND::InitTimer(int id, int interval)
{
	SetTimer(hwnd,id,interval,NULL);
}

// end timer
void MasterXHWND::DestroyTimer(int id)
{
	KillTimer(hwnd,id);
}

// cacher le pointeur de souris
void MasterXHWND::HideCursor()
{
	ShowCursor(false);
}

// montrer le curseur
void MasterXHWND::RestoreCursor()
{
	ShowCursor(true);
}

// temps changé?
bool MasterXHWND::TimeChange()
{
	if(pre_time == timer_count)
	{
	return false;
	}
	else
	{
		pre_time = timer_count;
		return true;
	}
}


// changer le temps
void MasterXHWND::ChangeTime()
{
	timer_count++;

	if(timer_count > 100)
	{
		timer_count = 0;
	}
}

// automatic
void MasterXHWND::SetTimeFlag()
{
	time_flag = TimeChange();
}

// un-sync
bool MasterXHWND::SyncChange()
{
	return time_flag;
}


/***** OBJETS TEXTES *****************************************/

void MasterText::setdefaultfont()
{
	font = CreateFont(13,
        0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        "Arial");
}


//initialisation
void MasterText::init(MasterScreen* x)
{
	
	scr = (MasterScreen*)x;
	font = CreateFont(13,
        0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        "Arial"); 
	
	underfont = CreateFont(13,
        0, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        "Arial"); 
	
	bk = TRANSPARENT;
	text = RGB(255,0,0);
	
}

// police
void MasterText::setfont(HFONT f)
{
    font = f;
	
}

// set HF
void MasterText::setunderfont(HFONT f)
{
	underfont = f;
}

// couleur de font du text
void MasterText::setbkcolor(COLORREF f)
{

	bk = f;
}

// couleur de texte
void MasterText::settextcolor(COLORREF f)
{
	
	text = f;
	
}

// afficher texte
void FAST MasterText::printtext(char* strtext, int x, int y)
{
	
	HDC dc;
    scr->lpBack->GetDC(&dc);
	
	SetTextColor(dc,text);
	SetBkColor(dc,bk);
	
	if(font != NULL)
	{
		SelectObject(dc, font);
	}
	
	TextOut(dc,x,y,strtext,strlen(strtext));
	
	scr->lpBack->ReleaseDC(dc);
	
}


void FAST MasterText::sprinttext(int x, int y, char* str, ...)
{
	
	va_list list; 
	char* string;
	string = new char [ strlen(str) + 2];
	va_start(list, str); 
	vsprintf((char*)string,str,list);
	va_end(list); 
	printtext(string,x,y);
	delete [] string;
	string = 0;
	
}

// affiche à l'écran dans rect
void FAST MasterText::printtextrectf(int x, int y, int x2, int y2, char* str, ...)
{
	va_list list; 
	char* string;
	string = new char [ strlen(str) + 2];
	va_start(list, str); 
	vsprintf((char*)string,str,list);
	va_end(list); 
	printtextrect(string,x,y,x2,y2);
	delete [] string;
	string = 0;
}

// idem + format
void FAST MasterText::printtextrect(char* strtext, int x, int y, int w, int h)
{
	RECT rt;
	SetRect(&rt,x,y,w,h);
	
	HDC dc;
    scr->lpBack->GetDC(&dc);
	
	SetTextColor(dc,text);
	SetBkColor(dc,bk);
	
	if(font != NULL)
	{
		SelectObject(dc, font);
	}
	
	DrawText(dc,strtext,strlen(strtext),&rt,DT_EDITCONTROL);
	
	scr->lpBack->ReleaseDC(dc);
	
	
	
}

// texte souligné
void FAST MasterText::printtextunderline(char* strtext, int x, int y)
{
	HDC dc;
    scr->lpBack->GetDC(&dc);
	
	SetTextColor(dc,text);
	SetBkColor(dc,bk);
	
	if(underfont != NULL)
	{
		SelectObject(dc, underfont);
	}
	
	TextOut(dc,x,y,strtext,strlen(strtext));
	
	scr->lpBack->ReleaseDC(dc);
	
}


//************************************* OBJECT GRAPHIQUE***************************************/

MasterGraphic::~MasterGraphic()
{
	if(surf != NULL)
	{
		surf->Release();
		surf = NULL;
	}
}

MasterGraphic::MasterGraphic()
{
	ckey = false;
	surf = NULL;
	scr  = NULL;
}

void MasterGraphic::Release()
{
	if(surf)
	{
		surf->Release();
	}
}

//afficher une partie d'un bitmap
void MasterGraphic::DisplayGraphicEx(int bx, int by, int bw, int bh,int image_x, int image_y)
{
	HRESULT ddrval;
	RECT     rcRect;

	SetRect(&rcRect,bx,by,bw,bh);

	if(surf)
	{

	if(ckey)
	{
		ddrval= scr->lpBack->BltFast(image_x, image_y, surf,&rcRect, DDBLTFAST_SRCCOLORKEY);

		if(ddrval != DD_OK)
		{
			ReLoadGraphic();
		}
	}
	else
	{
		ddrval = scr->lpBack->BltFast(image_x,image_y,surf,&rcRect,DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
		if(ddrval != DD_OK)
		{
			ReLoadGraphic();
		}
	}
	}
}


// Bitmap au complet
void MasterGraphic::DisplayGraphic(int x, int y)
{
	HRESULT ddrval;
	RECT     rcRect;

	SetRect(&rcRect,0,0,w,h);

	if(surf)
	{

	if(ckey)
	{
		ddrval= scr->lpBack->BltFast(x, y, surf,&rcRect, DDBLTFAST_SRCCOLORKEY);

		if(ddrval != DD_OK)
		{
			ReLoadGraphic();
		}
	}
	else
	{

		ddrval = scr->lpBack->BltFast(x,y,surf,&rcRect,DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
		if(ddrval != DD_OK)
		{
			ReLoadGraphic();
		}
	}
	}
}
// screen struct
void MasterGraphic::init(MasterScreen* xscr)
{
	scr = xscr;
}

// chargement mas-gra
bool MasterGraphic::LoadGraphic(char* filenamex, MasterScreen* scrx)
{
	scr = scrx;
	strcpy(filename,filenamex);
	return LoadGraphic(filename);
}

// re-charge graphique
void MasterGraphic::ReLoadGraphic()
{
	DDReLoadBitmap(surf,filename);
}

// chargement graphique 
bool MasterGraphic::LoadGraphic(char* filenamex)
{
	strcpy(filename,filenamex);
	if(scr != NULL)
	{
		surf = DDLoadBitmap(scr->lpDD,filename);
		if(surf) 
		{
			w = MXWIDTH; 
	    	h = MXHEIGHT;

		return true;

		}
		else
		{
			return false;
		}
	} else
	{
		return false;
	}
}


// transparance par couleur-clée  (génial!)
void MasterGraphic::SetColorKey(COLORREF r)
{
	if(surf)
	{

	DDSetColorKey(surf,r);
	ckey = true;
	
	}
}


/***************************** PAINT ******************************************/

// init
void MasterPaint::init(MasterScreen* xscr)
{
	scr = xscr;
}

// pixel
void FAST MasterPaint::mxsetpixel(int x, int y, COLORREF rgbval)
{
	HDC dc;
	scr->lpBack->GetDC(&dc);
	SetPixel(dc, x, y, rgbval);  
	scr->lpBack->ReleaseDC(dc);
}

// line verticale
void FAST MasterPaint::mxverticalline(int y1,int y2,int x,COLORREF color)
{
	HDC dc;
	scr->lpBack->GetDC(&dc);
	for(int i = y1; i <=y2; i++)
	{

	SetPixel(dc, x, i, color);
	
	}
	scr->lpBack->ReleaseDC(dc);
}

// line horizontale
void FAST MasterPaint::mxhorizontalline(int x1, int x2, int y,COLORREF color)
{
	HDC dc;
	scr->lpBack->GetDC(&dc);
	for(int i = x2; i>=x1; i--)
	{

	SetPixel(dc, i, y, color);
	
	}
	scr->lpBack->ReleaseDC(dc);
}

//ligne entre deux points
void FAST MasterPaint::mxline(int start_x,int start_y,int stop_x, int stop_y,COLORREF color)
{
	int y_unit,x_unit;
	int ydiff = stop_y-start_y;
	if(ydiff<0)
	{
		ydiff = ydiff;
		y_unit=-1;
	}
	else
	{
		y_unit =1;
	}
	int xdiff=stop_x-start_x;
	if(xdiff<0)
	{
		xdiff=-xdiff;
		x_unit = -1;
	}
	else
	{
		x_unit = 1;
	}


	HDC dc;
	scr->lpBack->GetDC(&dc);


	int error_term=0;
	if(xdiff>ydiff)
	{
		int length=xdiff+1;
		for(int i = 0; i <length; i++)
		{
			SetPixel(dc,start_x,start_y,color);
			start_x += x_unit;
			error_term+=ydiff;
			if(error_term>xdiff)
			{
				error_term-=xdiff;
				start_y+=y_unit;
			}
		}
	}
	else
	{
		int length = ydiff+1;
		for(int i = 0; i < length; i++)
		{
			SetPixel(dc,start_x,start_y,color);
			start_y += y_unit;
			error_term+=xdiff;
			if(error_term>0)
			{
				error_term-=ydiff;
				start_x += x_unit;
			}

		}
	}

	scr->lpBack->ReleaseDC(dc);
}


// obtenir la couleur d'un pixel
COLORREF FAST MasterPaint::mxgetpixel(int x, int y)
{
	COLORREF rgbval;
	HDC dc;
	scr->lpBack->GetDC(&dc);
	rgbval = GetPixel(dc, x, y);     // sauvegarde
	scr->lpBack->ReleaseDC(dc);
	return rgbval;
}


// arc-de-cercle
void FAST MasterPaint::mxarc(int x1, int x2, int x3, int x4,int x5, int x6,int x7, int x8,COLORREF fill,COLORREF outline)
{
	
	HDC dc;
	scr->lpBack->GetDC(&dc);
	
	   HBRUSH hNewBrush,hOldBrush;
	   HPEN hNewPen,hOldPen; 
	   
	   hNewBrush = (HBRUSH)CreateSolidBrush(fill);
	   hOldBrush = (HBRUSH)SelectObject(dc,(HBRUSH)hNewBrush); 
	   hNewPen = CreatePen(PS_SOLID,2,outline); 
	   hOldPen= (HPEN__*)SelectObject(dc,(HPEN__*)hNewPen); 
	   Arc(dc,x1,x2,x3,x4,x5,x6,x7,x8);
	   
	   SelectObject(dc,hOldPen);
	   SelectObject(dc,hOldBrush);
	   
	   DeleteObject(hNewBrush); 
	   DeleteObject(hNewPen); 
	   scr->lpBack->ReleaseDC(dc);
}

// ligne
void FAST MasterPaint::mxlineto(int x, int y,COLORREF fill,COLORREF outline)
{
	
	HDC dc;
	scr->lpBack->GetDC(&dc);
	
    HBRUSH hNewBrush,hOldBrush;
    HPEN hNewPen,hOldPen; 
	   
    hNewBrush = (HBRUSH)CreateSolidBrush(fill);
    hOldBrush = (HBRUSH)SelectObject(dc,(HBRUSH)hNewBrush); 
    hNewPen = CreatePen(PS_SOLID,2,outline); 
    hOldPen= (HPEN__*)SelectObject(dc,(HPEN__*)hNewPen);
	LineTo(dc,x,y);
	   
	SelectObject(dc,hOldPen);
	SelectObject(dc,hOldBrush);
	  
	DeleteObject(hNewBrush); 
	DeleteObject(hNewPen); 
	scr->lpBack->ReleaseDC(dc);
}

// angle
void FAST MasterPaint::mxanglearc(int x, int y, long radius, float startangle,float sweepangle,COLORREF outline)
{
	HDC dc;
	scr->lpBack->GetDC(&dc);
	
    HBRUSH hNewBrush,hOldBrush;
    HPEN hNewPen,hOldPen; 
	COLORREF fill = RGB(0,0,0);
    hNewBrush = (HBRUSH)CreateSolidBrush(fill);
    hOldBrush = (HBRUSH)SelectObject(dc,(HBRUSH)hNewBrush); 
    hNewPen = CreatePen(PS_SOLID,2,outline); 
    hOldPen= (HPEN__*)SelectObject(dc,(HPEN__*)hNewPen); 
	   
	AngleArc(dc,x,y,radius,startangle,sweepangle);
	   
	SelectObject(dc,hOldPen);
	SelectObject(dc,hOldBrush);
	  
	DeleteObject(hNewBrush); 
	DeleteObject(hNewPen); 
	scr->lpBack->ReleaseDC(dc);
}

// polygone
void FAST MasterPaint::mxpolygon(CONST POINT* point,int n_points,COLORREF fill,COLORREF outline)
{
	HDC dc;
	scr->lpBack->GetDC(&dc);
	
	   HBRUSH hNewBrush,hOldBrush;
	   HPEN hNewPen,hOldPen; 
	   
	   hNewBrush = (HBRUSH)CreateSolidBrush(fill);
	   hOldBrush = (HBRUSH)SelectObject(dc,(HBRUSH)hNewBrush); 
	   hNewPen = CreatePen(PS_SOLID,2,outline);
	   hOldPen= (HPEN__*)SelectObject(dc,(HPEN__*)hNewPen); 
	   Polygon(dc,point,n_points);
	   
	   SelectObject(dc,hOldPen);
	   SelectObject(dc,hOldBrush);
	   
	   DeleteObject(hNewBrush); 
	   DeleteObject(hNewPen); 
	   scr->lpBack->ReleaseDC(dc);
}


// Rectangle
void FAST MasterPaint::mxdrawrect(int x, int y, int xx, int yy, COLORREF fill, COLORREF outline)
{
	HDC dc;
	scr->lpBack->GetDC(&dc);
	
	   HBRUSH hNewBrush,hOldBrush;
	   HPEN hNewPen,hOldPen; 
	   
	   hNewBrush = (HBRUSH)CreateSolidBrush(fill);
	   hOldBrush = (HBRUSH)SelectObject(dc,(HBRUSH)hNewBrush); 
	   hNewPen = CreatePen(PS_SOLID,2,outline); 
	   hOldPen= (HPEN__*)SelectObject(dc,(HPEN__*)hNewPen); 
	   Rectangle(dc,x,y,xx,yy);
	   
	   SelectObject(dc,hOldPen);
	   SelectObject(dc,hOldBrush);
	   
	   DeleteObject(hNewBrush); 
	   DeleteObject(hNewPen); 
	   scr->lpBack->ReleaseDC(dc);
}

// Rectangle arrondis
void FAST MasterPaint::mxdrawroundrect(int x, int y, int x2, int y2, int cw, int ch, COLORREF fill,COLORREF outline)
{
	HDC dc;
	scr->lpBack->GetDC(&dc);
	
	HBRUSH hNewBrush,hOldBrush;
	   HPEN hNewPen,hOldPen; 
	   hNewBrush = (HBRUSH)CreateSolidBrush(fill);
	   hOldBrush = (HBRUSH)SelectObject(dc,(HBRUSH)hNewBrush);
	   hNewPen = CreatePen(PS_SOLID,2,outline); 
	   hOldPen= (HPEN__*)SelectObject(dc,(HPEN__*)hNewPen);
	   
	   
	   RoundRect(dc,x,y,x2,y2,cw,ch);
	   
	   
	   SelectObject(dc,hOldPen);
	   SelectObject(dc,hOldBrush);
	   
	   DeleteObject(hNewBrush);
	   DeleteObject(hNewPen); 
	   scr->lpBack->ReleaseDC(dc);
}

// Cercle ou portion
void FAST MasterPaint::mxdrawpie(int x, int y, int x2, int y2, int nx, int ny, int nx2, int ny2, COLORREF fill,COLORREF outline)
{
	HDC dc;
	scr->lpBack->GetDC(&dc);
	
	HBRUSH hNewBrush,hOldBrush;
	   HPEN hNewPen,hOldPen;
	   
	   hNewBrush = (HBRUSH)CreateSolidBrush(fill); 
	   hOldBrush = (HBRUSH)SelectObject(dc,(HBRUSH)hNewBrush); 
	   hNewPen = CreatePen(PS_SOLID,2,outline);
	   hOldPen= (HPEN__*)SelectObject(dc,(HPEN__*)hNewPen);
	   
	   Pie(dc,x,y,x2,y2,nx,ny,nx2,ny2);
	   
	   SelectObject(dc,hOldPen);
	   SelectObject(dc,hOldBrush);
	   
	   DeleteObject(hNewBrush); 
	   DeleteObject(hNewPen); 
	   scr->lpBack->ReleaseDC(dc);
}

//chord
void FAST MasterPaint::mxdrawchord(int x, int y, int x2, int y2, int nx, int ny, int nx2, int ny2,COLORREF fill,COLORREF outline)
{
	   HDC dc;
	   scr->lpBack->GetDC(&dc);
	   
       HBRUSH hNewBrush,hOldBrush;
	   HPEN hNewPen,hOldPen;
	   hNewBrush = (HBRUSH)CreateSolidBrush(fill); 
	   hOldBrush = (HBRUSH)SelectObject(dc,(HBRUSH)hNewBrush); 
	   
	   hNewPen = CreatePen(PS_SOLID,2,outline);
	   hOldPen= (HPEN__*)SelectObject(dc,(HPEN__*)hNewPen); 
	   Chord(dc,x,y,x2,y2,nx,ny,nx2,ny2);
	   
	   SelectObject(dc,hOldPen);
	   SelectObject(dc,hOldBrush);
	   
	   DeleteObject(hNewBrush); 
	   DeleteObject(hNewPen);
	   scr->lpBack->ReleaseDC(dc);
}
// Ellipse
void FAST MasterPaint::mxdrawellipse(int x, int y, int x2, int y2,COLORREF fill,COLORREF outline)
{
	   HDC dc;
	   scr->lpBack->GetDC(&dc);
	   
       HBRUSH hNewBrush,hOldBrush;
	   HPEN hNewPen,hOldPen; 
	   hNewBrush = (HBRUSH)CreateSolidBrush(fill); 
	   hOldBrush = (HBRUSH)SelectObject(dc,(HBRUSH)hNewBrush);
	   hNewPen = CreatePen(PS_SOLID,2,outline); 
	   hOldPen= (HPEN__*)SelectObject(dc,(HPEN__*)hNewPen); 
	   Ellipse(dc,x,y,x2,y2);
	   
	   
	   
	   SelectObject(dc,hOldPen);
	   SelectObject(dc,hOldBrush);
	   
	   DeleteObject(hNewBrush); 
	   DeleteObject(hNewPen); 
	   scr->lpBack->ReleaseDC(dc);
}

//*********************** OBJECT SON ***********************************************/

MasterSound::MasterSound()
{
	pSound = NULL;
}

MasterSound::~MasterSound()
{
	if(sound_ok)
	{

	if(pSound)
	{
		pSound->Release();
	}

	}
}

// Chargemenr de Son
bool MasterSound::LoadSound(IDirectSound *pds,LPCSTR name)
{
	if(sound_ok)
	{


	pSound = DSLoadSoundBuffer(pds,name);

	if(pSound)
	{
		return true;
	}
	else
	{
		return false;
	}

	}

	return false;
}

// Jouer le son
void MasterSound::Play()
{
	if(sound_ok)
	{


	if(pSound)
	{
		pSound->Play(0,0,0);
	}

	}
}

// status
bool MasterSound::PlayStatus()
{
	if(sound_ok) {
   DWORD status;
   pSound->GetStatus(&status);

	  if(status & DSBSTATUS_PLAYING)
	  { 
		  return true;
	  } 
	  else 
	  {
		  return false;
	  }
		
	  return false;

	}

	else {


		return false;
	}
}

void MasterSound::Release()
{
	if(pSound)
	{
		pSound->Release();
	}
}


//************** FONCTION _N-Class **************************************************/

// surface bmp DD
IDirectDrawSurface* DDLoadBitmap(IDirectDraw* pdd, LPCSTR szBitmap)
{
	 
	HBITMAP hbm;
	BITMAP bm;
	IDirectDrawSurface *pdds;
	
	
	hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	
	
	if(hbm == NULL) { return NULL; }
	
	GetObject(hbm, sizeof(bm), &bm); 

	
	int cw = bm.bmWidth;
	int ch = bm.bmHeight;

	MXWIDTH = cw;
	MXHEIGHT = ch;

	
	pdds = CreateOffScreenSurface(pdd, bm.bmWidth,bm.bmHeight);
	
	if(pdds)
	{
		
		DDCopyBitmap(pdds, hbm, bm.bmWidth,bm.bmHeight);
	}
	
	
	DeleteObject(hbm);
	
	return pdds;
	
}

mxbitmap DDLoadBitmapX(IDirectDraw* pdd, LPCSTR szBitmap)
{
	
	HBITMAP hbm;
	BITMAP bm;
	IDirectDrawSurface *pdds;
	

	
	hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	mxbitmap xbm;

	
	if(hbm == NULL) { xbm.surf = NULL; return xbm; }
	
	GetObject(hbm, sizeof(bm), &bm); 
	
	
	
	int cw = bm.bmWidth;
	int ch = bm.bmHeight;
	
	pdds = CreateOffScreenSurface(pdd, bm.bmWidth,bm.bmHeight);
	
	if(pdds)
	{
		
		DDCopyBitmap(pdds, hbm, bm.bmWidth,bm.bmHeight);
	}
	
	
	DeleteObject(hbm);
	
	xbm.surf = pdds;
	xbm.w = cw;
	xbm.h = ch;

	return xbm;
	
}

//clipping
IDirectDrawSurface* CreateOffScreenSurface(IDirectDraw* pdd, int dx, int dy)
{
	
	DDSURFACEDESC ddsd;
	IDirectDrawSurface* pdds;

	
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = dx;
	ddsd.dwHeight = dy;
	
	if(pdd->CreateSurface(&ddsd, &pdds,NULL) != DD_OK)
	{
		return NULL;
		
	} else {
		
		return pdds;
		
	}
	
}

// Copie BMP
HRESULT DDCopyBitmap(IDirectDrawSurface* pdds, HBITMAP hbm, int dx, int dy)
{
	
	HDC hdcImage;
	HDC hdc;
	HRESULT hr;
	HBITMAP hbmOld;
	

	
	hdcImage = CreateCompatibleDC(NULL);
	hbmOld = (HBITMAP)SelectObject(hdcImage, hbm);
	
	if((hr = pdds->GetDC(&hdc)) == DD_OK)
	{
		
		BitBlt(hdc,0,0,dx,dy,hdcImage,0,0,SRCCOPY);
		pdds->ReleaseDC(hdc);
	}
	
	SelectObject(hdcImage, hbmOld);
	DeleteDC(hdcImage);
	
	return hr;
	
}

// Son .Wav
static const TCHAR c_szWAV[] = {"WAVE"};

///////////////////////////////////////////////////////////////////////////////
//
// DSLoadSoundBuffer
//
///////////////////////////////////////////////////////////////////////////////

IDirectSoundBuffer *DSLoadSoundBuffer(IDirectSound *pDS, LPCTSTR lpName)
{
	IDirectSoundBuffer *pDSB = NULL;
	DSBUFFERDESC dsBD = {0};
	BYTE *pbWaveData;
	
	if (DSGetWaveResource(NULL, lpName, &dsBD.lpwfxFormat, &pbWaveData, &dsBD.dwBufferBytes))
	{
		dsBD.dwSize = sizeof(dsBD);
		dsBD.dwFlags = DSBCAPS_STATIC | DSBCAPS_GETCURRENTPOSITION2;
		
		if (SUCCEEDED(pDS->CreateSoundBuffer(&dsBD, &pDSB, NULL)))
		{
			if (!DSFillSoundBuffer(pDSB, pbWaveData, dsBD.dwBufferBytes))
			{
				pDSB->Release();
				pDSB = NULL;
			}
		}
		else
		{
			pDSB = NULL;
		}
	}
	
	return pDSB;
}

///////////////////////////////////////////////////////////////////////////////
//
// DSReloadSoundBuffer
//
///////////////////////////////////////////////////////////////////////////////

BOOL DSReloadSoundBuffer(IDirectSoundBuffer *pDSB, LPCTSTR lpName)
{
	BOOL result=FALSE;
	BYTE *pbWaveData;
	DWORD cbWaveSize;
	
	if (DSGetWaveResource(NULL, lpName, NULL, &pbWaveData, &cbWaveSize))
	{
		if (SUCCEEDED(pDSB->Restore()) &&
			DSFillSoundBuffer(pDSB, pbWaveData, cbWaveSize))
		{
			result = TRUE;
		}
	}
	
	return result;
}

///////////////////////////////////////////////////////////////////////////////
//
// DSGetWaveResource
//
///////////////////////////////////////////////////////////////////////////////

BOOL DSGetWaveResource(HMODULE hModule, LPCTSTR lpName,
					   WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pcbWaveSize)
{
	HRSRC hResInfo;
	HGLOBAL hResData;
	void *pvRes;
	
	if (((hResInfo = FindResource(hModule, lpName, c_szWAV)) != NULL) &&
		((hResData = LoadResource(hModule, hResInfo)) != NULL) &&
		((pvRes = LockResource(hResData)) != NULL) &&
		DSParseWaveResource(pvRes, ppWaveHeader, ppbWaveData, pcbWaveSize))
	{
		return TRUE;
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL DSFillSoundBuffer(IDirectSoundBuffer *pDSB, BYTE *pbWaveData, DWORD cbWaveSize)
{
	if (pDSB && pbWaveData && cbWaveSize)
	{
		LPVOID pMem1, pMem2;
		DWORD dwSize1, dwSize2;
		
		if (SUCCEEDED(pDSB->Lock(0, cbWaveSize, &pMem1, &dwSize1, &pMem2, &dwSize2, 0)))
		{
			CopyMemory(pMem1, pbWaveData, dwSize1);
			
			if ( 0 != dwSize2 )
				CopyMemory(pMem2, pbWaveData+dwSize1, dwSize2);
			
			pDSB->Unlock(pMem1, dwSize1, pMem2, dwSize2);
			return TRUE;
		}
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL DSParseWaveResource(void *pvRes, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData,DWORD *pcbWaveSize)
{
	DWORD *pdw;
	DWORD *pdwEnd;
	DWORD dwRiff;
	DWORD dwType;
	DWORD dwLength;
	
	if (ppWaveHeader)
		*ppWaveHeader = NULL;
	
	if (ppbWaveData)
		*ppbWaveData = NULL;
	
	if (pcbWaveSize)
		*pcbWaveSize = 0;
	
	pdw = (DWORD *)pvRes;
	dwRiff = *pdw++;
	dwLength = *pdw++;
	dwType = *pdw++;
	
	if (dwRiff != mmioFOURCC('R', 'I', 'F', 'F')) // formats inconnus
		goto exit;	
	
	if (dwType != mmioFOURCC('W', 'A', 'V', 'E'))
		goto exit;	
	
	pdwEnd = (DWORD *)((BYTE *)pdw + dwLength-4);
	
	while (pdw < pdwEnd)
	{
		dwType = *pdw++;
		dwLength = *pdw++;
		
		switch (dwType)
		{
		case mmioFOURCC('f', 'm', 't', ' '):
			if (ppWaveHeader && !*ppWaveHeader)
			{
				if (dwLength < sizeof(WAVEFORMAT))
					goto exit;	
				
				*ppWaveHeader = (WAVEFORMATEX *)pdw;
				
				if ((!ppbWaveData || *ppbWaveData) &&
					(!pcbWaveSize || *pcbWaveSize))
				{
					return TRUE;
				}
			}
			break;
			
		case mmioFOURCC('d', 'a', 't', 'a'):
			if ((ppbWaveData && !*ppbWaveData) ||
				(pcbWaveSize && !*pcbWaveSize))
			{
				if (ppbWaveData)
					*ppbWaveData = (LPBYTE)pdw;
				
				if (pcbWaveSize)
					*pcbWaveSize = dwLength;
				
				if (!ppWaveHeader || *ppWaveHeader)
					return TRUE;
			}
			break;
		}
		
		pdw = (DWORD *)((BYTE *)pdw + ((dwLength+1)&~1));
	}
	
exit:
	return FALSE;
}

HRESULT
DDSetColorKey(IDirectDrawSurface * pdds, COLORREF rgb)
{
    DDCOLORKEY              ddck;
	
    ddck.dwColorSpaceLowValue = DDColorMatch(pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
}


DWORD
DDColorMatch(IDirectDrawSurface * pdds, COLORREF rgb)
{
    COLORREF                rgbT;
    HDC                     hdc;
    DWORD                   dw = CLR_INVALID;
    DDSURFACEDESC          ddsd;
    HRESULT                 hres;
	
    //
    //  color matching
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);     
        SetPixel(hdc, 0, 0, rgb);     
        pdds->ReleaseDC(hdc);
    }
    //
    //  new-color 
    //
    ddsd.dwSize = sizeof(ddsd);
    while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING)
        ;
    if (hres == DD_OK)
    {
        dw = *(DWORD *) ddsd.lpSurface;                 // Get DWORD
        if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
            dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount) - 1;  // Mask
        pdds->Unlock(NULL);
    }
    //
    //  old-color
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        SetPixel(hdc, 0, 0, rgbT);
        pdds->ReleaseDC(hdc);
    }
    return dw;
}

HRESULT
DDReLoadBitmap(IDirectDrawSurface * pdds, LPCSTR szBitmap)
{
    HBITMAP                 hbm;
    HRESULT                 hr;

    //
    //  loading
    //
    hbm = (HBITMAP) LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, 0,
                              0, LR_CREATEDIBSECTION);
    if (hbm == NULL)
        hbm = (HBITMAP) LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0,
                                  LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (hbm == NULL)
    {
        OutputDebugString("handle is null\n");
        return E_FAIL;
    }

	hr = DDCopyBitmap2(pdds, hbm, 0, 0, 0, 0);

   
	if (hr != DD_OK)
    {
        OutputDebugString("ddcopybitmap failed\n");
    }
    DeleteObject(hbm);
    return hr;
}

HRESULT
DDCopyBitmap2(IDirectDrawSurface * pdds, HBITMAP hbm, int x, int y,
             int dx, int dy)
{
    HDC                     hdcImage;
    HDC                     hdc;
    BITMAP                  bm;
    DDSURFACEDESC          ddsd;
    HRESULT                 hr;

    if (hbm == NULL || pdds == NULL)
        return E_FAIL;
    //
    // surface re-new
    //
    pdds->Restore();
    //
    // --> MEM
    //
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage)
        OutputDebugString("createcompatible dc failed\n");
    SelectObject(hdcImage, hbm);
    //
    // format
    //
    GetObject(hbm, sizeof(bm), &bm);
    dx = dx == 0 ? bm.bmWidth : dx;     
    dy = dy == 0 ? bm.bmHeight : dy;
    //
    // suface (hxv)
    //
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    pdds->GetSurfaceDesc(&ddsd);

    if ((hr = pdds->GetDC(&hdc)) == DD_OK)
    {
        StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y,
                   dx, dy, SRCCOPY);
        pdds->ReleaseDC(hdc);
    }
    DeleteDC(hdcImage);
    return hr;
}

// ****** misc

HFONT MakeFont(char* ftype, int size)
{
	return CreateFont(size,0,0,0,FW_DONTCARE,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,ftype);
}



// ENTRY
int APIENTRY WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR l,int CmdShow)
{
	 
	return MasterMain(hInst,l);
}

