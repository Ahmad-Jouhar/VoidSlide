# VoidSlide
A 2D side-scrolling game I made using C++ and SFML

## requirements:
- the game requires the user to install the SFML library with the following command:
  **sudo apt-get install libsfml-dev**
- the compiled executable called "VoidSlide" can only be run on linux (running it on windows will require it to be compiled for windows)

you can run the game through the terminal by:
- right-clicking and selecting "Open in Terminal" in the folder
- or, opening a terminal and navigating to the game folder

next type "**./VoidSlide**" or double-click the program and enjoy!



if the game doesn't work, you can recompile it using
the following command while in the game folder:

**g++ voidSlide.cpp -o VoidSlide -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio**


g++ can be installed using:
**sudo apt install g++**
