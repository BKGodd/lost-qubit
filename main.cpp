// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
void movement(int action);
void lose(int game);
int state(int game);
int main();
bool convo = false;
bool scroll = false;
bool spiked = false;
int ptype;
int count=0;

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;        // Current locations
    int px, py;     // Previous locations
    int has_key;    // has the key
    int has_item;   // has the qubit
    int has_talked; // has talked to the NPC
    int god;        // in God-mode
    int has_stront; // has the atom
} Player;

struct {
    int x,y;
} npc;
/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define GOD_MODE 10
int get_action(GameInputs inputs)
{
    // using measured accelerometer values
    if (inputs.ax<=-0.2) return GO_UP;
    if (inputs.ax>=0.32) return GO_DOWN;
    if (inputs.ay<=-0.2) return GO_LEFT;
    if (inputs.ay>=0.32) return GO_RIGHT;
    if (!inputs.b1) return ACTION_BUTTON;
    if (!inputs.b2) return GOD_MODE;
    // if no action was caught, do nothing
    return NO_ACTION;
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define BEGIN     1
#define MIDDLE    2
#define ENDING    3
#define GAME_OVER 4
#define GOD_MODE  10
#define FULL_DRAW 6
int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    // show status bar info
    uLCD.color(WHITE);
    uLCD.locate(0,0);
    uLCD.printf("X:%i  Y:%d   ",Player.x,Player.y);
    uLCD.locate(0,15);
    if (Player.has_item && !Player.has_stront) uLCD.printf("I: qubit   ");
    if (Player.has_item && Player.has_stront) uLCD.printf("I: qubit, s-atom   ");
    if (Player.has_key && !Player.has_stront) uLCD.printf("I: key    ");
    if (Player.has_key && Player.has_stront) uLCD.printf("I: key, s-atom   ");
    if (!Player.has_item && !Player.has_key && !Player.has_stront) uLCD.printf("I: empty   ");
    if (Player.has_stront && !Player.has_item && !Player.has_key) uLCD.printf("I: s-atom   ");
    
    // get correct movement behavior 
    // toggle god-mode button
    if (action==GOD_MODE) {
        switch (Player.god) {
            case 0:
                Player.god=1;
                // slow it down!
                // holding for a moment too long will flip states too fast
                wait_ms(200);
            break;
            case 1:
                Player.god=0;
                // slow it down!
                wait_ms(200);
            break;
        }
    }
    movement(action);
    // redraw character when out of tree
    if (!(get_here(Player.x,Player.y)) && get_here(Player.px,Player.py)->type==TREE) {
        return FULL_DRAW;
    }
    // execute the game over screen if spiked
    if (get_here(Player.x,Player.y)->type==SPIKE && !spiked && Player.god==0) {
        spiked = true;
        // send in, so speech(10) can execute
        return state(BEGIN);
    }
    if (count==0 && (Player.x<7 && Player.y<11)) return FULL_DRAW;
    if (action==ACTION_BUTTON){
        if (Player.has_talked==0) return state(BEGIN);
        if (Player.has_talked==1) return state(MIDDLE);
        if (Player.has_talked==2) return state(ENDING);
    }
    return NO_RESULT;
}

int state(int game)
{
    // not the most efficient initialization here...
    int S = get_south(Player.x,Player.y)->type;
    int N = get_north(Player.x,Player.y)->type;
    int E = get_east(Player.x,Player.y)->type;
    int W = get_west(Player.x,Player.y)->type;
    int H = get_here(Player.x,Player.y)->type;
    int P = get_here(Player.px,Player.py)->type;
    if (!convo) {
        // make sure player is in correct position
        if (N==NPC || S==NPC || E==NPC || W==NPC) {
            convo = true;
            ptype = NPC;
        } else if (N==DOOR) {
            convo = true;
            ptype = DOOR;
        } else if (N==QUBIT || S==QUBIT || E==QUBIT || W==QUBIT) {
            convo = true;
            ptype = QUBIT;
        } else if (S==UNLOCK) {
            convo = true;
            ptype = UNLOCK;
        } else if (N==GOLD || E==GOLD || W==GOLD) {
            convo = true;
            ptype = GOLD;
        } else if (H==SPIKE || P==SPIKE) {
        // no matter what, if you go over spike, you LOSE
            convo = true;
            ptype = SPIKE;
        } else if (N==STRONT || S==STRONT || W==STRONT || E==STRONT) {
            convo = true;
            ptype = STRONT;
        }
        // display speech bubble, depending on state
        if (convo && ptype==NPC && game==BEGIN) {
            // first quest starts
            speech(0);
            Player.has_talked = 1;
        }
        if (convo && ptype==NPC && game==MIDDLE && Player.has_item==0) speech(1);
        if (convo && ptype==NPC && game==MIDDLE && Player.has_item==1) {
            // we now have key for reward
            speech(7);
            Player.has_talked = 2;
            // qubit removed from inventory
            Player.has_item = 0;
            // given key
            Player.has_key = 1;
        }
        if (convo && ptype==NPC && game==ENDING) speech(8);
        if (convo && ptype==DOOR && (game==BEGIN || game==MIDDLE)) speech(2);
        if (convo && ptype==DOOR && game==ENDING) speech(5);
        if (convo && ptype==UNLOCK) speech(3);
        if (convo && ptype==QUBIT && game==MIDDLE) speech(4);
        if (convo && ptype==QUBIT && game==BEGIN) speech(6);
        // game over screens 
        if (convo && ptype==GOLD) speech(9);   // win
        if (convo && ptype==SPIKE) speech(10); // lose
        if (convo && ptype==STRONT) {
            // put strontium in inventory
            speech(12);
            Player.has_stront = 1;
            // remove from map
            block_erase(17,23);
        }
        return NO_RESULT;
    } else {
        // open the door!
        if (ptype==DOOR && game==ENDING) {
            block_erase(33,9);
            block_erase(34,9);
            block_erase(35,9);
            block_erase(36,9);
            block_erase(37,9);
            block_erase(38,9);
            block_erase(39,9);
        }
        if (ptype==QUBIT && game==MIDDLE) {
            // qubit now in inventory
            block_erase(8,52);
            Player.has_item = 1;
        }
        if (ptype==UNLOCK) {
            // unlock the door
            block_erase(4,41);
            block_erase(5,41);
            block_erase(6,41);
            block_erase(7,41);
            block_erase(8,41);
            block_erase(9,41);
            block_erase(10,41);
            block_erase(11,41);
        }
        if (ptype==GOLD || ptype==SPIKE) {
            return NO_RESULT;
        }
        // exit conversation
        convo = false;
        return FULL_DRAW;
    }
    
}

void movement(int action)
{
    int S = get_south(Player.x,Player.y)->type;
    int N = get_north(Player.x,Player.y)->type;
    int E = get_east(Player.x,Player.y)->type;
    int W = get_west(Player.x,Player.y)->type;
    int H = get_here(Player.x,Player.y)->type;
    int P = get_here(Player.px,Player.py)->type;
    // NO MOVEMENT if we stepped on a spike
    if (Player.god) {
        // if in god mode, walk through everything
        switch(action)
        {
            // only move if NOT in a conversation
            case GO_UP:
                if (!convo) Player.y--;
            break;
            case GO_LEFT:
                if (!convo) Player.x--;
            break;
            case GO_DOWN:
                if (!convo) Player.y++;
            break;
            case GO_RIGHT:
                if (!convo) Player.x++;
            break;
            default: break;
        }
        if (!convo) draw_player(58,59,Player.has_key);
    } else {
        switch(action)
        {
            // only move if NOT in a conversation or....
            // if the block is either empty or a spike (we want you to die) or a tree
            case GO_UP:
                if ((!get_north(Player.x,Player.y) || N==SPIKE || N==TREE) && !convo) Player.y--;
            break;
            case GO_LEFT:
                if ((!get_west(Player.x,Player.y) || W==SPIKE || W==TREE) && !convo) Player.x--;
            break;
            case GO_DOWN:
                if ((!get_south(Player.x,Player.y) || S==SPIKE || S==TREE) && !convo) Player.y++;
            break;
            case GO_RIGHT:
                if ((!get_east(Player.x,Player.y) || E==SPIKE || E==TREE) && !convo) Player.x++;
            break;
            default: break;
        }
    }
}

void god_mode()
{
    
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) // Only draw the player on init
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }
            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars    
    draw_upper_status();
    draw_lower_status();
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    // place down NPC
    add_NPC(1,3);
    // place down the qubit
    add_qubit(8,52);
    // add strontium atom
    add_stront(17,23);
    
    // "Random" plants
    Map* map = set_active_map(0);
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_plant(i % map_width(), i / map_width());
    }
    pc.printf("plants\r\n");
    
    // border walls 
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");
    
    // make a house to enter
    add_wall(30,0,VERTICAL,10);
    add_wall(42,0,VERTICAL,10);
    add_wall(31,9,HORIZONTAL,2);
    add_wall(40,9,HORIZONTAL,2);
    //make walls for quest
    add_unlock(4,41);
    add_unlock(5,41);
    add_unlock(6,41);
    add_unlock(7,41);
    add_unlock(8,41);
    add_unlock(9,41);
    add_unlock(10,41);
    add_unlock(11,41);
    add_wall(15,41,VERTICAL,12);
    add_wall(12,41,HORIZONTAL,3);
    add_wall(1,41,HORIZONTAL,3);
    // erase plants inside house
    block_erase(9,50);
    block_erase(12,42);
    block_erase(36,4);
    block_erase(6,45);
    // make wooden door
    add_door(33,9);
    add_door(34,9);
    add_door(35,9);
    add_door(36,9);
    add_door(37,9);
    add_door(38,9);
    add_door(39,9);
    // add spikes
    add_spike(2,46);
    add_spike(3,46);
    add_spike(4,46);
    add_spike(5,46);
    add_spike(6,46);
    add_spike(7,46);
    add_spike(8,46);
    add_spike(9,46);
    add_spike(10,46);
    add_spike(11,46);
    add_spike(12,46);
    add_spike(13,46);
    // add gold
    add_gold(36,1);
    // add giant tree
    add_trunk(20,20);
    add_tree1(21,19);
    add_tree2(20,19);
    add_tree3(19,19);
    add_tree4(21,18);
    add_tree5(20,18);
    add_tree6(19,18);
    //print_map();
}

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    
    speech(11);
    // create startup page
    while (1){
           Timer t; t.start();
           GameInputs in = read_inputs();
           // if action button was pressed, continue to game
           if (!in.b1) break;
           t.stop();
           int dt = t.read_ms();
           if (dt < 100) wait_ms(100 - dt);
    }
    
    // Initialize the maps
    maps_init();
    init_main_map();
    
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;
    // Initial drawing
    // weird things happen if we dont make it wait
    wait_ms(1000);
    draw_game(true);
    // Main game loop
    while(1)
    {
        // Timer to measure game update speed
        Timer t; t.start();
        // Actually do the game update:
        // 1. Read inputs    
        GameInputs in = read_inputs();
        // 2. Determine action (get_action)  
        int a = get_action(in);
        // 3. Update game (update_game)
        int b = update_game(a);
        // 4. Draw frame (draw_game)
        if (!convo) draw_game(b);
        // walking NPC
        count = count+1;
        while (count==20) {
            // NPC, won't move if player is ...
            // north
            if (npc.y-1==Player.y && npc.x==Player.x || Player.x>=7 || Player.y>=11) {
                count = 0;
                break;
            }
            // south
            if (npc.y+1==Player.y && npc.x==Player.x || Player.x>=7 || Player.y>=11) {
                count = 0;
                break;
            }
            // east
            if (npc.x+1==Player.x && npc.y==Player.y || Player.x>=7 || Player.y>=11) {
                count = 0;
                break;
            }
            // west
            if (npc.x-1==Player.x && npc.y==Player.y || Player.x>=7 || Player.y>=11) {
                count = 0;
                break;
            }
            if (get_here(1,3)->type==NPC) { 
                block_erase(1,3);
                add_NPC(1,4);
                npc.x = 1;
                npc.y = 4;
                count = 0;
                break;
            }
            if (get_here(1,4)->type==NPC) { 
                block_erase(1,4);
                add_NPC(1,5);
                npc.x = 1;
                npc.y = 5;
                count = 0;
                break;
            }
            if (get_here(1,5)->type==NPC) { 
                block_erase(1,5);
                add_NPC(1,6);
                npc.x = 1;
                npc.y = 6;
                count = 0;
                break;
            }
            if (get_here(1,6)->type==NPC) { 
                block_erase(1,6);
                add_NPC(2,6);
                npc.x = 2;
                npc.y = 6;
                count = 0;
                break;
            }
            if (get_here(2,6)->type==NPC) { 
                block_erase(2,6);
                add_NPC(2,5);
                npc.x = 2;
                npc.y = 5;
                count = 0;
                break;
            }
            if (get_here(2,5)->type==NPC) { 
                block_erase(2,5);
                add_NPC(2,4);
                npc.x = 2;
                npc.y = 4;
                count = 0;
                break;
            }
            if (get_here(2,4)->type==NPC) { 
                block_erase(2,4);
                add_NPC(2,3);
                npc.x = 2;
                npc.y = 3;
                count = 0;
                break;
            }
            if (get_here(2,3)->type==NPC) { 
                block_erase(2,3);
                add_NPC(1,3);
                npc.x = 1;
                npc.y = 3;
                count = 0;
                break;
            }
        }
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
}
