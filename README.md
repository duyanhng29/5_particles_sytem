## OVERVIEW: 

This program simulates a general purpose particle system. The program must have some sort of graphical user interface that allows for adjusting several parameters interactively and displays the results on the fly (suggestion: take a look at NanoGui or Ant TweakBar as possible GUIs that integrate with GL). The particles should support physics for gravity, aerodynamic drag, and collisions with a ground plane at the least. New particles should be continually created at an adjustable rate, and the particles should also have a life span that is assigned when the particle is created and then counts down to 0, causing the particle to disappear. Particles can render as small spheres or as sprites. The following properties should be adjustable:

- Particle creation rate (particles per second)
- Initial position & variance (X, Y, Z)
- Initial velocity & variance (X, Y, Z)
- Initial life span & variance
- Gravity
- Air density
- Drag coefficient
- Particle radius (for both rendering & aerodynamic drag)
- Collision elasticity
- Collision friction

## PROGRAM OBJECTIVES:
- Particle system with basic physics (gravity, air friction, ground collisions)
- Dynamic creation & destruction (based on random life span) of particles
- Graphical user interface
- All listed properties adjustable