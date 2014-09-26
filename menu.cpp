#include "thehead.h"

long momu_x,momu_y;
int appear;

long mouse_x,mouse_y;


extern bool pad_on;
extern bool music_on;

bool menu_d,menu_g;

extern char linea[100];
extern char * line2;


extern bool mnu_ed;


void menu::load()
{

mxhwnd.LoadGraphic(&gui,"Bmps\\menu.bmp");

gui.SetColorKey(RGB(0,255,0));

}




void menu::update()
{

	
	//mxhwnd.paint.mxanglearc(500,400,100,-23.1f,2.6f,RGB(155,25,125));



mxhwnd.paint.mxdrawrect(0,0,275,25,RGB(160,168,160),RGB(160,168,160)); // mask console	
	
mxhwnd.GetMousePosWin(momu_x,momu_y);

if(momu_x>950)
menuA();

}




void menu::menuA()
{
	gui.DisplayGraphicEx(0,0,57,635,1024-57,30);
	strcpy(linea,"");

	if(momu_x>968 && momu_y >= 50 && momu_y <=103)
	{
		strcpy(linea,"Fichiers");
		gui.DisplayGraphicEx(62,21,114,73,1024-52,51);
	
	
	}
	
	
	if(momu_x>968 && momu_y >= 104 && momu_y <=156)
	{
		gui.DisplayGraphicEx(62,21+53,114,73+53,1024-52,51+53);
		strcpy(linea,"Log Pad");
		if(menu_g==true)
		{
			pad_on=true;
			
		}	
	}
		
	if(momu_x>968 && momu_y >= 157 && momu_y <=209)
	{
		gui.DisplayGraphicEx(62,21+53+53,114,73+53+53,1024-52,51+53+53);

	
	}
	
	if(momu_x>968 && momu_y >= 210 && momu_y <=209+53)
	{
		gui.DisplayGraphicEx(62,21+53+53+53,114,73+53+53+53,1024-52,51+53+53+53);
		strcpy(linea,"Éditeur d'environnement");
		if(menu_g==true)
		{
			if(mnu_ed==false)
			mnu_ed=true;
			//else
			//mnu_ed=false;
		}	
	}
	
	if(momu_x>968 && momu_y >= 209+54 && momu_y <=209+53+53)
	{
		gui.DisplayGraphicEx(62,21+53+53+53+53,114,73+53+53+53+53,1024-52,51+53+53+53+53);
		strcpy(linea,"Éditeur de tonalités");
		if(menu_g==true)
		{
			music_on=true;
			
		}	
	
	}

	
	if(momu_x>968 && momu_y >= 209+53+54 && momu_y <=209+53+53+54)
	{
		gui.DisplayGraphicEx(62,21+53+53+53+53+53,114,73+53+53+53+53+53,1024-52,51+53+53+53+53+53);
	
	
	}



}







