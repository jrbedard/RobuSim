/* 

  Console
	Jean-René Bédard
	  Console pour simulation de ROBUS


*/
#include <math.h>
#include "thehead.h"
#include <stdlib.h>
#include <assert.h>


int delay;
bool begin;
bool the_game;

int bar;

extern float motor_g,motor_d;
extern char * lcd_motg;
extern char * lcd_motd;
extern char * map_name;

extern bool the_game; 
extern float new_angle;

extern bool my_photo;

extern float new_x;
extern float new_y;


extern bool bumper_on;
extern bool trace_on;
extern bool path_on;

extern t;


bool con_text;
char* con_string;





// propriétés initiales

// couleur du font
void MasterConsole::setbkcolor(COLORREF x)
{
	back_color = x;
}



// textcolor
void MasterConsole::settextcolor(COLORREF x)
{
	fore_color = x;
}

// Police
void MasterConsole::setfont(HFONT f)
{
	font = f;
}


// ----->buff
void MasterConsole::concat(char* b)
{
	strcat(buffer, b);
	linescan();
}



//  key---->buff
void MasterConsole::concatkey(int key)
{
	if( do_input != true ) { return; }
 	switch(key)
	{
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
	case VK_ESCAPE:
	case VK_SHIFT:
	case VK_CONTROL:
	case VK_HOME:
		return;
		break;
	case VK_RETURN:
		{
			do_input = false;
			int stop_pos = strlen(buffer);
			char *inputbuffer;
			int   buf_size;
			
			buf_size = strlen(buffer) - start_pos + 5;
			inputbuffer = new char [ buf_size + 1];
			
		
			
			midcopy( buffer, inputbuffer, start_pos, stop_pos);
			proccmd( inputbuffer );
			
			
			
			delete [] inputbuffer;
			inputbuffer = 0;
			linescan();
			
			printf("\n\nROBUS>");
			input();
		
			
			return;
		}
		break;
	case VK_BACK:
		{
			if(the_game == true)
				bar--;
			
			int pos;
			pos = strlen(buffer);
			pos--;
			if( pos < 0 ) { pos = 0; buffer[0] = 0; return; }
			if( pos < start_pos ) { return; }
			buffer[pos] = 0;
			return;
		}
		break;
	case VK_CAPITAL:
		{
			
			if(icaps == true)
			{ 
				icaps = false; 
			} 
			else 
			{
				icaps = true;
			}
		}
		break;
		
	}

 	int len;
	len = strlen(buffer);
	buffer[len] = fixkeys(key,(int)icaps,false);
	len++;
	buffer[len] =0;
}



// cls de l'écran
void MasterConsole::flush()
{
	strcpy( buffer, "");
	buffer[0] = 0;
}




// streaming!
void MasterConsole::printf( const char *str, ... )
{
	if( do_input == true ) { return; } //  si écriture
	va_list list; 
	char* string;
	string = new char [ strlen(str) + 2];
	va_start(list, str); 
	vsprintf((char*)string,str,list);
	va_end(list); 
	concat(string);
	delete [] string;
	string = 0;
}



// data -> console (char)
MasterConsole& MasterConsole::operator<<(char* data)
{
 	concat(data);
	return(*this);
}


// data->buffer (int)
MasterConsole& MasterConsole::operator <<(int data)
{
 	char numeric[10];
	itoa( data,numeric,10);
	concat( numeric );
	return (*this);
}


MasterConsole& MasterConsole::operator<<(long data)
{
  	char numeric[25];
	ltoa(data,numeric,10);
	concat(numeric);
	return (*this);
}

MasterConsole& MasterConsole::operator<<(double data)
{
	char numeric[25];
	gcvt(data,10,numeric);
	concat(numeric);
	return (*this);
}

MasterConsole& MasterConsole::operator <<(short data)
{
 	char numeric[10];
	itoa( (int)data, numeric, 10);
	concat(numeric);
	return (*this);
}


//scan du buffer (vérification 120 car ou moins)
void MasterConsole::bufferscan()
{
	int len = strlen(buffer);
	if( len < 45 ) { return; } 

	char *temp;
	temp = new char [ strlen( buffer ) + 50 ]; 
	int counter = 0;
	int temp_pos = 0;
	bool change = false;

	for( unsigned int i = 0; i < strlen(buffer); i++)
	{
		if( buffer[i] == '\n' )
		{
			counter = 0;
		}
		else
		{
			counter++;
		}

		if( counter >= letter_max )
		{
			temp[temp_pos] = '\n';
			temp_pos++;
			temp[temp_pos] = buffer[i];
			temp_pos++;
			change = true;
		}
		else
		{
			temp[temp_pos] = buffer[i];
			temp_pos++;
		}
	}

	temp[temp_pos] = 0;

	if( change == true )
	{
		strcpy( buffer, temp );
	}

	delete [] temp;
	temp = NULL;
}



//scan pas plus de 18 lignes dans la console
void MasterConsole::linescan()
{
 	int n_count = 0;
	int f_pos = -1;

	for( unsigned int i = 0; i < strlen(buffer); i++)
	{
		if( buffer[i] == '\n' ) 
		{ 
			if(n_count == 0) 
			{ 
				f_pos = i;
			} 

			n_count++;
		}
	}

	if( n_count >= 18 && f_pos != -1 )
	{
		char *temp;
		temp = new char [ strlen( buffer ) + 10 ];
		rightcopy( buffer, temp, f_pos + 1 );
		strcpy( buffer,temp );
		delete [] temp;
		temp = NULL;
	
	}
}


// console ----> char[]
void MasterConsole::input()
{
	do_input = true;
	start_pos = strlen( buffer );
}




// proccess (utilisation de tokens, tokénisation!)
void MasterConsole::proccmd(char *cmd)
{
	if ( strlen(cmd) == 0 ) { return; }

	char *temp;
	temp = new char [ strlen(cmd) + 1 ];
	strcpy( temp,cmd );

	int num = findcharnum(temp);

 
	delete [] temp;
	temp = NULL;

	char *token;
	char seps[] = " ";

   token = strtok( cmd, seps );

   if( token == NULL ) { return; } // rien

   int cur_token = 0;
   char *tokens[500]; // pas plus de 500 tokens


   while( token != NULL )
   {
   	   tokens[cur_token] = token;
	   cur_token++;
       token = strtok( NULL, seps);

   }

   command( tokens, cur_token );

}




// switch des commandes
void MasterConsole::command( char* argv[], int argc )
{
	 
	switch(cmdstrtoint(argv[0]))
	{
	case FLUSH:
		flush();
		break;
	case ABOUT:
		printf("\n RobuSim par Jean-René Bédard 01 128 041");
		
		if(my_photo ==false)
		{
		my_photo=true;
		return;
		}
		
		if(my_photo ==true)
		{
		my_photo=false;
		return;
		}

		
		
		break;
	case TIME:

		time_t ct;
		time(&ct);
		char* t;
		t = ctime(&ct);
		char the_time[200];
		sprintf(the_time, "%s",t);
		printf("\n");
		printf(the_time);

		break;
	
	//le coeur du programme:
	case MOTOR:
	{	
		char dstr[1000];

			if( argc != 3 )
			{
				printf("\n");
				sprintf(dstr,"[syntax error]-the add command must\ntake two numeric arguments ");
				printf(dstr);

				return;
			}
			
			if( motor_g <-100 || motor_g > 100 || motor_d <-100 || motor_d > 100)
			{
				motor_d=0;
				motor_g=0;
				printf("\nmust be between -100 and 100 '/,");
				return;
			}
			
			motor_g = atol( argv[1] );
			motor_d = atol( argv[2] );
			lcd_motg=argv[1] ;
			lcd_motd=argv[2] ;
			
			t=0;
			printf("\n");
			sprintf(dstr,"left motor = %s '/, \nright motor = %s '/,",argv[1],argv[2]);
			printf(dstr);
		
		
	}
		break;
	
	case GAME:
		the_game = true;
		
		break;
	
	
	case HAPPY:

		printf(" \n hE hE hE..you are prEEtty happy hE hE hE");

		break;
	case ADD:
		{
			char dstr[1000];

			if( argc != 3 )
			{
				printf("\n");
				sprintf(dstr,"[syntax error]-the add command must\ntake two numeric arguments");
				printf(dstr);
				return;
			}

			long num1, num2, val;
			num1 = atol( argv[1] );
			num2 = atol( argv[2] );
			val = num1 + num2;
			sprintf(dstr,"\n %i + %i = %i",num1,num2,val);
			printf(dstr);

		}
		break;
	
	case ANGLE:
		{
			char dstr[1000];

			if( argc != 2 )
			{
				printf("\n");
				sprintf(dstr,"[syntax error]-the angle command must\ntake one numeric arguments");
				printf(dstr);
				return;
			}

			
			new_angle = atof(argv[1]);
			t=0;

			sprintf(dstr,"\nNew robus angle: %s",argv[1]);
			printf("\n");
			printf(dstr);

		}
		break;
	
	
	
	case SUB:
		{
			char dstr[1000];

			if( argc != 3 )
			{
				printf("\n");
				sprintf(dstr,"[syntax error]-the sub command must\ntake two numeric arguments");
				printf(dstr);
				return;
			
			}

			long num1, num2, val;
			num1 = atol( argv[1] );
			num2 = atol( argv[2] );
			val = num1 - num2;
			sprintf(dstr,"\n %i - %i = %i",num1,num2,val);
			printf(dstr);
		}
		break;

	case MAP:
		{
			char dstr[1000];

			if( argc != 2 )
			{
				printf("\n [syntax error] - the sub command must take\n two numeric arguments ");
				return;
			}

			//long num1, num2, val;
			
			map_name = argv[1];
			//num1 = atol( argv[1] );
			//num2 = atol( argv[2] );
			//val = num1 - num2;
			// en construction ->map editeur intégré
			sprintf(dstr,"\nloading the map!");
			printf(dstr);
			//map.loadmap();
		}
		break;


	case MUL:
		{
			char dstr[1000];

			if( argc != 3 )
			{
				printf("\n");
				sprintf(dstr,"[syntax error]-the mul command must\ntake two numeric arguments");
				printf(dstr);
				return;
			}

			long num1, num2, val;
			num1 = atol( argv[1] );
			num2 = atol( argv[2] );
			val = num1 * num2;
			sprintf(dstr,"\n %i * %i = %i",num1,num2,val);
			printf(dstr);
		}
		break;
	case DIV:
		{
			char dstr[1000];

			if( argc != 3 )
			{
				printf("\n");
				sprintf(dstr,"[syntax error]-the mul command must\ntake two numeric arguments");
				printf(dstr);
				return;
			}

			long num1, num2, val;
			num1 = atol( argv[1] );
			num2 = atol( argv[2] );

			if( num1 == 0 || num2 == 0 ) 
			{
				printf("\n [syntax error] - the div commad cannot divide by zero ");
				return;
			}

			val = num1 / num2;
			sprintf(dstr,"\n %i / %i = %i",num1,num2,val);
			printf(dstr);
		}
		break;
	case DUMP:
		{
			char dstr[1000];

			if ( argc != 2 )
			{
				printf("\n [syntax error] - the dump command takes one argument the filename to write the data to");
				return;
			}

			ofstream fout( argv[1] );
			fout << buffer << endl;
			fout.close();
			sprintf(dstr,"\n succesful dump of the buffer into the file %s",argv[1]);
			printf(dstr);
		}
		break;
	
	case PUT:
		{
			char dstr[1000];

			if ( argc != 3 )
			{
				printf("\n");
				sprintf(dstr,"[syntax error]-the put command must\ntake two numeric arguments");
				printf(dstr);
				return;
			}

		    
			new_x = atof( argv[1] );
			new_y = atof( argv[2] );
			
		
		}
		break;
	
	case EXIT:
		{
			printf("\ngoodbye!!!");
			mxhwnd.Kill();
		}
		break;
	
	case TRACE:
		{
			if(trace_on==true)
			{
				printf("\nTracing [off]");
				trace_on=false;
				return;
			}
			
			if(trace_on==false)
			{
				printf("\nTracing [on]");
				trace_on = true;
				return;
			}	
		}
		break;
	
	case PATH:
		{
			if(path_on==true)
			{
				printf("\nPath finder [off]");
				path_on=false;
				return;
			}
			
			if(path_on==false)
			{
				printf("\nPath finder [on]");
				printf("\nstart to begin");
				path_on = true;
				return;
			}	
		}
		break;


	case BUMPER:
		{
			if(bumper_on==true)
			{
				printf("\nBumpers [off]");
				bumper_on=false;
				return;
			}
			
			if(bumper_on==false)
			{
				printf("\nBumpers [on]");
				bumper_on = true;
				return;
			}	
		}
		break;
	
	case FCOLOR:
		{
			if( argc != 2 )
			{
				printf("\n[syntax error] - the set color command requires one argument");
				return;
			}

			int color = cmdstrtocolor(argv[1]);
			if( color == -1 )
			{
				printf("\n[syntax error] - the set color command requires a valid color");
				return;
			}

			fore_color = color;
		}
		break;
	case BCOLOR:
		{
			if( argc != 2 )
			{
				printf("\n[syntax error] - the set color command requires one argument");
				return;
			}

			int color = cmdstrtocolor(argv[1]);
			if( color == -1 )
			{
				printf("\n[syntax error] - the set color command requires a valid color");
				return;
			}

			back_color = color;

		}
		break;
	
	case HELP:
		{
				char dstr[1000];
				printf("\n");
				sprintf(dstr,"[help] v1.0b\nabout              [information]\nmotor  g  d          [set motors]\nput  x  y            [place robot coord]");
				printf(dstr);
				sprintf(dstr,"\nangle  a            [new robot angle]\nflush              [clear the buffer]\npath               [follow a path]");
				printf(dstr);
				return;
			
		}
		break;
	
	
	// commande inconnue
	case -1:
		printf("\n - unknown command");
		break;
	}
}

int  MasterConsole::cmdstrtoint( char* data )
{
	if( strcmp( data, "flush") == 0 )
	{
		return 0x1;
	}

	if( strcmp( data, "about") == 0 )
	{
		return 0x2;
	}

	if( strcmp( data, "time") == 0 )
	{
		return 0x3;
	}

	if( strcmp( data, ":)") == 0 )
	{
		return 0x4;
	}

	if( strcmp( data, "add") == 0 )
	{
		return 0x5;
	}

	if( strcmp( data, "sub") == 0 )
	{
		return 0x6;
	}

	if( strcmp( data, "mul") == 0 )
	{
		return 0x7;

	}

	if( strcmp( data, "div") == 0 )
	{
		return 0x8;
	}

	if( strcmp( data, "dump") == 0)
	{
		return 0x9;
	}

	if( strcmp( data,"exit") == 0)
	{
		return 0x10;
	}

	if( strcmp( data,"motor") == 0)
	{
		return 0x13;
	}

	if( strcmp( data,"load") == 0)
	{
		return 0x14;
	}
	
	if( strcmp( data,"settextcolor") == 0)
	{
		return 0x11;
	}

	if( strcmp(data, "setbkcolor") == 0 )
	{
		return 0x12;
	}

	if( strcmp( data,"help") == 0)
	{
		return 0x15;
	}

	if( strcmp(data, "command") == 0 )
	{
		return 0x15;
	}
	
	if( strcmp(data, "wtf") == 0 )
	{
		return 0x15;
	}
	if( strcmp(data, "game") == 0 )
	{
		return 0x16;
	}
	
	if( strcmp(data, "angle") == 0 )
	{
		return 0x17;
	}
	
	if( strcmp(data, "bumper") == 0 )
	{
		return 0x18;
	}
	if( strcmp(data, "stop") == 0 )
	{
		return 0x19;
	}
	
	if( strcmp(data, "reset") == 0 )
	{
		return 0x20;
	}
	if( strcmp(data, "start") == 0 )
	{
		return 0x21;
	}
	if( strcmp(data, "put") == 0 )
	{
		return 0x22;
	}
	if( strcmp(data, "trace") == 0 )
	{
		return 0x23;
	}
	if( strcmp(data, "path") == 0 )
	{
		return 0x24;
	}
	return -1;
}

int MasterConsole::cmdstrtocolor(char *data )
{
	if( strcmp(data,"white")  == 0)
	{
		return RGB(255,255,255);
	}

	if( strcmp(data,"red")  == 0)
	{
		return RGB(255,0,0);
	}

	if( strcmp(data,"blue") == 0 )
	{
		return RGB(0,0,255);
	}

	if( strcmp(data,"green") == 0)
	{
		return RGB(0,200,0);
	}

	if(strcmp(data,"black") == 0)
	{
		return RGB(0,0,0);
	}
	
	return -1;
}


//test
void MasterConsole::con_t()
{

if(con_text==false)
{
printf("\nsfsd");

con_text=false;
}

}

