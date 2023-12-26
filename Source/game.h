#pragma once
#include "raylib.h"
#include "level.h"

//This headerfile contains the functions for switching between gamestates, and the behaviour pertaining to screen transitions as well as the resources class containing the textures and audio.

struct Resources // This struct will store all the textures and sounds so they can be retrieved from the same place by calling Resources + name of asset list.
{
	static Texture2D player;
	static Texture2D laser;

	static std::vector <Sound> sounds;

	static void LoadResources();	// loads in textures and audio from the ./Media folder on start-up so they are available
	static void UnloadResource();	// unloads textures and audio from memory on shut-down.
};

enum struct GameState
{
	TITLESCREEN = 0,
	GAMEPLAY,
};

struct GameMaster 
{
	GameState gameState;// Gamemaster can store a single gamestate at the time which will determine what is showed on screen
	Level level;		// This is where i store all entities during GAMEPLAY state.

	void StartLevel();	// changes gamestate to gameplay and spawns in a player entity into the level list of entities.
	void EndLevel();	// sets all entities to dead, removes them from list of entities and then sets gamestate to titlescreen
};

