function MapDispose()
	FreeTileset(0);
	Event01=nil;
	Event02=nil;
	Event03=nil;
	Event04=nil;
end

function Event01()
	SetPlayerPosition(13,14);
	ChangeMap("app0:Stuff/Maps/StartTown");
end
function Event02()
	tempPort = LoadPNG("app0:Stuff/Portraits/Lump.png");

	if (Lang==1) then
		ShowMessageWithPortrait("It's a useless pile of snow clearly only there to make the area seem less empty.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Esta es nieve in'util.",false,tempPort,0);
	end

	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event03()
	tempPort = LoadPNG("app0:Stuff/Portraits/Sign.png");

	if (Lang==1) then
		ShowMessageWithPortrait("Go up to get to Big Foot Land.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Subir para ir a La Sala De Pie Grande.",false,tempPort,0);
	end

	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event04()
	SetPlayerPosition(7,13);
	ChangeMap("app0:Stuff/Maps/BigFootChamber");
end


tileset0=LoadPNG("app0:Stuff/Tilesets/SnowyTilesetA.png");
SetTileset(tileset0,0);
tileset0=nil;


--
battleEnemyLoadId=0;
dofile("app0:Stuff/BattleLua/Snowman.lua");
--
SetEncounterRate(8);

RedrawMap();
if (GetLevel(0)<4) then
	if (Lang==1) then
		ShowMessage("You feel you should fight more noob enemies.",false);
	elseif (Lang==2) then
		ShowMessage("T'u piensas que debes matar m'as enemigos f'aciles.",false)
	end
end
