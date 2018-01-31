/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hge_tut01 - Minimal HGE application
*/


//#include "hge/include/hge.h"//项目目录下查找该文件，找不到到系统目录下查找，再找不到报错(自己写的头文件)
#include<hge.h>//系统目录下查找该文件，找不到直接报错（系统的）.
#include <hgesprite.h>
#include <list>
#include <hgerect.h>
#include <hgefont.h>
#include <hgeparticle.h>
using namespace std;
HGE *hge = 0;//定义一个hge引擎的指针;
hgeSprite *spr=NULL;//精灵指针(背景)
HTEXTURE tex;//纹理句柄(背景)
float y3=0,y2=-852;//两张背景滚动的坐标

//开始界面
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

//暂停界面
hgeSprite *sprbutton3=NULL;
HTEXTURE texbutton3;
hgeSprite *sprbutton4=NULL;
HTEXTURE texbutton4;



hgeSprite *sprplane=NULL;//精灵指针(飞机)
HTEXTURE texplane;//纹理句柄(飞机)
hgeSprite *sprplane1=NULL;//精灵指针(飞机)
HTEXTURE texplane1;//纹理句柄(飞机)
float x=190,y=720;
bool Anistate=true;

hgeSprite *sprenemy1=NULL;//精灵指针(敌机1)
HTEXTURE texenemy1;//纹理句柄(敌机1)

enum Enemy1_state{no,hit,down} enemy1_state=no;
enum Enemy{enemy1,enemy2,enemy3} enemy;
struct Enemy1
{   Enemy1_state enemy1_state;
	float x_enemy1;
	float y_enemy1;
	Enemy enemy;
};
list<Enemy1> lEnemy1;


hgeSprite *sprenemy2=NULL;//精灵指针(敌机2)
HTEXTURE texenemy2;//纹理句柄(敌机2)

hgeSprite *sprenemy3=NULL;//精灵指针(敌机3)
HTEXTURE texenemy3;//纹理句柄(敌机3)


hgeSprite *sprbullet=NULL;//精灵指针(子弹)
HTEXTURE texbullet;//纹理句柄(子弹)
struct Bullet
{

	float x_bullet;
	float y_bullet;
};

list<Bullet> lBullet;

hgeSprite *sprstop=NULL;//精灵指针(暂停按钮)
HTEXTURE texstop;//纹理句柄(暂停)
enum Stop_state{stop,play} stop_state=play;


HEFFECT game_snd;//背景音乐
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


hgeSprite *spr_particle1=NULL;//精灵指针(粒子效果)
HTEXTURE tex_particle;//纹理句柄(粒子效果)
hgeParticleSystem*	par1=NULL;//粒子系统

hgeSprite *spr_particle2=NULL;//精灵指针(粒子效果)
hgeParticleSystem*	par2=NULL;//粒子系统



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
//判断是否静音
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
	//改变背景绘制的y坐标
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

	//判断ESC键是否按下
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
	
	return false;//退出游戏
}

//开始界面绘制
void RenderStart()
{
	spr->Render(0,0);
	sprstar->Render(20,100);
	sprbutton1->Render(100,400);
	sprbutton2->Render(100,500);
}

void RenderGame()
{

    //实际的绘制
	spr->Render(0,y3);
	spr->Render(0,y2);

	//飞机
	if(Anistate)
		sprplane->Render(x,y);
	else 
		sprplane1->Render(x,y);
    //子弹
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
	//敌机1
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
	//文字
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
    //实际的绘制
	spr->Render(0,y3);
	spr->Render(0,y2);

	//飞机
	sprplane->Render(x,y);

    //子弹
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
	//敌机1
	if(!lEnemy1.empty())
	{
		
		for (;it_enemy1!=lEnemy1.end();it_enemy1++)
		{

			sprenemy1->Render(it_enemy1->x_enemy1,it_enemy1->y_enemy1);
		
		}
    
		
	}
	//文字
	fnt->printf(190, 5, HGETEXT_LEFT, "score is:%d", score);
	fnt->printf(250, 300, HGETEXT_CENTER, "Game Over!Your Score is:%d", score);
	sprbutton4->Render(100,400);
	sprbutton2->Render(100,500);
	sprstop->Render(420,650);
	par2->Render();
	
}
//渲染的函数:循环：绘制图形
bool RenderFunc()
{

	//通知引擎开始绘制
	hge->Gfx_BeginScene();

	// 清屏（默认是黑色）
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

 
	//结束绘制
	hge->Gfx_EndScene();

	return false;
}
//入口函数或者主函数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
 //给hge指针赋值：hge引擎版本号
	hge = hgeCreate(HGE_VERSION);

	// 设置帧函数  FrameFunc函数名；
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);

	// 设置渲染函数  RenderFunc函数名
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);

	// 设置窗口的标题
	hge->System_SetState(HGE_TITLE, "飞机大战");
	
	// 窗口模式：默认是 800X600
	hge->System_SetState(HGE_WINDOWED, true);
	//设置窗口的大小
	hge->System_SetState(HGE_SCREENWIDTH,480);
	hge->System_SetState(HGE_SCREENHEIGHT,852);
	
	//是否隐藏鼠标
	hge->System_SetState(HGE_HIDEMOUSE,false);
	// 不使用bass声音库
	hge->System_SetState(HGE_USESOUND, true);
	hge->System_SetState(HGE_FPS,100);

	//初始化引擎
	if(hge->System_Initiate())
	{
	//自己的初始化:例如：加载图片
		
		//开始界面
     	//从文件中加载一张图片，并用纹理句柄保存
		texstar=hge->Texture_Load("AirplaneResource/ui/shoot_background/shoot_copyright.png");
		//创建一个精灵(关联一张图片)
		sprstar=new hgeSprite(texstar,0,0,441,225);
		//从文件中加载一张图片，并用纹理句柄保存
		texbutton1=hge->Texture_Load("AirplaneResource/game_start.png");
		//创建一个精灵(关联一张图片)
		sprbutton1=new hgeSprite(texbutton1,0,0,300,41);
		//从文件中加载一张图片，并用纹理句柄保存
		texbutton2=hge->Texture_Load("AirplaneResource/game_quit.png");
		//创建一个精灵(关联一张图片)
		sprbutton2=new hgeSprite(texbutton2,0,0,300,41);		
		//从文件中加载一张图片，并用纹理句柄保存
		texmusicplay=hge->Texture_Load("AirplaneResource/ui/bkmusic_play.png");
		//创建一个精灵(关联一张图片)
		sprmusicplay=new hgeSprite(texmusicplay,0,0,48,48);
		texmusicclose=hge->Texture_Load("AirplaneResource/ui/bkmusic_close.png");
		//创建一个精灵(关联一张图片)
		sprmusicclose=new hgeSprite(texmusicclose,0,0,48,48);
		
		
		tex=hge->Texture_Load("AirplaneResource/ui/shoot_background/background.png");
     	//创建一个精灵(关联一张图片)
		spr=new hgeSprite(tex,0,0,480,852);

		//从文件中加载一张图片（飞机），并用纹理句柄保存
		texplane=hge->Texture_Load("AirplaneResource/ui/shoot/hero1.png");
		//创建一个精灵飞机(关联一张图片)
		sprplane=new hgeSprite(texplane,0,0,102,126);
			
		//从文件中加载一张图片（飞机），并用纹理句柄保存
		texplane1=hge->Texture_Load("AirplaneResource/ui/shoot/hero2.png");
		//创建一个精灵飞机(关联一张图片)
		sprplane1=new hgeSprite(texplane1,0,0,102,126);

		//从文件中加载一张图片（子弹），并用纹理句柄保存
		texbullet=hge->Texture_Load("AirplaneResource/ui/shoot/bullet2.png");
		//创建一个精灵子弹(关联一张图片)
		sprbullet=new hgeSprite(texbullet,0,0,5,11);
        lBullet.clear();

		//从文件中加载一张图片（敌机1），并用纹理句柄保存
		texenemy1=hge->Texture_Load("AirplaneResource/ui/shoot/enemy1.png");
		//创建一个精灵敌机1(关联一张图片)
		sprenemy1=new hgeSprite(texenemy1,0,0,57,43);
		lEnemy1.clear();

		//从文件中加载一张图片（敌机2），并用纹理句柄保存
		texenemy2=hge->Texture_Load("AirplaneResource/ui/shoot/enemy2.png");
		//创建一个精灵敌机1(关联一张图片)
		sprenemy2=new hgeSprite(texenemy2,0,0,69,99);
		//从文件中加载一张图片（敌机1），并用纹理句柄保存
		texenemy3=hge->Texture_Load("AirplaneResource/ui/shoot/enemy3_n1.png");
		//创建一个精灵敌机1(关联一张图片)
		sprenemy3=new hgeSprite(texenemy3,0,0,169,258);

		//从文件中加载一张图片（暂停），并用纹理句柄保存
		texstop=hge->Texture_Load("AirplaneResource/ui/shoot/game_pause_nor.png");
		//创建一个精灵暂停(关联一张图片)
		sprstop=new hgeSprite(texstop,0,0,60,45);
			//从文件中加载一张图片（暂停），并用纹理句柄保存
		texbutton3=hge->Texture_Load("AirplaneResource/ui/shoot/game_continue.png");
		//创建一个精灵暂停(关联一张图片)
		sprbutton3=new hgeSprite(texbutton3,0,0,300,41);
			//从文件中加载一张图片（暂停），并用纹理句柄保存
		texbutton4=hge->Texture_Load("AirplaneResource/ui/shoot/game_again.png");
		//创建一个精灵暂停(关联一张图片)
		sprbutton4=new hgeSprite(texbutton4,0,0,300,41);

		bullet_snd=hge->Effect_Load("AirplaneResource/sound/bullet.mp3");
		game_snd=hge->Effect_Load("AirplaneResource/sound/game_music.mp3");
		down_snd=hge->Effect_Load("AirplaneResource/sound/enemy1_down.mp3");
		button_snd=hge->Effect_Load("AirplaneResource/sound/button.mp3");
		bomb_snd=hge->Effect_Load("AirplaneResource/sound/use_bomb.mp3");
		over_snd=hge->Effect_Load("AirplaneResource/sound/game_over.mp3");


       //文字加载
		fnt=new hgeFont("AirplaneResource/font/font1.fnt");
		fntover=new hgeFont("AirplaneResource/font/font1.fnt");

		//down粒子效果
		tex_particle=hge->Texture_Load("AirplaneResource/particles.png");
		//创建一个精灵粒子
		spr_particle1=new hgeSprite(tex_particle,64,32,32,32);
		spr_particle1->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		spr_particle1->SetHotSpot(16,16);
		//粒子系统绑定一个精灵
		par1=new hgeParticleSystem("AirplaneResource/down.psi",spr_particle1);
		

		//hit粒子效果
		//创建一个精灵粒子
		spr_particle2=new hgeSprite(tex_particle,32,1,32,32);
		spr_particle2->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		spr_particle2->SetHotSpot(16,16);
		//粒子系统绑定一个精灵
		par2=new hgeParticleSystem("AirplaneResource/hit.psi",spr_particle2);
		par2->Fire();
		

	



		


	
		//启动引擎:循环执行帧函数
		hge->System_Start();
	}
	else
	{	
		// If HGE initialization failed show error message
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

//窗口关闭

//关闭引擎
	hge->System_Shutdown();

	//释放hge指针
	hge->Release();

	return 0;
}
