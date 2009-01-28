#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#include <map>
#include <set>
#include <list>

#include "localmap.h"
#include "map.h"
#include "enums.h"

#define BUFFER_SIZE 256 

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

#define MIN_COST 100

typedef std::list<Pos> NodeList;
typedef std::map<uint32_t, NodeList> NodeMap;
typedef std::pair<uint32_t, NodeList> ListPair;
typedef std::set<Pos, bool (*) (const Pos&, const Pos&)> NodeSet;
typedef std::list<direction_t> Path;

Path* findPath (Pos start, Pos end, uint32_t[][18]);

LocalMap::LocalMap ()
{
        _walking = false;
}

bool LocalMap::at_target ()
{
        if (_client->gstate->map->getCurPos () == _to) {
                return true;
        } else {
                return false;
        }
}

void LocalMap::stop ()
{
        if (_walking) {
                GSMessageList gsml;
                gsml.add (new GSMAutoWalkCancel ());
                _client->sendToServer (gsml);
        }
        _walking = false;
}

bool LocalMap::kick ()
{
        if (!_walking) {
                return false;
        }
        const Pos& curPos = _client->gstate->map->getCurPos ();
        if (curPos == _to) {
                return true;
        }
        if (curPos == _tmpTo) {
                return walk ();
        }
        return true;
}

bool LocalMap::walk ()
{
        if (at_target ()) {
                _walking = false;
        }
        _walking = true;

        MapState* map = _client->gstate->map;
        const Pos& curPos = map->getCurPos ();

        Pos start = i_globalToLocal (curPos);
        Pos end = i_globalToLocal (_to); 
        /* quick check to make sure target in range to prevent crashing */
        if (!(0 <= end.x && end.x < 18) 
         || !(0 <= end.y && end.y < 14))
        {
                printf ("map walk error: end out of bounds\n");
                return false;
        }

        uint32_t (*m)[18];
        m = i_makeMap (curPos, _to);

        Path* path = findPath (start, end, m);

        if (path == NULL) {
                _walking = false;
                return false;
        }
        
        _tmpTo = curPos;
        uint32_t count = 0;
        TDirectionList* tPath = new TDirectionList ();
        Path::iterator i;
        for (i = path->begin (); i != path->end (); ++ i) {
                if (count == 10) {
                        break;
                }
                count ++;

                direction_t dir = *i;
                tPath->add (dir);

                /* follow the path to find the new tmpTo */
                if (dir == DIRECTION_SE 
                 || dir == DIRECTION_EAST 
                 || dir == DIRECTION_NE) 
                {
                        _tmpTo.x ++;
                } else if (dir == DIRECTION_SW 
                        || dir == DIRECTION_WEST
                        || dir == DIRECTION_NW) 
                {
                        _tmpTo.x --;
                }
                if (dir == DIRECTION_NE
                 || dir == DIRECTION_NORTH
                 || dir == DIRECTION_NW)
                {
                        _tmpTo.y --;
                } else if (dir == DIRECTION_SE
                        || dir == DIRECTION_SOUTH
                        || dir == DIRECTION_SW)
                {
                        _tmpTo.y ++;
                }
        }

        GSMessageList gsml;
        gsml.add (new GSMAutoWalk (tPath));
        _client->sendToServer (gsml);

        delete path;
        return true;
}

uint32_t LocalMap::calcPathCost (const Pos& from, const Pos& to)
{
        Pos start = i_globalToLocal (from);
        Pos end = i_globalToLocal (to); 
        /* quick check to make sure target in range to prevent crashing */
        if (!(0 <= start.x && start.x < 18) 
         || !(0 <= start.y && start.y < 14))
        {
                printf ("map walk error: start out of bounds\n");
                return false;
        }

        if (!(0 <= end.x && end.x < 18) 
         || !(0 <= end.y && end.y < 14))
        {
                printf ("map walk error: end out of bounds\n");
                return false;
        }

        uint32_t (*m)[18];
        m = i_makeMap (from, to, true);

        Path* path = findPath (start, end, m);

        if (path == NULL) {
                return UINT_MAX;
        }
        
        uint32_t cost = 0;
        Path::iterator i;
        for (i = path->begin (); i != path->end (); ++ i) {
                direction_t dir = *i;
                if (dir == DIRECTION_SE 
                 || dir == DIRECTION_EAST 
                 || dir == DIRECTION_NE) 
                {
                        start.x ++;
                } else if (dir == DIRECTION_SW 
                        || dir == DIRECTION_WEST
                        || dir == DIRECTION_NW) 
                {
                        start.x --;
                }
                if (dir == DIRECTION_NE
                 || dir == DIRECTION_NORTH
                 || dir == DIRECTION_NW)
                {
                        start.y --;
                } else if (dir == DIRECTION_SE
                        || dir == DIRECTION_SOUTH
                        || dir == DIRECTION_SW)
                {
                        start.y ++;
                }
                cost += m[start.y][start.x];
                printf ("%d\n", cost);
        }

        delete path;
        return cost;
}

bool LocalMap::set_target (uint32_t x, uint32_t y, uint32_t z)
{
        MapState* map = _client->gstate->map;
        const Pos& curPos = map->getCurPos ();
        if (!(curPos.x - 8 <= x && x <= curPos.x + 9)
         || !(curPos.y - 6 <= y && y <= curPos.y + 7))
        {
                printf ("map set_target error: local target out of bounds\n");
                return false;
        } else if (z != curPos.z) {
                printf ("map set_target error: local z != curPos.z\n");
                return false;
        }
        _to = Pos (x, y, z);
        _tmpTo = curPos;
        return true;
}

bool LocalMap::set_target (const Pos& pos)
{
        return set_target (pos.x, pos.y, pos.z);
}
        
        
uint32_t (*LocalMap::i_makeMap (const Pos& from, const Pos& to,
                          bool ignoreCreatures /*=false*/))[MAP_X]
{
        uint32_t (*m)[MAP_X];

        uint32_t curCycle = _client->getCycle ();
        if (ignoreCreatures) {
                m = _nc_map;
                if (_nc_cycle == curCycle) {
                        return m;
                }
                _nc_cycle = curCycle;
        } else {
                m = _c_map;
                if (_c_cycle == curCycle) {
                        return m;
                }
                _c_cycle = curCycle;
        }

        MapState* map = _client->gstate->map;
        const Pos& curPos = map->getCurPos ();

        uint32_t miny = curPos.y - 6;
        uint32_t maxy = curPos.y + 7;
        uint32_t minx = curPos.x - 8;
        uint32_t maxx = curPos.x + 9;
        uint32_t z = curPos.z;

        uint32_t my = 0;
        uint32_t mx = 0;

        my = 0;
        for (uint32_t y = miny; y <= maxy; y ++) {
                mx = 0;
                for (uint32_t x = minx; x <= maxx; x ++) {
                        const Tile& tile = map->getTile (x, y, z);
                        m[my][mx] = tile.getWalkCost (_client->dat, 
                                                      ignoreCreatures);
                        mx ++;
                }
                my ++;
        }
        /* we take for granted that start and end are open */
        Pos start = i_globalToLocal (to);
        Pos end = i_globalToLocal (from);

        if (!(0 <= start.x && start.x < 18) 
         || !(0 <= start.y && start.y < 14))
        {
                printf ("i_makeMap error start out of bounds\n");
                return m;
        }
        if (!(0 <= end.x && end.x < 18) 
         || !(0 <= end.y && end.y < 14))
        {
                printf ("i_makeMap error end out of bounds\n");
                return m;
        }
        if (m[start.y][start.x] == 0) {
                m[start.y][start.x] = MIN_COST;
        }
        if (m[end.y][end.x] == 0) {
                m[end.y][end.x] = MIN_COST;
        }

        for (uint32_t y = 0; y < 14; y ++) {
                for (uint32_t x = 0; x < 18; x ++) {
                        if (m[y][x] == 0) {
                                printf ("   ");
                        } else {
                                printf ("%d", m[y][x]);
                        }
                }
                printf ("\n");
        }
        printf ("\n");

        return m;
}
        
Pos LocalMap::i_globalToLocal (const Pos& pos)
{
        const Pos& curPos = _client->gstate->map->getCurPos ();
        return Pos (pos.x - curPos.x + 8, pos.y - curPos.y + 6, pos.z);
}

void LocalMap::i_load (uint32_t pluginId, Client* client)
{
        _client = client;
        _pluginId = pluginId;

        _c_cycle = _client->getCycle ();
        _nc_cycle = _client->getCycle ();
}

void LocalMap::i_unload ()
{
}

/******************************************************************
 * A* stuff
 ******************************************************************/

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
        
