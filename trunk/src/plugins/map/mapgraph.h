#ifndef __MAP_GRAPH_H
#define __MAP_GRAPH_H

#include <stdint.h>

#include <map>
#include <list>

#include "pos.h"
#include "astar.h"
#include "mapstate.h"

class GraphNode;

typedef GraphNode *                     YNode;

typedef std::pair<uint32_t, YNode>      XNodePair;
typedef std::map<uint32_t, YNode>       XNode;

typedef std::pair<uint32_t, XNode>      ZNodePair;
typedef std::map<uint32_t, XNode>       ZNode;
typedef ZNode                           NodeGraph[MAP_MAX_FLOOR];

typedef std::pair<Pos, uint32_t>        EdgePair;
typedef std::map<Pos, uint32_t>         EdgeMap;

typedef std::pair<Pos, EdgeMap>         AdjacencyPair;
typedef std::map<Pos, EdgeMap>          AdjacencyMap;

typedef std::list<Pos>                  PosList;

class GraphNode : public Node
{
        public:
                GraphNode (const Pos &pos);
                virtual ~GraphNode ();
                const Pos &getPos ();

                void show ();
        private:
                Pos _pos;
};

class MapGraph : public AStar
{
        public:
                virtual ~MapGraph ();

                virtual uint32_t getHScore (Node *n, Node *goal);
                virtual void getAdjacentNodes (Node *n, NodeCostList &ns);

                bool addNode (const Pos &pos, GraphNode *node);
                bool deleteNode (const Pos &pos);
                GraphNode *getNode (const Pos &pos);

                bool addEdge (const Pos &p1, const Pos &p2, uint32_t cost);
                bool deleteEdge (const Pos &p1, const Pos &p2);

                uint32_t getRange (const Pos &p1, const Pos &p2, PosList &nodes);

                const EdgeMap& getEdges (const Pos &pos);

                void show ();

        private:
                NodeGraph       _graph;
                AdjacencyMap    _edges;
};

#endif

