/* 

	Header principal
	
	Jean-René Bédard 01 128 041

	-Déclaration des classes / structures 
	-fonctions/struct/variables
*/


//environnement
#define LevelSize 2813




#include "SDH.h"
#include <time.h>
#include <fstream.h>
#define iDEBUG  //debug
#define BUFFER_SIZE 5000

// commandes console

enum {  FLUSH = 0x1, ABOUT = 0x2 , TIME = 0x3, HAPPY = 0x4, ADD = 0x5,
		SUB = 0x6, MUL = 0x7, DIV = 0x8 , DUMP = 0x9, EXIT = 0x10, 
		FCOLOR = 0x11, BCOLOR = 0x12, MOTOR = 0x13, MAP = 0x14, 
		HELP = 0x15, GAME = 0x16, ANGLE = 0x17, BUMPER= 0x18,
		STOP = 0x19, RESET = 0x20, START = 0x21, PUT = 0x22,
		TRACE = 0x23,PATH=0x24};

extern MasterXHWND mxhwnd;


//  prototypes
LRESULT APIENTRY event(HWND,UINT,WPARAM,LPARAM);
void render(MASTERSCREEN);

char fixkeys(int key,int caps,bool cmd);
void midcopy(const char *string, char *out, int start, int stop);
void rightcopy(const char *string, char *out, int pos);
int  findcharnum(char* data);
bool isnum(char* data);


// CONSOLE
class MasterConsole : public MasterGameObject {
	
private:
	
	char buffer[ BUFFER_SIZE ];
	COLORREF back_color;
	COLORREF fore_color;
	HFONT    font;
	bool  icaps;
	int   letter_max;
	bool  do_input;
	int   start_pos;
	int timer;
	
	int counter;
	int num_pos;

	bool say;

	int sound_buffer[101];
	
	void concat(char*);
	void concatkey(int key);
	void bufferscan();
	void linescan();
	void con_t();

public:
	

	virtual void load()
	{
		flush();
		font = MakeFont("Arial",15);
		back_color = RGB( 0,0,0 );
		fore_color = RGB( 30,250,36 );
		letter_max = 45;
		do_input = false;
		start_pos = 10;
	
	}
	
	//update general/consle
	virtual void update()
	{
	
		bufferscan();
		mxhwnd.text.setbkcolor(  back_color );
		if( mxhwnd.text.font != font ) 
		{
			mxhwnd.text.setfont( font );
		}
		mxhwnd.text.settextcolor( fore_color );
			
		if( do_input == false )
		{

	

		mxhwnd.paint.mxdrawrect(20,35,271,330,back_color,back_color);
		mxhwnd.text.printtextrectf( 20,30,271,330, buffer );


		}
		else
		{
			char* b;
			b = new char [ strlen(buffer) + 10 ];
			strcpy( b, buffer );
			if( rand()%10 > 5 )
			{
				strcat(b,"_");
			}
			mxhwnd.text.printtextrectf(20,30,271,330,b);
			delete [] b;
			b = NULL;
		}
	}
	
	//press
	inline void keypress(WPARAM key)
	{
		if( do_input == true )
		concatkey(key);
	}
	
	



	void flush();

	void printf( const char*, ...);

	void settextcolor(COLORREF);

	void setbkcolor(COLORREF);

	void setfont(HFONT font);

	void input();

	void proccmd(char *cmd);
	void command(char* argv[], int argc);
	int  cmdstrtoint( char* data );
	int  cmdstrtocolor(char *data );


	

	MasterConsole& operator<<(char*);
	MasterConsole& operator<<(int);
	MasterConsole& operator<<(long);
	MasterConsole& operator<<(double);
	MasterConsole& operator<<(short);

};


struct LevelStructure
{
	int level_block[LevelSize];

	inline LevelStructure()
	{
		for(int i = 0; i < LevelSize; i++)
		{
			level_block[i] = 0;
		}
	}
};







//map murs et plachers
struct map
{
LevelStructure level;
MasterGraphic tiles[16];		
MasterGraphic plant1;
MasterGraphic maped;

void load();
void update();
void draw();
void loadmap();
void drawmap();
void drawing();
void leftscro();
void sky();
void mapeditor();
int getblockbypos(int x, int y);
void drag();

int start_x;
int start_y; 
int i;
int sx;
int sy;
int clouds;
int clouds2;

};


//robus deplacement/pannels/simulation
struct robus
{
MasterGraphic robus_p;
	

void load();
void update();
void direction();
void draw();
void move();

void limit();

void lcd();
void hoover();


void ai();
void exept();

struct bot
{
float robus_x;
float robus_y;
float robus_angle;
int robus_bumper;
};

bot bot;
};


// path /menu
struct automatic
{
MasterGraphic explosion;
MasterGraphic pushed;
	
void load();
void update();
void bumper();
void draw();

void ai();
void menu();
void reset();

void tang();
void pathfinder();

};


struct lcd
{
MasterGraphic handy;

void load();
void update();

void draw();

};



struct menu
{
MasterGraphic gui;

void load();
void update();

void menuA();

};


struct pad
{
MasterGraphic logpad;
MasterGraphic musicpad;
	
void load();
void update();
void draw();
void text();
void button();
void winamp();
void midi();
void openfichier(char type[10], char name[50]);
//void WriteVarLen(register unsigned long value);
//unsigned long ReadVarLen();
};


struct kid
{
void load();
void update();
void draw();

};



struct captor
{
MasterGraphic sim;
MasterGraphic jrb;
MasterGraphic robusim;
MasterGraphic tab;
MasterGraphic electro;

void load();
void update();
void draw();
void position();
void grid();
void panel();
void digital();
void choice();

void bumper();
void ir();
void pyro();
void polaroid();
void gpd();
void usound();

};


struct opensave
{
void load();
void update();
void draw();
};





LevelStructure* GetLevelStructure();

static HFONT digital_font = CreateFont(17,0,0,0,FW_DONTCARE,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,"Quartz");
static HFONT d_font = CreateFont(13,0,0,0,FW_DONTCARE,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,"Quartz");
static HFONT lcd_font = CreateFont(16,0,0,0,FW_DONTCARE,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,"Quartz");
