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
	ChangeMap("app0:Stuff/Maps/NathansHouse")
end
function Event02()
	namelessmanport = LoadPNG("app0:Stuff/Portraits/BasicMan.png");
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
	signport = LoadPNG("app0:Stuff/Portraits/WaterBottle.png");
	if (flags[0]==0) then
		if (Lang==1) then
			ShowMessageWithPortrait("I'm Infinite Big Water Bottle. I can teach you how to use the spell 'Drink Water'.", false, signport, 0);
			questionanswer = ShowMessageWithPortrait('Want to learn how to use it?', true, signport, 0);
		elseif (Lang==2) then
			ShowMessageWithPortrait('Soy Botella De Agua Grande. Puedo ' .. "ese'nar la magia " .. '"Beber Agua".', false, signport, 0);
			questionanswer = ShowMessageWithPortrait("'?Quieres ese'nar la magia " .. '"Beber Agua"?', true, signport, 0);
		end
	
		if (questionanswer==true) then
			for i=0,GetPartySize()-1 do
				AddSpellToStats(GetPartyMemberStats(i),2);
			end
			if (Lang==1) then
				ShowMessageWithPortrait("You learned Drink Water!", false, signport, 0);
			elseif (Lang==2) then
				ShowMessageWithPortrait("'!T'u ense'naste " .. '"Beber Agua"!', false, signport, 0);
			end
			flags[0]=1;
			StartWildBattle();
		elseif (questionanswer==false) then
			if (Lang==1) then
				ShowMessageWithPortrait("Fine, have it your way, just don't pay me.", false, signport, 0);
			elseif (Lang==2) then
				ShowMessageWithPortrait("Bueno.", false, signport, 0);
			end
		end
	elseif (flags[0]==1) then
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
	ChangeMap("app0:Stuff/Maps/SnowLand")
end
function Event05()
	signport = LoadPNG("app0:Stuff/Portraits/Sign.png");
	if (Lang==1) then
		ShowMessageWithPortrait("Only the lenendary hero can pull the sword from the sword thingie.", false, signport, 0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Solamente el h'eroe legendario puede extraer la espada.", false, signport, 0);
	end
	UnloadTexture(signport);
	signport=nil;
end
function Event06()
	signport = LoadPNG("app0:Stuff/Portraits/PlayerSad.png");
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
	treeport = LoadPNG("app0:Stuff/Portraits/AngryTree.png");
	smileyplayer = LoadPNG("app0:Stuff/Portraits/Player.png");
	
	if (flags[2]==0) then

		if (Lang==1) then
			ShowMessageWithPortrait("You look angry.", false, smileyplayer, 0);
			-- I would put an acsent on arbol, but I didn't add support for capital acsented letters
			ShowMessageWithPortrait("That's right, I'm Angry Tree", false, treeport, 0);
			ShowMessageWithPortrait("Why're you angry?", false, smileyplayer, 0);
			ShowMessageWithPortrait("I can talk and think, but I'm a tree. So I can't move.", false, treeport, 0);
			ShowMessageWithPortrait("It isn't that bad. This world is very small, it has only four rooms. There isn't much to see.", false, smileyplayer, 0);
			ShowMessageWithPortrait(">:(", false, treeport, 0);
		elseif (Lang==2) then
			ShowMessageWithPortrait("'?Eres enojado?", false, smileyplayer, 0);
			-- I would put an acsent on arbol, but I didn't add support for capital acsented letters
			ShowMessageWithPortrait("S'i, me llamo Enojado Arbol.", false, treeport, 0);
			ShowMessageWithPortrait("'?Porque est'as enojado?", false, smileyplayer, 0);
			ShowMessageWithPortrait("Porque puedo hablar y pensar, pero soy un 'arbol. No puedo mover.", false, treeport, 0);
			ShowMessageWithPortrait("No es m'uy horrible. Este mundo es muy peque'no. 4 salas. No hay mucho para ver.", false, smileyplayer, 0);
			ShowMessageWithPortrait(">:(", false, treeport, 0);
		end

		
		-- START BATTLE
			WierdSlimeHybridMember = Malloc(true,2);
			WierdSlimeHybridStats = GetPartyMemberStats(WierdSlimeHybridMember);
			SetStats(WierdSlimeHybridStats,987,70,255,15,13,7,11,9,40);

			enemyidle0 = Malloc(true,0);
			enemyatk0 = Malloc(true,0);
			SetAnimation(enemyidle0,30,64,64,3,false,0,0,LoadPNG("app0:Stuff/Enemies/AngryTree.png"));
			SetAnimation(enemyatk0,8,64,64,3,false,0,0,LoadPNG("app0:Stuff/Enemies/AngryTreeAttack.png"));
		
			SetStatsSpells(WierdSlimeHybridMember,4);


			didWin = StartSpecificBattle(1,WierdSlimeHybridMember,enemyidle0,enemyatk0);
			if (didWin==true) then
				flags[2]=1;
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

			RestorePartyMember(0);
		
	else
		if (Lang==1) then
			ShowMessageWithPortrait("Ouch.", false, treeport, 0);
		elseif (Lang==2) then
			ShowMessageWithPortrait("Ay.", false, treeport, 0);
		end
	end
	
	UnloadTexture(treeport);
	treeport=nil;
	
end
function Event08()
	-- Flower
	signport = LoadPNG("app0:Stuff/Portraits/Flower.png");
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
	if (flags[1]==0) then
		signport = LoadPNG("app0:Stuff/Portraits/Matt.png");
		if (Lang==1) then
			questionanswer = ShowMessageWithPortrait("Can I come?", true, signport, 0);
		elseif (Lang==2) then
			questionanswer = ShowMessageWithPortrait("'?Puedo ir contigo?", true, signport, 0);
		end
		if (questionanswer==true) then

			-- Don't know if I need to or how to translate these
			if (Lang==1) then
				questionanswer = ShowMessageWithPortrait("o0o0oOOo, okay!!", false, signport, 0);
				ShowMessage("Matt joined the party!",false);
			end

			-- Party slots are 0 based, party size isn't.
			mattslot = GetPartySize();
			tempidle = GetPartyMemberAnimation(mattslot,1)
			tempatk = GetPartyMemberAnimation(mattslot,2)

			SetAnimation(tempidle,30,29,68,-1,false,0,0,LoadPNG("app0:Stuff/Battle/MattIdle.png"));
			SetAnimation(tempatk,10,56,68,-1,true,0,0,LoadPNG("app0:Stuff/Battle/MattAttack.png"));
			SetStats(GetPartyMemberStats(mattslot),1,35,30,8,8,15,15,7,0,MallocString("Matt"));
			SetPartySize(mattslot+1);
			RestorePartyMember(mattslot);

			tempidle=nil;
			tempatk=nil;
			mattslot=nil;

			flags[1]=1;
			-- Hide Matt
			SetMapOtherData(4,9,false,0);
			SetMapImageData(4,9,1,0,0);
		elseif (questionanswer==false) then
			if (Lang==1) then
				questionanswer = ShowMessageWithPortrait("k, fiiiine", false, signport, 0);
			elseif (Lang==2) then
				questionanswer = ShowMessageWithPortrait("'!NOo0oo0oOo!", false, signport, 0);
			end
		end
		UnloadTexture(signport);
		questionanswer=nil;
		signport=nil;
	else
		DebugMsg("I'm not here.");
	end
end
function Event10()
	signport = LoadPNG("app0:Stuff/Portraits/Sign.png");
	if (Lang==1) then
		ShowMessageWithPortrait("Nathan's humble shack is located to the left of this random sign.", false, signport, 0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("La casa humilde de Nathan est'a a la izquierda de este letrero.", false, signport, 0);
	end
	UnloadTexture(signport);
	signport=nil;
end


tileset0=LoadPNG("app0:Stuff/Tilesets/Town1.png");
SetTileset(tileset0,0);
tileset0=nil;

--
battleEnemyLoadId=0;
dofile("app0:Stuff/BattleLua/Slime.lua");
--

SetEncounterRate(10);