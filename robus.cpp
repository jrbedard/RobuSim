
/*
v1.0b
Pseudo emulation du Robot ROBUS de l'université de Sherbrooke
S1 a-2001

Jean-René Bédard  01 128 041

deslock1@hotmail.com

*/


#include "thehead.h"
#include "math.h"


// déclarations variables
float motor_g;
float motor_d;



bool reaction;
float new_angle;


int sy,ex,ey;
int timer=0;

double obj_y;
double obj_x;

float pie;
float variation;

float arg;

float block;

int t;

int debut;

int obstacle_x[200];
int obstacle_y[200];
int obstacle_nb;



extern bool stop;
bool reverse;
extern bool reculon;
bool bumper_on;





float new_x;
float new_y;

bool trace_on;

extern bool bmp_avant;
extern bool bmp_right;
extern bool bmp_left;
extern bool bmp_arr;

int val;	

int b;

/// PATH********
int point_x[10000];
int point_y[10000];

float angle_buff;

extern float bot_x;
extern float bot_y;
extern float bot_angle;


bool path_ok;
float path_motg;
float path_motd;


//*******************


//toward console******
extern bool con_text;
extern char* con_string;

//*********************



//*********************captors
extern int pos_x;
extern int pos_y;
extern int angles;
extern int dis_g;
extern int dis_d;




extern float sim_angle;



void robus::load()
{
  
bot.robus_x = 500;
bot.robus_y = 300;
bot.robus_angle =0;	
	

mxhwnd.LoadGraphic(&robus_p,"Bmps\\robus.bmp");


robus_p.SetColorKey(RGB(0,255,0));
}




void robus::update()
{

sim_angle = bot.robus_angle;


hoover();	

if(bumper_on==true)
ai();

limit();

exept();

move();
	


draw();


}



void robus::hoover()
{

	if(reverse==false && angle_buff==0)
	pie += variation;
	if(reverse==true && angle_buff==0)
	pie -= variation;


	if(pie >= 9.4247)
	pie=0;

	if(pie < 0)
	pie=9.4247;
	
	
	obj_y = sin(pie/1.5);
	obj_x = cos(pie/1.5);

}


////////////////////////////////////GENERAL MOVING////////////////////////////////

void robus::exept()
{
		bot_x=bot.robus_x;
		bot_y=bot.robus_y;    // pour le pathfinder
		bot_angle=bot.robus_angle;
		


	if(bot.robus_x < 290 || bot.robus_x > 1007 || bot.robus_y < 10 || bot.robus_y > 688)
	{
		bot.robus_x = 500;
		bot.robus_y = 500; 

	}


		if(path_ok==true)
		{
		motor_g = path_motg;
		motor_d = path_motd;
		}


		if(trace_on==true)
		{
		
		/*
		if(bot.robus_x != point_x[b] || bot.robus_y != point_y[b] && bot.robus_x != 500)
		{
		b++;
		point_x[b]=bot.robus_x;
		point_y[b]=bot.robus_y;
		}

		for(int p=0;p<b;b++)
		{
		mxhwnd.paint.mxdrawrect(bot.robus_x,bot.robus_y,bot.robus_x+1,bot.robus_y+1,RGB(0,0,0),RGB(0,0,0));
		}
		*/
		}


		
		if(new_angle !=0)
		{
		bot.robus_angle=new_angle;
		pie=new_angle;
		new_angle=0;

		}
		
		
		
		if(new_x != 0 && new_y != 0)
		{
		if(new_x > 626)
		new_x= 626;
		if(new_x < 0)
		new_x=1;
		if(new_y<0)
		new_y=1;
		if(new_y >595)
		new_y=595;
		bot.robus_x = new_x+296;
		bot.robus_y = new_y+16;
		new_x=0;
		new_y=0;
		}
	
	
		if(debut==0)
		{
		bot.robus_angle = 0.588;
		pie=bot.robus_angle;
		debut=1;
		}
		
		
		if(angle_buff!=0)
		{
		pie=angle_buff;
		bot.robus_angle=angle_buff;
		angle_buff=0;
		}
		
	
		if(motor_g >99)
			motor_g=99;
		
		if(motor_d >99)
			motor_d=99;




		//******************captors
		pos_x = bot.robus_x;
		pos_y = bot.robus_y;
		angles = bot.robus_angle;
		dis_g=motor_g;
		dis_d=motor_d;
}













void robus::move()
{
	
		if(motor_g > motor_d)
		{
			val = motor_g - motor_d;
		}
		
		if(motor_d > motor_g)
		{
			val = motor_d - motor_g;
		}
		
		if(val < 10)
		variation = 0.0001;
		
		if(val >= 10 && val <25)
		variation = 0.010;
		
		if(val >= 25 && val <50)
		variation = 0.025;
	
		if(val >= 50 && val <75)
		variation = 0.03;
			
		if(val >= 75 && val <= 100)
		variation = 0.015;
			
		/*
		if(val >= 100 && val <125)
		variation = 0.02;
			
		if(val >=125 && val <150)
		variation = 0.05;
			
		if(val >= 150 && val <= 175)
		variation = 0.1;
	
		if(val >= 175 && val <= 200)
		variation = 0.3;
		*/

		
		if(motor_g > motor_d && motor_d > 0)
		{
				if(t==0)
				{
					pie=bot.robus_angle;
					t=1;
				}

				bmp_avant=false;
				bmp_arr=false;
				bmp_right=false;
				bmp_left=false;

				if((motor_g < 0 && motor_d < 0) || -motor_d > motor_g) 
				{
				reverse=false;
				bot.robus_y -= obj_y*(motor_g/100+2);
				bot.robus_x -= obj_x*(motor_g/100+2);
				reculon =true;
				}
				else	
				{
				reverse=false;
				bot.robus_angle=pie;
				bot.robus_y += obj_y*(motor_g/100+1);
				bot.robus_x += obj_x*(motor_g/100+1);
				reculon = false;
				}
		}
		
		
		
		if(motor_g > motor_d && motor_d <= 0)
		{
			if(t==0)
			{
				pie=bot.robus_angle;
				t=1;
			}
			
			if(motor_d>-10)
			variation = 0.2;
			else
			variation = 0.3;

			if(motor_d<-70);
			variation = 0.4;
			
			bot.robus_angle=pie;
			bot.robus_y += obj_y*(-motor_d/100+1);
			bot.robus_x += obj_x*(-motor_d/100+1);
		}

		
		
		// DROIT DOMINANT******************************************************
		
		if(motor_d > motor_g && motor_g > 0)
		{
				if(t==0)
				{
					pie=bot.robus_angle;
					t=1;
				}

				bmp_avant=false;
				bmp_arr=false;
				bmp_right=false;
				bmp_left=false;

				if((motor_d < 0 && motor_g < 0) || -motor_g > motor_d) 
				{
				reverse=false;
				bot.robus_y -= obj_y*(motor_d/100+2);
				bot.robus_x -= obj_x*(motor_d/100+2);
				reculon =true;
				}
				else	
				{
				reverse=true;
				bot.robus_angle=pie;
				bot.robus_y += obj_y*(motor_d/100+1);
				bot.robus_x += obj_x*(motor_d/100+1);
				reculon = false;
				}
		}
		
		
		if(motor_d > motor_g && motor_g <= 0)
		{
			if(t==0)
			{
				pie=bot.robus_angle;
				t=1;
			}
			reverse=true;
			if(motor_g>-10)
			variation = 0.2;
			else
			variation = 0.3;

			if(motor_g<-70);
			variation = 0.4;
			
			bot.robus_angle=pie;
			bot.robus_y += obj_y*(-motor_d/100+1);
			bot.robus_x += obj_x*(-motor_d/100+1);
		}
		
		
		
		/*
		if(motor_d >= motor_g && motor_d !=0 && motor_g != 0)
		{
				if(t==0)
				{
					pie=bot.robus_angle;
					t=1;
				}

				bmp_avant=false;
				bmp_arr=false;
				bmp_right=false;
				bmp_left=false;

				if((motor_g < 0 && motor_d <0) || -motor_g > motor_d)
				{
				reverse =true;
				bot.robus_angle=pie; 
				bot.robus_y -= obj_y*(motor_g/100+2);
				bot.robus_x -= obj_x*(motor_g/100+2);
				reculon =true;
				}
				else
				{
				reverse =true;
				bot.robus_angle=pie;
				bot.robus_y += obj_y*(motor_g/100+1.5);
				bot.robus_x += obj_x*(motor_g/100+1.5);
				reculon =false;
				}
		}
	
		*/
	
	
		/*
		if(motor_d==0 && motor_g==0)
		{
			bmp_avant=false;
			bmp_arr=false;
		
		}
		*/
		
		
		
		
		
		
		/*
		if(motor_d == motor_g && motor_d !=0)
		{
				if(t==0)
				{
					pie=bot.robus_angle;
					t=1;
				}


				bmp_avant=false;
				bmp_arr=false;
				bmp_right=false;
				bmp_left=false;

				if(motor_g < 0)
				{
				reverse =true;
				bot.robus_angle=pie; 
				bot.robus_y -= obj_y*(motor_g/100+3);
				bot.robus_x -= obj_x*(motor_g/100+3);
				reculon =true;
				}
				else
				{
				reverse =true;
				bot.robus_angle=pie;
				bot.robus_y += obj_y*(motor_g/100+1.5);
				bot.robus_x += obj_x*(motor_g/100+1.5);
				reculon =false;
				}


		}
*/
}








// check-up si un obstacle est rencontré
///////////////////////////////////OBSTACLES/////////////////////////////////////////////
void robus::limit()
{

	//les murs
	
	for(int obs = 0 ; obs <= obstacle_nb; obs++)
	{
		
		for(int wo = 0 ; wo < 78; wo++)
		{
		
		if(reculon ==false)
		{
		// de la droite
		if(bot.robus_angle > 8.2465 || bot.robus_angle <= 1.767)
		if(bot.robus_x+78 > obstacle_x[obs] && bot.robus_x+78 < obstacle_x[obs] + 16 && bot.robus_y+wo > obstacle_y[obs] && bot.robus_y +wo < obstacle_y[obs] +16)
		{
		bot.robus_x = obstacle_x[obs]-78;
		bot.robus_y = bot.robus_y+wo;
		angle_buff = bot.robus_angle;
		motor_d =0;
		motor_g =0;
		t=0;
		wo= 78;
		bmp_arr =false;
		bmp_avant=true;
	
		//	con_string="hello";
		}
	
		// de la gauche
		if(bot.robus_angle > 3.534 && bot.robus_angle <= 6.479)
		if(bot.robus_x > obstacle_x[obs] && bot.robus_x < obstacle_x[obs] + 16 && bot.robus_y+wo > obstacle_y[obs] && bot.robus_y+wo < obstacle_y[obs] +16)
		{
		bot.robus_x = obstacle_x[obs]+16;
		bot.robus_y = bot.robus_y+wo;
		angle_buff = bot.robus_angle;
		motor_d =0;
		motor_g =0;
		t=0;
		wo=78;
		bmp_avant=true;
		bmp_arr =false;
		con_text=true;
	//	con_string="hello";
		}
	
		// de dessus
		if(bot.robus_angle > 1.1780 && bot.robus_angle <= 3.534)
		if(bot.robus_x+wo > obstacle_x[obs] && bot.robus_x+wo < obstacle_x[obs] + 16 && bot.robus_y+78 > obstacle_y[obs] && bot.robus_y+78 < obstacle_y[obs] +16)
		{
		bot.robus_x = bot.robus_x+wo;
		bot.robus_y = obstacle_y[obs]-78;
		angle_buff = bot.robus_angle;
		motor_d =0;
		motor_g =0;
		t=0;
		wo=78;
		bmp_avant=true;
		bmp_arr =false;
		}
	
		// de dessous
		if(bot.robus_angle > 6.479 && bot.robus_angle <= 8.2465)
		if(bot.robus_x+wo > obstacle_x[obs] && bot.robus_x+wo < obstacle_x[obs] + 16 && bot.robus_y > obstacle_y[obs] && bot.robus_y < obstacle_y[obs] +16)
		{
		bot.robus_x = bot.robus_x+wo;
		bot.robus_y = obstacle_y[obs]+16;
		angle_buff = bot.robus_angle;
		motor_d =0;
		motor_g =0;
		t=0;
		wo = 78;
		bmp_avant=true;
		bmp_arr =false;
		}
		}
	 	
		
		// obstacle percuté de reculon
		if(reculon ==true)
		{
		// de la droite
		if(bot.robus_angle > 3.534 && bot.robus_angle <= 6.479)
		if(bot.robus_x+78 > obstacle_x[obs] && bot.robus_x+78 < obstacle_x[obs] + 16 && bot.robus_y+wo > obstacle_y[obs] && bot.robus_y+wo < obstacle_y[obs]+16)
		{
		bot.robus_x = obstacle_x[obs]-78;
		bot.robus_y = bot.robus_y+wo;
		angle_buff = bot.robus_angle;
		motor_d =0;
		motor_g =0;
		t=0;
		wo=78;
		bmp_arr =true;
			bmp_avant=false;
		}
	
		// de la gauche
		if(bot.robus_angle > 8.2465 || bot.robus_angle <= 1.767)
		if(bot.robus_x > obstacle_x[obs] && bot.robus_x < obstacle_x[obs] + 16 && bot.robus_y+wo > obstacle_y[obs] && bot.robus_y+wo < obstacle_y[obs] +16)
		{
		bot.robus_x = obstacle_x[obs]+16;
		bot.robus_y = bot.robus_y+wo;
		angle_buff = bot.robus_angle;
		motor_d =0;
		motor_g =0;
		t=0;
		wo=78;
		bmp_arr =true;
			bmp_avant=false;
		}
	
		// de dessus
		if(bot.robus_angle > 6.479 && bot.robus_angle <= 8.2465)
		if(bot.robus_x+wo > obstacle_x[obs] && bot.robus_x+wo < obstacle_x[obs] + 16 && bot.robus_y+78 > obstacle_y[obs] && bot.robus_y+78 < obstacle_y[obs] +16)
		{
		bot.robus_x += wo;
		bot.robus_y = obstacle_y[obs]-78;
		angle_buff = bot.robus_angle;
		motor_d =0;
		motor_g =0;
		t=0;
		wo=78;
		bmp_arr =true;
			bmp_avant=false;
		}
	
		// de dessous
		if(bot.robus_angle > 1.1780 && bot.robus_angle <= 3.534)
		if(bot.robus_x+wo > obstacle_x[obs] && bot.robus_x+wo < obstacle_x[obs] + 16 && bot.robus_y > obstacle_y[obs] && bot.robus_y < obstacle_y[obs] +16)
		{
		bot.robus_x += wo;
		bot.robus_y = obstacle_y[obs]+16;
		angle_buff = bot.robus_angle;	
		motor_d =0;
		motor_g =0;
		t=0;
		wo=78;
		bmp_arr =true;
			bmp_avant=false;
		}
		}
	
		}	
		
	}
	
}




// dessiner robus sur l'écran sous UN angle et un status (bumperON || bumperOFF)
void robus::draw()
{
	bot.robus_angle += 0.589;
   

	// vers droite
	if(bot.robus_angle >= 0.0 && bot.robus_angle < 0.589 || bot.robus_angle >= 9.42477)	
	{
	if(bmp_avant==true)
	robus_p.DisplayGraphicEx(315,231,78+314,308,bot.robus_x,bot.robus_y);
	else
	robus_p.DisplayGraphicEx(0,231,78,308,bot.robus_x,bot.robus_y);
	}
	
	if(bot.robus_angle >= 0.589 && bot.robus_angle < 1.1780)	
	{
	if(bmp_avant==true)
	robus_p.DisplayGraphicEx(234+314,158,313+315,237,bot.robus_x,bot.robus_y);
	else
	robus_p.DisplayGraphicEx(233,158,313,237,bot.robus_x,bot.robus_y);
	}
	
	// bas-droite
	if(bot.robus_angle >= 1.1780 && bot.robus_angle < 1.767)	
	robus_p.DisplayGraphicEx(77,234,156,311,bot.robus_x,bot.robus_y);
	
	
	if(bot.robus_angle >= 1.767 && bot.robus_angle < 2.35619)	
	{
	if(bmp_avant==true)
	robus_p.DisplayGraphicEx(155+315,155,234+314,234,bot.robus_x,bot.robus_y);
	else
	robus_p.DisplayGraphicEx(155,155,234,234,bot.robus_x,bot.robus_y);
	}
	

	//bas
	if(bot.robus_angle >= 2.35619 && bot.robus_angle < 2.9451)	
	{
	if(bmp_avant==true)
	robus_p.DisplayGraphicEx(314,155,77+314,231,bot.robus_x,bot.robus_y);
	else
	robus_p.DisplayGraphicEx(0,155,77,231,bot.robus_x,bot.robus_y);
	}
	
	
	if(bot.robus_angle >= 2.9453 && bot.robus_angle < 3.534)	
	{
	if(bmp_avant==true)
	robus_p.DisplayGraphicEx(155+314,235,234+314,314,bot.robus_x,bot.robus_y);
	else
	robus_p.DisplayGraphicEx(155,235,234,314,bot.robus_x,bot.robus_y);
	}
	
	
	//bas_gauche
	if(bot.robus_angle >= 3.534 && bot.robus_angle < 4.123)
	robus_p.DisplayGraphicEx(77,77,155,156,bot.robus_x,bot.robus_y);
	
	
	if(bot.robus_angle >= 4.123 && bot.robus_angle < 4.71238)
	{
	if(bmp_avant==true)
	robus_p.DisplayGraphicEx(234+315,237,312+315,315,bot.robus_x,bot.robus_y);
	else
	robus_p.DisplayGraphicEx(234,237,312,315,bot.robus_x,bot.robus_y);
	}
	
	//gauche
	if(bot.robus_angle >= 4.71238 && bot.robus_angle < 5.30138)
	{
	if(bmp_avant==true)
	robus_p.DisplayGraphicEx(315,77,77+315,155,bot.robus_x,bot.robus_y);
	else
	robus_p.DisplayGraphicEx(0,77,77,155,bot.robus_x,bot.robus_y);
	}
	
	if(bot.robus_angle >= 5.30138 && bot.robus_angle < 5.8903)
	robus_p.DisplayGraphicEx(234,79,313,157,bot.robus_x,bot.robus_y);
	
	
	//haut gauche
	if(bot.robus_angle >= 5.8903 && bot.robus_angle < 6.4799)
	robus_p.DisplayGraphicEx(78,0,157,78,bot.robus_x,bot.robus_y);
	
	
	if(bot.robus_angle >= 6.4799 && bot.robus_angle < 7.06858)
	robus_p.DisplayGraphicEx(155,77,234,155,bot.robus_x,bot.robus_y);	
	
	
	//haut
	if(bot.robus_angle >= 7.06858 && bot.robus_angle < 7.65756)
	{
	if(bmp_avant==true)
	robus_p.DisplayGraphicEx(315,0,78+315,77,bot.robus_x,bot.robus_y);
	else
	robus_p.DisplayGraphicEx(0,0,78,77,bot.robus_x,bot.robus_y);	
	}
	

	if(bot.robus_angle >= 7.65756 && bot.robus_angle < 8.2465)
	robus_p.DisplayGraphicEx(157,0,234,77,bot.robus_x,bot.robus_y);
	
	
	//haut droite
	if(bot.robus_angle >= 8.2465 && bot.robus_angle < 8.8355)
	robus_p.DisplayGraphicEx(77,156,154,233,bot.robus_x,bot.robus_y);


	if(bot.robus_angle >= 8.8355 && bot.robus_angle < 9.42477)
	robus_p.DisplayGraphicEx(235,0,312,78,bot.robus_x,bot.robus_y);


	bot.robus_angle -= 0.589;
}




//Intelligence artificielle suite à l'ACTIVATION des bumpers ,commande "bumper"
// en construction
void robus::ai()
{

if(bmp_avant == true && reaction==false)
{
reverse=true;
motor_d = -100;
motor_g = 12;
pie=bot.robus_angle;
reculon=true;
reaction=true;
t=0;
bmp_avant =false;
}


}






