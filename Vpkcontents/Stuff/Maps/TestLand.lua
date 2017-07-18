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
	Event10=nil;
	Event11=nil;
end

-- Piece of paper
function Event11()
	tempPort = LoadPNG(FixString("Stuff/Portraits/Note.png"));

	ShowMessageWithPortrait("plz don't sue me",false,tempPort,0);

	UnloadTexture(tempPort);
	tempPort=nil;
end


tileset0=LoadPNG(FixString("Stuff/Tilesets/TestLand.png"));
SetTileset(tileset0,0);
tileset0=nil;
