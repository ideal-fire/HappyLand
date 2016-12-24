// on 10

/*
---O---OO--OOOOO-O--OO-OOOOO----O---O
--O-O-O-O--O---O-OOO---O---O---O-O-O-
-O--OO--O--O---O-O-----O---O--O--OO--
O---O---O--OOOOO-O-----OOOOO-O---O---
*/

#define MAPXSCALE 2
#define MAPYSCALE 2
#define MAXOBJECTS 20

#define SCREENWIDTH 960
#define SCREENHEIGHT 544

#define NOSLEEP 1

#define TEXTBOXY 420

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
		char drawXOffset; // An x offset.
		char drawYOffset; // A y offset.
	}animation;
	typedef struct Q_object{ 
		short x;
		short y;
		animation theAnimation;
		char slot;
	}object;
	typedef struct Q_stats{
		char level;
		short maxHp;
		short maxMp;
		unsigned char attack;
		unsigned char defence;
		unsigned char magicAttack;
		unsigned char magicDefence;
		unsigned char speed;
		short exp;
	}stats;
	typedef struct Q_partyMember{
		stats fighterStats;
		char* name;
		int x; // Position on screen
		int y; // Position on screen
		short hp;
		short mp;
	}partyMember;
	typedef struct Q_spell{
		char* name;
		char attack;
		char specialAttack;
	}spell;
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
// Where you are. Like Overworld, Menu, Battle, etc.
// Values in main function
unsigned char place=0;
/*
====================================================
== OPTIONS
===================================================
*/
// Max I think I'll make 8 chars.
char* playerName="Nathan";
int aButton=SCE_CTRL_CROSS;
int bButton=SCE_CTRL_CIRCLE;
char textboxNewCharSpeed=10;
char defaultSelected=1;
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
char singleTileWidth=32*MAPYSCALE;
char singleTileHeight=32*MAPXSCALE;
short cameraWidth;
short cameraHeight;
/*
=================================================
== BATTLE
=================================================
*/
partyMember party[4];
animation partyAttackAnimation[4];
animation partyIdleAnimation[4];
animation partySpellAnimation[4];
partyMember enemies[4];
animation enemyAttackAnimation[4];
animation enemyIdleAnimation[4];
animation enemySpellAnimation[4];
char partySize;
char numberOfEnemies;
partyMember dummyMember;

// Fix x that needs to be applied on ALL drawing
int FixX(int x){
	return ((floor(x/32))*singleTileWidth+((x%32)*MAPXSCALE))+drawXOffset;
}
int FixY(int x){
	return ((floor(x/32))*singleTileHeight+((x%32)*MAPYSCALE))+drawYOffset;
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
					vita2d_draw_texture_part_scale(tilesets[(int)GetMapImageData(x+cameraWholeOffsetX,y+cameraWholeOffsetY,i)->tileset],FixX(x*32-cameraPartialOffsetX),FixY(y*32-cameraPartialOffsetY),((GetMapImageData(x+cameraWholeOffsetX,y+cameraWholeOffsetY,i)->tile-1)*32),0,32,32,MAPXSCALE,MAPYSCALE);
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

	char withDotLua[strlen(path)+strlen(".lua")+1];
	sprintf(withDotLua,"%s%s",path,".lua");
	luaL_dofile(L,withDotLua);


	//((tileImageData*)GetGoodArray(&layers[0],1,1))->tile=1;
	//
}

void SetCameraValues(){
	// Configure variables

	// Set camera width and height to their max for the device.
	cameraWidth=floor((SCREENWIDTH/MAPXSCALE)/32);
	cameraHeight=floor((SCREENHEIGHT/MAPYSCALE)/32);
	// Restict to max values

	// Centers it
	drawXOffset=((SCREENWIDTH-cameraWidth*32*MAPXSCALE)/2);
	drawYOffset=((SCREENHEIGHT-cameraHeight*32*MAPYSCALE)/2);
}

void StartFrameStuff(){
	FpsCapStart();
	lastPad=pad;
	sceCtrlPeekBufferPositive(0, &pad, 1);
}
void EndFrameStuff(){
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
	vita2d_draw_texture_part_scale(animationToDraw->texture,FixXObjects(destX+animationToDraw->drawXOffset),FixYObjects(destY+animationToDraw->drawYOffset),animationToDraw->width*animationToDraw->currentFrame,0,animationToDraw->width,animationToDraw->height,MAPXSCALE,MAPYSCALE);
}
// Draw animation. Unchanged by map position
void DrawAnimation(animation* animationToDraw, int destX, int destY){
	UpdateAnimationIfNeed(animationToDraw);
	vita2d_draw_texture_part_scale(animationToDraw->texture,FixX(destX+animationToDraw->drawXOffset),FixY(destY+animationToDraw->drawYOffset),animationToDraw->width*animationToDraw->currentFrame,0,animationToDraw->width,animationToDraw->height,MAPXSCALE,MAPYSCALE);
}
// Draws it as given in arguments. No scaling or position changes.
void DrawAnimationAsISay(animation* animationToDraw, int destX, int destY, int scale){
	UpdateAnimationIfNeed(animationToDraw);
	vita2d_draw_texture_part_scale(animationToDraw->texture,(destX+animationToDraw->drawXOffset),(destY+animationToDraw->drawYOffset),animationToDraw->width*animationToDraw->currentFrame,0,animationToDraw->width,animationToDraw->height,scale,scale);
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
	vita2d_draw_texture_part_scale(fontImage,_x,_y,(letterId-32)*(8),0,8,8,MAPXSCALE*size,MAPYSCALE*size);
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


char ShowMessage(char* message, char isQuestion){
	// We count frames, and show a new letter every certian amount of frames.
	char frames=0;
	// Letter we're currently displaying.
	short currentLetter=0;

	char quitTextbox=0;

	signed char currentSelected=defaultSelected;

	// Letter scale is 2.5. 8*2.5=20.
	unsigned char maxLetters=floor(960/(20));

	short newlineSpaces[50];
	char totalNewLines=0;

	// Initialize them so I don't get compiler warnings.
	vita2d_texture* yesButtonTexture=(vita2d_texture*)7;
	vita2d_texture* noButtonTexture=(vita2d_texture*)0x1;

	int i=0;
	int j=0;
	if (isQuestion==1){
		yesButtonTexture=vita2d_load_PNG_file("app0:Stuff/Yes.png");
		noButtonTexture=vita2d_load_PNG_file("app0:Stuff/No.png");
	}

	//I guess this adds the newline segments?
	for (j = 1; j <= floor(strlen(message)/maxLetters)+1; j++){
		if (j==1){
			i=j*maxLetters-1;
		}else{
			i=newlineSpaces[j-2]+maxLetters;
		}
		if (i>strlen(message)){
			break;
		}
		while (1){
			if (message[i]==32){
				// Add the +1 so that space is left at the end of the previous line.
				newlineSpaces[j-1]=i+1;
				totalNewLines++;
				break;
			}else{
				i=i-1;
			}
		}
	}
	newlineSpaces[(int)totalNewLines]=strlen(message);

	char sectionOfCurrentMessage=3;
	if (totalNewLines<3){
		sectionOfCurrentMessage=totalNewLines;
	}
	short characterOffset=0;
	char startSectionOfNewLine=0;
	while (!quitTextbox){
		StartFrameStuff();
			
		if (WasJustPressed(aButton)){
			if (currentLetter==newlineSpaces[(int)sectionOfCurrentMessage]){
				// ADVANCE TO NEXT MESSAGE
				if (sectionOfCurrentMessage!=totalNewLines){
					startSectionOfNewLine+=4;
					characterOffset=newlineSpaces[(int)sectionOfCurrentMessage];
					currentLetter=newlineSpaces[(int)sectionOfCurrentMessage]+1;
					if (sectionOfCurrentMessage+4<=totalNewLines){
						sectionOfCurrentMessage+=4;
					}else{
						sectionOfCurrentMessage=totalNewLines;
					}
					frames=0;
				}else{
					quitTextbox=1;
					// Set isQuestion to 0 so one last render is done, but without the yes and no buttons.
					isQuestion=0;
				}
			}else{
				currentLetter=newlineSpaces[(int)sectionOfCurrentMessage];
			}
		}

		if (isQuestion==1){
			if (WasJustPressed(SCE_CTRL_DOWN)){
				currentSelected++;
				if (currentSelected>1){
					currentSelected=0;
				}
			}else if (WasJustPressed(SCE_CTRL_UP)){
				currentSelected--;
				if (currentSelected<0){
					currentSelected=1;
				}
			}
			
		}
		

		// Advance currentLetter every certian number of frames.
		if (frames==textboxNewCharSpeed && currentLetter!=newlineSpaces[(int)sectionOfCurrentMessage]){
			if (currentLetter<strlen(message)){
				frames=0;
				currentLetter++;
			}
			//Mix_PlayChannel(0,textboxBlip,0);
		}

		vita2d_start_drawing();
		vita2d_clear_screen();

		DrawMapThings();
		// Draw da white rectangle
		vita2d_draw_rectangle(0,TEXTBOXY,SCREENWIDTH,SCREENHEIGHT-TEXTBOXY,RGBA8(255,255,255,255));

		// Draw message
		short i;
		char currentYPos=0;
		unsigned char currentXPos=-1;
		// Position in newlineSpaces that we last used the newline thing.
		unsigned char nextNewLineSpace=startSectionOfNewLine;
		
		for (i = characterOffset; i < currentLetter; i++){
			if (i==newlineSpaces[nextNewLineSpace]){
				currentXPos=0;
				currentYPos++;
				nextNewLineSpace++;
			}else{
				currentXPos++;
			}
			// currentXPos*20 has a +5 for offset so not at screen edge.
			DrawLetterUnscaled(message[i],currentXPos*20+5,TEXTBOXY+currentYPos*20+currentYPos*5,2.5);
		}

		// Draw questions
		if (isQuestion==1){
			vita2d_draw_texture_scale(yesButtonTexture,SCREENWIDTH-vita2d_texture_get_width(yesButtonTexture)*2,TEXTBOXY-vita2d_texture_get_height(yesButtonTexture)*4,2,2);
			vita2d_draw_texture_scale(noButtonTexture,SCREENWIDTH-vita2d_texture_get_width(noButtonTexture)*2,TEXTBOXY-vita2d_texture_get_height(noButtonTexture)*2,2,2);
			DrawAnimationAsISay(&selectorAnimation,SCREENWIDTH-vita2d_texture_get_width(yesButtonTexture)*2-64,TEXTBOXY-(currentSelected)*64-64,3.7);
		}
		


		vita2d_end_drawing();
		vita2d_swap_buffers();


		frames++;
		EndFrameStuff();
	}

	if (isQuestion==1){
		vita2d_free_texture(yesButtonTexture);
		vita2d_free_texture(noButtonTexture);
	}

	///////
	
	return currentSelected;
}

void UnloadMap(){
	// ... what do I need to do here?
	lua_getglobal(L,"MapDispose");
	lua_call (L,0,0);

	SetGoodArray(&(tileOtherData),1,1,1);
	int i;
	for (i=0;i<5;i++){
		SetGoodArray(&(layers[(int)i]),1,1,1);
	}
}

void ChangeMap(char* newMap){
	UnloadMap();
	LoadMap(newMap);
}


void BattleAttackTemplate(animation* animationToModify, char* filePath, char width, int speed){
	animationToModify->speed=speed;
	animationToModify->texture=vita2d_load_PNG_file(filePath);
	animationToModify->width=width;
	/*39*/animationToModify->height=vita2d_texture_get_height(animationToModify->texture);
	animationToModify->numberOfFrames=vita2d_texture_get_width(animationToModify->texture)/animationToModify->width;
	animationToModify->goBackwards=1;
	animationToModify->addOnChange=1;
	//animationToModify->drawXOffset=(75-animationToModify->width)/2;
	//animationToModify->drawYOffset=(75-animationToModify->height)/2;
}

partyMember* GetBattlerById(char id){
	if (id<=4 && id>=1){
		return &(party[(int)(id-1)]);
	}else if (id>=5 && id<=8){
		return &(enemies[(int)(id-5)]);
	}else{
		return &dummyMember;
	}
}
// Id - the id
// animationType - 1 for idle animation, 2 for attack animation
animation* GetBattlerAnimationById(char id, char animationType){
	if (id<=4 && id>=1){
		if (animationType==2){
			return &(partyAttackAnimation[(int)(id-1)]);
		}else{
			return &(partyIdleAnimation[(int)(id-1)]);
		}
	}else if (id>=5 && id<=8){
		if (animationType==2){
			return &(enemyAttackAnimation[(int)(id-5)]);
		}else{
			return &(enemyIdleAnimation[(int)(id-5)]);
		}
	}else{
		return &partyIdleAnimation[0];
	}
}

/*
========================================================
===
===      PLACES
=== ｎathan
========================================================
*/

void MenuLop(){
	signed char selected=0;

	while(1){
		StartFrameStuff();
		
		if (pad.buttons & aButton){
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
		}else if (WasJustPressed(bButton)){
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
				if (WasJustPressed(aButton)){
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

		if (WasJustPressed(SCE_CTRL_START)){// TODO - PLAY SOUND
			place=1;
		}

		UpdateCameraValues(playerObject);


		// Drawing
		vita2d_start_drawing();
		vita2d_clear_screen();

		DrawMapThings();
		//DrawText(60,60,"Hello world",2);

		vita2d_end_drawing();
		vita2d_swap_buffers();

		

		EndFrameStuff();
}

void BattleInit(){
	numberOfEnemies=2;
	enemies[0].name="test00";
	enemies[0].fighterStats.maxHp=3;
	enemies[0].fighterStats.attack=3;
	enemies[0].fighterStats.defence=3;
	enemies[0].fighterStats.speed=4;
	enemies[0].fighterStats.magicAttack=1;
	enemies[0].fighterStats.magicDefence=1;
	enemies[0].fighterStats.maxMp=0;
	enemies[0].hp=enemies[0].fighterStats.maxHp;
	enemies[0].mp=enemies[0].fighterStats.maxMp;
	enemies[1].name="test01";
	enemies[1].fighterStats.maxHp=5;
	enemies[1].fighterStats.attack=1;
	enemies[1].fighterStats.defence=1;
	enemies[1].fighterStats.speed=3;
	enemies[1].fighterStats.magicAttack=3;
	enemies[1].fighterStats.magicDefence=3;
	enemies[1].fighterStats.maxMp=0;
	enemies[1].hp=enemies[1].fighterStats.maxHp;
	enemies[1].mp=enemies[1].fighterStats.maxMp;


	BattleAttackTemplate(&(enemyAttackAnimation[0]),"app0:Stuff/Battle/TestEnemyAttack.png",32,30);
	enemyIdleAnimation[0]=enemyAttackAnimation[0];
	enemyIdleAnimation[0].numberOfFrames=1;

	enemyAttackAnimation[1]=enemyAttackAnimation[0];
	enemyIdleAnimation[1]=enemyIdleAnimation[0];


	enemies[0].x=SCREENWIDTH-225-32;
	enemies[1].x=SCREENWIDTH-225-65;
	enemies[0].y=SCREENHEIGHT-200;
	enemies[0].y=SCREENHEIGHT-300;

	party[0].x=100;
	party[0].y=200;
	party[1].x=200;
	party[1].y=400;
}

void BattleLop(){
	// 0 - Ready for next person in line
	// 1 - Choosing attack
	// 2 - Choosing target
	// 3 - Fighter moving to target
	char battleStatus=0;
	// Array that holds turn order.
	// To say that somebody moves on a certian turn, use their value.
	// 1-4 is party members
	// 5-8 is enemies
	// input 9 to indicate the end of the cycle.
	// only use 9 if there isn't 4 party and 4 enemy
	char orderOfAction[8]={0};

	char target=0;

	// Sets orderOfAction
	int i,j,k;
	//char no;

	unsigned char currentOrder=0;

	signed char selected=0;

	signed short moveXPerFrame=0;
	signed short moveYPerFrame=0;

	// This generates the orderOfAction list
	// i is the the current orderOfAction slot.
	for (i=0;i<8;i++){
		if (i==partySize+numberOfEnemies){
			orderOfAction[i]=9;
			break;
		}
		// j is the current fighter we're testig
		for (j=1;j<=8;j++){
			// If current entity speed greater than
			if (GetBattlerById(j)->fighterStats.speed>GetBattlerById(orderOfAction[i])->fighterStats.speed){
				// Loops through all of orderOfAction to make sure you're not already in orderOfAction
				for (k=0;k<=7;k++){
					if (orderOfAction[k]==j){
						break;
					}else{
						if (k==7){
							orderOfAction[i]=j;
						}
					}
				}
			}
		}
	}

	vita2d_texture* attackButton = vita2d_load_PNG_file("app0:Stuff/AttackIcon.png");
	vita2d_texture* magicButton = vita2d_load_PNG_file("app0:Stuff/MagicIcon.png");
	vita2d_texture* runButton= vita2d_load_PNG_file("app0:Stuff/RunIcon.png");
	vita2d_texture* itemButton = vita2d_load_PNG_file("app0:Stuff/ItemIcon.png");


	while (1){
		StartFrameStuff();

		if (battleStatus==1){
			if (WasJustPressed(SCE_CTRL_RIGHT)){
				selected++;
				if (selected==4){
					selected=0;
				}
			}
			if (WasJustPressed(SCE_CTRL_LEFT)){
				selected--;
				if (selected<0){
					selected=3;
				}
			}
			if (WasJustPressed(aButton)){
				if (selected==0){
					// Attack
					battleStatus=2;
					if (orderOfAction[(int)currentOrder]<=4){
						target=5;
					}
				}else if (selected==1){
					// Magic
				}else if (selected==2){
					// Item
				}else if (selected==3){
					// Run
					place=0;
					break;
				}
			}
		}else if (battleStatus==2){
			if (WasJustPressed(SCE_CTRL_RIGHT)){
				target=target+1;
				if (target>numberOfEnemies+4){
					target=5;
				}
			}
			if (WasJustPressed(SCE_CTRL_LEFT)){
				target=target-1;
				if (target<5){
					target=numberOfEnemies+4;
				}
			}
			if (WasJustPressed(aButton)){
				battleStatus=3;

				//moveXPerFrame = ceil((GetBattlerById(target)->x-GetBattlerById(orderOfAction[currentOrder])->x)/60);
				//moveYPerFrame = ceil((GetBattlerById(target)->y-(GetBattlerById(orderOfAction[currentOrder])->y + (75*3)/2) )/60);
				moveXPerFrame=3;
				moveXPerFrame=floor((GetBattlerById(target)->x - GetBattlerById(orderOfAction[currentOrder])->x - GetBattlerAnimationById(orderOfAction[currentOrder],2)->width*3+32 )/30);
				moveYPerFrame=floor((GetBattlerById(target)->y - GetBattlerById(orderOfAction[currentOrder])->y + GetBattlerAnimationById(target,1)->height/4)/30);

				if (moveXPerFrame==0){
					moveXPerFrame=1;
				}
				if (moveYPerFrame==0){
					moveYPerFrame=1;
				}

			}else if (WasJustPressed(bButton)){
				battleStatus=1;
			}
		}else if (battleStatus==3){
			GetBattlerById(orderOfAction[currentOrder])->x=GetBattlerById(orderOfAction[currentOrder])->x+moveXPerFrame;
			GetBattlerById(orderOfAction[currentOrder])->y=GetBattlerById(orderOfAction[currentOrder])->y+moveYPerFrame;
			
			if (moveXPerFrame>=0 && GetBattlerById(orderOfAction[currentOrder])->x>=GetBattlerById(target)->x-GetBattlerAnimationById(orderOfAction[currentOrder],2)->width*3){
				moveXPerFrame=0;
			}
			if (moveXPerFrame<=0 && GetBattlerById(orderOfAction[currentOrder])->x<=GetBattlerById(target)->x+GetBattlerAnimationById(orderOfAction[currentOrder],2)->width*3){
				moveXPerFrame=0;
			}
			if (moveYPerFrame>=0 && GetBattlerById(orderOfAction[currentOrder])->y>=GetBattlerById(target)->y){
				moveYPerFrame=0;
			}
			if (moveYPerFrame<=0 && GetBattlerById(orderOfAction[currentOrder])->y<=GetBattlerById(target)->y){
				moveYPerFrame=0;
			}		
		}

		#if NOSLEEP==1
			sceKernelPowerTick(0);
		#endif

		// Alright, time to go to next turn
		if (battleStatus==0){
			if (orderOfAction[(int)currentOrder]<=4){
				battleStatus=1;
			}else if (orderOfAction[(int)currentOrder]<=8){
				// generate enmy turn
				ShowMessage("ENemy twern.",0);
			}else{
				// We're on 9 or some bad number.
				// Start new turn rotation.
				currentOrder=0;
			}
		}

		vita2d_start_drawing();
		vita2d_clear_screen();
	
		DrawMap();
		
		DrawUnusedAreaRect();

		// Draw selection buttons
		if (battleStatus==1){
			// Draw UI and selector
			vita2d_draw_texture_scale(attackButton,32,SCREENHEIGHT-vita2d_texture_get_height(attackButton)*2,2,2);
			vita2d_draw_texture_scale(magicButton,264,SCREENHEIGHT-vita2d_texture_get_height(magicButton)*2,2,2);
			vita2d_draw_texture_scale(itemButton,496,SCREENHEIGHT-vita2d_texture_get_height(itemButton)*2,2,2);
			vita2d_draw_texture_scale(runButton,728,SCREENHEIGHT-vita2d_texture_get_height(itemButton)*2,2,2);
			UpdateAnimationIfNeed(&selectorAnimation);
			vita2d_draw_texture_part_scale_rotate(selectorAnimation.texture,selected*200+selected*32+132,SCREENHEIGHT-128,selectorAnimation.currentFrame*selectorAnimation.width,0,selectorAnimation.width,selectorAnimation.height,3.7,3.7,1.57);
		}else if (battleStatus==2){
			// Draw the target selector
			DrawAnimationAsISay(&selectorAnimation,GetBattlerById(target)->x-64,GetBattlerById(target)->y + GetBattlerAnimationById(target,1)->height ,3.7);
		}

		for (i=0;i<partySize;i++){
			DrawAnimationAsISay(&(partyIdleAnimation[i]),party[i].x,party[i].y,3);
		}
		for (i=0;i<numberOfEnemies;i++){
			DrawAnimationAsISay(&enemyIdleAnimation[i],enemies[i].x,enemies[i].y,3);
		}

		vita2d_end_drawing();
		vita2d_swap_buffers();
	
		EndFrameStuff();
	}

	vita2d_wait_rendering_done();
	vita2d_free_texture(attackButton);
	vita2d_free_texture(magicButton);
	vita2d_free_texture(itemButton);
	vita2d_free_texture(runButton);

	// Make sure no rapid battle lop
	if (place==3){
		ShowMessage("That shouldn't have happenedededed.",0);
		place=0;
	}
}

/*
///////////////////////////////////////
// LUA FUNCTIONS
// Alright, so I had this idea. Instead of having set and get
// functions, I could just send pointers as light user data and
// then have a function to set those pointers.
///////////////////////////////////////
*/

// X, Y, layer, Tileset, Tile
int L_SetMapImageData(lua_State* passedState){
	tileImageData* _theChosenData = GetMapImageData(lua_tonumber(passedState,1),lua_tonumber(passedState,2),lua_tonumber(passedState,3));
	_theChosenData->tileset=lua_tonumber(passedState,4);
	_theChosenData->tile=lua_tonumber(passedState,5);
	return 0;
}

// Sets an int pointer to a value.
// ARGS - 
// intpointer, value (int)
int L_SetInt(lua_State* passedState){
	int* passedInt = lua_touserdata(passedState,1);
	int toSet = lua_tonumber(passedState,2);
	(*passedInt)=toSet;
	return 0;
}
// Shows a message or a question
// ARGS - 
// message (string), isQuestion (bool)
// Return - 
// if isQuestion==true, returns true if answered yes, false if answered no.
int L_ShowMessage(lua_State* passedState){
	char* passedMessage = lua_tostring(passedState,1);
	char passedIsQuestion = lua_toboolean(passedState,2);
	char messageAnswer = ShowMessage(passedMessage,passedIsQuestion);
	if (passedIsQuestion!=0){
		lua_pushboolean(passedState,messageAnswer);
		return 1;
	}
	return 0;
}

// Loads a PNG.
// ARGS - 
// path (string)
// Return - 
// an image pointer
int L_LoadPNG(lua_State* passedState){
	char* passedPath = lua_tostring(passedState,1);
	lua_pushlightuserdata(passedState,vita2d_load_PNG_file(passedPath));
	return 1;
}

// Destroys an image.
// ARGS - 
// some_imgage_pointer
int L_UnloadTexture(lua_State* passedState){
	vita2d_wait_rendering_done();
	vita2d_free_texture(lua_touserdata(passedState,1));
	return 0;
}

// Set tileset slot to an image
// ARGS - 
// image, slot
int L_SetTileset(lua_State* passedState){
	tilesets[(int)lua_tonumber(passedState,2)]=lua_touserdata(passedState,1);
	return 0;
}


// Makes Lua usefull.
void PushCFunctions(){
	lua_pushcfunction(L,L_SetMapImageData);
	lua_setglobal(L,"SetMapImageData");
	lua_pushcfunction(L,L_SetInt);
	lua_setglobal(L,"SetInt");
	lua_pushcfunction(L,L_ShowMessage);
	lua_setglobal(L,"ShowMessage");
	lua_pushcfunction(L,L_LoadPNG);
	lua_setglobal(L,"LoadPNG");
	lua_pushcfunction(L,L_UnloadTexture);
	lua_setglobal(L,"UnloadTexture");
	lua_pushcfunction(L,L_SetTileset);
	lua_setglobal(L,"SetTileset");
}

/*
///////////////////////////////////////
// LUA FUNCTIONS END
///////////////////////////////////////
*/


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
	
	L = luaL_newstate();
	luaL_openlibs(L);
	PushCFunctions();
	//luaL_dofile(L,"app0:Stuff/Happy.lua");

	LoadMap("app0:Stuff/test");


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
	mapObjects[0].theAnimation.drawXOffset=7;
	mapObjects[0].theAnimation.addOnChange=1;

	(playerObject)=&(mapObjects[0]);

	SetCameraValues();

	fontImage=vita2d_load_PNG_file("app0:Stuff/Font.png");

	dummyMember.name="Dummy";
	dummyMember.fighterStats.maxHp=0;
	dummyMember.fighterStats.attack=0;
	dummyMember.fighterStats.defence=0;
	dummyMember.fighterStats.magicDefence=0;
	dummyMember.fighterStats.magicAttack=0;
	dummyMember.fighterStats.speed=0;
	dummyMember.hp=0;
	dummyMember.mp=0;

	party[0].name=playerName;
	party[0].fighterStats.maxHp=15;
	party[0].fighterStats.attack=4;
	party[0].fighterStats.defence=4;
	party[0].fighterStats.magicDefence=1;
	party[0].fighterStats.magicAttack=1;
	party[0].fighterStats.speed=15;
	party[0].fighterStats.maxMp=5;
	party[0].hp=party[0].fighterStats.maxHp;
	party[0].mp=party[0].fighterStats.maxMp;
	//
	party[1].name="Amigo";
	party[1].fighterStats.maxHp=26;
	party[1].fighterStats.attack=1;
	party[1].fighterStats.defence=1;
	party[1].fighterStats.magicDefence=4;
	party[1].fighterStats.magicAttack=4;
	party[1].fighterStats.speed=5;
	party[1].fighterStats.maxMp=5;
	party[1].hp=party[1].fighterStats.maxHp;
	party[1].mp=party[1].fighterStats.maxMp;

	partyAttackAnimation[0].speed=30;
	partyAttackAnimation[0].texture=vita2d_load_PNG_file("app0:Stuff/Battle/PlayerAttack.png");
	partyAttackAnimation[0].width=44;
	/*39*/partyAttackAnimation[0].height=vita2d_texture_get_height(partyAttackAnimation[0].texture);
	partyAttackAnimation[0].numberOfFrames=vita2d_texture_get_width(partyAttackAnimation[0].texture)/partyAttackAnimation[0].width;
	partyAttackAnimation[0].goBackwards=1;
	partyAttackAnimation[0].addOnChange=1;
	//partyAttackAnimation[0].drawXOffset=(75-partyAttackAnimation[0].width)/2;
	//partyAttackAnimation[0].drawYOffset=(75-partyAttackAnimation[0].height)/2;

	partyIdleAnimation[0]=partyAttackAnimation[0];
	partyIdleAnimation[0].numberOfFrames=1;

	partyAttackAnimation[1].speed=30;
	partyAttackAnimation[1].texture=vita2d_load_PNG_file("app0:Stuff/Battle/TestMemberAttack.png");
	partyAttackAnimation[1].width=25;
	partyAttackAnimation[1].height=vita2d_texture_get_height(partyAttackAnimation[1].texture);
	partyAttackAnimation[1].numberOfFrames=vita2d_texture_get_width(partyAttackAnimation[1].texture)/partyAttackAnimation[1].width;
	partyAttackAnimation[1].goBackwards=1;
	partyAttackAnimation[1].addOnChange=1;
	//partyAttackAnimation[1].drawXOffset=(75-partyAttackAnimation[1].width)/2;
	//partyAttackAnimation[1].drawYOffset=(75-partyAttackAnimation[1].height)/2;

	partyIdleAnimation[1]=partyAttackAnimation[1];
	partyIdleAnimation[1].numberOfFrames=1;

	partySize=2;
}

int main(){
	Init();	
	//tilesets[0] = vita2d_load_PNG_file("app0:Stuff/a.png");

	//ChangeMap("app0:Stuff/test");
	//ChangeMap("app0:Stuff/test");
	//ChangeMap("app0:Stuff/test");
	//ChangeMap("app0:Stuff/test");

	/*
		0 - Overworld
		1 - Pause in Overworld
		2 - Quit (breaks)
		3 - Battle
	*/
	
	BattleInit();

	place=3;

	while (1){
		if (place==0){
			Overworld();
		}else if (place==1){
			MenuLop();
		}else if (place==2){
			break;
		}else if (place==3){
			BattleLop();
		}
	}
	

	// End this stuff
	vita2d_fini();
	vita2d_free_pgf(defaultPgf);
	// I really don't get why I need this when the user can just use the PS button.
	sceKernelExitProcess(0);
	return 0;
}