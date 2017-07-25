/*
	9/19/16, MyLegGuy
	The code in this file is under public domain.
	It usually finds a path. It may not be the shortest one.
	//////////////////////////////////////////
	// Example project:
	//////////////////////////////////////////
	// For printf
	#include <stdio.h>
	// For pathfinding
	#include "pathfinding.h"
	
	// You must have this function.
	char IsFree(short x, short y){
		// The entire map is empty, except for the spot (11,10).
		if (x==11 && y==10){
			return 0;
		}
		return 1;
	}
	
	int main(){
		printf("Start test program\n");
		// Call this once
		LegfinderInit();
		// Width and height of the map. These are like array sizes. A width of 12 will go from 0 to 11.
		legfinderMapWidth=12;
		legfinderMapHeight=12;
		// Call this every time you change legfinderMapWidth
		LegfinderFixList();
		
		// A variable to hold the resulting path's length
		short pathLength=999;
	
		// An array. You need to free this when you're done.
		coordinates* pathResultArray;
	
		// The path will start at (0,0) and go to (11,11)
		// The path's length will be stored in a signed short called pathLength
		pathResultArray = FindPath(&pathLength,0,0,11,11);
	
		// Only do stuff if a path is found. If the path length variable is -1, no path was found.
		if (pathLength!=-1){
			printf("Found a path that needs %d movements.\n",pathLength-1); // index 0 of resulting array is just the starting position
			int i;
			for (i = 0; i < pathLength; ++i){ // The first one is the start position and the last one is the end position
				printf("(%d,%d)\n",pathResultArray[i].x,pathResultArray[i].y);
			}
		}else{
			printf("No path found\n");
		}
	}
*/

#ifndef FILE_PATHFINDING_SEEN
#define FILE_PATHFINDING_SEEN

// 1 if the end of the path can be a solid and have a path still found
#define ENDCANBESOLID 1

#include <stdio.h>
// For abs()
#include <stdlib.h>
// For memset
#include <string.h>
// FOR A FAIRLY FERSH ARRAY
#include "GoodArray.h"

// TODO Variable name cleanup. Don't use variables like 'short x' Maybe short _pathfindingX_a
// TODO Do something if an already open node is detected.
// TODO I don't know. Just feels stupid to pass around endx and endy

typedef struct pfn {
   unsigned short parentX;
   unsigned short parentY;
   signed char status;
   unsigned short fScore;
   unsigned short fromStart;
} pathfindingNode;

typedef struct pfnwlocation{
	pathfindingNode theNode;
	unsigned short x;
	unsigned short y;
} pathfindingNodeWithLocation;

typedef struct coordinateshidden{
	short x;
	short y;
} coordinates;

//static pathfindingNode mapPathListHolder[20][20];
static Good2dArray mapPathListHolder={NULL,0,0,0};

// This is a value. If mapPathListHolder has currentListValue in a spot, then
// the spot is open. If mapPathListHolder has negative currentListValue in a spot,
// then the spot is closed. Any other value (including non-0 values) are
// considered not on either list.
// SHOULD NEVER BE >127 or == 0
static signed char currentListValue=1;

static short legfinderMapHeight;
static short legfinderMapWidth;

// Ned to put actual code elsewhere.
// This function should return 1 if the tile at the position isn't solid
char IsFree(short x, short y);

// Same as IsFree, but returns 1 if it's the end spot
char IsFreeEndException(int x, int y, int endX, int endY){
	if (x==endX && y==endY){
		return 1;
	}else{
		return IsFree(x,y);
	}
}

pathfindingNode* EasyGetListHolder(short _x, short _y){
	return (pathfindingNode*)GetGoodArray(&mapPathListHolder,_x,_y);
}

void LegfinderInit(){
	InitGoodArray(&mapPathListHolder);
}

void LegfinderFixList(){
	//void SetGoodArray(Good2dArray* _array, short _width, short _height, short _typeSize){
	SetGoodArray(&mapPathListHolder,legfinderMapWidth,legfinderMapHeight,sizeof(pathfindingNode));
}

signed char GenerateNextListValue(signed char _currentListValue){
	// 126 just to play it safe
	if (_currentListValue+1==126){
		return 1;
	}else{
		return _currentListValue+1;
	}
}

// Calculates H, distance from this point to end.
short CalculateDistanceToEnd(short _x, short _y, short endX, short endY){
	return (abs(_y-endY)+abs(_x-endX));
}

unsigned short CalculateFScore(short _x, short _y, short _g, short endX, short endY){
	return (_g+CalculateDistanceToEnd(_x,_y,endX,endY));
}

void AddToClosedList(short _x, short _y){
	//(*EasyGetListHolder(_x,_y)).status=(currentListValue*-1);
	EasyGetListHolder(_x,_y)->status=(currentListValue*-1);
	//printf("(%d,%d) is closed with parent (%d,%d)\n",_x,_y,EasyGetListHolder(_x,_y)->parentX,EasyGetListHolder(_x,_y)->parentY);
	//printf("(%d,%d) to closed list\n", _x,_y);
}
void AddToOpenList(short _x, short _y, short _xParent, short _yParent, short _fromStart, short endX, short endY){
	((*EasyGetListHolder(_x,_y))).status=currentListValue;
	(*EasyGetListHolder(_x,_y)).parentY=_yParent;
	(*EasyGetListHolder(_x,_y)).parentX=_xParent;
	(*EasyGetListHolder(_x,_y)).fScore=CalculateFScore(_x,_y,_fromStart, endX, endY);
	//printf("(%d,%d) to open list. f:%d and fromstart:%d\n", _x,_y,(*EasyGetListHolder(_x,_y)).fScore,_fromStart);
}

unsigned char GetPositionList(short _x, short _y){
	if ((*EasyGetListHolder(_x,_y)).status==currentListValue){
		return 1;
	}else if ((*EasyGetListHolder(_x,_y)).status==currentListValue*-1){
		return 2;
	}else{
		return 0;
	}
}

void AddToOpenListIfFree(short _x, short _y, short _xParent, short _yParent, short _fromStart, short endX, short endY){
	#if ENDCANBESOLID == 1
		if (IsFreeEndException(_x,_y,endX,endY)){
			AddToOpenList(_x,_y, _xParent, _yParent,_fromStart, endX, endY);
		}
	#else
		if (IsFree(_x,_y,endX,endY)){
			AddToOpenList(_x,_y, _xParent, _yParent,_fromStart, endX, endY);
		}
	#endif
}

// TOOD useless function. PROBABLY WONT EVER BE CALLED!!! idk what 2 do!!!
void HappyDoWithOpen(short _x, short _y, short _fromStart, short _endX, short _endY){
	if ((*EasyGetListHolder(_x,_y)).fromStart>_fromStart+1){
		(*EasyGetListHolder(_x,_y)).fromStart=_fromStart;
		(*EasyGetListHolder(_x,_y)).parentX=_x;
		(*EasyGetListHolder(_x,_y)).parentY=_y;
		(*EasyGetListHolder(_x,_y)).fScore=CalculateFScore(_x,_y,_fromStart+1,_endX,_endY);
	}
}

void AddNeighborsToOpenList(short _x, short _y, short _fromStart, short endX, short endY){
	if (_x!=legfinderMapWidth && (*EasyGetListHolder(_x+1,_y)).status>currentListValue*-1){
		AddToOpenListIfFree(_x+1,_y, _x, _y, _fromStart+1, endX, endY);
	}else if ((*EasyGetListHolder(_x+1,_y)).status==currentListValue){
		HappyDoWithOpen(_x+1,_y,_fromStart, endX, endY);
	}
	if (_x!=0 && (*EasyGetListHolder(_x-1,_y)).status!=currentListValue && (*EasyGetListHolder(_x-1,_y)).status>currentListValue*-1){
		AddToOpenListIfFree(_x-1,_y, _x, _y, _fromStart+1, endX, endY);
	}else if ((*EasyGetListHolder(_x-1,_y)).status==currentListValue){
		HappyDoWithOpen(_x-1,_y,_fromStart, endX, endY);
	}
	if (_y!=legfinderMapHeight && (*EasyGetListHolder(_x,_y+1)).status>currentListValue*-1){
		AddToOpenListIfFree(_x,_y+1, _x, _y, _fromStart+1, endX, endY);
	}else if ((*EasyGetListHolder(_x,_y+1)).status==currentListValue){
		HappyDoWithOpen(_x,_y+1,_fromStart, endX, endY);
	}
	if (_y!=0 && (*EasyGetListHolder(_x,_y-1)).status>currentListValue*-1){
		AddToOpenListIfFree(_x,_y-1, _x, _y, _fromStart+1, endX, endY);
	}else if ((*EasyGetListHolder(_x,_y-1)).status==currentListValue){
		HappyDoWithOpen(_x,_y-1,_fromStart, endX, endY);
	}
}

pathfindingNodeWithLocation GetWithLowestFScore(){
	pathfindingNodeWithLocation _lowestSoFar;
	_lowestSoFar.theNode.fScore=65535;
	short i;
	short j;
	for (j=0;j<legfinderMapHeight;j++){
		for (i=0;i<legfinderMapWidth;i++){
			if ((*EasyGetListHolder(i,j)).status==currentListValue){
				////printf("%d,%d\n",i,j);
				// It's in the open list
				if ((*EasyGetListHolder(i,j)).fScore<_lowestSoFar.theNode.fScore){
					_lowestSoFar.theNode=(*EasyGetListHolder(i,j));
					_lowestSoFar.x=i;
					_lowestSoFar.y=j;
				}
			}
		}
	}
	return _lowestSoFar;
}

void ResetForNewPath(){
	memset(mapPathListHolder.array,0,legfinderMapWidth*legfinderMapHeight*sizeof(pathfindingNode));
}

// Returns malloc'd memory that's an array of the sturct coordinates. Argument is pointer to short that will store the length of the finished. path.
// resultLength pointer you supplied thing will be -1 if no path was found.
coordinates* FindPath(short* resultLength, short startX, short startY, short endX, short endY){
	ResetForNewPath();
	pathfindingNodeWithLocation lastNodeReturned;

	currentListValue = GenerateNextListValue(currentListValue);

	// Adds initial start spot's neighbors to list.
	AddNeighborsToOpenList(startX,startY, 0,endX,endY);
	AddToClosedList(startX,startY);

	//printf("%d x %d y\n",mapPathListHolder[7][11].parentX,mapPathListHolder[7][11].parentY);

	while (1){
		lastNodeReturned = GetWithLowestFScore();

		if (lastNodeReturned.theNode.fScore==65535){
			printf("No path is possible.");
			*resultLength = -1;
			// Should I really do this? I don't really care.
			return (coordinates*)0;
		}

		//printf("Found (%d,%d) with %d\n",lastNodeReturned.x,lastNodeReturned.y,lastNodeReturned.theNode.fScore);

		if (lastNodeReturned.x==endX && lastNodeReturned.y==endY){
			//printf("Done!");
			break;
		}

		AddNeighborsToOpenList(lastNodeReturned.x,lastNodeReturned.y, lastNodeReturned.theNode.fromStart, endX, endY);
		AddToClosedList(lastNodeReturned.x,lastNodeReturned.y);

		//printf("FindSesstion end\n");
	}
	//printf("StartUDES!\n");
	//printf("%d x %d y\n",mapPathListHolder[7][11].parentX,mapPathListHolder[7][11].parentY);

	unsigned short pathLength=1;

	unsigned short _x=endX;
	unsigned short _y=endY;
	unsigned short tempX;
	unsigned short tempY;
	while (1){
		if(_x==startX && _y==startY){
			break;
		}else{
			tempX=EasyGetListHolder(_x,_y)->parentX;
			tempY=EasyGetListHolder(_x,_y)->parentY;
			//printf("(%d,%d) parent (%d,%d),%d\n",_x,_y,tempX,tempY,pathLength+1);
			_x=tempX;
			_y=tempY;
			
			pathLength=pathLength+1;
		}
	}

	_x=endX;
	_y=endY;
	//printf("Bout to malloc\n");
	//coordinates* returnArray = calloc(pathLength,sizeof(coordinates));
	coordinates* returnArray = malloc(sizeof(coordinates)*pathLength);

	short i;
	//printf(" pathLength %d\n",pathLength);
	for (i = pathLength-1; i >= 0; i=i-1)
	{
		returnArray[i].x=_x;
		returnArray[i].y=_y;
		////printf("i:%d, (%d,%d)\n",i,returnArray[i].x,returnArray[i].y);
		if(_x==startX && _y==startY){
			break;
		}else{
			tempX=(*EasyGetListHolder(_x,_y)).parentX;
			tempY=(*EasyGetListHolder(_x,_y)).parentY;
			_x=tempX;
			_y=tempY;
		}
	}

	*resultLength = pathLength;
	
	return returnArray;
}


#endif