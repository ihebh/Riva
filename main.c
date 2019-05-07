#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef struct
{
	int status;
	float x,y;
	float vx,vy;
	SDL_Surface *image;
} Sprite;

void InitSprite(Sprite* Sp)
{
	Sp->x = 0;
	Sp->y = 220.0f;
	Sp->status = 0;
	Sp->vx = Sp->vy = 0.0f;
}

void Render(Sprite* Sp,SDL_Surface* screen)
{
	SDL_Rect R;
	R.x = (Sint16)Sp->x;
	R.y = (Sint16)Sp->y;
	//R.w = 20;
	//R.h = 40;

	Sp->image=IMG_Load("1c.png");
	SDL_BlitSurface(Sp->image,NULL,screen,&R);

}

void Saute(Sprite* Sp,float impulsion)
{
	Sp->vy = -impulsion;
	Sp->status = 1;
}

void ControleSol(Sprite* Sp)
{
	if (Sp->y>260)
	{
		Sp->y = 260;
		if (Sp->vy>0)
			Sp->vy = 0.0f;
		Sp->status = 0;
	}
}

void Gravite(Sprite* Sp,float factgravite,float factsautmaintenu,Uint8* keys)
 {
	if (Sp->status == 1 && keys[SDLK_UP])
		factgravite/=factsautmaintenu;
	Sp->vy += factgravite;
}

void Evolue(Sprite* Sp,Uint8* keys)
{
	float lateralspeed = 0.5f;
	float airlateralspeedfacteur = 1.0f;
	float maxhspeed = 3.0f;
	float adherance = 3.0f;
	float impulsion = 6.0f;
	float factgravite = 0.5f;
	float factsautmaintenu = 3.0f;
// controle lateral
	if (Sp->status == 1) // (*2)
		lateralspeed*= airlateralspeedfacteur;
	if (keys[SDLK_LEFT])
	{Sp->vx-=lateralspeed;
	 if (Sp->vx>maxhspeed) // (*4)
		{Sp->vx = maxhspeed;}
	  if (Sp->vx<-maxhspeed)
		{Sp->vx = -maxhspeed;}
	}
	if (keys[SDLK_LEFT]&&(keys[SDLK_SPACE])&&(Sp->y==260)) // (*1)
		Sp->vx-=5;
	if (keys[SDLK_RIGHT]  )
		{Sp->vx+=lateralspeed;
		if (Sp->vx>maxhspeed) // (*4)
		{Sp->vx = maxhspeed;}
	  if (Sp->vx<-maxhspeed)
		{Sp->vx = -maxhspeed;}}
	if (keys[SDLK_RIGHT] &&(keys[SDLK_SPACE])&&(Sp->y==260) )
		{Sp->vx+=5;}
        
             


	if (Sp->status == 0 && !keys[SDLK_LEFT] && !keys[SDLK_RIGHT]) // (*3)
		Sp->vx/=adherance;

	if (keys[SDLK_UP] && Sp->status == 0)
		Saute(Sp,impulsion);
	Gravite(Sp,factgravite,factsautmaintenu,keys);
	ControleSol(Sp);
// application du vecteur à la position.
	Sp->x +=Sp->vx;
	Sp->y +=Sp->vy;
}

int main(int argc,char** argv)
{
	SDL_Surface* screen , *fond ;
	SDL_Rect pos;
	int numkeys;
	Uint8 * keys;
	Sprite S;
	Uint32 timer,elapsed;
	SDL_Init(SDL_INIT_VIDEO);
	screen=SDL_SetVideoMode(1000, 390, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	fond=IMG_Load("55802425_2113073908778379_3648462241348452352_n.png");
	InitSprite(&S);
	pos.x=0;
	pos.y=0;
	do
	{
		timer = SDL_GetTicks();
		SDL_BlitSurface(fond,NULL,screen,&pos);
		SDL_PumpEvents();
		keys = SDL_GetKeyState(&numkeys);
		//printf ("%d\n",keys);
		Evolue(&S,keys);
		Render(&S,screen);
		SDL_Flip(screen);
		elapsed = SDL_GetTicks() - timer;
		if (elapsed<20)
		SDL_Delay(20-elapsed);
	} while (!keys[SDLK_ESCAPE]);
	SDL_Quit();
	return 0;
}
