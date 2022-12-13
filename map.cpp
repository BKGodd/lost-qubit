#include "map.h"
#include "globals.h"
#include "graphics.h"
#include "hash_table.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map;
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    // using a Cantor pairing function
    return (((X+Y)*(X+Y+1))/2 + Y);
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    // numb buckets = 54
    // 54x54 map
    return (key % 54);
}

void maps_init()
{
    // init the hash function pointer
    HashFunction hf;
    hf = map_hash;
    // Initialize hash table, number of buckets = 54
    map.items = createHashTable(hf, 54);
    // set width and height
    map.w = 54;
    map.h = 54;
}

Map* get_active_map()
{
    // There's only one map
    return &map;
}

Map* set_active_map(int m)
{
    active_map = m;
    return &map;
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P', 'N', 'D', 'S', 'Q', 'U', 'G', 'T', 'E', 'A'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) {
                pc.printf("%c", lookup[item->type]);
            } else {
                pc.printf(" ");
            }
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    return map.w;
}

int map_height()
{
    return map.h;
}

int map_area()
{
    return (map.w*map.h);
}

MapItem* get_north(int x, int y)
{
    int a = x;
    int b = y-1;
    unsigned key = XY_KEY(a,b);
    return ((MapItem*)getItem(map.items, key));
}

MapItem* get_south(int x, int y)
{
    int a = x;
    int b = y+1;
    unsigned key = XY_KEY(a,b);
    return ((MapItem*)getItem(map.items, key));
}

MapItem* get_east(int x, int y)
{
    int a = x+1;
    int b = y;
    unsigned key = XY_KEY(a,b);
    return ((MapItem*)getItem(map.items, key));
}

MapItem* get_west(int x, int y)
{
    int a = x-1;
    int b = y;
    unsigned key = XY_KEY(a,b);
    return ((MapItem*)getItem(map.items, key));
}

MapItem* get_here(int x, int y)
{
    unsigned key = XY_KEY(x,y);
    return ((MapItem*)getItem(get_active_map()->items, key));
}

void block_erase(int x, int y)
{
    deleteItem(get_active_map()->items,XY_KEY(x, y));
}

void map_erase(int x, int y)
{
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        //if (val) pc.printf("x=%d  y=%d",x,y+i);
        //if (val) free(val); // If something is already there, free it
        //if (x==49 && y+i==12) pc.printf("%d,",w1->type);
    }
}
void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_NPC(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = NPC;
    w1->draw = draw_npc;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
}

void add_door(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = DOOR;
    w1->draw = draw_door;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
}

void add_spike(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = SPIKE;
    w1->draw = draw_spike;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
}

void add_qubit(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = QUBIT;
    w1->draw = draw_qubit;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
}

void add_unlock(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = UNLOCK;
    w1->draw = draw_unlock;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
}

void add_gold(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = GOLD;
    w1->draw = draw_gold;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
}

void add_trunk(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = TRUNK;
    w1->draw = draw_trunk;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
}
void add_tree1(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = TREE;
    w1->draw = draw_tree1;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
}
void add_tree2(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = TREE;
    w1->draw = draw_tree2;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
}
void add_tree3(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = TREE;
    w1->draw = draw_tree3;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
}
void add_tree4(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = TREE;
    w1->draw = draw_tree4;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
}
void add_tree5(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = TREE;
    w1->draw = draw_tree5;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
}
void add_tree6(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = TREE;
    w1->draw = draw_tree6;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
}

void add_stront(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    if (w1==NULL) pc.printf("NULL");
    w1->type = STRONT;
    w1->draw = draw_stront;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
}
