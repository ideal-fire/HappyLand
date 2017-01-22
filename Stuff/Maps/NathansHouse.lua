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
end

function Event01()
	tempPort = LoadPNG("app0:Stuff/Portraits/Player.png");

	if (Lang==1) then
		ShowMessageWithPortrait("It's my bed.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Esta es mi cama.",false,tempPort,0);
	end

	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event02()
	tempPort = LoadPNG("app0:Stuff/Portraits/Player.png");
	if (Lang==1) then
		ShowMessageWithPortrait("It's my fresh computer. I like to play my favorite game on it, Space Pinball. It came free with the XP version of my operating system. XP standing for" .. '"eXtra exPencive", of course.',false,tempPort,0);
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
	tempPort = LoadPNG("app0:Stuff/Portraits/Sign.png");
	if (Lang==1) then
		ShowMessageWithPortrait("Note2self: Put more stuff in my house.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Nota personal: '!Pone m'as cosas en mi casa!",false,tempPort,0);
	end
	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event04()
	tempPort = LoadPNG("app0:Stuff/Portraits/Player.png");
	if (Lang==1) then
		ShowMessageWithPortrait("I love having useless boxes in my house.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Me encanta cajas in'utiles en mi casa.",false,tempPort,0);
	end
	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event05()
	tempPort = LoadPNG("app0:Stuff/Portraits/Player.png");
	tempPort2 = LoadPNG("app0:Stuff/Portraits/PlayerSad.png");
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
	tempPort = LoadPNG("app0:Stuff/Portraits/Chair.png");
	if (Lang==1) then
		ShowMessageWithPortrait("I'm special.",false,tempPort,0);
	elseif (Lang==2) then
		ShowMessageWithPortrait("Yo soy especial.",false,tempPort,0);
	end
	UnloadTexture(tempPort);
	tempPort=nil;
end
function Event07()
	if (flags[1]==0) then
			
	else
		SetPlayerPosition(6,8);
		ChangeMap("app0:Stuff/Maps/StartTown")
	end
end
-- Matt
function Event08()
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
			SetMapOtherData(6,12,false,0);
			SetMapImageData(6,12,1,0,0);
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


if (flags[1]==1) then
	-- Hide Matt
	SetMapOtherData(6,12,false,0);
	SetMapImageData(6,12,1,0,0);
end




tileset0=LoadPNG("app0:Stuff/Tilesets/Inside1.png");
SetTileset(tileset0,0);
tileset0=nil;
