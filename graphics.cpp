#include "graphics.h"
#include "globals.h"

// some sprites to tickle my fancy
// grass / bush
const char sprite1[] = "0000000000000000000000000G000G000G00G0G000G0G00G00G0GG0GG00G00G0GGG0G00GGGGGG000G0G0GG0GGGGGGG0GG00G0GG00GG0GGGGGGGGGGGGG";
// spikes
const char sprite2[] = "0004004443500350003350040050405300003005544400040505300353433000045043040003430350043405303004330300500454040003500034400";
// stone wall
const char sprite3[] = "5555555555555555555555555555555555553333333355555555555555555522222225555555555555533333555555555553333335555555555555555";
// wooden door
const char sprite4[] = "0000000000000000000000DDDDDDDDDDDDDD33333DDDDDDDDDDD3333DDDDDDD33DDDDDDDDDDDDDDDDDD33333DDDDDDDDDDD0000000000000000000000";
// qubit
const char sprite5[] = "00000P00000000PPPPP00000PPPPPPP00000PPPPP0000000PPP0000OOOOOOOOOOO00000P00000000PPPPP00000PPPPPPP00000PPPPP0000000PPP0000";
// NPC
const char sprite6[] = "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR5RRRRRRR5555555RRRRRRR5RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR";
// gold
const char sprite7[] = "000DDDDD00000DDDDDDD000DDDDDDDDD0DDDDYYYDDDDDDYYYYYYYDDDDDDYYYDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD00000000000";
// strontium atom
const char sprite8[] = "0000000000000000000000000333330000033333330003355333330333333333330333335533000355333300000333330000000000000000000000000";
//tree
const char   trunk[] = "000DDDDD000000DDDDD000000DDDDD000000DDDDD000000DDDDD000000DDDDD000000DDDDD000000DDDDD000000DDDDD00000DDDDDDD000DDDDDDDDD0";
const char   tree1[] = "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG333GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG3333GGGGGGGGGGG0GGGGGGGGG00GGGGGGGG000";
const char   tree2[] = "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG333GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG333GGGGGGGGGGGGGG";
const char   tree3[] = "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG33GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG0GGGGGGGGGG00G33333GGG000GGGGGGGG";
const char   tree4[] = "GGGGGGGG000GGGGGGGGG00GGGGGGGGGG0GGGGG33GGGGGGGGGGGGGGGGGGGGGGGGGGGG3333GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG33333GGGGGGGGGGGGGG";
const char   tree5[] = "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG333GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG333GGGGGGGGGGGGGG";
const char   tree6[] = "000GGGGGGGG00GGGGGGGGG0GGGGGGGGGGGGGGG33GGGGGGGGGGGGGGGGGGGGGGGGGGGGG333GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG333GGGGGGGGGGGGGG";

void array(char* a, const char* color1)
{
    for (int i=0;i<11*11;i++)
    {
        a[i] = color1[0];
    }
}

void draw_trunk(int u, int v)
{
    const char *p_t = trunk;
    draw_img(u,v,p_t);
}
void draw_tree1(int u, int v)
{
    const char *p_t1 = tree1;
    draw_img(u,v,p_t1);
}
void draw_tree2(int u, int v)
{
    const char *p_t2 = tree2;
    draw_img(u,v,p_t2);
}
void draw_tree3(int u, int v)
{
    const char *p_t3 = tree3;
    draw_img(u,v,p_t3);
}
void draw_tree4(int u, int v)
{
    const char *p_t4 = tree4;
    draw_img(u,v,p_t4);
}
void draw_tree5(int u, int v)
{
    const char *p_t5 = tree5;
    draw_img(u,v,p_t5);
}
void draw_tree6(int u, int v)
{
    const char *p_t6 = tree6;
    draw_img(u,v,p_t6);
}

void draw_player(int u, int v, int key)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, RED);
    pc.printf("u=%d   v=%d\r\n",u,v);
}

void draw_npc(int u, int v)
{
    //uLCD.filled_rectangle(u, v, u+10, v+10, RED);  default
    const char *ptr6 = sprite6;
    draw_img(u,v,ptr6);
}

void draw_gold(int u, int v)
{
    const char *ptr7 = sprite7;
    draw_img(u,v,ptr7);
}

void draw_spike(int u, int v)
{
    const char *ptr2 = sprite2;
    draw_img(u,v,ptr2);
}

void draw_qubit(int u, int v)
{
    const char *ptr5 = sprite5;
    draw_img(u,v,ptr5);
}

void draw_stront(int u, int v)
{
    const char *ptr8 = sprite8;
    draw_img(u,v,ptr8);
}

#define YELLOW 0xFFFF00
#define BROWN  0xD2691E
#define DIRT   BROWN
#define PURP   0x8A2BE2
#define ORG    0xFFA500
void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = YELLOW;
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = DIRT;
        else if (img[i] == '5') colors[i] = LGREY;
        else if (img[i] == '3') colors[i] = DGREY;
        else if (img[i] == 'P') colors[i] = PURP;
        else if (img[i] == 'O') colors[i] = ORG;
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(300); // Recovery time! 
}

// Helper functions for rendering
void draw_nothing(int u, int v)
{
    // Fill a tile with blackness
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_wall(int u, int v)
{
    //uLCD.filled_rectangle(u, v, u+10, v+10, BROWN); default
    const char *ptr3 = sprite3;
    draw_img(u,v,ptr3);
}

void draw_door(int u, int v)
{
    const char *ptr4 = sprite4;
    draw_img(u,v,ptr4);
}

void draw_unlock(int u, int v)
{
    const char *ptr4 = sprite4;
    draw_img(u,v,ptr4);
}

void draw_plant(int u, int v)
{
    //uLCD.filled_rectangle(u, v, u+10, v+10, GREEN); default
    const char *ptr1 = sprite1;
    draw_img(u,v,ptr1);
    
}

void draw_upper_status()
{
    // Draw bottom border of status bar
    uLCD.line(0, 9, 127, 9, GREEN);
}

void draw_lower_status()
{
    // Draw top border of status bar
    uLCD.line(0, 118, 127, 118, GREEN);
}

void draw_border()
{
    uLCD.filled_rectangle(0,     9, 127,  14, WHITE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, WHITE); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, WHITE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE); // Right
}


