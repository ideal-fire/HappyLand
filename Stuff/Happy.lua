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
SetAnimation(tempidle,35,16,32,-1,true,0,0,LoadPNG("app0:Stuff/Battle/PlayerIdle.png"));
SetAnimation(tempatk,3,44,39,-1,true,0,0,LoadPNG("app0:Stuff/Battle/PlayerAttack.png"));
SetStats(GetPartyMemberStats(0),1,35,10,10,10,10,10,5,0,MallocString("Nathan"));
SetPartySize(1);
RestorePartyMember(0);
tempidle=nil;
tempatk=nil;
SetStatsSpells(GetPartyMemberStats(0),3);


--[[

	SKILL LIST

	ADD 1 TO THESE VALUES IF ADDING TO PLAYER STATS
	DONT IF ADDING TO ENEMY

	 0 - Ice
	 1 - Drink Water
	 2 - Heal

]]

-- Add spell ice
animationSpell1 = GetSpellAnimation(GetSpell(0));
SetAnimation(animationSpell1,60,32,32,2,false,0,0);
if (Lang==1) then
	SetSpell(GetSpell(0),MallocString("Ice"),0,12,MallocString("app0:Stuff/Spell/Ice.png"),5);
elseif (Lang==2) then
	SetSpell(GetSpell(0),MallocString("Hielo"),0,12,MallocString("app0:Stuff/Spell/Ice.png"),5);
end

--[[
TODO - Make this drink water bottle
]]

AddSpell();
animationSpell1 = GetSpellAnimation(GetSpell(1));
SetAnimation(animationSpell1,30,32,64,2,false,0,0);
if (Lang==1) then
	SetSpell(GetSpell(1),MallocString("Drink Water"),12,24,MallocString("app0:Stuff/TestSpell2.png"),10);
elseif (Lang==2) then
	SetSpell(GetSpell(1),MallocString("Beber Agua"),12,24,MallocString("app0:Stuff/TestSpell2.png"),10);
end


-- Add heal
AddSpell();
animationSpell1 = GetSpellAnimation(GetSpell(2));
SetAnimation(animationSpell1,30,32,32,2,false,0,0);
if (Lang==1) then
	SetSpell(GetSpell(2),MallocString("Heal"),-30,0,MallocString("app0:Stuff/Spell/Heal.png"),3,1);
elseif (Lang==2) then
	SetSpell(GetSpell(2),MallocString("Curar"),12,24,MallocString("app0:Stuff/Spell/Heal.png"),3,1);
end
