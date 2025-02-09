# ✨ Cozy Cottage ✨

A game where you can explore a wonderful little cottage tucked away in the woods. Rather odd place for a cottage to be you may suppose but none the less you venture in and, alas, the door seems to be jammed. Perhaps there is something in this quaint little place that may pry open that door? It is rather intriguing how small it is, peculiar even. And you can't help but feel as if something is watching you... 

![ccMainImg](https://github.com/thumun/Horror-Game/blob/main/images/ccmainimg.png)


## How to build

**NOTE: This project will only build for mac due to the FMOD**

*Windows*

Open git bash to the directory containing this repository.

```
Horror-Game $ mkdir build
Horror-Game $ cd build
Horror-Game/build $ cmake ..
Horror-Game /build $ start project-template.sln
```

Your solution file should contain four projects.
To run from the git bash command shell, 

```
Horror-Game/build $ ../bin/Debug/demo.exe
```

*macOS*

Open terminal to the directory containing this repository.

```
Horror-Game $ mkdir build
Horror-Game $ cd build
Horror-Game/build $ cmake ..
Horror-Game/build $ make
```

To run each program from build, you would type

```
Horror-Game/build $ ../bin/demo
```

## Idea and Potential Future Mechanics

The idea behind this game is that the player is trapped in a rather small house (two rooms). They are free to explore the space and are able to see via a "flashlight" (spotlight). However, little do they know that there is a monster watching their every move. 

For future mechanics, I was thinking of making it an escape room style game where the player could potentially escape the scene by finding clues hidden throughout the space. This would involve object interactibility. 

## Gameplay 

The game starts off with the player beginning in the room with the fireplace. This room has bookshelves lining two of the walls as well as a few books on the shelves. There is also a viola on the floor. The other room has a portrait of headless figure, two chairs with a table, and an impassable door. The player is free to walk around the space but there is a creepy ambience (due to the music and the spotlight). After 20 seconds, the light starts flashing and after 5 more seconds, the endscreen appears- the jumpscare with the monster (accompanied by jumpscare music). This jumpscare is possible via a transition from perspective view to orthographic view. 

![gameplay](https://github.com/thumun/Horror-Game/blob/main/images/youdied.gif)

*Important Note: Music is in the game but not seen in demo above as it is a gif; there was also something weird going on with the gif- the actual program does not have the blue grid dots*

## Features 

**Flythrough Camera:**

The player navigates the space using the flythrough camera. In order to simulate walking, the player cannot move in the y-axis however they are free to look anywhere they like. 

**Spotlight:**

The spotlight shines wherever the player looks while the surrounding area has a darkened hue. The spotlight itself has a slightly green tone due to the color of the light in combination with the bump map. By increasing the exponent, the light is blurred around the edges. In addition, the angle of the light is 60 degrees which makes the spotlight light up most of the screen. 

After a certain amount of time passes (20 seconds), the spotlight begins to flash on/off until the endscreen happens. 

**Bump Map:**

The bump map uses the normals of the textures to simulate a 3d texture to the models based on the light hitting the model. This helps in adding more details while not having to have more complex models. 

![bumpmapinfo](https://github.com/thumun/Horror-Game/blob/main/images/bumpmapinfo.png)

**Scene:**

The scene itself was constructed using the matrix stack. Almost every model has a rotation in the x-axis of -pi/2 in order to ensure they are facing the proper direction. Each model is scaled and centered in the scene and are translated based on where they fit best. 

I found out later that some of the models were positioned oddly in blender which led to the y-direction of the rotation matrix to be skewed but since their locations/placements seemed satisfactory to me I did not change their PLY files-except for the monster model as that required more a precise rotation to ensure it faced the camera/player. 

![room1](https://github.com/thumun/Horror-Game/blob/main/images/room1.png)
![room2](https://github.com/thumun/Horror-Game/blob/main/images/room2.png)

**Custom models:**

I found that the models available were a bit lacking (mainly that the room itself was too small and it did not have two walls). As such, I added a wall and another room. I resued some of the textures from the original room and painted some new textures in the room. 

![monster](https://github.com/thumun/Horror-Game/blob/main/images/monsterscreenshot.png)

**Music:**

I used the FMOD dependency, to add music to the game. The background music is looped and is called "It's In The Fog" by Darren Curtis. 
When the spotlight starts flashing, one of the monster tracks begins 'mixkit terror transition' and then when the monster appears the background music changed to a loop of 'beat-n-bass'.

## Data Structures

Incorporated a map in order to organize the meshData (PLYmesh files, textures, normal textures). 

C++ STL Vectors were used to organize shaders being used in the project. 

Vectors were also used in calculations (namely for matrix transformations and camera movement). 

Used sound channels and library elements to implement music and sound effects. 

## Dependencies Used

FMOD (Information here: https://www.fmod.com/ ) 

## Credits 

Monster model:

https://sketchfab.com/3d-models/brute-from-amnesia-the-dark-descent-2f8dd46255f44af7bf80c92977b43e95

Scene assets: 

https://foxdevart.itch.io/victorian-study-3d-asset-pack?download

Stock Photo for Wall Portrait: 

https://www.dreamstime.com/photos-images/general-oil-painting.html 

Guides for OpenGL:

https://learnopengl.com/

Background Music: 

https://www.youtube.com/watch?v=5oF5lS0sajs&list=PLfP6i5T0-DkI74kb_BWxYNZRjIvF1D4Lp&index=1
"Darren Curtis - It's In The Fog" is under a Creative Commons license (CC BY 3.0).
 / darrencurtismusic   
Music promoted by BreakingCopyright: https://bit.ly/b-in-the-fog 

Sound effect: 

https://mixkit.co/free-sound-effects/horror/
"Terror transition"

Second Sound effect: 

beat-n-bass from https://github.com/BrynMawr-CS313-S23/simple-sound-fmod/tree/main/sounds

