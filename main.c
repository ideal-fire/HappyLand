// Support acsents by using string similar to english international keyboard
// 'a for acsent on a

/*
---O---OO--OOOOO-O-OO-OOOOO----O---O
--O-O-O-O--O---O-OO---O---O---O-O-O-
-O--OO--O--O---O-O----O---O--O--OO--
O---O---O--OOOOO-O----OOOOO-O---O---
*/

// Only turned off when I'm working on my game. Needs to be set to 1 before release
#define SHOWSPLASH 1

//#define LANGUAGE LANG_SPANISH
#define LANG_ENGLISH 1
#define LANG_SPANISH 2
int LANGUAGE=LANG_ENGLISH;


// Translatos for hardcoded strings
#include "HardcodedLanguage.h"

#define MAPXSCALE 2
#define MAPYSCALE 2
#define MAXOBJECTS 15

#define SCREENWIDTH 960
#define SCREENHEIGHT 544

// Not needed. Can use registry if you don't want to sleep.
#define NOSLEEP 0

#define TEXTBOXY 420

// UNCHANGED BY SCALE
int drawXOffset=0;
int drawYOffset=0;

// Include cool libraries and stuff
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/rtc.h>
#include <psp2/types.h>
#include <psp2/touch.h>
#include <psp2/display.h>

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
		unsigned short level;
		short maxHp;
		short maxMp;
		unsigned short attack;
		unsigned short defence;
		unsigned short magicAttack;
		unsigned short magicDefence;
		unsigned short speed;
		unsigned short exp; // For enemies, this is their exp reward. For players, it's their current exp.
		char spells[10];
		char* name;
	}stats;
	typedef struct Q_partyMember{
		stats fighterStats;
		int x; // Position on screen //
		int y; // Position on screen //
		short hp;
		short mp;
	}partyMember;
	typedef struct Q_spell{
		char* name;
		signed char attack;
		signed char magicAttack;
		char* spellPath;
		animation theAnimation;
		char mpCost;
		unsigned char lastMinuteFix;
	}spell;
	typedef struct Q_spellLinkedList{
		struct Q_spellLinkedList* nextEntry;
		spell theSpell;
	}spellLinkedList;
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

spellLinkedList* spellLinkedListStart;
unsigned short spellLinkedListSize=1;

/*
==================
== TILDE
==================
*/
vita2d_texture* tilde_a;
vita2d_texture* tilde_e;
vita2d_texture* tilde_i;
vita2d_texture* tilde_o;
vita2d_texture* tilde_u;
vita2d_texture* tilde_n;
vita2d_texture* tilde_questionMark;
vita2d_texture* tilde_esclimationPoint;

/*
====================================================
== OPTIONS
===================================================
*/
// Max I think I'll make 8 chars.
char* playerName="Nathan";
int aButton=SCE_CTRL_CROSS;
int bButton=SCE_CTRL_CIRCLE;
char textboxNewCharSpeed=1;
char defaultSelected=1;
SceTouchData currentTouch;
SceTouchData previousTouchData;

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
short encounterRate=0;
short nextEncounter=0;
/*
=================================================
== BATTLE
=================================================
*/
partyMember party[4];
animation partyAttackAnimation[4];
animation partyIdleAnimation[4];
partyMember enemies[4];
animation* enemyAttackAnimation[4];
animation* enemyIdleAnimation[4];
char partySize;
char numberOfEnemies;
partyMember dummyMember;
// Enemies you can encounter here.
stats possibleEnemies[10];
animation possibleEnemiesIdleAnimation[10];
animation possibleEnemiesAttackAnimation[10];

// pl0x, I beg you, don't crash when I'm disposing textures.
void PlzNoCrashOnDispose(){
	vita2d_wait_rendering_done();
	sceDisplayWaitVblankStart();
}

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
	sceTouchPeek(SCE_TOUCH_PORT_FRONT, &currentTouch, 1);
}
void EndFrameStuff(){
	FpsCapWait();
	previousTouchData=currentTouch;
}

// Resets an animation
void ResetAnimation(animation* passedAnimation){
	passedAnimation->lastChange = numberOfFrames;
	passedAnimation->currentFrame = 0;
	passedAnimation->addOnChange = 1;
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
	if (x<0 || y<0 || y>=tileOtherData.height || x>=tileOtherData.width){
		return 1;
	}
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

char IsThisTilde(char* passedString, int passedPosition){
	if (passedString[passedPosition]==39){
		if (passedString[passedPosition+1]==97 || passedString[passedPosition+1]==101 || passedString[passedPosition+1]==105 || passedString[passedPosition+1]==111 || passedString[passedPosition+1]==117 || passedString[passedPosition+1]==110 || passedString[passedPosition+1]==63 || passedString[passedPosition+1]==33){
			return 1;
		}
	}
	return 0;
}

void DrawLetter(int letterId, int _x, int _y, int size){
	//DrawTexture(_mainRenderer,fontImage, (letterId-32)*8, 0, 8, 8, _x, _y, 16, 16);
	vita2d_draw_texture_part_scale(fontImage,_x,_y,(letterId-32)*(8),0,8,8,MAPXSCALE*size,MAPYSCALE*size);
}

void DrawLetterUnscaled(int letterId, int _x, int _y, int size){
	vita2d_draw_texture_part_scale(fontImage,_x,_y,(letterId-32)*(8),0,8,8,size,size);
}

void DrawTildeLetterUnscaled(int letterId, int _x, int _y, int size){
	switch(letterId){
		case 97:
			vita2d_draw_texture_scale(tilde_a,_x,_y,size,size);
		break;
		case 101:
			vita2d_draw_texture_scale(tilde_e,_x,_y,size,size);
		break;
		case 105:
			vita2d_draw_texture_scale(tilde_i,_x,_y,size,size);
		break;
		case 111:
			vita2d_draw_texture_scale(tilde_o,_x,_y,size,size);
		break;
		case 117:
			vita2d_draw_texture_scale(tilde_u,_x,_y,size,size);
		break;
		case 110:
			vita2d_draw_texture_scale(tilde_n,_x,_y,size,size);
		break;
		case 63:
			vita2d_draw_texture_scale(tilde_questionMark,_x,_y,size,size);
		break;
		case 33:
			vita2d_draw_texture_scale(tilde_esclimationPoint,_x,_y,size,size);
		break;
	}
}

void DrawText(int x, int y, const char* text, int size){
	int i=0;
	int notICounter=0;
	for (i = 0; i < strlen(text); i++){
		if (5==39){
			DrawTildeLetterUnscaled(text[i+1],FixX(x+(notICounter*(8*size))+notICounter),FixY(y),size);
			i++;
		}else{
			DrawLetter(text[i],FixX(x+(notICounter*(8*size))+notICounter),FixY(y),size);
		}
		notICounter++;
	}
}

void DrawTextAsISay(int x, int y, const char* text, int size){
	int i=0;
	int notICounter=0;
	for (i = 0; i < strlen(text); i++){
		if (IsThisTilde((char*)text,i)){
			DrawTildeLetterUnscaled(text[i+1],(x+(notICounter*(8*size))+notICounter),(y),size);
			i++;
		}else{
			DrawLetterUnscaled(text[i],(x+(notICounter*(8*size))+notICounter),(y),size);
		}
		notICounter++;
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

void BasicMessage(char* message){


	// Letter we're currently displaying.
	short currentLetter=0;

	char quitTextbox=0;

	// Letter scale is 2.5. 8*2.5=20.
	unsigned char maxLetters=floor(960/(20));

	short newlineSpaces[50];
	char totalNewLines=0;


	int i=0;
	int j=0;

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

	currentLetter=newlineSpaces[(int)sectionOfCurrentMessage];

	while (!quitTextbox){
		StartFrameStuff();
			
		if (WasJustPressed(aButton)){
			
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
				currentLetter=newlineSpaces[(int)sectionOfCurrentMessage];
			}else{
				quitTextbox=1;
				
			}
			
		}

		vita2d_start_drawing();
		vita2d_clear_screen();

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
			if (IsThisTilde(message,i)){
				DrawTildeLetterUnscaled(message[i+1],currentXPos*20+5,TEXTBOXY+currentYPos*20+currentYPos*5+5,2.5);
				i++;
				continue;
			}
			// currentXPos*20 has a +5 for offset so not at screen edge.
			DrawLetterUnscaled(message[i],currentXPos*20+5,TEXTBOXY+currentYPos*20+currentYPos*5+5,2.5);
		}


		vita2d_end_drawing();
		vita2d_swap_buffers();

		EndFrameStuff();
	}
}

char ShowMessageWithPortrait(char* message, char isQuestion, vita2d_texture* portrait, double portScale){
	if (portScale==0 && portrait!=NULL){
		portScale = floor((double)200/vita2d_texture_get_width(portrait));
	}

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
	vita2d_texture* yesButtonTexture=(vita2d_texture*)1;
	vita2d_texture* noButtonTexture=(vita2d_texture*)1;
	//vita2d_wait_rendering_done();

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
			if (IsThisTilde(message,i)){
				DrawTildeLetterUnscaled(message[i+1],currentXPos*20+5,TEXTBOXY+currentYPos*20+currentYPos*5+5,2.5);
				i++;
				continue;
			}
			// currentXPos*20 has a +5 for offset so not at screen edge.
			DrawLetterUnscaled(message[i],currentXPos*20+5,TEXTBOXY+currentYPos*20+currentYPos*5+5,2.5);
		}

		// Draw questions
		if (isQuestion==1){
			vita2d_draw_texture_scale(yesButtonTexture,SCREENWIDTH-vita2d_texture_get_width(yesButtonTexture)*2,TEXTBOXY-vita2d_texture_get_height(yesButtonTexture)*4,2,2);
			vita2d_draw_texture_scale(noButtonTexture,SCREENWIDTH-vita2d_texture_get_width(noButtonTexture)*2,TEXTBOXY-vita2d_texture_get_height(noButtonTexture)*2,2,2);
			DrawAnimationAsISay(&selectorAnimation,SCREENWIDTH-vita2d_texture_get_width(yesButtonTexture)*2-64,TEXTBOXY-(currentSelected)*64-50,3.7);
		}
		
		if (portrait!=NULL){
			vita2d_draw_texture_scale(portrait,0,TEXTBOXY-vita2d_texture_get_height(portrait)*portScale,portScale,portScale);
		}

		vita2d_end_drawing();
		vita2d_swap_buffers();


		frames++;
		EndFrameStuff();
	}

	if (isQuestion==1){
		PlzNoCrashOnDispose();
		vita2d_free_texture(yesButtonTexture);
		vita2d_free_texture(noButtonTexture);
	}

	///////
	
	return currentSelected;
}

char StrLenTildes(char* passedString){
	int i=0;
	int length=0;
	for (i=0;i<strlen(passedString);i++){
		if (passedString[i]!=39){
			length++;
		}
	}
	return length;
}

char ShowMessage(char* message, char isQuestion){
	return ShowMessageWithPortrait(message,isQuestion,NULL,0);
}

void LoadMap(char* path){
	FILE* openedFile;
	openedFile = fopen(path,"r");

	unsigned short _width;
	unsigned short _height;

	
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

// TODO - reset and dispose and stuff enemies
void UnloadMap(){
	// ... what do I need to do here?
	PlzNoCrashOnDispose();
	lua_getglobal(L,"MapDispose");
	lua_call (L,0,0);
	SetGoodArray(&(tileOtherData),1,1,1);
	int i;
	for (i=0;i<5;i++){
		SetGoodArray(&(layers[i]),1,1,1);
	}

	//// Free enemy animation
	PlzNoCrashOnDispose();
	for (i=0;i<10;i++){
		if (possibleEnemies[i].maxHp!=-1){
			possibleEnemies[i].maxHp=-1;
			if (possibleEnemiesAttackAnimation[i].texture!=NULL){
				vita2d_free_texture(possibleEnemiesAttackAnimation[i].texture);
				possibleEnemiesAttackAnimation[i].texture=NULL;
			}
			if (possibleEnemiesIdleAnimation[i].texture!=NULL){
				vita2d_free_texture(possibleEnemiesIdleAnimation[i].texture);
				possibleEnemiesIdleAnimation[i].texture=NULL;
			}
		}
	}
	
	// Set all enemy pointers to null
	for (i=0;i<4;i++){
		enemies[i].hp=-1;
		enemyIdleAnimation[i]=NULL;
		enemyAttackAnimation[i]=NULL;
	}
	for (i=0;i<10;i++){
		// -1 maxHp means that the slot isn't used.
		possibleEnemies[i].maxHp=-1;
	}
}

void ChangeMap(char* newMap){
	encounterRate=0;
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
			return (enemyAttackAnimation[(int)(id-5)]);
		}else{
			return (enemyIdleAnimation[(int)(id-5)]);
		}
	}else{
		return &partyIdleAnimation[0];
	}
}

int CalculateDamage(int moveAttack, int moveMagicAttack, int fighterAttack, int fighterMagicAttack, int victimDefence, int victimMagicDefence){
	return floor(((double)fighterAttack/victimDefence)*moveAttack+((double)fighterMagicAttack/victimMagicDefence)*moveMagicAttack);
}

int Damage(partyMember* attacker, partyMember* victim, int moveAttack, int moveMagicAttack, int lastMinuteFix){
	int doneDamage;
	if (lastMinuteFix==1){
		doneDamage=moveAttack+moveMagicAttack;
	}else{
		doneDamage = CalculateDamage(moveAttack,moveMagicAttack,attacker->fighterStats.attack,attacker->fighterStats.magicAttack,victim->fighterStats.defence,victim->fighterStats.magicDefence);
	}
	victim->hp-=doneDamage;
	if (victim->hp<0){
		victim->hp=0;
	}
	if (victim->hp>victim->fighterStats.maxHp+20){
		victim->hp=victim->fighterStats.maxHp+20;
	}
	return doneDamage;
}

void WaitForPush(int button){
	while (!(pad.buttons & button)){
		sceCtrlPeekBufferPositive(0, &pad, 1);
	}
}

// Includes left and right bounds
int RandBetween(int leftBound, int rightBound){
	rightBound++;
	return ((rand()%(rightBound-leftBound))+leftBound);
}

void InitWildBattle(){
	int i=0;

	int firstRandomA = RandBetween(1,4);
	if (firstRandomA>2){
		int firstRandomB = RandBetween(2,4);
		numberOfEnemies=firstRandomB;
	}else{
		numberOfEnemies=firstRandomA;
	}


	int numberOfPossibleEnemies=10;
	for (i=0;i<10;i++){
		if (possibleEnemies[i].maxHp==-1){
			numberOfPossibleEnemies=i;
			break;
		}
	}

	for (i=0;i<numberOfEnemies;i++){
		firstRandomA=RandBetween(0,numberOfPossibleEnemies-1);
		enemies[i].fighterStats=(possibleEnemies[firstRandomA]);
		enemies[i].hp=enemies[i].fighterStats.maxHp;
		enemyIdleAnimation[i]=&(possibleEnemiesIdleAnimation[firstRandomA]);
		enemyAttackAnimation[i]=&(possibleEnemiesAttackAnimation[firstRandomA]);
	}
}

void BattleInit(){
	//numberOfEnemies=1;

	int i=0;
	
	// Generates random positions for characters.
	
	for (i=0;i<numberOfEnemies;i++){
		enemies[i].y=RandBetween(106, 416-enemyIdleAnimation[i]->height*2);
		enemies[i].x=RandBetween(SCREENWIDTH/2,SCREENWIDTH-enemyIdleAnimation[i]->width*2);
	}
	for (i=0;i<partySize;i++){
		party[i].y=RandBetween(106, 416-partyIdleAnimation[i].height*2);
		party[i].x=RandBetween(64,SCREENWIDTH/2-partyIdleAnimation[i].width*2);
	}

	for (i=partySize;i<4;i++){
		party[i].hp=0;
	}	
}

spellLinkedList* GetSpellList(int num){
	spellLinkedList* listOn = spellLinkedListStart;
	int i=1;
	for (i=1;i<=num;i++){
		listOn=listOn->nextEntry;
	}
	return listOn;
}

spellLinkedList* AddToSpellList(){
	spellLinkedList* tempList = calloc(1,sizeof(spellLinkedList));
	spellLinkedList* listOn = GetSpellList(spellLinkedListSize-1);

	if (listOn->nextEntry){
		// ??? There's already another entry???
		ShowMessage("Aw snap. Problem add to list. Maybe we'll loose some entries.",0);
	}
	listOn->nextEntry=tempList;
	spellLinkedListSize++;

	return tempList;
}

int CenterSomething(int width){
	return (SCREENWIDTH/2-floor(((double)width)/2));
}

// Returns x position for this to be centered
int CenterText(char* text, char scale){
	return CenterSomething(strlen(text)*(scale*8));
}

// Returns spell id selected
signed char SelectSpell(partyMember member){
	unsigned char selected=0;
	char tempMessage[30];
	short numberOfSkillsSelect=0;

	int i=0;
	for (i=0;i<10;i++){
		if (member.fighterStats.spells[i]!=0){
			numberOfSkillsSelect++;
		}else{
			break;
		}
	}

	// No haz magic
	if (numberOfSkillsSelect==0){
		return -1;
	}

	while (1){
		StartFrameStuff();

		if (WasJustPressed(SCE_CTRL_DOWN)){
			selected++;

			if (selected==numberOfSkillsSelect){
				selected=0;
			}
		}else if (WasJustPressed(SCE_CTRL_UP)){
			selected--;
			// Unsigned char go from -1 to 255
			if (selected>100){
				selected=numberOfSkillsSelect-1;
			}
		}else if (WasJustPressed(aButton)){
			if (member.fighterStats.spells[selected]!=0){
				if (GetSpellList(member.fighterStats.spells[selected]-1)->theSpell.mpCost>member.mp){
					ShowMessage(N_NEEDMOREMP,0);
				}else{
					return member.fighterStats.spells[selected]-1;
				}
			}
		}else if (WasJustPressed(bButton)){
			return -1;
		}

		vita2d_start_drawing();
		vita2d_clear_screen();

		DrawTextAsISay( CenterText(N_MAGICLIST,6),2,N_MAGICLIST,6);

		
		for (i=0;i<numberOfSkillsSelect;i++){
			if (member.fighterStats.spells[i]!=0){
				//ShowMessage("a",0);
				spellLinkedList* tempList = GetSpellList(member.fighterStats.spells[i]-1);
				sprintf((char*)tempMessage,"%s:%d",tempList->theSpell.name,tempList->theSpell.mpCost);
				DrawTextAsISay(88,i*48+i*2+64,tempMessage,6);
			}else{
				break;
			}
		}
		//ShowMessage("b",0);
		DrawAnimationAsISay(&selectorAnimation,0,selected*48+selected*2+64-10,4);

		vita2d_end_drawing();
		vita2d_swap_buffers();

		EndFrameStuff();
	}
}

void DisposeAllLoadedSpellImages(){
	int i=0;
	spellLinkedList* gotList;
	for (i=0;i<spellLinkedListSize;i++){
		gotList = GetSpellList(i);
		if (gotList->theSpell.theAnimation.texture!=NULL){
			vita2d_free_texture(gotList->theSpell.theAnimation.texture);
			gotList->theSpell.theAnimation.texture=NULL;
		}
	}
}

void LoadSpellImageIfNeeded(int spellId){
	spell* gotSpell = &(GetSpellList(spellId)->theSpell);
	if (gotSpell->theAnimation.texture==NULL){
		gotSpell->theAnimation.texture = vita2d_load_PNG_file(gotSpell->spellPath);
	}
}

void AutodetectNumberOfFrames(animation* passedAnimation){
	passedAnimation->numberOfFrames = (vita2d_texture_get_width(passedAnimation->texture)/passedAnimation->width);
}

char DidJustTouch(){
	if (currentTouch.reportNum>0 && previousTouchData.reportNum==0){
		return 1;
	}else{
		return 0;
	}
}

void AddSpellToStats(stats* passedMember, int passedSpellId){
	int i=0;
	for (i=0;i<10;i++){
		if (passedMember->spells[i]==passedSpellId){
			break;
		}
		if (passedMember->spells[i]==0){
			passedMember->spells[i]=passedSpellId;
			break;
		}
	}
}

char GetNumberOfSpells(stats* passedStats){
	int numberOfSpells=0;
	int i=0;
	for (i=0;i<10;i++){
		if (passedStats->spells[i]!=0){
			numberOfSpells++;
		}
	}
	return numberOfSpells;
}

/*
========================================================
===
===      PLACES
=== ｎathan
========================================================
*/


void StatusLoop(){
	signed char selectedMember=0;
	char statNumberString[10];
	while (1){
		StartFrameStuff();
		if (WasJustPressed(bButton)){
			place=1;
			break;
		}else if (WasJustPressed(SCE_CTRL_LEFT)){
			selectedMember--;
			if (selectedMember<0){
				selectedMember=partySize-1;
			}
		}else if (WasJustPressed(SCE_CTRL_RIGHT)){
			selectedMember++;
			if (selectedMember==partySize){
				selectedMember=0;
			}
		}
		vita2d_start_drawing();
		vita2d_clear_screen();

		DrawTextAsISay(SCREENWIDTH/2-strlen(party[selectedMember].fighterStats.name)*64/2,3,party[selectedMember].fighterStats.name,8);
		DrawTextAsISay(3,69,N_ATK,4);
		sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.attack);
		DrawTextAsISay(strlen(N_ATK)*32+32,69,statNumberString,4);
		DrawTextAsISay(3,106,N_MATK,4);
		sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.magicAttack);
		DrawTextAsISay(strlen(N_MATK)*32+32,106,statNumberString,4);
		DrawTextAsISay(3,143,N_DEF,4);
		sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.defence);
		DrawTextAsISay(strlen(N_DEF)*32+32,143,statNumberString,4);
		DrawTextAsISay(3,180,N_MDEF,4);
		sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.magicDefence);
		DrawTextAsISay(strlen(N_MDEF)*32+32,180,statNumberString,4);
		DrawTextAsISay(3,217,N_SPEED,4);
		sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.speed);
		DrawTextAsISay(strlen(N_SPEED)*32+32,217,statNumberString,4);

		sprintf((char*)&statNumberString,"%d/%d",party[selectedMember].hp,party[selectedMember].fighterStats.maxHp);
		DrawTextAsISay(strlen(N_HP)*32+32,254,statNumberString,4);
		DrawTextAsISay(3,254,N_HP,4);
		sprintf((char*)&statNumberString,"%d/%d",party[selectedMember].mp,party[selectedMember].fighterStats.maxMp);
		DrawTextAsISay(strlen(N_MP)*32+32,291,statNumberString,4);
		DrawTextAsISay(3,291,N_MP,4);
		
		sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.level);
		DrawTextAsISay(strlen(N_LV)*32+32,328,statNumberString,4);
		DrawTextAsISay(3,328,N_LV,4);
		
		double animationScale=(SCREENHEIGHT-180)/ partyIdleAnimation[selectedMember].height;
		DrawAnimationAsISay(&partyIdleAnimation[selectedMember],SCREENWIDTH-partyIdleAnimation[selectedMember].width*animationScale-100,180,animationScale);


		vita2d_end_drawing();
		vita2d_swap_buffers();
		EndFrameStuff();
	}
}

void MenuLop(){
	signed char selected=0;
	// 0 - menu
	// 1 - options
	char subspot=0;

	//char tempStringAssembly[5];

	while(1){
		StartFrameStuff();
		
		if (WasJustPressed(aButton)){
			if (subspot==0){
				switch(selected){
					case 0:
						place=0;
						return;
					break;
					case 1:
						place=4;
						return;
					break;
					case 3:
						if (LANGUAGE==LANG_ENGLISH){
							LANGUAGE=LANG_SPANISH;
						}else if (LANGUAGE==LANG_SPANISH){
							LANGUAGE=LANG_ENGLISH;
						}
						SetupHardcodedLanguage();
						if (LANGUAGE == LANG_ENGLISH){
							lua_pushnumber(L,1);
						}else if (LANGUAGE==LANG_SPANISH){
							lua_pushnumber(L,2);
						}
						lua_setglobal(L,"Lang");

						break;
					case 4:
						subspot=1;
						selected=0;
					break;
					case 2:
						place=2;
						return;
					break;
				}
			}
		}else if (WasJustPressed(SCE_CTRL_START)){
			place=0;
			return;
		}else if (WasJustPressed(bButton)){
			if (subspot==0){
				place=0;
			}else{
				subspot=0;
			}
			return;
		}
		if (WasJustPressed(SCE_CTRL_DOWN)){
			selected++;
			if (selected>2){
				selected=0;
			}
		}else if (WasJustPressed(SCE_CTRL_UP)){
			selected--;
			if (selected<0){
				selected=2;
			}
		}else if (WasJustPressed(SCE_CTRL_RIGHT)){
			if (subspot==1){
				if (selected==0){
					textboxNewCharSpeed--;
					if (textboxNewCharSpeed==0){
						textboxNewCharSpeed=1;
					}
				}

			}
		}else if (WasJustPressed(SCE_CTRL_LEFT)){
			if (subspot==1){
				if (selected==0){
					textboxNewCharSpeed++;
					if (textboxNewCharSpeed==0){
						textboxNewCharSpeed=1;
					}
				}
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

		if (subspot==0){
			DrawTextAsISay(CenterText(N_HAPPYMENU,4),146,N_HAPPYMENU,4);
			DrawTextAsISay(287,183,N_RESUME,4);
			DrawTextAsISay(287,183+32+5,playerName,4);
			//DrawTextAsISay(287,183+64+10,N_OPTIONS,4);
			/*if (LANGUAGE==LANG_ENGLISH){
				DrawTextAsISay(287,183+64+15,"Espa'nol",4);
			}else if (LANGUAGE==LANG_SPANISH){
				DrawTextAsISay(287,183+64+15,"English",4);
			}*/
			DrawTextAsISay(287,183+64+15,N_QUIT,4);
		}else if (subspot==1){
			/*DrawTextAsISay(320+32,146,N_OPTIONS,4);
			DrawTextAsISay(287,183,N_TEXTSPEED,4);
			sprintf((char*)&tempStringAssembly,"%d",255-textboxNewCharSpeed);
			DrawTextAsISay(287+strlen(N_TEXTSPEED)*32+32,183,tempStringAssembly,4);*/
		}
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
				nextEncounter--;

				// Player reached their destination
				tileSpotData* eventTempSpotData = GetMapSpotData(playerObject->x/32,playerObject->y/32);
				if (eventTempSpotData->event!=0){
					ExecuteEvent(playerObject,eventTempSpotData->event);
				}

				if (nextEncounter<=0 && encounterRate>=1){
						nextEncounter = encounterRate+RandBetween(0,5);

						InitWildBattle();
						BattleInit();
						place=3;

						playerObject->theAnimation.addOnChange=1;
						playerObject->theAnimation.numberOfFrames=1;
						playerObject->theAnimation.currentFrame=0;
						isWalking=0;
				}else{
					isWalking=0;
					if (!((pad.buttons & SCE_CTRL_UP) || (pad.buttons & SCE_CTRL_DOWN) || (pad.buttons & SCE_CTRL_LEFT) || (pad.buttons & SCE_CTRL_RIGHT))){
						// Stop
						playerObject->theAnimation.addOnChange=1;
						playerObject->theAnimation.numberOfFrames=1;
						playerObject->theAnimation.currentFrame=0;
					}
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
		vita2d_wait_rendering_done();
		

		EndFrameStuff();
}

void BattleLop(){
	// 0 - Ready for next person in line
	// 1 - Choosing attack
	// 2 - Choosing target
	// 3 - Fighter moving to target
	// 4 - Fighter moving back from target
	// 5 - Showing spell animation
	char battleStatus=0;
	// Array that holds turn order.
	// To say that somebody moves on a certian turn, use their value.
	// 1-4 is party members
	// 5-8 is enemies
	// input 9 to indicate the end of the cycle.
	// only use 9 if there isn't 4 party and 4 enemy
	char orderOfAction[8]={0};

	char target=0;

	// Them some fresh variables. 
	int i,j,k;
	// The current place in the orderOfAction
	unsigned char currentOrder=0;
	// For all menus in battle.
	signed char selected=0;
	// Move 
	signed short moveXPerFrame=0;
	signed short moveYPerFrame=0;
	// Original position before moving.
	short originalX=0;
	short originalY=0;

	// In battleStatus 4, used to hold damage delt.
	// In battleStatus 5, used for animation stuff
	// Between 
	int temp=0;
	// In battleStatus 4 and 5, used as string to display damage delt
	// At the end of the battle, used to display earned EXP.
	char temp2[10];
	// X speed for magic moving
	// Also temp while var in start
	signed char temp3=0;
	// Y speed for magic moving
	signed char temp4=1;

	// This generates the orderOfAction list
	// i is the the current orderOfAction slot.
	for (i=0;i<8;i++){
		if (i==partySize+numberOfEnemies){
			orderOfAction[i]=9;
			break;
		}
		// j is the current fighter we're testig
		for (j=1;j<=8;j++){
			if (j<5){
				if (j>partySize){
					continue;
				}
			}else{
				if (j>numberOfEnemies+4){
					continue;
				}
			}
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
	vita2d_texture* winTexture = vita2d_load_PNG_file("app0:Stuff/Battle/Win.png");

	spell* selectedSpell=&(GetSpellList(0)->theSpell);

	// Battle moving in intro
	temp3=1;
	while (temp3==1){
		// Entrence positions
		int xPositions[8]={0};
		int yPositions[8]={0};
		signed char addPerFrame[8]={0,0,0,0,0,0,0,0};

		for (i=1;i<9;i++){
			if (j<5){
				if (i>partySize){
					continue;
				}
			}else{
				if (i>numberOfEnemies+4){
					continue;
				}
			}
			if (GetBattlerById(i)->hp==0){
				continue;
			}
			yPositions[i-1]=GetBattlerById(i)->y;
			if (GetBattlerById(i)->x<SCREENWIDTH/2){
				xPositions[i-1]=GetBattlerAnimationById(i,0)->width*-1;
			}else{
				xPositions[i-1]=GetBattlerAnimationById(i,0)->width+SCREENWIDTH;
			}

			if (xPositions[i-1]>SCREENWIDTH/2){
				addPerFrame[i-1]=-7;
			}else{
				addPerFrame[i-1]=7;
			}

		}

		char whileVar=1;

		while(whileVar){
			StartFrameStuff();
			vita2d_start_drawing();
			vita2d_clear_screen();
			DrawMap();
			DrawUnusedAreaRect();

			// Upfsyr all members
			for (i=1;i<9;i++){
				if (i<5){
					if (i>partySize){
						continue;
					}
				}else{
					if (i>numberOfEnemies+4){
						continue;
					}
				}
				if (GetBattlerById(i)->hp==0){
					continue;
				}
				if (GetBattlerById(i)->x==xPositions[i-1]  || (addPerFrame[i-1]<0 && xPositions[i-1]<=GetBattlerById(i)->x)  || (addPerFrame[i-1]>0 && xPositions[i-1]>=GetBattlerById(i)->x)   ){
					DrawAnimationAsISay((GetBattlerAnimationById(i,0)),xPositions[i-1],yPositions[i-1],3);
					continue;
				}
				xPositions[i-1]+=addPerFrame[i-1];

				DrawAnimationAsISay((GetBattlerAnimationById(i,0)),xPositions[i-1],yPositions[i-1],3);
			}
			vita2d_end_drawing();
			vita2d_swap_buffers();

			// Check if all there yet
			for (i=1;i<10;i++){
				if (i==9){
					whileVar=0;
					temp3=0;
				}
				if (i<5){
					if (i>partySize){
						continue;
					}
				}else{
					if (i>numberOfEnemies+4){
						continue;
					}
				}
				if (GetBattlerById(i)->hp==0){
					continue;
				}
				if (!(addPerFrame[i-1]<0 && xPositions[i-1]<=GetBattlerById(i)->x) && !(addPerFrame[i-1]>0 && xPositions[i-1]>=GetBattlerById(i)->x)   ){
					break;
				}
			}

			EndFrameStuff();
		}
	}

	while (1){
		StartFrameStuff();

		// Alright, time to go to next turn
		if (battleStatus==0)/*My body is ready for next turn*/{

			// Check if alive
				char isOneAlive=0;
				for (i=0;i<partySize;i++){
				if (party[i].hp>0){
					isOneAlive=1;
				}
				}
				if (isOneAlive==0){
				// TODO - player has lost.
				ShowMessage(N_DEADMESSAGE,0);
				place=2;
				//place=0;
				break;
				}
				isOneAlive=0;
				for (i=0;i<numberOfEnemies;i++){
					if (enemies[i].hp>0){
						isOneAlive=1;
					}
				}
				if (isOneAlive==0){
				// j k

				int totalEarnExp=0;
				for (i=0;i<numberOfEnemies;i++){
					totalEarnExp+=enemies[i].fighterStats.exp;
					for (k=0;k<10;k++){
						if (enemies[i].fighterStats.spells[k]!=0){
						for (j=0;j<partySize;j++){
								AddSpellToStats(&party[i].fighterStats,enemies[i].fighterStats.spells[k]);
							}
						}
					}
				}
				
				for (j=0;j<partySize;j++){
					party[j].fighterStats.exp+=totalEarnExp;
				}
				
				char didLevelUp[partySize];
				// level*5+10
				for (i=0;i<partySize;i++){
					while (party[i].fighterStats.exp>=party[i].fighterStats.level*5+10){
						party[i].fighterStats.exp-=party[i].fighterStats.level*5+10;
						party[i].fighterStats.level++;
			
						party[i].fighterStats.attack+=3;
						party[i].fighterStats.magicAttack+=3;
						party[i].fighterStats.defence+=3;
						party[i].fighterStats.magicDefence+=3;
						party[i].fighterStats.speed+=3;
						party[i].fighterStats.maxHp+=7;
						party[i].fighterStats.maxMp+=7;
						party[i].hp=party[i].fighterStats.maxHp;
						party[i].mp=party[i].fighterStats.maxMp;
			
						didLevelUp[i]=1;
					}
				}
			
				sprintf((char*)&temp2,"%d",totalEarnExp);
				while (1){
					StartFrameStuff();
			
					if (WasJustPressed(aButton)){
						break;
					}
			
					vita2d_start_drawing();
					vita2d_clear_screen();
				
					vita2d_draw_texture_scale(winTexture,CenterSomething(vita2d_texture_get_width(winTexture)*2),3,2,2);
					//DrawTextAsISay(SCREENWIDTH/2-(strlen(N_WIN)*64/2),3,N_WIN,8)CenterSomething;
					DrawTextAsISay(0,206+64,"EXP:",8);
					DrawTextAsISay(256,206+64,temp2,8);
			
					for (i=0;i<partySize;i++){
						if (didLevelUp[i]==1){
							DrawTextAsISay(0,300+i*32+i*5+64,party[i].fighterStats.name,4);
							DrawTextAsISay(strlen(party[i].fighterStats.name)*32+32,300+i*32+i*5+64,N_LEVELEDUP,4);
						}
					}
			
					vita2d_end_drawing();
					vita2d_swap_buffers();
			
					EndFrameStuff();
				}
				place=0;
				break;
				}

			if (GetBattlerById(orderOfAction[currentOrder])->hp<=0){
				currentOrder++;
			}else{
				//ShowMessage("happy",0);
				if (orderOfAction[(int)currentOrder]<=4){
					originalX = GetBattlerById(orderOfAction[currentOrder])->x;
					originalY = GetBattlerById(orderOfAction[currentOrder])->y;
	
					battleStatus=1;
				}else if (orderOfAction[(int)currentOrder]<=8){
					// generate enmy turn
					char numberOfLivePartyMembers=0;
					for (i=0;i<partySize;i++){
						if (party[i].hp>0){
							numberOfLivePartyMembers++;
						}
					}
					target = RandBetween(1,numberOfLivePartyMembers);

					char numberOfPartyMembersPassed=0;
					// Interpret target var
					for (i=0;i<8;i++){
						if (party[i].hp>0){
							numberOfPartyMembersPassed++;
						}
						if (numberOfPartyMembersPassed==target){
							target=i+1;
							break;
						}
					}

					// 1/5 to use spell
					if (RandBetween(1,4)==1){
						int enemySpellSelected = GetBattlerById(orderOfAction[currentOrder])->fighterStats.spells[RandBetween(0,GetNumberOfSpells(&GetBattlerById(orderOfAction[(int)currentOrder])->fighterStats)-1)];
						selectedSpell = &(GetSpellList(enemySpellSelected)->theSpell);
						ResetAnimation(&(selectedSpell->theAnimation));
						LoadSpellImageIfNeeded(enemySpellSelected);
						
						int tempDone = Damage(GetBattlerById(orderOfAction[currentOrder]),GetBattlerById(target),selectedSpell->attack,selectedSpell->magicAttack,selectedSpell->lastMinuteFix);
						sprintf((char*)&temp2,"%d",tempDone);
						GetBattlerById(orderOfAction[currentOrder])->mp -= selectedSpell->mpCost;
						
						battleStatus=5;
					}else{
						// Using normal attack
						battleStatus=3;
						originalY=GetBattlerById(orderOfAction[currentOrder])->y;
						originalX=GetBattlerById(orderOfAction[currentOrder])->x;
	
						moveXPerFrame=3;
						moveXPerFrame=floor((GetBattlerById(target)->x - GetBattlerById(orderOfAction[currentOrder])->x - GetBattlerAnimationById(orderOfAction[currentOrder],2)->width*3+32 )/30);
						moveYPerFrame=floor((GetBattlerById(target)->y - GetBattlerById(orderOfAction[currentOrder])->y + GetBattlerAnimationById(target,1)->height/4)/30);
		
						if (moveXPerFrame==0){
							moveXPerFrame=ceil((GetBattlerById(target)->x - GetBattlerById(orderOfAction[currentOrder])->x - GetBattlerAnimationById(orderOfAction[currentOrder],2)->width*3+32 )/30);
						}
						if (moveYPerFrame==0){
							moveYPerFrame=ceil((GetBattlerById(target)->y - GetBattlerById(orderOfAction[currentOrder])->y + GetBattlerAnimationById(target,1)->height/4)/30);
						}
					}
				}else{
					// We're on 9 or some bad number.
					// Start new turn rotation.
					currentOrder=0;
				}
			}
		}else if (battleStatus==1)/*Choosing type of move*/{
			if (WasJustPressed(SCE_CTRL_RIGHT)){
				selected++;
				if (selected==3){
					selected=0;
				}
			}
			if (WasJustPressed(SCE_CTRL_LEFT)){
				selected--;
				if (selected<0){
					selected=2;
				}
			}
			if (WasJustPressed(aButton)/* || DidJustTouch()==1*/){
				// TOuch controls
				/*if (DidJustTouch()==1){
					if (currentTouch.report[0].y/2>=SCREENHEIGHT-vita2d_texture_get_height(attackButton)*2){
						if (currentTouch.report[0].x/2<264){
							selected=0;
						}else if (currentTouch.report[0].x/2<496){
							selected=1;
						}else if (currentTouch.report[0].x/2<728){
							selected=2;
						}else if (currentTouch.report[0].x/2<960){
							selected=2;
						}
					}
				}*/
				temp=0;
				if (selected==0){
					// Attack
					battleStatus=2;
					if (orderOfAction[(int)currentOrder]<=4){
						target=5;
					}
				}else if (selected==1){
					// Magic
					int selectedSpellId = SelectSpell(*GetBattlerById(orderOfAction[currentOrder]));
					if (selectedSpellId!=-1){
						target=5;

						selectedSpell = &(GetSpellList(selectedSpellId)->theSpell);

						ResetAnimation(&(selectedSpell->theAnimation));

						LoadSpellImageIfNeeded(selectedSpellId);
						// Set temp to 7 to say we're choosing target for spell and not normal attack
						temp=7;

						battleStatus=2;
					}
				}else if (selected==3){
					// Item
				}else if (selected==2){
					// Run
					place=0;
					break;
				}
			}
		}else if (battleStatus==2)/*Choosing target*/{
			if (WasJustPressed(SCE_CTRL_RIGHT) || WasJustPressed(SCE_CTRL_DOWN)){
				target=target+1;
				if (target>partySize && target<5){
					target=5;
				}
				if (target>numberOfEnemies+4){
					target=1;
				}
				// Skip over dead enemies

				char temp77=target;
				while (1){
					if (temp77==9){
						temp77=1;
					}

					if (GetBattlerById(temp77)->hp>0){
						break;
					}

					temp77++;
				}
				target=temp77;
			}
			if (WasJustPressed(SCE_CTRL_LEFT) || WasJustPressed(SCE_CTRL_UP)){
				target=target-1;
				if (target>partySize && target<5){
					target=partySize;
				}
				if (target<=0){
					target=numberOfEnemies+4;
				}

				// Skip over dead enemies
				for (i=target;i>=0;i--){
					if (i==0){
						i=8;
						continue;
					}
					if (GetBattlerById(i)->hp>0){
						target=i;
						break;
					}
				}
			}
			if (WasJustPressed(aButton)){
				GetBattlerById(orderOfAction[currentOrder])->x=originalX;
				GetBattlerById(orderOfAction[currentOrder])->y=originalY;

				// Chose target for spell
				if (temp==7){
					temp=0;
					ResetAnimation(&(selectedSpell->theAnimation));
					battleStatus=5;
					int tempDone = Damage(GetBattlerById(orderOfAction[currentOrder]),GetBattlerById(target),selectedSpell->attack,selectedSpell->magicAttack,selectedSpell->lastMinuteFix);
					sprintf((char*)&temp2,"%d",tempDone);
					GetBattlerById(orderOfAction[currentOrder])->mp -= selectedSpell->mpCost;
				}else{ // Chose target for normal attack
					battleStatus=3;
	
					//moveXPerFrame = ceil((GetBattlerById(target)->x-GetBattlerById(orderOfAction[currentOrder])->x)/60);
					//moveYPerFrame = ceil((GetBattlerById(target)->y-(GetBattlerById(orderOfAction[currentOrder])->y + (75*3)/2) )/60);
						
					originalY=GetBattlerById(orderOfAction[currentOrder])->y;
					originalX=GetBattlerById(orderOfAction[currentOrder])->x;
	
					moveXPerFrame=3;
					moveXPerFrame=floor((GetBattlerById(target)->x - GetBattlerById(orderOfAction[currentOrder])->x - GetBattlerAnimationById(orderOfAction[currentOrder],2)->width*3+32 )/30);
					moveYPerFrame=floor((GetBattlerById(target)->y - GetBattlerById(orderOfAction[currentOrder])->y + GetBattlerAnimationById(target,1)->height/4)/30);
	
					if (moveXPerFrame==0){
						moveXPerFrame=ceil((GetBattlerById(target)->x - GetBattlerById(orderOfAction[currentOrder])->x - GetBattlerAnimationById(orderOfAction[currentOrder],2)->width*3+32 )/30);
					}
					if (moveYPerFrame==0){
						moveYPerFrame=ceil((GetBattlerById(target)->y - GetBattlerById(orderOfAction[currentOrder])->y + GetBattlerAnimationById(target,1)->height/4)/30);
					}
				}

			}else if (WasJustPressed(bButton)){
				battleStatus=1;
			}
		}else if (battleStatus==3)/*Moving to  target*/{
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
			if (moveXPerFrame==0 && moveYPerFrame==0){
					//ShowMessage("Hurt enemy now.",0);
					
					// TODO - Use weapon attack
					temp = Damage(GetBattlerById(orderOfAction[currentOrder]),GetBattlerById(target),8,0,0);

					moveXPerFrame=floor((originalX - GetBattlerById(orderOfAction[currentOrder])->x)/30);
					moveYPerFrame=floor((originalY - GetBattlerById(orderOfAction[currentOrder])->y)/30);
					
					if (moveXPerFrame==0){
						if (originalX<GetBattlerById(orderOfAction[currentOrder])->x){
							moveXPerFrame=-1;
						}else{
							moveXPerFrame=1;
						}
					}
					if (moveYPerFrame==0){
						if (originalY<GetBattlerById(orderOfAction[currentOrder])->y){
							moveYPerFrame=-1;
						}else{
							moveYPerFrame=1;
						}
					}

					sprintf((char*)&temp2,"%d",temp);

					battleStatus=4;
			}
		}else if (battleStatus==4)/*Moving back from target*/{
			GetBattlerById(orderOfAction[currentOrder])->x=GetBattlerById(orderOfAction[currentOrder])->x+moveXPerFrame;
			GetBattlerById(orderOfAction[currentOrder])->y=GetBattlerById(orderOfAction[currentOrder])->y+moveYPerFrame;
			
			if (moveXPerFrame>=0 && GetBattlerById(orderOfAction[currentOrder])->x>originalX){
				moveXPerFrame=0;
				GetBattlerById(orderOfAction[currentOrder])->x=originalX;
			}
			if (moveXPerFrame<=0 && GetBattlerById(orderOfAction[currentOrder])->x<=originalX){
				moveXPerFrame=0;
				GetBattlerById(orderOfAction[currentOrder])->x=originalX;
			}
			if (moveYPerFrame>=0 && GetBattlerById(orderOfAction[currentOrder])->y>=originalY){
				moveYPerFrame=0;
				GetBattlerById(orderOfAction[currentOrder])->y=originalY;
			}
			if (moveYPerFrame<=0 && GetBattlerById(orderOfAction[currentOrder])->y<=originalY){
				moveYPerFrame=0;
				GetBattlerById(orderOfAction[currentOrder])->y=originalY;
			}

			if (moveXPerFrame==0 && moveYPerFrame==0){
				// We're reggie for next turn.
				battleStatus=0;
				// Go to the next turn.
				currentOrder++;
			}
		}else if (battleStatus==5)/*Waiting for spell animation*/{
			if (selectedSpell->theAnimation.currentFrame==0 && temp!=0){
				currentOrder++;
				battleStatus=0;
			}
		}

		#if NOSLEEP==1
			sceKernelPowerTick(0);
		#endif

		vita2d_start_drawing();
		vita2d_clear_screen();
		DrawMap();
		DrawUnusedAreaRect();

		// Draw selection buttons
		if (battleStatus==1){
			// Draw UI and selector
			vita2d_draw_texture_scale(attackButton,32,SCREENHEIGHT-vita2d_texture_get_height(attackButton)*2,2,2);
			vita2d_draw_texture_scale(magicButton,264,SCREENHEIGHT-vita2d_texture_get_height(magicButton)*2,2,2);
			//vita2d_draw_texture_scale(itemButton,496,SCREENHEIGHT-vita2d_texture_get_height(itemButton)*2,2,2);
			vita2d_draw_texture_scale(runButton,/*728*/496,SCREENHEIGHT-vita2d_texture_get_height(itemButton)*2,2,2);
			UpdateAnimationIfNeed(&selectorAnimation);
			vita2d_draw_texture_part_scale_rotate(selectorAnimation.texture,selected*200+selected*32+132,SCREENHEIGHT-128,selectorAnimation.currentFrame*selectorAnimation.width,0,selectorAnimation.width,selectorAnimation.height,3.7,3.7,1.57);
		}else if (battleStatus==2){
			// Fix cursor if selecting dead person.
			if (GetBattlerById(target)->hp==0){
				for (i=target;i<=10;i++){
					if (i==10){
						i=1;
						continue;
					}
					if (GetBattlerById(i)->hp>0){
						target=i;
						break;
					}
				}
			}
			// Draw the target selector
			DrawAnimationAsISay(&selectorAnimation,GetBattlerById(target)->x-64,GetBattlerById(target)->y + GetBattlerAnimationById(target,1)->height ,3.7);
		}
		// Draw the good stuff.
		for (i=0;i<partySize;i++){

			if (party[i].hp==0){
				continue;
			}

			// Draw health and MP bar for party member
			// hp
			vita2d_draw_rectangle(32+i*128+i*16,16,128,32,RGBA8(0,0,0,255));
			if (party[i].hp>=party[i].fighterStats.maxHp+20){
				// Draw more red rectangle if overheal
				vita2d_draw_rectangle(32+i*128+i*16,16,floor(128*(((double)party[i].hp)/party[i].fighterStats.maxHp)),32,RGBA8(255,0,0,255));
			}else{
				// Draw normal health red
				vita2d_draw_rectangle(32+i*128+i*16,16,floor(128*(((double)party[i].hp)/party[i].fighterStats.maxHp)),32,RGBA8(190,0,0,255));
			}
			// mp
			vita2d_draw_rectangle(32+i*128+i*16,48,128,32,RGBA8(0,0,0,255));
			vita2d_draw_rectangle(32+i*128+i*16,48,floor(128*(((double)party[i].mp)/party[i].fighterStats.maxMp)),32,RGBA8(0,0,190,255));
			// name
			DrawTextAsISay(32+i*128+i*16,90,party[i].fighterStats.name,2);

			// Draws attack animation for person moving to attack
			if (battleStatus==3){
				if (i+1==orderOfAction[currentOrder]){
					DrawAnimationAsISay(&(partyAttackAnimation[i]),party[i].x,party[i].y,3);
					continue;
				}
			}else if (battleStatus==1 || battleStatus==2){
				// Draw magic moving
				if (i+1==orderOfAction[currentOrder]){
					// Updating magic moving
					party[i].x+=temp3;
					party[i].y+=temp4;
					if (party[i].x<=originalX-32){
						temp3=2;
					}else if (party[i].x>=originalX+32){
						temp3=-2;
					}
					if (party[i].y<=originalY-16){
						temp4=1;
					}else if (party[i].y>=originalY+16){
						temp4=-1;
					}
					
					DrawAnimationAsISay(&(partyIdleAnimation[i]),party[i].x,party[i].y,3);
					continue;
				}
				
			}
			DrawAnimationAsISay(&(partyIdleAnimation[i]),party[i].x,party[i].y,3);			
		}
		for (i=0;i<numberOfEnemies;i++){
			if (enemies[i].hp==0){
				continue;
			}

			if (battleStatus==3){
				if (i+5==orderOfAction[currentOrder]){
					DrawAnimationAsISay((enemyAttackAnimation[i]),enemies[i].x,enemies[i].y,3);
					continue;
				}
			}
			DrawAnimationAsISay(enemyIdleAnimation[i],enemies[i].x,enemies[i].y,3);
		}

		if (battleStatus==5){
			temp=selectedSpell->theAnimation.currentFrame;
			DrawAnimationAsISay(&(selectedSpell->theAnimation),GetBattlerById(target)->x,GetBattlerById(target)->y,4);
			if (GetBattlerById(target)->y-70>=0){
				DrawTextAsISay(GetBattlerById(target)->x-floor(strlen(temp2)/2)*64,GetBattlerById(target)->y-70,temp2,8);
			}else{
				DrawTextAsISay(GetBattlerById(target)->x-floor(strlen(temp2)/2)*64,GetBattlerById(target)->y+ GetBattlerAnimationById(target,1)->height+70,temp2,8);
			}
		}else if (battleStatus==4 || battleStatus==5){
			if (GetBattlerById(target)->y-70>=0){
				DrawTextAsISay(GetBattlerById(target)->x-floor(strlen(temp2)/2)*64,GetBattlerById(target)->y-70,temp2,8);
			}else{
				DrawTextAsISay(GetBattlerById(target)->x-floor(strlen(temp2)/2)*64,GetBattlerById(target)->y+ GetBattlerAnimationById(target,1)->height+70,temp2,8);
			}
		}else if (battleStatus==2){
			// hp
			vita2d_draw_rectangle(GetBattlerById(target)->x,GetBattlerById(target)->y-32,128,32,RGBA8(0,0,0,255));
			vita2d_draw_rectangle(GetBattlerById(target)->x,GetBattlerById(target)->y-32,floor(128*(((double)GetBattlerById(target)->hp)/GetBattlerById(target)->fighterStats.maxHp)),32,RGBA8(190,0,0,255));

		}

		vita2d_end_drawing();
		vita2d_swap_buffers();
	
		EndFrameStuff();
	}

	PlzNoCrashOnDispose();
	vita2d_free_texture(attackButton);
	vita2d_free_texture(magicButton);
	vita2d_free_texture(itemButton);
	vita2d_free_texture(runButton);
	vita2d_free_texture(winTexture);

	DisposeAllLoadedSpellImages();

	// Make sure no rapid battle lop
	if (place==3){
		ShowMessage("That shouldn't have happenedededed. Overworld, go!",0);
		place=0;
	}
}

void TitleLoop(){
	vita2d_texture* titleImage = vita2d_load_PNG_file("app0:Stuff/title.png");
	while (1){
		StartFrameStuff();

		if (pad.buttons & aButton){

			if (LANGUAGE == LANG_ENGLISH){
				lua_pushnumber(L,1);
			}else if (LANGUAGE==LANG_SPANISH){
				lua_pushnumber(L,2);
			}
			lua_setglobal(L,"Lang");

			SetupHardcodedLanguage();

			// Need to load here as Lang variable has been set.
			luaL_dofile(L,"app0:Stuff/Happy.lua");
			LoadMap("app0:Stuff/Maps/StartTown");
			place=0;
			break;
		}
		if (WasJustPressed(SCE_CTRL_TRIANGLE)){
			if (LANGUAGE==LANG_ENGLISH){
				LANGUAGE=LANG_SPANISH;
			}else if (LANGUAGE==LANG_SPANISH){
				LANGUAGE=LANG_ENGLISH;
			}
		}
		if (WasJustPressed(SCE_CTRL_SQUARE)){
			if (aButton==SCE_CTRL_CROSS){
				aButton=SCE_CTRL_CIRCLE;
				bButton=SCE_CTRL_CROSS;
			}else{
				bButton=SCE_CTRL_CIRCLE;
				aButton=SCE_CTRL_CROSS;
			}
		}

		vita2d_start_drawing();
		vita2d_clear_screen();

		vita2d_draw_texture(titleImage,0,0);


		if (LANGUAGE==LANG_ENGLISH){
			//DrawTextAsISay(int x, int y, const char* text, int size
			DrawTextAsISay(49+8,495,"Espa'nol",5.875);
		}else if (LANGUAGE==LANG_SPANISH){
			DrawTextAsISay(49+8,495,"English",5.875);
		}

		if (aButton==SCE_CTRL_CROSS){
			if (LANGUAGE==LANG_ENGLISH){
				DrawTextAsISay(51+8,443+15,"X and O: Not Japan",3.5);
			}else if (LANGUAGE==LANG_SPANISH){
				DrawTextAsISay(51+8,443+15,"X y O: NO Jap'on",3.5);
			}
		}else{
			if (LANGUAGE==LANG_ENGLISH){
				DrawTextAsISay(51+8,443+15,"X and O: Japan",3.5);
			}else if (LANGUAGE==LANG_SPANISH){
				DrawTextAsISay(51+8,443+15,"X y O: Jap'on",3.5);
			}
		}

		vita2d_end_drawing();
		vita2d_swap_buffers();
		vita2d_wait_rendering_done();
		
		EndFrameStuff();
	}
	PlzNoCrashOnDispose();
	vita2d_free_texture(titleImage);
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
	char* passedMessage = (char*)lua_tostring(passedState,1);
	char passedIsQuestion = lua_toboolean(passedState,2);
	char messageAnswer = ShowMessage(passedMessage,passedIsQuestion);
	if (passedIsQuestion!=0){
		lua_pushboolean(passedState,messageAnswer);
		return 1;
	}
	return 0;
}

// Shows a message or a question with a portrait
// ARGS - 
// message (string), isQuestion (bool), portrait (vita2d_texture*), portraitScale (double 0 for autodetect to 200)
// Return - 
// if isQuestion==true, returns true if answered yes, false if answered no.
int L_ShowMessageWithPortrait(lua_State* passedState){
	char* passedMessage = (char*)lua_tostring(passedState,1);
	char passedIsQuestion = lua_toboolean(passedState,2);
	vita2d_texture* passedPortrait = lua_touserdata(passedState,3);
	signed char passedScale = lua_tonumber(passedState,4);
	char messageAnswer = ShowMessageWithPortrait(passedMessage,passedIsQuestion,passedPortrait,passedScale);
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
	char* passedPath = (char*)lua_tostring(passedState,1);
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
	tilesets[(int)lua_tonumber(passedState,2)]=(vita2d_texture*)lua_touserdata(passedState,1);
	return 0;
}

// Destroy tileset
// ARGS
// slot
int L_DestroyTileset(lua_State* passedState){
	int passedNumber = lua_tonumber(passedState,1);

	PlzNoCrashOnDispose();
	vita2d_free_texture(tilesets[passedNumber]);
	return 0;
}

// Adds a spell
// ARGS - 
// name, attack, magicAttack, texturePath
int L_AddAttack(lua_State* passedState){

	return 0;
}

// Returns a spell
// ARGS - 
// slot
// RETURN
// spell
int L_GetSpell(lua_State* passedState){
	lua_pushlightuserdata(passedState,&(GetSpellList(lua_tonumber(passedState,1))->theSpell));
	return 1;
}

// Adds to the end of the spell list.
// Returns
// spell
int L_AddSpell(lua_State* passedState){
	spellLinkedList* returnedSpell = AddToSpellList();
	returnedSpell->theSpell.theAnimation.texture=NULL;
	lua_pushlightuserdata(passedState, &(returnedSpell->theSpell));
	return 1;
}

// Sets spell stuff
// Args - 
// spell, name, atk, magicAttack, texturePath, mpCost, lastMinuteFix
// PLEASE DO NOT LOAD THE ANIMATION'S IMAGE. 
// lastMinuteFix is a last minute variable fix thing I made
// special values give special results
// 1 - damage unchanged by defence
int L_SetSpell(lua_State* passedState){
	spell* passedSpell = lua_touserdata(passedState,1);
	passedSpell->name = (char*)lua_touserdata(passedState,2);
	passedSpell->attack = lua_tonumber(passedState,3);
	passedSpell->magicAttack = lua_tonumber(passedState,4);
	passedSpell->spellPath = (char*)lua_touserdata(passedState,5);
	passedSpell->mpCost = lua_tonumber(passedState,6);
	if (lua_gettop(passedState)==7){
		passedSpell->lastMinuteFix = lua_tonumber(passedState,7);
	}
	return 0;
}

// Gets a spell's animation
// Args - 
// spell
int L_GetSpellAnimation(lua_State* passedState){
	spell* passedSpell = lua_touserdata(passedState,1);
	lua_pushlightuserdata(passedState,&(passedSpell->theAnimation));
	return 1;
}

// Sets an animation's values
// ARGS - 
// animation, speed, width, height, numberOfFrames, goBackwards, drawXOffset, drawYOffset, texture(optional)
// Set numberOfFrames to -1 for autodetect
int L_SetAnimation(lua_State* passedState){
	animation* passedAnimation = ((animation*)lua_touserdata(passedState,1));

	passedAnimation->speed = lua_tonumber(passedState,2);
	passedAnimation->width=lua_tonumber(passedState,3);
	passedAnimation->height = lua_tonumber(passedState,4);
	passedAnimation->goBackwards = lua_toboolean(passedState,6);
	passedAnimation->drawXOffset = lua_tonumber(passedState,7);
	passedAnimation->drawYOffset = lua_tonumber(passedState,8);
	passedAnimation->addOnChange=1;

	if (lua_gettop(passedState)==9){
		passedAnimation->texture = lua_touserdata(passedState,9);
	}
	if (lua_tonumber(passedState,5)==-1){
		if (passedAnimation->texture!=NULL){
			passedAnimation->numberOfFrames = (vita2d_texture_get_width(passedAnimation->texture)/passedAnimation->width);
		}else{
			BasicMessage("No will work!");
		}
	}else{
		passedAnimation->numberOfFrames = lua_tonumber(passedState,5);
	}
	return 0;
}

// GEt possible enemy
// ARGS -
// slot
// RETURNS -
// stats* happy
int L_GetPossibleEnemies(lua_State* passedState){
	// Possible enemy they want
	int want = lua_tonumber(passedState,1);
	lua_pushlightuserdata(passedState,&(possibleEnemies[want]));
	return 1;
}

// Set party member data
// ARGS -
// partyMember* passedMember
// stats* passedStats
int L_SetPartyMember(lua_State* passedState){
	partyMember* passedMember = lua_touserdata(passedState,1);
	stats* passedStats = lua_touserdata(passedState,2);
	passedMember->fighterStats = *passedStats;
	return 0;
}


// Set stats data
// ARGS -
// stats* passedStats
// char level;
// short maxHp;
// short maxMp;
// unsigned char attack;
// unsigned char defence;
// unsigned char magicAttack;
// unsigned char magicDefence;
// unsigned char speed;
// short exp;
// char* name
int L_SetStats(lua_State* passedState){
	stats* passedMember = lua_touserdata(passedState,1);
	char level = lua_tonumber(passedState,2);
	short maxHp = lua_tonumber(passedState,3);
	short maxMp = lua_tonumber(passedState,4);
	unsigned char attack = lua_tonumber(passedState,5);
	unsigned char defence = lua_tonumber(passedState,6);
	unsigned char magicAttack = lua_tonumber(passedState,7);
	unsigned char magicDefence = lua_tonumber(passedState,8);
	unsigned char speed = lua_tonumber(passedState,9);
	short exp = lua_tonumber(passedState,10);

	passedMember->level=level;
	passedMember->maxHp=maxHp;
	passedMember->maxMp=maxMp;
	passedMember->attack=attack;
	passedMember->defence=defence;
	passedMember->magicAttack=magicAttack;
	passedMember->magicDefence=magicDefence;
	passedMember->speed=speed;
	passedMember->exp=exp;
	if (lua_gettop(passedState)==11){
		passedMember->name = (char*)lua_touserdata(passedState,11);
	}
	return 0;
}

// Get possible enemy animation
// ARGS
// slot
// animation wanted 1 for idle else for attack
int L_GetPossibleEnemyAnimation(lua_State* passedState){
	int slot = lua_tonumber(passedState,1);
	if (lua_tonumber(passedState,2)==1){
		lua_pushlightuserdata(passedState,&(possibleEnemiesIdleAnimation[slot]));
	}else{
		lua_pushlightuserdata(passedState,&(possibleEnemiesAttackAnimation[slot]));
	}
	return 1;
}

// Changes the map
// ARGS
// mapfilename
int L_ChangeMap(lua_State* passedState){
	char* passedFilename = (char*)lua_tostring(passedState,1);
	ChangeMap(passedFilename);
	return 0;
}

// Adds spell to passed stats
// ARGS
// stats* passedMember
// int passedSpellId
int L_AddSpellToStats(lua_State* passedState){
	stats* passedMember = lua_touserdata(passedState,1);
	int passedSpellId = lua_tonumber(passedState,2);

	AddSpellToStats(passedMember,passedSpellId);
	return 0;
}

/*
Returns the party's size
RETURNS
party's size
*/
int L_GetPartySize(lua_State* passedState){
	lua_pushnumber(passedState,partySize);
	return 1;
}

// Sets the party's size
// ARGS
// newsize
int L_SetPartySize(lua_State* passedState){
	partySize=lua_tonumber(passedState,1);
	return 0;
}

/*
Returns a party member's stats
ARGS
int partyMemberId

// OR, if we're talking about the struct, pass a partyMember*
*/
int L_GetPartyMemberStats(lua_State* passedState){
	if (lua_isnumber(passedState,1)){
		int passedId = lua_tonumber(passedState,1);
		lua_pushlightuserdata(passedState,&(party[passedId].fighterStats));
	}else{
		partyMember* passedMember = lua_touserdata(passedState,1);
		lua_pushlightuserdata(passedState,&(passedMember->fighterStats));
	}	
	return 1;
}

// Starts a wild battle
int L_StartWildBattle(lua_State* passedState){
	InitWildBattle();
	BattleInit();
	place=3;
	BattleLop();
	return 0;
}

// Pass string and will malloc and put that string in that memory and return
// light user data to that string
int L_MallocString(lua_State* passedState){
	const char* passedString = lua_tostring(passedState,1);
	char* returnString = malloc(strlen(passedString)+1);
	strcpy(returnString,passedString);
	lua_pushlightuserdata(passedState,returnString);
	return 1;
}

// Call before disposing images..actually, no.
// you don't really need this.
int L_PlzNoCrashOnDispose(lua_State* passedState){
	PlzNoCrashOnDispose();
	return 0;
}

// GETS A PARTY MEMBER'S ANIMATION
// ARGS -
// slot, whichone
// whichone is 1 for idle and 2 for attack
int L_GetPartyMemberAnimation(lua_State* passedState){
	int whichone = lua_tonumber(passedState,2);
	int passedSlot = lua_tonumber(passedState,1);

	if (whichone==1){
		lua_pushlightuserdata(passedState,&(partyIdleAnimation[passedSlot]));
	}else{
		lua_pushlightuserdata(passedState,&(partyAttackAnimation[passedSlot]));
	}

	return 1;
}

// Quick and ez debug message that doesn't display map in background
int L_DebugMsg(lua_State* passedState){
	BasicMessage((char*)lua_tostring(passedState,1));
	return 0;
}

// Restores party member's health
// ARGS
// slot
int L_RestorePartyMember(lua_State* passedState){
	int passedSlot = lua_tonumber(passedState,1);
	party[passedSlot].hp=party[passedSlot].fighterStats.maxHp;
	party[passedSlot].mp=party[passedSlot].fighterStats.maxMp;
	return 0;
}

// Return map stuff
// ARGS
// x, y
// RETURN
// tileimg, tileset, isSolid, blockevent
int L_GetMap(lua_State* passedState){
	int x = lua_tonumber(passedState,1);
	int y = lua_tonumber(passedState,2);
	int layer = lua_tonumber(passedState,3);
	lua_pushnumber(passedState,GetMapImageData(x,y,layer)->tile);
	lua_pushnumber(passedState,GetMapImageData(x,y,layer)->tileset);
	lua_pushboolean(passedState,GetMapSpotData(x,y)->isSolid);
	lua_pushnumber(passedState,GetMapSpotData(x,y)->event);
	return 4;
}

// Set map stuff
// ARGS
// isSolid, blockEvent
int L_SetMapOtherData(lua_State* passedState){
	int x = lua_tonumber(passedState,1);
	int y = lua_tonumber(passedState,2);
	GetMapSpotData(x,y)->isSolid=lua_toboolean(passedState,3);
	GetMapSpotData(x,y)->event=lua_tonumber(passedState,4);
	return 0;
}

// Sets the player's position
// ARGS
// x, y
// position is tiles
int L_SetPlayerPosition(lua_State* passedState){
	int x = lua_tonumber(passedState,1);
	int y = lua_tonumber(passedState,2);
	playerObject->x=x*32;
	playerObject->y=y*32;
	return 0;
}

// Wait a number of miliseconds
// ARGS
// miliseconds
int L_Wait(lua_State* passedState){
	int miliseconds = lua_tonumber(passedState,1);
	Wait(miliseconds);
	return 0;
}

// Redraws the map.
// Honestly, this was just for torches part
int L_RedrawMap(lua_State* passedState){
	// Drawing
	vita2d_start_drawing();
	vita2d_clear_screen();

	DrawMapThings();
	//DrawText(60,60,"Hello world",2);

	vita2d_end_drawing();
	vita2d_swap_buffers();
	vita2d_wait_rendering_done();
	return 0;
}

// Set's teh encounter rate
// ARGS
// theAmount
int L_SetEncounterRate(lua_State* passedState){
	encounterRate = lua_tonumber(passedState,1);
	nextEncounter = encounterRate+RandBetween(0,5);
	return 0;
}


// Allocate memory.
// ARGS
// memorySize
// ALT ARG
// true, presetId
// Presets will use certian sizes for you.
// presetId list
// 0 - animation
// 1 - stats
// 2 - partyMember
// RETURN
// void*
// This actually uses calloc and not malloc.
int L_Malloc(lua_State* passedState){
	if (lua_gettop(passedState)==2){
		if (lua_tonumber(passedState,2)==0){
			lua_pushlightuserdata(passedState,calloc(1,sizeof(animation)));
		}else if (lua_tonumber(passedState,2)==1){
			lua_pushlightuserdata(passedState,calloc(1,sizeof(stats)));
		}else if (lua_tonumber(passedState,2)==2){
			lua_pushlightuserdata(passedState,calloc(1,sizeof(partyMember)));
		}
	}else if (lua_gettop(passedState)==1){
		lua_pushlightuserdata(passedState,calloc(1,lua_tonumber(passedState,1)));
	}else{
		BasicMessage("Broken L_Malloc call, wrong args");
	}
	return 1;
}


// Free malloc memory
// ARGS
// void*
int L_Free(lua_State* passedState){
	free(lua_touserdata(passedState,1));
	return 0;
}

// Frees an animation's image
int L_FreeAnimationImage(lua_State* passedState){
	animation* passedAnimation = lua_touserdata(passedState,1);
	if (passedAnimation->texture!=NULL){
		vita2d_free_texture(passedAnimation->texture);
		passedAnimation->texture=NULL;
	}
	return 0;
}

// Starts a battle with the enemies you want
// ARGS
// numberOfEnemies, enemyPartyMember 0, enemyIdleAnimation 0, enemyAttackAnimation 0, enemyStats 1, enemyIdleAnimation 1, enemyAttackAnimation 1, etc...
// Max is 4 enemies
// You may ommit args for enemies unused.
int L_StartSpecificBattle(lua_State* passedState){ 
	int i=0;
	// Current arg we're using. Start at 2 for first stats
	int argI=2;
	for (i=0;i<lua_tonumber(passedState,1);i++){
		enemies[i]=*((partyMember*)lua_touserdata(passedState,argI));
		enemies[i].hp=enemies[i].fighterStats.maxHp;
		enemies[i].mp=enemies[i].fighterStats.maxMp;
		argI++;
		enemyIdleAnimation[i]=(lua_touserdata(passedState,argI));
		argI++;
		enemyAttackAnimation[i]=(lua_touserdata(passedState,argI));
		argI++;
	}

	numberOfEnemies=lua_tonumber(passedState,1);

	BattleInit();
	place=3;
	BattleLop();
	return 0;
}

// Sets spells
// ARGS
// stats*, spell 1, spell 2, etc
int L_SetStatsSpells(lua_State* passedState){
	stats* passedStats = lua_touserdata(passedState,1);
	int i=0;
	for (i=2;i<=lua_gettop(passedState);i++){
		passedStats->spells[i-2]=lua_tonumber(passedState,i);
	}
	return 0;
}

// Makes Lua usefull.
void PushCFunctions(){
	lua_pushcfunction(L,L_SetMapImageData);
	lua_setglobal(L,"SetMapImageData");
	//
	lua_pushcfunction(L,L_SetInt);
	lua_setglobal(L,"SetInt");
	//
	lua_pushcfunction(L,L_ShowMessage);
	lua_setglobal(L,"ShowMessage");
	//
	lua_pushcfunction(L,L_LoadPNG);
	lua_setglobal(L,"LoadPNG");
	//
	lua_pushcfunction(L,L_UnloadTexture);
	lua_setglobal(L,"UnloadTexture");
	//
	lua_pushcfunction(L,L_SetTileset);
	lua_setglobal(L,"SetTileset");
	//
	lua_pushcfunction(L,L_GetSpell);
	lua_setglobal(L,"GetSpell");
	//
	lua_pushcfunction(L,L_AddSpell);
	lua_setglobal(L,"AddSpell");
	//
	lua_pushcfunction(L,L_SetSpell);
	lua_setglobal(L,"SetSpell");
	//
	lua_pushcfunction(L,L_SetAnimation);
	lua_setglobal(L,"SetAnimation");
	//
	lua_pushcfunction(L,L_GetSpellAnimation);
	lua_setglobal(L,"GetSpellAnimation");
	//
	lua_pushcfunction(L,L_GetPossibleEnemies);
	lua_setglobal(L,"GetMapEnemy");
	//
	lua_pushcfunction(L,L_GetPossibleEnemyAnimation);
	lua_setglobal(L,"GetMapEnemyAnimation");
	//
	lua_pushcfunction(L,L_SetPartyMember);
	lua_setglobal(L,"SetPartyMember");
	//
	lua_pushcfunction(L,L_SetStats);
	lua_setglobal(L,"SetStats");
	//
	lua_pushcfunction(L,L_ShowMessageWithPortrait);
	lua_setglobal(L,"ShowMessageWithPortrait");
	//
	lua_pushcfunction(L,L_ChangeMap);
	lua_setglobal(L,"ChangeMap");
	//
	lua_pushcfunction(L,L_AddSpellToStats);
	lua_setglobal(L,"AddSpellToStats");
	//
	lua_pushcfunction(L,L_GetPartyMemberStats);
	lua_setglobal(L,"GetPartyMemberStats");
	//
	lua_pushcfunction(L,L_GetPartySize);
	lua_setglobal(L,"GetPartySize");
	//
	lua_pushcfunction(L,L_StartWildBattle);
	lua_setglobal(L,"StartWildBattle");
	//
	lua_pushcfunction(L,L_DestroyTileset);
	lua_setglobal(L,"FreeTileset");
	//
	lua_pushcfunction(L,L_MallocString);
	lua_setglobal(L,"MallocString");
	//
	lua_pushcfunction(L,L_PlzNoCrashOnDispose);
	lua_setglobal(L,"PlzNoCrashOnDispose");
	//
	lua_pushcfunction(L,L_SetPartySize);
	lua_setglobal(L,"SetPartySize");
	//
	lua_pushcfunction(L,L_GetPartyMemberAnimation);
	lua_setglobal(L,"GetPartyMemberAnimation");
	//
	lua_pushcfunction(L,L_DebugMsg);
	lua_setglobal(L,"DebugMsg");
	//
	lua_pushcfunction(L,L_RestorePartyMember);
	lua_setglobal(L,"RestorePartyMember");
	//
	lua_pushcfunction(L,L_GetMap);
	lua_setglobal(L,"GetMap");
	//
	lua_pushcfunction(L,L_SetMapOtherData);
	lua_setglobal(L,"SetMapOtherData");
	//
	lua_pushcfunction(L,L_SetPlayerPosition);
	lua_setglobal(L,"SetPlayerPosition");
	//
	lua_pushcfunction(L,L_Wait);
	lua_setglobal(L,"Wait");
	//
	lua_pushcfunction(L,L_RedrawMap);
	lua_setglobal(L,"RedrawMap");
	//
	lua_pushcfunction(L,L_SetEncounterRate);
	lua_setglobal(L,"SetEncounterRate");
	//
	lua_pushcfunction(L,L_Malloc);
	lua_setglobal(L,"Malloc");
	//
	lua_pushcfunction(L,L_Free);
	lua_setglobal(L,"Free");
	//
	lua_pushcfunction(L,L_FreeAnimationImage);
	lua_setglobal(L,"FreeAnimationImage");
	//
	lua_pushcfunction(L,L_StartSpecificBattle);
	lua_setglobal(L,"StartSpecificBattle");
	//
	lua_pushcfunction(L,L_SetStatsSpells);
	lua_setglobal(L,"SetStatsSpells");
}


/*
///////////////////////////////////////
// LUA FUNCTIONS END
///////////////////////////////////////
*/


void Init(){
	// Good stuff
	spellLinkedListStart=malloc(sizeof(spellLinkedList));

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

	#if SHOWSPLASH==1
		vita2d_texture* happy = vita2d_load_PNG_file("app0:OtherStuff/Splash.png");
	
		vita2d_start_drawing();
		vita2d_clear_screen();
	
		vita2d_draw_texture(happy,0,0);
	
		vita2d_end_drawing();
		vita2d_swap_buffers();
		sceCtrlPeekBufferPositive(0, &pad, 1);
		if (!((pad.buttons & SCE_CTRL_LTRIGGER) && (pad.buttons & SCE_CTRL_UP))){
			Wait(500);
		}
	#endif

	
	if (pad.buttons & SCE_CTRL_RTRIGGER){
		LANGUAGE=LANG_SPANISH;
	}

	// Init Lua
	L = luaL_newstate();
	luaL_openlibs(L);
	PushCFunctions();


	// Set all enemy pointers to null
	for (i=0;i<4;i++){
		enemies[i].hp=-1;
		enemyIdleAnimation[i]=NULL;
		enemyAttackAnimation[i]=NULL;
	}
	for (i=0;i<10;i++){
		// -1 hp means that the slot isn't used.
		possibleEnemies[i].maxHp=-1;
	}


	// Load player frames
	playerDown=vita2d_load_PNG_file("app0:Stuff/PlayerDown.png");
	playerUp=vita2d_load_PNG_file("app0:Stuff/PlayerUp.png");
	playerLeft=vita2d_load_PNG_file("app0:Stuff/PlayerLeft.png");
	playerRight=vita2d_load_PNG_file("app0:Stuff/PlayerRight.png");
	
	// Load tilde
	tilde_a = vita2d_load_PNG_file("app0:Stuff/Tilde/a.png");
	tilde_e = vita2d_load_PNG_file("app0:Stuff/Tilde/e.png");
	tilde_i = vita2d_load_PNG_file("app0:Stuff/Tilde/i.png");
	tilde_o = vita2d_load_PNG_file("app0:Stuff/Tilde/o.png");
	tilde_u = vita2d_load_PNG_file("app0:Stuff/Tilde/u.png");
	tilde_n = vita2d_load_PNG_file("app0:Stuff/Tilde/n.png");
	tilde_esclimationPoint = vita2d_load_PNG_file("app0:Stuff/Tilde/EsclimationPoint.png");
	tilde_questionMark = vita2d_load_PNG_file("app0:Stuff/Tilde/QuestionMark.png");

	// Selector animation for good looking menus
	selectorAnimation.texture=vita2d_load_PNG_file("app0:Stuff/Selector.png");
	selectorAnimation.numberOfFrames=8;
	selectorAnimation.width=22;
	selectorAnimation.height=17;
	selectorAnimation.goBackwards=1;
	selectorAnimation.speed=15;
	selectorAnimation.addOnChange=1;

	// Make player object
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
	// Sets map scale and stuff.
	SetCameraValues();
	// Load what, I guess, could be called a font. It's just an image.
	fontImage=vita2d_load_PNG_file("app0:Stuff/Font.png");
	// Becuase I'm lazy and take the E-z way out
	dummyMember.fighterStats.name="Dummy";
	dummyMember.fighterStats.maxHp=1;
	dummyMember.fighterStats.attack=01;
	dummyMember.fighterStats.defence=01;
	dummyMember.fighterStats.magicDefence=01;
	dummyMember.fighterStats.magicAttack=01;
	dummyMember.fighterStats.speed=01;
	dummyMember.hp=01;
	dummyMember.mp=01;

	// Get touch
	sceTouchPeek(SCE_TOUCH_PORT_FRONT, &currentTouch, 1);

	// Magic line to fix touch. It's really magic, I promise.
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);
	
	place=5;

	PlzNoCrashOnDispose();
	#if SHOWSPLASH==1
		vita2d_free_texture(happy);
	#endif
}

int main(){
	srand(time(NULL));
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
	
	//BattleInit();

	while (1){
		if (place==0){
			Overworld();
		}else if (place==1){
			MenuLop();
		}else if (place==2){
			break;
		}else if (place==3){
			BattleLop();
		}else if (place==4){
			StatusLoop();
		}else if (place==5){
			TitleLoop();
		}
	}
	

	// End this stuff
	vita2d_fini();
	vita2d_free_pgf(defaultPgf);
	// I really don't get why I need this when the user can just use the PS button.
	sceKernelExitProcess(0);
	return 0;
}
