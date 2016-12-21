#define XSCALE 2
#define YSCALE 2
#define MAXOBJECTS 20

#define SCREENWIDTH 960
#define SCREENHEIGHT 544

#define NOSLEEP 1

// UNCHANGED BY SCALE
int drawXOffset=0;
int drawYOffset=0;

// Include cool libraries and stuff
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/rtc.h>
#include <psp2/types.h>

#include <vita2d.h>

// Include stuff I made.
#include "GoodArray.h"
//#include "main.h" TODO - Make this
// main.h
	// Make stuff fresh
	typedef int8_t		s8;
	typedef int16_t		s16;
	typedef int32_t		s32;
	typedef int64_t		s64;
	typedef uint8_t 	u8;
	typedef uint16_t 	u16;
	typedef uint32_t	u32;
	typedef uint64_t	u64;

	void Wait(int miliseconds);
	u64 GetTicks();

	typedef struct Q_tileImageData{
		char tile;
		char tileset;
	}tileImageData;
	typedef struct Q_tileOtherData{
		char isSolid;
		char event;
	}tileSpotData;
	typedef struct Q_animation{
		u64 lastChange; // The last frame that it changed at
		short speed; // Number of frames until change
		vita2d_texture* texture; // Image.
		short width; // Width of a single frame
		short height; // Height of a single frame
		char currentFrame; // Current frame I'm on.
		char numberOfFrames; // Total number of frames.
		char goBackwards; // Go backwards at the end?
		signed char addOnChange; // What is added on change.
		char drawdrawXOffset; // An x offset.
		char drawdrawYOffset; // A y offset.
	}animation;
	typedef struct Q_object{
		short x;
		short y;
		animation theAnimation;
		char slot;
	}object;
// 
#include "FpsCapper.h"

vita2d_pgf* defaultPgf;

// Controls at start of frame.
SceCtrlData pad;
// Controls from start of last frame.
SceCtrlData lastPad;

unsigned int unusedAreaColor = RGBA8(0,0,0,255);

/*
=================================================
== STUFF
=================================================
*/
lua_State* L;
vita2d_texture* fontImage;
animation selectorAnimation;
// Max I think I'll make 8 chars.
char* playerName="Nathan";
// Where you are. Like Overworld, Menu, Battle, etc.
// Values in main function
unsigned char place=0;
/*
==================================================
== WORLD MAP
==================================================
*/
vita2d_texture* tilesets[5];
object mapObjects[MAXOBJECTS];
Good2dArray layers[5];
Good2dArray tileOtherData;
char numberOfLayers=0;
vita2d_texture* playerDown;
vita2d_texture* playerUp;
vita2d_texture* playerLeft;
vita2d_texture* playerRight;
// 0 - Not walking. 1 - up, 2 - down, 3 - left, 4 - right
char isWalking;
object* playerObject;
short cameraWholeOffsetX=0;
char cameraPartialOffsetX=0;
short cameraWholeOffsetY=0;
char cameraPartialOffsetY=0;
char singleTileWidth=32*YSCALE;
char singleTileHeight=32*XSCALE;
short cameraWidth;
short cameraHeight;
/*
=================================================
== BATTLE
=================================================
*/

// Fix x that needs to be applied on ALL drawing
int FixX(int x){
	return ((floor(x/32))*singleTileWidth+((x%32)*XSCALE))+drawXOffset;
}
int FixY(int x){
	return ((floor(x/32))*singleTileHeight+((x%32)*YSCALE))+drawYOffset;
}
// Make objects appear where they should when the screen scrolls.
int FixXObjects(int x){
	return FixX(x-cameraWholeOffsetX*32-cameraPartialOffsetX);
}
int FixYObjects(int x){
	return FixY(x-cameraWholeOffsetY*32-cameraPartialOffsetY);
}

tileImageData* GetMapImageData(short x, short y, short layer){
	return (tileImageData*)GetGoodArray(&(layers[layer]),x,y);
}
tileSpotData* GetMapSpotData(short x, short y){
	return (tileSpotData*)GetGoodArray(&(tileOtherData),x,y);
}

void DrawMap(){
	char yAmount=cameraHeight;
	char xAmount=cameraWidth;
	if (cameraPartialOffsetX>0){
		xAmount++;
	}
	if (cameraPartialOffsetY>0){
		yAmount++;
	}

	int x,y,i;
	for (i=0;i<numberOfLayers;i++){
		for (y=0;y<yAmount;y++){
			for (x=0;x<xAmount;x++){
				if (GetMapImageData(x+cameraWholeOffsetX,y+cameraWholeOffsetY,i)->tile!=0){
					vita2d_draw_texture_part_scale(tilesets[(int)GetMapImageData(x+cameraWholeOffsetX,y+cameraWholeOffsetY,i)->tileset],FixX(x*32-cameraPartialOffsetX),FixY(y*32-cameraPartialOffsetY),((GetMapImageData(x+cameraWholeOffsetX,y+cameraWholeOffsetY,i)->tile-1)*32),0,32,32,XSCALE,YSCALE);
				}
			}
		}
	}
}

// Waits for a number of miliseconds.
void Wait(int miliseconds){
	sceKernelDelayThread(miliseconds*1000);
}

u64 GetTicks(){
	SceRtcTick temp;
	sceRtcGetCurrentTick(&temp);
	return temp.tick;
}

void LoadMap(char* path){
	FILE* openedFile;
	openedFile = fopen(path,"r");

	short _width;
	short _height;
	// Read width and height
	fread(&_width,2,1,openedFile);
	fread(&_height,2,1,openedFile);
	// Set array to the size it needs
	SetGoodArray(&(tileOtherData),_width,_height,sizeof(tileOtherData));

	int x,y,i;

	for (y=0;y<_height;y++){
		for (x=0;x<_width;x++){
			// Read isSolid and then event
			fread(&(GetMapSpotData(x,y)->isSolid),1,1,openedFile);
			fread(&(GetMapSpotData(x,y)->event),1,1,openedFile);
		}
	}
	// Read number of loayers
	fread(&numberOfLayers,1,1,openedFile);

	for (i=0;i<numberOfLayers;i++){
		SetGoodArray(&(layers[i]),tileOtherData.width,tileOtherData.height,sizeof(tileImageData));
	}
	for (i=4;i>=numberOfLayers;i--){
		SetGoodArray(&(layers[i]),1,1,sizeof(char));
	}

	for (i=0;i<numberOfLayers;i++){
		for (y=0;y<_height;y++){
			for (x=0;x<_width;x++){
				// Tile then tileset
				fread(&(GetMapImageData(x,y,i)->tile),1,1,openedFile);
				fread(&(GetMapImageData(x,y,i)->tileset),1,1,openedFile);
			}
		}
	}

	fclose (openedFile);


	//((tileImageData*)GetGoodArray(&layers[0],1,1))->tile=1;
	//
}


// X, Y, layer, Tileset, Tile
int L_SetMapImageData(lua_State* passedState){
	tileImageData* _theChosenData = GetMapImageData(lua_tonumber(passedState,1),lua_tonumber(passedState,2),lua_tonumber(passedState,3));
	_theChosenData->tileset=lua_tonumber(passedState,4);
	_theChosenData->tile=lua_tonumber(passedState,5);
	return 0;
}

void SetCameraValues(){
	// Configure variables

	// Set camera width and height to their max for the device.
	cameraWidth=floor((SCREENWIDTH/XSCALE)/32);
	cameraHeight=floor((SCREENHEIGHT/YSCALE)/32);
	// Restict to max values

	// Centers it
	drawXOffset=((SCREENWIDTH-cameraWidth*32*XSCALE)/2);
	drawYOffset=((SCREENHEIGHT-cameraHeight*32*YSCALE)/2);
}

void Init(){
	// Init vita2d and set its clear color.
	vita2d_init();
	vita2d_set_clear_color(RGBA8(212, 208, 200, 0xFF));
	// We love default fonts.
	defaultPgf = vita2d_load_default_pgf();

	// ... I guess I have to do this? I'm... most probrablly not basing this off the sample.
	memset(&pad, 0, sizeof(pad));

	InitGoodArray(&tileOtherData);
	int i=0;
	for (i=0;i<5;i++){
		InitGoodArray(&(layers[i]));
	}
	LoadMap("app0:Stuff/test");

	L = luaL_newstate();
	luaL_openlibs(L);
	lua_pushcfunction(L,L_SetMapImageData);
	lua_setglobal(L,"SetMapImageData");
	luaL_dofile(L,"app0:Stuff/Test.lua");


	playerDown=vita2d_load_PNG_file("app0:Stuff/PlayerDown.png");
	playerUp=vita2d_load_PNG_file("app0:Stuff/PlayerUp.png");
	playerLeft=vita2d_load_PNG_file("app0:Stuff/PlayerLeft.png");
	playerRight=vita2d_load_PNG_file("app0:Stuff/PlayerRight.png");
	
	selectorAnimation.texture=vita2d_load_PNG_file("app0:Stuff/Selector.png");
	selectorAnimation.numberOfFrames=8;
	selectorAnimation.width=22;
	selectorAnimation.height=17;
	selectorAnimation.goBackwards=1;
	selectorAnimation.speed=15;
	selectorAnimation.addOnChange=1;

	mapObjects[0].theAnimation.speed=10;
	mapObjects[0].theAnimation.texture = playerDown;
	mapObjects[0].theAnimation.width=18;
	mapObjects[0].theAnimation.height=32;
	mapObjects[0].theAnimation.numberOfFrames=3;
	mapObjects[0].theAnimation.goBackwards=1;
	mapObjects[0].theAnimation.lastChange=0;
	mapObjects[0].theAnimation.currentFrame=0;
	mapObjects[0].x=64;
	mapObjects[0].y=64;
	mapObjects[0].theAnimation.drawdrawXOffset=7;
	mapObjects[0].theAnimation.addOnChange=1;

	(playerObject)=&(mapObjects[0]);

	SetCameraValues();

	fontImage=vita2d_load_PNG_file("app0:Stuff/Font.png");
}

void StartFrameStuff(){
	FpsCapStart();
	sceCtrlPeekBufferPositive(0, &pad, 1);
}
void EndFrameStuff(){
	lastPad=pad;
	FpsCapWait();
}

// Updates the animation's frame if it needs to be.
void UpdateAnimationIfNeed(animation* animationToDraw){
	if (animationToDraw->numberOfFrames>1 && numberOfFrames-animationToDraw->lastChange>=animationToDraw->speed){
		animationToDraw->lastChange = numberOfFrames;
		
		if (animationToDraw->addOnChange<0 && animationToDraw->currentFrame==0){
			animationToDraw->addOnChange=1;
		}

		if ((animationToDraw->currentFrame==animationToDraw->numberOfFrames-1)){
			if (animationToDraw->goBackwards==1){
				animationToDraw->addOnChange=-1;
				animationToDraw->currentFrame--;
			}else{
				animationToDraw->currentFrame=0;
			}
		}else{
			animationToDraw->currentFrame=animationToDraw->currentFrame+animationToDraw->addOnChange;
		}
	}
}

// For drawing object's animations. Position changed by map scroll.
void DrawAnimationAsObject(animation* animationToDraw, int destX, int destY){
	UpdateAnimationIfNeed(animationToDraw);
	vita2d_draw_texture_part_scale(animationToDraw->texture,FixXObjects(destX+animationToDraw->drawdrawXOffset),FixYObjects(destY+animationToDraw->drawdrawYOffset),animationToDraw->width*animationToDraw->currentFrame,0,animationToDraw->width,animationToDraw->height,XSCALE,YSCALE);
}
// Draw animation. Unchanged by map position
void DrawAnimation(animation* animationToDraw, int destX, int destY){
	UpdateAnimationIfNeed(animationToDraw);
	vita2d_draw_texture_part_scale(animationToDraw->texture,FixX(destX+animationToDraw->drawdrawXOffset),FixY(destY+animationToDraw->drawdrawYOffset),animationToDraw->width*animationToDraw->currentFrame,0,animationToDraw->width,animationToDraw->height,XSCALE,YSCALE);
}
// Draws it as given in arguments. No scaling or position changes.
void DrawAnimationAsISay(animation* animationToDraw, int destX, int destY, int scale){
	UpdateAnimationIfNeed(animationToDraw);
	vita2d_draw_texture_part_scale(animationToDraw->texture,(destX+animationToDraw->drawdrawXOffset),(destY+animationToDraw->drawdrawYOffset),animationToDraw->width*animationToDraw->currentFrame,0,animationToDraw->width,animationToDraw->height,scale,scale);
}

void DrawMapObjects(){
	int i;
	for (i=0;i<MAXOBJECTS;i++){
		if (mapObjects[i].theAnimation.width!=0){
			DrawAnimationAsObject(&(mapObjects[i].theAnimation),(mapObjects[i].x),(mapObjects[i].y));
		}
	}
}

void UpdateCameraValues(object* theThingie){
	// If past middle...
	if (theThingie->x>(cameraWidth/2)*32){
		cameraWholeOffsetX=(theThingie->x-(cameraWidth/2)*32)/32;
		cameraPartialOffsetX=theThingie->x%32;
		if (tileOtherData.width-cameraWholeOffsetX-cameraWidth<=0){
			cameraWholeOffsetX=tileOtherData.width-cameraWidth;
			cameraPartialOffsetX=0;
		}
	}else{
		cameraWholeOffsetX=0;
		cameraPartialOffsetX=0;
	}
	//printf("%d;%d\n",(cameraHeight/2)*32,theThingie->y);
	if (theThingie->y>(cameraHeight/2)*32){
		cameraWholeOffsetY=(theThingie->y-(cameraHeight/2)*32)/32;
		cameraPartialOffsetY=theThingie->y%32;
		//tileOtherData.height-cameraWholeOffsetY-cameraHeight
		if (tileOtherData.height-cameraWholeOffsetY-cameraHeight<=0){
			cameraWholeOffsetY=tileOtherData.height-cameraHeight;
			cameraPartialOffsetY=0;
		}
	}else{
		cameraWholeOffsetY=0;
		cameraPartialOffsetY=0;
	}
}

void DrawUnusedAreaRect(){
	// Top
	vita2d_draw_rectangle(0,0,SCREENWIDTH,drawYOffset,unusedAreaColor);
	// Bottom
	vita2d_draw_rectangle(0,SCREENHEIGHT-drawYOffset,SCREENWIDTH,drawYOffset,unusedAreaColor);
	// Left
	vita2d_draw_rectangle(0,0,drawXOffset,SCREENHEIGHT,unusedAreaColor);
	// Right
	vita2d_draw_rectangle(SCREENWIDTH,0,drawXOffset,SCREENHEIGHT,unusedAreaColor);
}

char checkCollision(int x, int y){
	return GetMapSpotData(x,y)->isSolid;
}

char ExecuteEvent(object* theThingie,int eventId){
	char eventString[8];
	sprintf(eventString,"%s%02d","Event",eventId);

	// Adds function to stack
	lua_getglobal(L,eventString);
	// Pass object
	lua_pushlightuserdata(L,theThingie);
	// Call funciton. Removes function from stack.
	lua_call(L, 1, 1);

	char returnedValue=lua_tonumber(L,-1);
	lua_pop(L,1);
	return returnedValue;
}

void DrawLetter(int letterId, int _x, int _y, int size){
	//DrawTexture(_mainRenderer,fontImage, (letterId-32)*8, 0, 8, 8, _x, _y, 16, 16);
	vita2d_draw_texture_part_scale(fontImage,_x,_y,(letterId-32)*(8),0,8,8,XSCALE*size,YSCALE*size);
}

void DrawLetterUnscaled(int letterId, int _x, int _y, int size){
	vita2d_draw_texture_part_scale(fontImage,_x,_y,(letterId-32)*(8),0,8,8,size,size);
}

void DrawText(int x, int y, const char* text, int size){
	int i=0;
	for (i = 0; i < strlen(text); i++){
		DrawLetter(text[i],FixX(x+(i*(8*size))+i),FixY(y),size);
	}
}

void DrawTextAsISay(int x, int y, const char* text, int size){
	int i=0;
	for (i = 0; i < strlen(text); i++){
		DrawLetterUnscaled(text[i],(x+(i*(8*size))+i),(y),size);
	}
}

char WasJustPressed(int value){
	if (pad.buttons & value && !(lastPad.buttons & value)){
		return 1;
	}
	return 0;
}

// Draws everything we draw on map.
void DrawMapThings(){
	DrawMap();
	DrawMapObjects();
	DrawUnusedAreaRect();
}

void MenuLop(){
	signed char selected=0;

	while(1){
		StartFrameStuff();
		
		if (pad.buttons & SCE_CTRL_CROSS){
			switch(selected){
				case 0:
					place=0;
					return;
				break;
				case 1:
					place=/*WHATVER STATUS/PARTY IS*/ 2;
					return;
				break;
				case 2:
					place=/*WHATEVER OPTIONS IS*/ 2;
					return;
				break;
				case 3:
					place=2;
					return;
				break;
			}
		}else if (WasJustPressed(SCE_CTRL_START)){
			place=0;
			return;
		}
		if (WasJustPressed(SCE_CTRL_DOWN)){
			selected++;
			if (selected>5){
				selected=0;
			}
		}else if (WasJustPressed(SCE_CTRL_UP)){
			selected--;
			if (selected<0){
				selected=0;
			}
		}


		vita2d_start_drawing();
		vita2d_clear_screen();
	
		DrawMapThings();
		// Draw fancy shade thingie
		vita2d_draw_rectangle(720,136,5,272,RGBA8(0,0,0,255));
		vita2d_draw_rectangle(240,408,485,5,RGBA8(0,0,0,255));
		// Draw border
		vita2d_draw_rectangle(240,136,480,272,RGBA8(0,255,0,255));
		// Draw real rectangle
		vita2d_draw_rectangle(245,141,470,262,RGBA8(252,255,255,255));

		DrawTextAsISay(320,146,"Happy Menu",4);
		DrawTextAsISay(287,183,"Not Quit",4);
		DrawTextAsISay(287,183+32+5,playerName,4);
		DrawTextAsISay(287,183+64+10,"Options",4);
		DrawTextAsISay(287,183+96+15,"Quit",4);
		
		DrawAnimationAsISay(&selectorAnimation,245,selected*32+183+selected*5,2);


		vita2d_end_drawing();
		vita2d_swap_buffers();
		EndFrameStuff();
	}
}

void Overworld(){
		StartFrameStuff();
		// Main logic
		#if NOSLEEP==1
			sceKernelPowerTick(0);
		#endif

		// Controls only if not walking
		if (isWalking==0){
			if (pad.buttons!=0){
				if (pad.buttons & SCE_CTRL_UP){
					if (checkCollision(playerObject->x/32,playerObject->y/32-1)!=1){
						// TODO - Execute event before move.
						isWalking=1;
						playerObject->theAnimation.numberOfFrames=3;
						playerObject->y=playerObject->y-4;
					}
					playerObject->theAnimation.texture=playerUp;
				}else if (pad.buttons & SCE_CTRL_DOWN){
					if (checkCollision(playerObject->x/32,playerObject->y/32+1)!=1){
						isWalking=2;
						playerObject->theAnimation.numberOfFrames=3;
						playerObject->y=playerObject->y+4;
					}
					playerObject->theAnimation.texture=playerDown;
				}else if (pad.buttons & SCE_CTRL_LEFT){
					if (checkCollision(playerObject->x/32-1,playerObject->y/32)!=1){
						isWalking=3;
						playerObject->theAnimation.numberOfFrames=3;
						playerObject->x=playerObject->x-4;
					}
					playerObject->theAnimation.texture=playerLeft;
				}else if (pad.buttons & SCE_CTRL_RIGHT){
					if (checkCollision(playerObject->x/32+1,playerObject->y/32)!=1){
						isWalking=4;
						playerObject->theAnimation.numberOfFrames=3;
						playerObject->x=playerObject->x+4;
					}
					playerObject->theAnimation.texture=playerRight;
				}else if (pad.buttons & SCE_CTRL_CROSS){
					short eventUseX;
					short eventUseY;

					eventUseX=playerObject->x/32;
					eventUseY=playerObject->y/32;

					if (playerObject->theAnimation.texture==playerRight){
						eventUseX++;
					}else if (playerObject->theAnimation.texture==playerLeft){
						eventUseX--;
					}else if (playerObject->theAnimation.texture==playerDown){
						eventUseY++;
					}else if (playerObject->theAnimation.texture==playerUp){
						eventUseY--;
					}
					tileSpotData* eventTempSpotData = GetMapSpotData(eventUseX,eventUseY);
					if (eventTempSpotData->event!=0){
						ExecuteEvent(playerObject,eventTempSpotData->event);
					}
				}
			
				if (playerObject->x%32==0 && playerObject->y%32==0){
					// Player didn't move.
					playerObject->theAnimation.addOnChange=1;
					playerObject->theAnimation.numberOfFrames=1;
					playerObject->theAnimation.currentFrame=0;
				}

			}
		}else{
			if (playerObject->x%32==0 && playerObject->y%32==0){
				// Player reached their destination
				tileSpotData* eventTempSpotData = GetMapSpotData(playerObject->x/32,playerObject->y/32);
				if (eventTempSpotData->event!=0){
					ExecuteEvent(playerObject,eventTempSpotData->event);
				}

				isWalking=0;
				if (!((pad.buttons & SCE_CTRL_UP) || (pad.buttons & SCE_CTRL_DOWN) || (pad.buttons & SCE_CTRL_LEFT) || (pad.buttons & SCE_CTRL_RIGHT))){
					// Stop
					playerObject->theAnimation.addOnChange=1;
					playerObject->theAnimation.numberOfFrames=1;
					playerObject->theAnimation.currentFrame=0;
				}
			}else{
				switch(isWalking){
					case 1:
						playerObject->y=playerObject->y-4;
					break;
					case 2:
						playerObject->y=playerObject->y+4;
					break;
					case 3:
						playerObject->x=playerObject->x-4;
					break;
					case 4:
						playerObject->x=playerObject->x+4;
					break;
				}
			}
		}

		UpdateCameraValues(playerObject);


		// Drawing
		vita2d_start_drawing();
		vita2d_clear_screen();

		DrawMapThings();
		//DrawText(60,60,"Hello world",2);

		vita2d_end_drawing();
		vita2d_swap_buffers();

		if (pad.buttons & SCE_CTRL_START)// TODO - PLAY SOUND
			MenuLop();

		EndFrameStuff();
}

int main(){
	Init();	
	tilesets[0] = vita2d_load_PNG_file("app0:Stuff/a.png");

	/*
		0 - Overworld
		1 - Pause in Overworld
		2 - Quit (breaks)
	*/

	while (1){
		if (place==0){
			Overworld();
		}else if (place==1){
			MenuLop();
		}else if (place==2){
			break;
		}
	}

	// End this stuff
	vita2d_fini();
	vita2d_free_pgf(defaultPgf);
	// I really don't get why I need this when the user can just use the PS button.
	sceKernelExitProcess(0);
	return 0;
}