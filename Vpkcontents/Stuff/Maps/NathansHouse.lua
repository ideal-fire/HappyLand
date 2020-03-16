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
end

function Event01()
	tempPort = LoadPNG(FixString("Stuff/Portraits/Player.png"));

	if (Lang==1) then
		ShowMessageWithPortrait("It's my bed.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Esta es mi cama.",false,tempPort,0);
	end

	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event02()
	tempPort = LoadPNG(FixString("Stuff/Portraits/Player.png"));
	if (Lang==1) then
		ShowMessageWithPortrait("It's my fresh computer. I like to play my favorite game on it, Space Pinball. It came free with the XP version of my operating system. XP standing for " .. '"eXtra exPencive," of course.',false,tempPort,0);
		ShowMessage("(The computer is just a monitor connected to a potato.)",false);
	elseif (Lang==2) then
		-- Does not include XP joke.
		ShowMessageWithPortrait("Esta es mi computadora fantastica. Me gusta jugar videojuegos en esta. Mi videojuego favorito es Pinball De Espacio. Era gratis.",false,tempPort,0);
		ShowMessage("(La computadora es s'olo un monitor que est'a conectado con una papa.)",false);
	end
	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event03()
	tempPort = LoadPNG(FixString("Stuff/Portraits/Sign.png"));
	if (Lang==1) then
		ShowMessageWithPortrait("Note2self: Put more stuff in my house.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Nota personal: '!Pone m'as cosas en mi casa!",false,tempPort,0);
	end
	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event04()
	tempPort = LoadPNG(FixString("Stuff/Portraits/Player.png"));
	if (Lang==1) then
		ShowMessageWithPortrait("I love having useless boxes in my house.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Me encanta cajas in'utiles en mi casa.",false,tempPort,0);
	end
	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event05()
	tempPort = LoadPNG(FixString("Stuff/Portraits/Player.png"));
	tempPort2 = LoadPNG(FixString("Stuff/Portraits/PlayerSad.png"));
	if (Lang==1) then
		ShowMessageWithPortrait("All these chairs are here so I can play musical chairs.",false,tempPort,0);
		ShowMessageWithPortrait("Unfortunately, I have more chairs than friends.",false,tempPort2,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Yo tengo muchas sillas para jugar el juego de sillas musicales con mis amigos.",false,tempPort,0);
		ShowMessageWithPortrait("Desafortunadamente, yo tengo m'as sillas que amigos.",false,tempPort2,0);
	end
	UnloadTexture(tempPort);
	tempPort=nil;
	UnloadTexture(tempPort2);
	tempPort2=nil;
end
function Event06()
	tempPort = LoadPNG(FixString("Stuff/Portraits/Chair.png"));
	if (Lang==1) then
		ShowMessageWithPortrait("I'm special.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Yo soy especial.",false,tempPort,0);
	end
	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event07()
   if (flags[7]==1) then
	  ShowMessage("There's no escape.",false);
   else
	  if (flags[2]==0 and flags[5]==0) then
		 Event08();
	  end
	  SetPlayerPosition(6,8);
	  ChangeMap(FixString("Stuff/Maps/StartTown"));
   end
end
-- Matt
function Event08()
	if (flags[2]==0) then
		signport = LoadPNG(FixString("Stuff/Portraits/Matt.png"));
		if (Lang==1) then
			questionanswer = ShowMessageWithPortrait("Can I come?", true, signport, 0);
		elseif (Lang==2) then
			questionanswer = ShowMessageWithPortrait("'?Puedo ir contigo?", true, signport, 0);
		end

		if (questionanswer==false) then
			--tempPort33 = LoadPNG(FixString("Stuff/Portraits/Player.png"));
			--questionanswer = ShowMessageWithPortrait("(The game will be very hard without Matt.) Can Matt come?",true,tempPort33,0);
			--UnloadTexture(tempPort33);
		end

		if (questionanswer==true) then

			-- Don't know if I need to or how to translate these
			if (Lang==1) then
				questionanswer = ShowMessageWithPortrait("o0o0oOOo, okay!!", false, signport, 0);
				ShowMessage("Matt joined the party!",false);
			end

			-- Party slots are 0 based, party size isn't.
			AddPartyMember2();

			flags[2]=1;
			-- Hide Matt
			SetMapOtherData(6,12,false,0);
			SetMapImageData(6,12,1,0,0);
		elseif (questionanswer==false) then
			if (Lang==1) then
				questionanswer = ShowMessageWithPortrait("k, fiiiine", false, signport, 0);
			elseif (Lang==2) then
				questionanswer = ShowMessageWithPortrait("'!NOo0oo0oOo!", false, signport, 0);
			end
			flags[5]=1;
		end
		UnloadTexture(signport);
		questionanswer=nil;
		signport=nil;
	else
		DebugMsg("I'm not here.");
	end
end
function Event09()
	tempPort = LoadPNG(FixString("Stuff/Portraits/SmileyFace.png"));

	RestoreEntireParty();

	if (Lang==1) then
		ShowMessageWithPortrait("HP and MP restored.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("HP y MP restauraron.",false,tempPort,0);
	end

	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event10()
	signport = LoadPNG(FixString("Stuff/Portraits/Matt.png"));
	if (Lang==1) then
	   ShowMessageWithPortrait("I want a potato.", false, signport, 0);
	elseif (Lang==2) then
	   ShowMessageWithPortrait("Quiero una papa.", false, signport, 0);
	end
	UnloadTexture(signport);
	signport=nil;
   
	-- Battle
		InlineEnemy0Member = Malloc(true,2);
		InlineEnemy0Stats = GetPartyMemberStats(InlineEnemy0Member);
		-- IMPORTANT LINE
		SetStats(InlineEnemy0Stats,987,60,255,44,30,99,38,30,70);
		
		InlineEnemy0Idle = Malloc(true,0);
		InlineEnemy0Attack = Malloc(true,0);
		SetAnimation(InlineEnemy0Idle,10,100,100,4,true,0,0,LoadPNG(FixString("Stuff/Enemies/BlueBigFoot.png")));
		SetAnimation(InlineEnemy0Attack,8,100,100,4,true,0,0,LoadPNG(FixString("Stuff/Enemies/BlueBigFootAttack.png")));

		InlineEnemy1Member = Malloc(true,2);
		InlineEnemy1Stats = GetPartyMemberStats(InlineEnemy1Member);
		-- IMPORTRANT LINE
		SetStats(InlineEnemy1Stats,987,16,255,28,30,0,50,30,30);
		
		InlineEnemy1Idle = Malloc(true,0);
		InlineEnemy1Attack = Malloc(true,0);
		SetAnimation(InlineEnemy1Idle,10,100,100,8,false,0,0,LoadPNG(FixString("Stuff/Enemies/BigFoot.png")));
		SetAnimation(InlineEnemy1Attack,15,79,78,3,false,0,0,LoadPNG(FixString("Stuff/Enemies/BigFootAttack.png")));

		didWin = StartSpecificBattle(4,InlineEnemy0Member,InlineEnemy0Idle,InlineEnemy0Attack, InlineEnemy1Member, InlineEnemy1Idle, InlineEnemy1Attack, InlineEnemy1Member, InlineEnemy1Idle, InlineEnemy1Attack, InlineEnemy1Member, InlineEnemy1Idle, InlineEnemy1Attack);
		
		FreeAnimationImage(InlineEnemy0Idle);
		FreeAnimationImage(InlineEnemy0Attack);
		Free(InlineEnemy0Idle);
		Free(InlineEnemy0Attack);
		Free(InlineEnemy0Member);
		FreeAnimationImage(InlineEnemy1Idle);
		FreeAnimationImage(InlineEnemy1Attack);
		Free(InlineEnemy1Idle);
		Free(InlineEnemy1Attack);
		Free(InlineEnemy1Member);
		
		InlineEnemy0Member=nil;
		InlineEnemy0Stats=nil;
		InlineEnemy0Idle = nil;
		InlineEnemy0Attack = nil;
		InlineEnemy1Member=nil;
		InlineEnemy1Stats=nil;
		InlineEnemy1Idle = nil;
		InlineEnemy1Attack = nil;

		if (didWin==true) then
			if (Lang==1) then
				ToBlack();
				ShowMessage("The End. [Bonus End]",false)
				ThanksForPlaying();
			elseif (Lang==2) then
				ToBlack();
				ShowMessage("Fin. [Extra]",false);
				ThanksForPlaying();
			end
		else
		   tempPort33 = LoadPNG(FixString("Stuff/Portraits/PlayerSad.png"));
			if (Lang==1) then
				ShowMessageWithPortrait("So it ends here...",false,tempPort33,0);
			else
				ShowMessageWithPortrait("Esto es el fin...",false,tempPort33,0);
			end
			UnloadTexture(tempPort33);
			ExitTheGame();
		end
end

if (flags[2]==1) then
	-- Hide Matt
	SetMapOtherData(6,12,false,0);
	SetMapImageData(6,12,1,0,0);
end

tileset0=LoadPNG(FixString("Stuff/Tilesets/Inside1.png"));
SetTileset(tileset0,0);
tileset0=nil;

PlayBGM(FixString("Stuff/Sound/Town-Matt.ogg"))

if (flags[7]==1) then
	if (flags[6]==0) then
		-- if flags[6] is 0 but flags[7] is 1 then the player is on the final quest and entered the room normally

		-- move matt
		SetMapImageData(6,12,1,0,0);
		SetMapImageData(7,12,1,0,12);
		SetMapOtherData(7,12,true,10);
		
		SetMapImageData(6,13,0,0,1); -- wall
		SetMapOtherData(6,13,true,0);
		SetMapImageData(8,13,0,0,1); -- wall
		SetMapOtherData(8,13,true,0);

		-- make the door solid
		SetMapOtherData(7,14,true,7);
	else
		-- the player came here by dying. kick them out.
		SetPlayerPosition(7,1);
		ChangeMap(FixString("Stuff/Maps/BigFootChamber"));
	end
end
