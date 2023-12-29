# AsteroidClone
Another small assignment project using the Raylib library. This time making an asteroid clone.

I seem to have lost my implementation for acceleration and decceleration. Right now it's tank controls with no inertia. That's fairly easy to add, so will be fun to try to reimplement this in the near future to give it a better game feel.

The main challenge of this assignment was using trigonometric functions to create asteroids that would spawn in a circle around the map center, and would be given a bearing towards the middle of the screen with a slight offset. A similar function is also used when an asteroid is shot to make it spawns smaller asteroids to move in a random direction away from the explosion.

There was also some work with ensuring that when asteroids spawn that the spawning and deleting of entities occured in the correct order so that the size of the list of entities didn't change during an update cycle.

It's a similar project to the Shoot Rock clone I made previously, and I even used the same assets for the project.

If I were to revisit this game here's what I would change:

1) I want to create my own assets to give it a bit of a "made by me" touch.

2) Reimplement the acceleration and deceleration scrips so the movement has a gravity-less feel to it.

3) Add collision between asteroids to add extra chaos.

4) Implement a hiscore system.

5) Consider implementing some enemies and some power-up mechanics.