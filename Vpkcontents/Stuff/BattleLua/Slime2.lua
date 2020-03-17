function setupSlimeStats(id)
   local scaleStat;
   if (GetLevel(0)<40) then
	  scaleStat=99; -- force the player back to snow land
   else
	  scaleStat=GetLevel(0)-50;
   end
   -- // char level;
   -- // short maxHp;
   -- // short maxMp;
   -- // unsigned char attack;
   -- // unsigned char defence;
   -- // unsigned char magicAttack;
   -- // unsigned char magicDefence;
   -- // unsigned char speed;
   -- // short exp;
   SetStats(GetMapEnemy(id),1,80*(1+scaleStat*.1),255,1000*(1+scaleStat*.1),24*(1+scaleStat*.1),84*(1+scaleStat*.1),40*(1+scaleStat*.1),32*(1+scaleStat*.1),255*(1+scaleStat*.1));
   scaleStat=nil;
end

enemyidle0 = GetMapEnemyAnimation(battleEnemyLoadId,1)
enemyatk0 = GetMapEnemyAnimation(battleEnemyLoadId,2)
SetAnimation(enemyidle0,30,32,32,3,false,0,0,LoadPNG(FixString("Stuff/Enemies/Slime2.png")));
SetAnimation(enemyatk0,20,32,32,4,false,0,0,LoadPNG(FixString("Stuff/Enemies/Slime2Attack.png")));
-- the slime scales with your level!
setupSlimeStats(battleEnemyLoadId);

SetStatsSpells(GetMapEnemy(battleEnemyLoadId),1);

enemyidle0=nil;
enemyatk0=nil;
