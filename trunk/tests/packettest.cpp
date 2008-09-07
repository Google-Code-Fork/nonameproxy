#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "testpacket.h"
#include "networkmessage.h"
#include "messagelist.h"
#include "datreader.h"

class GameState;

int main (int argc, char** argv)
{
        DatReader* dat = new DatReader ();
        GameState* gs = NULL;

        uint8_t inpacket[TEST_PACKET_SIZE] = TEST_PACKET;
        uint8_t outpacket[TEST_PACKET_SIZE];
        uint8_t* tmp = new uint8_t[TEST_PACKET_SIZE];

        memcpy (tmp, inpacket, TEST_PACKET_SIZE);

        NetworkMessage* inmsg = new NetworkMessage (TEST_PACKET_SIZE, tmp);
        inmsg->setPos (4);
        inmsg->show ();
        printf ("\n\n");

        GRMessageList ml (inmsg, gs, dat);

        NetworkMessage* outmsg = ml.put ();
        outmsg->show();
        memcpy (outpacket, outmsg->getBuffer (), TEST_PACKET_SIZE);

        int len = *(int*)&inpacket[2] & 0x0000FFFF;
        int nerrors = 0;
        for (int i = 4; i < len; i ++) {
                if (inpacket[i] != outpacket[i]) {
                        nerrors ++;
                        printf ("in: 0x%X out: 0x%X at: %d\n", inpacket[i], outpacket[i], i);
                        if (nerrors > 10) break;
                }
        }
        if (!nerrors) {
                printf ("yay\n");
        }
        return 0;
}
        
