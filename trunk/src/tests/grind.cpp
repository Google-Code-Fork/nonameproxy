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
        DatReader* dat = NULL;
        GameState* gs = NULL;

        uint8_t* inpacket;
        int packet_size = loadpacket (&inpacket);

        uint8_t* tmp = new uint8_t[packet_size];
        memcpy (tmp, inpacket, packet_size);

        NetworkMessage* inmsg = new NetworkMessage (packet_size, tmp);
        inmsg->setPos (4);

        TibiaMessage* tm = new GRMFYI (inmsg, gs, dat);

        delete tm;
        delete inpacket;
        delete inmsg;
        return 0;
}
        
