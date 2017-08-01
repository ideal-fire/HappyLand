enemyidle0 = GetMapEnemyAnimation(battleEnemyLoadId,1)
enemyatk0 = GetMapEnemyAnimation(battleEnemyLoadId,2)
SetAnimation(enemyidle0,30,32,32,3,false,0,0,LoadPNG(FixString("Stuff/Enemies/Slime.png")));
SetAnimation(enemyatk0,20,32,32,4,false,0,0,LoadPNG(FixString("Stuff/Enemies/SlimeAttack.png")));
SetStats(GetMapEnemy(battleEnemyLoadId),1,40,255,9,4,7,5,4,6); -- old exp is 5

SetStatsSpells(GetMapEnemy(battleEnemyLoadId),1);

enemyidle0=nil;
enemyatk0=nil;