enemyidle0 = GetMapEnemyAnimation(battleEnemyLoadId,1)
enemyatk0 = GetMapEnemyAnimation(battleEnemyLoadId,2)
SetAnimation(enemyidle0,100,32,32,2,false,0,0,LoadPNG("app0:Stuff/Battle/TestEnemyIdle.png"));
SetAnimation(enemyatk0,20,32,32,2,false,0,0,LoadPNG("app0:Stuff/Battle/TestEnemyAttack.png"));
SetStats(GetMapEnemy(0),99,40,0,3,3,1,1,4,32);

enemyidle0=nil;
enemyatk0=nil;