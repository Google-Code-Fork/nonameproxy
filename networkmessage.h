//Yes, i copied the name from yact. I also like the way they
//use macros to read the network message, so i stole those too
#ifndef __NETWORK_MESSAGE_H
#define __NETWORK_MESSAGE_H

#include <stdint.h>
#include "connection.h"
#include "tibiacrypt.h"
#include "tibiatypes.h"

class NetworkMessage
{
        public:
                NetworkMessage (uint32_t size);
                virtual ~NetworkMessage ();

                void show ();
//        protected:
                //these functions should only be used by connection
                //for efficient socket use
                NetworkMessage (uint32_t size, uint8_t* buffer);
                uint8_t* getBuffer ();

                //these functions provide the fundamental methods of reading
                //and writing data to a network message all other all
                //other operations should be defined in terms of these
                bool getU8  (uint8_t&  val);
                bool getU16 (uint16_t&  val);
                bool getU32 (uint32_t& val);
                bool getN   (uint8_t* dest, uint32_t n);

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
        private:
                uint8_t* _buffer;
                uint32_t _size;
                uint32_t _curpos;
};

#endif

