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

	ShowMessageWithPortrait("The FitnessGram Pacer Test is a multistage aerobic capacity test that progressively gets more difficult as it continues. The 20 meter pacer test will begin in 30 seconds. Line up at the start. The running speed starts slowly, but gets faster each minute after you hear this signal. [beep] A single lap should be completed each time you hear this sound. [ding] Remember to run in a straight line, and run as long as possible. The second time you fail to complete a lap before the sound, your test is over. The test will begin on the word start. On your mark, get ready, start.",false,tempPort,0);

	UnloadTexture(tempPort);
	tempPort=nil;
end


tileset0=LoadPNG(FixString("Stuff/Tilesets/TestLand.png"));
SetTileset(tileset0,0);
tileset0=nil;
