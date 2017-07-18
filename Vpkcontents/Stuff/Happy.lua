PLAT_VITA = 1;
PLAT_WINDOWS = 2;
PLAT_3DS = 3;

function SetFlag(index,val)
	flags[index]=val;
end

function FixString(passedString)
	if (Platform==PLAT_VITA) then
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

-- Nathan
function AddPartyMember1()
	tempidle = GetPartyMemberAnimation(0,1)
	tempatk = GetPartyMemberAnimation(0,2)
	SetAnimation(tempidle,35,16,32,-1,true,0,0,LoadPNG(FixString("Stuff/Battle/PlayerIdle.png")));
	SetAnimation(tempatk,3,44,39,-1,true,0,0,LoadPNG(FixString("Stuff/Battle/PlayerAttack.png")));
	SetStats(GetPartyMemberStats(0),1,35,10,10,10,10,10,5,0,MallocString("Nathan"));
	SetPartySize(1);
	RestorePartyMember(0);
	tempidle=nil;
	tempatk=nil;
	SetStatsSpells(GetPartyMemberStats(0),3);
end

-- Matt
function AddPartyMember2()
	mattslot = GetPartySize();
	tempidle = GetPartyMemberAnimation(mattslot,1)
	tempatk = GetPartyMemberAnimation(mattslot,2)

	SetAnimation(tempidle,30,29,68,-1,false,0,0,LoadPNG(FixString("Stuff/Battle/MattIdle.png")));
	SetAnimation(tempatk,10,56,68,-1,true,0,0,LoadPNG(FixString("Stuff/Battle/MattAttack.png")));
	SetStats(GetPartyMemberStats(mattslot),1,35,20,8,8,15,15,7,0,MallocString("Matt"));
	SetPartySize(mattslot+1);
	RestorePartyMember(mattslot);

	tempidle=nil;
	tempatk=nil;
	mattslot=nil;
end

-- Called when game starts
--[[
 0 - Used big water bottle (0 or 1)
]]
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
	SetSpell(GetSpell(2),MallocString("Heal"),-20,0,MallocString(FixString("Stuff/Spell/Heal.png")),3,1);
elseif (Lang==2) then
	SetSpell(GetSpell(2),MallocString("Curar"),-20,0,MallocString(FixString("Stuff/Spell/Heal.png")),3,1);
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
