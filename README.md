# ✨ Cozy Cottage ✨

A game where you can explore a wonderful little cottage tucked away in the woods. Rather odd place for a cottage to be you may suppose but none the less you venture in and, alas, the door seems to be jammed. Perhaps there is something in this quaint little place that may pry open that door? It is rather intriguing how small it is, peculiar even. And you can't help but feel as if something is watching you... 

![ccMainImg](https://github.com/thumun/Horror-Game/blob/main/images/ccmainimg.png)


## How to build

*Windows*

Open git bash to the directory containing this repository.

```
project-template $ mkdir build
project-template $ cd build
project-template/build $ cmake ..
project-template/build $ start project-template.sln
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

The idea behind this game is that the player is trapped in a rather small house (two rooms). They are free to explore the space and are able to see via a "flashlight" (spotlight). However, little do they know that behind them is a rather ominous prescence watching their every move. 
For future mechanics, I was thinking of making it an escape room style game where the player could potentially escape the scene by finding clues hidden throughout the space. This would involve object interactibility. 

## Features 

Flythrough Camera: 

The player navigates the space using the flythrough camera. In order to simulate walking, the player cannot move in the y-axis however they are free to look anywhere they like. 

Spotlight: 

The spotlight shines wherever the player looks. When they are farther away, more of the space is lit up but when they're close to objects, there is a smaller radius. Due to the combination of the bumpmap, the spotlight has crisp lines and makes it rather difficult for the player to see outside the circle of visibility.  

This light uses the phong shader as a base but has a few added elements. Unlike directional light, spotlight has... 

Bump Map: 

The bump map uses the normals of the textures to simulate a 3d texture to the models based on the light hitting the model. 
In this case, the bump map added a bit of a color shift along with the texture which made the scene seem more cottage core esque. 

Scene: 

The scene itself was constructed using the matrix stack. Almost every model has a rotation in the x-axis of -pi/2 in order to ensure they are facing the proper direction. Each model is scaled and centered in the scene and are translated based on where they fit best. 

I found out later that some of the models were positioned oddly in blender which led to the y-direction of the rotation matrix to be skewed but since their locations/placements seemed satisfactory to me I did not change their PLY files-except for the monster model as that required more a precise rotation to ensure it faced the camera/player. 

Custom models: 

I found that the models available were a bit lacking (mainly that the room itself was too small and it did not have two walls). As such, I added a wall and another room. I resued some of the textures from the original room and painted some new textures in the room. 

## Credits 

https://sketchfab.com/3d-models/brute-from-amnesia-the-dark-descent-2f8dd46255f44af7bf80c92977b43e95
used this model as the 'monster'

https://foxdevart.itch.io/victorian-study-3d-asset-pack?download
used the models/assets in the scene 

https://www.dreamstime.com/photos-images/general-oil-painting.html 
for the stock photo image used in the portrait on the wall

https://learnopengl.com/
used for finding constants of materials, lights, and for resources

Music [potentially]
https://www.youtube.com/watch?v=5oF5lS0sajs&list=PLfP6i5T0-DkI74kb_BWxYNZRjIvF1D4Lp&index=1
"Darren Curtis - It's In The Fog" is under a Creative Commons license (CC BY 3.0).
 / darrencurtismusic   
Music promoted by BreakingCopyright: https://bit.ly/b-in-the-fog 
