
/*
Jean-René Bédard

  MAP 
		création de l'environnement

*/


#include "thehead.h"
#include <fstream.h>

int start_block=0;
int row_count;

char * map_name;

extern int obstacle_x[800];
extern int obstacle_y[800];

extern int obstacle_nb;

bool obstacle_checked=false;

bool place_brick=false;
long mouse2_x,mouse2_y;

bool mnu_ed;
bool map_g,map_d;

bool place;
int item;


void map::load()
{
    map_name= "map.des";
	loadmap();
	mxhwnd.LoadGraphic(&tiles[0],"Bmps\\wood.bmp");
	mxhwnd.LoadGraphic(&tiles[2],"Bmps\\stone.bmp");

	mxhwnd.LoadGraphic(&maped,"Bmps\\maped.bmp");
	
	mxhwnd.LoadGraphic(&plant1,"Bmps\\elements.bmp");
	plant1.SetColorKey(RGB(0,0,0));
}


void map::loadmap()
{
	ifstream fin(map_name,ios::binary);
	fin.read((char*)&level,sizeof(level));
	fin.close();
	

}


void map::update()
{
	

	
	mxhwnd.paint.mxdrawrect(0,0,1024,768,RGB(173,173,173),RGB(230,210,190));
	drawmap();
	
  

	if(mnu_ed==true)
	{
	mxhwnd.GetMousePosWin(mouse2_x,mouse2_y);	
	mapeditor();

	
	if(place==true)
	drag();
	}	
	
}




void map::drawmap()
{

	start_x = 280;
	start_y = 25;
	row_count = 0;		

	sx = start_x;
	sy = start_y;
			
	
			
		for(i = start_block; i < start_block+1886; i++)
					
		{
			
			if(level.level_block[i] == 0)
			tiles[0].DisplayGraphic(sx,sy);
			
			
			
			if(level.level_block[i] == 2 )
			{
				
				tiles[2].DisplayGraphic(sx,sy);
				
				
				if(obstacle_checked==false)
				{
				obstacle_x[obstacle_nb] = sx;
				obstacle_y[obstacle_nb] = sy;
				obstacle_nb++;
				}
			
			}
				
				row_count ++;
				sy = sy + 16;
				
				
				if(row_count > 40)
				{
					row_count = 0;
					sx = sx + 16;
					sy = start_y;
					
				}
		}

		obstacle_checked=true;
}





void map::mapeditor()
{
  maped.DisplayGraphicEx(0,0,635,57,315,0);
		
if(place==false)
{

  if(mouse2_x > 320 && mouse2_y >= 1 && mouse2_x <= 374 && mouse2_y <= 57)
  {
	maped.DisplayGraphicEx(0,57,59,114,315,0);
	
	if(map_g==true)
	{
		item=1;
		place=true;
	}
  }
  
	  
  if(mouse2_x > 374 && mouse2_y >= 1 && mouse2_x <= 427 && mouse2_y <= 57)
  {
	maped.DisplayGraphicEx(58,57,112,114,373,0);
  
	if(map_g==true)
	{
		item=2;
		place=true;
	}
  }
  
  

}

  
  if(mouse2_x > 927 && mouse2_y >= 37 && mouse2_x <= 943 && mouse2_y <= 51  && map_g==true)
	mnu_ed=false;
}



void map::drag()
{
	
	switch(item)
	{
	case 1:
		tiles[2].DisplayGraphic(mouse2_x-8,mouse2_y-8);
		break;
	case 2:
		
		break;



	}


	
	if(map_g==true)
	{
	switch(item)
	{
	case 1:
		mxhwnd.GetMousePosWin(mouse2_x,mouse2_y);	
		level.level_block[getblockbypos(mouse2_x,mouse2_y)]=2;
		break;
	case 2:
		
		break;
	}





	}



if(map_d==true)
{
place=false;

}


}






int map::getblockbypos(int x, int y)
{
	
	int start_x = 280;
	int start_y = 25;
	int sx = start_x;
	int sy = start_y;
	
	int row_count = 0;
	
	for(int i = 0; i < 1886; i++)
	{
		
		if(x > sx && x < sx+16 && y >  sy && y < sy+16)
		{
			return i;
		}
		
		row_count ++;
		
		sy = sy + 16;
		
		if(row_count > 40)
		{
			row_count = 0;
			sx = sx + 16;
			sy = start_y;
		}
	}

	return -1; // not a block
}



/* if(place_brick==true)
   {
	mxhwnd.GetMousePosWin(mouse2_x,mouse2_y);	
	
	level.level_block[getblockbypos(mouse2_x,mouse2_y)+45]=2;
    place_brick=false;
    obstacle_checked=false;
   }
*/