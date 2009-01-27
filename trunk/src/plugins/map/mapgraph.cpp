#include <stdio.h>
#include <stdlib.h>

#include "mapgraph.h"

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAN(x,y) ((x) > (y) ? (x) : (y))

GraphNode::GraphNode (const Pos &pos)
{
        _pos = pos;
}

GraphNode::~GraphNode ()
{
}

const Pos& GraphNode::getPos ()
{
        return _pos;
}

void GraphNode::show ()
{
        printf ("(%d,%d,%d)\n", _pos.x, _pos.y, _pos.z);
}

MapGraph::~MapGraph ()
{
        /*TODO massive cleanup */
}

uint32_t MapGraph::getHScore (Node *n, Node *goal)
{
        const Pos &npos = ((GraphNode *)n)->getPos ();
        const Pos &gpos = ((GraphNode *)goal)->getPos ();

        return abs (npos.x - gpos.x) + 
               abs (npos.y - gpos.y) + 
               abs (npos.z - gpos.z);
}

void MapGraph::getAdjacentNodes (Node *n, NodeCostList &ns)
{
        const EdgeMap &edges = getEdges (((GraphNode *)n)->getPos ());
        EdgeMap::const_iterator i;
        for (i = edges.begin (); i != edges.end (); ++ i) {
                GraphNode *add = getNode ((*i).first);
                if (add == NULL) {
                        printf ("getAdjacentNodes error: ");
                        printf ("edge exists but node does not\n");
                }
                ns.push_back (NodeCost (add, (*i).second));
        }
}

bool MapGraph::addNode (const Pos &pos, GraphNode *node)
{
        ZNode &Z = _graph[pos.z];
        ZNode::iterator zi = Z.find (pos.x);
        if (zi == Z.end ()) {
                std::pair<ZNode::iterator, bool> res;
                res = Z.insert (ZNodePair (pos.x, XNode ()));
                if (res.second == false) {
                        printf ("addNode error: XNode insert failed\n");
                        return false;
                }
                zi = res.first;
        }
        
        XNode &X = (*zi).second;
        if (X.count (pos.y) != 0) {
                printf ("addNode error: pos already exists\n");
                return false;
        }

        X.insert (XNodePair (pos.y, node));
        return true;
}

bool MapGraph::deleteNode (const Pos &pos)
{
        ZNode &Z = _graph[pos.z];
        ZNode::iterator zi = Z.find (pos.x);
        if (zi == Z.end ()) {
                printf ("deleteNode error: pos doesnt exist\n");
                return false;
        }

        XNode &X = (*zi).second;
        XNode::iterator xi = X.find (pos.y);
        if (xi == X.end ()) {
                printf ("deleteNode error: pos doesnt exist\n");
                return false;
        }

        /* note we need a copy here as deleteEdge will modify the original */
        const EdgeMap &edges = getEdges (pos);
        EdgeMap::const_iterator i;
        for (i = edges.begin (); i != edges.end (); ++ i) {
                deleteEdge ((*i).first, pos);
        }

        delete (*xi).second;
        X.erase (xi);
        if (X.size () == 0) {
                Z.erase (zi);
        }
        return true;
}

GraphNode *MapGraph::getNode (const Pos &pos)
{
        ZNode &Z = _graph[pos.z];
        ZNode::iterator zi = Z.find (pos.x);
        if (zi == Z.end ()) {
                return NULL;
        }

        XNode &X = (*zi).second;
        XNode::iterator xi = X.find (pos.y);
        if (xi == X.end ()) {
                return NULL;
        }

        return (*xi).second;
}

bool MapGraph::addEdge (const Pos &p1, const Pos &p2, uint32_t cost)
{
        AdjacencyMap::iterator i1 = _edges.find (p1);
        AdjacencyMap::iterator i2 = _edges.find (p2);
        if (i1 == _edges.end ()) {
                std::pair<AdjacencyMap::iterator, bool> res;
                res = _edges.insert (AdjacencyPair (p1, EdgeMap ()));
                if (res.second == false) {
                        printf ("addEdge error: p1 insert failed\n");
                        return false;
                }
                i1 = res.first;
        }
        if (i2 == _edges.end ()) {
                std::pair<AdjacencyMap::iterator, bool> res;
                res = _edges.insert (AdjacencyPair (p2, EdgeMap ()));
                if (res.second == false) {
                        printf ("addEdge error: p2 insert failed\n");
                        return false;
                }
                i2 = res.first;
        }
        (*i1).second.insert (EdgePair (p2, cost));
        (*i2).second.insert (EdgePair (p1, cost));
        return true;
}

bool MapGraph::deleteEdge (const Pos &p1, const Pos &p2)
{
        if (p1 == p2) {
                return true;
        }
        AdjacencyMap::iterator i1 = _edges.find (p1);
        AdjacencyMap::iterator i2 = _edges.find (p2);
        if (i2 == _edges.end ()) {
                printf ("deleteEdge error: p1 doesnt exist\n");
                return false;
        }
        if (i2 == _edges.end ()) {
                printf ("deleteEdge error: p2 doesnt exist\n");
                return false;
        }
        if ((*i1).second.erase (p2) == 0) {
                printf ("deleteEdge error: p1 -> p2 failed\n");
                return false;
        }
        if ((*i1).second.size () == 0) {
                _edges.erase (i1);
        }
        if ((*i2).second.erase (p1) == 0) {
                printf ("deleteEdge error: p2 -> p1 failed\n");
                return false;
        }
        if ((*i2).second.size () == 0) {
                _edges.erase (i2);
        }
        return true;
}

const EdgeMap &MapGraph::getEdges (const Pos &pos)
{
        AdjacencyMap::iterator i = _edges.find (pos);
        if (i == _edges.end ()) {
                printf ("getEdges error: pos doesnt exist\n");
        }
        return (*i).second;
}

uint32_t MapGraph::getRange (const Pos &p1, const Pos &p2, PosList &nodes)
{
        uint32_t minz = MIN (p1.z, p2.z);
        uint32_t maxz = MAN (p1.z, p2.z);

        uint32_t minx = MIN (p1.x, p2.x);
        uint32_t maxx = MAN (p1.x, p2.x);

        uint32_t miny = MIN (p1.y, p2.y);
        uint32_t maxy = MAN (p1.y, p2.y);

        for (uint32_t z = minz; z != maxz; z ++) {
                ZNode::iterator x    = _graph[z].lower_bound (minx);
                ZNode::iterator xend = _graph[z].upper_bound (maxx);
                for (; x != xend; ++ x) {
                        XNode &X = (*x).second;
                        XNode::iterator y    = X.lower_bound (miny);
                        XNode::iterator yend = X.upper_bound (maxy);
                        for (; y != yend; ++ y) {
                                nodes.push_back (Pos (x->first, y->first, z));
                        }
                }
        }
        return nodes.size ();
}

void MapGraph::show ()
{
        printf ("Vertices\n");
        for (uint32_t z = 0; z != MAP_MAX_FLOOR; z ++) {
                if(_graph[z].size () == 0) {
                        continue;
                }
                printf ("z = %d\n", z);
                ZNode::iterator x;
                for (x = _graph[z].begin (); x != _graph[z].end (); ++ x) {
                        printf ("\tx = %d\n", (*x).first);
                        const XNode &X = (*x).second;
                        XNode::const_iterator y;
                        for (y = X.begin (); y != X.end (); ++ y) {
                                printf ("\t\ty = %d\n", (*y).first);
                        }
                }
        }

        printf ("Edges\n");
        AdjacencyMap::iterator e;
        for (e = _edges.begin (); e != _edges.end (); ++ e) {
                Pos k = (*e).first;
                printf ("(%d,%d,%d):", k.x, k.y, k.z);
                
                const EdgeMap &connections = (*e).second;
                EdgeMap::const_iterator i;
                for (i = connections.begin (); i != connections.end (); ++ i) {
                        Pos p = (*i).first;
                        printf (" (%d,%d,%d;%d)", p.x, p.y, p.z, (*i).second);
                }
                printf ("\n");
        }
}

