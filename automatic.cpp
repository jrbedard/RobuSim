/*

   ROBUS université de Sherbrooke
S1 a-2001

Jean-René Bédard  01 128 041

deslock1@hotmail.com

Automatisation des fonctions de bases
--bumpers
--menu
--path!!!!!
*/


#include "thehead.h"
#include <math.h>






bool path_on;

extern long mouse_x,mouse_y;

float bot_x;
float bot_y;

int path_x[100];
int path_y[100];


 bool lb_down;
 bool rb_down;

bool first_time=true;

int w;

extern bool path_ok;
extern float path_motg;
extern float path_motd;

int distance;
double angle;
double dx,dy;
float inter;
int g;
bool just_enter;

bool go_back;


float bot_angle;


double supera;

bool explo;
int counter_e;

bool stop_it;

bool turn_over;


float temps;

//toward the note pad
int changed;
extern char pad_buff[100][10];
 
int lineA;




void automatic::load()
{

}



void automatic::update()
{
	

	if(path_on==true)
	{
	mxhwnd.GetMousePosWin(mouse_x,mouse_y);
	pathfinder();
	ai();
	}
	else
	{
	reset();
	}

	//menu(); version 1.2


}



void automatic::tang()
{
	if((path_x[g+1]-path_x[g]) != 0)
	supera= ((double)(path_y[g+1]-path_y[g])/(double)(path_x[g+1]-path_x[g]));
	
}





// parcours
void automatic::pathfinder()
{

//	mxhwnd.RestoreCursor();
	
	if(first_time==true)
	{
	w=0;
	path_x[w] = (int)bot_x+35;
	path_y[w] = (int)bot_y+35;
	first_time=false;
	}
	
	if(path_y[w] <0)
	{
	if(mouse_y >= -path_y[w])
	mxhwnd.paint.mxline(-path_x[w],-path_y[w],mouse_x,mouse_y,RGB(173,214,245));
	else
	mxhwnd.paint.mxline(mouse_x,mouse_y,-path_x[w],-path_y[w],RGB(173,214,245));
	}
	else
	{
	if(mouse_y >= path_y[w])
	mxhwnd.paint.mxline(path_x[w],path_y[w],mouse_x,mouse_y,RGB(173,214,245));
	else
	mxhwnd.paint.mxline(mouse_x,mouse_y,path_x[w],path_y[w],RGB(173,214,245));
	}
	
	
	
	if(lb_down==true && mouse_x != path_x[w] && mouse_x > 296 && mouse_x < (1000) && mouse_y > 16 && mouse_y < (688))
	{
	w++;
	path_x[w]=mouse_x;
	path_y[w]=mouse_y;
	path_ok=true;
	}

	if(rb_down==true && mouse_x != path_x[w] && mouse_x > 296 && mouse_x < (1000) && mouse_y > 16 && mouse_y < (688))
	{
	w++;
	path_x[w]=-mouse_x;
	path_y[w]=-mouse_y;
	path_ok=true;
	}

	
	for(int q=0; q<w ;q++)
	{
	
	if(path_x[q]>0 && path_x[q+1]>0) // avance
	{
	if(path_y[q+1] >= path_y[q])
	mxhwnd.paint.mxline(path_x[q],path_y[q],path_x[q+1],path_y[q+1],RGB(173,214,245));
	else
	mxhwnd.paint.mxline(path_x[q+1],path_y[q+1],path_x[q],path_y[q],RGB(173,214,245));
	}
	
	if(path_x[q+1]<0 && path_x[q]>0) // reculon
	{
	if(-path_y[q+1] >= path_y[q])
	mxhwnd.paint.mxline(path_x[q],path_y[q],-path_x[q+1],-path_y[q+1],RGB(173,204,245));
	else
	mxhwnd.paint.mxline(-path_x[q+1],-path_y[q+1],path_x[q],path_y[q],RGB(173,204,245));
	}
	
	if(path_x[q]<0 && path_x[q+1]>0) // reculon
	{
	if(path_y[q+1] >= -path_y[q])
	mxhwnd.paint.mxline(-path_x[q],-path_y[q],path_x[q+1],path_y[q+1],RGB(173,204,245));
	else
	mxhwnd.paint.mxline(path_x[q+1],path_y[q+1],-path_x[q],-path_y[q],RGB(173,204,245));
	}

	if(path_x[q]<0 && path_x[q+1]<0) // reculon
	{
	if(-path_y[q+1] >= -path_y[q])
	mxhwnd.paint.mxline(-path_x[q],-path_y[q],-path_x[q+1],-path_y[q+1],RGB(173,204,245));
	else
	mxhwnd.paint.mxline(-path_x[q+1],-path_y[q+1],-path_x[q],-path_y[q],RGB(173,204,245));
	}
	
	
	//CERCLES 
	if(path_x[q+1]<0) //reculon
	mxhwnd.paint.mxdrawellipse(-path_x[q+1]-5,-path_y[q+1]-5,-path_x[q+1]+5,-path_y[q+1]+5,RGB(43,43,233),RGB(43,43,233));
	else // avant
	mxhwnd.paint.mxdrawellipse(path_x[q+1]-5,path_y[q+1]-5,path_x[q+1]+5,path_y[q+1]+5,RGB(233,43,43),RGB(233,43,43));
	
	}

}


//semi-intelligence au niveau du parcours à suivre
void automatic::ai()    
{
	
	
	if(path_x[g]<0)
	{
	path_x[g] = -path_x[g];
	path_y[g] = -path_y[g];
	}
	
	if(path_x[g+1]<0)
	{
	path_x[g+1] = -path_x[g+1];
	path_y[g+1] = -path_y[g+1];
	go_back=true;
	}
	
	
	dx= (path_x[g+1]-path_x[g]);
	dy= (path_y[g+1]-path_y[g]);
	
	//dx=	(int)(path_x[g+1]-bot_x+35);
	//dy= (int)(path_y[g+1]-bot_y+35);
	


	distance = (int)sqrt((dx*dx + dy*dy));
	

	//if(dy>0)
	//bot_angle += 1.589;
	//else
	bot_angle += 1.589;

	just_enter=true;

	if(bot_angle > 9.4247)
	bot_angle = 0 + (bot_angle - 9.8247);

	
	if(turn_over == false)     //   TURNING  BIG FAT ROBUS'ASS
	{
		
		//droite en bas
		if( dx >= 0 && dy >= 0)
		{
		angle = atan(dy/dx);
		
		if(bot_angle< ((angle*1.655+0.1)) && bot_angle > ((angle*1.655-0.1)))
		{
		turn_over =true;
		}
		else
		{
		path_motg= 50;
		path_motd= -50;
		}
		}
	
	    
		// gauche en bas
		if( dx < 0 && dy >= 0)
		{
		bot_angle -= 2.6; 
		
	 	angle = atan(dx/dy);
		angle = 0-angle;

		if(bot_angle< ((angle*1.655+0.1)) && bot_angle > ((angle*1.655-0.1)))
		{
		turn_over =true;
		}
		else
		{
		path_motg= 50;
		path_motd= -50;
		}
		bot_angle += 2.6; 
		}


		//gauche en haut
		if( dx <= 0 && dy <= 0)
		{
		bot_angle -= 2.5*2; 
		angle = atan(dy/dx);
		
		if(bot_angle< ((angle*1.655+0.1)) && bot_angle > ((angle*1.655-0.1)))
		{
		turn_over =true;
		}
		else
		{
		path_motg= 50;
		path_motd= -50;
		}
		bot_angle += 2.5*2; 
		}

	
		//droite en haut
		if( dx > 0 && dy < 0)
		{
		bot_angle -= 2.48*3; 
		angle = atan(dx/dy);
		angle = 0-angle;

		if(bot_angle< ((angle*1.655+0.1)) && bot_angle > ((angle*1.655-0.1)))
		{
		turn_over =true;
		}
		else
		{
		path_motg= 50;
		path_motd= -50;
		}
		bot_angle += 2.48*3; 
		}

	
		bot_angle -= 1.589;
	
	}
	else  // ne trourne plus: avance !!!!
	{
	bot_angle -= 1.589;
	temps = (distance / 235.0) * 4.0;
	path_motg=85;
	path_motd=84;
	
	}
	


	if(path_motg != changed)  // nouveau texte
	{
	itoa((int)path_motg,pad_buff[lineA],10);
	

	lineA++;
	}
	changed = path_motg;


    //bot_angle += 0.589;


	//mxhwnd.text.sprinttext(650,703,"%d",dx);
	mxhwnd.text.sprinttext(650,723,"%d",distance);
	//mxhwnd.text.sprinttext(650,743,"%d",dy);
	
	if(go_back==true)
	{
	path_motg = -path_motg;
	path_motd = -path_motd;
	go_back=false;
	}
	
	
	if(bot_x+35 < path_x[g+1]+35 && bot_x+35 > path_x[g+1]-35 && bot_y+35 < path_y[g+1]+35 && bot_y+35 > path_y[g+1]-35)	
	{
	path_motg=0;
	path_motd=0;

	g++;
	
	
	//path_x[g]=bot_x+35;
	//path_y[g]=bot_y+35;
	

	turn_over=false;
	just_enter=false;
	}

}




/*

MAP editor dans la  version 1.2   
void automatic::menu()
{
	if(mouse_x > 803 && mouse_x < 832 && mouse_y > 717 && mouse_y < 746 && lb_down == true)
	{
	if(map_ed==true)
	{
	map_ed=false;
	return;
	}
	else
	{
	map_ed=true;
	menu_up=0;
	return;
	}
	}
	
	if(mouse_x > 740 && mouse_x < 768 && mouse_y > 717 && mouse_y < 746 && lb_down == true)
	{
	if(explo==true)
	{
		explo=false;
		return;
	}
	else
	{
		explo=true;
		return;
	}
	}
	
	
	if(map_ed==true)
	{
	pushed.DisplayGraphicEx(0,87,29,116,803,717);
		
	mxhwnd.paint.mxdrawrect(650,685-menu_up,850,706,RGB(4,25,102),RGB(207,191,191));
	mxhwnd.text.printtext("Map Editor",660,686-menu_up);
	
	if(menu_up < 100)
	menu_up+=10;
	
	
	if(lb_down==true && mouse_x > 296 && mouse_x < (1000) && mouse_y > 16 && mouse_y < (688))
	{
	if(place_brick==false)
	place_brick=true; //map ed			
	else
	place_brick=false; //map ed	
	}

	}
	
}
*/


void automatic::reset()
{
	//if(stop_it==false)
	{
	path_motd=0;
	path_motg=0;
	stop_it=true;
	path_ok=false;
	}
}





