# Destino
A Multiplayer First Person shooter built around a from scratch game engine.  
Inspired by early first person shooters such as Doom, Quake, and Unreal Tournament.  

![image](https://github.com/user-attachments/assets/8f974323-aa59-4703-a3a3-1d3604051abd)

## Tech
The 3D rendering is done by a custom built software renderer.  
Possible collisions are scanned with an interval kd tree to limit comparisons.  
All textures are designed within Gimp.  
All models and texture data is built in Blender and exported to the obj and mtl formats.  
OBJ and material files are loaded with a custom library into the scene.  
The server tracks entities in the system and provides updates to the user on their velocities and positions over a UDP connection.
