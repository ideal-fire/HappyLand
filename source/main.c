/*
---O---OO--OOOOO-O-OO-OOOOO----O---O
--O-O-O-O--O---O-OO---O---O---O-O-O-
-O--OO--O--O---O-O----O---O--O--OO--
O---O---O--OOOOO-O----OOOOO-O---O---
*/

// Only turned off when I'm working on my game. Needs to be set to 1 before release
#define SHOWSPLASH 1

#define LUAREGISTER(x,y) lua_pushcfunction(L,x);\
	lua_setglobal(L,y);

//#define LANGUAGE LANG_SPANISH
#define LANG_ENGLISH 1
#define LANG_SPANISH 2
int LANGUAGE=LANG_ENGLISH;

#define PLAT_VITA 1
#define PLAT_WINDOWS 2
#define PLAT_3DS 3

#define PLATFORM PLAT_WINDOWS

#define SUB_ANDROID 1
#define SUBPLATFORM SUB_NONE

#define SND_SDL 1
#define SOUNDPLAYER SND_SDL

#define REND_UNDEFINED 0
#define REND_SDL 1
#define REND_VITA2D 2
#define REND_SF2D 3

#define RENDERER REND_SDL

#define TEXT_UNDEFINED 0
#define TEXT_DEBUG 1
#define TEXT_VITA2D 2
#define TEXT_FONTCACHE 3

#define TEXTRENDERER TEXT_FONTCACHE

#define TYPE_UNDEFINED 0
#define TYPE_DATA 1
#define TYPE_EMBEDDED 2

// Change the place variable to one of these
#define PLACE_OVERWORLD 0
#define PLACE_MENU 1
#define PLACE_QUIT 2
#define PLACE_BATTLE 3
#define PLACE_STATUS 4
#define PLACE_TITLE 5

#if PLATFORM == PLAT_VITA
	#define STARTINGMAP "app0:Stuff/Maps/NathansHouse"
#elif PLATFORM == PLAT_WINDOWS
	#define STARTINGMAP "./Stuff/Maps/TestLand"
#elif PLATFORM == PLAT_3DS
	#define STARTINGMAP "/3ds/data/HappyLand/Stuff/Maps/NathansHouse"
#endif

// Translatos for hardcoded strings
#include "HardcodedLanguage.h"

#define MAXOBJECTS 15
#if PLATFORM==PLAT_VITA
	#define SCREENWIDTH 960
	#define SCREENHEIGHT 544
	#define MAPXSCALE 2
	#define MAPYSCALE 2
	#define BATTLEENTITYSCALE 3
	#define SPELLSCALE 4
	// Minimum y position for an entity to be at during a battle.
	// This is so they don't overlap the healthbar
	#define MINYBATTLE 106
	// This is so they don't overlap the selection buttons. Put as the screen height for no restriction
	#define MAXYBATTLE 416

	// CROSS TYPES
	#define CrossTexture vita2d_texture
	
	// Textbox's y position
	#define TEXTBOXY 420
	
	// Size of letters in textbox
	#define TEXTBOXFONTSIZE 2.5

	// Lines on one screen of textbox
	#define LINESPERSCREEN 4
	// w and height of portertit
	#define DEFAULTPORTRAITSIZE 200
	// You know how in the battle when you hit somebody it shows the damage
	// well this is the scale of the text
	#define DAMAGETEXTSIZE 8
#elif PLATFORM==PLAT_WINDOWS
	#define SCREENWIDTH 960
	#define SCREENHEIGHT 544
	#define MAPXSCALE 2
	#define MAPYSCALE 2
	#define BATTLEENTITYSCALE 3
	#define SPELLSCALE 4
	#define MINYBATTLE 106
	#define MAXYBATTLE 416

	#define CrossTexture SDL_Texture
	#define TEXTBOXY 420
	#define TEXTBOXFONTSIZE 2.5

	// Lines on one screen of textbox
	#define LINESPERSCREEN 4

	#define DEFAULTPORTRAITSIZE 200

	#define DAMAGETEXTSIZE 8
#elif PLATFORM==PLAT_3DS
	#define SCREENWIDTH 400
	#define SCREENHEIGHT 240
	#define BOTTOMSCREENWIDTH 320
	#define BOTTOMSCREENHEIGHT 240
	#define MAPXSCALE 1
	#define MAPYSCALE 1
	
	#define BATTLEENTITYSCALE 1
	#define SPELLSCALE 2
	// No need to restrict this as stuff is on bottom screen
	#define MINYBATTLE 0
	#define MAXYBATTLE SCREENHEIGHT

	#define SCE_CTRL_CROSS KEY_A
	#define SCE_CTRL_CIRCLE KEY_B
	#define SCE_CTRL_UP KEY_UP
	#define SCE_CTRL_DOWN KEY_DOWN
	#define SCE_CTRL_LEFT KEY_LEFT
	#define SCE_CTRL_RIGHT KEY_RIGHT
	#define SCE_CTRL_TRIANGLE KEY_X
	#define SCE_CTRL_SQUARE KEY_Y
	#define SCE_CTRL_START KEY_START
	
	#define CrossTexture sf2d_texture
	
	#define TEXTBOXFONTSIZE 1.5
	// SCREENHEIGHT - (Font*8)*desiredlines-5-5*desiredlines
	#define TEXTBOXY 172
	
	#define LINESPERSCREEN 3

	#define DEFAULTPORTRAITSIZE 100

	#define DAMAGETEXTSIZE 2
#endif

// Not needed. Can use registry if you don't want to sleep.
#define NOSLEEP 0


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

#if PLATFORM == PLAT_VITA
	#include <psp2/ctrl.h>
	#include <psp2/kernel/processmgr.h>
	#include <psp2/rtc.h>
	#include <psp2/types.h>
	#include <psp2/touch.h>
	#include <psp2/display.h>
	
	#include <vita2d.h>

	typedef uint8_t 	u8;
	typedef uint16_t 	u16;
	typedef uint32_t	u32;
	typedef uint64_t	u64;
	typedef int8_t		s8;
	typedef int16_t		s16;
	typedef int32_t		s32;
	typedef int64_t		s64;
#elif PLATFORM == PLAT_WINDOWS
	#include <time.h>

	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>

	#include <dirent.h>
#elif PLATFORM == PLAT_3DS
	#include <3ds.h>
	#include <stdio.h>
	#include <sf2d.h>
	#include <sfil.h>
	#include <3ds/svc.h>
#endif

// Include stuff I made.
#include "GoodArray.h"


//vita2d_pgf* defaultPgf;

#if PLATFORM == PLAT_VITA
	// Controls at start of frame.
	SceCtrlData pad;
	// Controls from start of last frame.
	SceCtrlData lastPad;

	SceTouchData currentTouch;
	SceTouchData previousTouchData;
#elif PLATFORM == PLAT_WINDOWS
	//The window we'll be rendering to
	SDL_Window* mainWindow;
	
	//The window renderer
	SDL_Renderer* mainWindowRenderer;

#elif PLATFORM==PLAT_3DS
	u32 pad;
	u32 wasJustPad;
#endif

#define unusedAreaColor 0,0,0,255
//unsigned int unusedAreaColor = RGBA8(0,0,0,255);

char tempPathFixBuffer[256];

void FixPath(char* filename,unsigned char _buffer[], char type);
char ShowMessageWithPortrait(char* _tempMsg, char isQuestion, CrossTexture* portrait, double portScale);

#include "GeneralGoodExtended.h"
#include "FpsCapper.h"
//TODO - Make this
// main.h
	// Make stuff fresh
	//typedef int8_t		s8;
	//typedef int16_t		s16;
	//typedef int32_t		s32;
	//typedef int64_t		s64;
	//typedef uint8_t 	u8;
	//typedef uint16_t 	u16;
	//typedef uint32_t	u32;
	//typedef uint64_t	u64;
	void BasicMessage(char* message);

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
		CrossTexture* texture; // Image.
		short width; // Width of a single frame
		short height; // Height of a single frame
		char currentFrame; // Current frame I'm on.
		char numberOfFrames; // Total number of frames.
		char goBackwards; // Go backwards at the end?
		signed char addOnChange; // What is added on change.
		signed short drawXOffset; // An x offset.
		signed short drawYOffset; // A y offset.
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

/*
=================================================
== STUFF
=================================================
*/
lua_State* L;
//CrossTexture* fontImage;
animation selectorAnimation;
// Where you are. Like Overworld, Menu, Battle, etc.
// Values in main function
unsigned char place=0;

spellLinkedList* spellLinkedListStart={0};
unsigned short spellLinkedListSize=1;

char* currentMapFilepath;

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

/*
==================================================
== WORLD MAP
==================================================
*/
CrossTexture* tilesets[5];
object mapObjects[MAXOBJECTS];
Good2dArray layers[5];
Good2dArray tileOtherData;
char numberOfLayers=0;
CrossTexture* playerDown;
CrossTexture* playerUp;
CrossTexture* playerLeft;
CrossTexture* playerRight;
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

/*
//////////////////////////////////////
// CROSS PLATFORM SUPPORT
///////////////////////////////////////
*/

void StartDrawing(){
	StartDrawingA();
}

void EndDrawing(){
	EndDrawingA();
}


/*
////////////////////////////////////////////////
// 
///////////////////////////////////////////////
*/

void FixPath(char* filename,unsigned char _buffer[], char type){
	#if SUBPLATFORM == SUB_ANDROID
		if (type==TYPE_DATA){
			strcpy((char*)_buffer,"/sdcard/HIGURASHI/");
		}else if (type==TYPE_EMBEDDED){
			strcpy((char*)_buffer,"a/");
		}
		strcat((char*)_buffer,filename);
	#elif PLATFORM == PLAT_WINDOWS
		if (type==TYPE_DATA){
			strcpy((char*)_buffer,"./");
		}else if (type==TYPE_EMBEDDED){
			strcpy((char*)_buffer,"./");
		}
		strcat((char*)_buffer,filename);
	#elif PLATFORM == PLAT_VITA
		if (type==TYPE_DATA){
			strcpy((char*)_buffer,"ux0:data/HIGURASHI/");
		}else if (type==TYPE_EMBEDDED){
			strcpy((char*)_buffer,"app0:a/");
		}
		strcat((char*)_buffer,filename);
	#endif
}

void RestorePartyMember(int id){
	int passedSlot = id;
	party[passedSlot].hp=party[passedSlot].fighterStats.maxHp;
	party[passedSlot].mp=party[passedSlot].fighterStats.maxMp;
}

// pl0x, I beg you, don't crash when I'm disposing textures.
void PlzNoCrashOnDispose(){
	#if PLATFORM == PLAT_VITA
		vita2d_wait_rendering_done();
		sceDisplayWaitVblankStart();
	#endif
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
					DrawTexturePartScale(tilesets[(int)GetMapImageData(x+cameraWholeOffsetX,y+cameraWholeOffsetY,i)->tileset],FixX(x*32-cameraPartialOffsetX),FixY(y*32-cameraPartialOffsetY),((GetMapImageData(x+cameraWholeOffsetX,y+cameraWholeOffsetY,i)->tile-1)*32),0,32,32,MAPXSCALE,MAPYSCALE);
				}
			}
		}
	}
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
	#if PLATFORM == PLAT_VITA
		lastPad=pad;
		sceCtrlPeekBufferPositive(0, &pad, 1);
		sceTouchPeek(SCE_TOUCH_PORT_FRONT, &currentTouch, 1);
	#elif PLATFORM == PLAT_WINDOWS
		// Update keyboard stuff
		SDL_Event e;
		while( SDL_PollEvent( &e ) != 0 ){
			if( e.type == SDL_QUIT ){
				place=2;
			}

			if( e.type == SDL_KEYDOWN ){
				if (e.key.keysym.sym==SDLK_z){ /* X */
					pad[SCE_CTRL_CROSS]=1;
				}else if (e.key.keysym.sym==SDLK_x){/* O */
					pad[SCE_CTRL_CIRCLE]=1;
				}else if (e.key.keysym.sym==SDLK_LEFT){/* Left */
					pad[SCE_CTRL_LEFT]=1;
				}else if (e.key.keysym.sym==SDLK_RIGHT){ /* Right */
					pad[SCE_CTRL_RIGHT]=1;
				}else if (e.key.keysym.sym==SDLK_DOWN){ /* Down */
					pad[SCE_CTRL_DOWN]=1;
				}else if (e.key.keysym.sym==SDLK_UP){ /* Up */
					pad[SCE_CTRL_UP]=1;
				}else if (e.key.keysym.sym==SDLK_a){ /* Square */
					pad[SCE_CTRL_SQUARE]=1;
				}else if (e.key.keysym.sym==SDLK_s){ /* Triangle */
					pad[SCE_CTRL_TRIANGLE]=1;
				}else if (e.key.keysym.sym==SDLK_ESCAPE){ /* Start */
					pad[SCE_CTRL_START]=1;
				}
			}else if (e.type == SDL_KEYUP){
				if (e.key.keysym.sym==SDLK_z){ /* X */
					pad[SCE_CTRL_CROSS]=0;
				}else if (e.key.keysym.sym==SDLK_x){/* O */
					pad[SCE_CTRL_CIRCLE]=0;
				}else if (e.key.keysym.sym==SDLK_LEFT){/* Left */
					pad[SCE_CTRL_LEFT]=0;
				}else if (e.key.keysym.sym==SDLK_RIGHT){ /* Right */
					pad[SCE_CTRL_RIGHT]=0;
				}else if (e.key.keysym.sym==SDLK_DOWN){ /* Down */
					pad[SCE_CTRL_DOWN]=0;
				}else if (e.key.keysym.sym==SDLK_UP){ /* Up */
					pad[SCE_CTRL_UP]=0;
				}else if (e.key.keysym.sym==SDLK_a){ /* Square */
					pad[SCE_CTRL_SQUARE]=0;
				}else if (e.key.keysym.sym==SDLK_s){ /* Triangle */
					pad[SCE_CTRL_TRIANGLE]=0;
				}else if (e.key.keysym.sym==SDLK_ESCAPE){ /* Start */
					pad[SCE_CTRL_START]=0;
				}
			}
		}
	#elif PLATFORM==PLAT_3DS
		hidScanInput();
		wasJustPad = hidKeysDown();
		pad = hidKeysHeld();
	#endif
}

void EndFrameStuff(){
	#if PLATFORM == PLAT_VITA
		previousTouchData=currentTouch;
	#elif PLATFORM == PLAT_WINDOWS
		memcpy(lastPad,pad,19);
	#endif
	FpsCapWait();
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
	DrawTexturePartScale(animationToDraw->texture,FixXObjects(destX+animationToDraw->drawXOffset),FixYObjects(destY+animationToDraw->drawYOffset),animationToDraw->width*animationToDraw->currentFrame,0,animationToDraw->width,animationToDraw->height,MAPXSCALE,MAPYSCALE);
}
// Draw animation. Unchanged by map position
void DrawAnimation(animation* animationToDraw, int destX, int destY){
	UpdateAnimationIfNeed(animationToDraw);
	DrawTexturePartScale(animationToDraw->texture,FixX(destX+animationToDraw->drawXOffset),FixY(destY+animationToDraw->drawYOffset),animationToDraw->width*animationToDraw->currentFrame,0,animationToDraw->width,animationToDraw->height,MAPXSCALE,MAPYSCALE);
}
// Draws it as given in arguments. No scaling or position changes.
void DrawAnimationAsISay(animation* animationToDraw, int destX, int destY, float scale){
	UpdateAnimationIfNeed(animationToDraw);
	DrawTexturePartScale(animationToDraw->texture,(destX+animationToDraw->drawXOffset),(destY+animationToDraw->drawYOffset),animationToDraw->width*animationToDraw->currentFrame,0,animationToDraw->width,animationToDraw->height,scale,scale);
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
	DrawRectangle(0,0,SCREENWIDTH,drawYOffset,unusedAreaColor);
	// Bottom
	DrawRectangle(0,SCREENHEIGHT-drawYOffset,SCREENWIDTH,drawYOffset,unusedAreaColor);
	// Left
	DrawRectangle(0,0,drawXOffset,SCREENHEIGHT,unusedAreaColor);
	// Right
	DrawRectangle(SCREENWIDTH-drawXOffset,0,drawXOffset,SCREENHEIGHT,unusedAreaColor);
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
	if (lua_getglobal(L,eventString)==0){
		printf("Failed to get event with function name %s\n",eventString);
		char _tempBuf[strlen("Failed to get event with function name ")+8];
		strcpy(_tempBuf,"Failed to get event with function name ");
		strcat(_tempBuf,eventString);
		BasicMessage(_tempBuf);
		return 0;
	}
	// Pass object
	lua_pushlightuserdata(L,theThingie);
	// Call funciton. Removes function from stack.
	lua_call(L, 1, 1);

	char returnedValue=lua_tonumber(L,-1);
	lua_pop(L,1);
	return returnedValue;
}

// Draws everything we draw on map.
void DrawMapThings(){
	DrawMap();
	DrawMapObjects();
	DrawUnusedAreaRect();
}

void BasicMessage(char* _tempMsg){
	ControlsReset();
	// The string needs to be copied. We're going to modify it, at we can't if we just type the string into the function and let the compiler do everything else
	char message[strlen(_tempMsg)+1];
	strcpy(message,_tempMsg);
	int textboxLinesPerScreens = (SCREENHEIGHT-TEXTBOXY)/TextHeight(fontSize);
	short newLineLocations[50];
	int totalMessageLength = strlen(message);
	int i, j;

	// This will loop through the entire message, looking for where I need to add new lines. When it finds a spot that
	// needs a new line, that spot in the message will become 0. So, when looking for the place to 
	int lastNewlinePosition=0;
	for (i = 0; i < totalMessageLength; i++){
		if (message[i]==32){ // Only check when we meet a space. 32 is a space in ASCII
			message[i]='\0';
			//printf("Space found at %d\n",i);
			//printf("%s\n",&message[lastNewlinePosition+1]);
			//printf("%d\n",TextWidth(fontSize,&(message[lastNewlinePosition+1])));
			if (TextWidth(fontSize,&(message[lastNewlinePosition+1]))>SCREENWIDTH-20){
				char _didWork=0;
				for (j=i-1;j>lastNewlinePosition+1;j--){
					//printf("J:%d\n",j);
					if (message[j]==32){
						message[j]='\0';
						_didWork=1;
						message[i]=32;
						lastNewlinePosition=j;
						break;
					}
				}
				if (_didWork==0){
					message[i]='\0';
					lastNewlinePosition=i+1;
				}
			}else{
				message[i]=32;
			}
		}
	}
	// This spot in the message will be the one that's 0 char. The player won't be able to see this character.
	int currentLetter=0;
	int currentLine=0;
	signed int nextCharStorage=-1;
	char currentlyVisibleLines=1;
	char frames=0;
	// This variable is the location of the start of the first VISIBLE line
	// This will change if the text box has multiple screens because the text is too long
	int offsetStrlen=0;
	//  textboxNewCharSpeed
	
	nextCharStorage = message[0];
	message[0]='\0';
	while (currentlyVisibleLines<=textboxLinesPerScreens && currentLetter!=totalMessageLength){
		if (nextCharStorage==0){
			currentlyVisibleLines++;
		}
		message[currentLetter]=nextCharStorage;
		currentLetter++;
		if (currentLetter!=totalMessageLength){
			nextCharStorage = message[currentLetter];
			message[currentLetter]='\0';
		}else{
			nextCharStorage=0;
		}
	}
	while (1){
		FpsCapStart();
		ControlsStart();
		if (WasJustPressed(SCE_CTRL_CROSS)){
			if (currentLetter==totalMessageLength){
				break;
			}else{
				offsetStrlen=currentLetter;
				//currentLetter=0;
				//nextCharStorage=-1;
				currentlyVisibleLines=1;
				frames=0;
			}
			while (currentlyVisibleLines<=textboxLinesPerScreens && currentLetter!=totalMessageLength){
				if (nextCharStorage==0){
					currentlyVisibleLines++;
				}
				message[currentLetter]=nextCharStorage;
				currentLetter++;
				if (currentLetter!=totalMessageLength){
					nextCharStorage = message[currentLetter];
					message[currentLetter]='\0';
				}else{
					nextCharStorage=0;
				}
			}
		}
		ControlsEnd();
		
		StartDrawing();

		// We need this variable so we know the offset in the message for the text that is for the next line
		int _lastStrlen=0;
		for (i=0;i<currentlyVisibleLines;i++){
			DrawText(5,TEXTBOXY+TextHeight(fontSize)*i,&message[_lastStrlen+offsetStrlen],fontSize);
			// This offset will have the first letter for the next line
			_lastStrlen = strlen(&message[_lastStrlen+offsetStrlen])+1+_lastStrlen;
		}

		EndDrawing();
		FpsCapWait();
		frames++;
	}
	ControlsEnd();
}

char TestNewMessage(char* message, char isQuestion, CrossTexture* portrait, double portScale){
	EndFrameStuff();
	if (portScale==0 && portrait!=NULL){
		portScale = floor((double)DEFAULTPORTRAITSIZE/GetTextureWidth(portrait));
	}

	// We count frames, and show a new letter every certian amount of frames.
	char frames=0;
	// Letter we're currently displaying.
	short currentLetter=0;

	CrossTexture* yesButtonTexture=(CrossTexture*)1;
	CrossTexture* noButtonTexture=(CrossTexture*)1;

	char quitTextbox=0;

	signed char currentSelected=defaultSelected;

	// 68/(8*2)
	unsigned char linesPerScreen = LINESPERSCREEN;/*floor((SCREENHEIGHT-TEXTBOXY)/(8*TEXTBOXFONTSIZE));*/

	// Letter scale is 2.5. 8*2.5=20.
	unsigned char maxLetters=floor(SCREENWIDTH/(8*TEXTBOXFONTSIZE));

	short newlineSpaces[50];
	char totalNewLines=0;

	//vita2d_wait_rendering_done();

	int i=0;
	int j=0;
	char questionScale=1;
	if (isQuestion==1){
		FixPath("Stuff/Yes.png",tempPathFixBuffer,TYPE_EMBEDDED);
		yesButtonTexture=LoadPNG(tempPathFixBuffer);
		FixPath("Stuff/No.png",tempPathFixBuffer,TYPE_EMBEDDED);
		noButtonTexture=LoadPNG(tempPathFixBuffer);
		

		if (SCREENWIDTH/GetTextureWidth(yesButtonTexture)>=8){
			questionScale=2;
		}else{
			questionScale=1;
		}
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
			if (message[i]==32){ // Checks if it's a space
				// Add the +1 so that the space is left at the end of the previous line.
				newlineSpaces[j-1]=i+1;
				totalNewLines++;
				break;
			}else{
				i=i-1;
			}
		}
	}
	newlineSpaces[(int)totalNewLines]=strlen(message);

	char sectionOfCurrentMessage=linesPerScreen-1;
	if (totalNewLines<linesPerScreen-1){
		sectionOfCurrentMessage=totalNewLines;
	}
	short characterOffset=0;
	char startSectionOfNewLine=0;

	// In this variable, we store the character that was replaced with 0.
	// It should only be 0-255, but it's a signed short so it can start at -1
	signed short _tempCharStorage=-1;

	// This stores the entire length of the message before we change it for display
	int totalMessageLength = strlen(message);

	unsigned char nextNewLineSpace=startSectionOfNewLine;

	while (!quitTextbox){
		StartFrameStuff();
			
		if (WasJustPressed(aButton)){
			if (currentLetter==newlineSpaces[(int)sectionOfCurrentMessage]){
				// ADVANCE TO NEXT MESSAGE
				if (sectionOfCurrentMessage!=totalNewLines){
					startSectionOfNewLine+=linesPerScreen;
					characterOffset=newlineSpaces[(int)sectionOfCurrentMessage];
					currentLetter=newlineSpaces[(int)sectionOfCurrentMessage]+1;
					if (sectionOfCurrentMessage+linesPerScreen<=totalNewLines){
						sectionOfCurrentMessage+=linesPerScreen;
					}else{
						sectionOfCurrentMessage=totalNewLines;
					}
					frames=0;
					nextNewLineSpace=startSectionOfNewLine;
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
			printf("advance\n");
			if (currentLetter<totalMessageLength){

				if (currentLetter == newlineSpaces[nextNewLineSpace]){
					nextNewLineSpace++;
					message[currentLetter]='\0';
					_tempCharStorage=-1;
				}else{
					printf("ok!\n");
					frames=0;
					currentLetter++;
					if (_tempCharStorage!=-1){
						message[currentLetter-1]=_tempCharStorage;
					}
					_tempCharStorage = message[currentLetter];
					message[currentLetter]='\0';
				}
				printf("%s\n",message);
			}
		}

		StartDrawing();

		DrawMapThings();
		// Draw da white rectangle
		DrawRectangle(0,TEXTBOXY,SCREENWIDTH,SCREENHEIGHT-TEXTBOXY,255,255,255,255);

		// Draw message
		short i;
		// Position in newlineSpaces that we last used the newline thing.
		

		//for (i = characterOffset; i < currentLetter; i++){
			//if (i==newlineSpaces[nextNewLineSpace]){
			//	currentXPos=0;
			//	currentYPos++;
			//	nextNewLineSpace++;
			//}else{
			//	currentXPos++;
			//}
			//if (IsThisTilde(message,i)){
			//	DrawTildeLetterUnscaled(message[i+1],currentXPos*(TEXTBOXFONTSIZE*8)+5,TEXTBOXY+currentYPos*(TEXTBOXFONTSIZE*8)+currentYPos*5+5,TEXTBOXFONTSIZE);
			//	i++;
			//	continue;
			//}
			//// currentXPos*20 has a +5 for offset so not at screen edge.
			//DrawLetterUnscaled(message[i],currentXPos*(TEXTBOXFONTSIZE*8)+5,TEXTBOXY+currentYPos*(TEXTBOXFONTSIZE*8)+currentYPos*5+5,TEXTBOXFONTSIZE);
			//DrawText(int x, int y, const char* text, float size)
			int k=0;
			printf("current secction of message :%d\n", sectionOfCurrentMessage);
			for (k=LINESPERSCREEN;k>0;k--){
				DrawText(5,TEXTBOXY+TextHeight(fontSize)*k,&(message[newlineSpaces[sectionOfCurrentMessage-k]]),fontSize);
			}
		//}

		// Draw questions
		if (isQuestion==1){
			DrawTextureScale(yesButtonTexture,SCREENWIDTH-GetTextureWidth(yesButtonTexture)*questionScale,TEXTBOXY-GetTextureHeight(yesButtonTexture)*questionScale-GetTextureHeight(yesButtonTexture)*questionScale,questionScale,questionScale);
			DrawTextureScale(noButtonTexture,SCREENWIDTH-GetTextureWidth(noButtonTexture)*questionScale,TEXTBOXY-GetTextureHeight(noButtonTexture)*questionScale,questionScale,questionScale);
			DrawAnimationAsISay(&selectorAnimation,SCREENWIDTH-GetTextureWidth(yesButtonTexture)*questionScale-questionScale*22-5,TEXTBOXY-(currentSelected)*(GetTextureHeight(yesButtonTexture)*questionScale)-((GetTextureHeight(yesButtonTexture)*questionScale)/2),1);
		}
		
		if (portrait!=NULL){
			DrawTextureScale(portrait,0,TEXTBOXY-GetTextureHeight(portrait)*portScale,portScale,portScale);
		}

		EndDrawing();


		frames++;
		EndFrameStuff();
	}

	if (isQuestion==1){
		PlzNoCrashOnDispose();
		FreeTexture(yesButtonTexture);
		FreeTexture(noButtonTexture);
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


	free(currentMapFilepath);
	currentMapFilepath = malloc(strlen(path)+1);
	strcpy(currentMapFilepath,path);

	//((tileImageData*)GetGoodArray(&layers[0],1,1))->tile=1;
	//
}

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
				FreeTexture(possibleEnemiesAttackAnimation[i].texture);
				possibleEnemiesAttackAnimation[i].texture=NULL;
			}
			if (possibleEnemiesIdleAnimation[i].texture!=NULL){
				FreeTexture(possibleEnemiesIdleAnimation[i].texture);
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
	animationToModify->texture=LoadPNG(filePath);
	animationToModify->width=width;
	/*39*/animationToModify->height=GetTextureHeight(animationToModify->texture);
	animationToModify->numberOfFrames=GetTextureWidth(animationToModify->texture)/animationToModify->width;
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
	if (victim->hp>victim->fighterStats.maxHp+15){
		victim->hp=victim->fighterStats.maxHp+15;
	}
	return doneDamage;
}

// Includes left and right bounds
int RandBetween(int leftBound, int rightBound){
	rightBound++;
	return ((rand()%(rightBound-leftBound))+leftBound);
}

void InitWildBattle(){
	int i=0;

	int firstRandomA = RandBetween(1,4);
	numberOfEnemies=firstRandomA;
	if (firstRandomA>2){
		int firstRandomB = RandBetween(2,4);
		numberOfEnemies=firstRandomB;
	}else if (firstRandomA==1){
		if (RandBetween(1,3)!=1){
			int firstRandomB = RandBetween(2,4);
			numberOfEnemies=firstRandomB;
		}
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
		enemies[i].y=RandBetween(MINYBATTLE, MAXYBATTLE-enemyIdleAnimation[i]->height*BATTLEENTITYSCALE);
		enemies[i].x=RandBetween(SCREENWIDTH/2,SCREENWIDTH-enemyIdleAnimation[i]->width*BATTLEENTITYSCALE);
	}
	for (i=0;i<partySize;i++){
		party[i].y=RandBetween(MINYBATTLE, MAXYBATTLE-partyIdleAnimation[i].height*BATTLEENTITYSCALE);
		party[i].x=RandBetween(0,SCREENWIDTH/2-partyIdleAnimation[i].width*BATTLEENTITYSCALE);
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
		BasicMessage("Aw snap. Problem add to list. Maybe we'll loose some entries.");
	}
	listOn->nextEntry=tempList;
	spellLinkedListSize++;

	return tempList;
}

int CenterSomething(int width){
	return (SCREENWIDTH/2-floor(((double)width)/2));
}

int CenterSomethingCustomWidth(int width, int customScreenWidth){
	return (customScreenWidth/2-floor(((double)width)/2));
}

// Returns x position for this to be centered
int CenterText(char* text, char scale){
	return CenterSomething(strlen(text)*(scale*8));
}

// Returns x position for this to be centered
int CenterTextCustomWidth(char* text, char scale, int customScreenWidth){
	return CenterSomethingCustomWidth(strlen(text)*(scale*8),customScreenWidth);
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

		
		StartDrawing();
		#if PLATFORM != PLAT_3DS

			DrawText( CenterText(N_MAGICLIST,6),2,N_MAGICLIST,6);
	
			for (i=0;i<numberOfSkillsSelect;i++){
				if (member.fighterStats.spells[i]!=0){
					//ShowMessage("a",0);
					spellLinkedList* tempList = GetSpellList(member.fighterStats.spells[i]-1);
					sprintf((char*)tempMessage,"%s:%d",tempList->theSpell.name,tempList->theSpell.mpCost);
					DrawText(88,i*48+i*2+64,tempMessage,6);
				}else{
					break;
				}
			}
			DrawAnimationAsISay(&selectorAnimation,0,selected*48+selected*2+64-10,4);
		#else
			DrawText( CenterText(N_MAGICLIST,2),2,N_MAGICLIST,2);
	
			for (i=0;i<numberOfSkillsSelect;i++){
				if (member.fighterStats.spells[i]!=0){
					//ShowMessage("a",0);
					spellLinkedList* tempList = GetSpellList(member.fighterStats.spells[i]-1);
					sprintf((char*)tempMessage,"%s:%d",tempList->theSpell.name,tempList->theSpell.mpCost);
					DrawText(88,i*16+i*2+20,tempMessage,2);
				}else{
					break;
				}
			}
			DrawAnimationAsISay(&selectorAnimation,0,selected*16+selected*2+20-10,2);
		#endif

		EndDrawing();

		EndFrameStuff();
	}
}

void DisposeAllLoadedSpellImages(){
	int i=0;
	spellLinkedList* gotList;
	for (i=0;i<spellLinkedListSize;i++){
		gotList = GetSpellList(i);
		if (gotList->theSpell.theAnimation.texture!=NULL){
			FreeTexture(gotList->theSpell.theAnimation.texture);
			gotList->theSpell.theAnimation.texture=NULL;
		}
	}
}

void LoadSpellImageIfNeeded(int spellId){
	spell* gotSpell = &(GetSpellList(spellId)->theSpell);
	if (gotSpell->theAnimation.texture==NULL){
		gotSpell->theAnimation.texture = LoadPNG(gotSpell->spellPath);
	}
}

void AutodetectNumberOfFrames(animation* passedAnimation){
	passedAnimation->numberOfFrames = (GetTextureWidth(passedAnimation->texture)/passedAnimation->width);
}

char DidJustTouch(){
	#if PLATFORM == PLAT_VITA
	if (currentTouch.reportNum>0 && previousTouchData.reportNum==0){
		return 1;
	}else{
		return 0;
	}
	#elif PLATFORM == PLAT_WINDOWS
		printf("DidJustTOuch not yet for Windows.");
	#elif PLATFORM == PLAT_3DS
		return 0;
	#endif
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

void ClearBottomScreen(){
	#if PLATFORM == PLAT_3DS
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		EndDrawing();
	#endif
}

void LevelUp(partyMember* passedMember, int removexp){
	if (removexp==1){
		passedMember->fighterStats.exp-=passedMember->fighterStats.level*5+10;
	}
	passedMember->fighterStats.level++;
	
	passedMember->fighterStats.attack+=3;
	passedMember->fighterStats.magicAttack+=3;
	passedMember->fighterStats.defence+=3;
	passedMember->fighterStats.magicDefence+=3;
	passedMember->fighterStats.speed+=3;
	passedMember->fighterStats.maxHp+=3;
	passedMember->fighterStats.maxMp+=3;
	passedMember->hp=passedMember->fighterStats.maxHp;
	passedMember->mp=passedMember->fighterStats.maxMp;
}
// DOES NOT CHANGE EXP
void LevelDown(partyMember* passedMember){
	passedMember->fighterStats.level--;
	
	passedMember->fighterStats.attack-=3;
	passedMember->fighterStats.magicAttack-=3;
	passedMember->fighterStats.defence-=3;
	passedMember->fighterStats.magicDefence-=3;
	passedMember->fighterStats.speed-=3;
	passedMember->fighterStats.maxHp-=3;
	passedMember->fighterStats.maxMp-=3;
	passedMember->hp=passedMember->fighterStats.maxHp;
	passedMember->mp=passedMember->fighterStats.maxMp;
}

void Save(){
	int i=0;
	int j=0;
	FILE* fp;

	#if PLATFORM == PLAT_WINDOWS
		fp = fopen ("./savefile", "w");
	#elif PLATFORM == PLAT_VITA
		fp = fopen ("savedata0:savefile", "w");
	#elif PLATFORM == PLAT_3DS
		fp = fopen ("/3ds/data/HappyLand/savefile", "w");
	#endif

	short pathLength = strlen(currentMapFilepath);

	fwrite(&(pathLength),2,1,fp);
	fwrite((currentMapFilepath),pathLength,1,fp);
	fwrite(&(playerObject->x),2,1,fp);
	fwrite(&(playerObject->y),2,1,fp);
	fwrite(&(partySize),1,1,fp);
	for (i=0;i<4;i++){
		fwrite(&(party[i].fighterStats.level),2,1,fp);
		fwrite(&(party[i].fighterStats.exp),2,1,fp);
		fwrite(&(party[i].hp),2,1,fp);
		fwrite(&(party[i].mp),2,1,fp);
	}

	// Write flags

	char passedNumberOfFlags;
	lua_getglobal(L,"numberOfFlags");
	passedNumberOfFlags = (char)lua_tonumber(L,-1);
	fwrite(&(passedNumberOfFlags),1,1,fp);
	//printf("numberofflags: %d\n", passedNumberOfFlags);
	for (i=1;i<passedNumberOfFlags;i++){
		lua_getglobal(L,"flags");
		lua_pushnumber(L,i);
		lua_gettable(L,-2);
		// get flag value
		char got = lua_tonumber(L,-1);
		// Pop table
		lua_pop(L,-1);
		fwrite(&(got),1,1,fp);
		//printf("wrote %d\n",got);
	}

	char zero=0;
	// Save the skills of the party members
	// Write 0 at the end
	for (j=0;j<4;j++){
		for (i=0;i<20;i++){
			if (party[j].fighterStats.spells[i]!=0){
				printf("Wrote %d for party member %d\n",party[j].fighterStats.spells[i],j);
				fwrite(&(party[j].fighterStats.spells[i]),1,1,fp);
			}else{
				printf("Stopped as %d has %d.",j,i);
				break;
			}
		}
		fwrite(&(zero),1,1,fp);
	}



	fclose(fp);
}

void Load(){
	int i=0;
	int j=0;

	FILE* fp;

	#if PLATFORM == PLAT_WINDOWS
		fp = fopen ("./savefile", "r");
	#elif PLATFORM == PLAT_VITA
		fp = fopen ("savedata0:savefile", "r");
	#elif PLATFORM == PLAT_3DS
		fp = fopen ("/3ds/data/HappyLand/savefile", "r");
	#endif
	//fprintf(fp, "%s\n", currentMapFilepath);
	////fread
	//fwrite(playerObject->x,2,1,fp);
	//fwrite(playerObject->y,2,1,fp);

	free(currentMapFilepath);

	// All your memories
	// Levels
	// I'll bring them all back to zero.
	for (j=2;j<=party[i].fighterStats.level;j++){
		printf("Leveled down %d/%d\n",j-1,party[i].fighterStats.level);
		LevelDown(&(party[i]));
	}

	short pathLength;

	fread(&pathLength,2,1,fp);
	currentMapFilepath = malloc(pathLength+1);
	currentMapFilepath[pathLength]='\0';
	fread(currentMapFilepath,pathLength,1,fp);

	fread(&(playerObject->x),2,1,fp);
	fread(&(playerObject->y),2,1,fp);

	unsigned short readLevels[4];
	short readHp[4];
	short readMp[4];

	char newPartySize;
	fread(&(newPartySize),1,1,fp);

	printf("party size is %d\n", newPartySize);

	char happy[17];
	if (newPartySize>partySize){
		for (i=partySize+1;i<=newPartySize;i++){
			sprintf((char*)(&happy),"AddPartyMember%d",i);
			lua_getglobal(L,happy);
			lua_call(L,0,0);
		}
	}



	for (i=0;i<4;i++){
		fread(&(readLevels[i]),2,1,fp);
		fread(&(party[i].fighterStats.exp),2,1,fp);
		fread(&(readHp[i]),2,1,fp);
		fread(&(readMp[i]),2,1,fp);
	}

	// READ FLAGS
	char numberOfFlags;
	fread(&(numberOfFlags),1,1,fp);
	printf("Totalflags: %d\n",numberOfFlags);
	lua_getglobal(L,"flags");
	for (i=1;i<numberOfFlags;i++){
		printf("LOAD FLAG\n");
		lua_pushnumber(L,i);
		char justReadValue;
		fread(&(justReadValue),1,1,fp);
		lua_pushnumber(L,justReadValue);
		lua_settable(L,-3);
	}
	lua_pop(L,-1);

	// TODO - saving and loading skills.
	// ugh.
	// But I kind of like working on saving and loading.
	// I think I should've added it before the game was released.

	char lastRead;
	for (j=0;j<4;j++){
		for (i=0;i<20;i++){
			fread(&lastRead,1,1,fp);
			if (lastRead!=0){
				printf("Spell %d for party %d is %d\n",i,j,lastRead);
				party[j].fighterStats.spells[i]=lastRead;
			}else{
				break;
			}
		}
	}



	fclose(fp);

	for (i=0;i<4;i++){
		
		for (j=1;j<readLevels[i];j++){
			//printf("Leveled up %d\n",j);
			LevelUp(&(party[i]),0);
		}
		party[i].hp=readHp[i];
		party[i].mp=readMp[i];
	}

	printf("pathlength: %d\n", pathLength);
	printf("Mappath: %s\n", currentMapFilepath);
	printf("x: %d, y: %d\n",playerObject->x,playerObject->y);


	ChangeMap(currentMapFilepath);
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
		StartDrawing();

		#if PLATFORM != PLAT_3DS
			DrawText(SCREENWIDTH/2-strlen(party[selectedMember].fighterStats.name)*64/2,3,party[selectedMember].fighterStats.name,8);
			DrawText(3,69,N_ATK,4);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.attack);
			DrawText(strlen(N_ATK)*32+32,69,statNumberString,4);
			DrawText(3,106,N_MATK,4);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.magicAttack);
			DrawText(strlen(N_MATK)*32+32,106,statNumberString,4);
			DrawText(3,143,N_DEF,4);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.defence);
			DrawText(strlen(N_DEF)*32+32,143,statNumberString,4);
			DrawText(3,180,N_MDEF,4);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.magicDefence);
			DrawText(strlen(N_MDEF)*32+32,180,statNumberString,4);
			DrawText(3,217,N_SPEED,4);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.speed);
			DrawText(strlen(N_SPEED)*32+32,217,statNumberString,4);
	
			sprintf((char*)&statNumberString,"%d/%d",party[selectedMember].hp,party[selectedMember].fighterStats.maxHp);
			DrawText(strlen(N_HP)*32+32,254,statNumberString,4);
			DrawText(3,254,N_HP,4);
			sprintf((char*)&statNumberString,"%d/%d",party[selectedMember].mp,party[selectedMember].fighterStats.maxMp);
			DrawText(strlen(N_MP)*32+32,291,statNumberString,4);
			DrawText(3,291,N_MP,4);
			
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.level);
			DrawText(strlen(N_LV)*32+32,328,statNumberString,4);
			DrawText(3,328,N_LV,4);
			
			double animationScale=(SCREENHEIGHT-180)/ partyIdleAnimation[selectedMember].height;
			DrawAnimationAsISay(&partyIdleAnimation[selectedMember],SCREENWIDTH-partyIdleAnimation[selectedMember].width*animationScale-100,180,animationScale);
		#elif PLATFORM == PLAT_3DS
			DrawMapThings();
			EndDrawing();
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			
			// Draw name
			DrawText(CenterTextCustomWidth(party[selectedMember].fighterStats.name,3,BOTTOMSCREENWIDTH),3,party[selectedMember].fighterStats.name,3);
			
			int i=0;
			// Draw attack
			i++;
			DrawText(3,20*i+2*i+10,N_ATK,2);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.attack);
			DrawText(strlen(N_ATK)*16+16,20*i+2*i+10,statNumberString,2);

			i++;
			DrawText(3,20*i+2*i+10,N_MATK,2);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.magicAttack);
			DrawText(strlen(N_MATK)*16+16,20*i+2*i+10,statNumberString,2);

			i++;
			DrawText(3,20*i+2*i+10,N_DEF,2);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.defence);
			DrawText(strlen(N_DEF)*16+16,20*i+2*i+10,statNumberString,2);

			i++;
			DrawText(3,20*i+2*i+10,N_MDEF,2);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.magicDefence);
			DrawText(strlen(N_MDEF)*16+16,20*i+2*i+10,statNumberString,2);

			i++;
			DrawText(3,20*i+2*i+10,N_SPEED,2);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.speed);
			DrawText(strlen(N_SPEED)*16+16,20*i+2*i+10,statNumberString,2);
			
			i++;
			sprintf((char*)&statNumberString,"%d/%d",party[selectedMember].hp,party[selectedMember].fighterStats.maxHp);
			DrawText(strlen(N_HP)*16+16,20*i+2*i+10,statNumberString,2);
			DrawText(3,20*i+2*i+10,N_HP,2);

			i++;
			sprintf((char*)&statNumberString,"%d/%d",party[selectedMember].mp,party[selectedMember].fighterStats.maxMp);
			DrawText(strlen(N_MP)*16+16,20*i+2*i+10,statNumberString,2);
			DrawText(3,20*i+2*i+10,N_MP,2);
			
			i++;
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.level);
			DrawText(strlen(N_LV)*16+32,20*i+2*i+10,statNumberString,2);
			DrawText(3,20*i+2*i+10,N_LV,2);			
			
			double animationScale=(BOTTOMSCREENHEIGHT-120)/ (double)partyIdleAnimation[selectedMember].height;
			DrawAnimationAsISay(&partyIdleAnimation[selectedMember],BOTTOMSCREENWIDTH-partyIdleAnimation[selectedMember].width*animationScale-32,120,animationScale);
		#endif

		EndDrawing();
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
					// 	Resume
					case 0:
						place=0;
						return;
					break;
					// Status
					case 1:
						place=4;
						return;
					break;
					// SAVE
					case 2:
						Save();
						return;
					break;
					// LOAD
					case 3:
						subspot=1;
						selected=1;
					break;
					// QUIT
					case 4:
						place=2;
						return;
					break;
				}
			}else if (subspot==1){
				switch(selected){
					case 0:
						Load();
						place=0;
						return;
					break;
					case 1:
						subspot=0;
					break;
				}
			}
		}else if (WasJustPressed(SCE_CTRL_START)){
			place=0;
			return;
		}else if (WasJustPressed(bButton)){
			if (subspot==0){
				place=0;
				return;
			}else{
				subspot=0;
			}
		}
		if (WasJustPressed(SCE_CTRL_DOWN)){
			selected++;
			if (subspot==0){
				if (selected>4){
					selected=0;
				}
			}else if (subspot==1){
				if (selected>1){
					selected=0;
				}
			}
		}else if (WasJustPressed(SCE_CTRL_UP)){
			selected--;
			if (subspot==0){
				if (selected<0){
					selected=4;
				}
			}else if (subspot==1){
				if (selected<0){
					selected=0;
				}
			}
		}


		StartDrawing();
	
		DrawMapThings();


		#if PLATFORM!=PLAT_3DS
			// Draw fancy shade thingie
			DrawRectangle(720,136,5,272,0,0,0,255);
			DrawRectangle(240,408,485,5,0,0,0,255);
			// Draw border
			DrawRectangle(240,136,480,272,0,255,0,255);
			// Draw real rectangle
			DrawRectangle(245,141,470,262,252,255,255,255);
	
			if (subspot==0){
				DrawText(CenterText(N_HAPPYMENU,4),146,N_HAPPYMENU,4);
				// 0
				DrawText(287,183,N_RESUME,4);
				// 1
				DrawText(287,183+32+5,playerName,4);
				// 2
				DrawText(287,183+32+32+5+5,N_SAVE,4);
				// 3
				DrawText(287,183+32+32+32+5+5+5,N_LOAD,4);
				// 4
				DrawText(287,183+32+32+32+32+5+5+5+5,N_QUIT,4);
			}else if (subspot==1){
				if (LANGUAGE==LANG_SPANISH){
					DrawText(CenterText("?Quieres cargar?",4),146,"'?Quieres cargar?",4);
					DrawText(287,183,"S'i.",4);
					DrawText(287,183+32+5,"No.",4);
				}else if (LANGUAGE==LANG_ENGLISH){
					DrawText(CenterText("Really load?",4),146,"Really load?",4);
					DrawText(287,183,"Yes.",4);
					DrawText(287,183+32+5,"No.",4);
				}
			}
			DrawAnimationAsISay(&selectorAnimation,245,selected*32+183+selected*5,2);
		#elif PLATFORM == PLAT_3DS
			EndDrawing();
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			// Draw real rectangle
			DrawRectangle(0,0,320,240,252,255,255,255);
	
			if (subspot==0){
				DrawText(5,0,N_HAPPYMENU,3.5);
				// 0
				DrawText(30,32+24+5,N_RESUME,3);
				// 1
				DrawText(30,32+24+24+5+5,playerName,3);
				// 2
				DrawText(30,32+24+24+24+5+5+5,N_SAVE,3);
				// 3
				DrawText(30,32+24+24+24+24+5+5+5+5,N_LOAD,3);
				// 4
				DrawText(30,32+24+24+24+24+24+5+5+5+5+5,N_QUIT,3);
			}else if (subspot==1){
				if (LANGUAGE==LANG_SPANISH){
					DrawText(5,0,"'?Quieres cargar?",3.5);
					DrawText(30,32+24+5,"S'i.",3);
					DrawText(30,32+24+25+5+5,"No.",3);
				}else if (LANGUAGE==LANG_ENGLISH){
					DrawText(5,0,"Really load?",3.5);
					DrawText(30,32+24+5,"Yes.",3);
					DrawText(30,32+24+25+5+5,"No.",3);
				}
			}
			DrawAnimationAsISay(&selectorAnimation,0,selected*24+32+24+selected*5+5,1);
		#endif


		EndDrawing();
		EndFrameStuff();
	}
}

void Overworld(){
	StartFrameStuff();
	// Main logic

	// Controls only if not walking
	if (isWalking==0){
		if (1){
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
			if (IsDown(SCE_CTRL_UP)){
				if (checkCollision(playerObject->x/32,playerObject->y/32-1)!=1){
					isWalking=1;
					playerObject->theAnimation.numberOfFrames=3;
					playerObject->y=playerObject->y-4;
				}
				playerObject->theAnimation.texture=playerUp;
			}else if (IsDown(SCE_CTRL_DOWN)){
				if (checkCollision(playerObject->x/32,playerObject->y/32+1)!=1){
					isWalking=2;
					playerObject->theAnimation.numberOfFrames=3;
					playerObject->y=playerObject->y+4;
				}
				playerObject->theAnimation.texture=playerDown;
			}else if (IsDown(SCE_CTRL_LEFT)){
				if (checkCollision(playerObject->x/32-1,playerObject->y/32)!=1){
					isWalking=3;
					playerObject->theAnimation.numberOfFrames=3;
					playerObject->x=playerObject->x-4;
				}
				playerObject->theAnimation.texture=playerLeft;
			}else if (IsDown(SCE_CTRL_RIGHT)){
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
				if (!((IsDown(SCE_CTRL_UP)) || (IsDown(SCE_CTRL_DOWN)) || (IsDown(SCE_CTRL_LEFT)) || (IsDown(SCE_CTRL_RIGHT)))){
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
	StartDrawing();

	DrawMapThings();
	//DrawText(60,60,"Hello world",2);

	EndDrawing();
	EndFrameStuff();
}

char BattleLop(char canRun){
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

	// FOr checking if somebody is alive.
	// if 0 after battle end, player lost
	char isOneAlive=0;

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

	CrossTexture* attackButton=NULL;
	CrossTexture* magicButton=NULL;
	CrossTexture* runButton=NULL;
	CrossTexture* itemButton=NULL;
	CrossTexture* winTexture=NULL;

	FixPath("Stuff/AttackIcon.png",tempPathFixBuffer,TYPE_EMBEDDED);
	attackButton = LoadPNG(tempPathFixBuffer);
	FixPath("Stuff/MagicIcon.png",tempPathFixBuffer,TYPE_EMBEDDED);
	magicButton = LoadPNG(tempPathFixBuffer);
	FixPath("Stuff/RunIcon.png",tempPathFixBuffer,TYPE_EMBEDDED);
	runButton = LoadPNG(tempPathFixBuffer);
	FixPath("Stuff/ItemIcon.png",tempPathFixBuffer,TYPE_EMBEDDED);
	itemButton = LoadPNG(tempPathFixBuffer);
	FixPath("Stuff/Battle/Win.png",tempPathFixBuffer,TYPE_EMBEDDED);
	winTexture = LoadPNG(tempPathFixBuffer);

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
			StartDrawing();
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
					DrawAnimationAsISay((GetBattlerAnimationById(i,0)),xPositions[i-1],yPositions[i-1],BATTLEENTITYSCALE);
					continue;
				}
				xPositions[i-1]+=addPerFrame[i-1];

				DrawAnimationAsISay((GetBattlerAnimationById(i,0)),xPositions[i-1],yPositions[i-1],BATTLEENTITYSCALE);
			}
			EndDrawing();

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
				isOneAlive=0;
				for (i=0;i<partySize;i++){
					if (party[i].hp>0){
						isOneAlive=1;
					}
				}
				if (isOneAlive==0){
					// player has lost.
					break;
				}
				isOneAlive=0;
				for (i=0;i<numberOfEnemies;i++){
					if (enemies[i].hp>0){
						isOneAlive=1;
					}
				}
				if (isOneAlive==0){
					// Set this back to 1 to not trigger thing after battle end
					isOneAlive=1;
	
					int totalEarnExp=0;
					for (i=0;i<numberOfEnemies;i++){
						totalEarnExp+=enemies[i].fighterStats.exp;
						for (k=0;k<10;k++){
							if (enemies[i].fighterStats.spells[k]!=0){
								for (j=0;j<partySize;j++){
									AddSpellToStats(&party[j].fighterStats,enemies[i].fighterStats.spells[k]);
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
							LevelUp(&(party[i]),1);
				
							didLevelUp[i]=1;
						}
					}
				
					sprintf((char*)&temp2,"%d",totalEarnExp);
					while (1){
						StartFrameStuff();
				
						if (WasJustPressed(aButton)){
							break;
						}
						#if PLATFORM != PLAT_3DS
							StartDrawing();
							DrawTextureScale(winTexture,CenterSomething(GetTextureWidth(winTexture)*2),3,2,2);

							DrawText(0,206+64,"EXP:",8);
							DrawText(256,206+64,temp2,8);
					
							for (i=0;i<partySize;i++){
								if (didLevelUp[i]==1){
									DrawText(0,300+i*32+i*5+64,party[i].fighterStats.name,4);
									DrawText(strlen(party[i].fighterStats.name)*32+32,300+i*32+i*5+64,N_LEVELEDUP,4);
								}
							}
						#else
							StartDrawing();
								DrawTextureScale(winTexture,CenterSomething(GetTextureWidth(winTexture)*1.8),3,1.8,1.8);
							EndDrawing();
							sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);

							DrawText(5,5,"EXP:",4);
							DrawText(8*4*5+5,5,temp2,4);
					
							for (i=0;i<partySize;i++){
								if (didLevelUp[i]==1){
									DrawText(5,40+i*24+i*5,party[i].fighterStats.name,2);
									DrawText(5+strlen(party[i].fighterStats.name)*16+16,40+i*24+i*5,N_LEVELEDUP,2);
								}
							}
						#endif
				
						EndDrawing();
				
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
					if (RandBetween(1,4)==1 && GetNumberOfSpells(&GetBattlerById(orderOfAction[(int)currentOrder])->fighterStats)>0){
						int enemySpellSelected = GetBattlerById(orderOfAction[currentOrder])->fighterStats.spells[RandBetween(0,GetNumberOfSpells(&GetBattlerById(orderOfAction[(int)currentOrder])->fighterStats)-1)]-1;
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
						moveXPerFrame=floor((GetBattlerById(target)->x - GetBattlerById(orderOfAction[currentOrder])->x - GetBattlerAnimationById(orderOfAction[currentOrder],2)->width*BATTLEENTITYSCALE+32 )/30);
						moveYPerFrame=floor((GetBattlerById(target)->y - GetBattlerById(orderOfAction[currentOrder])->y + GetBattlerAnimationById(target,1)->height/4)/30);
		
						if (moveXPerFrame==0){
							moveXPerFrame=ceil((GetBattlerById(target)->x - GetBattlerById(orderOfAction[currentOrder])->x - GetBattlerAnimationById(orderOfAction[currentOrder],2)->width*BATTLEENTITYSCALE+32 )/30);
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
			#if PLATFORM != PLAT_3DS
			if (WasJustPressed(SCE_CTRL_RIGHT))
			#else
			if (WasJustPressed(SCE_CTRL_DOWN))
			#endif
			{
				selected++;
				if (canRun==1){
					if (selected==3){
						selected=0;
					}
				}else{
					if (selected==2){
						selected=0;
					}
				}
			}
			#if PLATFORM!=PLAT_3DS
			if (WasJustPressed(SCE_CTRL_LEFT))
			#else
			if (WasJustPressed(SCE_CTRL_UP))
			#endif
			{
				selected--;
				if (selected<0){
					if (canRun==1){
						selected=2;
					}else{
						selected=1;
					}
				}
			}
			if (WasJustPressed(aButton)/* || DidJustTouch()==1*/){
				// TOuch controls
				/*if (DidJustTouch()==1){
					if (currentTouch.report[0].y/2>=SCREENHEIGHT-GetTextureHeight(attackButton)*2){
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
					EndFrameStuff();
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
					moveXPerFrame=floor((GetBattlerById(target)->x - GetBattlerById(orderOfAction[currentOrder])->x - GetBattlerAnimationById(orderOfAction[currentOrder],2)->width*BATTLEENTITYSCALE+32 )/30);
					moveYPerFrame=floor((GetBattlerById(target)->y - GetBattlerById(orderOfAction[currentOrder])->y + GetBattlerAnimationById(target,1)->height/4)/30);
	
					if (moveXPerFrame==0){
						moveXPerFrame=ceil((GetBattlerById(target)->x - GetBattlerById(orderOfAction[currentOrder])->x - GetBattlerAnimationById(orderOfAction[currentOrder],2)->width*BATTLEENTITYSCALE+32 )/30);
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
			
			if (moveXPerFrame>=0 && GetBattlerById(orderOfAction[currentOrder])->x>=GetBattlerById(target)->x-GetBattlerAnimationById(orderOfAction[currentOrder],2)->width*BATTLEENTITYSCALE){
				moveXPerFrame=0;
			}
			if (moveXPerFrame<=0 && GetBattlerById(orderOfAction[currentOrder])->x<=GetBattlerById(target)->x+GetBattlerAnimationById(orderOfAction[currentOrder],2)->width*BATTLEENTITYSCALE){
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
			UpdateAnimationIfNeed(&(selectedSpell->theAnimation));
			if (selectedSpell->theAnimation.currentFrame==0 && temp!=0){
				currentOrder++;
				battleStatus=0;
			}
		}

		#if NOSLEEP==1
			sceKernelPowerTick(0);
		#endif

		StartDrawing();
		DrawMap();
		DrawUnusedAreaRect();

		// Draw selection buttons
		if (battleStatus==1){
			// Draw later on the bottom screen if on 3ds
			#if PLATFORM != PLAT_3DS
				// Draw UI and selector
				DrawTextureScale(attackButton,32,SCREENHEIGHT-GetTextureHeight(attackButton)*2,2,2);
				DrawTextureScale(magicButton,264,SCREENHEIGHT-GetTextureHeight(magicButton)*2,2,2);
				//DrawTextureScale(itemButton,496,SCREENHEIGHT-GetTextureHeight(itemButton)*2,2,2);
				if (canRun==1){
					DrawTextureScale(runButton,/*728*/496,SCREENHEIGHT-GetTextureHeight(itemButton)*2,2,2);
				}
				UpdateAnimationIfNeed(&selectorAnimation);
				DrawTexturePartScaleRotate(selectorAnimation.texture,selected*200+selected*32+132,SCREENHEIGHT-128,selectorAnimation.currentFrame*selectorAnimation.width,0,selectorAnimation.width,selectorAnimation.height,3.7,3.7,1.57);
			#endif
		}
		// Draw the good stuff.
		for (i=0;i<partySize;i++){

			if (party[i].hp==0){
				continue;
			}

			#if PLATFORM==PLAT_WINDOWS || PLATFORM==PLAT_VITA
				// Draw health and MP bar for party member
				// hp
				DrawRectangle(32+i*128+i*16,16,128,32,0,0,0,255);
				if (party[i].hp>=party[i].fighterStats.maxHp+15){
					// Draw more red rectangle if overheal
					DrawRectangle(32+i*128+i*16,16,floor(128*(((double)party[i].hp)/party[i].fighterStats.maxHp)),32,255,0,0,255);
				}else{
					// Draw normal health red
					DrawRectangle(32+i*128+i*16,16,floor(128*(((double)party[i].hp)/party[i].fighterStats.maxHp)),32,190,0,0,255);
				}
				// mp
				DrawRectangle(32+i*128+i*16,48,128,32,0,0,0,255);
				DrawRectangle(32+i*128+i*16,48,floor(128*(((double)party[i].mp)/party[i].fighterStats.maxMp)),32,0,0,190,255);
				// name
				DrawText(32+i*128+i*16,90,party[i].fighterStats.name,2);
			#endif
			// Draws attack animation for person moving to attack
			if (battleStatus==3){
				if (i+1==orderOfAction[currentOrder]){
					DrawAnimationAsISay(&(partyAttackAnimation[i]),party[i].x,party[i].y,BATTLEENTITYSCALE);
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
					
					DrawAnimationAsISay(&(partyIdleAnimation[i]),party[i].x,party[i].y,BATTLEENTITYSCALE);
					continue;
				}
				
			}
			DrawAnimationAsISay(&(partyIdleAnimation[i]),party[i].x,party[i].y,BATTLEENTITYSCALE);			
		}
		for (i=0;i<numberOfEnemies;i++){
			if (enemies[i].hp==0){
				continue;
			}

			if (battleStatus==3){
				if (i+5==orderOfAction[currentOrder]){
					DrawAnimationAsISay((enemyAttackAnimation[i]),enemies[i].x,enemies[i].y,BATTLEENTITYSCALE);
					continue;
				}
			}
			DrawAnimationAsISay(enemyIdleAnimation[i],enemies[i].x,enemies[i].y,BATTLEENTITYSCALE);
		}

		if (battleStatus==5){// SPELL
			temp=selectedSpell->theAnimation.currentFrame;
			DrawAnimationAsISay(&(selectedSpell->theAnimation),GetBattlerById(target)->x,GetBattlerById(target)->y,SPELLSCALE);
			if (GetBattlerById(target)->y-(8*DAMAGETEXTSIZE)-drawYOffset>=0){
				DrawText(GetBattlerById(target)->x-floor(strlen(temp2)/2)*(8*DAMAGETEXTSIZE),GetBattlerById(target)->y-(8*DAMAGETEXTSIZE),temp2,DAMAGETEXTSIZE);
			}else{
				DrawText(GetBattlerById(target)->x-floor(strlen(temp2)/2)*(8*DAMAGETEXTSIZE),GetBattlerById(target)->y+ GetBattlerAnimationById(target,1)->height+70,temp2,DAMAGETEXTSIZE);
			}
		}else if (battleStatus==4){ // BACK FROM ATTACK
			if (GetBattlerById(target)->y-(8*DAMAGETEXTSIZE)-drawYOffset>=0){
				DrawText(GetBattlerById(target)->x-floor(strlen(temp2)/2)*(8*DAMAGETEXTSIZE),GetBattlerById(target)->y-(8*DAMAGETEXTSIZE),temp2,DAMAGETEXTSIZE);
			}else{
				DrawText(GetBattlerById(target)->x-floor(strlen(temp2)/2)*(8*DAMAGETEXTSIZE),GetBattlerById(target)->y+ GetBattlerAnimationById(target,1)->height+(8*DAMAGETEXTSIZE),temp2,DAMAGETEXTSIZE);
			}
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

			// hp
			DrawRectangle(GetBattlerById(target)->x,GetBattlerById(target)->y-32,64*BATTLEENTITYSCALE,32,0,0,0,255);
			DrawRectangle(GetBattlerById(target)->x,GetBattlerById(target)->y-32,floor(64*BATTLEENTITYSCALE*(((double)GetBattlerById(target)->hp)/GetBattlerById(target)->fighterStats.maxHp)),32,190,0,0,255);

			// Draw the target selector
			DrawAnimationAsISay(&selectorAnimation,GetBattlerById(target)->x-22*DAMAGETEXTSIZE,GetBattlerById(target)->y,DAMAGETEXTSIZE);
		}
		EndDrawing();
	
		// Draw bottom screen stuff on 3ds
		#if PLATFORM == PLAT_3DS
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			if (battleStatus==1){
				// Draw UI and selector
				DrawTextureScale(attackButton,100,48,1,1);
				DrawTextureScale(magicButton,100,112,1,1);
				if (canRun==1){
					DrawTextureScale(runButton,100,176,1,1);
				}
				
				//DrawAnimationAsISay(animation* animationToDraw, int destX, int destY, int scale)
				DrawAnimationAsISay(&selectorAnimation,73,selected*64+48+22,1);
				//(selectorAnimation.texture,73,selected*64+15+32,selectorAnimation.currentFrame*selectorAnimation.width,0,selectorAnimation.width,selectorAnimation.height,3.7,3.7,1.57);
			
				// Draw health and MP bar for party member and also name
				for (i=0;i<partySize;i++){
		
					if (party[i].hp==0){
						continue;
					}
					// hp
					DrawRectangle((i+1)*10+i*70,2,70,15,0,0,0,255);
					if (party[i].hp>=party[i].fighterStats.maxHp+15){
						// Draw more red rectangle if overheal
						DrawRectangle((i+1)*10+i*70,2,floor(70*(((double)party[i].hp)/party[i].fighterStats.maxHp)),2150,0,0,0,255);
					}else{
						// Draw normal health red
						DrawRectangle((i+1)*10+i*70,2,floor(70*(((double)party[i].hp)/party[i].fighterStats.maxHp)),15,190,0,0,255);
					}
					// mp
					DrawRectangle((i+1)*10+i*70,17,70,15,0,0,0,255);
					DrawRectangle((i+1)*10+i*70,17,floor(70*(((double)party[i].mp)/party[i].fighterStats.maxMp)),15,0,0,190,255);
					// name
					DrawText((i+1)*10+i*70,34,party[i].fighterStats.name,1);
				}

			}
			sf2d_end_frame();
			sf2d_swapbuffers();
		#endif

		EndFrameStuff();
	}
	PlzNoCrashOnDispose();
	FreeTexture(attackButton);
	FreeTexture(magicButton);
	FreeTexture(itemButton);
	FreeTexture(runButton);
	FreeTexture(winTexture);
	DisposeAllLoadedSpellImages();

	ClearBottomScreen();

	if (isOneAlive==0){
		if (canRun==1){
			ShowMessage(N_DEADMESSAGE,0);
			place=0;
			playerObject->x=64;
			playerObject->y=64;

			RestorePartyMember(0);
			RestorePartyMember(1);
			RestorePartyMember(2);
			RestorePartyMember(3);

			FixPath("Stuff/Maps/NathansHouse",tempPathFixBuffer,TYPE_EMBEDDED);
			ChangeMap(tempPathFixBuffer);
			
		}else{
			place=0;
		}
		return 0;
	}

	// Make sure no rapid battle lop
	if (place==3){
		ShowMessage("That shouldn't have happenedededed. Overworld, go!",0);
		place=0;
	}

	return 1;
}

void TitleLoop(){
	FixPath("Stuff/title.png",tempPathFixBuffer,TYPE_EMBEDDED);
	CrossTexture* titleImage = LoadPNG(tempPathFixBuffer);
	
	while (place!=2){
		StartFrameStuff();

		if (IsDown(aButton)){
			if (LANGUAGE == LANG_ENGLISH){
				lua_pushnumber(L,1);
			}else if (LANGUAGE==LANG_SPANISH){
				lua_pushnumber(L,2);
			}
			lua_setglobal(L,"Lang");

			SetupHardcodedLanguage();

			// Need to load here as Lang variable has been set.
			FixPath("Stuff/Happy.lua",tempPathFixBuffer,TYPE_EMBEDDED);
			luaL_dofile(L,tempPathFixBuffer);

			if (IsDown(SCE_CTRL_UP)==1 && IsDown(SCE_CTRL_CROSS)==1 && IsDown(SCE_CTRL_CIRCLE)==1){
				BasicMessage("You activated the top secret key combo. You will get a high level. Restart the game to play normally");
				party[0].fighterStats.maxHp=999;
				party[0].fighterStats.speed=999;
				party[0].fighterStats.attack=999;
				party[0].fighterStats.defence=999;
				party[0].fighterStats.magicDefence=999;
				party[0].fighterStats.level=100;
			}

			LoadMap(STARTINGMAP);
			place=0;	

			ClearBottomScreen();
			break;
		}
		if (WasJustPressed(SCE_CTRL_SQUARE)){
			#if PLATFORM != PLAT_VITA
				if (aButton==SCE_CTRL_CROSS){
					aButton=SCE_CTRL_CIRCLE;
					bButton=SCE_CTRL_CROSS;
				}else{
					bButton=SCE_CTRL_CIRCLE;
					aButton=SCE_CTRL_CROSS;
				}
			#endif
		}

		StartDrawing();

		//DrawTexture(titleImage,0,0);
		DrawTextureScale(titleImage,0,0,(float)SCREENWIDTH/GetTextureWidth(titleImage),(float)SCREENHEIGHT/GetTextureHeight(titleImage));

		#if PLATFORM != PLAT_3DS
			if (aButton==SCE_CTRL_CROSS){
				if (LANGUAGE==LANG_ENGLISH){
					DrawText(51+8,443+15,"X and O: Not Japan",3.5);
				}else if (LANGUAGE==LANG_SPANISH){
					DrawText(51+8,443+15,"X y O: NO Jap'on",3.5);
				}
			}else{
				if (LANGUAGE==LANG_ENGLISH){
					DrawText(51+8,443+15,"X and O: Japan",3.5);
				}else if (LANGUAGE==LANG_SPANISH){
					DrawText(51+8,443+15,"X y O: Jap'on",3.5);
				}
			}
		#elif PLATFORM == PLAT_3DS
			EndDrawing();
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			if (LANGUAGE==LANG_ENGLISH){
				DrawText(0,0,"X: Espa'nol",2);
			}else if (LANGUAGE==LANG_SPANISH){
				DrawText(0,0,"X: English",2);
			}
		#endif

		EndDrawing();
		
		EndFrameStuff();
	}
	PlzNoCrashOnDispose();
	FreeTexture(titleImage);
}

/*
///////////////////////////////////////
// LUA FUNCTIONS
// Alright, so I had this idea. Instead of having set and get
// functions, I could just send pointers as light user data and
// then have a function to set those pointers.
///////////////////////////////////////
*/

#include "LuaFunctions.h"

/*
///////////////////////////////////////
// LUA FUNCTIONS END
///////////////////////////////////////
*/

char ShowMessageWithPortrait(char* _tempMsg, char isQuestion, CrossTexture* portrait, double portScale){
	ControlsReset();

	CrossTexture* yesButtonTexture=(CrossTexture*)1;
	CrossTexture* noButtonTexture=(CrossTexture*)1;
	char questionScale=1;
	// The string needs to be copied. We're going to modify it, at we can't if we just type the string into the function and let the compiler do everything else
	char message[strlen(_tempMsg)+1];
	strcpy(message,_tempMsg);
	signed char currentSelected=defaultSelected;
	int textboxLinesPerScreens = (SCREENHEIGHT-TEXTBOXY)/TextHeight(fontSize);
	short newLineLocations[50];
	int totalMessageLength = strlen(message);
	int i, j;
	
	if (isQuestion==1){
		FixPath("Stuff/Yes.png",tempPathFixBuffer,TYPE_EMBEDDED);
		yesButtonTexture=LoadPNG(tempPathFixBuffer);
		FixPath("Stuff/No.png",tempPathFixBuffer,TYPE_EMBEDDED);
		noButtonTexture=LoadPNG(tempPathFixBuffer);
		
		if (SCREENWIDTH/GetTextureWidth(yesButtonTexture)>=8){
			questionScale=2;
		}else{
			questionScale=1;
		}
	}
	if (portScale==0 && portrait!=NULL){
		portScale = floor((double)DEFAULTPORTRAITSIZE/GetTextureWidth(portrait));
	}

	// This will loop through the entire message, looking for where I need to add new lines. When it finds a spot that
	// needs a new line, that spot in the message will become 0. So, when looking for the place to 
	int lastNewlinePosition=0;
	for (i = 0; i < totalMessageLength; i++){
		if (message[i]==32){ // Only check when we meet a space. 32 is a space in ASCII
			message[i]='\0';
			//printf("Space found at %d\n",i);
			//printf("%s\n",&message[lastNewlinePosition+1]);
			//printf("%d\n",TextWidth(fontSize,&(message[lastNewlinePosition+1])));
			if (TextWidth(fontSize,&(message[lastNewlinePosition+1]))>SCREENWIDTH-20){
				char _didWork=0;
				for (j=i-1;j>lastNewlinePosition+1;j--){
					//printf("J:%d\n",j);
					if (message[j]==32){
						message[j]='\0';
						_didWork=1;
						message[i]=32;
						lastNewlinePosition=j;
						break;
					}
				}
				if (_didWork==0){
					message[i]='\0';
					lastNewlinePosition=i+1;
				}
			}else{
				message[i]=32;
			}
		}
	}
	printf("WIll try drawing....\n");

	// This spot in the message will be the one that's 0 char. The player won't be able to see this character.
	int currentLetter=0;
	int currentLine=0;
	signed int nextCharStorage=-1;
	char currentlyVisibleLines=1;
	char frames=0;
	// This variable is the location of the start of the first VISIBLE line
	// This will change if the text box has multiple screens because the text is too long
	int offsetStrlen=0;
	//  textboxNewCharSpeed
	
	nextCharStorage = message[0];
	message[0]='\0';

	while (1){
		FpsCapStart();
		ControlsStart();
		if (WasJustPressed(SCE_CTRL_CROSS)){
			if (currentlyVisibleLines<=textboxLinesPerScreens && currentLetter<totalMessageLength){
				while (currentlyVisibleLines<=textboxLinesPerScreens && currentLetter!=totalMessageLength){
					// Copied code from the normal if frames==textboxNewCharSpeed block
					if (nextCharStorage==0){
						currentlyVisibleLines++;
					}
					message[currentLetter]=nextCharStorage;
					currentLetter++;
					if (currentLetter!=totalMessageLength){
						nextCharStorage = message[currentLetter];
						message[currentLetter]='\0';
					}else{
						nextCharStorage=0;
					}
				}
			}else{
				if (currentLetter==totalMessageLength){
					break;
				}else{
					offsetStrlen=currentLetter;
					//currentLetter=0;
					//nextCharStorage=-1;
					currentlyVisibleLines=1;
					frames=0;
				}
			}
		}
		if (isQuestion==1){
			if (WasJustPressed(SCE_CTRL_DOWN)){
				currentSelected++;
				if (currentSelected>1){
					currentSelected=0;
				}
			}else if (WasJustPressed(SCE_CTRL_UP)){
				if (currentSelected==0){
					currentSelected=1;
				}else{
					currentSelected--;
				}
			}
		}
		ControlsEnd();
		
		// No Logic
		if (frames==textboxNewCharSpeed){
			if (currentLetter!=totalMessageLength && currentlyVisibleLines<=textboxLinesPerScreens){
				frames=0;
				if (nextCharStorage==0){
					currentlyVisibleLines++;
				}
				message[currentLetter]=nextCharStorage;
				currentLetter++;
				if (currentLetter!=totalMessageLength){
					nextCharStorage = message[currentLetter];
					message[currentLetter]='\0';
				}else{
					nextCharStorage=0;
				}
			}
		}

		StartDrawing();
		
		

		DrawMapThings();
		DrawRectangle(0,TEXTBOXY,SCREENWIDTH,SCREENHEIGHT-TEXTBOXY,255,255,255,255);

		// We need this variable so we know the offset in the message for the text that is for the next line
		int _lastStrlen=0;
		for (i=0;i<currentlyVisibleLines;i++){
			DrawText(5,TEXTBOXY+TextHeight(fontSize)*i,&message[_lastStrlen+offsetStrlen],fontSize);
			// This offset will have the first letter for the next line
			_lastStrlen = strlen(&message[_lastStrlen+offsetStrlen])+1+_lastStrlen;
		}

		// Draw questions
		if (isQuestion==1){
			DrawTextureScale(yesButtonTexture,SCREENWIDTH-GetTextureWidth(yesButtonTexture)*questionScale,TEXTBOXY-GetTextureHeight(yesButtonTexture)*questionScale-GetTextureHeight(yesButtonTexture)*questionScale,questionScale,questionScale);
			DrawTextureScale(noButtonTexture,SCREENWIDTH-GetTextureWidth(noButtonTexture)*questionScale,TEXTBOXY-GetTextureHeight(noButtonTexture)*questionScale,questionScale,questionScale);
			DrawAnimationAsISay(&selectorAnimation,SCREENWIDTH-GetTextureWidth(yesButtonTexture)*questionScale-questionScale*22-5,TEXTBOXY-(currentSelected)*(GetTextureHeight(yesButtonTexture)*questionScale)-((GetTextureHeight(yesButtonTexture)*questionScale)/2),questionScale);
		}
		
		if (portrait!=NULL){
			DrawTextureScale(portrait,0,TEXTBOXY-GetTextureHeight(portrait)*portScale,portScale,portScale);
		}

		EndDrawing();
		FpsCapWait();
		frames++;
	}
	printf("Done with textbox.\n");
	ControlsEnd();
	return currentSelected;
}

void Init(){
	// Good stuff
	spellLinkedListStart=calloc(1,sizeof(spellLinkedList));

	#if PLATFORM == PLAT_VITA
		// Init vita2d and set its clear color.
		vita2d_init();
		vita2d_set_clear_color(RGBA8(212, 208, 200, 0xFF));
		// We love default fonts.
		//defaultPgf = vita2d_load_default_pgf();
	#elif PLATFORM == PLAT_WINDOWS
		SDL_Init(SDL_INIT_VIDEO);
		mainWindow = SDL_CreateWindow( "HappyWindo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_SHOWN );
		ShowErrorIfNull(mainWindow);

		if (0==1){
			mainWindowRenderer = SDL_CreateRenderer( mainWindow, -1, SDL_RENDERER_ACCELERATED);
		}else{
			mainWindowRenderer = SDL_CreateRenderer( mainWindow, -1, SDL_RENDERER_PRESENTVSYNC);
		}
		ShowErrorIfNull(mainWindowRenderer);

		// This is the default background color.
		SDL_SetRenderDrawColor( mainWindowRenderer, 212, 208, 200, 255 );
		// Check if this fails?
		IMG_Init( IMG_INIT_PNG );

		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	#elif PLATFORM == PLAT_3DS
		sf2d_init();
		sf2d_set_clear_color(RGBA8(212, 208, 200, 0xFF));
	#endif

	LoadFont();

	#if PLATFORM == PLAT_VITA
		// ... I guess I have to do this? I'm... most probrablly not basing this off the sample.
		memset(&pad, 0, sizeof(pad));
	#endif

	InitGoodArray(&tileOtherData);
	int i=0;
	for (i=0;i<5;i++){
		InitGoodArray(&(layers[i]));
	}

	currentMapFilepath = malloc(1);

	// Init Lua
	L = luaL_newstate();
	luaL_openlibs(L);
	PushCFunctions();

	// Set platform lua variable
	lua_pushnumber(L,PLATFORM);
	lua_setglobal(L,"Platform");


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

	FixPath("Stuff/PlayerDown.png",tempPathFixBuffer,TYPE_EMBEDDED);
	playerDown=LoadPNG(tempPathFixBuffer);
	FixPath("Stuff/PlayerUp.png",tempPathFixBuffer,TYPE_EMBEDDED);
	playerUp=LoadPNG(tempPathFixBuffer);
	FixPath("Stuff/PlayerLeft.png",tempPathFixBuffer,TYPE_EMBEDDED);
	playerLeft=LoadPNG(tempPathFixBuffer);
	FixPath("Stuff/PlayerRight.png",tempPathFixBuffer,TYPE_EMBEDDED);
	playerRight=LoadPNG(tempPathFixBuffer);
	FixPath("Stuff/Selector.png",tempPathFixBuffer,TYPE_EMBEDDED);
	selectorAnimation.texture=LoadPNG(tempPathFixBuffer);

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

	#if PLATFORM == PLAT_VITA
		// Get touch
		sceTouchPeek(SCE_TOUCH_PORT_FRONT, &currentTouch, 1);
	
		// Magic line to fix touch. It's really magic, I promise.
		sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);
	#endif

	place=5;

	#if SHOWSPLASH==1
		#if PLATFORM == PLAT_VITA
			#if PLATFORM == PLAT_VITA
				CrossTexture* happy = LoadPNG("app0:OtherStuff/Splash.png");
			#elif PLATFORM == PLAT_WINDOWS
				CrossTexture* happy = LoadPNG("./OtherStuff/Splash.png");
			#endif

			StartDrawing();
			DrawTexture(happy,0,0);
			EndDrawing();
			
			StartFrameStuff();
			//sceCtrlPeekBufferPositive(0, &pad, 1);
			if (!(IsDown(SCE_CTRL_LTRIGGER))){
				Wait(700);
			}
		#endif
	#endif

	PlzNoCrashOnDispose();
	#if SHOWSPLASH==1
		#if PLATFORM == PLAT_VITA
			FreeTexture(happy);
		#endif
	#endif
}

int main(int argc, char *argv[]){
	srand(time(NULL));
	Init();	

	/*
		0 - Overworld
		1 - Pause in Overworld
		2 - Quit (breaks)
		3 - Battle
	*/
	
	//BattleInit();

	//TestNewMessage("The FitnessGram Pacer Test is a multistage aerobic capacity test that progressively gets more difficult as it continues. The 20 meter pacer test will begin in 30 seconds. Line up at the start. The running speed starts slowly, but gets faster each minute after you hear this signal. [beep] A single lap should be completed each time you hear this sound. [ding] Remember to run in a straight line, and run as long as possible. The second time you fail to complete a lap before the sound, your test is over. The test will begin on the word start. On your mark, get ready, start.");
	////TestNewMessage("This is a really long sentence that is also a run-on sentence that is going to go on, and on, and on because I need it to reach the END!!");
	//TestNewMessage("");
	//return;

	while (1){
		if (place==PLACE_OVERWORLD){
			Overworld();
		}else if (place==PLACE_MENU){
			MenuLop();
			ClearBottomScreen();
		}else if (place==PLACE_QUIT){
			break;
		}else if (place==PLACE_BATTLE){
			BattleLop(1);
			EndFrameStuff();
		}else if (place==PLACE_STATUS){
			StatusLoop();
		}else if (place==PLACE_TITLE){
			TitleLoop();
		}
	}
	

	// End this stuff
	#if PLATFORM == PLAT_VITA
		vita2d_fini();
		//vita2d_free_pgf(defaultPgf);
		// I really don't get why I need this when the user can just use the PS button.
		sceKernelExitProcess(0);
	#elif PLATFORM == PLAT_WINDOWS
		//Destroy window
		SDL_DestroyRenderer( mainWindowRenderer );
		SDL_DestroyWindow( mainWindow );
		mainWindow = NULL;
		mainWindowRenderer = NULL;
		// QUit SDL subsystems
		IMG_Quit();
		SDL_Quit();
		lua_close(L);
	#elif PLATFORM == PLAT_3DS
		// TODO 3ds cleanup.
		// This may actually be used.
		sf2d_fini();
	#endif
	return 0;
}