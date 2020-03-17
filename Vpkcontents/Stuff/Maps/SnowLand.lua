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
	Event09=nil;
	snowAX = nil;
	snowAY = nil;
	snowBX = nil;
	snowBY = nil;
	snowCX = nil;
	snowCY = nil;
	placeSnowmanMaybe=nil;
	snowPosOverlap=nil;
	pushSnowAt=nil;
	setupSnowmanStats=nil;
	snowBlobTalkCount=nil;
	didInitiallyPlaceUseful=nil;
end

function Event01()
   if (didInitiallyPlaceUseful==0 and flags[7]==1 and GetLevel(0)>=30) then
	  didInitiallyPlaceUseful=1;
	  SetMapImageData(10,8,1,0,4);
	  SetMapOtherData(10,8,true,9);
	  SetPlayerPosition(7,1);
   else
	  SetPlayerPosition(13,14);
	  ChangeMap(FixString("Stuff/Maps/StartTown"));
   end
end
function Event02()
   if (snowBlobTalkCount==30) then
	  SetMapImageData(4,8,1,0,0);
	  SetMapOtherData(4,8,false,0);
		-- START BATTLE
			WierdSlimeHybridMember = Malloc(true,2);
			
			enemyidle0 = Malloc(true,0);
			enemyatk0 = Malloc(true,0);
			SetAnimation(enemyidle0,10,32,32,1,false,0,0,LoadPNG(FixString("Stuff/Enemies/blob.png")));
			SetAnimation(enemyatk0,10,32,32,1,false,0,0,LoadPNG(FixString("Stuff/Enemies/blob.png")));

			WierdSlimeHybridStats = GetPartyMemberStats(WierdSlimeHybridMember);
			-- nep style boss
			SetStats(WierdSlimeHybridStats,2000,2000,2000,2000,2000,2000,2000,2000,65535);
			SetStatsSpells(WierdSlimeHybridMember,1,2,3,4,5);
			
			didWin = StartSpecificBattle(1,WierdSlimeHybridMember,enemyidle0,enemyatk0);
			FreeAnimationImage(enemyidle0);
			FreeAnimationImage(enemyatk0);
			Free(enemyidle0);
			Free(enemyatk0);
			Free(WierdSlimeHybridMember);			
			WierdSlimeHybridMember=nil;
			WierdSlimeHybridStats=nil;
			enemyidle0 = nil;
			enemyatk0 = nil;
			RestoreEntireParty();

		if (didWin==true) then
		   flags[9]=1;
		else
		   SetPlayerPosition(13,14);
		   ChangeMap(FixString("Stuff/Maps/StartTown"));
		end
   else
	  snowBlobTalkCount=snowBlobTalkCount+1;
	  tempPort = LoadPNG(FixString("Stuff/Portraits/Lump.png"));
	  if (Lang==1) then
		 if (snowBlobTalkCount==30) then
			ShowMessageWithPortrait("Leave me alone.",false,tempPort,0);
		 else
			ShowMessageWithPortrait("It's a useless pile of snow clearly only there to make the area seem less empty.",false,tempPort,0);
		 end
	  elseif (Lang==2) then
		 if (snowBlobTalkCount==30) then
			ShowMessageWithPortrait("Voy a matar tu.",false,tempPort,0);
		 else
			ShowMessageWithPortrait("Esta es nieve in'util.",false,tempPort,0);
		 end
	  end

	  UnloadTexture(tempPort);
	  tempPort=nil;
   end
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
   local _willKill;
   if (Lang==1) then
	  _willKill = ShowMessage("Kill?",true);
   else
	  _willKill = ShowMessage("Matar?",true);
   end
   if (_willKill) then
		SetMapImageData(snowBX,snowBY,1,0,0);
		SetMapOtherData(snowBX,snowBY,false,0);
		-- START BATTLE
			WierdSlimeHybridMember = Malloc(true,2);
			WierdSlimeHybridStats = GetPartyMemberStats(WierdSlimeHybridMember);
			SetStats(WierdSlimeHybridStats,255,200,255,62,14,50,50,20,150);
			
			enemyidle0 = Malloc(true,0);
			enemyatk0 = Malloc(true,0);
			SetAnimation(enemyidle0,10,50,50,3,false,0,0,LoadPNG(FixString("Stuff/Enemies/Snowman.png")));
			SetAnimation(enemyatk0,5,50,55,6,false,0,0,LoadPNG(FixString("Stuff/Enemies/SnowmanAttack.png")));

			SetStatsSpells(WierdSlimeHybridMember,1,5);
			
			didWin = StartSpecificBattle(1,WierdSlimeHybridMember,enemyidle0,enemyatk0);
			FreeAnimationImage(enemyidle0);
			FreeAnimationImage(enemyatk0);
			Free(enemyidle0);
			Free(enemyatk0);
			Free(WierdSlimeHybridMember);			
			WierdSlimeHybridMember=nil;
			WierdSlimeHybridStats=nil;
			enemyidle0 = nil;
			enemyatk0 = nil;
			RestoreEntireParty();
			
		-- if lose
		if (didWin==false) then
		   placeSnowmanMaybe();
		   if (Lang==1) then
			  ShowMessage("You're a bully. I'm going home.",false);
		   else
			  ShowMessage("Eres antipatico. Voy a ir a mi casa.",false);
		   end
		   SetMapImageData(snowBX,snowBY,1,0,0);
		   SetMapOtherData(snowBX,snowBY,false,0);
		   flags[8]=2;
		else
		   ShowMessage("(The local snow men don't like you even more.)",false);
		   setupSnowmanStats();
		   flags[8]=1;
		end
		didWin=nil;
   else
	  if (Lang==1) then
		 _willKill = ShowMessage("Friend?",true);
	  else
		 _willKill = ShowMessage("Amigo?",true);
	  end
	  if (_willKill==true) then
		 flags[8]=3;
		 SetMapImageData(snowBX,snowBY,1,0,0);
		 SetMapOtherData(snowBX,snowBY,false,0);
	  else
		 return;
	  end
   end
end
function Event09()
   -- learn big heal
   if (Lang==1) then
	  ShowMessage("I'm Actually Useful Pile of Snow. I'm here to teach you how to use 'Big Heal'.", false);
	  _willLearn = ShowMessage("Learn it?", true);
   else
	  ShowMessage("Yo soy la nieve 'util", false);
	  _willLearn = ShowMessage("Quieres ense'nar \"Curar Grande?\"", true);
   end
   if (_willLearn==true) then
	  for i=0,GetPartySize()-1 do
		 AddSpellToStats(GetPartyMemberStats(i),6);
	  end
	  HideTile(10,8,1);
   end
   _willLearn=nil;
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


snowBlobTalkCount=0;
if (flags[9]==1) then
   SetMapImageData(4,8,1,0,0);
   SetMapOtherData(4,8,false,0);
end

didInitiallyPlaceUseful=0;
if (GetLevel(0)>=30) then
   if (flags[7]==1) then
	  didInitiallyPlaceUseful=1;
	  if (flags[10]==0) then
		 SetMapImageData(10,8,1,0,4);
		 SetMapOtherData(10,8,true,9);
	  end
   end
end

--
battleEnemyLoadId=0;
dofile(FixString("Stuff/BattleLua/Snowman.lua"));
--
SetEncounterRate(8);

if (flags[8]==0 and flags[7]==0) then
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
