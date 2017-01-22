-- MAKE BIGFOOTS FALL


function MapDispose()
	FreeTileset(0);
	Event01=nil;
	Event02=nil;
	Event03=nil;
	Event04=nil;
end

function Event01()
	-- Way back
	SetPlayerPosition(7,1);
	ChangeMap("app0:Stuff/Maps/BigFootChamber");
end
function Event02()
	tempPort = LoadPNG("app0:Stuff/Portraits/SmileyFace.png");

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
	ShowMessage("...",false);
end
function Event04()
	-- POTATOZZZZZZZZ

	BigFootPackPortrait = LoadPNG("app0:Stuff/Portraits/BigFootPack.png");
	PlayerPortrait = LoadPNG("app0:Stuff/Portraits/Player.png");
	PlayerAngryPortrait = LoadPNG("app0:Stuff/Portraits/PlayerAngry.png");
	PlayerSurprised = LoadPNG("app0:Stuff/Portraits/PlayerSurprised.png");
	NamelessPortrait = LoadPNG("app0:Stuff/Portraits/AngryBasicMan.png");
	NamelessDeadPortrait = LoadPNG("app0:Stuff/Portraits/HurtBasicMan.png");

	if (Lang==1) then
		ShowMessageWithPortrait("What are you doing? You can't take back the potatoes yet.",false,BigFootPackPortrait,0);
		ShowMessageWithPortrait("Why not?",false,PlayerPortrait,0);
		ShowMessageWithPortrait("You have to fight us first. That was the point of the portal.",false,BigFootPackPortrait,0);
		ShowMessageWithPortrait("What?!",false,PlayerSurprised,0);
		ShowMessageWithPortrait("I see, so you're scared.",false,BigFootPackPortrait,0);
		ShowMessageWithPortrait("No, that's not it. This is not what I signed up for! I was told that all I had to do was fight Big Foot and grab some potatoes. No more, no less. I wasn't informed that there would be more Big Foots! I was scammed!",false,PlayerPortrait,0);
		ShowMessageWithPortrait("Oh....",false,BigFootPackPortrait,0);
		questionAnswer = ShowMessageWithPortrait("Should I fight?",true,PlayerPortrait,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("'?Qu'e est'as haciendo? No puedes tomar las papas.",false,BigFootPackPortrait,0);
		ShowMessageWithPortrait("'?Por que no?",false,PlayerPortrait,0);
		ShowMessageWithPortrait("Tienes que pelear con nosotros.",false,BigFootPackPortrait,0);
		ShowMessageWithPortrait("'!'?Qu'e?!",false,PlayerSurprised,0);
		ShowMessageWithPortrait("Yo entiendo. Eres asustado.",false,BigFootPackPortrait,0);
		ShowMessageWithPortrait('No, ' .. "'el dijo " .. '"' .. "T'u tienes que matar Pie Grande y tocar las papas!" .. '"' .. ". 'el no dijo que yo tengo que matar m'as Pies Grandes. '!Esta es una estafa!",false,PlayerPortrait,0);
		ShowMessageWithPortrait("Oh....",false,BigFootPackPortrait,0);
		questionAnswer = ShowMessageWithPortrait("'?Debo pelear?",true,PlayerPortrait,0);
	end

	if (questionAnswer==true) then
		if (Lang==1) then
			ShowMessageWithPortrait("I'll still fight.",false,PlayerPortrait,0);
			ShowMessageWithPortrait("Okay, let's go!",false,BigFootPackPortrait,0);
			-- battle is like 3 normal bigfoots and 1 blue big foot
			DebugMsg("Start battle");
			-- Normal dialouge from aftawards
			-- Add that i'll eat it b/c guy scammed me

			ShowMessageWithPortrait("Ouch.",false,BigFootPackPortrait,0);
			ShowMessageWithPortrait("Yes, I am, the WinnerMan!",false,PlayerPortrait,0);
			ShowMessageWithPortrait("Fine, take your potatoes back.",false,BigFootPackPortrait,0);
			DebugMsg("Epiloguge");
			ShowMessageWithPortrait("Nathan ate the potatoes as a snack on the way back.",false,PlayerPortrait,0);
			ShowMessage("The End. [Easy End]",false)
		elseif (Lang==2) then
			ShowMessageWithPortrait("Yo voy a pelear.",false,PlayerPortrait,0);
			ShowMessageWithPortrait("'!Vamos, Pies Grandes!",false,BigFootPackPortrait,0);
			
			DebugMsg("Start battle");
			ShowMessageWithPortrait("'!Ay!",false,BigFootPackPortrait,0);
			ShowMessageWithPortrait("S'i, yo soy el Gan'oHomebre.",false,PlayerPortrait,0);
			ShowMessageWithPortrait("Bueno. '!Toma tu papas!",false,BigFootPackPortrait,0);
			DebugMsg("Epiloguge");
			ShowMessageWithPortrait("Cuando Nathan est'a yendo a su casa, 'el comi'o las papas.",false,PlayerPortrait,0);
			ShowMessage("Fin. [Fin F'acil]",false);
		end
		DebugMsg("END GAME");
	elseif (questionAnswer==false) then
		-- He wants to fight as to not end his streak of morons falling for his scams.
		if (Lang==1) then
			ShowMessageWithPortrait("No, you must fight the Big Foots!",false,NamelessPortrait,0);
			ShowMessageWithPortrait("Why do you care?",false,PlayerPortrait,0);
			ShowMessageWithPortrait("Because every person I've scammed so far has fallen for the scam. I don't want to end my streak now!",false,PlayerPortrait,0);
			ShowMessageWithPortrait("I'll never abide to your demands, scammer!",false,PlayerAngryPortrait,0);
			DebugMsg("BATTLE");
			ShowMessageWithPortrait("No0o0o00oOOoo0o0oo! You win this round. I shall now retreat to my hidden fortress.",false,NamelessDeadPortrait,0);
			DebugMsg("Epilouge");
			ShowMessage("Nathan was happy that he wasn't scammed. He returned to his house and slept for the rest of his life because the world was at peace.",false);
		elseif (Lang==2) then
			ShowMessageWithPortrait("'!No, tienes que matar los Pies Grandes!",false,NamelessPortrait,0);
			ShowMessageWithPortrait("'?Por qu'e te importa?",false,PlayerPortrait,0);
			ShowMessageWithPortrait("Porque todas las personas que yo trato de estafar perd'ian. Ellos siempre hac'ian mas trabajo.",false,PlayerPortrait,0);
			ShowMessageWithPortrait("'!Yo no voy a acatar, eres un estafador!",false,PlayerAngryPortrait,0);
			DebugMsg("BATTLE");
			ShowMessageWithPortrait("'!No0o0o00oOOoo0o0oo! T'u ganate. Yo voy a ir a mi fortaleza castillo.",false,NamelessDeadPortrait,0);
			DebugMsg("Epilouge");
			ShowMessage("Nathan era feliz porque 'el no se estaf'o . 'el fu'i a su casa y dorm'i por el resto de su vida porque el mundo era tranquilo.",false);
		end
	end

	UnloadTexture(BigFootPackPortrait); 
	UnloadTexture(PlayerPortrait);
	UnloadTexture(PlayerAngryPortrait); 
	UnloadTexture(PlayerSurprised);
	UnloadTexture(NamelessPortrait);
	UnloadTexture(NamelessDeadPortrait);
end


tileset0=LoadPNG("app0:Stuff/Tilesets/BigFootLand.png");
SetTileset(tileset0,0);
tileset0=nil;
