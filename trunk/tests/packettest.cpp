#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "networkmessage.h"
#include "messagelist.h"
#include "datreader.h"
#include "gamestate.h"

//load a packet into a buffer and return its size
int loadpacket (uint8_t** buffer_ptr)
{
        uint32_t s1 = 0, s2 = 0;
        //read the size
        scanf ("%X", &s1);
        scanf ("%X", &s2);

        uint32_t size = 2 + s1 + s2 * 256 ;
        printf ("size: %d\n", size);

        *buffer_ptr = new uint8_t[size];
        (*buffer_ptr)[0] = s1;
        (*buffer_ptr)[1] = s2;

        uint32_t i;
        for (i = 2; i < size; i ++) {
                uint32_t tmp;
                scanf ("%X", &tmp);
                (*buffer_ptr)[i] = tmp;
                if (i % 16 == 15) {
                        while (getchar () != '\n');
                }
        }
        if (i % 16 != 0) while (getchar () != '\n');

        return (int)size;
}

int main (int argc, char** argv)
{
        DatReader* dat = new DatReader ();
        GameState* gs = new GameState ();
        Pos pos (100,100,7); //ground floor
        gs->map->setCurPos (pos);

        int packetn = 1;
        while (1) {
                printf ("testing packet %d\n", packetn);
                uint8_t* inpacket;
                int packet_size = loadpacket (&inpacket);
                uint8_t* outpacket = new uint8_t[packet_size];
                uint8_t* tmp = new uint8_t[packet_size];
                memcpy (tmp, inpacket, packet_size);
                
                NetworkMessage* inmsg = new NetworkMessage (packet_size, tmp);
                inmsg->setPos (4);
                inmsg->show ();
                printf ("\n\n");

                GRMessageList ml (inmsg, gs, dat);

                NetworkMessage* outmsg = ml.put ();
                outmsg->show();
                memcpy (outpacket, outmsg->getBuffer (), packet_size);

                int len = *(int*)&inpacket[2] & 0x0000FFFF;
                int nerrors = 0;
                for (int i = 4; i < len; i ++) {
                        if (inpacket[i] != outpacket[i]) {
                                nerrors ++;
                                printf ("in: 0x%X out: 0x%X at: %d packet %d",
                                        inpacket[i], outpacket[i], i, packetn);
                                if (nerrors > 10) break;
                        }
                }
                if (!nerrors) {
                        printf ("yay packet %d\n", packetn);
                }
                packetn ++;
                delete inpacket;
                delete outpacket;
        }
        return 0;
}
        
