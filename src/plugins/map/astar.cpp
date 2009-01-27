#include <set>
#include <queue>

#include "astar.h"

Node::~Node ()
{
}

Node *Node::next ()
{
        return _next;
}

Node *Node::last ()
{
        return _last;
}

void Node::show ()
{
}

AStar::~AStar ()
{
}

bool AStar::NodeCmp::operator () (const Node *a, const Node *b)
{
        return a->f_score >= b->f_score;
}

Node *AStar::getPath (Node *start, Node *goal)
{
typedef std::priority_queue<Node *, std::vector<Node *>, AStar::NodeCmp> NodePQ;
typedef std::set<Node *> NodeSet;

        NodeCmp cmp;
        NodePQ  open (cmp);
        NodeSet closed;
        /**
         * Annoyingly STL PQ does not provide count which is needed to test
         * if a node is already in the open set. Because I'm lazy, I keep a
         * concurrent set with all the open nodes in them
         */
        NodeSet openSet;
        std::pair<NodeSet::iterator, bool> res;

        NodeCostList adjacent;

        Node *current;
        Node *next;
        uint32_t cost;
        uint32_t cur_g_score;

        start->g_score = 0;
        start->h_score = getHScore (start, goal);
        start->f_score = start->g_score + start->h_score;
        start->_last = NULL;
        open.push (start);
        openSet.insert (start);

        while (!open.empty ()) {
                current = open.top ();
                if (current == goal) {
                        return _backtrace (current);
                }
                open.pop ();
                openSet.erase (current);
                closed.insert (current);

                adjacent.clear ();
                getAdjacentNodes (current, adjacent);

                NodeCostList::iterator i;
                for (i = adjacent.begin (); i != adjacent.end (); ++ i) {
                        next = (*i).first;
                        cost = (*i).second;
                
                        if (closed.count (next) != 0) {
                                continue;
                        }
                        cur_g_score = current->g_score + cost;

                        res = openSet.insert (next);
                        if (res.second) {
                                next->g_score = current->g_score + cost;
                                next->h_score = getHScore (next, goal);
                                next->f_score = next->g_score + next->h_score;
                                next->_last = current;
                                open.push (next);
                        } else if (cur_g_score < next->g_score) {
                                next->g_score = cur_g_score;
                                next->f_score = next->g_score + next->h_score;
                                next->_last = current;
                        }
                }
        }
        return NULL;
}

Node *AStar::_backtrace (Node *goal)
{
        goal->_next = NULL;
        while (goal->_last != NULL) {
                goal->_last->_next = goal;
                goal = goal->_last;
        }
        return goal;
}

