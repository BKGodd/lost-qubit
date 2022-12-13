#include "speech.h"

#include "globals.h"
#include "hardware.h"

/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
//static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define NPC1    0
#define NPC2    1
//static void draw_speech_line(const char* line, int which);

/**
 * Delay until it is time to scroll.
 */
//static void speech_bubble_wait();

void draw_speech_bubble()
{
    // top line
    uLCD.line(3,93,124,93,BLUE);
    uLCD.line(3,94,124,94,BLUE);
    uLCD.line(3,95,124,95,BLACK);
    // bottom line
    uLCD.line(5,112,122,112,BLACK);
    uLCD.line(3,114,124,114,BLUE);
    uLCD.line(3,113,124,113,BLUE);
    // left line
    uLCD.line(3,95,3,113,BLUE);
    uLCD.line(4,95,4,113,BLUE);
    uLCD.line(5,96,5,113,BLACK);
    uLCD.line(6,96,6,113,BLACK);
    // right line
    uLCD.line(122,95,122,113,BLUE);
    uLCD.line(123,95,123,113,BLUE);
}
/*
void erase_speech_bubble()
{
}
void draw_speech_line(const char* line, int which)
{
}
void speech_bubble_wait()
{
}
*/
void speech(int type)
{
    switch (type){
        case 0:
            draw_speech_bubble();
            uLCD.locate(1,12);
            uLCD.printf("Help me find my ");
            uLCD.locate(1,13);
            uLCD.printf("qubit!          ");
        break;
        case 1:
            draw_speech_bubble();
            uLCD.locate(1,12);
            uLCD.printf("Aren't you going");
            uLCD.locate(1,13);
            uLCD.printf("to help me?!    ");
        break;
        case 2:
            draw_speech_bubble();
            uLCD.locate(1,12);
            uLCD.printf("You must have a ");
            uLCD.locate(1,13);
            uLCD.printf("key to open ... ");
        break;
        case 3:
            draw_speech_bubble();
            uLCD.locate(1,12);
            uLCD.printf("Success! Watch  ");
            uLCD.locate(1,13);
            uLCD.printf("out for spikes! ");
        break;
        case 4: 
            draw_speech_bubble();
            uLCD.locate(1,12);
            uLCD.printf("Found the qubit!");
            uLCD.locate(1,13);
            uLCD.printf("                ");
        break;
        case 5:
            draw_speech_bubble();
            uLCD.locate(1,12);
            uLCD.printf("Success! What   ");
            uLCD.locate(1,13);
            uLCD.printf("hides inside ...");
        break;
        case 6:
            draw_speech_bubble();
            uLCD.locate(1,12);
            uLCD.printf("What is this    ");
            uLCD.locate(1,13);
            uLCD.printf("thing? ...      ");
        break;
        case 7:
            draw_speech_bubble();
            uLCD.locate(1,12);
            uLCD.printf("YAY! Take this  ");
            uLCD.locate(1,13);
            uLCD.printf("key as a gift   ");
        break;
        case 8:
            draw_speech_bubble();
            uLCD.locate(1,12);
            uLCD.printf("uummm, I already");
            uLCD.locate(1,13);
            uLCD.printf("gave you the key");
        break;
        case 9:
            uLCD.locate(0,0);
            uLCD.printf("                    ");
            uLCD.locate(0,1);
            uLCD.printf("|                 | ");
            uLCD.locate(0,2);
            uLCD.printf("|-----------------| ");
            uLCD.locate(0,3);
            uLCD.printf("|--- GAME OVER ---| ");
            uLCD.locate(0,4);
            uLCD.printf("|-----------------| ");
            uLCD.locate(0,5);
            uLCD.printf("|                 | ");
            uLCD.locate(0,6);
            uLCD.printf("|     You won     | ");
            uLCD.locate(0,7);
            uLCD.printf("|  all the gold!  | ");
            uLCD.locate(0,8);
            uLCD.printf("|                 | ");
            uLCD.locate(0,9);
            uLCD.printf("|                 | ");
            uLCD.locate(0,10);
            uLCD.printf("|  Press restart  | ");
            uLCD.locate(0,11);
            uLCD.printf("|    button on    | ");
            uLCD.locate(0,12);
            uLCD.printf("|      MBED       | ");
            uLCD.locate(0,13);
            uLCD.printf("|                 | ");
            uLCD.locate(0,14);
            uLCD.printf("|                 | ");
            uLCD.locate(0,15);
            uLCD.printf("                    ");
        break;
        case 10:
            uLCD.locate(0,0);
            uLCD.printf("                    ");
            uLCD.locate(0,1);
            uLCD.printf("|                 | ");
            uLCD.locate(0,2);
            uLCD.printf("|-----------------| ");
            uLCD.locate(0,3);
            uLCD.printf("|--- GAME OVER ---| ");
            uLCD.locate(0,4);
            uLCD.printf("|-----------------| ");
            uLCD.locate(0,5);
            uLCD.printf("|                 | ");
            uLCD.locate(0,6);
            uLCD.printf("| Death by spikes | ");
            uLCD.locate(0,7);
            uLCD.printf("|                 | ");
            uLCD.locate(0,8);
            uLCD.printf("|                 | ");
            uLCD.locate(0,9);
            uLCD.printf("|                 | ");
            uLCD.locate(0,10);
            uLCD.printf("|  Press restart  | ");
            uLCD.locate(0,11);
            uLCD.printf("|    button on    | ");
            uLCD.locate(0,12);
            uLCD.printf("|      MBED       | ");
            uLCD.locate(0,13);
            uLCD.printf("|                 | ");
            uLCD.locate(0,14);
            uLCD.printf("|                 | ");
            uLCD.locate(0,15);
            uLCD.printf("                    ");
        break;
        case 11:
            uLCD.color(WHITE);
            uLCD.locate(0,0);
            uLCD.printf("                   ");
            uLCD.locate(0,1);
            uLCD.printf("                   ");
            uLCD.locate(0,2);
            uLCD.printf("------------------ ");
            uLCD.locate(0,3);
            uLCD.printf("  THE LOST QUBIT   ");
            uLCD.locate(0,4);
            uLCD.printf("------------------ ");
            uLCD.locate(0,5);
            uLCD.printf("                   ");
            uLCD.locate(0,6);
            uLCD.printf("                   ");
            uLCD.locate(0,7);
            uLCD.printf("                   ");
            uLCD.locate(0,8);
            uLCD.printf("                   ");
            uLCD.locate(0,9);
            uLCD.printf("                   ");
            uLCD.locate(0,10);
            uLCD.printf("                   ");
            uLCD.locate(0,11);
            uLCD.printf("                   ");
            uLCD.locate(0,12);
            uLCD.printf("    Press action   ");
            uLCD.locate(0,13);
            uLCD.printf("  button to begin! ");
            uLCD.locate(0,14);
            uLCD.printf("                   ");
            uLCD.locate(0,15);
            uLCD.printf("                   ");
            uLCD.filled_circle(65,50,10,0x8A2BE2);
            uLCD.line(51,60,81,60,0xFFA500);
            // right
            uLCD.line(70,59,79,59,0xFFA500);
            uLCD.line(71,58,77,58,0xFFA500);
            // left
            uLCD.line(53,59,62,59,0xFFA500);
            uLCD.line(55,58,61,58,0xFFA500);
            uLCD.line(55,40,55,55,BLACK);
            uLCD.line(75,40,75,55,BLACK);
            uLCD.filled_circle(65,71,10,0x8A2BE2);
            uLCD.line(55,65,55,85,BLACK);
            uLCD.line(75,65,75,85,BLACK);
            //bottom
            uLCD.line(53,61,79,61,0xFFA500);
            uLCD.line(55,62,77,62,0xFFA500);
            break;
            case 12:
                draw_speech_bubble();
                uLCD.locate(1,12);
                uLCD.printf("Hey, look! A     ");
                uLCD.locate(1,13);
                uLCD.printf("Strontium atom   ");
            break;
        default: break;
    }
}

void long_speech(const char* lines[], int n)
{
}
