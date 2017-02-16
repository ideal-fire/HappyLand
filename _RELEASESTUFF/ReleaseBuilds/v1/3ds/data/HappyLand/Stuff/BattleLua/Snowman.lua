enemyidle0 = GetMapEnemyAnimation(battleEnemyLoadId,1);
enemyatk0 = GetMapEnemyAnimation(battleEnemyLoadId,2);
SetAnimation(enemyidle0,10,50,50,3,false,0,0,LoadPNG(FixString("Stuff/Enemies/Snowman.png")));
SetAnimation(enemyatk0,5,50,55,6,false,0,0,LoadPNG(FixString("Stuff/Enemies/SnowmanAttack.png")));
SetStats(GetMapEnemy(battleEnemyLoadId),255,20,255,21,14,18,19,16,15);

SetStatsSpells(GetMapEnemy(battleEnemyLoadId),5);

enemyidle0=nil;
enemyatk0=nil;