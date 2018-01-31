/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hge_tut01 - Minimal HGE application
*/


//#include "hge/include/hge.h"//��ĿĿ¼�²��Ҹ��ļ����Ҳ�����ϵͳĿ¼�²��ң����Ҳ�������(�Լ�д��ͷ�ļ�)
#include<hge.h>//ϵͳĿ¼�²��Ҹ��ļ����Ҳ���ֱ�ӱ���ϵͳ�ģ�.
#include <hgesprite.h>
#include <list>
#include <hgerect.h>
#include <hgefont.h>
#include <hgeparticle.h>
using namespace std;
HGE *hge = 0;//����һ��hge�����ָ��;
hgeSprite *spr=NULL;//����ָ��(����)
HTEXTURE tex;//������(����)
float y3=0,y2=-852;//���ű�������������

//��ʼ����
hgeSprite *sprstar=NULL;
HTEXTURE texstar;
hgeSprite *sprbutton1=NULL;
HTEXTURE texbutton1;
hgeSprite *sprbutton2=NULL;
HTEXTURE texbutton2;
hgeSprite *sprmusicplay=NULL;
HTEXTURE texmusicplay;
hgeSprite *sprmusicclose=NULL;
HTEXTURE texmusicclose;
enum Game_state{Game,Start,Restart,Over} game_state=Start;

//��ͣ����
hgeSprite *sprbutton3=NULL;
HTEXTURE texbutton3;
hgeSprite *sprbutton4=NULL;
HTEXTURE texbutton4;



hgeSprite *sprplane=NULL;//����ָ��(�ɻ�)
HTEXTURE texplane;//������(�ɻ�)
hgeSprite *sprplane1=NULL;//����ָ��(�ɻ�)
HTEXTURE texplane1;//������(�ɻ�)
float x=190,y=720;
bool Anistate=true;

hgeSprite *sprenemy1=NULL;//����ָ��(�л�1)
HTEXTURE texenemy1;//������(�л�1)

enum Enemy1_state{no,hit,down} enemy1_state=no;
enum Enemy{enemy1,enemy2,enemy3} enemy;
struct Enemy1
{   Enemy1_state enemy1_state;
	float x_enemy1;
	float y_enemy1;
	Enemy enemy;
};
list<Enemy1> lEnemy1;


hgeSprite *sprenemy2=NULL;//����ָ��(�л�2)
HTEXTURE texenemy2;//������(�л�2)

hgeSprite *sprenemy3=NULL;//����ָ��(�л�3)
HTEXTURE texenemy3;//������(�л�3)


hgeSprite *sprbullet=NULL;//����ָ��(�ӵ�)
HTEXTURE texbullet;//������(�ӵ�)
struct Bullet
{

	float x_bullet;
	float y_bullet;
};

list<Bullet> lBullet;

hgeSprite *sprstop=NULL;//����ָ��(��ͣ��ť)
HTEXTURE texstop;//������(��ͣ)
enum Stop_state{stop,play} stop_state=play;


HEFFECT game_snd;//��������
HEFFECT bullet_snd;
HEFFECT down_snd;
HEFFECT button_snd;
HEFFECT bomb_snd;
HEFFECT over_snd;
enum Music_state{Play,Close} music_state=Play;
//score
hgeFont* fnt;
int score=0;
//over
hgeFont* fntover;


hgeSprite *spr_particle1=NULL;//����ָ��(����Ч��)
HTEXTURE tex_particle;//������(����Ч��)
hgeParticleSystem*	par1=NULL;//����ϵͳ

hgeSprite *spr_particle2=NULL;//����ָ��(����Ч��)
hgeParticleSystem*	par2=NULL;//����ϵͳ



bool Judge_down(float x,float y)
{
	if(!lEnemy1.empty())
	{
		list<Enemy1>::iterator it_enemy1=lEnemy1.begin();
	for(;it_enemy1!=lEnemy1.end();it_enemy1++)
	{
		hgeRect rect_bullet,rect_enemy1;
		sprbullet->GetBoundingBox(x,y,&rect_bullet);	
		sprenemy1->GetBoundingBox(it_enemy1->x_enemy1,it_enemy1->y_enemy1,&rect_enemy1);
		if(rect_bullet.Intersect(&rect_enemy1))
		{
			
				
				it_enemy1->y_enemy1=852;
				it_enemy1->enemy1_state=down;
				par1->Fire();
				return true;
				//break;
		}
	}
		
		
	}
	return false;
}

bool Judge_hit(float x,float y)
{
	if(!lEnemy1.empty())
		{
		list<Enemy1>::iterator it_enemy1=lEnemy1.begin();
	for(;it_enemy1!=lEnemy1.end();it_enemy1++)
	{
		hgeRect rect_plane,rect_enemy1;
		sprplane->GetBoundingBox(x,y,&rect_plane);	
		sprenemy1->GetBoundingBox(it_enemy1->x_enemy1,it_enemy1->y_enemy1,&rect_enemy1);
		if(rect_plane.Intersect(&rect_enemy1))
		{
			it_enemy1->enemy1_state=hit;
			return true;
		}
		}
		
	}
	return false;
}
//�ж��Ƿ���
void FrameMusic()
{
	float x=0,y=0;
	hge->Input_GetMousePos(&x,&y);
	if(hge->Input_KeyDown(HGEK_LBUTTON)&&x>=430&&x<=430+48&&y>=700&&y<=700+48&&music_state==Play)
	{	
		hge->Effect_Play(button_snd);
		music_state=Close;
		hge->System_SetState(HGE_USESOUND, false);
	}
	else if(hge->Input_KeyDown(HGEK_LBUTTON)&&x>=430&&x<=430+48&&y>=700&&y<=700+48&&music_state==Close)
	{
	
		
		music_state=Play;
		hge->System_SetState(HGE_USESOUND,true);
		bullet_snd=hge->Effect_Load("AirplaneResource/sound/bullet.mp3");
		game_snd=hge->Effect_Load("AirplaneResource/sound/game_music.mp3");
		down_snd=hge->Effect_Load("AirplaneResource/sound/enemy1_down.mp3");
		button_snd=hge->Effect_Load("AirplaneResource/sound/button.mp3");
		bomb_snd=hge->Effect_Load("AirplaneResource/sound/use_bomb.mp3");
		over_snd=hge->Effect_Load("AirplaneResource/sound/game_over.mp3");
	}
}

void FrameStart()
{
	
	float x=0,y=0;
	hge->Input_GetMousePos(&x,&y);
	if(hge->Input_KeyDown(HGEK_LBUTTON)&&x>=100&&x<=100+300&&y>=400&&y<=500+41)
	{	
		hge->Effect_Play(button_snd);
		game_state=Game;
	}
	if(hge->Input_KeyDown(HGEK_LBUTTON)&&x>=100&&x<=100+300&&y>=500&&y<=500+41)
	{	
		hge->Effect_Play(button_snd);
		exit(0);
	}

}

void FrameGame()
{
	//�ı䱳�����Ƶ�y����
	y3++;
	
	y2++;
	if(y3>=852)
		y3=0;
	if(y2>=0)
		y2=-852;
	
	if (hge->Input_GetKeyState(HGEK_RIGHT))
	if(x<=380)
		x+=4;
	if(hge->Input_GetKeyState(HGEK_LEFT))
    if(x>=0)
		x-=4;
	if(hge->Input_GetKeyState(HGEK_UP))
        if(y>0)
		y-=4;
	if(hge->Input_GetKeyState(HGEK_DOWN))
        if(y<726)
		y+=4;

	static float begintime=hge->Timer_GetTime();
	float endtime=hge->Timer_GetTime();

	if(endtime-begintime>=0.2)
	{
		if(Anistate)
			Anistate=false;
		else
			Anistate=true;

	}	

	if(endtime-begintime>=0.2)
	{
		Bullet bullet;
		bullet.x_bullet=x+50;
		bullet.y_bullet=y-2;
		lBullet.push_back(bullet);
		hge->Effect_Play(bullet_snd);
		begintime=endtime;
	}

	static float begintime_enemy1=hge->Timer_GetTime();
	float endtime_enemy1=hge->Timer_GetTime();
	static float begintime0=0;
	float endtime0=hge->Timer_GetTime();
	int n=0;

	if(endtime_enemy1-begintime_enemy1>=hge->Random_Float(0.5,0.8))
	{
		Enemy1 enemy;
		enemy.enemy=enemy1;
		enemy.x_enemy1=rand()%(480-57);
		enemy.y_enemy1=0;
		lEnemy1.push_back(enemy);
		begintime_enemy1=endtime_enemy1;
	}
	else if(endtime_enemy1-begintime_enemy1>hge->Random_Float(0.5,1))
	{
		Enemy1 enemy;
		if((score+1)%100==1)
			enemy.enemy=enemy3;
		else
			enemy.enemy=enemy2;
		enemy.x_enemy1=rand()%(480-57);
		enemy.y_enemy1=0;
		lEnemy1.push_back(enemy);
		begintime_enemy1=endtime_enemy1;
	}
	/*if(endtime0-begintime0==1000)
	{
		Enemy1 enemy;
		enemy.enemy=enemy3;
		enemy.x_enemy1=190;
		enemy.y_enemy1=0;
		lEnemy1.push_back(enemy);
		
		
	}*/
	
	

	if(!lEnemy1.empty())
	{
		list<Enemy1>::iterator it=lEnemy1.begin();
		for(;it!=lEnemy1.end();it++)
		{
			it->y_enemy1+=3;
			if(Judge_hit(x,y))
			{
				par2->MoveTo(x+51,y+61);
				hge->Effect_Play(bomb_snd);
				hge->Effect_Play(over_snd);
			}
		
				
		}
	}

	if(!lBullet.empty())
	{
		list<Bullet>::iterator it_bullet=lBullet.begin();
		for(;it_bullet!=lBullet.end();)
		{
			it_bullet->y_bullet-=3;	
			if(Judge_down(it_bullet->x_bullet,it_bullet->y_bullet))
			{	
			   	
				it_bullet=lBullet.erase(it_bullet);
				hge->Effect_Play(down_snd);	 
				par1->MoveTo(it_bullet->x_bullet,it_bullet->y_bullet);
				score+=10;

			}
			else
				it_bullet++; 
		}

	}
	float x0=0,y0=0;
	hge->Input_GetMousePos(&x0,&y0);
	if(hge->Input_KeyDown(HGEK_LBUTTON)&&x0>=420&&x0<=420+60&&y0>=650&&y0<=650+45&&stop_state==play)
	{	
		stop_state=stop;
		hge->Effect_Play(button_snd);
	}


	float dt=hge->Timer_GetDelta();
	par1->Update(dt);
	par2->Update(dt);
}

void FrameStop()
{
	float x0=0,y0=0;
	hge->Input_GetMousePos(&x0,&y0);
	if(hge->Input_KeyDown(HGEK_LBUTTON)&&x0>=100&&x0<=100+300&&y0>=300&&y0<=300+41)
	{	
		stop_state=play;
		hge->Effect_Play(button_snd);
		
	}
	if(hge->Input_KeyDown(HGEK_LBUTTON)&&x0>=100&&x0<=100+300&&y0>=400&&y0<=400+41)
	{
		game_state=Restart;
		stop_state=play;
		hge->Effect_Play(button_snd);
		y3=0,y2=-852;
		x=190;y=720;
		score=0;
		lEnemy1.clear();
		lBullet.clear();
	}
	if(hge->Input_KeyDown(HGEK_LBUTTON)&&x0>=100&&x0<=100+300&&y0>=500&&y0<=500+41)
	{	
		exit(0);
		
	}

}

void FrameOver()
{
	float x0=0,y0=0;
	hge->Input_GetMousePos(&x0,&y0);
	if(hge->Input_KeyDown(HGEK_LBUTTON)&&x0>=100&&x0<=100+300&&y0>=400&&y0<=400+41)
	{
		game_state=Restart;
		stop_state=play;
		hge->Effect_Play(button_snd);
		y3=0,y2=-852;
		x=190;y=720;
		score=0;
		lEnemy1.clear();
		lBullet.clear();
	}
	if(hge->Input_KeyDown(HGEK_LBUTTON)&&x0>=100&&x0<=100+300&&y0>=500&&y0<=500+41)
	{	
		exit(0);
		
	}

}

	bool FrameFunc()
{

	//�ж�ESC���Ƿ���
	if (hge->Input_GetKeyState(HGEK_ESCAPE))
		return true;

	switch(game_state)
	{
		case Start:
			FrameStart();
			break;
		case Game:
		    if(stop_state==play) FrameGame();
			if(stop_state==stop) FrameStop();
			break;
		case Restart:
			game_state=Game;
			break;
		case Over:
			FrameOver();
			break;
	}

	FrameMusic();
	if(music_state==Play) hge->Effect_Play(game_snd);
	
	return false;//�˳���Ϸ
}

//��ʼ�������
void RenderStart()
{
	spr->Render(0,0);
	sprstar->Render(20,100);
	sprbutton1->Render(100,400);
	sprbutton2->Render(100,500);
}

void RenderGame()
{

    //ʵ�ʵĻ���
	spr->Render(0,y3);
	spr->Render(0,y2);

	//�ɻ�
	if(Anistate)
		sprplane->Render(x,y);
	else 
		sprplane1->Render(x,y);
    //�ӵ�
	list<Bullet>::iterator it_bullet=lBullet.begin();
	list<Enemy1>::iterator it_enemy1=lEnemy1.begin();
	if(!lBullet.empty())
	{
		
		for (;it_bullet!=lBullet.end();it_bullet++)
		{
			sprbullet->Render(it_bullet->x_bullet,it_bullet->y_bullet);
		}
	}
	static float begintime=hge->Timer_GetTime();
    float endtime=hge->Timer_GetTime();
	static float begintime0=hge->Timer_GetTime();
    float endtime0=hge->Timer_GetTime();
	float i=0;
	//�л�1
	if(!lEnemy1.empty())
	{
		
		for (;it_enemy1!=lEnemy1.end();it_enemy1++)
		{
			if(it_enemy1->enemy1_state==down)
			{

				par1->Render();
				if(endtime-begintime>=0.1)
					par1->Stop();
				
			}
			else if(it_enemy1->enemy1_state==hit)
			{
					game_state=Over;
			}
			else
			{	
				if(it_enemy1->enemy==enemy1)
				{
					sprenemy1->Render(it_enemy1->x_enemy1,it_enemy1->y_enemy1);
					
				}
				else if(it_enemy1->enemy==enemy2)
					sprenemy2->Render(it_enemy1->x_enemy1,it_enemy1->y_enemy1);
				else if(it_enemy1->enemy==enemy3)
					sprenemy3->Render(it_enemy1->x_enemy1,it_enemy1->y_enemy1);

			} 
		}
    
		
	}
	//����
	fnt->printf(190, 5, HGETEXT_LEFT, "score is:%d", score);
	sprstop->Render(420,650);
	
	
}

void RenderStop()
{
	spr->Render(0,0);
	sprbutton3->Render(100,300);
	sprbutton4->Render(100,400);
	sprbutton2->Render(100,500);

}


void RenderOver()
{
    //ʵ�ʵĻ���
	spr->Render(0,y3);
	spr->Render(0,y2);

	//�ɻ�
	sprplane->Render(x,y);

    //�ӵ�
	list<Bullet>::iterator it_bullet=lBullet.begin();
	list<Enemy1>::iterator it_enemy1=lEnemy1.begin();
	if(!lBullet.empty())
	{
		
		for (;it_bullet!=lBullet.end();it_bullet++)
		{
			sprbullet->Render(it_bullet->x_bullet,it_bullet->y_bullet);
		}
	}
	static float begintime=hge->Timer_GetTime();
    float endtime=hge->Timer_GetTime();
	//�л�1
	if(!lEnemy1.empty())
	{
		
		for (;it_enemy1!=lEnemy1.end();it_enemy1++)
		{

			sprenemy1->Render(it_enemy1->x_enemy1,it_enemy1->y_enemy1);
		
		}
    
		
	}
	//����
	fnt->printf(190, 5, HGETEXT_LEFT, "score is:%d", score);
	fnt->printf(250, 300, HGETEXT_CENTER, "Game Over!Your Score is:%d", score);
	sprbutton4->Render(100,400);
	sprbutton2->Render(100,500);
	sprstop->Render(420,650);
	par2->Render();
	
}
//��Ⱦ�ĺ���:ѭ��������ͼ��
bool RenderFunc()
{

	//֪ͨ���濪ʼ����
	hge->Gfx_BeginScene();

	// ������Ĭ���Ǻ�ɫ��
	hge->Gfx_Clear(0);

	switch(game_state) {
	case Start:
		if(stop_state==play) RenderStart();
		break;
	case Game:
		RenderGame();
		if(stop_state==stop) RenderStop();
		break;
	case Over:
		RenderOver();
		break;


	}
		switch(music_state)
	{
	case Play:
		sprmusicplay->Render(430,700);
		break;
	case Close:
		sprmusicclose->Render(430,700);
		break;
	}

 
	//��������
	hge->Gfx_EndScene();

	return false;
}
//��ں�������������
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
 //��hgeָ�븳ֵ��hge����汾��
	hge = hgeCreate(HGE_VERSION);

	// ����֡����  FrameFunc��������
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);

	// ������Ⱦ����  RenderFunc������
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);

	// ���ô��ڵı���
	hge->System_SetState(HGE_TITLE, "�ɻ���ս");
	
	// ����ģʽ��Ĭ���� 800X600
	hge->System_SetState(HGE_WINDOWED, true);
	//���ô��ڵĴ�С
	hge->System_SetState(HGE_SCREENWIDTH,480);
	hge->System_SetState(HGE_SCREENHEIGHT,852);
	
	//�Ƿ��������
	hge->System_SetState(HGE_HIDEMOUSE,false);
	// ��ʹ��bass������
	hge->System_SetState(HGE_USESOUND, true);
	hge->System_SetState(HGE_FPS,100);

	//��ʼ������
	if(hge->System_Initiate())
	{
	//�Լ��ĳ�ʼ��:���磺����ͼƬ
		
		//��ʼ����
     	//���ļ��м���һ��ͼƬ����������������
		texstar=hge->Texture_Load("AirplaneResource/ui/shoot_background/shoot_copyright.png");
		//����һ������(����һ��ͼƬ)
		sprstar=new hgeSprite(texstar,0,0,441,225);
		//���ļ��м���һ��ͼƬ����������������
		texbutton1=hge->Texture_Load("AirplaneResource/game_start.png");
		//����һ������(����һ��ͼƬ)
		sprbutton1=new hgeSprite(texbutton1,0,0,300,41);
		//���ļ��м���һ��ͼƬ����������������
		texbutton2=hge->Texture_Load("AirplaneResource/game_quit.png");
		//����һ������(����һ��ͼƬ)
		sprbutton2=new hgeSprite(texbutton2,0,0,300,41);		
		//���ļ��м���һ��ͼƬ����������������
		texmusicplay=hge->Texture_Load("AirplaneResource/ui/bkmusic_play.png");
		//����һ������(����һ��ͼƬ)
		sprmusicplay=new hgeSprite(texmusicplay,0,0,48,48);
		texmusicclose=hge->Texture_Load("AirplaneResource/ui/bkmusic_close.png");
		//����һ������(����һ��ͼƬ)
		sprmusicclose=new hgeSprite(texmusicclose,0,0,48,48);
		
		
		tex=hge->Texture_Load("AirplaneResource/ui/shoot_background/background.png");
     	//����һ������(����һ��ͼƬ)
		spr=new hgeSprite(tex,0,0,480,852);

		//���ļ��м���һ��ͼƬ���ɻ�������������������
		texplane=hge->Texture_Load("AirplaneResource/ui/shoot/hero1.png");
		//����һ������ɻ�(����һ��ͼƬ)
		sprplane=new hgeSprite(texplane,0,0,102,126);
			
		//���ļ��м���һ��ͼƬ���ɻ�������������������
		texplane1=hge->Texture_Load("AirplaneResource/ui/shoot/hero2.png");
		//����һ������ɻ�(����һ��ͼƬ)
		sprplane1=new hgeSprite(texplane1,0,0,102,126);

		//���ļ��м���һ��ͼƬ���ӵ�������������������
		texbullet=hge->Texture_Load("AirplaneResource/ui/shoot/bullet2.png");
		//����һ�������ӵ�(����һ��ͼƬ)
		sprbullet=new hgeSprite(texbullet,0,0,5,11);
        lBullet.clear();

		//���ļ��м���һ��ͼƬ���л�1������������������
		texenemy1=hge->Texture_Load("AirplaneResource/ui/shoot/enemy1.png");
		//����һ������л�1(����һ��ͼƬ)
		sprenemy1=new hgeSprite(texenemy1,0,0,57,43);
		lEnemy1.clear();

		//���ļ��м���һ��ͼƬ���л�2������������������
		texenemy2=hge->Texture_Load("AirplaneResource/ui/shoot/enemy2.png");
		//����һ������л�1(����һ��ͼƬ)
		sprenemy2=new hgeSprite(texenemy2,0,0,69,99);
		//���ļ��м���һ��ͼƬ���л�1������������������
		texenemy3=hge->Texture_Load("AirplaneResource/ui/shoot/enemy3_n1.png");
		//����һ������л�1(����һ��ͼƬ)
		sprenemy3=new hgeSprite(texenemy3,0,0,169,258);

		//���ļ��м���һ��ͼƬ����ͣ������������������
		texstop=hge->Texture_Load("AirplaneResource/ui/shoot/game_pause_nor.png");
		//����һ��������ͣ(����һ��ͼƬ)
		sprstop=new hgeSprite(texstop,0,0,60,45);
			//���ļ��м���һ��ͼƬ����ͣ������������������
		texbutton3=hge->Texture_Load("AirplaneResource/ui/shoot/game_continue.png");
		//����һ��������ͣ(����һ��ͼƬ)
		sprbutton3=new hgeSprite(texbutton3,0,0,300,41);
			//���ļ��м���һ��ͼƬ����ͣ������������������
		texbutton4=hge->Texture_Load("AirplaneResource/ui/shoot/game_again.png");
		//����һ��������ͣ(����һ��ͼƬ)
		sprbutton4=new hgeSprite(texbutton4,0,0,300,41);

		bullet_snd=hge->Effect_Load("AirplaneResource/sound/bullet.mp3");
		game_snd=hge->Effect_Load("AirplaneResource/sound/game_music.mp3");
		down_snd=hge->Effect_Load("AirplaneResource/sound/enemy1_down.mp3");
		button_snd=hge->Effect_Load("AirplaneResource/sound/button.mp3");
		bomb_snd=hge->Effect_Load("AirplaneResource/sound/use_bomb.mp3");
		over_snd=hge->Effect_Load("AirplaneResource/sound/game_over.mp3");


       //���ּ���
		fnt=new hgeFont("AirplaneResource/font/font1.fnt");
		fntover=new hgeFont("AirplaneResource/font/font1.fnt");

		//down����Ч��
		tex_particle=hge->Texture_Load("AirplaneResource/particles.png");
		//����һ����������
		spr_particle1=new hgeSprite(tex_particle,64,32,32,32);
		spr_particle1->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		spr_particle1->SetHotSpot(16,16);
		//����ϵͳ��һ������
		par1=new hgeParticleSystem("AirplaneResource/down.psi",spr_particle1);
		

		//hit����Ч��
		//����һ����������
		spr_particle2=new hgeSprite(tex_particle,32,1,32,32);
		spr_particle2->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		spr_particle2->SetHotSpot(16,16);
		//����ϵͳ��һ������
		par2=new hgeParticleSystem("AirplaneResource/hit.psi",spr_particle2);
		par2->Fire();
		

	



		


	
		//��������:ѭ��ִ��֡����
		hge->System_Start();
	}
	else
	{	
		// If HGE initialization failed show error message
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

//���ڹر�

//�ر�����
	hge->System_Shutdown();

	//�ͷ�hgeָ��
	hge->Release();

	return 0;
}
