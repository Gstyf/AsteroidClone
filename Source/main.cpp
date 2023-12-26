/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*b
*   Copyright (c) 2013-2022 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "game.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 900;
	const int screenHeight = 900;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
	InitAudioDevice();              // Initializes audio so we can head anything at all.

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	GameMaster gameMaster = { GameState::TITLESCREEN };	// Initialise a master to activate game functions.
	Resources::LoadResources();

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();
		switch (gameMaster.gameState)
		{
		case GameState::TITLESCREEN:
			if (IsKeyPressed(KEY_X))
			{
				gameMaster.StartLevel();
			}
			DrawText("Gurra's Asteroids!", (GetScreenWidth() / 5), (GetScreenHeight() / 3), 60, RED);
			DrawText("Press 'X' to start!", (GetScreenWidth() / 3), (GetScreenHeight() / 2), 30, LIGHTGRAY);
			DrawText("Press 'Q' for Main menu!", (GetScreenWidth() / 10  *  3), ((GetScreenHeight() / 10) * 6), 30, LIGHTGRAY);
			DrawText("'Up Key' to Accelerate!", ((GetScreenWidth() / 10) * 3), ((GetScreenHeight() / 10) * 7), 30, LIGHTGRAY);
			DrawText("'Left Key' & 'Right Key' to Rotate!", ((GetScreenWidth() / 10) * 2), ((GetScreenHeight() / 10) * 8), 30, LIGHTGRAY);
			DrawText("Press 'Z' fire!", ((GetScreenWidth() / 10) * 4), ((GetScreenHeight() / 10) * 9), 30, LIGHTGRAY);
			break;
		case GameState::GAMEPLAY:
			gameMaster.level.Update();
			if (gameMaster.level.allEntities[0]->kind != EntityKind::PLAYER) 
			{
				// if after update the first entity isn't player it means player is dead, so return to titlescreen
				gameMaster.EndLevel(); 
			}
			if (IsKeyPressed(KEY_Q)) //Q for quit, get it?
			{
				gameMaster.EndLevel();
			}
			break;
		default: // you shouldn't be able to get here.
			break;
		}
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------//
	Resources::UnloadResource();
	CloseAudioDevice();     // Close Audio Device
	CloseWindow();        // Close window and OpenGL context
	//-------------------------------------------------------------------------------------->>

	return 0;
}