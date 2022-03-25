[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-f059dc9a6f8d3a56e377f745f24479a46679e63a5d9fe6f495e02850cd0d8118.svg)](https://classroom.github.com/online_ide?assignment_repo_id=6004567&assignment_repo_type=AssignmentRepo)
# Computer Graphics 203.3710
## Students Team data

Name: Tal Zohar

Student Number: 


Name: 

Student Number: 

The first problem I encountered is that there is no such object. you cant tile a sphere with hexagons, that is due to euler charitaristic. That means that we will have to have a couple of pentagons as well. The tiling will be by projecting an icosahedron to a spherical plance. Where each face will represent a chunck which will have a 3d hexaprism coordinate system. 

![](MDAssets/64559-goldberg-polyhedron-10-0.png)

Then I thought to reasearch hexagon coordinate systems. I will most likely use the axial representation even though that if I want to fill up an entire 2d array, the chunck will be need to be comprimised of two face of the icosahedron, leaving us with 10 chucks.


**Total Time** ~20 hours (that led nowhere)
### 25/3 
Started the implementation this week. Most of the week went away to bashing my head with CMAKE, the general program flow is similar to the Computer Graphics Course assignments. Then I started implementing the openGL framework. It took so much more time than I'd like to admit until there was a visible triangle on screen. Then I built the basic program chunck layout. Each chunck is a face of the rendered icosahedron. 

![](MDAssets/basicShape.PNG)

Finally I added basic ImGui functionality. 

**Total Time** ~8 hours

