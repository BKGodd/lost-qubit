# The Lost Qubit
This is a RPG game developed on `ARM` for `MBED`, using `C` and `C++`.

## Implementation
The main character is controlled via an accelerometer (tilting the board), along with action buttons and a small LCD display.
A custom-built hash-map for fast graphical rendering was developed and demonstrated for this project.
With the pre-compiled binary `RPG.LPC1768.bin`, you can try it out yourself (assuming you have an `MBED` microcontroller 
with the appropriate hardware).


## Gameplay
The objective of the game is to help another NPC retrieve their lost qubit. To do so, you must traverse through the map
and gain access to a dungeon of sorts. After the qubit is found, return it to the character for a reward and completion of
the game. Along the way you may collect bonus items in your inventory, such as a strontium atom. 
"God-mode" can also be activated with one of the action buttons, allowing you to float over any obstructions on the map.
