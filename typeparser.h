#ifndef __TYPE_PARSER
#define __TYPE_PARSER

/**************************************************************************
 * The type parser is a thin layer that sits between the tibia types used
 * for communicating over the network and the internal types used in the
 * gamestate.
 * The hook mechanism has a draw back in that it isolates any added
 * functionality to a single hook. Instead of parsing a type in a hook,
 * add the neccesary funcionality to the type parser, and then use it
 * inside your hook.
 **************************************************************************/

#include "tibiatypes.h"
 /* gamestate.h includes many other types such as pos, tile, and thing
  * *** note that you will not be able to include typeparser.h inside any
  *     header that includes gamestate (which is included by client).
  *     Forward declare this class and include it in your .cpp file. */
#include "gamestate.h"

class TypeParser
{
        public:
                /* conversion functions */
                Pos toPos (const TPos& tpos);
                TPos toTPos (const Pos& pos);

                /* due to the fact that these classes are derived we need to
                 * know what were converting, before we make the conversion */
                Item toItem (const TItem& titem);
                XItem toXItem (const TXItem& txitem);

                /* parsing functions */
                bool parseMapDescription (GameState* gs, 
                                          TMapDescription& map,
                                          DatReader* dat);

                /* gamestate -> tibia type functions */

        private:
                bool i_parseFloor (GameState* gs, 
                                   TMapDescription& map,
                                   uint32_t z,
                                   uint32_t& skip,
                                   DatReader* dat);

                bool i_parseTile (GameState* gs, 
                                  TMapDescription& map,
                                  Tile& t,
                                  uint32_t& skip,
                                  DatReader* dat);

                bool i_parseThing (GameState* gs,
                                   const TThing& tthing,
                                   Tile& t,
                                   DatReader* dat);
};

#endif 

