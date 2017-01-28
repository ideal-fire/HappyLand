PLAT_VITA = 1
PLAT_WINDOWS = 2

function FixString(passedString)
	if (Platform==PLAT_VITA) then
		return ("app0:" .. passedString);
	elseif (Platform==PLAT_WINDOWS) then
		return ("./" .. passedString);
	end
end

print("platform is " .. Platform);

-- Called when game starts
--[[
 0 - Used big water bottle (0 or 1)
]]
flags = {};
-- Blottle used
flags[0]=0;
-- Matt hiered
flags[1]=0;
-- tree fought
flags[2]=0;
-- noob bigfoot beat
flags[3]=0;

-- MAKE NATHAN
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
