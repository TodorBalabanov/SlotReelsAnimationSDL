#!/bin/bash
# g++ main.cpp -o SlotReelsAnimationSDL.exe -I /Library/Frameworks/SDL2.framework/Headers -I /Library/Frameworks/SDL2_image.framework/Headers -F /Library/Frameworks/ -framework SDL2 -framework SDL2_image
g++ main.cpp -o SlotReelsAnimationSDL.exe -lSDL2 -lSDL2_image #-lSDL2_ttf
./SlotReelsAnimationSDL.exe
