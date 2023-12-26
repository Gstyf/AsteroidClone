#pragma once
#include "raylib.h"
#include <vector>

// This header file defines the class Entity.

enum class EntityKind
{
	NONE = 0, //I wonder if this is actually useful to include at all, ever, or if we have a 0 just for the hell of it.
	PLAYER,
	PROJECTILE,
	ROCK,
	BIGROCK,
	SMALLROCK,
	SCORE,
};

class Entity
{
public:
	EntityKind	kind = {};			// Used to teremine behaviour of object during update & collision.
	int			size = {};			// Determines the "hitbox" of the object, not the size of it's texture. It's a radius of a circle.
	int			score = {};			// Only used for SCORE entities to show how many points a coin awarded
	Vector2		position = {};		// Where in the gameworld the object is.
	Vector2		direction = {};		// The direction the object is heading when it isn't going straight down. Also a normal for movement/launch direction
	float		rotation = {};		// Current rotation amount (as degrees)
	int			texture = 0;		// Which texture to render (for Entities that can have different ones). Not used for all entities.
	Rectangle	rectangle = {};		// Rectangle area of the sprite/texture being used.
	Vector2		center = {};		// The centre of the texture/sprite
	float		speed = {};			// How many pixels it moves per update.
	bool		isDead = false;		// Whether the entity is dead and should be removed from the game and memory at end of update
};