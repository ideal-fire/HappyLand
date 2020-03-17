function setupSnowmanStats()
   if (flags[8]==1) then
	  if (flags[7]==1) then
		 -- strongest snowman varient
		 -- this is justified by saying that the player had plenty of extra EXP before this point
		 SetStats(GetMapEnemy(battleEnemyLoadId),255,20*5,255,120,14*2,84,19*2,16*5,17*12);
	  else
		 -- slightly stronger snowman varient
		 SetStats(GetMapEnemy(battleEnemyLoadId),255,20,255,21*2,14*2,18*2,19*2,16*2,25);
	  end
   else
	  if (flags[7]==1) then
		 -- secret mission stronger snowman
		 SetStats(GetMapEnemy(battleEnemyLoadId),255,20*4,255,105,12*2,84,19*2,16*5,17*12);
	  else
		 -- normal snowman varient
		 SetStats(GetMapEnemy(battleEnemyLoadId),255,20,255,21,14,18,19,16,15);
	  end
   end
end
enemyidle0 = GetMapEnemyAnimation(battleEnemyLoadId,1);
enemyatk0 = GetMapEnemyAnimation(battleEnemyLoadId,2);
SetAnimation(enemyidle0,10,50,50,3,false,0,0,LoadPNG(FixString("Stuff/Enemies/Snowman.png")));
SetAnimation(enemyatk0,5,50,55,6,false,0,0,LoadPNG(FixString("Stuff/Enemies/SnowmanAttack.png")));

setupSnowmanStats();

SetStatsSpells(GetMapEnemy(battleEnemyLoadId),5);

enemyidle0=nil;
enemyatk0=nil;
