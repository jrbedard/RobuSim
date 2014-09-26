/* 

 msMAIN 
 
   Win32 API call
   initialisation DirectDraw
   Parametres Update/initialisation

  Jean-René Bédard

 ------septembre/octobre----->2001


*/


#include "thehead.h"






// instance 
MasterXHWND mxhwnd;
// instance pour la console
MasterConsole console;
map map;
robus robus;
automatic automatic;
lcd lcd;
menu menu;
pad pad;
kid kid;
captor captor;

//variables externes
extern bool lb_down,rb_down;;
extern bool menu_g,menu_d;

extern bool pad_g,pad_d;
extern bool cap_g,cap_d;
extern bool map_g,map_d;

inline void load();

// entrée principale
int XSTD MasterMain(HINSTANCE hInst,LPSTR line)
{

	if( mxhwnd.CreateMasterX( "RobuSim version 1.2",1024,768,COLOR_DEFAULT,event,hInst,NULL,NULL) )
	{
		
		mxhwnd.InitTimer(1,50); // pour win 2k

		mxhwnd.text.printtext("loading RobuSIM...",400,300);
		load();
		return (mxhwnd.InitLoop(render));
	}

	return (0);
}

// initilisation du programme 
inline void load()
{
	mxhwnd.text.printtext("loading RobuSIM...",400,300);
	pad.load();
	menu.load();
	lcd.load();
	map.load();
	robus.load();
	automatic.load();
	kid.load();
	captor.load();

	console.load();
	console << "************* RobuSim **************\n Pseudo-Emulation de Robus \n---Université de Sherbrooke---\n\nJean-René Bédard \n\n";
	console << "ROBUS> ";
	console.input();
}


// callback des event
long XSTD event(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		
		break;
	case WM_ACTIVATEAPP:
		mxhwnd.activeapp = wParam;
		break;
	case WM_LBUTTONDOWN:
		lb_down=true;
		menu_g=true;
		pad_g=true;
		cap_g=true;
		map_g=true;
		break;
	case WM_LBUTTONUP:
		lb_down=false;
		menu_g=false;
		pad_g=false;
		cap_g=false;
		map_g=false;
		break;
	case WM_RBUTTONDOWN:
		rb_down=true;
		menu_d=true;
		pad_d=true;
		cap_d=true;
		map_d=true;
		break;
	case WM_RBUTTONUP:
		rb_down=false;
		menu_d=false;
		pad_d=false;
		cap_d=false;
		map_d=true;
		break;
	case WM_KEYDOWN:
		{

#ifdef iDEBUG
			if( wParam == VK_ESCAPE ) 
			{
				mxhwnd.Kill(); 
			
			}
#endif
			console.keypress(wParam);
		}
		break;
	default: return DefWindowProc(hwnd,msg,wParam,lParam);
	}

	return (0);
}




// rendu de l'écran---main rendering
void render( MASTERSCREEN screen )
{
	

//	mxhwnd.processgfx();
	
	
	map.update();
	
	
	lcd.update();
	robus.update();
	captor.update();
	
	console.update();
	
	kid.update();
	automatic.update();
	

	pad.update();
	menu.update();
}































void midcopy(const char *string, char *out, int start, int stop)
{
	int i;
	int p = 0;

	for(i = start; i < stop; i++)
	{
		out[p] = string[i];
		p++;
	}

	out[p] =0;
}



void rightcopy(const char *string, char *out, int pos)
{
	int len = strlen(string);
	int i;
	int p = 0;

	for(i = pos; i < len; i++)
	{
		out[p] = string[i];
		p++;
	}

	out[p] = 0;
}






// combien de caractères dans un tableau?
int  findcharnum(char* string)
{
	char seps[] = " ";
	char *token;
	int count = 0;

	 /* premier token: */
   token = strtok( string, seps );
   while( token != NULL )
   {
   
      token = strtok( NULL, seps );
	  count ++;

   }

   return count;
}






bool isnum(char* data)
{
	for( unsigned int i = 0; i < strlen(data); i++)
	{
		if( data[i] != '0' && data[i] != '1' && data[i] != '2' && data[i] == '3' && data[i] != '4' && data[i] != '5' && data[i] != '6' && data[i] != '7' && data[i] != '8' && data[i] != '9' )
		{
			return false;
		}
	}

	return true;
}















// corriger les commandes
char fixkeys(int key,int caps,bool cmd)
{
	
	char cur;	
	int shift;
	shift = 0;

	if(GetAsyncKeyState(16))
	{
		shift = 1;
		if(caps)
		{
			cur = tolower(key);
		}
		else
		{
			cur = toupper(key);
		}
		// si appui sur shift
		
		switch(cur)
		{
		case '0':
			cur = ')';
			break;
		case '1':
			cur = '!';
			break;
		case '2':
			cur = '@';
			break;
		case '3':
			cur = '#';
			break;
		case '4':
			cur = '$';
			break;
		case '5':
			cur = '%';
			break;
		case '6':
			cur = '^';
			break;
		case '7':
			cur = '&';
			break;
		case '8':
			cur = '*';
			break;
		case '9':
			cur = '(';
			break;
		}
	}
	else
	{
		
		if(caps)
		{
			cur =  toupper(key);
		}
		else
		{
			cur = tolower(key);
		}
	}
	
	// special char
	switch(key)
	{
	case 220:
		
		cur = '\\';
		
		break;
		
	case 191:
		if(shift)
		{
			cur = '?';
		}
		else
		{
			
			cur = '/';
		}
		
		break;
		
	case 192:
		
		cur = 'µ';
		break;
		
	case 186:
		if(shift)
		{
			cur = ':';
		}
		else
		{
			cur = ';';
		}
		break;
		
	case 219:
		
		if(shift)
		{
			if(cmd == false)
			{
				cur = '{';
			}
			else
			{
                cur = '/';
			}
		}
		else
		{
			cur = '[';
		}
		
		break;
	case 221:
		if(shift)
		{
			if(cmd == false)
			{
				cur = '}';
			}
			else
			{
                cur = '\\';
			}
		}
		else
		{
			cur = ']';
		}
		
		break;
	case 187:
		if(shift)
		{
			cur = '+';
		}
		else
		{
			cur = '=';
		}
		break;
	case 189:
		
		if(shift)
		{
			cur = '_';
		}
		else
		{
			cur = '-';
		}
		
		break;
	case 188:
		
		if(shift)
		{
			cur = '<';
		}
		else
		{
			cur = ',';
		}
		
		break;
		
	case 190:
		
		if(shift)
		{
			cur = '>';
		}
		else
		{
			cur = '.';
		}
		
		
		break;
	case 222:
		if(shift)
		{
			cur = '\"';
			break;
		}
		else
		{
			cur = '\'';
			break;
		}
		
		
		
		break;
		
		}
		
		return cur;
}

