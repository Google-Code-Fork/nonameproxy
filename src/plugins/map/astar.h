#ifndef __ASTAR_H
#define __ASTAR_H

#include <stdint.h>
#include <list>

class Node
{
        public:
                virtual ~Node ();
                virtual void show ();
                Node *next ();
                Node *last ();

        private:
                uint32_t g_score;
                uint32_t h_score;
                uint32_t f_score;
                Node *_next;
                Node *_last;
                friend class AStar;
};
        
typedef std::pair<Node *, uint32_t> NodeCost;
typedef std::list<NodeCost> NodeCostList;

class AStar
{
        public:
                virtual ~AStar ();

                virtual uint32_t getHScore (Node *n, Node *goal) = 0;
                virtual void getAdjacentNodes (Node *n, NodeCostList &ns) = 0;

                Node *getPath (Node *start, Node *goal);
        private:
                class NodeCmp
                {
                        public:
                                bool operator () (const Node *a, const Node *b);
                };

                Node *_backtrace (Node *goal);
};

#endif

