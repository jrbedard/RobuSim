#include "thehead.h"


int photo;
bool my_photo;

int pos_x ;
int pos_y;
int angles;

int dis_g;
int dis_d;

int wheels;

bool bmp_avant;
bool bmp_right;
bool bmp_left;
bool bmp_arr;

bool bmp_avant_b;
bool bmp_right_b;
bool bmp_left_b;
bool bmp_arr_b;

bool bmp_avant_e;
bool bmp_right_e;
bool bmp_left_e;
bool bmp_arr_e;

float lenght;
float lenght_b;
float lenght_e;


bool stop;
bool reculon;

long cap_x,cap_y;
int active=0;

bool cap_g;
bool cap_d;


float sim_angle;

extern char cap_line1[100];
extern char cap_line2[100];


void captor::load()
{

mxhwnd.LoadGraphic(&sim,"Bmps\\360.bmp");
mxhwnd.LoadGraphic(&jrb,"Bmps\\jrb.bmp");
mxhwnd.LoadGraphic(&robusim,"Bmps\\robusim.bmp");
mxhwnd.LoadGraphic(&tab,"Bmps\\tab.bmp");

mxhwnd.LoadGraphic(&electro,"Bmps\\electro.bmp");


electro.SetColorKey(RGB(0,255,0));
tab.SetColorKey(RGB(0,255,0));

}





void captor::update()
{
mxhwnd.GetMousePosWin(cap_x,cap_y);







panel();


choice();
	
position();

grid();



}


void captor::draw(){}



void captor::choice()
{

if(cap_x >= 5 && cap_y >=330  && cap_x <= 56 && cap_y <= 350  && cap_g==true)
active=0;

if(cap_x >= 57 && cap_y >=330  && cap_x <= 111 && cap_y <= 350  && cap_g==true)
active=1;

if(cap_x >= 113 && cap_y >=330  && cap_x <= 139 && cap_y <= 350  && cap_g==true)
active=2;

if(cap_x >= 140 && cap_y >=330  && cap_x <= 174 && cap_y <= 350  && cap_g==true)
active=3;

if(cap_x >= 175 && cap_y >=330  && cap_x <= 225 && cap_y <= 350  && cap_g==true)
active=4;

if(cap_x >= 226 && cap_y >=330  && cap_x <= 274 && cap_y <= 350  && cap_g==true)
active=5;



	switch(active)
	{
	case 0:
		tab.DisplayGraphicEx(0,0,270,21,5,330);
		tab.DisplayGraphicEx(0,21,53,42,5,330);
		bumper();
		digital();
		break;

	
	case 1:
		tab.DisplayGraphicEx(0,0,270,21,5,330);
		tab.DisplayGraphicEx(52,21,109,42,57,330);
		usound();
		break;
	
	
	case 2:
		tab.DisplayGraphicEx(0,0,270,21,5,330);
		tab.DisplayGraphicEx(108,21,136,42,113,330);
		ir();
		break;

	case 3:
		tab.DisplayGraphicEx(0,0,270,21,5,330);
		tab.DisplayGraphicEx(135,21,171,42,140,330);
		pyro();
		break;

	case 4:
		tab.DisplayGraphicEx(0,0,270,21,5,330);
		tab.DisplayGraphicEx(170,21,222,42,175,330);
		break;

	case 5:
		tab.DisplayGraphicEx(0,0,270,21,5,330);
		tab.DisplayGraphicEx(221,21,272,42,224,330);
		gpd();
		break;
	}

}










// affichage des instruments / panneau latéral gauche / bumpers
void captor::panel()
{
	mxhwnd.paint.mxdrawrect(5,0,275,320,RGB(0,0,0),RGB(210,193,189));
	mxhwnd.paint.mxdrawrect(5,350,275,639,RGB(40,80,90),RGB(210,193,189));

	
	//mask->grid
	mxhwnd.paint.mxdrawrect(145,515,276,639,RGB(175,175,175),RGB(175,175,175));
	
	
	//CONSOLE & i/O
	//mxhwnd.paint.mxdrawrect(5,319,70,335,RGB(230,230,230),RGB(210,193,189));
	//mxhwnd.paint.mxdrawrect(240,335,275,350,RGB(230,230,230),RGB(210,193,189));
}




void captor::bumper()
{

	mxhwnd.text.setfont(d_font);
	mxhwnd.text.settextcolor(RGB(23,23,21));
	mxhwnd.text.setbkcolor(RGB(230,230,230));
	//mxhwnd.text.printtext("Console",8,320);
	//mxhwnd.text.printtext("I/O",240,335);
	

	mxhwnd.text.setfont(digital_font);
	mxhwnd.text.settextcolor(RGB(234,233,214));
	mxhwnd.text.setbkcolor(RGB(40,80,90));
	
	
	mxhwnd.text.printtext("BMP avant",10,360);
	mxhwnd.text.printtext("BMP arrière",10,380);
	mxhwnd.text.printtext("BMP droit",10,400);
	mxhwnd.text.printtext("BMP gauche",10,420);
	
	//mxhwnd.text.printtext("Digital 11",10,450);
	//mxhwnd.text.printtext("SS/PD5",10,470);
	//mxhwnd.text.printtext("MISO/PD2",10,490);
	
		mxhwnd.text.sprinttext(230,360,"0");
	mxhwnd.text.sprinttext(230,381,"0");
}



void captor::ir()
{

electro.DisplayGraphicEx(0,0,78,66,26,540);
	mxhwnd.paint.mxdrawrect(111,400,217,417,RGB(149,1,25),RGB(0,0,0));
	mxhwnd.paint.mxdrawrect(111,420,217,437,RGB(149,1,25),RGB(0,0,0));


}


void captor::usound()
{
electro.DisplayGraphicEx(79,0,146,66,26,540);

}

void captor::pyro()
{

electro.DisplayGraphicEx(149,0,233,66,26,540);

}

void captor::gpd()
{

	electro.DisplayGraphicEx(235,0,310,66,26,540);
}


// Panneau des positions/motor/angle horizontal dans le bas de l'éCran
void captor::position()
{

		mxhwnd.paint.mxdrawrect(290,705-20,1010,760-20,RGB(13,42,135),RGB(210,193,189));

	robusim.DisplayGraphic(700,690);
	
	mxhwnd.text.setfont(d_font);
	mxhwnd.text.settextcolor(RGB(255,255,255));
	mxhwnd.text.setbkcolor(RGB(13,42,135));
  
  mxhwnd.text.sprinttext(300,713-20,"X coord: ");
  mxhwnd.text.sprinttext(300,733-20,"Y coord: ");		


 

  mxhwnd.text.settextcolor(RGB(255,0,0));
  mxhwnd.text.sprinttext(370,713-20,"%d",pos_x-296);
  mxhwnd.text.sprinttext(370,733-20,"%d",pos_y-16);
  
  
  
  mxhwnd.text.sprinttext(610,713-20,"%d",dis_g);
  mxhwnd.text.sprinttext(610,733-20,"%d",dis_d);
  
  mxhwnd.text.sprinttext(440,733-20,"%d",angles);
  mxhwnd.text.settextcolor(RGB(255,255,255));
  
  
  mxhwnd.text.sprinttext(425,713-20,"Angle:");		
  //mxhwnd.paint.mxdrawpie(480,710,520,750,100,100,100,100,RGB(253,153,161),RGB(253,153,161));

 
  mxhwnd.text.sprinttext(500,713-20,"Motor left: ");
  mxhwnd.text.sprinttext(500,733-20,"Motor right: ");	

  mxhwnd.text.sprinttext(680,713-20,"Freq.: ");
  mxhwnd.text.sprinttext(680,733-20,"Battery level: ");	
}








// mini-simulation en temps réel / orientation des roues / bumpers
void captor::grid()
{
	sim_angle += 0.589;
	
	mxhwnd.paint.mxdrawrect(140,510,275,639,RGB(0,0,0),RGB(210,193,189));


	// vers droite
	if(sim_angle >= 0.0 && sim_angle < 0.589 || sim_angle >= 9.42477)	
	sim.DisplayGraphicEx(428,0,536,108,150,521);
	
	if(sim_angle >= 0.589 && sim_angle < 1.1780)	
	sim.DisplayGraphicEx(428+108,0,536+110,108,150,521);
	
	// bas-droite
	if(sim_angle >= 1.1780 && sim_angle < 1.767)	
	sim.DisplayGraphicEx(428+218,0,536+219,108,150,521);
	
	
	if(sim_angle >= 1.767 && sim_angle < 2.35619)	
	sim.DisplayGraphicEx(428+216+112,0,536+216+116,108,150,521);
	

	//bas
	if(sim_angle >= 2.35619 && sim_angle < 2.9451)	
	sim.DisplayGraphicEx(428+216+108+114,0,536+216+108+122,108,150,521);
	
	
	if(sim_angle >= 2.9453 && sim_angle < 3.534)	
	sim.DisplayGraphicEx(0,116,108,227,150,521);
	
	
	//bas_gauche
	if(sim_angle >= 3.534 && sim_angle < 4.123)
	sim.DisplayGraphicEx(108,116,216,227,150,521);
	
	if(sim_angle >= 4.123 && sim_angle < 4.71238)
	sim.DisplayGraphicEx(216,116,216+108,227,150,521);	
	
	//gauche
	if(sim_angle >= 4.71238 && sim_angle < 5.30138)
	sim.DisplayGraphicEx(216+110,116,216+216,227,150,521);
	

	if(sim_angle >= 5.30138 && sim_angle < 5.8903)
	sim.DisplayGraphicEx(216+110+108,116,216+216+108,227,150,521);

	//haut gauche
	if(sim_angle >= 5.8903 && sim_angle < 6.4799)
	sim.DisplayGraphicEx(216+110+216,116,216+216+216,227,150,521);
	
	if(sim_angle >= 6.4799 && sim_angle < 7.06858)
	sim.DisplayGraphicEx(432+110+108,116,216+216+216+108,227,150,521);
	
	//haut
	if(sim_angle >= 7.06858 && sim_angle < 7.65756)
	sim.DisplayGraphicEx(0,0,108,108,150,521);
	

	if(sim_angle >= 7.65756 && sim_angle < 8.2465)
	sim.DisplayGraphicEx(108,0,108+104,108,150,521);
	
	
	//haut droite
	if(sim_angle >= 8.2465 && sim_angle < 8.8355)
	sim.DisplayGraphicEx(216,0,216+104,108,150,521);
	
	
	if(sim_angle >= 8.8355 && sim_angle < 9.42477)
	sim.DisplayGraphicEx(216+108,0,428,108,150,521);
	
	
	sim_angle -= 0.589;
	
	if(my_photo==true)
	{
	photo++;

	if(photo<100)	
	jrb.DisplayGraphic(145,455);
	else	
	{
		my_photo=false;
		photo=0;
	}
	}

	
}



void captor::digital()
{
	mxhwnd.paint.mxdrawrect(111,360,217,377,RGB(149,1,25),RGB(0,0,0));
	mxhwnd.paint.mxdrawrect(111,380,217,397,RGB(149,1,25),RGB(0,0,0));
	mxhwnd.paint.mxdrawrect(111,400,217,417,RGB(149,1,25),RGB(0,0,0));
	mxhwnd.paint.mxdrawrect(111,420,217,437,RGB(149,1,25),RGB(0,0,0));

	

	
	if(lenght < (213-112))
	lenght+=1;

	if(lenght_b < (213-112))
	lenght_b+=1;

	if(lenght_e < (213-112))
	lenght_e+=1;

//**************************************************************************
	if(bmp_avant == false && bmp_avant_e == false)
	mxhwnd.paint.mxline(213-lenght,375,213,375,RGB(255,244,2));
	
	if(bmp_avant == true && bmp_avant_b == false)
	{
	bmp_avant_b=true;
	lenght_b=0;
	}
	
	if(bmp_avant == true && bmp_avant_b == true)
	{

	if(lenght_b<101)
	{
		mxhwnd.paint.mxline(213-lenght_b,362,213,362,RGB(255,244,2));
		mxhwnd.paint.mxline(213-lenght_b,362,213-lenght_b,375,RGB(255,244,2));
		mxhwnd.paint.mxline(112,375,213-lenght_b,375,RGB(255,244,2));
	}
	else
	{
		mxhwnd.paint.mxline(112,362,213,362,RGB(255,244,2));
	}
	mxhwnd.text.sprinttext(230,360,"1");
	}
	

	if(bmp_avant == false && bmp_avant_b == true)
	{
	bmp_avant_b=false;
	bmp_avant_e=true;
	lenght_e=0;
	}
	
	if(bmp_avant == false && bmp_avant_e == true)  //fin de l'échelon
	{
	
	if(lenght_e<101)
	{
		mxhwnd.paint.mxline(213-lenght_b,362,213-lenght_e,362,RGB(255,244,2));
	
		if(lenght_b<101)
		{
		mxhwnd.paint.mxline(112,375,213-lenght_b,375,RGB(255,244,2));
		mxhwnd.paint.mxline(213-lenght_b,362,213-lenght_b,375,RGB(255,244,2));
		}

		mxhwnd.paint.mxline(213-lenght_e,362,213-lenght_e,375,RGB(255,244,2));
		mxhwnd.paint.mxline(213-lenght_e,375,213,375,RGB(255,244,2));
	}
	else
	{
		mxhwnd.paint.mxline(112,375,213,375,RGB(255,244,2));
	}
	
	}


//**************************************************************************
	if(bmp_arr == false && bmp_arr_e == false)
	mxhwnd.paint.mxline(213-lenght,395,213,395,RGB(255,244,2));
	
	if(bmp_arr == true && bmp_arr_b == false)
	{
	bmp_arr_b=true;
	lenght_b=0;
	}
	
	if(bmp_arr == true && bmp_arr_b == true)
	{

	if(lenght_b<101)
	{
		mxhwnd.paint.mxline(213-lenght_b,382,213,382,RGB(255,244,2));
		mxhwnd.paint.mxline(213-lenght_b,382,213-lenght_b,395,RGB(255,244,2));
		mxhwnd.paint.mxline(112,395,213-lenght_b,395,RGB(255,244,2));
	}
	else
	{
		mxhwnd.paint.mxline(112,382,213,382,RGB(255,244,2));
	}
	mxhwnd.text.sprinttext(230,380,"1");
	}
	

	if(bmp_arr == false && bmp_arr_b == true)
	{
	bmp_arr_b=false;
	bmp_arr_e=true;
	lenght_e=0;
	}
	
	if(bmp_arr == false && bmp_arr_e == true)  //fin de l'échelon
	{
	
	if(lenght_e<101)
	{
		mxhwnd.paint.mxline(213-lenght_b,382,213-lenght_e,382,RGB(255,244,2));
	
		if(lenght_b<101)
		{
		mxhwnd.paint.mxline(112,395,213-lenght_b,395,RGB(255,244,2));
		mxhwnd.paint.mxline(213-lenght_b,382,213-lenght_b,395,RGB(255,244,2));
		}

		mxhwnd.paint.mxline(213-lenght_e,382,213-lenght_e,395,RGB(255,244,2));
		mxhwnd.paint.mxline(213-lenght_e,395,213,395,RGB(255,244,2));
	}
	else
	{
		mxhwnd.paint.mxline(112,395,213,395,RGB(255,244,2));
	}
	
	}


	
	//mxhwnd.paint.mxline(112,382,214,382,RGB(255,244,2));
	//mxhwnd.paint.mxline(213-lenght,395,214,395,RGB(255,244,2));
	//mxhwnd.paint.mxline(112,402,214,402,RGB(255,244,2));
	mxhwnd.paint.mxline(213-lenght,415,214,415,RGB(255,244,2));
	//mxhwnd.paint.mxline(112,422,214,422,RGB(255,244,2));
	mxhwnd.paint.mxline(213-lenght,433,214,433,RGB(255,244,2));
}

