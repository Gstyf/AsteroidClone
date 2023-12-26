#include "game.h"

Texture2D Resources::player;
Texture2D Resources::laser;

std::vector <Sound> Resources::sounds;

void Resources::LoadResources() 
{
	player = LoadTexture("./Media/playerShip1_red.png");

	laser = LoadTexture("./Media/laserRed02.png");

	sounds.push_back(LoadSound("./Media/explosionCrunch_000.ogg")); 
	sounds.push_back(LoadSound("./Media/laserLarge_000.ogg"));
	sounds.push_back(LoadSound("./Media/laserRetro_001.ogg"));
	sounds.push_back(LoadSound("./Media/explosionCrunch_002.ogg"));
}

void Resources::UnloadResource()
{
	UnloadTexture(player);

	UnloadTexture(laser);

	for (int i = 0; i < sounds.size(); i++)
	{
		StopSoundMulti();
		UnloadSound(sounds[i]);
	}
}

void GameMaster::StartLevel()
{
	PlaySoundMulti(Resources::sounds[2]);
	gameState = GameState::GAMEPLAY;
	level.SpawnEntity(EntityKind::PLAYER, Vector2{ ((float)GetRenderWidth() / 2), ((float)GetRenderHeight() / 2) });
}

void GameMaster::EndLevel()
{
	PlaySoundMulti(Resources::sounds[3]);
	level.Reset();
	gameState = GameState::TITLESCREEN;
}
