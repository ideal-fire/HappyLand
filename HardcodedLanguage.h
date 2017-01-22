#ifndef HAPPYLANG
#define HAPPYLANG

char* N_MAGICLIST;
char* N_NEEDMOREMP;
char* N_DEADMESSAGE;
char* N_HAPPYMENU;
char* N_RESUME;
char* N_QUIT;
char* N_OPTIONS;
char* N_WIN;
char* N_LEVELEDUP;
char* N_ATK;
char* N_DEF;
char* N_MATK;
char* N_MDEF;
char* N_SPEED;
char* N_HP;
char* N_MP;
char* N_LV;

void SetupHardcodedLanguage(){
	if (LANGUAGE==LANG_ENGLISH){
		N_MAGICLIST = "Magic List";
		N_NEEDMOREMP = "You don't have enough MP.";
		N_DEADMESSAGE = "You're a dead noob.";
		N_HAPPYMENU = "Happy Menu";
		N_RESUME = "Not Quit";
		N_QUIT = "Quit";
		N_OPTIONS = "Options";
		N_WIN = "WIN";
		N_LEVELEDUP = "leveled up!";
		N_ATK = "Attack";
		N_DEF = "Defence";
		N_MATK = "Magic Attack";
		N_MDEF = "Magic Defence";
		N_SPEED = "Speed";
		N_HP = "Health";
		N_MP = "MP";
		N_LV = "Level";
	}
	if (LANGUAGE==LANG_SPANISH){
		// NEed acsent on a;
		// (The Magic List);
		N_MAGICLIST = "La Lista M'agico";
		// Need acsent on mas;
		// You need more MP;
		N_NEEDMOREMP = "Necesitas m'as MP.";
		// Need acsent on tu;
		// You died.;
		N_DEADMESSAGE = "T'u moriste.";
		// The Happy Menu;
		N_HAPPYMENU = "El Men'u Feliz";
		// Next 3 from Google Translate. Probrablly all right.;
		N_RESUME = "Continuar";
		N_QUIT = "Dejar";
		N_OPTIONS = "Opciones";
		
		// You won;
		N_WIN = "T'u Ganaste";
		
		N_ATK = "Ataque";
		N_DEF = "Defensa";
		N_MATK = "Ataque M'agico";
		N_MDEF = "Defensa M'agico";
		N_SPEED = "Velocidad";
		N_HP = "HP";
		N_MP = "MP";
		// I don't know how to say 'leveled up';
		// I'll just settle for the next best thing;
		// is more strong!;
		N_LEVELEDUP = "es m'as fuerte!";
		
		N_LV = "LV";
	}
}


#endif