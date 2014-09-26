#include "thehead.h"
#include "resource.h"

bool pad_g,pad_d;

bool drag;

int pad_x=200;
int pad_y=300;

long mov_x,mov_y;

bool pad_on;
bool music_on;

char * log;

int next;
bool new_text;

int c_t;


char pad_buff[100][10];

int line;




BOOL CALLBACK DialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{	
	switch(uMsg)
	{
	
	case WM_INITDIALOG:    
			
		
		//	reset(hwndDlg);
			return TRUE;
	
	
	}		
	
	
	return TRUE;
}


//int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
//{
//	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_PAD), HWND_DESKTOP, (DLGPROC)DialogProc, 0l); 
//	return 0;
//} // WinMain
 


void pad::load()
{
mxhwnd.text.printtext("loading RobuSIM...",400,300);
mxhwnd.LoadGraphic(&logpad,"Bmps\\logpad.bmp");
mxhwnd.LoadGraphic(&musicpad,"Bmps\\music.bmp");

logpad.SetColorKey(RGB(0,255,0));
musicpad.SetColorKey(RGB(0,255,0));

midi();
}




void pad::update()
{
	


	if(pad_on==true)
	{
		mxhwnd.GetMousePosWin(mov_x,mov_y);
		//DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_PAD), HWND_DESKTOP, NULL, 0l);
		
		button();
		
		logpad.DisplayGraphic(pad_x,pad_y);

		draw();
		
		if(drag==false)
		text();
	}

	
	if(music_on==true)
	{
		mxhwnd.GetMousePosWin(mov_x,mov_y);
		
		button();
		
		musicpad.DisplayGraphic(pad_x,pad_y);
		
		draw();
		
		winamp();
	}
}







void pad::draw()
{
	
if(mov_x >= pad_x+93 && mov_y >= pad_y+3 && mov_x <= pad_x+156 && mov_y <= pad_y+21  && pad_g==true)
{
drag=true;
//mxhwnd.HideCursor();
}

if(pad_g==false)
{
drag=false;
//mxhwnd.RestoreCursor();
}


if(drag==true)
{
pad_x = mov_x-123;
pad_y = mov_y-16;
}


}




void pad::button()
{

if(pad_y <= 5)
pad_y=5;

if(pad_x <=5)
pad_x=5;

if(pad_x >= 1020-190)
pad_x=1020-190;

if(pad_y >= 760-241)
pad_y=760-241;




if(mov_x >= pad_x+162 && mov_y >= pad_y+5 && mov_x <= pad_x+179 && mov_y <= pad_y+19  && pad_g==true && drag==false)
{
pad_on=false;
music_on=false;
}

if(mov_x >= pad_x+150 && mov_y >= pad_y+5 && mov_x <= pad_x+160 && mov_y <= pad_y+19  && pad_g==true)
{
new_text=true;
}




}




void pad::text()
{
	mxhwnd.text.setbkcolor(RGB(255,255,255));
	mxhwnd.text.settextcolor(RGB(0,0,0));


	//strcpy(pad_buff[0],"motor 54 34");
	//strcpy(pad_buff[1],"motor 666");
	
	
	//	buff_len = strlen(pad_buff);
	
	
	
	
	while(line <20)
	{
	mxhwnd.text.printtext(pad_buff[line],pad_x+12,pad_y+30+next);
	line++;
	next += 15;
	}
	
	line=0;
	next=0;

	
	if(mov_x > pad_x+23 && mov_y >= pad_y+1 && mov_x <= pad_x+46 && mov_y <= pad_y+23  && pad_g==true)
	{
	ofstream fout("dance.txt");
	for(int h=0;h<=45;h++)
	fout << pad_buff[h] << endl;
	fout.close();
	}

}



void pad::winamp()
{


if(mov_x > pad_x+23 && mov_y >= pad_y+1 && mov_x <= pad_x+46 && mov_y <= pad_y+23  && pad_g==true)
openfichier(".mid","Midi musical files");


}



void pad::midi()
{


struct CHUNK_HEADER
{
   char           ID[4];
   unsigned long  Length; 
};


struct MTHD_CHUNK
{
   //Here's the 8 byte header that all chunks must have 
   char           ID[4];  // This will be 'M','T','h','d' 
   unsigned long  Length; // This will be 6 

   //Here are the 6 bytes 
   unsigned short Format;
   unsigned short NumTracks;
   unsigned short Division;
};


struct MTRK_CHUNK
{
   // Here's the 8 byte header that all chunks must have 
   char           ID[4];   // This will be 'M','T','r','k' 
   unsigned long  Length;  // This will be the actual size of Data[] 

   // Here are the data bytes 
   unsigned char  Data[];  // Its actual size is Data[Length] 
};


//WriteVarLen(ReadVarLen());

}
/*
void WriteVarLen(register unsigned long value)
{
   register unsigned long buffer;
   char* outfile;

   outfile ="text.txt";
   
   buffer = value & 0x7F;

   while ( (value >>= 7) )
   {
     buffer <<= 8;
     buffer |= ((value & 0x7F) | 0x80);
   }

   while (TRUE)
   {
      
	  putc(buffer,outfile);
      if (buffer & 0x80)
          buffer >>= 8;
      else
          break;
   }
}


unsigned long ReadVarLen()
{
    register unsigned long value;
    register unsigned char c;
	
	char* infile;

	infile="intel.mid";

    if ( (value = getc(infile)) & 0x80 )
    {
       value &= 0x7F;
       do
       {
         value = (value << 7) + ((c = getc(infile)) & 0x7F);
       } while (c & 0x80);
    }

    return(value);
}

*/



void pad::openfichier(char type[10], char name[50])
{
	char lfile[100];
	char filename[100];

	if(mxhwnd.openfile(lfile,type,name))
		{
				strcpy(filename,lfile);
	
				ifstream fin(filename,ios::binary);
				//fin.read(char* level,sizeof(level));
				fin.close();
		}
}
