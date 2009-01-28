#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "mapgraph.h"

int32_t main (int32_t argc, char **argv)
{
        MapGraph graph;
#if 1
        GraphNode *n00 = new GraphNode (Pos (0, 0, 0));
        GraphNode *n30 = new GraphNode (Pos (3, 0, 0));
        GraphNode *n33 = new GraphNode (Pos (3, 3, 0));
        GraphNode *n03 = new GraphNode (Pos (0, 3, 0));

        GraphNode *n11 = new GraphNode (Pos (1, 1, 0));
        GraphNode *n21 = new GraphNode (Pos (2, 1, 0));
        GraphNode *n22 = new GraphNode (Pos (2, 2, 0));
        GraphNode *n12 = new GraphNode (Pos (1, 2, 0));

        graph.addNode (Pos (0, 0, 0), n00);
        graph.addNode (Pos (3, 0, 0), n30);
        graph.addNode (Pos (3, 3, 0), n33);
        graph.addNode (Pos (0, 3, 0), n03);

        graph.addNode (Pos (1, 1, 0), n11);
        graph.addNode (Pos (2, 1, 0), n21);
        graph.addNode (Pos (2, 2, 0), n22);
        graph.addNode (Pos (1, 2, 0), n12);

        graph.addEdge (Pos (0, 0, 0), Pos (3, 0, 0), 3);
        graph.addEdge (Pos (3, 0, 0), Pos (3, 3, 0), 4);
        graph.addEdge (Pos (3, 3, 0), Pos (0, 3, 0), 3);
        graph.addEdge (Pos (0, 3, 0), Pos (0, 0, 0), 3);

        graph.addEdge (Pos (1, 1, 0), Pos (2, 1, 0), 1);
        graph.addEdge (Pos (2, 1, 0), Pos (2, 2, 0), 1);
        graph.addEdge (Pos (2, 2, 0), Pos (1, 2, 0), 1);
        graph.addEdge (Pos (1, 2, 0), Pos (1, 1, 0), 1);

        graph.addEdge (Pos (0, 0, 0), Pos (1, 1, 0), 3);
        graph.addEdge (Pos (3, 0, 0), Pos (2, 1, 0), 2);
        graph.addEdge (Pos (3, 3, 0), Pos (2, 2, 0), 2);
        graph.addEdge (Pos (0, 3, 0), Pos (1, 2, 0), 2);

        graph.show ();

        Node *path = graph.getPath (n00, n33);
        printf ("yay\n");

        while (path != NULL) {
                ((GraphNode *)path)->show ();
                path = path->next ();
        }
#else

        GraphNode *n123 = new GraphNode (Pos (1, 2, 3));
        GraphNode *n456 = new GraphNode (Pos (4, 5, 6));
        GraphNode *n789 = new GraphNode (Pos (7, 8, 9));

        graph.addNode (Pos (1, 2, 3), n123);
        graph.addNode (Pos (4, 5, 6), n456);
        graph.addNode (Pos (7, 8, 9), n789);
        graph.addEdge (Pos (1, 2, 3), Pos (4, 5, 6), 1);

        graph.show ();
        Node *path = graph.getPath (n123, n456);
        printf ("yay\n");

        while (path != NULL) {
                ((GraphNode *)path)->show ();
                path = path->next ();
        }
#endif
}

