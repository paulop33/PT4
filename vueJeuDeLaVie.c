#include "vueJeuDeLaVie.h"
#include "SDL.h"
#include "SDL_image.h"
#include <string.h>

/*using namespace std;*/

const int SCREEN_WIDTH=742;//largeur de la fenêtre
const int SCREEN_HEIGHT=548;//hauteur de la fenêtre
const int SCREEN_BPP=32;// bits par pixel

/*SDL_Surface *
loadImage(string filename) 
{
     return SDL_DisplayFormat(IMG_Load( filename.c_str()));
}


SDL_Surface *
loadImageWithColorKey(string filename, int r, int g, int b) 
{
    SDL_Surface* optimizedImage;
    optimizedImage = SDL_DisplayFormat(IMG_Load( filename.c_str()));
    SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, 
		    SDL_MapRGB(optimizedImage->format, r, g, b));
    return optimizedImage;
}*/


void applySurface(int x, int y, SDL_Surface* source, 
	     SDL_Surface* destination, SDL_Rect* clip)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, clip, destination, &offset );
}

void afficherVue(Plateau p){
	 bool quit=false;

  SDL_Surface *screen, *plateau; /*, *fondmenu, *fondjeu, *plateau, *flag, *pirates, *button*/
  SDL_Event event;  
  
  SDL_Init(SDL_INIT_EVERYTHING);
   SDL_WM_SetCaption("Jeu de la vie", NULL);//nom de la fenetre
  screen= SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_SWSURFACE);
  
  //plateau
  plateau= SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_WIDTH-40, SCREEN_HEIGHT-40, SCREEN_BPP, 0, 0, 0, 0);
  SDL_FillRect(plateau, NULL, SDL_MapRGB(plateau->format, 255, 255, 255));

  SDL_Rect posPlateau;
	posPlateau.x = 20;
	posPlateau.y = 20;
  
  SDL_BlitSurface(plateau, NULL, screen, &posPlateau);

  
 /* fondmenu=loadImage("menu.bmp"); 
  fondjeu=loadImage("blanc.bmp"); 
  plateau=loadImage("fond.JPG");
  flag=loadImage("flag.bmp");
  pirates=loadImageWithColorKey("piratesSprite2.bmp",0,255,255);
  button=loadImageWithColorKey("bouttons.bmp",0,255,255);*/
  

  
  int etape=1;
  while(!quit)
    {

      SDL_Flip(screen);
     // SDL_Flip(plateau);
      

      while(SDL_PollEvent(&event))
	if(event.type==SDL_QUIT)
	    quit=true;
      
      SDL_Delay(100);
    }

  SDL_FreeSurface(screen);
  SDL_FreeSurface(plateau);
  SDL_Quit();
	
}

int main(){
	
	Plateau p;
	
	afficherVue(p);
  return EXIT_SUCCESS;    
	
}
//gcc -o Vue vueJeuDeLaVie.c -I/usr/local/include -lSDL -ldl -lpthread
//g++ -I/usr/include/SDL -L/usr/lib -lSDLmain -lSDL -lSDL_image -lSDL_ttf vueJeuDeLaVie.c -o Vue
