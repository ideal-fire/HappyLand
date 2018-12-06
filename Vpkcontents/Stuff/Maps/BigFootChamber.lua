function MapDispose()
	FreeTileset(0);
	Event01=nil;
	Event02=nil;
	Event03=nil;
	Event04=nil;
	Event05=nil;
	Event06=nil;
	Event07=nil;
end

function Event06()
	DebugMsg("I really hope you modded the game or something. Because otherwise, there's a glitch. I don't like glitches.");
end

function Event01()
	tempPort = LoadPNG(FixString("Stuff/Portraits/Note.png"));

	if (Lang==1) then
		ShowMessageWithPortrait("Welcome to Big Foot Chamber.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Bienvenido a La Sala De Pie Grande.",false,tempPort,0);
	end

	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event02()
	tempPort = LoadPNG(FixString("Stuff/Portraits/SmileyFace.png"));

	for i=0,GetPartySize()-1 do
		RestorePartyMember(i);
	end

	if (Lang==1) then
		ShowMessageWithPortrait("HP and MP restored.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("HP y MP restauraron.",false,tempPort,0);
	end

	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event03()

	if (Lang==1) then
		ShowMessage("I am Manly Snow.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessage("Yo soy varonil.",false,tempPort,0);
		ShowMessage("...Necesito un chiste nuevo. Yo digo un chiste en 'ingles.",false,tempPort,0);
	end
end
function Event04()
	--DebugMsg("Big conversation");
	if (flags[4]==0) then

		-- Load needed portraits
		bigfootport = LoadPNG(FixString("Stuff/Portraits/BigFoot.png"));
		playerSurprised = LoadPNG(FixString("Stuff/Portraits/PlayerSurprised.png"));
		playerHappy = LoadPNG(FixString("Stuff/Portraits/Player.png"));
		
		if (Lang==1) then
			ShowMessageWithPortrait("...",false,playerSurprised,0);
			ShowMessageWithPortrait("You're speachless because of the epic aura that I give off.",false,bigfootport,0);
			ShowMessageWithPortrait("Yes.",false,playerHappy,0);
			ShowMessageWithPortrait("I know why you've come.",false,bigfootport,0)
			ShowMessageWithPortrait("You do?!",false,playerSurprised,0);
			ShowMessageWithPortrait("Yes, you came to get my signature.",false,bigfootport,0);
			ShowMessageWithPortrait("No.",false,playerHappy,0);
			ShowMessageWithPortrait("Then why are you in Big Foot Chamber?",false,bigfootport,0);
			ShowMessageWithPortrait("I've come to defat you and get my potatoes back from you.",false,playerHappy,0);
			ShowMessageWithPortrait("Ah, okay. If you defeat me, all you need to do is go in the wierd portal behind me to get to the potatoes.",false,bigfootport,0);
			ShowMessageWithPortrait("Okay, let's fight now.",false,playerHappy,0);
		elseif (Lang==2) then
			ShowMessageWithPortrait("...",false,playerSurprised,0);
			ShowMessageWithPortrait("No est'as hablando porque soy 'epico.",false,bigfootport,0);
			ShowMessageWithPortrait("S'i.",false,playerHappy,0);
			ShowMessageWithPortrait("Yo s'e porque t'u veniste aqui.",false,bigfootport,0)
			ShowMessageWithPortrait("?!",false,playerSurprised,0);
			ShowMessageWithPortrait("S'i, t'u quieres mi firma.",false,bigfootport,0);
			ShowMessageWithPortrait("No.",false,playerHappy,0);
			ShowMessageWithPortrait("'?Por que est'as en La Sala De Pie Grande?",false,bigfootport,0);
			ShowMessageWithPortrait("Porque you quiero matar t'u y tomar mis papas.",false,playerHappy,0);
			ShowMessageWithPortrait("'!Ah, s'i! Si me matas, debes entrar el portal para recibir tus papas.",false,bigfootport,0);
			ShowMessageWithPortrait("Bueno.",false,playerHappy,0);
		end

		--DebugMsg("STart fight");

		SetMapImageData(7,3,1,0,0);
		SetMapImageData(8,3,1,0,0);
		SetMapImageData(7,4,1,0,0);
		SetMapImageData(8,4,1,0,0);

		-- START BATTLE
			WierdSlimeHybridMember = Malloc(true,2);
			WierdSlimeHybridStats = GetPartyMemberStats(WierdSlimeHybridMember);
			SetStats(WierdSlimeHybridStats,255,80,255,50,35,0,43,21,140);

			enemyidle0 = Malloc(true,0);
			enemyatk0 = Malloc(true,0);
			SetAnimation(enemyidle0,10,100,100,8,false,0,0,LoadPNG(FixString("Stuff/Enemies/BigFoot.png")));
			SetAnimation(enemyatk0,15,79,78,3,false,0,0,LoadPNG(FixString("Stuff/Enemies/BigFootAttack.png")));
		
			--SetStatsSpells(WierdSlimeHybridMember,1,2);


			didWin = StartSpecificBattle(1,WierdSlimeHybridMember,enemyidle0,enemyatk0);
			if (didWin==true) then
				flags[4]=1;
				HideTile(7,3,1);
				HideTile(8,3,1);
				HideTile(7,4,1);
				HideTile(8,4,1);
			else
				-- Only restore Big Foot if you don't kill him
				SetMapImageData(7,3,1,0,2);
				SetMapImageData(8,3,1,0,3);
				SetMapImageData(7,4,1,0,4);
				SetMapImageData(8,4,1,0,5);
				if (Lang==1) then
					ShowMessageWithPortrait("Hahahaha! I'm more big than you!",false,bigfootport,0);
				elseif (Lang==2) then
					ShowMessageWithPortrait("'!Jajajaja! '!You soy m'as grande que t'u!",false,bigfootport,0);
				end
			end
			didWin=nil;

			FreeAnimationImage(enemyidle0);
			FreeAnimationImage(enemyatk0);
			Free(enemyidle0);
			Free(enemyatk0);
			Free(WierdSlimeHybridMember);


			WierdSlimeHybridMember=nil;
			WierdSlimeHybridStats=nil;
			enemyidle0 = nil;
			enemyatk0 = nil;

			RestorePartyMember(0);
			RestorePartyMember(1);
		-- After you've killed bigfoot
		if (flags[4]==1) then
	
			tempx=0;
			tempy=5;
			while (true) do
				SetMapImageData(tempx,tempy,1,0,6);
				SetMapImageData(14-tempx,tempy,1,0,6);
		
				if (tempy==0) then
					break;
				end
		
				tempx=tempx+1;
				tempy=tempy-1;
				RedrawMap();
				Wait(200);
			end
			
			-- Add portal
			SetMapImageData(7,0,1,0,7);
			SetMapOtherData(7,0,false,5);
		
			tempx=nil;
			tempy=nil;
		end


		

		-- Dispose portraits
		UnloadTexture(bigfootport);
		bigfootport=nil;
		UnloadTexture(playerSurprised);
		playerSurprised=nil;
		UnloadTexture(playerHappy);
		playerHappy=nil;
	else
		deadfootport = LoadPNG(FixString("Stuff/Portraits/BigFootDead.png"));
		if (Lang==1) then
			ShowMessageWithPortrait("Ouch.", false, deadfootport, 0);
		elseif (Lang==2) then
			ShowMessageWithPortrait("Ay.", false, deadfootport, 0);
		end
		UnloadTexture(deadfootport);
		deadfootport=nil;
	end
end
function Event05()
	if (flags[4]==1) then
		SetPlayerPosition(7,13);
		ChangeMap(FixString("Stuff/Maps/BigFootLand"));
	else
		tempPort = LoadPNG(FixString("Stuff/Portraits/Player.png"));
		if (Lang==1) then
			ShowMessageWithPortrait("I should go fight Big Foot.",false,tempPort,0);
		elseif (Lang==2) then
			ShowMessageWithPortrait("Yo debo matar Pie Grande.",false,tempPort,0);
		end
		UnloadTexture(tempPort);
		tempPort=nil;
	end
end

function Event07()
	SetPlayerPosition(7,1);
	ChangeMap(FixString("Stuff/Maps/SnowLand"));
end


tileset0=LoadPNG(FixString("Stuff/Tilesets/BigFootChamber.png"));
SetTileset(tileset0,0);
tileset0=nil;

if (flags[4]==1) then
	tempx=0;
	tempy=5;
	-- Set torches
	while (true) do
		SetMapImageData(tempx,tempy,1,0,6);
		SetMapImageData(14-tempx,tempy,1,0,6);
	
		if (tempy==0) then
			break;
		end
	
		tempx=tempx+1;
		tempy=tempy-1;
	end
	
	-- Add portal
	SetMapImageData(7,0,1,0,7);
	SetMapOtherData(7,0,false,5);

	-- Remove Big Foot
	HideTile(7,3,1);
	HideTile(8,3,1);
	HideTile(7,4,1);
	HideTile(8,4,1);
else
	-- Remove portal
	SetMapImageData(7,0,1,0,0);
	SetMapOtherData(7,0,false,0);
end

RedrawMap();
if (GetLevel(0)<6) then
	if (Lang==1) then
		ShowMessage("You feel you should fight more noob enemies.",false);
	elseif (Lang==2) then
		ShowMessage("T'u piensas que debes matar m'as enemigos f'aciles.",false)
	end
end


PlayBGM(FixString("Stuff/Sound/HolFix-Savior.ogg"))