/*****************************************************************************
 * noname proxy
 *****************************************************************************
 *
 *****************************************************************************
 * This program is free software; you can redistribute it and*or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *****************************************************************************/

//Yes, i copied the name from yact. I also like the way they
//use macros to read the network message, so i stole those too
#ifndef __NETWORK_MESSAGE_H
#define __NETWORK_MESSAGE_H

#include <stdint.h>
#include "connection.h"
#include "tibiacrypt.h"
#include "tibiatypes.h"
#include "messagefactory.h"

//testing puposes
#include "tibiamessage.h"
extern uint32_t stamp;

class NetworkMessage
{
        public:
                NetworkMessage (uint32_t size);
                virtual ~NetworkMessage ();

                void prepRSAHeader ();
                void prepHeader ();
                void writeRSAHeader ();
                void writeHeader ();

                void show ();
        protected:
                //these functions should only be used by connection
                //for efficient socket use
                NetworkMessage (uint32_t size, uint8_t* buffer);
                uint8_t* getBuffer ();

                void setPos (uint32_t pos);
                uint32_t getPos ();

                bool isRSA ();
                bool isEOF ();
                bool isXTEAEOF ();
                bool isRSAEOF ();

                //these functions provide the fundamental methods of reading
                //and writing data to a network message all other all
                //other operations should be defined in terms of these
                bool getU8  (uint8_t&  val);
                bool getU16 (uint16_t&  val);
                bool getU32 (uint32_t& val);
                bool getN   (uint8_t* dest, uint32_t n);

                //Peek functions are exclusively for the factories
                bool peekU8  (uint8_t& val);
                bool peekU16 (uint16_t& val);
                bool peekU32 (uint32_t& val);

                bool putU8  (uint8_t  val);
                bool putU16 (uint16_t val);
                bool putU32 (uint32_t val);
                bool putN   (const uint8_t* src, uint32_t n);

                //all other more complex data types should be defined as
                //class derived from TibiaMessage
                //friend class TibiaMessage;
                friend class TWord8;
                friend class TWord16;
                friend class TWord32;

                friend class TString;
                friend class TByteBuffer;

                friend class Connection;
                friend class TibiaCrypt;

                friend class LSMessageFactory;
                friend class LRMessageFactory;
                friend class GSMessageFactory;
                friend class GRMessageFactory;

                friend class TThingFactory;
                friend class TOutfitFactory;
                friend class TSpeakFactory;

                //this is for testing only
                friend class GRMTileUpdate;

        private:
                uint8_t* _buffer;
                uint32_t _size;
                uint32_t _curpos;
};

#endif

