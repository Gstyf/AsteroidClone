#include "level.h"
#include "game.h"
#include "myMath.h"
#include "raymath.h"


void Level::ShootLaser()
{
	if(IsKeyPressed(KEY_Z))
	{
		SpawnEntity(EntityKind::PROJECTILE,
			Vector2{ (allEntities[0]->position.x), ((allEntities[0]->position.y) - 10) });
		PlaySoundMulti(Resources::sounds[1]);
	}
}

void Level::AddScore()
{
	score += (50 + (10 * (scoreBonus)));
	scoreBonus++;
	scoreBonusCounter = 0;
}

void Level::AddEntity(Entity temp)
{
	entityList.push_back(temp);
	allEntities.push_back(&entityList.back());
}

void Level::RemoveEntity()
{ 
	auto dead_entity = std::remove_if(allEntities.begin(), allEntities.end(), [](const Entity* e) -> bool { return e->isDead; });
	allEntities.erase(dead_entity, allEntities.end());
	entityList.remove_if([](const Entity& e) -> bool { return e.isDead; });
}

void Level::SpawnEntity(EntityKind e, Vector2 v)
{
	Entity tEntity;
	tEntity.kind = e;
	tEntity.position = v;
	switch (e)
	{
	case EntityKind::PLAYER:
		tEntity.size = 40;
		tEntity.speed = 4;
		tEntity.rectangle = {0, 0, 99, 75};
		tEntity.center = { 99 / 2, 75 / 2 };
		break;
	case EntityKind::PROJECTILE:
		tEntity.size = 6;
		tEntity.speed = 15;
		tEntity.rectangle = {0, 0, 13, 37};
		tEntity.center = { 6.5, 18.5 };
		tEntity.rotation = allEntities[0]->rotation;
		tEntity.direction = allEntities[0]->direction;
		break;
	case EntityKind::BIGROCK:
		tEntity.size = 90;
		tEntity.speed = 2;
		tEntity.direction = { NormalisedHeading(tEntity.position) }; // Heads the rocks towards the center
		break;
	case EntityKind::ROCK:
		tEntity.size = 45;
		tEntity.speed = 4;
		tEntity.direction = { sinf(GetRandomValue(0,360) * (PI / 180)), cosf(GetRandomValue(0,360) * (PI / 180)) }; // gives rocks their random trajectory
		break;
	case EntityKind::SMALLROCK:
		tEntity.size = 20;
		tEntity.speed = 6;
		tEntity.direction = { sinf(GetRandomValue(0,360) * (PI / 180)), cosf(GetRandomValue(0,360) * (PI / 180)) }; // gives rocks their random trajectory
		break;
	case EntityKind::SCORE:
		tEntity.size = 30;  // Used here as a counter in update. Once 0 it will disappear.
		tEntity.speed = 5;
		tEntity.direction = { -1,-1 }; // Score moves diagonally away from player and upwards.
		tEntity.score = 50 + (10 * scoreBonus); // this is the number that will be read out when shooting rocks.
		break;
	default:
		break;
	}
	AddEntity(tEntity);
}

void Level::AddFutureEntity(EntityKind e, Vector2 v)
{
	Entity fEntity;
	fEntity.kind = e;
	fEntity.position = v;
	futureEntities.push_back(fEntity);
}

void Level::SpawnFutureEntity()
{
	for (const Entity& e : futureEntities) // retrieves the entities in futureEntities by address instead of copying them over. Saves memory.
	{
		if  (e.kind == EntityKind::SCORE)
		{
			SpawnEntity(e.kind, e.position);
		}
		else if (e.kind == EntityKind::SMALLROCK)
		{
			SpawnEntity(e.kind, e.position);
		}
		else if (e.kind == EntityKind::ROCK)
		{
			SpawnEntity(e.kind, e.position);
		}
	}
	futureEntities.clear();
}

Vector2 Level::CircularSpawnPoint()
{
	Vector2 Center = { GetScreenWidth() / 2, GetScreenHeight() / 2 };
	float angle = GetRandomValue(0, 360);
	Vector2 Point =
	{
		Center.x + (GetScreenWidth() * cosf(angle)),
		Center.y + (GetScreenHeight() * sinf(angle))
	};
	return Point;
}

Vector2 Level::NormalisedHeading(Vector2 pos)
{	//RAYLIB ALREADY HAD THIS FUNCTION! Why did I not know about this?
	Vector2 center = { GetScreenWidth() / 2, GetScreenHeight() / 2 };
	Vector2 heading =
	{
		center.x - pos.x,
		center.y - pos.y
	};
	float absoluteValue = sqrtf(heading.x * heading.x + heading.y * heading.y);
	Vector2 normalised =
	{
		heading.x / absoluteValue,
		heading.y / absoluteValue
	};
	return normalised;
}

void Level::Reset()
{
	for (Entity* e : allEntities)
	{
		e->isDead = true;
	}
	RemoveEntity();
	score = 0;
}

float Level::RotatePlayer()
{
	float zRotation = (float)IsKeyDown(KEY_RIGHT) - (float)IsKeyDown(KEY_LEFT);
	return zRotation;
}

float Level::ThrustPlayer()
{
	float movement = (float)(IsKeyDown(KEY_UP));
	return -movement;
}

void Level::ResolveCollision()
{
	for (Entity* e : allEntities)
	{
		for (Entity* c : allEntities)
		{
			switch (e->kind)
			{
			case EntityKind::PLAYER:
				if (c->kind == EntityKind::BIGROCK)
				{
					if (CheckCollisionPointCircle(c->position, e->position, e->size))
					{
						e->isDead = true;
						c->isDead = true;
						PlaySoundMulti(Resources::sounds[0]);
					}
				}
				else if (c->kind == EntityKind::ROCK)
				{
					if (CheckCollisionPointCircle(c->position, e->position, e->size))
					{
						e->isDead = true;
						c->isDead = true;
						PlaySoundMulti(Resources::sounds[0]);
					}
				}
				else if(c->kind == EntityKind::SMALLROCK)
				{
					if (CheckCollisionPointCircle(c->position, e->position, e->size))
					{
						e->isDead = true;
						c->isDead = true;
						PlaySoundMulti(Resources::sounds[0]);
					}
				}
				break;
			case EntityKind::PROJECTILE:
				if (c->kind == EntityKind::BIGROCK)
				{
					if (CheckCollisionPointCircle(e->position, c->position, c->size))
					{
						c->isDead = true;
						e->isDead = true;
						AddScore();
						AddFutureEntity(EntityKind::SCORE, e->position);
						AddFutureEntity(EntityKind::ROCK, c->position + Vector2{ (float)GetRandomValue(-20,20), (float)GetRandomValue(-20,20) });
						AddFutureEntity(EntityKind::ROCK, c->position + Vector2{ (float)GetRandomValue(-20,20), (float)GetRandomValue(-20,20) });
						PlaySoundMulti(Resources::sounds[0]);
					}
				}
				else if (c->kind == EntityKind::ROCK)
				{
					if (CheckCollisionPointCircle(e->position, c->position, c->size))
					{
						c->isDead = true;
						e->isDead = true;
						AddScore();
						AddFutureEntity(EntityKind::SCORE, e->position);
						AddFutureEntity(EntityKind::SMALLROCK, c->position + Vector2{(float)GetRandomValue(-20,20), (float)GetRandomValue(-20,20)});
						AddFutureEntity(EntityKind::SMALLROCK, c->position + Vector2{(float)GetRandomValue(-20,20), (float)GetRandomValue(-20,20)});
						PlaySoundMulti(Resources::sounds[0]);
					}
				}
				else if (c->kind == EntityKind::SMALLROCK)
				{
					if (CheckCollisionPointCircle(e->position, c->position, c->size))
					{
						c->isDead = true;
						AddScore();
						AddFutureEntity(EntityKind::SCORE, e->position);
						PlaySoundMulti(Resources::sounds[0]);
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

bool Level::CheckOutOfBounds(Vector2 v)
{
	if (v.x < -600 || v.x > GetRenderWidth() + 600 || v.y < -600 || v.y > GetRenderHeight() + 600)
	{
		return true;
	}
	else return false;
}

void Level::Update()
{
	for (Entity* e : allEntities) // This for-each with a nested switch-case moves all objects every frame. I use this loop because I can retrieve EntityKind easier (e->kind, rather than allEntities[e]->kind).
	{
		switch (e->kind)
		{
		case EntityKind::PLAYER:
			e->rotation += RotatePlayer() * e->speed;
			e->direction = { -sinf(e->rotation * (PI / 180)), cosf(e->rotation * (PI / 180)) };
			e->position.x += ThrustPlayer() * e->direction.x * e->speed;
			e->position.y += ThrustPlayer() * e->direction.y * e->speed;
			
			//The following statements just returns the player inside bounds if they try to move out of the screen.
			if (e->position.x < -40) { e->position.x = GetScreenWidth() + 20; }
			else if (e->position.x > GetRenderWidth() + 40) { e->position.x = -20; }
			if (e->position.y < -40) { e->position.y = GetScreenHeight() + 20; }
			else if (e->position.y > GetRenderHeight() + 40) { e->position.y = - 20; }
			break;
		case EntityKind::PROJECTILE:
			e->position.y -= e->direction.y * e->speed;
			e->position.x -= e->direction.x * e->speed;
			if (CheckOutOfBounds(e->position)) { e->isDead = true; };
			break;
		case EntityKind::BIGROCK:
			e->position.x += e->direction.x * e->speed;
			e->position.y += e->direction.y * e->speed;
			e->rotation = GetTime() * 90;
			if (CheckOutOfBounds(e->position)) { e->isDead = true; };
			break;
		case EntityKind::ROCK:
			e->position.x += e->direction.x * e->speed;
			e->position.y += e->direction.y * e->speed;
			e->rotation = GetTime() * 120;
			if (CheckOutOfBounds(e->position)) { e->isDead = true; };
			break;
		case EntityKind::SMALLROCK:
			e->position.x += e->direction.x * e->speed;
			e->position.y += e->direction.y * e->speed;
			e->rotation = GetTime() * 150;
			if (CheckOutOfBounds(e->position)) { e->isDead = true; };
			break;
		case EntityKind::SCORE:
			e->position += e->direction * e->speed;
			e->size--;
			if (e->size <= 0) { e->isDead = true; }
			break;
		default:
			break;
		}
	}

	ResolveCollision();
	RemoveEntity();
	ShootLaser();

	if (futureEntities.size() != 0)
	{
		SpawnFutureEntity();
	}

	int nextAsteroidIn = 120;
	frameCounter++;
	if (frameCounter == nextAsteroidIn)
	{
		SpawnEntity(EntityKind::BIGROCK, CircularSpawnPoint());
		frameCounter = 0;
		nextAsteroidIn = GetRandomValue(30, 120);
	}

	scoreBonusCounter++;
	if (scoreBonusCounter > 120)
	{
		scoreBonus = 0;
	}

	Render();
}

void Level::Render()
{
	for (Entity* e : allEntities)
	{
		switch (e->kind)
		{
		case EntityKind::PLAYER:
			DrawTexturePro(Resources::player, e->rectangle,
				{ e->position.x, e->position.y, e->rectangle.width, e->rectangle.height },
				e->center, e->rotation, WHITE);
			break;
		case EntityKind::PROJECTILE:
			DrawTexturePro(Resources::laser, e->rectangle,
				{ e->position.x, e->position.y, e->rectangle.width, e->rectangle.height },
				e->center, e->rotation, WHITE);
			if (CheckOutOfBounds(e->position)) {}
			break;
		case EntityKind::BIGROCK:
			DrawRectanglePro({ e->position.x, e->position.y, (float)e->size, (float)e->size }, 
				{ (float)e->size / 2, (float)e->size / 2 }, e->rotation, WHITE);
			break;
		case EntityKind::ROCK:
			DrawRectanglePro({ e->position.x, e->position.y, (float)e->size, (float)e->size }, 
				{ (float)e->size / 2, (float)e->size / 2 }, e->rotation, WHITE);
			break;
		case EntityKind::SMALLROCK:
			DrawRectanglePro({ e->position.x, e->position.y, (float)e->size, (float)e->size }, 
				{ (float)e->size / 2, (float)e->size / 2 }, e->rotation, WHITE);
			break;
		case EntityKind::SCORE:
			DrawText(TextFormat("%i", e->score), e->position.x, e->position.y, 20, RAYWHITE);
			break;
		default:
			break;
		}
	}
	DrawText(TextFormat("Score: %i", score), ((GetRenderWidth() / 5) * 2), 20, 20, LIGHTGRAY);
}



