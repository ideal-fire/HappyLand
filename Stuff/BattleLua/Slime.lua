enemyidle0 = GetMapEnemyAnimation(battleEnemyLoadId,1)
enemyatk0 = GetMapEnemyAnimation(battleEnemyLoadId,2)
SetAnimation(enemyidle0,100,32,32,3,false,0,0,LoadPNG("app0:Stuff/Enemies/Slime.png"));
SetAnimation(enemyatk0,20,32,32,4,false,0,0,LoadPNG("app0:Stuff/Enemies/SlimeAttack.png"));
SetStats(GetMapEnemy(battleEnemyLoadId),1,40,255,9,4,1,1,4,5);

SetStatsSpells(GetMapEnemy(battleEnemyLoadId),0);

enemyidle0=nil;
enemyatk0=nil;