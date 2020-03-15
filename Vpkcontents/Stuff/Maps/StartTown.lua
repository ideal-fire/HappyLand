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

function Event01()
--
	--WierdSlimeHybridMember = Malloc(true,2);
	--WierdSlimeHybridStats = GetPartyMemberStats(WierdSlimeHybridMember);
	--SetStats(WierdSlimeHybridStats,10,10,1,3,3,1,1,4,500);
--
	--enemyidle0 = Malloc(true,0);
	--enemyatk0 = Malloc(true,0);
	--SetAnimation(enemyidle0,100,64,64,3,false,0,0,LoadPNG("app0:Stuff/Enemies/AngryTree.png"));
	--SetAnimation(enemyatk0,20,32,32,3,false,0,0,LoadPNG("app0:Stuff/Enemies/Slime.png"));
--
	--StartSpecificBattle(2,WierdSlimeHybridMember,enemyidle0,enemyatk0,WierdSlimeHybridMember,enemyidle0,enemyatk0);
--
	--FreeAnimationImage(enemyidle0);
	--FreeAnimationImage(enemyatk0);
	--Free(enemyidle0);
	--Free(enemyatk0);
	--Free(WierdSlimeHybridMember);
--
	SetPlayerPosition(7,13);
	ChangeMap(FixString("Stuff/Maps/NathansHouse"));
end
function Event02()
	namelessmanport = LoadPNG(FixString("Stuff/Portraits/BasicMan.png"));
	if (Lang==1) then
		ShowMessageWithPortrait("What are you stil doing here? You have to go and fight BigFoot to get our potatoes back!",false,namelessmanport,0);
	elseif (Lang==2) then
		-- TODO - In spanish version, Big Foot is a big pie.
		ShowMessageWithPortrait("'?Porque est'as aqui? Tienes que matar Pie Grande porque 'el rob'o nuestros papas!",false,namelessmanport,0);
	end
	UnloadTexture(namelessmanport);
	namelessmanport=nil;
end
function Event03()
	signport = LoadPNG(FixString("Stuff/Portraits/WaterBottle.png"));
	if (flags[1]==0) then
		if (Lang==1) then
			ShowMessageWithPortrait("I'm Infinite Big Water Bottle. I can teach you how to use the spell 'Drink Water'.", false, signport, 0);
			questionanswer = ShowMessageWithPortrait('Want to learn how to use it?', true, signport, 0);
		elseif (Lang==2) then
			ShowMessageWithPortrait('Soy Botella De Agua Grande. Puedo ' .. "ese'nar la magia " .. '"Beber Agua".', false, signport, 0);
			questionanswer = ShowMessageWithPortrait("'?Quieres aprender la magia " .. '"Beber Agua"?', true, signport, 0);
		end
	
		if (questionanswer==true) then
			for i=0,GetPartySize()-1 do
				AddSpellToStats(GetPartyMemberStats(i),2);
			end
			if (Lang==1) then
				ShowMessageWithPortrait("You learned Drink Water!", false, signport, 0);
			elseif (Lang==2) then
				ShowMessageWithPortrait("'!T'u aprendiste " .. '"Beber Agua"!', false, signport, 0);
			end
			flags[1]=1;
			StartWildBattle();
		elseif (questionanswer==false) then
			if (Lang==1) then
				ShowMessageWithPortrait("Fine, have it your way, just don't pay me.", false, signport, 0);
			elseif (Lang==2) then
				ShowMessageWithPortrait("Bueno.", false, signport, 0);
			end
		end
	elseif (flags[1]==1) then
		if (Lang==1) then
			ShowMessageWithPortrait("Take a hike, freeloader.", false, signport, 0);
		elseif (Lang==2) then
			-- You're a freeloader.
			-- I'm pretty sure this translation is wrong.
			ShowMessageWithPortrait("Eres un gorr'on.", false, signport, 0);
		end
	end
	UnloadTexture(signport);
	questionanswer=nil;
	signport=nil;
end
function Event04()
	SetPlayerPosition(1,10);
	-- Exit to snow area
	ChangeMap(FixString("Stuff/Maps/SnowLand"));
end
function Event05()
	signport = LoadPNG(FixString("Stuff/Portraits/Sign.png"));
	if (Lang==1) then
		ShowMessageWithPortrait("Only the lenendary hero can pull the sword from the sword thingie.", false, signport, 0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Solamente el h'eroe legendario puede extraer la espada.", false, signport, 0);
	end
	UnloadTexture(signport);
	signport=nil;
end
function Event06()
	signport = LoadPNG(FixString("Stuff/Portraits/PlayerSad.png"));
	if (Lang==1) then
		ShowMessageWithPortrait("I can't pull the sword.", false, signport, 0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("No puedo extraer la espada.", false, signport, 0);
	end
	UnloadTexture(signport);
	signport=nil;
end
function Event07()
	
	-- Tree
	treeport = LoadPNG(FixString("Stuff/Portraits/AngryTree.png"));
	smileyplayer = LoadPNG(FixString("Stuff/Portraits/Player.png"));
	
	if (flags[3]==0) then

		if (Lang==1) then
			ShowMessageWithPortrait("You look angry.", false, smileyplayer, 0);
			ShowMessageWithPortrait("That's right, I'm Angry Tree", false, treeport, 0);
			ShowMessageWithPortrait("Why're you angry?", false, smileyplayer, 0);
			ShowMessageWithPortrait("Everybody around here keeps stepping on my roots. But I'll be putting an end to that today.", false, treeport, 0);
			ShowMessageWithPortrait("Really? How do you plan on doing that?", false, smileyplayer, 0);
			pauseBGM();
			ShowMessageWithPortrait("These roots aren't just for being stepped on!", false, treeport, 0);
			resumeBGM();
			ShowMessageWithPortrait("What else can they be used for?", false, smileyplayer, 0);
			ShowMessageWithPortrait("Getting nutriets, water, and other stuff.", false, treeport, 0);
			ShowMessageWithPortrait("Anyway, now that I've enlightened you, I'm now going to kill you using logs.", false, treeport, 0);
		elseif (Lang==2) then
			-- Puedes venir entras de mi casa...
			local _angryPlayer = LoadPNG(FixString("Stuff/Portraits/PlayerAngry.png"));

			ShowMessageWithPortrait("'?Eres enojado?", false, smileyplayer, 0);
			-- I would put an acsent on arbol, but I didn't add support for capital acsented letters
			ShowMessageWithPortrait("S'i, me llamo Enojado Arbol.", false, treeport, 0);
			ShowMessageWithPortrait("'?Porque est'as enojado?", false, smileyplayer, 0);
			ShowMessageWithPortrait("Porque no me gusta el sol y t'u tienes una casa.", false, treeport, 0);
			ShowMessageWithPortrait("No me importa, t'u nunca me das manzanas. Yo voy a te mato porque eres est'upido.", false, smileyplayer, 0);
			ShowMessageWithPortrait("noooooooooooo", false, treeport, 0);

			UnloadTexture(_angryPlayer);
		end

		
		-- START BATTLE
			WierdSlimeHybridMember = Malloc(true,2);
			WierdSlimeHybridStats = GetPartyMemberStats(WierdSlimeHybridMember);
			SetStats(WierdSlimeHybridStats,987,70,255,15,13,7,11,9,40);

			enemyidle0 = Malloc(true,0);
			enemyatk0 = Malloc(true,0);
			SetAnimation(enemyidle0,30,64,64,3,false,0,0,LoadPNG(FixString("Stuff/Enemies/AngryTree.png")));
			SetAnimation(enemyatk0,8,64,64,3,false,0,0,LoadPNG(FixString("Stuff/Enemies/AngryTreeAttack.png")));
		
			SetStatsSpells(WierdSlimeHybridMember,4);


			didWin = StartSpecificBattle(1,WierdSlimeHybridMember,enemyidle0,enemyatk0);
			if (didWin==true) then
				flags[3]=1;
				SetMapImageData(0,7,1,0,0);
				SetMapOtherData(0,7,false,0);
			else
				if (Lang==1) then
					ShowMessageWithPortrait("Hahahaha! I'm more angry than you!",false,treeport,0);
				elseif (Lang==2) then
					ShowMessageWithPortrait("'!Jajajaja! '!You estoy m'as enojado que t'u!",false,treeport,0);
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

			RestoreEntireParty();
	else
		if (Lang==1) then
			ShowMessageWithPortrait("Ouch.", false, treeport, 0);
		elseif (Lang==2) then
			ShowMessageWithPortrait("Ay.", false, treeport, 0);
		end
	end
	
	UnloadTexture(treeport);
	UnloadTexture(smileyplayer);
	treeport=nil;
	
end
function Event08()
	-- Flower
	signport = LoadPNG(FixString("Stuff/Portraits/Flower.png"));
	if (Lang==1) then
		ShowMessageWithPortrait("I'm The Lone Flower.", false, signport, 0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Me llamo El Flor Solo.", false, signport, 0);
	end
	UnloadTexture(signport);
	signport=nil;
end
-- Matt
function Event09()
end
function Event10()
	signport = LoadPNG(FixString("Stuff/Portraits/Sign.png"));
	if (Lang==1) then
		ShowMessageWithPortrait("Nathan's humble shack is located to the left of this random sign.", false, signport, 0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("La casa humilde de Nathan est'a a la izquierda de este letrero.", false, signport, 0);
	end
	UnloadTexture(signport);
	signport=nil;
end
function Event11()
	tempPort = LoadPNG(FixString("Stuff/Portraits/Note.png"));

	if (Lang==1) then
		ShowMessageWithPortrait("Come to Big Foot Land if you ever want to see your potatoes again. - Big Foot",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Viene al Mundo de Pie Grande si quieres tus papas. - Pie Grande",false,tempPort,0);
	end

	UnloadTexture(tempPort);
	tempPort=nil;
end


tileset0=LoadPNG(FixString("Stuff/Tilesets/Town1.png"));
SetTileset(tileset0,0);
tileset0=nil;

--
battleEnemyLoadId=0;
dofile(FixString("Stuff/BattleLua/Slime.lua"));
--

SetEncounterRate(10);

if (flags[6]==0) then
   PlayBGM(FixString("Stuff/Sound/HolFix-PixelParade.ogg"))
end

if (flags[3]==1) then
	-- Remove the tree if you've fought it
	SetMapImageData(0,7,1,0,0);
	SetMapOtherData(0,7,false,0);
end
