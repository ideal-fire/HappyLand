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
char* N_SAVE;
char* N_LOAD;

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
		N_SAVE = "Save";
		N_LOAD = "Load";
	}
	if (LANGUAGE==LANG_SPANISH){
		// (The Magic List);
		N_MAGICLIST = "La Lista Mágico";
		// You need more MP;
		N_NEEDMOREMP = "Necesitas más MP.";
		// Need acsent on tu;
		// You died.;
		N_DEADMESSAGE = "Tú moriste.";
		// The Happy Menu;
		N_HAPPYMENU = "El Menú Feliz";
		// Next 3 from Google Translate. Probrablly all right.;
		N_RESUME = "Continuar";
		N_QUIT = "Dejar";
		N_OPTIONS = "Opciones";
		// You won;
		N_WIN = "Tú Ganaste";
		N_ATK = "Ataque";
		N_DEF = "Defensa";
		N_MATK = "Ataque Mágico";
		N_MDEF = "Defensa Mágico";
		N_SPEED = "Velocidad";
		N_HP = "HP";
		N_MP = "MP";
		// I don't know how to say 'leveled up';
		// I'll just settle for the next best thing;
		// is more strong!;
		N_LEVELEDUP = "es más fuerte!";
		N_LV = "LV";
		N_SAVE = "Guardar";
		N_LOAD = "Cargar";
	}
}


#endif