#include "thehead.h"



char linea[100];
char * line2;

char * lcd_motg;
char * lcd_motd;


char cap_line1[100];
char cap_line2[100];



void lcd::load()
{

mxhwnd.LoadGraphic(&handy,"Bmps\\handy.bmp");


}





void lcd::update()
{
		draw();
}




void lcd::draw()
{
	handy.DisplayGraphic(5,640);
	mxhwnd.text.setfont(lcd_font);
	mxhwnd.text.settextcolor(RGB(10,10,10));
	mxhwnd.text.setbkcolor(RGB(143,155,139));
	mxhwnd.paint.mxdrawrect(40,680,240,710,RGB(143,155,139),RGB(143,155,139));

	
	
	//strcpy(linea,"fe");
	

	mxhwnd.text.printtext(cap_line1,40,678);
	mxhwnd.text.printtext(linea,40,678);
//	mxhwnd.text.printtext(line2,40,694);
}








