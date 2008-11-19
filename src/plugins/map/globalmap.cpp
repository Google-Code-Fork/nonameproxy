#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#include <map>
#include <set>
#include <list>

#include "globalmap.h"
#include "map.h"
#include "enums.h"

#define BUFFER_SIZE 256 

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

#define MIN_COST 100

class MoveHook : public ReadHook
{
        public:
                virtual void func (TibiaMessage* tm, Client* client);
};

GlobalMap::GlobalMap ()
{
        for (uint32_t i = 0; i != MAX_MOVE_HOOK; i ++) {
                _movehooks[i] = 0;
        }
}

/*
void GlobalMap::i_load (uint32_t pluginId, Client* client)
{
        _client = client;
        _pluginId = pluginId;

        _movehooks[0] = _client->addRecvReadHook (_pluginId, GRM_MAP_NORTH_ID,
                                                        new MoveHook);
        _movehooks[1] = _client->addRecvReadHook (_pluginId, GRM_MAP_EAST_ID,
                                                        new MoveHook);
        _movehooks[2] = _client->addRecvReadHook (_pluginId, GRM_MAP_SOUTH_ID,
                                                        new MoveHook);
        _movehooks[3] = _client->addRecvReadHook (_pluginId, GRM_MAP_WEST_ID,
                                                        new MoveHook);
}

void GlobalMap::i_unload ()
{
        for (uint32_t i = 0; i != MAX_MOVE_HOOK; i ++) {
                _client->deleteRecvReadHook (_pluginId, _movehooks[i]);
        }
}
*/

/******************************************************************
 * A* stuff
 ******************************************************************/

#if 0
bool poscomp (const Pos& p1, const Pos& p2)
{
        return POS_2_INDEX (p1) < POS_2_INDEX (p2);
}

void showMap (uint32_t m[][18])
{
        for (uint32_t y = 0; y < 14; y ++) {
                for (uint32_t x = 0; x < 18; x ++) {
                        if (m[y][x] == UINT_MAX) {
                                printf (".");
                        } else {
                                printf ("%d", m[y][x] % 10);
                        }
                }
                printf ("\n");
        }
        printf ("\n");
}

void showOpenList (NodeMap& nodes)
{
        NodeMap::iterator i;
        printf ("==== open list ===\n");
        for (i = nodes.begin (); i != nodes.end (); ++ i) {
                printf ("key = %d\n", (*i).first);
                NodeList& n = (*i).second;
                NodeList::iterator ii;
                for (ii = n.begin (); ii != n.end (); ++ ii) {
                        printf ("%d %d\n", (*ii).x, (*ii).y);
                }
        }
}

/* inserting and deleting a node is rather complex */
inline void insertNode (uint32_t key, Pos& pos, NodeMap& nodes)
{
        NodeMap::iterator i_map = nodes.find (key);
        if (i_map != nodes.end ()) {
                (*i_map).second.push_front (pos);
        } else {
                NodeList n;
                n.push_back (pos);
                nodes.insert (ListPair (key, n));
        }
}

inline bool findNode (uint32_t key, Pos& pos, NodeMap& nodes)
{
        NodeMap::iterator i_map = nodes.find (key);
        if (i_map != nodes.end ()) {
                NodeList& n = (*i_map).second;
                NodeList::iterator i;
                for (i = n.begin (); i != n.end (); ++ i) {
                        if (*i == pos) {
                                n.erase (i);
                                break;
                        }
                }
                if (n.size () == 0) {
                        nodes.erase (i_map);
                }
        }
        return false;
}

inline void deleteNode (uint32_t key, Pos& pos, NodeMap& nodes)
{
        NodeMap::iterator i_map = nodes.find (key);
        if (i_map != nodes.end ()) {
                NodeList& n = (*i_map).second;
                NodeList::iterator i;
                for (i = n.begin (); i != n.end (); ++ i) {
                        if (*i == pos) {
                                n.erase (i);
                                break;
                        }
                }
                if (n.size () == 0) {
                        nodes.erase (i_map);
                }
        }
}

inline Pos popMinNode (NodeMap& nodes)
{
        NodeMap::iterator i_map = nodes.begin ();
        if (i_map != nodes.end ()) {
                NodeList& n = (*i_map).second;
                Pos ret = n.front ();

                if (n.size () == 1) {
                        nodes.erase (i_map);
                } else {
                        n.pop_front ();
                }
                return ret;
        } else {
                printf ("popMinNode error: empty node map\n");
                return Pos (0, 0, 0);
        }
}

void getNextNodes (Pos pos, NodeList& nodes)
{
        if (pos.x > 0) {
                nodes.push_back (Pos (pos.x - 1, pos.y, pos.z));
        }
        if (pos.y > 0) {
                nodes.push_back (Pos (pos.x, pos.y - 1, pos.z));
        }
        if (pos.x < 17) {
                nodes.push_back (Pos (pos.x + 1, pos.y, pos.z));
        }
        if (pos.y < 13) {
                nodes.push_back (Pos (pos.x, pos.y + 1, pos.z));
        }
        if (pos.x > 0 && pos.y > 0) {
                nodes.push_back (Pos (pos.x - 1, pos.y - 1, pos.z));
        }
        if (pos.x < 17 && pos.y > 0) {
                nodes.push_back (Pos (pos.x + 1, pos.y - 1, pos.z));
        }
        if (pos.x < 17 && pos.y < 13) {
                nodes.push_back (Pos (pos.x + 1, pos.y + 1, pos.z));
        }
        if (pos.x > 0 && pos.y < 13) {
                nodes.push_back (Pos (pos.x - 1, pos.y + 1, pos.z));
        }
}

uint32_t h_score (Pos start, Pos end)
{
        return MIN_COST * (abs (start.x - end.x) + abs (start.y - end.y));
}

direction_t calcDirection (Pos from, Pos to)
{
        if (from == to) {
                printf ("from == to\n");
                return DIRECTION_UNUSED;
        }
        if (from.y == to.y) {
                if (from.x < to.x) {
                        return DIRECTION_EAST;
                } else {
                        return DIRECTION_WEST;
                }
        } else if (from.y < to.y) {
                if (from.x == to.x) {
                        return DIRECTION_SOUTH;
                } else if (from.x < to.x) {
                        return DIRECTION_SE;
                } else {
                        return DIRECTION_SW;
                }
        } else {
                if (from.x == to.x) {
                        return DIRECTION_NORTH;
                } else if (from.x < to.x) {
                        return DIRECTION_NE;
                } else {
                        return DIRECTION_NW;
                }
        }
}

void recoverPath (Pos start, Pos end, Path& path, direction_t direction[][18])
{
        while (start != end) {
                direction_t dir = direction[end.y][end.x];
                path.push_front (dir);
                if (dir == DIRECTION_SE 
                 || dir == DIRECTION_EAST 
                 || dir == DIRECTION_NE) 
                {
                        end.x --;
                } else if (dir == DIRECTION_SW 
                        || dir == DIRECTION_WEST
                        || dir == DIRECTION_NW) 
                {
                        end.x ++;
                }
                if (dir == DIRECTION_NE
                 || dir == DIRECTION_NORTH
                 || dir == DIRECTION_NW)
                {
                        end.y ++;
                } else if (dir == DIRECTION_SE
                        || dir == DIRECTION_SOUTH
                        || dir == DIRECTION_SW)
                {
                        end.y --;
                }
        }
}

Path* findPath (Pos start, Pos end, uint32_t m[][18])
{
        if (m[start.y][start.x] == 0) {
                return NULL;
        }
        if (m[end.y][end.x] == 0) {
                return NULL;
        }

        NodeMap open;
        NodeSet closed (poscomp);

        uint32_t key;
        uint32_t g_score[14][18];
        for (uint32_t y = 0; y < 14; y ++) {
                for (uint32_t x = 0; x < 18; x ++) {
                        g_score[y][x] = UINT_MAX;
                }
        }
        direction_t direction[14][18];
        for (uint32_t y = 0; y < 14; y ++) {
                for (uint32_t x = 0; x < 18; x ++) {
                        direction[y][x] = DIRECTION_UNUSED;
                }
        }

        g_score[start.y][start.x] = 0;
        key = h_score (start, end) +  g_score[start.y][start.x];
        
        insertNode (key, start, open);

        while (open.size () != 0) {
                //showOpenList (open);
                Pos cur = popMinNode (open);
                if (cur == end) {
                        Path* path = new Path;
                        recoverPath (start, end, *path, direction);
                        for (Path::iterator i = path->begin (); i != path->end (); ++ i) {
                                printf ("%d\n", *i);
                        }
                        return path;
                }
                closed.insert (cur);
                
                uint32_t cur_g_score = g_score[cur.y][cur.x];

                NodeList connected;
                getNextNodes (cur, connected);
                NodeList::iterator i;
                for (i = connected.begin (); i != connected.end (); ++ i) {
                        Pos next = *i;
                        uint32_t x = next.x;
                        uint32_t y = next.y;
                        uint32_t cost = m[y][x];
                        if (cost == 0) {
                                continue;
                        }
                        if (closed.count (next) != 0) {
                                continue;
                        }
                        if (x != cur.x && y != cur.y) {
                                /* we penalize diagonals */
                                cost *= 3;
                        }

                        uint32_t h = h_score (next, end);
                        uint32_t test_g_score = cost + cur_g_score;

                        if (g_score[y][x] == UINT_MAX) {
                                g_score[y][x] = test_g_score;
                                key = h + g_score[y][x];
                                insertNode (key, next, open);
                                direction[y][x] = calcDirection (cur, next);
                        } else if (test_g_score < g_score[y][x]) {
                                uint32_t old_f = h + g_score[y][x];
                                deleteNode (old_f, next, open); 
                                g_score[y][x] = test_g_score;
                                key = h + g_score[y][x];
                                insertNode (key, next, open);
                                direction[y][x] = calcDirection (cur, next);
                        }
                }
        }
        return NULL;
}
#endif

        
