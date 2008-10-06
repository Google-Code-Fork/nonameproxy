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
        if (scanf ("%X", &s1) == EOF) return 0;
        if (scanf ("%X", &s2) == EOF) {
                printf ("Mal formed packet\n");
                return 0;
        }

        uint32_t size = 2 + s1 + s2 * 256 ;

        *buffer_ptr = new uint8_t[size];
        (*buffer_ptr)[0] = s1;
        (*buffer_ptr)[1] = s2;

        uint32_t i;
        for (i = 2; i < size; i ++) {
                uint32_t tmp;
                if (scanf ("%X", &tmp) == EOF) {
                        printf ("Mal formed packet\n");
                        return 0;
                }
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
        Pos& pos = gs->map->getCurPos ();
        pos.x = 100;
        pos.y = 100;
        pos.z = 6;

        int packetn = 1;
        while (1) {
                uint8_t* inpacket;
                int packet_size = loadpacket (&inpacket);
                if (packet_size == 0) {
                        break;
                }
                printf ("testing packet %d, size = %d\n", packetn, packet_size);
                uint8_t* outpacket = new uint8_t[packet_size];
                uint8_t* tmp = new uint8_t[packet_size];
                memcpy (tmp, inpacket, packet_size);
                
                NetworkMessage* inmsg = new NetworkMessage (packet_size, tmp);
                inmsg->setPos (8);
                inmsg->show ();
                printf ("\n");

//                GRMessageList ml (inmsg, gs, dat);
                GSMessageList ml (inmsg, gs, dat);

                NetworkMessage* outmsg = ml.put ();
                outmsg->show();
                memcpy (outpacket, outmsg->getBuffer (), packet_size);

                int len = *(int*)&inpacket[6] & 0x0000FFFF;
                int nerrors = 0;
                for (int i = 8; i < len + 8; i ++) {
                        if (inpacket[i] != outpacket[i]) {
                                nerrors ++;
                                printf ("in: 0x%X out: 0x%X at: %d packet %d",
                                        inpacket[i], outpacket[i], i, packetn);
                                if (nerrors > 10) break;
                        }
                }
                if (!nerrors) {
                        printf ("yay packet %d\n\n", packetn);
                } else {
                        break;
                }
                packetn ++;
                delete inpacket;
                delete outpacket;
        }
        return 0;
}
        
