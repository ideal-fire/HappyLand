PLAT_VITA = 1;
PLAT_WINDOWS = 2;
PLAT_3DS = 3;

SUB_NONE = 0;
SUB_ANDROID = 1;

TYPE_SLOT = 1;
TYPE_ID = 2;

ANIMATION_IDLE = 1;
ANIMATION_ATTACK = 2;

function SetFlag(index,val)
	flags[index]=val;
end

-- Includes the dot
-- Example return value:
-- .png
function GetFileExtention(_toFindExtention)
	return string.sub(_toFindExtention,string.len(_toFindExtention)-3,-1);
end

function FixString(passedString)
	if (Subplatform == SUB_ANDROID) then
		_tempExtention = GetFileExtention(passedString);
		if (_tempExtention~=".png" and _tempExtention~=".ogg" and _tempExtention~=".wav") then
			return ("/sdcard/Android/data/" .. ANDROIDPACKAGENAME .. "/" .. passedString);
		else
			return passedString;
		end
	elseif (Platform==PLAT_VITA) then
		return ("app0:" .. passedString);
	elseif (Platform==PLAT_WINDOWS) then
		return ("./" .. passedString);
	elseif (Platform==PLAT_3DS) then
		return ("/3ds/data/HappyLand/" .. passedString);
	end
end

-- Removes the image at this spot and makes it not solid and removes its block event
function HideTile(x, y, layer)
	SetMapImageData(x,y,layer,0,0);
	SetMapOtherData(x,y,false,0);
end

-- Restores the HP and MP of everybody in the party. This is often used after boss fights that the player has lost so the battle won't instantly end if the player retries without healing
function RestoreEntireParty()
	for i=0,GetPartySize()-1 do
		RestorePartyMember(i);
	end
end

-- Only argument is the party member special id
-- Please reference the party member using TYPE_ID
function AddPartyMember(_specialID)
	-- Will return 1 if there is 1 party member in the party
	mattslot = GetPartySize();
	if (mattslot>=4) then
		print("oh no, we're out of slots. R.I.P world.\n");
		DebugMsg("oh no, we're out of party member slots. R.I.P world. We already have " .. mattslot .. "/4 slots.");
	end
	-- But, remember, slots are 0 based. So GetPartySize() is actually the next free slot
	SetSpecialId(mattslot,_specialID);
	-- We need to do this too. Increase party size to, for example, 2.
	SetPartySize(mattslot+1);
end

-- Nathan, id 1
function AddPartyMember1()
	AddPartyMember(1);

	tempidle = GetPartyMemberAnimation(1,ANIMATION_IDLE, TYPE_ID);
	tempatk = GetPartyMemberAnimation(1,ANIMATION_ATTACK, TYPE_ID);
	SetAnimation(tempidle,35,16,32,-1,true,0,0,LoadPNG(FixString("Stuff/Battle/PlayerIdle.png")));
	SetAnimation(tempatk,3,44,39,-1,true,0,0,LoadPNG(FixString("Stuff/Battle/PlayerAttack.png")));
	SetStats(GetPartyMemberStats(1, TYPE_ID),1,35,10,10,10,10,10,5,0,MallocString("Nathan"));
	RestorePartyMember(1, TYPE_ID);
	SetStatsSpells(GetPartyMemberStats(1,TYPE_ID),3);
	tempidle=nil;
	tempatk=nil;
end

-- Matt, id 2
function AddPartyMember2()
	AddPartyMember(2);

	tempidle = GetPartyMemberAnimation(2,ANIMATION_IDLE, TYPE_ID);
	tempatk = GetPartyMemberAnimation(2,ANIMATION_ATTACK, TYPE_ID);
	SetAnimation(tempidle,30,29,68,-1,false,0,0,LoadPNG(FixString("Stuff/Battle/MattIdle.png")));
	SetAnimation(tempatk,10,56,68,-1,true,0,0,LoadPNG(FixString("Stuff/Battle/MattAttack.png")));
	SetStats(GetPartyMemberStats(2, TYPE_ID),1,35,20,8,8,15,15,7,0,MallocString("Matt"));
	RestorePartyMember(2, TYPE_ID);
	tempidle=nil;
	tempatk=nil;
end

--[[
	EXAMPLE PARTY MEMBER CODE
	function AddPartyMemberSPECIALIDHERE()
		_theSpecialID = SPECIALIDHERE
		AddPartyMember(SPECIALIDHERE);
		tempidle = GetPartyMemberAnimation(_theSpecialID,ANIMATION_IDLE, TYPE_ID);
		tempatk = GetPartyMemberAnimation(_theSpecialID,ANIMATION_ATTACK, TYPE_ID);
		SetAnimation(tempidle,30,29,68,-1,false,0,0,LoadPNG(FixString("Stuff/Battle/MattIdle.png")));
		SetAnimation(tempatk,10,56,68,-1,true,0,0,LoadPNG(FixString("Stuff/Battle/MattAttack.png")));
		SetStats(GetPartyMemberStats(_theSpecialID, TYPE_ID),1,35,20,8,8,15,15,7,0,MallocString("Matt"));
		RestorePartyMember(_theSpecialID, TYPE_ID);
		tempidle=nil;
		tempatk=nil;
		_theSpecialID=nil;
	end
]]

-- Called when game starts
flags = {};
-- Blottle used
flags[1]=0;
-- Matt hiered
flags[2]=0;
-- tree fought
flags[3]=0;
-- noob bigfoot beat
flags[4]=0;
-- Denied Matt's join request
flags[5]=0;

numberOfFlags=#flags+1;

-- MAKE NATHAN
AddPartyMember1();

--[[

	In the code, GetSpell is 0 based. When giving players/monsters spells, not zero based. Ice starts at 1.

	SKILL LIST

	 1 - Ice
	 2 - Drink Water
	 3 - Heal
	 4 - Throw Tree
	 5 - Carrot Drill

]]
SPELLSPECIAL_NONE = 0;
SPELLSPECIAL_NODEFENCE = 1;

-- Add spell ice
animationSpell1 = GetSpellAnimation(GetSpell(0));
SetAnimation(animationSpell1,7,32,32,4,false,0,0);
if (Lang==1) then
	SetSpell(GetSpell(0),MallocString("Ice"),0,12,MallocString(FixString("Stuff/Spell/Ice.png")),6);
elseif (Lang==2) then
	SetSpell(GetSpell(0),MallocString("Hielo"),0,12,MallocString(FixString("Stuff/Spell/Ice.png")),6);
end

-- Add Drink Water
AddSpell();
animationSpell1 = GetSpellAnimation(GetSpell(1));
SetAnimation(animationSpell1,8,32,32,5,false,64,0);
if (Lang==1) then
	SetSpell(GetSpell(1),MallocString("Drink Water"),-10,0,MallocString(FixString("Stuff/Spell/DrinkWater.png")),0,1);
elseif (Lang==2) then
	SetSpell(GetSpell(1),MallocString("Beber Agua"),-10,0,MallocString(FixString("Stuff/Spell/DrinkWater.png")),0,1);
end

-- Add heal
AddSpell();
animationSpell1 = GetSpellAnimation(GetSpell(2));
SetAnimation(animationSpell1,6,32,32,4,false,0,0);
if (Lang==1) then
	SetSpell(GetSpell(2),MallocString("Heal"),-20,0,MallocString(FixString("Stuff/Spell/Heal.png")),3,SPELLSPECIAL_NODEFENCE);
elseif (Lang==2) then
	SetSpell(GetSpell(2),MallocString("Curar"),-20,0,MallocString(FixString("Stuff/Spell/Heal.png")),3,SPELLSPECIAL_NODEFENCE);
end

-- Add Throw Tree
AddSpell();
animationSpell1 = GetSpellAnimation(GetSpell(3));
SetAnimation(animationSpell1,6,45,45,6,false,-160,-160);
if (Lang==1) then
	SetSpell(GetSpell(3),MallocString("Throw Tree"),16,0,MallocString(FixString("Stuff/Spell/ThrowTree.png")),10);
elseif (Lang==2) then
	SetSpell(GetSpell(3),MallocString("Lanzar 'arbol"),16,0,MallocString(FixString("Stuff/Spell/ThrowTree.png")),10);
end

-- Add Carrot Drill
AddSpell();
animationSpell1 = GetSpellAnimation(GetSpell(4));
SetAnimation(animationSpell1,6,40,40,4,false,-100,-100);
if (Lang==1) then
	SetSpell(GetSpell(4),MallocString("Carrot Drill"),0,16,MallocString(FixString("Stuff/Spell/CarrotDrill.png")),10);
elseif (Lang==2) then
	-- Taladro De Zanahoria??
	SetSpell(GetSpell(4),MallocString("Zanahoria"),0,16,MallocString(FixString("Stuff/Spell/CarrotDrill.png")),10);
end