#ifndef __MAP_GRAPH_H
#define __MAP_GRAPH_H

#include <stdint.h>

#include <map>
#include <list>

#include "pos.h"
#include "mapstate.h"

class Node;

typedef Node*                           YNode;

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

class MapGraph
{
        public:
                bool addNode (const Pos& pos, Node* node);
                bool deleteNode (const Pos& pos);

                bool addEdge (const Pos& p1, const Pos& p2, uint32_t cost);
                bool deleteEdge (const Pos& p1, const Pos& p2);

                uint32_t getRange (const Pos& p1, const Pos& p2, PosList& nodes);

                const EdgeMap& getEdges (const Pos& pos);

                void show ();

        private:
                NodeGraph       _graph;
                AdjacencyMap    _edges;
};

#endif

