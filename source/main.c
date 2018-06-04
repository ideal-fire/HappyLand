/*
---O---OO--OOOOO-O-OO-OOOOO----O---O
--O-O-O-O--O---O-OO---O---O---O-O-O-
-O--OO--O--O---O-O----O---O--O--OO--
O---O---O--OOOOO-O----OOOOO-O---O---
TODO - Put a scale factor in the animation struct. That scale factor will be changed depending on size of sprite. Normally, it will be the same as ENTITYSCALE. But, in cases like BIg Foot in battle, it will be less than ENTITYSCALE. That is so I can have higher resolution images.
TODO - Sometimes, enemy attack animations dissapear.
	Jon said that the slime ones dissapeared after fighting angry tree
	Snowmen ones dissapeared after angry tree also
	Noob Big Foot had animatioin
TODO - Slight rumble when you walk into wall
TODO - Why lag?
	The animations are slow, so I'm probably not getting milliseconds correctly.
*/

#define BGMENABLE 1

#include "GeneralGoodConfig.h"

// Only turned off when I'm working on my game. Needs to be set to 1 before release
#define SHOWSPLASH 1

// Temp
#define StartFrameStuff(); FpsCapStart(); \
	controlsStart();
#define EndFrameStuff(); controlsEnd(); \
	FpsCapWait();

//#define LANGUAGE LANG_SPANISH
#define LANG_ENGLISH 1
#define LANG_SPANISH 2
int LANGUAGE=LANG_ENGLISH;

#define GOODGRAY 212, 208, 200

#define TYPE_UNDEFINED 0
#define TYPE_DATA 1
#define TYPE_EMBEDDED 2

#define QUEUE_X 1
#define QUEUE_O 6
#define QUEUE_LEFT 2
#define QUEUE_RIGHT 3
#define QUEUE_DOWN 4
#define QUEUE_UP 5
//#define ??? 7

// Change the place variable to one of these
#define PLACE_OVERWORLD 0
#define PLACE_MENU 1
#define PLACE_QUIT 2
#define PLACE_BATTLE 3
#define PLACE_STATUS 4
#define PLACE_TITLE 5

#define SPELLSPECIAL_NONE 0
#define SPELLSPECIAL_NODEFENCE 1

#define TYPE_SLOT 1
#define TYPE_ID 2

#define ANIMATION_IDLE 1
#define ANIMATION_ATTACK 2

#define BATTLESTATUS_NEXTTURN 0
#define BATTLESTATUS_CHOOSINGMOVE 1
#define BATTLESTATUS_CHOOSINGTARGET 2
#define BATTLESTATUS_MOVETOTARGET 3
#define BATTLESTATUS_MOVEBACKFROMTARGET 4
#define BATTLESTATUS_SPELLANIMATION 5

#define STARTINGMAP "Stuff/Maps/NathansHouse"

#define USEVSYNC 0

// Translatos for hardcoded strings
#include "HardcodedLanguage.h"

#define MAXOBJECTS 15

#define ANDROIDTEST 1

#define TOUCHENABLED 0

#if PLATFORM==PLAT_VITA
	#define SAFEDRAW 1
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

	#define YESNOSCALE 2
	#define SELECTIONBUTTONSCALE 2
#elif PLATFORM==PLAT_COMPUTER
	#define SAFEDRAW 0
	#define SCREENWIDTH 704
	#define SCREENHEIGHT 512
	#define MAPXSCALE 2
	#define MAPYSCALE 2
	#define BATTLEENTITYSCALE 2.5
	#define SPELLSCALE 3
	#define MINYBATTLE 106
	#define MAXYBATTLE 300
	#define TEXTBOXY 300
	#define DEFAULTPORTRAITSIZE 200
	#define UISCALE 3
	#define DAMAGETEXTSIZE fontSize
	#define YESNOSCALE 3
	#define SELECTIONBUTTONSCALE 1.7
#elif PLATFORM==PLAT_3DS
	#define SAFEDRAW 1
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

	#define TEXTBOXFONTSIZE 1.5
	// SCREENHEIGHT - (Font*8)*desiredlines-5-5*desiredlines
	#define TEXTBOXY 172
	
	#define LINESPERSCREEN 3

	#define DEFAULTPORTRAITSIZE 100
	#define YESNOSCALE 1
	#define DAMAGETEXTSIZE 2
#elif PLATFORM==PLAT_SWITCH
	#define SCE_TOUCH KEY_TOUCH

	#define SAFEDRAW 1
	#define SCREENWIDTH 1280
	#define SCREENHEIGHT 720
	#define MAPXSCALE 3
	#define MAPYSCALE 3
	#define BATTLEENTITYSCALE 3
	#define SPELLSCALE 4
	// Minimum y position for an entity to be at during a battle.
	// This is so they don't overlap the healthbar
	#define MINYBATTLE 106
	// This is so they don't overlap the selection buttons. Put as the screen height for no restriction
	#define MAXYBATTLE 416
	
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

	#define YESNOSCALE 2
	#define SELECTIONBUTTONSCALE 2

	#define UISCALE 3
#endif

// CAHNGED BY SCALE
int drawXOffset=0;
int drawYOffset=0;
// UNCHANGED BY SCALE, DON'T USE FOR ANYTHING BUT TOUCH
int realDrawXOffset=0;
int realDrawYOffset=0;
// Include cool libraries and stuff
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

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
	typedef uint8_t 	u8;
	typedef uint16_t 	u16;
	typedef uint32_t	u32;
	typedef uint64_t	u64;
	typedef int8_t		s8;
	typedef int16_t		s16;
	typedef int32_t		s32;
	typedef int64_t		s64;
#elif PLATFORM == PLAT_COMPUTER
	#include <time.h>

	#include <dirent.h>
#elif PLATFORM == PLAT_3DS
	#include <3ds.h>
	#include <stdio.h>
	#include <sf2d.h>
	#include <sfil.h>
	#include <3ds/svc.h>
#endif

#define unusedAreaColor 0,0,0,255
//unsigned int unusedAreaColor = RGBA8(0,0,0,255);

char tempPathFixBuffer[256];

#include "GeneralGoodExtended.h"
#include "GeneralGood.h"
#include "GeneralGoodGraphics.h"
#include "GeneralGoodText.h"
#include "GeneralGoodImages.h"
#include "GeneralGoodSound.h"
#include "GeneralGood.h"
#include "GoodArray.h"
#include "FpsCapper.h"
#include "pathfinding.h"
#if PLATFORM == PLAT_COMPUTER
	#include "SDLLua_GeneralGood.h"
#endif
//TODO - Make this
// main.h
	// Make stuff fresh
	void BasicMessage(char* message);
	void fixPath(char* filename,char _buffer[], char type);
	char ShowMessageWithPortrait(char* _tempMsg, char isQuestion, CrossTexture* portrait, double portScale);

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
		short partyMemberScriptID; // The function called AddPartyMember x ( NO SPACE BETWEEN THEM ) with X being partyMemberScriptID is called when this party member is loaded.
	}partyMember;
	typedef struct Q_spell{
		char* name;
		signed char attack;
		signed char magicAttack;
		char* spellPath;
		animation theAnimation;
		char mpCost;
		unsigned char spellSpecialProperty;
	}spell;
	typedef struct Q_spellLinkedList{
		struct Q_spellLinkedList* nextEntry;
		spell theSpell;
	}spellLinkedList;

/*
/////////////////////////////////////////////////
== STUFF
/////////////////////////////////////////////////
*/
lua_State* L;
//CrossTexture* fontImage;
animation selectorAnimation;
// Where you are. Like Overworld, Menu, Battle, etc.
// Values in main function
unsigned char place=PLACE_OVERWORLD;

spellLinkedList* spellLinkedListStart={0};
unsigned short spellLinkedListSize=1;

char* currentMapFilepath;

int currentTextHeight=0;

/*
////////////////////////////////////////////////////
== OPTIONS
///////////////////////////////////////////////////
*/
// Max I think I'll make 8 chars.
char* playerName="Nathan";
u64 aButton=SCE_CTRL_CROSS;
u64 bButton=SCE_CTRL_CIRCLE;
char textboxNewCharSpeed=1;
char defaultSelected=1;

/*
//////////////////////////////////////////////////
== WORLD MAP
//////////////////////////////////////////////////
*/
CrossTexture* tilesets[5];
object mapObjects[MAXOBJECTS];
Good2dArray layers[5]={NULL,0,0,0,0};
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
// This is an array of coordinates for pathfinding
coordinates* pathfindingPath;
short foundPathLength=-1;
short walkingPathProgress=0;
float androidXScale;
float androidYScale;
CROSSMUSIC* currentBGM=NULL;
CROSSSFX* selectSoundEffect=NULL;
CROSSSFX* softSelectSoundEffect=NULL;
CrossTexture* pauseButtonTexture;
char isMapLoaded=0;
/*
/////////////////////////////////////////////////
== BATTLE
/////////////////////////////////////////////////
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
////////////////////////////////////////////////
// 
///////////////////////////////////////////////
*/

void clearDebugFile(){
	FILE* fp = fopen("/a.txt","w");
	fclose(fp);
}
void writeToDebugFile(const char* _messageToWrite){
	FILE* fp = fopen("/a.txt","a");
	fprintf(fp,"%s\n",_messageToWrite);
	fclose(fp);
}

// Same as fixing a path and then loading a PNG
CrossTexture* LoadEmbeddedPNG(char* filename){
	fixPath(filename,tempPathFixBuffer,TYPE_EMBEDDED);
	return loadPNG(tempPathFixBuffer);
}

void LoadFont(){
	#if TEXTRENDERER == TEXT_DEBUG
		fontImage=LoadEmbeddedPNG("Stuff/Font.png");
	#elif TEXTRENDERER == TEXT_FONTCACHE
		//fontSize = (SCREENHEIGHT-TEXTBOXY)/3.5;
		FC_FreeFont(fontImage);
		fontImage = NULL;
		fontImage = FC_CreateFont();
		fixPath("Stuff/LiberationSans-Regular.ttf",tempPathFixBuffer,TYPE_EMBEDDED);
		FC_LoadFont(fontImage, mainWindowRenderer, tempPathFixBuffer, fontSize, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);
	#elif TEXTRENDERER == TEXT_VITA2D
		// TODO - Change this
		fixPath("Stuff/LiberationSans-Regular.ttf",tempPathFixBuffer,TYPE_EMBEDDED);
		fontImage=loadPNG(tempPathFixBuffer);
	#endif
	currentTextHeight = textHeight(fontSize);
}

void RestorePartyMember(int id){
	int passedSlot = id;
	party[passedSlot].hp=party[passedSlot].fighterStats.maxHp;
	party[passedSlot].mp=party[passedSlot].fighterStats.maxMp;
}

int FixX(int x){
	return x+drawXOffset;
}
int FixY(int y){
	return y+drawYOffset;
}
int NegativeFixX(int x){
	#if DOFIXCOORDS == 1
		return x-drawXOffset;
	#else
		return x;
	#endif
}
int NegativeFixY(int y){
	#if DOFIXCOORDS == 1
		return y-drawYOffset;
	#else
		return y;
	#endif
}
// Fix x that needs to be applied on ALL drawing ... (that's done on the map?)
int FixXMap(int x){
	return x*MAPXSCALE;
}
int FixYMap(int x){
	return x*MAPYSCALE;
}
// Make objects appear where they should when the screen scrolls.
int FixXObjects(int x){
	return FixXMap(x-cameraWholeOffsetX*32-cameraPartialOffsetX);
}
int FixYObjects(int x){
	return FixYMap(x-cameraWholeOffsetY*32-cameraPartialOffsetY);
}
// Give real device touch coords and get logical resolution touch cords
int FixTouchY(int touch){
	return floor((touch-realDrawYOffset)/androidYScale);
}
int FixTouchX(int touch){
	return floor((touch-realDrawXOffset)/androidXScale);
}

tileImageData* GetMapImageData(short x, short y, short layer){
	return (tileImageData*)GetGoodArray(&(layers[layer]),x,y);
}

tileSpotData* GetMapSpotData(short x, short y){
	return (tileSpotData*)GetGoodArray(&(tileOtherData),x,y);
}

void DrawGrayBackground(){
	drawRectangle(0,0,SCREENWIDTH,SCREENHEIGHT,GOODGRAY,255);
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
	if (xAmount>tileOtherData.width){
		xAmount = tileOtherData.width;
	}
	if (yAmount>tileOtherData.height){
		yAmount = tileOtherData.height;
	}

	int x,y,i;
	for (i=0;i<numberOfLayers;i++){
		for (y=0;y<yAmount;y++){
			for (x=0;x<xAmount;x++){
				if (GetMapImageData(x+cameraWholeOffsetX,y+cameraWholeOffsetY,i)->tile!=0){
					drawTexturePartScale(tilesets[(int)GetMapImageData(x+cameraWholeOffsetX,y+cameraWholeOffsetY,i)->tileset],FixXMap(x*32-cameraPartialOffsetX),FixYMap(y*32-cameraPartialOffsetY),((GetMapImageData(x+cameraWholeOffsetX,y+cameraWholeOffsetY,i)->tile-1)*32),0,32,32,MAPXSCALE,MAPYSCALE);
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

		if (animationToDraw->currentFrame==animationToDraw->numberOfFrames-1){
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
	drawTexturePartScale(animationToDraw->texture,FixXObjects(destX+animationToDraw->drawXOffset),FixYObjects(destY+animationToDraw->drawYOffset),animationToDraw->width*animationToDraw->currentFrame,0,animationToDraw->width,animationToDraw->height,MAPXSCALE,MAPYSCALE);
}
// Draw animation. Unchanged by map position
void DrawAnimation(animation* animationToDraw, int destX, int destY){
	UpdateAnimationIfNeed(animationToDraw);
	drawTexturePartScale(animationToDraw->texture,FixXMap(destX+animationToDraw->drawXOffset),FixYMap(destY+animationToDraw->drawYOffset),animationToDraw->width*animationToDraw->currentFrame,0,animationToDraw->width,animationToDraw->height,MAPXSCALE,MAPYSCALE);
}
// Draws it as given in arguments. No scaling or position changes.
void DrawAnimationAsISay(animation* animationToDraw, int destX, int destY, float scale){
	UpdateAnimationIfNeed(animationToDraw);
	drawTexturePartScale(animationToDraw->texture,(destX+animationToDraw->drawXOffset),(destY+animationToDraw->drawYOffset),animationToDraw->width*animationToDraw->currentFrame,0,animationToDraw->width,animationToDraw->height,scale,scale);
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
	drawRectangle(NegativeFixX(0),NegativeFixY(0),SCREENWIDTH,drawYOffset,unusedAreaColor);
	// Left
	drawRectangle(NegativeFixX(0),NegativeFixY(0),drawXOffset,SCREENHEIGHT,unusedAreaColor);
	#if SAFEDRAW == 1
		// Right
		drawRectangle(NegativeFixX((cameraWidth)*(32*MAPXSCALE)+drawXOffset),NegativeFixY(0),drawXOffset,SCREENHEIGHT,unusedAreaColor);
		// Bottom
		drawRectangle(NegativeFixX(0),NegativeFixY(SCREENHEIGHT-drawYOffset),SCREENWIDTH,drawYOffset,unusedAreaColor);
	#else
		// Right
		drawRectangle(NegativeFixX((cameraWidth)*(32*MAPXSCALE)+drawXOffset),NegativeFixY(0),32*MAPXSCALE,SCREENHEIGHT,unusedAreaColor);
		// Bottom
		drawRectangle(NegativeFixX(0),NegativeFixY((cameraHeight)*(32*MAPYSCALE)+drawYOffset),SCREENWIDTH,32*MAPYSCALE,unusedAreaColor);
	#endif
}

char FitToWidth(int _imgWidth, int _imgHeight){
	double _tempWidthResult = (double)_imgWidth/SCREENWIDTH; // .8
	double _tempHeightResult = (double)_imgHeight/SCREENHEIGHT; // .92
	if (_tempWidthResult>_tempHeightResult){
		return 0;
	}
	return 1;
}

void PlayMenuSoundEffect(){
	playSound(selectSoundEffect,1,0);
}
void PlaySoftMenuSoundEffect(){
	playSound(softSelectSoundEffect,1,0);
}

char CheckCollision(int x, int y){
	if (x<0 || y<0 || y>=tileOtherData.height || x>=tileOtherData.width){
		return 1;
	}
	return GetMapSpotData(x,y)->isSolid;
}

char IsFree(short _x, short _y){
	if (CheckCollision(_x,_y)==0){
		return 1;
	}
	return 0;
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

char _luaAlreadyInit=0;
void RunHappyLua(){
	if (_luaAlreadyInit==0){
		lua_pushnumber(L,LANGUAGE);
		lua_setglobal(L,"Lang");
		// Need to load here as Lang variable has been set.
		fixPath("Stuff/Happy.lua",tempPathFixBuffer,TYPE_EMBEDDED);
		luaL_dofile(L,tempPathFixBuffer);
		_luaAlreadyInit=1;
	}
}

// Draws everything we draw on map.
void DrawMapThings(){
	DrawMap();
	DrawMapObjects();
	DrawUnusedAreaRect();
}

// This is the same as ShowMessage, but all the text is shown instantly and no map or objects are drawn
void BasicMessage(char* _tempMsg){
	controlsReset();
	// The string needs to be copied. We're going to modify it, at we can't if we just type the string into the function and let the compiler do everything else
	char message[strlen(_tempMsg)+1];
	strcpy(message,_tempMsg);
	int textboxLinesPerScreens = (SCREENHEIGHT-TEXTBOXY)/textHeight(fontSize);
	short newLineLocations[50];
	int totalMessageLength = strlen(message);
	int i, j;

	// This will loop through the entire message, looking for where I need to add new lines. When it finds a spot that
	// needs a new line, that spot in the message will become 0. So, when looking for the place to 
	int lastNewlinePosition=-1;
	for (i = 0; i < totalMessageLength; i++){
		if (message[i]==32){ // Only check when we meet a space. 32 is a space in ASCII
			message[i]='\0';
			//printf("Space found at %d\n",i);
			//printf("%s\n",&message[lastNewlinePosition+1]);
			//printf("%d\n",textWidth(fontSize,&(message[lastNewlinePosition+1])));
			if (textWidth(fontSize,&(message[lastNewlinePosition+1]))>SCREENWIDTH-20){
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
	// This code will make a new line if there needs to be one because of the last word
	if (textWidth(fontSize,&(message[lastNewlinePosition+1]))>SCREENWIDTH-20){
		for (j=totalMessageLength-1;j>lastNewlinePosition+1;j--){
			if (message[j]==32){
				message[j]='\0';
				break;
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
		controlsStart();
		if (wasJustPressed(SCE_CTRL_CROSS) || wasJustPressed(SCE_TOUCH)){
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
		controlsEnd();
		
		startDrawing();
		DrawGrayBackground();
		// We need this variable so we know the offset in the message for the text that is for the next line
		int _lastStrlen=0;
		for (i=0;i<currentlyVisibleLines;i++){
			goodDrawText(5,TEXTBOXY+textHeight(fontSize)*i,&message[_lastStrlen+offsetStrlen],fontSize);
			// This offset will have the first letter for the next line
			_lastStrlen = strlen(&message[_lastStrlen+offsetStrlen])+1+_lastStrlen;
		}

		endDrawing();
		FpsCapWait();
		frames++;
	}
	controlsEnd();
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

// TODO - If you have a question that takes multiple textbox screens to show, you can't see any of them except the first one.
char ShowMessageWithPortrait(char* _tempMsg, char isQuestion, CrossTexture* portrait, double portScale){
	controlsReset();

	CrossTexture* yesButtonTexture=(CrossTexture*)1;
	CrossTexture* noButtonTexture=(CrossTexture*)1;
	//char YESNOSCALE=1;
	// The string needs to be copied. We're going to modify it, at we can't if we just type the string into the function and let the compiler do everything else
	char message[strlen(_tempMsg)+1];
	strcpy(message,_tempMsg);
	signed char currentSelected=defaultSelected;
	int textboxLinesPerScreens = (SCREENHEIGHT-TEXTBOXY)/textHeight(fontSize);
	short newLineLocations[50];
	int totalMessageLength = strlen(message);
	int i, j;
	
	if (isQuestion==1){
		yesButtonTexture=LoadEmbeddedPNG("Stuff/Yes.png");
		noButtonTexture=LoadEmbeddedPNG("Stuff/No.png");
		
		//if (SCREENWIDTH/getTextureWidth(yesButtonTexture)>=8){
		//	YESNOSCALE=2;
		//}else{
		//	YESNOSCALE=1;
		//}
	}
	if (portScale==0 && portrait!=NULL){
		portScale = floor((double)DEFAULTPORTRAITSIZE/getTextureWidth(portrait));
	}

	// This will loop through the entire message, looking for where I need to add new lines. When it finds a spot that
	// needs a new line, that spot in the message will become 0. So, when looking for the place to 
	int lastNewlinePosition=-1; // If this doesn't start at -1, the first character will be cut off
	for (i = 0; i < totalMessageLength; i++){
		if (message[i]==32){ // Only check when we meet a space. 32 is a space in ASCII
			message[i]='\0';
			//printf("Space found at %d\n",i);
			//printf("%s\n",&message[lastNewlinePosition+1]);
			//printf("%d\n",textWidth(fontSize,&(message[lastNewlinePosition+1])));
			if (textWidth(fontSize,&(message[lastNewlinePosition+1]))>SCREENWIDTH-20){
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
	// This code will make a new line if there needs to be one because of the last word
	if (textWidth(fontSize,&(message[lastNewlinePosition+1]))>SCREENWIDTH-20){
		for (j=totalMessageLength-1;j>lastNewlinePosition+1;j--){
			if (message[j]==32){
				message[j]='\0';
				break;
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

	controlsEnd();
	while (1){
		FpsCapStart();
		controlsStart();
		if (isQuestion==1){
			if (wasJustPressed(SCE_CTRL_DOWN)){
				currentSelected++;
				if (currentSelected>1){
					currentSelected=0;
				}
			}else if (wasJustPressed(SCE_CTRL_UP)){
				if (currentSelected==0){
					currentSelected=1;
				}else{
					currentSelected--;
				}
			}else
			#if TOUCHENABLED
			if (wasJustPressed(SCE_TOUCH)){
				if (FixTouchX(touchX)>=(SCREENWIDTH-getTextureWidth(yesButtonTexture)*YESNOSCALE) && FixTouchX(touchX)<SCREENWIDTH){
					if (FixTouchY(touchY)>(TEXTBOXY-getTextureHeight(yesButtonTexture)*YESNOSCALE*2)){
						if (FixTouchY(touchY)<(TEXTBOXY-getTextureHeight(yesButtonTexture)*YESNOSCALE)){ // Pressed yes
							currentSelected = 1;
							break;
						}else{ // pressed no if higher than TEXTBOXY
							if (FixTouchY(touchY)<TEXTBOXY){
								currentSelected = 0;
								break;
							}
						}
					}
				}
			}
			#else
			{

			}
			#endif
		}
		if (wasJustPressed(SCE_CTRL_CROSS) || (wasJustPressed(SCE_TOUCH) && isQuestion==0) ){
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
		controlsEnd();
		
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

		startDrawing();
		
		DrawMapThings();
		drawRectangle(0,TEXTBOXY,SCREENWIDTH,SCREENHEIGHT-TEXTBOXY,255,255,255,255);

		// We need this variable so we know the offset in the message for the text that is for the next line
		int _lastStrlen=0;
		for (i=0;i<currentlyVisibleLines;i++){
			goodDrawText(5,TEXTBOXY+textHeight(fontSize)*i,&message[_lastStrlen+offsetStrlen],fontSize);
			// This offset will have the first letter for the next line
			_lastStrlen = strlen(&message[_lastStrlen+offsetStrlen])+1+_lastStrlen;
		}

		// Draw questions
		if (isQuestion==1){
			drawTextureScale(yesButtonTexture,SCREENWIDTH-getTextureWidth(yesButtonTexture)*YESNOSCALE,TEXTBOXY-getTextureHeight(yesButtonTexture)*YESNOSCALE-getTextureHeight(yesButtonTexture)*YESNOSCALE,YESNOSCALE,YESNOSCALE);
			drawTextureScale(noButtonTexture,SCREENWIDTH-getTextureWidth(noButtonTexture)*YESNOSCALE,TEXTBOXY-getTextureHeight(noButtonTexture)*YESNOSCALE,YESNOSCALE,YESNOSCALE);
			#if PLATFORM != PLAT_COMPUTER
				DrawAnimationAsISay(&selectorAnimation,SCREENWIDTH-getTextureWidth(yesButtonTexture)*YESNOSCALE-YESNOSCALE*22-5,TEXTBOXY-(currentSelected)*(getTextureHeight(yesButtonTexture)*YESNOSCALE)-((getTextureHeight(yesButtonTexture)*YESNOSCALE)/2),YESNOSCALE);
			#endif
		}
		
		if (portrait!=NULL){
			drawTextureScale(portrait,0,TEXTBOXY-getTextureHeight(portrait)*portScale,portScale,portScale);
		}

		endDrawing();
		FpsCapWait();
		frames++;
	}
	controlsEnd();
	return currentSelected;
}

void LoadMap(char* path){
	CROSSFILE* openedFile;
	openedFile = crossfopen(path,"r");
	unsigned short _width;
	unsigned short _height;

	// Read width and height
	crossfread(&_width,2,1,openedFile);
	crossfread(&_height,2,1,openedFile);
	// Set array to the size it needs
	SetGoodArray(&(tileOtherData),_width,_height,sizeof(tileOtherData));
	int x,y,i;

	for (y=0;y<_height;y++){
		for (x=0;x<_width;x++){
			// Read isSolid and then event
			crossfread(&(GetMapSpotData(x,y)->isSolid),1,1,openedFile);
			crossfread(&(GetMapSpotData(x,y)->event),1,1,openedFile);
		}
	}
	// Read number of loayers
	crossfread(&numberOfLayers,1,1,openedFile);
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
				crossfread(&(GetMapImageData(x,y,i)->tile),1,1,openedFile);
				crossfread(&(GetMapImageData(x,y,i)->tileset),1,1,openedFile);
			}
		}
	}
	crossfclose (openedFile);

	char withDotLua[strlen(path)+strlen(".lua")+1];
	sprintf(withDotLua,"%s%s",path,".lua");
	luaL_dofile(L,withDotLua);
	// The reason we need this temp buffer stupidity is because I tried to pass currentMapFilepath to this function, which makes this function strcpy with dest and src the same
	char _tempBuf[strlen(path)+1];
	strcpy(_tempBuf,path);
	free(currentMapFilepath);
	currentMapFilepath = malloc(strlen(_tempBuf)+1);
	strcpy(currentMapFilepath,_tempBuf);

	//((tileImageData*)GetGoodArray(&layers[0],1,1))->tile=1;
	//
	legfinderMapWidth = tileOtherData.width;
	legfinderMapHeight = tileOtherData.height;
	LegfinderFixList();
	isMapLoaded=1;
}

// Give it a special id. Returns -1 if not found
int PartyMemberIDToSlot(int _tempId){
	int i;
	for (i=0;i<partySize;i++){
		if (party[i].partyMemberScriptID==_tempId){
			return i;
		}
	}
	printf("Party member with special id %d not in party right now.\n",_tempId);
	return -1;
}

void UnloadMap(){
	// No map, don't free to prevent errors
	if (isMapLoaded==0){
		return;
	}
	lua_getglobal(L,"MapDispose");
	lua_call (L,0,0);
	SetGoodArray(&(tileOtherData),1,1,1);
	int i;
	for (i=0;i<5;i++){
		SetGoodArray(&(layers[i]),1,1,1);
	}

	//// Free enemy animation
	for (i=0;i<10;i++){
		if (possibleEnemies[i].maxHp!=-1){
			possibleEnemies[i].maxHp=-1;
			if (possibleEnemiesAttackAnimation[i].texture!=NULL){
				freeTexture(possibleEnemiesAttackAnimation[i].texture);
				possibleEnemiesAttackAnimation[i].texture=NULL;
			}
			if (possibleEnemiesIdleAnimation[i].texture!=NULL){
				freeTexture(possibleEnemiesIdleAnimation[i].texture);
				possibleEnemiesIdleAnimation[i].texture=NULL;
			}
		}
		possibleEnemies[i].maxHp=-1;
	}
	
	// Set all enemy pointers to null
	for (i=0;i<4;i++){
		enemies[i].hp=-1;
		enemyIdleAnimation[i]=NULL;
		enemyAttackAnimation[i]=NULL;
	}
	isMapLoaded=0;
}

void ChangeMap(char* newMap){
	encounterRate=0;
	UnloadMap();
	LoadMap(newMap);
}

void BattleAttackTemplate(animation* animationToModify, char* filePath, char width, int speed){
	animationToModify->speed=speed;
	animationToModify->texture=loadPNG(filePath);
	animationToModify->width=width;
	/*39*/animationToModify->height=getTextureHeight(animationToModify->texture);
	animationToModify->numberOfFrames=getTextureWidth(animationToModify->texture)/animationToModify->width;
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
// animationType - 
// ANIMATION_IDLE
// ANIMATION_ATTACK
animation* GetBattlerAnimationById(char id, char animationType){
	if (id<=4 && id>=1){
		if (animationType==ANIMATION_ATTACK){
			return &(partyAttackAnimation[(int)(id-1)]);
		}else if (animationType==ANIMATION_IDLE){
			return &(partyIdleAnimation[(int)(id-1)]);
		}
	}else if (id>=5 && id<=8){
		if (animationType==ANIMATION_ATTACK){
			return (enemyAttackAnimation[(int)(id-5)]);
		}else if (animationType==ANIMATION_IDLE || animationType == 0){
			return (enemyIdleAnimation[(int)(id-5)]);
		}
	}
	return &partyIdleAnimation[0];
}

int CalculateDamage(int moveAttack, int moveMagicAttack, int fighterAttack, int fighterMagicAttack, int victimDefence, int victimMagicDefence){
	return floor(((double)fighterAttack/victimDefence)*moveAttack+((double)fighterMagicAttack/victimMagicDefence)*moveMagicAttack);
}

int Damage(partyMember* attacker, partyMember* victim, int moveAttack, int moveMagicAttack, int spellSpecialProperty){
	int doneDamage;
	if (spellSpecialProperty==SPELLSPECIAL_NODEFENCE){
		doneDamage=moveAttack+moveMagicAttack;
	}else{
		doneDamage = CalculateDamage(moveAttack,moveMagicAttack,attacker->fighterStats.attack,attacker->fighterStats.magicAttack,victim->fighterStats.defence,victim->fighterStats.magicDefence);
	}
	victim->hp-=doneDamage;
	if (victim->hp<0){
		victim->hp=0;
	}
	// Limit overhealing
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
	return CenterSomething(textWidth(scale,text));
}

// Returns x position for this to be centered
int CenterTextCustomWidth(char* text, char scale, int customScreenWidth){
	return CenterSomethingCustomWidth(textWidth(scale,text),customScreenWidth);
}

// Draws the damage text.
// arguments are self explanitory
void DrawDamageText(int battlerId, int damage, char _passedString[]){
	if (GetBattlerById(battlerId)->y-currentTextHeight-drawYOffset>=0){
		if (damage<0){
			goodDrawTextColored(GetBattlerById(battlerId)->x-textWidth(fontSize,_passedString)/2,GetBattlerById(battlerId)->y-currentTextHeight,_passedString,DAMAGETEXTSIZE,0,255,0);
		}else{
			goodDrawTextColored(GetBattlerById(battlerId)->x-textWidth(fontSize,_passedString)/2,GetBattlerById(battlerId)->y-currentTextHeight,_passedString,DAMAGETEXTSIZE,255,0,0);
		}
	}else{
		if (damage<0){
			goodDrawTextColored(GetBattlerById(battlerId)->x-textWidth(fontSize,_passedString)/2,GetBattlerById(battlerId)->y+GetBattlerAnimationById(battlerId,ANIMATION_IDLE)->height+currentTextHeight,_passedString,DAMAGETEXTSIZE,0,255,0);
		}else{
			goodDrawTextColored(GetBattlerById(battlerId)->x-textWidth(fontSize,_passedString)/2,GetBattlerById(battlerId)->y+GetBattlerAnimationById(battlerId,ANIMATION_IDLE)->height+currentTextHeight,_passedString,DAMAGETEXTSIZE,255,0,0);
		}
	}
}

// Returns spell id selected
signed char SelectSpell(partyMember member){
	unsigned char selected=0;
	char tempMessage[255];
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

		char _actionQueue;
		#if TOUCHENABLED
			if (wasJustPressed(SCE_TOUCH)){
				if (FixTouchY(touchY)>currentTextHeight){
					//goodDrawText(88,(i+1)*currentTextHeight+currentTextHeight/2,tempMessage,6);
					//drawRectangle(88,(i+1)*currentTextHeight+(currentTextHeight/4)*i,textWidth(fontSize,tempMessage)<SCREENWIDTH-100 ? (SCREENWIDTH-100) : textWidth(fontSize,tempMessage),currentTextHeight+(currentTextHeight/4),0,162,0,255);
					int _oldSelected=selected;
					selected=(FixTouchY(touchY)-currentTextHeight)/(currentTextHeight+(currentTextHeight/4));
					if (selected>200){
						selected=0;
					}else if (selected>=numberOfSkillsSelect){
						_actionQueue=QUEUE_O;
					}else if (_oldSelected==selected){
						_actionQueue=QUEUE_X;
					}else{
						PlaySoftMenuSoundEffect();
					}
				}
			}
		#endif
		if (wasJustPressed(aButton)){
			PlayMenuSoundEffect();
			_actionQueue = QUEUE_X;
		}else if (wasJustPressed(bButton)){
			_actionQueue = QUEUE_O;
		}

		if (wasJustPressed(SCE_CTRL_DOWN)){
			selected++;

			if (selected==numberOfSkillsSelect){
				selected=0;
			}
		}else if (wasJustPressed(SCE_CTRL_UP)){
			selected--;
			// Unsigned char go from -1 to 255
			if (selected>100){
				selected=numberOfSkillsSelect-1;
			}
		}else if (_actionQueue==QUEUE_X){
			if (member.fighterStats.spells[selected]!=0){
				if (GetSpellList(member.fighterStats.spells[selected]-1)->theSpell.mpCost>member.mp){
					ShowMessage(N_NEEDMOREMP,0);
					_actionQueue = 0;
					controlsReset();
				}else{
					return member.fighterStats.spells[selected]-1;
				}
			}
		}else if (_actionQueue==QUEUE_O){
			return -1;
		}
		
		startDrawing();
		DrawGrayBackground();

		#if PLATFORM != PLAT_3DS

			goodDrawText( CenterText(N_MAGICLIST,6),2,N_MAGICLIST,6);
	
			for (i=0;i<numberOfSkillsSelect;i++){
				if (member.fighterStats.spells[i]!=0){
					spellLinkedList* tempList = GetSpellList(member.fighterStats.spells[i]-1);
					sprintf((char*)tempMessage,"[%s:%d]",tempList->theSpell.name,tempList->theSpell.mpCost);
					if (member.mp>=tempList->theSpell.mpCost){
						drawRectangle(88,(i+1)*currentTextHeight+(currentTextHeight/4)*i,textWidth(fontSize,tempMessage)<SCREENWIDTH-100 ? (SCREENWIDTH-100) : textWidth(fontSize,tempMessage),currentTextHeight+(currentTextHeight/4),0,162,0,255);
					}else{
						drawRectangle(88,(i+1)*currentTextHeight+(currentTextHeight/4)*i,textWidth(fontSize,tempMessage)<SCREENWIDTH-100 ? (SCREENWIDTH-100) : textWidth(fontSize,tempMessage),currentTextHeight+(currentTextHeight/4),162,0,0,255);
					}
					
					goodDrawText(88,(i+1)*currentTextHeight+(currentTextHeight/4)*i,tempMessage,6);
				}else{
					break;
				}
			}
			DrawAnimationAsISay(&selectorAnimation,0,(selected+1)*currentTextHeight+(currentTextHeight/4)*selected,4);
			#if PLATFORM == PLAT_COMPUTER
				goodDrawText(88,SCREENHEIGHT-currentTextHeight,"Back",6);
			#endif
		#else
			goodDrawText( CenterText(N_MAGICLIST,2),2,N_MAGICLIST,2);
	
			for (i=0;i<numberOfSkillsSelect;i++){
				if (member.fighterStats.spells[i]!=0){
					//ShowMessage("a",0);
					spellLinkedList* tempList = GetSpellList(member.fighterStats.spells[i]-1);
					sprintf((char*)tempMessage,"%s:%d",tempList->theSpell.name,tempList->theSpell.mpCost);
					goodDrawText(88,i*16+i*2+20,tempMessage,2);
				}else{
					break;
				}
			}
			DrawAnimationAsISay(&selectorAnimation,0,selected*16+selected*2+20-10,2);
		#endif

		endDrawing();

		EndFrameStuff();
	}
}

void DisposeAllLoadedSpellImages(){
	int i=0;
	spellLinkedList* gotList;
	for (i=0;i<spellLinkedListSize;i++){
		gotList = GetSpellList(i);
		if (gotList->theSpell.theAnimation.texture!=NULL){
			freeTexture(gotList->theSpell.theAnimation.texture);
			gotList->theSpell.theAnimation.texture=NULL;
		}
	}
}

void LoadSpellImageIfNeeded(int spellId){
	spell* gotSpell = &(GetSpellList(spellId)->theSpell);
	if (gotSpell->theAnimation.texture==NULL){
		gotSpell->theAnimation.texture = loadPNG(gotSpell->spellPath);
	}
}

void AutodetectNumberOfFrames(animation* passedAnimation){
	passedAnimation->numberOfFrames = (getTextureWidth(passedAnimation->texture)/passedAnimation->width);
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
		endDrawing();
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

// Call this after removing party members
// _forcedPartySize is the party size if you want to force it because you changed the variable elsewhere.
// set _forcedPartySize to -1 if you want to use partySize as the party size. If you do set it to -1, the variable will be changed to the fixed party size after.
char FixPartyOrder(int _forcedPartySize){
	int i;
	int _partySizeCache;
	if (_forcedPartySize==-1){
		_partySizeCache = partySize;
	}else{
		_partySizeCache = _forcedPartySize;
	}
	partyMember _tempPartySwap;
	for (i=0;i<_forcedPartySize;i++){
		if (party[i].fighterStats.maxHp==-1){
			if (i!=partySize-1){
				_tempPartySwap=party[i];
				party[i]=party[i+1];
				party[i+1]=_tempPartySwap;
			}
		}
	}
	int _tempCounter=0;
	for (i=0;i<4;i++){
		if (party[i].fighterStats.maxHp!=-1){
			_tempCounter++;
		}else{
			break;
		}
	}
	if (_forcedPartySize==-1){
		partySize = _tempCounter;
	}
	return _tempCounter;
}

// Free a party member and all their resources
// slot is 1 through 4 and corresponds to the party array
// _changePartySize_DO_NOT_USE is 1 if you want to change the party size variable
// IT IS RECOMMENDED THAT YOU DON'T DO THIS
// Instead, call FixPartyOrder(-1) after this command
void DestroyPartyMember(int _slot, int _changePartySize_DO_NOT_USE){
	if (_changePartySize_DO_NOT_USE==1){
		partySize--;
	}
	party[_slot].hp=-1;
	party[_slot].fighterStats.maxHp=-1;
	free(party[_slot].fighterStats.name);
	freeTexture(partyAttackAnimation[_slot].texture);
	freeTexture(partyIdleAnimation[_slot].texture);
}

void DestroyEntireParty(){
	int i=0;
	int _partySizeCache=partySize;
	for (i=0;i<_partySizeCache;i++){
		DestroyPartyMember(i,0);
	}
	FixPartyOrder(-1);
}

void AddPartyMemberViaLua(int specialId){
	if (specialId>=100){
		printf("Something horrible happened. The value passed was over 100. Either I've wasted my time making over 100 party members, or the value is corrupted. The value is %d\n",specialId);
	}
	// enough for double digit id
	char happy[17];
	sprintf((char*)(&happy),"AddPartyMember%d",specialId);
	if (lua_getglobal(L,happy)==0){
		printf("Party member %d does not exist in LUA land\n",specialId);
	}else{
		printf("Call %s\n",happy);
		lua_call(L,0,0);
	}
}

char DoesSavefileExist(){
	fixPath("savefile",tempPathFixBuffer,TYPE_DATA);
	if (checkFileExist(tempPathFixBuffer)==1){
		return 1;
	}
	return 0;
}

/*
char savefile version ( this byte is only in savefile version 1+ )
short pathLength ( the length of the string of the last map )
string currentMapFilepath ( as long as pathLength )
short playerObject->x
short playerObject->y
char partySize
for (i=0;i<partySize;i++){
short party member i special id
}
for (i=0;i<partySize;i++){
short party member i current level
short party member i current exp
short party member i current hp
short party member i current mp
}
char number of flags
for (i=1;i<passedNumberOfFlags;i++){
	char flag value
}
for (j=0;j<partySize;j++){
	for (i=0;i<20;i++){
		if party[j] has spell
			char spell i for party member j
		else
			break
	}
	char 0
}
*/
#define SAVEFILEVERSION 2
#define MINSAVEFILEVERSION 2
void Save(){
	int i=0;
	int j=0;
	FILE* fp;

	//Can use
	//https://github.com/vitasdk/vita-headers/blob/236582e4aef188071dc83c9e8338282b12ab1731/include/psp2/apputil.h
	fixPath("savefile",tempPathFixBuffer,TYPE_DATA);
	fp = fopen (tempPathFixBuffer, "w");

	short pathLength = strlen(currentMapFilepath);
	char _tempToWrite = SAVEFILEVERSION;
	fwrite(&(_tempToWrite),1,1,fp);
	fwrite(&(pathLength),2,1,fp);
	fwrite((currentMapFilepath),pathLength,1,fp);
	fwrite(&(playerObject->x),2,1,fp);
	fwrite(&(playerObject->y),2,1,fp);
	fwrite(&(partySize),1,1,fp);
	for (i=0;i<partySize;i++){
		fwrite(&(party[i].partyMemberScriptID),2,1,fp);
	}
	for (i=0;i<partySize;i++){
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
	for (j=0;j<partySize;j++){
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
	if (DoesSavefileExist()==0){
		BasicMessage("No savefile found AND YOU STILL TRIED TO LOAD! The game will now crash.");
	}
	DestroyEntireParty();
	int i=0;
	int j=0;

	FILE* fp;

	fixPath("savefile",tempPathFixBuffer,TYPE_DATA);
	fp = fopen (tempPathFixBuffer, "r");
	//fprintf(fp, "%s\n", currentMapFilepath);
	////fread
	//fwrite(playerObject->x,2,1,fp);
	//fwrite(playerObject->y,2,1,fp);

	char _loadedSavefileversion;
	short pathLength;
	fread(&_loadedSavefileversion,1,1,fp);
	if (_loadedSavefileversion<MINSAVEFILEVERSION){
		BasicMessage("Sorry, that savefile is too old and is now incompatible.");
	}
	fread(&pathLength,2,1,fp);
	free(currentMapFilepath);
	currentMapFilepath = malloc(pathLength+1);
	memset(currentMapFilepath, 0, pathLength+1);
	fread(currentMapFilepath,pathLength,1,fp);

	fread(&(playerObject->x),2,1,fp);
	fread(&(playerObject->y),2,1,fp);

	// Fixes the player's position if they saved while walking
	playerObject->x = ceil(playerObject->x/32)*32;
	playerObject->y = ceil(playerObject->y/32)*32;

	unsigned short readLevels[4];
	short readHp[4];
	short readMp[4];

	char newPartySize;
	fread(&(newPartySize),1,1,fp);

	printf("loaded party size is %d\n", newPartySize);

	short _lastReadSpecialId;
	for (i=0;i<newPartySize;i++){
		fread(&(_lastReadSpecialId),2,1,fp);
		AddPartyMemberViaLua(_lastReadSpecialId); // partySize will be fixed here
	}

	for (i=0;i<partySize;i++){
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
	for (j=0;j<partySize;j++){
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

	for (i=0;i<partySize;i++){
		
		for (j=1;j<readLevels[i];j++){
			//printf("Leveled up %d\n",j);
			LevelUp(&(party[i]),0);
		}
		party[i].hp=readHp[i];
		party[i].mp=readMp[i];
	}

	printf("pathLength: %d\n", pathLength);
	printf("Mappath: %s\n", currentMapFilepath);
	printf("x: %d, y: %d\n",playerObject->x,playerObject->y);


	ChangeMap(currentMapFilepath);
	isWalking=0;
}

/*
////////////////////////////////////////////////////////
///
///      PLACES
/// ｎathan
////////////////////////////////////////////////////////
*/

void StatusLoop(){
	signed char selectedMember=0;
	char statNumberString[10];
	while (1){
		StartFrameStuff();
		if (wasJustPressed(bButton)){
			place=1;
			break;
		}else if (wasJustPressed(SCE_CTRL_LEFT)){
			selectedMember--;
			if (selectedMember<0){
				selectedMember=partySize-1;
			}
		}else if (wasJustPressed(SCE_CTRL_RIGHT)){
			selectedMember++;
			if (selectedMember==partySize){
				selectedMember=0;
			}
		}
		startDrawing();
		DrawGrayBackground();
		#if PLATFORM != PLAT_3DS
			goodDrawText(SCREENWIDTH/2-strlen(party[selectedMember].fighterStats.name)*64/2,3,party[selectedMember].fighterStats.name,8);
			goodDrawText(3,69,N_ATK,4);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.attack);
			goodDrawText(strlen(N_ATK)*32+32,69,statNumberString,4);
			goodDrawText(3,106,N_MATK,4);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.magicAttack);
			goodDrawText(strlen(N_MATK)*32+32,106,statNumberString,4);
			goodDrawText(3,143,N_DEF,4);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.defence);
			goodDrawText(strlen(N_DEF)*32+32,143,statNumberString,4);
			goodDrawText(3,180,N_MDEF,4);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.magicDefence);
			goodDrawText(strlen(N_MDEF)*32+32,180,statNumberString,4);
			goodDrawText(3,217,N_SPEED,4);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.speed);
			goodDrawText(strlen(N_SPEED)*32+32,217,statNumberString,4);
	
			sprintf((char*)&statNumberString,"%d/%d",party[selectedMember].hp,party[selectedMember].fighterStats.maxHp);
			goodDrawText(strlen(N_HP)*32+32,254,statNumberString,4);
			goodDrawText(3,254,N_HP,4);
			sprintf((char*)&statNumberString,"%d/%d",party[selectedMember].mp,party[selectedMember].fighterStats.maxMp);
			goodDrawText(strlen(N_MP)*32+32,291,statNumberString,4);
			goodDrawText(3,291,N_MP,4);
			
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.level);
			goodDrawText(strlen(N_LV)*32+32,328,statNumberString,4);
			goodDrawText(3,328,N_LV,4);
			
			double animationScale=(SCREENHEIGHT-180)/ partyIdleAnimation[selectedMember].height;
			DrawAnimationAsISay(&partyIdleAnimation[selectedMember],SCREENWIDTH-partyIdleAnimation[selectedMember].width*animationScale-100,180,animationScale);
		#elif PLATFORM == PLAT_3DS
			DrawMapThings();
			endDrawing();
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			
			// Draw name
			goodDrawText(CenterTextCustomWidth(party[selectedMember].fighterStats.name,3,BOTTOMSCREENWIDTH),3,party[selectedMember].fighterStats.name,3);
			
			int i=0;
			// Draw attack
			i++;
			goodDrawText(3,20*i+2*i+10,N_ATK,2);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.attack);
			goodDrawText(strlen(N_ATK)*16+16,20*i+2*i+10,statNumberString,2);

			i++;
			goodDrawText(3,20*i+2*i+10,N_MATK,2);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.magicAttack);
			goodDrawText(strlen(N_MATK)*16+16,20*i+2*i+10,statNumberString,2);

			i++;
			goodDrawText(3,20*i+2*i+10,N_DEF,2);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.defence);
			goodDrawText(strlen(N_DEF)*16+16,20*i+2*i+10,statNumberString,2);

			i++;
			goodDrawText(3,20*i+2*i+10,N_MDEF,2);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.magicDefence);
			goodDrawText(strlen(N_MDEF)*16+16,20*i+2*i+10,statNumberString,2);

			i++;
			goodDrawText(3,20*i+2*i+10,N_SPEED,2);
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.speed);
			goodDrawText(strlen(N_SPEED)*16+16,20*i+2*i+10,statNumberString,2);
			
			i++;
			sprintf((char*)&statNumberString,"%d/%d",party[selectedMember].hp,party[selectedMember].fighterStats.maxHp);
			goodDrawText(strlen(N_HP)*16+16,20*i+2*i+10,statNumberString,2);
			goodDrawText(3,20*i+2*i+10,N_HP,2);

			i++;
			sprintf((char*)&statNumberString,"%d/%d",party[selectedMember].mp,party[selectedMember].fighterStats.maxMp);
			goodDrawText(strlen(N_MP)*16+16,20*i+2*i+10,statNumberString,2);
			goodDrawText(3,20*i+2*i+10,N_MP,2);
			
			i++;
			sprintf((char*)&statNumberString,"%d",party[selectedMember].fighterStats.level);
			goodDrawText(strlen(N_LV)*16+32,20*i+2*i+10,statNumberString,2);
			goodDrawText(3,20*i+2*i+10,N_LV,2);			
			
			double animationScale=(BOTTOMSCREENHEIGHT-120)/ (double)partyIdleAnimation[selectedMember].height;
			DrawAnimationAsISay(&partyIdleAnimation[selectedMember],BOTTOMSCREENWIDTH-partyIdleAnimation[selectedMember].width*animationScale-32,120,animationScale);
		#endif

		endDrawing();
		EndFrameStuff();
	}
}

#if PLATFORM != PLAT_COMPUTER
	void MenuLop(){
	signed char selected=0;
	// 0 - menu
	// 1 - load confirmation
	// 2 - save confirmation
	char subspot=0;

	//char tempStringAssembly[5];

	while(1){
		StartFrameStuff();
		
		if (wasJustPressed(aButton)){
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
						subspot=2;
						selected=1;
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
			}else if (subspot==2){
				switch(selected){
					case 0:
						Save();
						place=0;
						return;
					break;
					case 1:
						subspot=0;
					break;
				}
			}
		}else if (wasJustPressed(SCE_CTRL_START)){
			place=0;
			return;
		}else if (wasJustPressed(bButton)){
			if (subspot==0){
				place=0;
				return;
			}else{
				subspot=0;
			}
		}
		if (wasJustPressed(SCE_CTRL_DOWN)){
			selected++;
			if (subspot==0){
				if (selected>4){
					selected=0;
				}
			}else if (subspot==1 || subspot==2){
				if (selected>1){
					selected=0;
				}
			}
		}else if (wasJustPressed(SCE_CTRL_UP)){
			selected--;
			if (subspot==0){
				if (selected<0){
					selected=4;
				}
			}else if (subspot==1 || subspot==2){
				if (selected<0){
					selected=0;
				}
			}
		}


		startDrawing();
	
		DrawMapThings();


		#if PLATFORM!=PLAT_3DS
			// Draw fancy shade thingie
			drawRectangle(720,136,5,272,0,0,0,255);
			drawRectangle(240,408,485,5,0,0,0,255);
			// Draw border
			drawRectangle(240,136,480,272,0,255,0,255);
			// Draw real rectangle
			drawRectangle(245,141,470,262,252,255,255,255);
	
			if (subspot==0){
				goodDrawText(CenterText(N_HAPPYMENU,4),146,N_HAPPYMENU,4);
				goodDrawText(287,183,N_RESUME,4);
				goodDrawText(287,183+32+5,playerName,4);
				goodDrawText(287,183+32+32+5+5,N_SAVE,4);
				goodDrawText(287,183+32+32+32+5+5+5,N_LOAD,4);
				goodDrawText(287,183+32+32+32+32+5+5+5+5,N_QUIT,4);
			}else if (subspot==1){
				if (LANGUAGE==LANG_SPANISH){
					goodDrawText(CenterText("?Quieres cargar?",4),146,"'?Quieres cargar?",4);
					goodDrawText(287,183,"S'i.",4);
					goodDrawText(287,183+32+5,"No.",4);
				}else if (LANGUAGE==LANG_ENGLISH){
					goodDrawText(CenterText("Really load?",4),146,"Really load?",4);
					goodDrawText(287,183,"Yes.",4);
					goodDrawText(287,183+32+5,"No.",4);
				}
			}else if (subspot==2){
				if (LANGUAGE==LANG_SPANISH){
					goodDrawText(CenterText("?Quieres Salvar?",4),146,"'?Quieres Salvar?",4);
					goodDrawText(287,183,"S'i.",4);
					goodDrawText(287,183+32+5,"No.",4);
				}else if (LANGUAGE==LANG_ENGLISH){
					goodDrawText(CenterText("Really save?",4),146,"Really save?",4);
					goodDrawText(287,183,"Yes.",4);
					goodDrawText(287,183+32+5,"No.",4);
				}
			}
			DrawAnimationAsISay(&selectorAnimation,245,selected*selectorAnimation.height*2+(selectorAnimation.height/2)+183+selected*5,2);
		#elif PLATFORM == PLAT_3DS
			endDrawing();
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			// Draw real rectangle
			drawRectangle(0,0,320,240,252,255,255,255);
	
			if (subspot==0){
				goodDrawText(5,0,N_HAPPYMENU,3.5);
				// 0
				goodDrawText(30,32+24+5,N_RESUME,3);
				// 1
				goodDrawText(30,32+24+24+5+5,playerName,3);
				// 2
				goodDrawText(30,32+24+24+24+5+5+5,N_SAVE,3);
				// 3
				goodDrawText(30,32+24+24+24+24+5+5+5+5,N_LOAD,3);
				// 4
				goodDrawText(30,32+24+24+24+24+24+5+5+5+5+5,N_QUIT,3);
			}else if (subspot==1){
				if (LANGUAGE==LANG_SPANISH){
					goodDrawText(5,0,"'?Quieres cargar?",3.5);
					goodDrawText(30,32+24+5,"S'i.",3);
					goodDrawText(30,32+24+25+5+5,"No.",3);
				}else if (LANGUAGE==LANG_ENGLISH){
					goodDrawText(5,0,"Really load?",3.5);
					goodDrawText(30,32+24+5,"Yes.",3);
					goodDrawText(30,32+24+25+5+5,"No.",3);
				}
			}else if (subspot==2){
				if (LANGUAGE==LANG_SPANISH){
					goodDrawText(5,0,"'?Quieres salvar?",3.5);
					goodDrawText(30,32+24+5,"S'i.",3);
					goodDrawText(30,32+24+25+5+5,"No.",3);
				}else if (LANGUAGE==LANG_ENGLISH){
					goodDrawText(5,0,"Really save?",3.5);
					goodDrawText(30,32+24+5,"Yes.",3);
					goodDrawText(30,32+24+25+5+5,"No.",3);
				}
			}
			DrawAnimationAsISay(&selectorAnimation,0,selected*24+32+24+selected*5+5,1);
		#endif


		endDrawing();
		EndFrameStuff();
	}
	}
#else
	#define SUBMENU_MAIN 0
	#define SUBMENU_SAVECONFIRM 1
	#define SUBMENU_LOADCONFIRM 2
	void MenuLop(){

		char submenu=SUBMENU_MAIN;

		CrossTexture* resumeButton = LoadEmbeddedPNG("Stuff/Menu/ResumeIcon.png");
		CrossTexture* backButton = LoadEmbeddedPNG("Stuff/Menu/QuitIcon.png");
		CrossTexture* saveButton = LoadEmbeddedPNG("Stuff/Menu/SaveIcon.png");
		CrossTexture* loadButton = LoadEmbeddedPNG("Stuff/Menu/LoadIcon.png");
		CrossTexture* yesButton = LoadEmbeddedPNG("Stuff/Battle/SelectIcon.png");
		CrossTexture* noButton = LoadEmbeddedPNG("Stuff/Battle/BackIcon.png");
		float _buttonScale = (SCREENHEIGHT/4)/64;
		while (1){
			FpsCapStart();

			controlsStart();

			if (wasJustPressed(SCE_ANDROID_BACK)){
				place=PLACE_OVERWORLD;
				break;
			}

			if (wasJustPressed(SCE_TOUCH)){
				if (FixTouchX(touchX)>CenterSomething(getTextureWidth(yesButton)*_buttonScale) && FixTouchX(touchX)<CenterSomething(getTextureWidth(yesButton)*_buttonScale)+getTextureWidth(yesButton)*_buttonScale){
					int i;
					for (i=3;i!=-1;i--){	
						if  (FixTouchY(touchY)>=getTextureHeight(resumeButton)*_buttonScale*i){
							break;
						}
					}
					if (submenu == SUBMENU_MAIN){
						// Pressed button is 0 based i
						if (i==0){
							place=PLACE_OVERWORLD;
							PlayMenuSoundEffect();
							break;
						}else if (i==1){
							submenu=SUBMENU_SAVECONFIRM;
							PlaySoftMenuSoundEffect();
						}else if (i==2){
							submenu=SUBMENU_LOADCONFIRM;
							PlaySoftMenuSoundEffect();
						}else{
							place=PLACE_QUIT;
							break;
						}
					}else if (submenu==SUBMENU_SAVECONFIRM){
						if (i==1){
							Save();
							place = PLACE_OVERWORLD;
							PlayMenuSoundEffect();
							break;
						}else if (i==2){
							submenu = SUBMENU_MAIN;
							PlaySoftMenuSoundEffect();
						}
					}else if (submenu==SUBMENU_LOADCONFIRM){
						if (i==1){
							if (DoesSavefileExist()==0){
								BasicMessage("No savefile found.");
							}else{
								Load();
							}
							place = PLACE_OVERWORLD;
							PlayMenuSoundEffect();
							break;
						}else if (i==2){
							submenu = SUBMENU_MAIN;
							PlaySoftMenuSoundEffect();
						}
					}
				}
			}
			controlsEnd();

			startDrawing();
			DrawGrayBackground();
			if (submenu==SUBMENU_MAIN){
				drawTextureScale(resumeButton,CenterSomething(getTextureWidth(resumeButton)*_buttonScale),getTextureHeight(resumeButton)*_buttonScale*0,_buttonScale,_buttonScale);
				drawTextureScale(saveButton,CenterSomething(getTextureWidth(resumeButton)*_buttonScale),getTextureHeight(resumeButton)*_buttonScale*1,_buttonScale,_buttonScale);
				drawTextureScale(loadButton,CenterSomething(getTextureWidth(resumeButton)*_buttonScale),getTextureHeight(resumeButton)*_buttonScale*2,_buttonScale,_buttonScale);
				drawTextureScale(backButton,CenterSomething(getTextureWidth(resumeButton)*_buttonScale),getTextureHeight(resumeButton)*_buttonScale*3,_buttonScale,_buttonScale);
			}else if (submenu==SUBMENU_SAVECONFIRM){
				goodDrawText(CenterSomething(textWidth(fontSize,"Really save?")),currentTextHeight/2,"Really save?",fontSize);
				drawTextureScale(yesButton,CenterSomething(getTextureWidth(yesButton)*_buttonScale),getTextureHeight(resumeButton)*_buttonScale*1,_buttonScale,_buttonScale);
				drawTextureScale(noButton,CenterSomething(getTextureWidth(noButton)*_buttonScale),getTextureHeight(resumeButton)*_buttonScale*2,_buttonScale,_buttonScale);
			}else if (submenu==SUBMENU_LOADCONFIRM){
				goodDrawText(CenterSomething(textWidth(fontSize,"Really load?")),currentTextHeight/2,"Really load?",fontSize);
				drawTextureScale(yesButton,CenterSomething(getTextureWidth(yesButton)*_buttonScale),getTextureHeight(resumeButton)*_buttonScale*1,_buttonScale,_buttonScale);
				drawTextureScale(noButton,CenterSomething(getTextureWidth(noButton)*_buttonScale),getTextureHeight(resumeButton)*_buttonScale*2,_buttonScale,_buttonScale);
			}
			endDrawing();

			FpsCapWait();
		}
		freeTexture(resumeButton);
		freeTexture(backButton);
		freeTexture(saveButton);
		freeTexture(loadButton);
		freeTexture(yesButton);
		freeTexture(noButton);
		controlsReset();
	}
#endif

void CreditsView(){
	// Squint to view.
	#if TEXTRENDERER == TEXT_FONTCACHE
		fontSize = 20;
		LoadFont();
	#endif


	const char* _creditsFileList[] = {
	"Credits.txt",
	"Liberation-Sans-License.txt",
	"libvita2d-License.txt",
	"LICENSE.FLAC.txt",
	"LICENSE.freetype.txt",
	//"LICENSE.modplug.txt",
	"LICENSE.ogg-vorbis.txt",
	"LICENSE.smpeg.txt",
	//"LICENSE.zlib.txt",
	"Lua-License.txt",
	"SDL_FontCache-License.txt" };
	//"SDL-License.txt",
	//"SDL-Mixer-License.txt",
	//"SDL-ttf-License.txt" };
	int selected=0;
	const int _selectedMax=sizeof(_creditsFileList)/sizeof(char*);
	CrossTexture* leftButton = loadPNG("Stuff/Battle/LeftIcon.png");
	CrossTexture* rightButton = loadPNG("Stuff/Battle/RightIcon.png");
	CrossTexture* selectButton = loadPNG("Stuff/Battle/SelectIcon.png");
	CrossTexture* backButton = loadPNG("Stuff/Battle/BackIcon.png");

	int _creditsLinePerScreen = floor(((SCREENHEIGHT-32-getTextureHeight(selectButton)*2))/(double)currentTextHeight);
	char _currentLoadedCredits[_creditsLinePerScreen][200];

	while (1){
		FpsCapStart();
		controlsStart();
		#if TOUCHENABLED
			if (wasJustPressed(SCE_TOUCH)){
				if (FixTouchX(touchX)<getTextureWidth(leftButton)*2 && FixTouchY(touchY)>SCREENHEIGHT-getTextureHeight(leftButton)*2){
					selected--;
					if (selected<0){
						selected=_selectedMax-1;
					}
				}else if (FixTouchX(touchX)>SCREENWIDTH-getTextureWidth(leftButton)*2 && FixTouchY(touchY)>SCREENHEIGHT-getTextureHeight(leftButton)*2){
					selected++;
					if (selected>=_selectedMax){
						selected=0;
					}
				}else if (FixTouchX(touchX)>((SCREENWIDTH/2)-getTextureWidth(selectButton)*2/2) && FixTouchX(touchX)<((SCREENWIDTH/2)-getTextureWidth(selectButton)*2/2)+getTextureWidth(selectButton)*2 && FixTouchY(touchY)>SCREENHEIGHT-getTextureHeight(selectButton)*2){
					break;
				}
			}
		#endif
		controlsEnd();

		startDrawing();
		DrawGrayBackground();
		drawTextureScale(leftButton,0,SCREENHEIGHT-getTextureHeight(leftButton)*2,2,2);
		drawTextureScale(rightButton,SCREENWIDTH-getTextureWidth(leftButton)*2,SCREENHEIGHT-getTextureHeight(leftButton)*2,2,2);
		drawTextureScale(selectButton,(SCREENWIDTH/2)-getTextureWidth(selectButton)*2/2,SCREENHEIGHT-getTextureHeight(selectButton)*2,2,2);
		
		//goodDrawText(int x, int y, const char* text, float size)
		goodDrawText(32,32,"Use the buttons to select a credits or license file.",fontSize);
		goodDrawText(32,32+currentTextHeight*2,_creditsFileList[selected],fontSize);
		endDrawing();

		FpsCapWait();
	}
	printf("Player selected License/%s\n",_creditsFileList[selected]);
	CROSSFILE* currentCreditsFile;
	char _tempCreditsFilepathConcat[strlen(_creditsFileList[selected])+strlen("Stuff/License/")+1];
	strcpy(_tempCreditsFilepathConcat,"Stuff/License/");
	strcat(_tempCreditsFilepathConcat,_creditsFileList[selected]);
	fixPath(_tempCreditsFilepathConcat,tempPathFixBuffer, TYPE_EMBEDDED);
	currentCreditsFile = crossfopen(_tempCreditsFilepathConcat,"r");
	if (currentCreditsFile==NULL){
		BasicMessage("Failed to load file.");
	}
	char _quit=0;
	while (_quit==0){
		int i, j;
		for (i=0;i<_creditsLinePerScreen;i++){
			for (j=0;j<199;j++){
				_currentLoadedCredits[i][j]=0;
			}
		}
		for (i=0;i<_creditsLinePerScreen;i++){
			for (j=0;j<199;j++){
				if (crossfread((&(_currentLoadedCredits[i][j])),1,1,currentCreditsFile)==0){
					if (j==0 && i==0){
						_quit=1;
					}
					break;
				}
				if (isNewLine(currentCreditsFile,_currentLoadedCredits[i][j])==1){
					break;
				}
				_currentLoadedCredits[i][j+1]='\0';
				//printf("%s;%d\n",_currentLoadedCredits[i],textWidth(fontSize,_currentLoadedCredits[i]));
				if (textWidth(fontSize,_currentLoadedCredits[i])>=SCREENWIDTH-32){
					crossfseek(currentCreditsFile,-1,CROSSFILE_CUR);
					break;
				}
			}
			_currentLoadedCredits[i][j]='\0';
			//printf("%s\n",_currentLoadedCredits[i],i,_creditsLinePerScreen);
		}
		#if PLATFORM != PLAT_SWITCH
			controlsResetEmpty();
		#else
			controlsStart();
			controlsEnd();
		#endif
		if (_quit==1){
			break;
		}
		while (1){
			FpsCapStart();
			controlsStart();
			#if TOUCHENABLED
				if (wasJustPressed(SCE_TOUCH)){
					if (FixTouchX(touchX)<getTextureWidth(leftButton)*2 && FixTouchY(touchY)>SCREENHEIGHT-getTextureHeight(leftButton)*2){
						_quit=1;
						break;
					}else if (FixTouchX(touchX)>SCREENWIDTH-getTextureWidth(leftButton)*2 && FixTouchY(touchY)>SCREENHEIGHT-getTextureHeight(leftButton)*2){
						break;
					}
				}
			#endif
			controlsEnd();
			startDrawing();
			DrawGrayBackground();
			drawTextureScale(backButton,0,SCREENHEIGHT-getTextureHeight(leftButton)*2,2,2);
			drawTextureScale(rightButton,SCREENWIDTH-getTextureWidth(leftButton)*2,SCREENHEIGHT-getTextureHeight(leftButton)*2,2,2);
			
			//goodDrawText(int x, int y, const char* text, float size)
			for (i=0;i<_creditsLinePerScreen;i++){
				goodDrawText(32,32+i*currentTextHeight,_currentLoadedCredits[i],fontSize);
			}
			
			//goodDrawText(32,32+currentTextHeight*2,_creditsFileList[selected],fontSize);
			
			endDrawing();
			FpsCapWait();
		}
	}
	crossfclose(currentCreditsFile);
	freeTexture(leftButton);
	freeTexture(rightButton);
	freeTexture(selectButton);
	freeTexture(backButton);
	#if TEXTRENDERER == TEXT_FONTCACHE
		fontSize = 50;
		LoadFont();
	#endif
}

void Overworld(){
	FpsCapStart();
	controlsStart();
	// Main logic
	#if PLATFORM == PLAT_COMPUTER
		if (wasJustPressed(SCE_TOUCH)){
			if (FixTouchX(touchX)<32*MAPXSCALE && FixTouchY(touchY)<32*MAPYSCALE){
				PlaySoftMenuSoundEffect();
				place=1;
				foundPathLength=-1;
				controlsEnd();
				controlsReset();
			}
		}
	#endif

	// Controls only if not walking
	if (isWalking==0){
		// This are relative to the player. Moving up would have the y be -1 and x be 0
		signed char _moveXQueue=127;
		signed char _moveYQueue=127;
		// 1 if they press aButton
		signed char _actionQueue=127;
		#if PLATFORM == PLAT_COMPUTER
			if (wasJustPressed(SCE_TOUCH)){
				int _touchedTileX = floor( (( FixTouchX(touchX) )/MAPXSCALE)/32)+cameraWholeOffsetX;
				int _touchedTileY = floor( (( FixTouchY(touchY) )/MAPYSCALE)/32)+cameraWholeOffsetY;
				//printf("They pressed %d,%d\n",_touchedTileX,_touchedTileY);
				currentListValue=1;
				free(pathfindingPath);
				pathfindingPath = FindPath(&foundPathLength,playerObject->x/32,playerObject->y/32,_touchedTileX,_touchedTileY);
				if (foundPathLength!=-1){ // -1 or 0
					walkingPathProgress=0;
				}
			}
		#endif

		if (foundPathLength!=-1){
			walkingPathProgress++;
			_moveXQueue =  pathfindingPath[walkingPathProgress].x-playerObject->x/32;
			_moveYQueue =  pathfindingPath[walkingPathProgress].y-playerObject->y/32;
			// foundPathLength is 1 more than the actual length. This checks if the path was already completed.
			// We need to have foundPathLength equal -1 before isWalking is set to 0 for the animation to stop correctly
			// A -2 foundPathLength value means that the last move in the path started on this frame
			// With a -2 value, we can check if the user hasn't moved. If they haven't, that means that the destination is a place they should interact with.
			if (walkingPathProgress==foundPathLength-1){
				foundPathLength=-2;
				walkingPathProgress=0;
			}
		}

		if (isDown(SCE_CTRL_UP)){
			_moveXQueue = 0;
			_moveYQueue = -1;
		}else if (isDown(SCE_CTRL_DOWN)){
			_moveXQueue = 0;
			_moveYQueue = 1;
		}else if (isDown(SCE_CTRL_LEFT)){
			_moveXQueue = -1;
			_moveYQueue = 0;
		}else if (isDown(SCE_CTRL_RIGHT)){
			_moveXQueue = 1;
			_moveYQueue = 0;
		}

		// Only do the tests if they are not the default values
		if (_moveXQueue!=127 && _moveYQueue!=127){
			if (_moveYQueue<0){
				if (CheckCollision(playerObject->x/32,playerObject->y/32+_moveYQueue)!=1){
					isWalking=1;
					playerObject->theAnimation.numberOfFrames=3;
					playerObject->y=playerObject->y-4;
				}
				playerObject->theAnimation.texture=playerUp;
			}else if (_moveYQueue>0){
				if (CheckCollision(playerObject->x/32,playerObject->y/32+_moveYQueue)!=1){
					isWalking=2;
					playerObject->theAnimation.numberOfFrames=3;
					playerObject->y=playerObject->y+4;
				}
				playerObject->theAnimation.texture=playerDown;
			}else if (_moveXQueue<0){
				if (CheckCollision(playerObject->x/32+_moveXQueue,playerObject->y/32)!=1){
					isWalking=3;
					playerObject->theAnimation.numberOfFrames=3;
					playerObject->x=playerObject->x-4;
				}
				playerObject->theAnimation.texture=playerLeft;
			}else if (_moveXQueue>0){
				if (CheckCollision(playerObject->x/32+_moveXQueue,playerObject->y/32)!=1){
					isWalking=4;
					playerObject->theAnimation.numberOfFrames=3;
					playerObject->x=playerObject->x+4;
				}
				playerObject->theAnimation.texture=playerRight;
			}

			// The player tried to move, but didn't because it was solid.
			if (playerObject->x%32==0 && playerObject->y%32==0){
				// Player didn't move.
				playerObject->theAnimation.addOnChange=1;
				playerObject->theAnimation.numberOfFrames=1;
				playerObject->theAnimation.currentFrame=0;
				// This checks if the player is on a path. If they are, and didn't move, that means that their destination is a wall and they should try to interact with it.
				if (foundPathLength==-2){
					_actionQueue=QUEUE_X;
				}
			}
			// This frame is over. It is no longer the frame on which the user just started to walk the last spot on their path
			if (foundPathLength==-2){
				foundPathLength=-1;
			}
		}

		if (wasJustPressed(aButton)){
			_actionQueue=QUEUE_X;
		}

		if (_actionQueue==QUEUE_X){
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
				if (!(isDown(SCE_CTRL_UP) || (isDown(SCE_CTRL_DOWN)) || (isDown(SCE_CTRL_LEFT)) || (isDown(SCE_CTRL_RIGHT)) || foundPathLength!=-1)){
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
	if (wasJustPressed(SCE_CTRL_START)){
		place=1;
	}

	UpdateCameraValues(playerObject);

	// Drawing
	startDrawing();
	DrawMapThings();
	#if PLATFORM == PLAT_COMPUTER
		drawTextureScale(pauseButtonTexture,0,0,MAPXSCALE,MAPYSCALE);
	#endif
	endDrawing();
	controlsEnd();
	FpsCapWait();
}

// Returns 1 if the player won, 0 otherwise
char BattleLop(char canRun){
	// 0 - Ready for next person in line
	// 1 - Choosing attack
	// 2 - Choosing target
	// 3 - Fighter moving to target
	// 4 - Fighter moving back from target
	// 5 - Showing spell animation
	char battleStatus=BATTLESTATUS_NEXTTURN;
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
	#if PLATFORM == PLAT_COMPUTER
		// Start offscreen
		signed char selected=-4;
	#else
		signed char selected=0;
	#endif
	// Move 
	signed short moveXPerFrame=0;
	signed short moveYPerFrame=0;
	// Original position before moving.
	short originalX=0;
	short originalY=0;

	// In battleStatus BATTLESTATUS_MOVEBACKFROMTARGET, used to hold damage delt.
	// In battleStatus BATTLESTATUS_SPELLANIMATION, used for animation stuff
	// Between 
	int temp=0;
	// In battleStatus BATTLESTATUS_MOVEBACKFROMTARGET and BATTLESTATUS_SPELLANIMATION, used as string to display damage delt
	// At the end of the battle, used to display earned EXP.
	// Note - this used to be size 10, but GCC told me to make it size 11.
	char temp2[11];
	// X speed for magic moving in BATTLESTATUS_CHOOSINGMOVE and BATTLESTATUS_CHOOSINGTARGET
	// Damage delt in BATTLESTATUS_MOVEBACKFROMTARGET and BATTLESTATUS_SPELLANIMATION
	// Also temp while var in start
	int temp3=0;
	// Y speed for magic moving in BATTLESTATUS_CHOOSINGMOVE and BATTLESTATUS_CHOOSINGTARGET
	int temp4=1;

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
	CrossTexture* iconHighlightTexture=NULL;
	// Used when choosing target
	CrossTexture* leftButton=NULL;
	CrossTexture* rightButton=NULL;
	CrossTexture* selectButton=NULL;
	CrossTexture* backButton=NULL;

	attackButton = LoadEmbeddedPNG("Stuff/Battle/AttackIcon.png");
	magicButton = LoadEmbeddedPNG("Stuff/Battle/MagicIcon.png");
	runButton = LoadEmbeddedPNG("Stuff/Battle/RunIcon.png");
	itemButton = LoadEmbeddedPNG("Stuff/Battle/ItemIcon.png");
	winTexture = LoadEmbeddedPNG("Stuff/Battle/Win.png");
	iconHighlightTexture = LoadEmbeddedPNG("Stuff/Battle/HighlightIcon.png");
	selectButton = LoadEmbeddedPNG("Stuff/Battle/SelectIcon.png");
	leftButton = LoadEmbeddedPNG("Stuff/Battle/LeftIcon.png");
	rightButton = LoadEmbeddedPNG("Stuff/Battle/RightIcon.png");
	backButton = LoadEmbeddedPNG("Stuff/Battle/BackIcon.png");

	spell* selectedSpell=&(GetSpellList(0)->theSpell);

	// Battle moving in intro
	temp3=1;
	while (temp3==1){
		// Entrence positions ONLY. The position that the partyMember struct holds is their end position, this is their current position in the battle intro
		int xPositions[8]={0};
		int yPositions[8]={0};
		signed char addPerFrame[8]={0,0,0,0,0,0,0,0};

		// This prepares variables that are used when the enemies are going in from off screen
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
			// Start the battle intro positions off screen
			if (GetBattlerById(i)->x<SCREENWIDTH/2){
				xPositions[i-1]=GetBattlerAnimationById(i,ANIMATION_IDLE)->width*-1;
			}else{
				xPositions[i-1]=GetBattlerAnimationById(i,ANIMATION_IDLE)->width+SCREENWIDTH;
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
			startDrawing();
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
					DrawAnimationAsISay((GetBattlerAnimationById(i,ANIMATION_IDLE)),xPositions[i-1],yPositions[i-1],BATTLEENTITYSCALE);
					continue;
				}
				xPositions[i-1]+=addPerFrame[i-1];

				DrawAnimationAsISay((GetBattlerAnimationById(i,ANIMATION_IDLE)),xPositions[i-1],yPositions[i-1],BATTLEENTITYSCALE);
			}
			endDrawing();

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

	// This is the main battle loop
	//controlsResetEmpty();
	controlsStart();
	controlsEnd();
	while (1){
		FpsCapStart();
		controlsStart();

		// Alright, time to go to next turn
		if (battleStatus==BATTLESTATUS_NEXTTURN)/*My body is ready for next turn*/{

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
						FpsCapStart();
						controlsStart();
						
						if (wasJustPressed(aButton)){
							break;
						}
						#if PLATFORM == PLAT_COMPUTER
							if (wasJustPressed(SCE_TOUCH)){
								break;
							}
						#endif
						#if PLATFORM != PLAT_3DS
							startDrawing();
							DrawGrayBackground();
							drawTextureScale(winTexture,CenterSomething(getTextureWidth(winTexture)*2),3,2,2);

							goodDrawText(0,10+getTextureHeight(winTexture)*2,"EXP:",8);
							goodDrawText(textWidth(fontSize,"EXP: "),10+getTextureHeight(winTexture)*2,temp2,8);
					
							for (i=0;i<partySize;i++){
								if (didLevelUp[i]==1){
									goodDrawText(0,10+getTextureHeight(winTexture)*2+(i+1)*currentTextHeight,party[i].fighterStats.name,4);
									goodDrawText(textWidth(fontSize,party[i].fighterStats.name)+textWidth(fontSize," "),10+getTextureHeight(winTexture)*2+(i+1)*currentTextHeight,N_LEVELEDUP,4);
								}
							}
						#else
							startDrawing();
							drawTextureScale(winTexture,CenterSomething(getTextureWidth(winTexture)*1.8),3,1.8,1.8);
							endDrawing();
							sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);

							goodDrawText(5,5,"EXP:",4);
							goodDrawText(8*4*5+5,5,temp2,4);
					
							for (i=0;i<partySize;i++){
								if (didLevelUp[i]==1){
									goodDrawText(5,40+i*24+i*5,party[i].fighterStats.name,2);
									goodDrawText(5+strlen(party[i].fighterStats.name)*16+16,40+i*24+i*5,N_LEVELEDUP,2);
								}
							}
						#endif
				
						endDrawing();
						controlsEnd();
						FpsCapWait();
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
					// Fix the magic moving
					temp4=1;
					temp3=0;
					battleStatus=1;
				}else if (orderOfAction[(int)currentOrder]<=8){
					// generate enemy turn
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
						
						int tempDone = Damage(GetBattlerById(orderOfAction[currentOrder]),GetBattlerById(target),selectedSpell->attack,selectedSpell->magicAttack,selectedSpell->spellSpecialProperty);
						sprintf((char*)&temp2,"%d",tempDone);
						GetBattlerById(orderOfAction[currentOrder])->mp -= selectedSpell->mpCost;
						
						battleStatus=BATTLESTATUS_SPELLANIMATION;
					}else{
						// Using normal attack
						battleStatus=BATTLESTATUS_MOVETOTARGET;
						originalY=GetBattlerById(orderOfAction[currentOrder])->y;
						originalX=GetBattlerById(orderOfAction[currentOrder])->x;
	
						moveXPerFrame=3;
						moveXPerFrame=floor((GetBattlerById(target)->x - GetBattlerById(orderOfAction[currentOrder])->x - GetBattlerAnimationById(orderOfAction[currentOrder],ANIMATION_ATTACK)->width*BATTLEENTITYSCALE+32 )/30);
						moveYPerFrame=floor((GetBattlerById(target)->y - GetBattlerById(orderOfAction[currentOrder])->y + GetBattlerAnimationById(target,ANIMATION_IDLE)->height/4)/30);
		
						if (moveXPerFrame==0){
							moveXPerFrame=ceil((GetBattlerById(target)->x - GetBattlerById(orderOfAction[currentOrder])->x - GetBattlerAnimationById(orderOfAction[currentOrder],ANIMATION_ATTACK)->width*BATTLEENTITYSCALE+32 )/30);
						}
						if (moveYPerFrame==0){
							moveYPerFrame=ceil((GetBattlerById(target)->y - GetBattlerById(orderOfAction[currentOrder])->y + GetBattlerAnimationById(target,ANIMATION_IDLE)->height/4)/30);
						}
					}
				}else{
					// We're on 9 or some bad number.
					// Start new turn rotation.
					currentOrder=0;
				}
			}
		}else if (battleStatus==BATTLESTATUS_CHOOSINGMOVE)/*Choosing type of move*/{
			char _actionQueue=0;
			#if PLATFORM == PLAT_COMPUTER
				if (wasJustPressed(SCE_TOUCH)){
					if (FixTouchY(touchY)>SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE){
						if ( FixTouchX(touchX)>(SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2 ){
							int i;
							int _oldSelected=selected;
							char _didChange=0;
							for (i=3;i!=-1;i--){
								if  (FixTouchX(touchX)>=((SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2)+getTextureWidth(attackButton)*SELECTIONBUTTONSCALE*i){
									selected=i;
									_didChange=1;
									break;
								}
							}
							if (_didChange==1){
								if (selected==_oldSelected){
									_actionQueue = QUEUE_X;
								}else{
									PlaySoftMenuSoundEffect();
								}
							}
						}
					}
					//drawTextureScale(attackButton,(SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2,SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE);
					//drawTextureScale(magicButton,((SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2)+getTextureWidth(attackButton)*SELECTIONBUTTONSCALE,SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE);
					//drawTextureScale(itemButton,((SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2)+getTextureWidth(attackButton)*SELECTIONBUTTONSCALE*2,SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE);
					//drawTextureScale(runButton,((SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2)+getTextureWidth(attackButton)*SELECTIONBUTTONSCALE*3,SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE);
				}
			#endif
			if (wasJustPressed(SCE_CTRL_DOWN) || wasJustPressed(SCE_CTRL_RIGHT)){
				selected++;
				if (canRun==1){
					if (selected==4){
						selected=0;
					}
				}else{
					if (selected==3){
						selected=0;
					}
				}
			}
			if (wasJustPressed(SCE_CTRL_UP) || wasJustPressed(SCE_CTRL_LEFT)){
				selected--;
				if (selected<0){
					if (canRun==1){
						selected=3;
					}else{
						selected=1;
					}
				}
			}
			if (wasJustPressed(aButton)){
				_actionQueue = QUEUE_X;
			}
			if (_actionQueue==QUEUE_X){
				PlayMenuSoundEffect();
				temp=0;
				if (selected==0){
					// Attack
					battleStatus=BATTLESTATUS_CHOOSINGTARGET;
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

						battleStatus=BATTLESTATUS_CHOOSINGTARGET;
					}
				}else if (selected==2){
					// Item
					BasicMessage("Maybe one day, there will be items you can use in battle.");
				}else if (selected==3){
					// Run
					place=0;
					break;
				}
			}
		}else if (battleStatus==BATTLESTATUS_CHOOSINGTARGET)/*Choosing target*/{
			char _actionQueue=0;
			
			#if PLATFORM == PLAT_COMPUTER
				if (wasJustPressed(SCE_TOUCH)){
					if (FixTouchY(touchY)>SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE){
						if ( FixTouchX(touchX)>(SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2 ){
							int i;
							for (i=3;i!=-1;i--){
								if  (FixTouchX(touchX)>=((SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2)+getTextureWidth(attackButton)*SELECTIONBUTTONSCALE*i){
									switch(i){
										case 0:
											_actionQueue = QUEUE_LEFT;
											break;
										case 1:
											_actionQueue = QUEUE_X;
											break;
										case 2:
											_actionQueue = QUEUE_O;
											break;
										case 3:
											_actionQueue = QUEUE_RIGHT;
											break;
										default:
											printf("Invalid i value %d\n",i);
											break;
									}
									break;
								}
							}
						}
					}
				}
			#endif

			if (wasJustPressed(SCE_CTRL_RIGHT) || wasJustPressed(SCE_CTRL_DOWN)){
				_actionQueue = QUEUE_RIGHT;
			}
			if (wasJustPressed(SCE_CTRL_LEFT) || wasJustPressed(SCE_CTRL_UP)){
				_actionQueue = QUEUE_LEFT;
			}
			if (wasJustPressed(aButton)){
				_actionQueue = QUEUE_X;
			}
			if (wasJustPressed(bButton)){
				_actionQueue = QUEUE_O;
			}

			if (_actionQueue==QUEUE_RIGHT){
				PlaySoftMenuSoundEffect();
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
			if (_actionQueue==QUEUE_LEFT){
				PlaySoftMenuSoundEffect();
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
			if (_actionQueue==QUEUE_X){
				PlayMenuSoundEffect();
				GetBattlerById(orderOfAction[currentOrder])->x=originalX;
				GetBattlerById(orderOfAction[currentOrder])->y=originalY;

				// Chose target for spell
				if (temp==7){
					temp=0;
					ResetAnimation(&(selectedSpell->theAnimation));
					battleStatus=BATTLESTATUS_SPELLANIMATION;
					int tempDone = Damage(GetBattlerById(orderOfAction[currentOrder]),GetBattlerById(target),selectedSpell->attack,selectedSpell->magicAttack,selectedSpell->spellSpecialProperty);
					if (tempDone<0){
						tempDone*=-1; // Multiply it by -1 so I can easily remove the minus sign
						sprintf((char*)&temp2,"%d",tempDone);
						tempDone*=-1; // Fix it
					}else{
						sprintf((char*)&temp2,"%d",tempDone);
					}
					GetBattlerById(orderOfAction[currentOrder])->mp -= selectedSpell->mpCost;
					temp3=tempDone;
				}else{ // Chose target for normal attack
					battleStatus=BATTLESTATUS_MOVETOTARGET;
					originalY=GetBattlerById(orderOfAction[currentOrder])->y;
					originalX=GetBattlerById(orderOfAction[currentOrder])->x;
					
					moveXPerFrame=3;
					moveXPerFrame=floor((GetBattlerById(target)->x - GetBattlerById(orderOfAction[currentOrder])->x - GetBattlerAnimationById(orderOfAction[currentOrder],ANIMATION_ATTACK)->width*BATTLEENTITYSCALE+32 )/30);
					moveYPerFrame=floor((GetBattlerById(target)->y - GetBattlerById(orderOfAction[currentOrder])->y + GetBattlerAnimationById(target,ANIMATION_IDLE)->height/4)/30);
	
					if (moveXPerFrame==0){
						moveXPerFrame=ceil((GetBattlerById(target)->x - GetBattlerById(orderOfAction[currentOrder])->x - GetBattlerAnimationById(orderOfAction[currentOrder],ANIMATION_ATTACK)->width*BATTLEENTITYSCALE+32 )/30);
					}
					if (moveYPerFrame==0){
						moveYPerFrame=ceil((GetBattlerById(target)->y - GetBattlerById(orderOfAction[currentOrder])->y + GetBattlerAnimationById(target,ANIMATION_IDLE)->height/4)/30);
					}
				}

			}else if (_actionQueue==QUEUE_O){
				battleStatus=BATTLESTATUS_CHOOSINGMOVE;
			}
		}else if (battleStatus==BATTLESTATUS_MOVETOTARGET)/*Moving to  target*/{
			GetBattlerById(orderOfAction[currentOrder])->x=GetBattlerById(orderOfAction[currentOrder])->x+moveXPerFrame;
			GetBattlerById(orderOfAction[currentOrder])->y=GetBattlerById(orderOfAction[currentOrder])->y+moveYPerFrame;
			
			if (moveXPerFrame>0 && GetBattlerById(orderOfAction[currentOrder])->x>=GetBattlerById(target)->x-GetBattlerAnimationById(orderOfAction[currentOrder],ANIMATION_ATTACK)->width*BATTLEENTITYSCALE){
				moveXPerFrame=0;
			}
			if (moveXPerFrame<0 && GetBattlerById(orderOfAction[currentOrder])->x<=GetBattlerById(target)->x+GetBattlerAnimationById(target,ANIMATION_IDLE)->width*BATTLEENTITYSCALE){
				moveXPerFrame=GetBattlerById(target)->x+GetBattlerAnimationById(target,ANIMATION_IDLE)->width*BATTLEENTITYSCALE;
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

				// Store the damage as a string into temp2
				// Negative damage will not have the string start with a negative sign
				if (temp<0){
					temp*=-1; // Multiply it by -1 so I can easily remove the minus sign
					sprintf((char*)&temp2,"%d",temp);
					temp*=-1; // Fix it
				}else{
					sprintf((char*)&temp2,"%d",temp);
				}
				temp3=temp;

				battleStatus=BATTLESTATUS_MOVEBACKFROMTARGET;
			}
		}else if (battleStatus==BATTLESTATUS_MOVEBACKFROMTARGET)/*Moving back from target*/{
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
				battleStatus=BATTLESTATUS_NEXTTURN;
				// Go to the next turn.
				currentOrder++;
			}
		}else if (battleStatus==BATTLESTATUS_SPELLANIMATION)/*Waiting for spell animation*/{
			UpdateAnimationIfNeed(&(selectedSpell->theAnimation));
			if (selectedSpell->theAnimation.currentFrame==0 && temp!=0){
				currentOrder++;
				battleStatus=BATTLESTATUS_NEXTTURN;
			}
		}

		startDrawing();
		DrawMap();
		DrawUnusedAreaRect();

		// Draw selection buttons
		if (battleStatus==BATTLESTATUS_CHOOSINGMOVE){
			// Draw later on the bottom screen if on 3ds
			#if PLATFORM != PLAT_3DS
				// Draw UI and selector
				drawTextureScale(attackButton,(SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2,SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE);
				drawTextureScale(magicButton,((SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2)+getTextureWidth(attackButton)*SELECTIONBUTTONSCALE,SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE);
				drawTextureScale(itemButton,((SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2)+getTextureWidth(attackButton)*SELECTIONBUTTONSCALE*2,SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE);
				
				if (canRun==1){
					drawTextureScale(runButton,((SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2)+getTextureWidth(attackButton)*SELECTIONBUTTONSCALE*3,SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE);
				}
				//UpdateAnimationIfNeed(&selectorAnimation);
				//drawTexturePartScaleRotate(selectorAnimation.texture,selected*200+selected*32+132,SCREENHEIGHT-128,selectorAnimation.currentFrame*selectorAnimation.width,0,selectorAnimation.width,selectorAnimation.height,3.7,3.7,1.57);
				drawTextureScale(iconHighlightTexture,((SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2)+(getTextureWidth(attackButton)*SELECTIONBUTTONSCALE*(selected)),SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE);
			#endif
		}

		#if PLATFORM == PLAT_COMPUTER
			if (battleStatus==BATTLESTATUS_CHOOSINGTARGET){
				drawTextureScale(leftButton,(SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2,SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE);
				drawTextureScale(selectButton,((SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2)+getTextureWidth(attackButton)*SELECTIONBUTTONSCALE,SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE);
				drawTextureScale(backButton,((SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2)+getTextureWidth(attackButton)*SELECTIONBUTTONSCALE*2,SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE);
				drawTextureScale(rightButton,((SCREENWIDTH-(getTextureWidth(attackButton)*4*SELECTIONBUTTONSCALE))/2)+getTextureWidth(attackButton)*SELECTIONBUTTONSCALE*3,SCREENHEIGHT-getTextureHeight(attackButton)*SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE,SELECTIONBUTTONSCALE);
			}
		#endif

		// Draw the party members and the party member hp and mp
		int _lastPartyUIX=32;
		for (i=0;i<partySize;i++){

			if (party[i].hp==0){
				continue;
			}

			#if PLATFORM==PLAT_COMPUTER || PLATFORM==PLAT_VITA
				// Draw health and MP bar for party member

				if (i!=0){
					if (textWidth(fontSize,party[i-1].fighterStats.name)+textWidth(fontSize, " ")>144){
						_lastPartyUIX+=textWidth(fontSize,party[i-1].fighterStats.name)+textWidth(fontSize, " ");
					}else{
						_lastPartyUIX+=144;
					}
				}

				// hp
				drawRectangle(_lastPartyUIX,16,128,32,0,0,0,255);
				drawRectangle(_lastPartyUIX,16,floor(128*(((double)party[i].hp)/party[i].fighterStats.maxHp)),32,party[i].hp>=party[i].fighterStats.maxHp+15 ? 255: 190,0,0,255);
				// mp
				drawRectangle(_lastPartyUIX,48,128,32,0,0,0,255);
				drawRectangle(_lastPartyUIX,48,floor(128*(((double)party[i].mp)/party[i].fighterStats.maxMp)),32,0,0,190,255);
				// name
				goodDrawText(_lastPartyUIX,90,party[i].fighterStats.name,2);
			#endif
			// Draws attack animation for person moving to attack
			if (battleStatus==BATTLESTATUS_MOVETOTARGET){
				if (i+1==orderOfAction[currentOrder]){
					DrawAnimationAsISay(&(partyAttackAnimation[i]),party[i].x,party[i].y,BATTLEENTITYSCALE);
					continue;
				}
			}else if (battleStatus==BATTLESTATUS_CHOOSINGMOVE || battleStatus==BATTLESTATUS_CHOOSINGTARGET){
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
		// Draw the enemies
		for (i=0;i<numberOfEnemies;i++){
			if (enemies[i].hp==0){
				continue;
			}

			if (battleStatus==BATTLESTATUS_MOVETOTARGET){
				if (i+5==orderOfAction[currentOrder]){
					DrawAnimationAsISay((enemyAttackAnimation[i]),enemies[i].x,enemies[i].y,BATTLEENTITYSCALE);
					continue;
				}
			}
			DrawAnimationAsISay(enemyIdleAnimation[i],enemies[i].x,enemies[i].y,BATTLEENTITYSCALE);
		}

		if (battleStatus==BATTLESTATUS_SPELLANIMATION){// SPELL
			temp=selectedSpell->theAnimation.currentFrame;
			DrawAnimationAsISay(&(selectedSpell->theAnimation),GetBattlerById(target)->x,GetBattlerById(target)->y,SPELLSCALE);
			DrawDamageText(target,temp3,temp2);
		}else if (battleStatus==BATTLESTATUS_MOVEBACKFROMTARGET){ // Moving back from the attack. Draw the resulting damage
			DrawDamageText(target,temp3,temp2);
		}else if (battleStatus==BATTLESTATUS_CHOOSINGTARGET){
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
			drawRectangle(GetBattlerById(target)->x,GetBattlerById(target)->y-32,64*BATTLEENTITYSCALE,32,0,0,0,255);
			drawRectangle(GetBattlerById(target)->x,GetBattlerById(target)->y-32,floor(64*BATTLEENTITYSCALE*(((double)GetBattlerById(target)->hp)/GetBattlerById(target)->fighterStats.maxHp)),32,190,0,0,255);

			// Draw the target selector
			DrawAnimationAsISay(&selectorAnimation,GetBattlerById(target)->x-selectorAnimation.width*UISCALE,GetBattlerAnimationById(target,ANIMATION_IDLE)->height*BATTLEENTITYSCALE/2+GetBattlerById(target)->y-selectorAnimation.height*UISCALE/2,UISCALE); // Draw cursor // Draw selector
		}
		endDrawing();
	
		// Draw bottom screen stuff on 3ds
		#if PLATFORM == PLAT_3DS
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			if (battleStatus==BATTLESTATUS_CHOOSINGMOVE){
				// Draw UI and selector
				drawTextureScale(attackButton,100,48,1,1);
				drawTextureScale(magicButton,100,112,1,1);
				if (canRun==1){
					drawTextureScale(runButton,100,176,1,1);
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
					drawRectangle((i+1)*10+i*70,2,70,15,0,0,0,255);
					if (party[i].hp>=party[i].fighterStats.maxHp+15){
						// Draw more red rectangle if overheal
						drawRectangle((i+1)*10+i*70,2,floor(70*(((double)party[i].hp)/party[i].fighterStats.maxHp)),2150,0,0,0,255);
					}else{
						// Draw normal health red
						drawRectangle((i+1)*10+i*70,2,floor(70*(((double)party[i].hp)/party[i].fighterStats.maxHp)),15,190,0,0,255);
					}
					// mp
					drawRectangle((i+1)*10+i*70,17,70,15,0,0,0,255);
					drawRectangle((i+1)*10+i*70,17,floor(70*(((double)party[i].mp)/party[i].fighterStats.maxMp)),15,0,0,190,255);
					// name
					goodDrawText((i+1)*10+i*70,34,party[i].fighterStats.name,1);
				}

			}
			sf2d_end_frame();
			sf2d_swapbuffers();
		#endif

		controlsEnd();
		FpsCapWait();
	}
	
	freeTexture(attackButton);
	freeTexture(magicButton);
	freeTexture(itemButton);
	freeTexture(runButton);
	freeTexture(winTexture);
	DisposeAllLoadedSpellImages();

	ClearBottomScreen();

	// Do stuff if the player lost the game. show message and return 0 if they lost
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

			fixPath("Stuff/Maps/NathansHouse",tempPathFixBuffer,TYPE_EMBEDDED);
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

// Press back 3 times and tap in top left
// Hold up and O as you press X
void TitleLoop(){
	#if PLATFORM != PLAT_COMPUTER
		CrossTexture* titleImage = LoadEmbeddedPNG("Stuff/Title.png");
	#else
		CrossTexture* titleImage = LoadEmbeddedPNG("Stuff/TitleAndroid.png");
	#endif
	char _actionQueue=0;
	char _didSecretCode=0;
	while (place!=2){
		StartFrameStuff();

		#if PLATFORM == PLAT_COMPUTER
			if (wasJustPressed(SCE_ANDROID_BACK)){
				if (_didSecretCode==0){
					_didSecretCode=2;
				}else{
					_didSecretCode++;
				}
				if (_didSecretCode==5){
					place=PLACE_QUIT;
					break;
				}
			}
			// 445
			// 74
			if (wasJustPressed(SCE_TOUCH)){
				if (FixTouchX(touchX)<100 && FixTouchY(touchY)<100){
					if (_didSecretCode==4){
						_didSecretCode=1;
						_actionQueue=QUEUE_X;
					}
				}
				if (FixTouchY(touchY)>352 && FixTouchY(touchY)<471){
					if (FixTouchX(touchX)>8 && FixTouchX(touchX)<222){
						_actionQueue=QUEUE_X;
					}
					if (FixTouchX(touchX)>233 && FixTouchX(touchX)<447){
						if (DoesSavefileExist()==1){
							RunHappyLua();
							Load();
							_actionQueue=QUEUE_LEFT;
						}else{
							_actionQueue=QUEUE_X;
						}
					}
				}
				if (FixTouchX(touchX)>445 && FixTouchY(touchY)<74){
					CreditsView();
					controlsResetEmpty();
				}
			}
		#endif

		if (wasJustPressed(aButton)){
			// TODO - Crash on NIntendo Switch after this
			if (isDown(SCE_CTRL_UP)==1 && isDown(SCE_CTRL_CROSS)==1 && isDown(SCE_CTRL_CIRCLE)==1){
				_didSecretCode=1;
			}
			_actionQueue = QUEUE_X;
		}

		if (_actionQueue==QUEUE_LEFT){
			PlayMenuSoundEffect();
			controlsEnd();
			SetupHardcodedLanguage();
			place=0;
			ClearBottomScreen();
			break;
		}

		if (_actionQueue==QUEUE_X){
			PlayMenuSoundEffect();
			controlsEnd();
			SetupHardcodedLanguage();
			RunHappyLua();
			if (_didSecretCode==1){
				BasicMessage("You activated the top secret key combo. You will get a high level. Restart the game to play normally");
				party[0].fighterStats.maxHp=999;
				party[0].fighterStats.speed=999;
				party[0].fighterStats.attack=999;
				party[0].fighterStats.defence=999;
				party[0].fighterStats.magicDefence=999;
				party[0].fighterStats.level=100;
			}
			
			fixPath(STARTINGMAP,tempPathFixBuffer,TYPE_EMBEDDED);
			LoadMap(tempPathFixBuffer);
			place=0;	
			
			ClearBottomScreen();
			break;
		}
		if (wasJustPressed(SCE_CTRL_SQUARE)){
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

		startDrawing();

		//DrawTexture(titleImage,0,0);
		drawTextureScale(titleImage,0,0,(float)SCREENWIDTH/getTextureWidth(titleImage),(float)SCREENHEIGHT/getTextureHeight(titleImage));
		//DrawUnusedAreaRect();
		#if PLATFORM != PLAT_COMPUTER
			#if PLATFORM != PLAT_3DS
				if (aButton==SCE_CTRL_CROSS){
					goodDrawText(51+8,SCREENHEIGHT-currentTextHeight,"Select Button: X",3.5);
				}else{
					goodDrawText(51+8,SCREENHEIGHT-currentTextHeight,"Select Button: O",3.5);
				}
			#elif PLATFORM == PLAT_3DS
				endDrawing();
				sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
				if (LANGUAGE==LANG_ENGLISH){
					goodDrawText(0,0,"X: Espa'nol",2);
				}else if (LANGUAGE==LANG_SPANISH){
					goodDrawText(0,0,"X: English",2);
				}
			#endif
		#endif

		endDrawing();
		
		EndFrameStuff();
	}
	freeTexture(titleImage);
}

/*
///////////////////////////////////////
// LUA FUNCTIONS
///////////////////////////////////////
*/
// I'll put my header files wherever I want, thank you very much.
#include "LuaFunctions.h"
/*
///////////////////////////////////////
// LUA FUNCTIONS END
///////////////////////////////////////
*/

void Init(){
	generalGoodInit();
	// Good stuff
	spellLinkedListStart=calloc(1,sizeof(spellLinkedList));
	// Variables for splash
	int _screenWidthReal=0;
	int _screenHeightReal=0;
	#if PLATFORM == PLAT_VITA
		_screenWidthReal=960;
		_screenHeightReal=544;
		// Init vita2d and set its clear color.
		vita2d_init();
		vita2d_set_clear_color(RGBA8(212, 208, 200, 0xFF));
		// 0 this out so we start the game without having pushed any buttons
		memset(&pad, 0, sizeof(pad));
	#elif PLATFORM == PLAT_COMPUTER
		SDL_Init(SDL_INIT_VIDEO);
		#if SUBPLATFORM == SUB_ANDROID
			// For knowing screen resolution and stuff.
			SDL_DisplayMode displayMode;
			if( SDL_GetCurrentDisplayMode( 0, &displayMode ) == 0 ){
				mainWindow = SDL_CreateWindow( "HappyWindo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, displayMode.w, displayMode.h, SDL_WINDOW_SHOWN );
				screenWidth=displayMode.w;
				screenHeight=displayMode.h;
			}else{
				printf("Failed to get display mode....\n");
			}
			_screenWidthReal = SCREENWIDTH;
			_screenHeightReal = SCREENHEIGHT;
		#else
			mainWindow = SDL_CreateWindow( "HappyWindo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_SHOWN );
			//ShowErrorIfNull(mainWindow);
			//screenWidth=1024;
			//screenHeight=736;
			SDL_DisplayMode displayMode;
			displayMode.w=SCREENWIDTH;
			displayMode.h=SCREENHEIGHT;

			_screenWidthReal = SCREENWIDTH;
			_screenHeightReal = SCREENHEIGHT;
		#endif
		if (USEVSYNC){
			mainWindowRenderer = SDL_CreateRenderer( mainWindow, -1, SDL_RENDERER_PRESENTVSYNC);
		}else{
			mainWindowRenderer = SDL_CreateRenderer( mainWindow, -1, SDL_RENDERER_ACCELERATED);
		}
		ShowErrorIfNull(mainWindowRenderer);

		// This is the default background color.
		//SDL_SetRenderDrawColor( mainWindowRenderer, 212, 208, 200, 255 );
		// Check if this fails?
		IMG_Init( IMG_INIT_PNG );

		// We need to set this because SDL_FontCache will change it if we don't.
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	#elif PLATFORM == PLAT_3DS
		sf2d_init();
		sf2d_set_clear_color(RGBA8(212, 208, 200, 0xFF));
	#elif PLATFORM == PLAT_SWITCH
		initGraphics(640,480, &_screenWidthReal, &_screenHeightReal);
	#endif

	#if SHOWSPLASH==1
			#if PLATFORM == PLAT_VITA
				CrossTexture* happy = LoadEmbeddedPNG("OtherStuff/Splash.png");
			#else
				if (_screenWidthReal==0 || _screenHeightReal==0){
					_screenWidthReal=SCREENWIDTH;
					_screenHeightReal=SCREENHEIGHT;
				}
				CrossTexture* happy = LoadEmbeddedPNG("Stuff/AndroidSplash.png");
			#endif

			startDrawing();
			drawTextureScale(happy,0,0,(double)_screenWidthReal/getTextureWidth(happy),(double)_screenHeightReal/getTextureHeight(happy));
			endDrawing();
			
			StartFrameStuff();
			//sceCtrlPeekBufferPositive(0, &pad, 1);
			//#if PLATFORM == PLAT_VITA
				if (!(isDown(SCE_CTRL_LTRIGGER))){
					wait(700);
				}
			//#endif
			
			freeTexture(happy);
	#endif


	initAudio();
	makeDataDirectory();
	loadFont("TODO - Make font filename");

	InitGoodArray(&tileOtherData);
	int i=0;
	for (i=0;i<5;i++){
		InitGoodArray(&(layers[i]));
	}

	currentMapFilepath = malloc(1);
	currentMapFilepath[0]=0;

	// Init Lua
	L = luaL_newstate();
	luaL_openlibs(L);
	PushCFunctions();

	// Set platform lua variable
	lua_pushnumber(L,PLATFORM);
	lua_setglobal(L,"Platform");
	lua_pushnumber(L,SUBPLATFORM);
	lua_setglobal(L,"Subplatform");
	lua_pushstring(L,ANDROIDPACKAGENAME);
	lua_setglobal(L,"ANDROIDPACKAGENAME");

	// Set all enemy pointers to null
	for (i=0;i<4;i++){
		enemies[i].hp=-1;
		enemyIdleAnimation[i]=NULL;
		enemyAttackAnimation[i]=NULL;
		// -1 hp means that the slot isn't used.
		party[i].hp=-1;
		party[i].fighterStats.maxHp=-1;
	}
	for (i=0;i<10;i++){
		// -1 hp means that the slot isn't used.
		possibleEnemies[i].maxHp=-1;
	}

	playerDown=LoadEmbeddedPNG("Stuff/PlayerDown.png");
	playerUp=LoadEmbeddedPNG("Stuff/PlayerUp.png");
	playerLeft=LoadEmbeddedPNG("Stuff/PlayerLeft.png");
	playerRight=LoadEmbeddedPNG("Stuff/PlayerRight.png");
	selectorAnimation.texture=LoadEmbeddedPNG("Stuff/Selector.png");
	pauseButtonTexture = LoadEmbeddedPNG("Stuff/PauseButton.png");

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

	place=5;

	pathfindingPath = malloc(1);
	foundPathLength=-1;

	// Init pathfinding list.
	LegfinderInit();

	#if ANDROIDTEST==1
		fontSize=50;
		LoadFont();
	#endif

	#if PLATFORM == PLAT_COMPUTER
		double foundRatio;
		if (FitToWidth(displayMode.w,displayMode.h)==1){
			foundRatio = ((double)displayMode.w)/SCREENWIDTH;
			//printf("Fit to width %f\n",foundRatio);
		}else{
			// 736/800 = .92
			foundRatio = (double)displayMode.h/SCREENHEIGHT;
			//printf("Fit to height %f\n",(double)displayMode.h/SCREENHEIGHT);
		}
		androidXScale = foundRatio;
		androidYScale = foundRatio;
		
		// 320
		drawXOffset+=((displayMode.w)-SCREENWIDTH*androidXScale)/2;
		// -32
		drawYOffset+=((displayMode.h)-SCREENHEIGHT*androidYScale)/2;
		
		realDrawYOffset = drawYOffset;
		realDrawXOffset = drawXOffset;
	
		drawXOffset/=androidXScale;
		drawYOffset/=androidYScale;
		//drawXOffset=0;
		//drawYOffset=0;
	
		// 1024 - nexus 7
		// 736 - nexus 7
		/*
		640 - internal resolution
		512 - internal resolution
		*/
	
		SDL_RenderSetScale(mainWindowRenderer,androidXScale,androidYScale);
		//printf("%.6fx%.6f is scale.",androidXScale,androidYScale);
		//printf("%d,%d is offset\n",drawXOffset,drawYOffset);
	#else
		// drawXOffset and drawYOffset are already set from SetCameraValues
		androidXScale=1;
		androidYScale=1;
		realDrawXOffset = drawXOffset;
		realDrawYOffset = drawYOffset;
	#endif

	setClearColor(0,0,0,255);
	fixPath("Stuff/Sound/Select.wav",tempPathFixBuffer,TYPE_EMBEDDED);
	selectSoundEffect = loadSound((char*)tempPathFixBuffer);
	fixPath("Stuff/Sound/SelectSoft.wav",tempPathFixBuffer,TYPE_EMBEDDED);
	softSelectSoundEffect = loadSound((char*)tempPathFixBuffer);
}


#if RENDERER == REND_SDL
	extern SDL_Window* mainWindow;
	extern SDL_Renderer* mainWindowRenderer;
#endif
// May not actually quit out of the program. You still need to return from the main function
void Quit(lua_State* L){
	lua_close(L);
	#if RENDERER == REND_VITA2D
		vita2d_fini();
	#elif RENDERER == REND_SDL
		//Destroy window
		SDL_DestroyRenderer( mainWindowRenderer );
		SDL_DestroyWindow( mainWindow );
		mainWindow = NULL;
		mainWindowRenderer = NULL;
		// QUit SDL subsystems
		IMG_Quit();
	#elif RENDERER == REND_SF2D
		sf2d_fini();
	#endif
	#if SOUNDPLAYER == SND_SDL
		// TODO sdl mixer quit
	#endif
	#if PLATFORM == PLAT_COMPUTER
		#if RENDERER == REND_SDL
			SDL_Quit();
		#else
			printf("No quit function avalible for Windows without SDL.\n");
		#endif
	#elif PLATFORM == PLAT_VITA
		sceKernelExitProcess(0);
	#elif PLATFORM == PLAT_3DS
		// Nothing needed for 3ds?
	#endif
}

int main(int argc, char *argv[]){
	srand(time(NULL));
	Init();	

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

	Quit(L);
	return 0;
}
