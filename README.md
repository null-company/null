# null

## Buidling

Debian-like OS: `sudo apt-get install build-essential protobuf-compiler xorg-dev libudev-dev libvorbis-dev libalut-dev libflac-dev`

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
cmake --build . --target NULL_GAME NULL_GAME_SERVER
./NULL_GAME # keep in mind you are not supposed to launch it from root of the project
```

Windows: use [vcpkg](https://github.com/microsoft/vcpkg) to install protobuf and then use vcpkg as cmake toolchain. Also, Windows users are supposed to have openal32.dll in their wd or on PATH to run the application. You can copy one from extlibs/bin.

## Game controls
*(for as long as we do not have a help in-game)*

 - Pick up a weapon using **T**
 - Change weapons using **E**
 - Shoot by **LMB**
 - Move using **WASD** and **SPACE**


## Team members
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;* **Brek Roman**, 19213 - r.brek@g.nsu.ru

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; **Vasilev Pavel**, 19213 - p.vasilev@g.nsu.ru

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; **Patrushev Borya**, 19213 -  b.patrushev@g.nsu.ru

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; **Tarasov Artёm**, 19214 -  a.tarasov5@g.nsu.ru

## Project vision
The project is a dynamic 2d action pvp-multiplayer platformer with lots of fast rounds. The environment in the game
is dynamic and players can interact with it.
The game is located in cyberpunk setting, and the graphic style can be described as "pixel art".
Since the game is situated in a cyberpunk setting, there are augmentations on the map for characters.
The augmentations can be picked up by the characters (augmentations is another word for "perk"). 
And the cyberpunk style is reflected in the style of the weapons, both melee and ranged.

## Project risks

 1. We won't understand how to properly get SFML to work.
    Possible solution: Switch to SDL or any other library.
 2. We won't be able to make the graphics as good as we imagined it when making the vision.
    Possible solution: buy art from an artist.
 3. The networking won't work as good as we expected.
    Possible solution: consult local professionals
 4. Won't be able to escape vim.
    Possible solution: turn off electricity.

## MVP Release

MVP Release is end of 1st semester
MVP Features:

 1. At least one map.

 2. The game has multiplayer (at least 2 players can be in a game).
 
 3. One melee and one ranged weapon (/augmentaions).
 
 4. First version of the main menu and UI, with which you can create a game and join a game

## First project release

End of second semester

## Trello
[null-board](https://trello.com/b/AjHWnnq4/title-null)
