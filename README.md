# ✨ Cozy Cottage ✨

A game where you can explore a wonderful little cottage tucked away in the woods. Rather odd place for a cottage to be you may suppose but none the less you venture in and, alas, the door seems to be jammed. Perhaps there is something in this quaint little place that may pry open that door? It is rather intriguing how small it is, peculiar even. And you can't help but feel as if something is watching you... 

![ccMainImg](https://github.com/thumun/Horror-Game/blob/main/images/ccmainimg.png)


## How to build

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

## Features 

Flythrough Camera: 

The player navigates the space using the flythrough camera. In order to simulate walking, the player cannot move in the y-axis however they are free to look anywhere they like. 

Spotlight: 

The spotlight shines wherever the player looks while the surrounding area has a darkened hue. The spotlight itself has a slightly green tone due to the color of the light in combination with the bump map. By increasing the exponent, the light is blurred around the edges. In addition, the angle of the light is 60 degrees which makes the spotlight light up most of the screen. 

After a certain amount of time passes (20 seconds), the spotlight begins to flash on/off until the endscreen happens. 

Bump Map: 

The bump map uses the normals of the textures to simulate a 3d texture to the models based on the light hitting the model. This helps in adding more details while not having to have more complex models. 

Scene: 

The scene itself was constructed using the matrix stack. Almost every model has a rotation in the x-axis of -pi/2 in order to ensure they are facing the proper direction. Each model is scaled and centered in the scene and are translated based on where they fit best. 

I found out later that some of the models were positioned oddly in blender which led to the y-direction of the rotation matrix to be skewed but since their locations/placements seemed satisfactory to me I did not change their PLY files-except for the monster model as that required more a precise rotation to ensure it faced the camera/player. 

Custom models: 

I found that the models available were a bit lacking (mainly that the room itself was too small and it did not have two walls). As such, I added a wall and another room. I resued some of the textures from the original room and painted some new textures in the room. 

Music: 

I used the FMOD dependency, to add music to the game. The background music is looped and is called "It's In The Fog" by Darren Curtis. 
When the spotlight starts flashing, one of the monster tracks begins 'mixkit terror transition' and then when the monster appears the background music changed to a loop of 'beat-n-bass'.

## Credits 

https://sketchfab.com/3d-models/brute-from-amnesia-the-dark-descent-2f8dd46255f44af7bf80c92977b43e95
used this model as the 'monster'

https://foxdevart.itch.io/victorian-study-3d-asset-pack?download
used the models/assets in the scene 

https://www.dreamstime.com/photos-images/general-oil-painting.html 
for the stock photo image used in the portrait on the wall

https://learnopengl.com/
used for finding constants of materials, lights, and for resources

Background Music 
https://www.youtube.com/watch?v=5oF5lS0sajs&list=PLfP6i5T0-DkI74kb_BWxYNZRjIvF1D4Lp&index=1
"Darren Curtis - It's In The Fog" is under a Creative Commons license (CC BY 3.0).
 / darrencurtismusic   
Music promoted by BreakingCopyright: https://bit.ly/b-in-the-fog 

https://mixkit.co/free-sound-effects/horror/
"Terror transition"

beat-n-bass from https://github.com/BrynMawr-CS313-S23/simple-sound-fmod/tree/main/sounds

