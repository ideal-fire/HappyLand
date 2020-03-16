function MapDispose()
	FreeTileset(0);
	Event01=nil;
	Event02=nil;
	Event03=nil;
	Event04=nil;
	Event05=nil;
	Event06=nil;
	Event07=nil;
	Event08=nil;
	snowAX = nil;
	snowAY = nil;
	snowBX = nil;
	snowBY = nil;
	snowCX = nil;
	snowCY = nil;
	placeSnowmanMaybe=nil;
	snowPosOverlap=nil;
	pushSnowAt=nil;
end

function Event01()
	SetPlayerPosition(13,14);
	ChangeMap(FixString("Stuff/Maps/StartTown"));
end
function Event02()
	tempPort = LoadPNG(FixString("Stuff/Portraits/Lump.png"));

	if (Lang==1) then
		ShowMessageWithPortrait("It's a useless pile of snow clearly only there to make the area seem less empty.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Esta es nieve in'util.",false,tempPort,0);
	end

	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event03()
	tempPort = LoadPNG(FixString("Stuff/Portraits/Sign.png"));

	if (Lang==1) then
	   if (flags[7]==1) then
		  ShowMessageWithPortrait("Go left to get to StartTown.",false,tempPort,0);
	   else
		  ShowMessageWithPortrait("Go up to get to Big Foot Land.",false,tempPort,0);
	   end
	elseif (Lang==2) then
		ShowMessageWithPortrait("Subir para ir a El Mundo de Pie Grande.",false,tempPort,0);
	end

	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event04()
	SetPlayerPosition(7,13);
	ChangeMap(FixString("Stuff/Maps/BigFootChamber"));
end
-- returns new positoin
function pushSnowAt(snowx,snowy,tilenum,eventnum)
   local destx=snowx;
   local desty=snowy;
   
   px, py = getPlayerPos();
   if (py~=snowy) then
	  if (py>snowy) then
		 desty=desty-1;
	  else
		 desty=desty+1;
	  end
   else
	  if (px<snowx) then
		 destx=destx+1;
	  else
		 destx=destx-1;
	  end
   end

   if (destx<0 or destx>14 or desty>14) then
	  print("out of bounds move attempt");
	  return snowx,snowy;
   end
   _,_,destissolid,destevent = GetMap(destx,desty);
   if (destissolid==false and destevent==0) then
	  -- hide it
	  SetMapImageData(snowx,snowy,1,0,0);
	  SetMapOtherData(snowx,snowy,false,0);
	  -- place it
	  SetMapImageData(destx,desty,1,0,tilenum);
	  SetMapOtherData(destx,desty,true,eventnum);
   else
	  print("cant move here");
	  return snowx,snowy;
   end
   destissolid=nil;
   destevent=nil;
   return destx,desty;
end
function Event05()
   snowBX, snowBY = pushSnowAt(snowBX,snowBY,12,5);
   placeSnowmanMaybe();
end
function Event06()
   snowMX, snowMY = pushSnowAt(snowMX,snowMY,13,6);
   placeSnowmanMaybe();
end
function Event07()
   snowSX, snowSY = pushSnowAt(snowSX,snowSY,14,7);
   placeSnowmanMaybe();
end
function Event08()
   ShowMessage("aaaaaaa",false);
   flags[8]=1;
   SetMapImageData(snowBX,snowBY,1,0,0);
   SetMapOtherData(snowBX,snowBY,false,0);
end
function snowPosOverlap(x,y)
   if (x==snowBX and y==snowBY) then
	  return true;
   end
   if (x==snowMX and y==snowMY) then
	  return true;
   end
   if (x==snowSX and y==snowSY) then
	  return true;
   end
   return false;
end
-- place the snowman if we're ready
function placeSnowmanMaybe()
   if (snowSY==snowMY-1 and snowMY==snowBY-1 and snowBX==snowMX and snowMX==snowSX) then
	  SetMapImageData(snowBX,snowBY,1,0,15);
	  SetMapOtherData(snowBX,snowBY,true,8);

	  SetMapImageData(snowMX,snowMY,1,0,0);
	  SetMapOtherData(snowMX,snowMY,false,0);
	  SetMapImageData(snowSX,snowSY,1,0,0);
	  SetMapOtherData(snowSX,snowSY,false,0);
   end
end

tileset0=LoadPNG(FixString("Stuff/Tilesets/SnowyTilesetA.png"));
SetTileset(tileset0,0);
tileset0=nil;


--
battleEnemyLoadId=0;
dofile(FixString("Stuff/BattleLua/Snowman.lua"));
--
SetEncounterRate(99); -- 8

if (flags[8]==0) then
   -- generate random snow positions. x position 1 is reserved for fixed fallback positions
   tempx=math.random(2,13);
   tempy=math.random(7,13);
   if (snowPosOverlap(tempx,tempy)) then
	  tempx=1;
	  tempy=12;
   end
   snowBX=tempx;
   snowBY=tempy;
   --
   tempx=math.random(2,13);
   tempy=math.random(7,13);
   if (snowPosOverlap(tempx,tempy)) then
	  tempx=1;
	  tempy=9;
   end
   snowMX=tempx;
   snowMY=tempy;
   --
   tempx=math.random(2,13);
   tempy=math.random(7,13);
   if (snowPosOverlap(tempx,tempy)) then
	  tempx=1;
	  tempy=12;
   end
   snowSX=tempx;
   snowSY=tempy;
   tempx=nil;
   tempy=nil;
   -- place the snow pieces
   SetMapImageData(snowBX,snowBY,1,0,12);
   SetMapOtherData(snowBX,snowBY,true,5);
   SetMapImageData(snowMX,snowMY,1,0,13);
   SetMapOtherData(snowMX,snowMY,true,6);
   SetMapImageData(snowSX,snowSY,1,0,14);
   SetMapOtherData(snowSX,snowSY,true,7);

   placeSnowmanMaybe();
end

RedrawMap();
if (GetLevel(0)<4) then
	if (Lang==1) then
		ShowMessage("You feel you should fight more noob enemies.",false);
	elseif (Lang==2) then
		ShowMessage("T'u piensas que debes matar m'as enemigos f'aciles.",false)
	end
end
