#ifndef NATHANHAPPYCONFIG
#define NATHANHAPPYCONFIG
	
	#define PLAT_VITA 1
	#define PLAT_WINDOWS 2
	#define PLAT_3DS 3
	
	#define SUB_NONE 0
	#define SUB_ANDROID 1
		
	#define SND_NONE 0
	#define SND_SDL 1

	#define REND_UNDEFINED 0
	#define REND_SDL 1
	#define REND_VITA2D 2
	#define REND_SF2D 3
	
	#define TEXT_UNDEFINED 0
	#define TEXT_DEBUG 1
	#define TEXT_VITA2D 2
	#define TEXT_FONTCACHE 3
	
	// Avalible presets
	#define PRE_NONE 0
	#define PRE_WINDOWS 1
	#define PRE_VITA 2
	#define PRE_3DS 3

	// Change this to change platform
	#define PRESET PRE_WINDOWS

	#if PRESET == PRE_WINDOWS
		#define PLATFORM PLAT_WINDOWS
		#define SUBPLATFORM SUB_NONE
		#define SOUNDPLAYER SND_SDL
		#define RENDERER REND_SDL
		#define TEXTRENDERER TEXT_FONTCACHE
		#define SUBPLATFORM SUB_NONE
	#elif PRESET == PRE_VITA
		#define PLATFORM PLAT_VITA
		#define SUBPLATFORM SUB_NONE
		#define SOUNDPLAYER SND_NONE
		#define RENDERER REND_VITA2D
		#define TEXTRENDERER TEXT_DEBUG
		#define SUBPLATFORM SUB_NONE
	#elif PRESET == PRE_3DS
		#define PLATFORM PLAT_3DS
		#define SUBPLATFORM SUB_NONE
		#define SOUNDPLAYER SND_NONE
		#define RENDERER REND_SF2D
		#define TEXTRENDERER TEXT_DEBUG
		#define SUBPLATFORM SUB_NONE
	#else
		// Put custom stuff here
		// #define PLATFORM a
		// #define SUBPLATFORM a
		// #define SOUNDPLAYER a
		// #define RENDERER a
		// #define TEXTRENDERER a
	#endif

#endif