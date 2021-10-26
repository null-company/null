# Modules

1. Game-engine-like library 
2. Physics engine
3. Rendering wrapper
4. Game logic
5. Map editor
6. Networking

# Project structure

All files are divided to HEADERS and SOURCES.

SOURCES can be found at src/

HEADERS can be found at include/

Both directories then divide for module names, e.g. Main, GameEngine, etc.


# GameEngine

## Scene

A **Scene** is a (somewhat) serializable piece of data which dictates what a game level is (it's structure,
what it looks like etc). Essentially, it defines a tree of GameObjects

Scenes are stored as files and are read by SceneLoader.


## GameObject

A **GameObject** is a (somewhat) serializable piece of data which describes some entity within the game with such aspects
as: what it looks like, what its behaviour in the scene is, what physical characteristics it has.
GameObject comes down to be a collection of some Components.

## Component
A **Component** is a part of a GameObject which is aware of the scene and the event loop (e.g. the game has started, 
next frame is about to be drawn, and possibly more) and can manipulate the GameObject it is associated with (i.e. 
it can interact with other components of the GameObject). Because it is associated with the scene, it can interact with
other GameObjects in the Scene's tree.

There are different types of Components. A GameObject can have more than one Component of the same type(?).

### Different Components

Examples of component types: Transform (rotation and coordinates of the object), RigidBody (physics),
Script (GameEngine's user defined behaviour), possibly more?

## SceneLoader

**SceneLoader** is a singleton object / a set of functions in the global scope that can substitute the currently
loaded scene for some other.





