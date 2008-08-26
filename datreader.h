#ifndef __DAT_READER_H
#define __DAT_READER_H

#include "tibiatypes.h"

class SafeFile;

class ThingData
{
        public:
                //a ThingData will read itself from the dat file
                ThingData (SafeFile* datfile, uint16_t id);
                void show ();

                bool isTopOrder1 ();
                bool isTopOrder2 ();
                bool isTopOrder3 ();
                bool isContainer ();
                bool isStackable ();
                bool isCorpse ();
                bool isUsable ();
                bool isRune ();
                bool isWritable ();
                bool isReadable ();
                bool isFluidContainer ();
                bool isSplash ();
                bool isBlocking ();
                bool isImmobile ();
                bool isMissileBlocking ();
                bool isPathBlocking ();
                bool isPickupable ();
                bool isHangable ();
                bool isHangableHorizontal ();
                bool isHangableVertical ();
                bool isRotatable ();
                bool isLightSource ();
                bool isFloorChange ();
                bool isOffset ();
                bool isRaised ();
                bool isIdleAnimation ();
                bool isMiniMap ();
                bool isGround ();
                bool isLadder ();
                bool isSewer ();
                bool isRopeSpot ();
                bool isSwitch ();
                bool isDoor ();
                bool isDoorWithLock ();
                bool isStairs ();
                bool isMailBox ();
                bool isDepot ();
                bool isTrash ();
                bool isHole ();
                bool isSpecialDescription ();
                bool isReadOnly ();
                //these functions actually return something
                TWord16*        getSpeed ();
                TWord16*        getWriteLimit ();
                TWord16*        getReadLimit ();
                //TItemLight*     getLight ();
                TWord16*        getOffsetX ();
                TWord16*        getOffsetY ();
                TWord16*        getRaised ();
                TWord16*        getMiniMap ();

        private:
                void parseOp (SafeFile* datfile, uint8_t op);

                enum TibiaItemFlags {
                        f_isTopOrder1         = 0x00000002,
                        f_isTopOrder2         = 0x00000004,
                        f_isTopOrder3         = 0x00000008,
                        f_isContainer         = 0x00000010,
                        f_isStackable         = 0x00000020,
                        f_isCorpse            = 0x00000040,
                        f_isUsable            = 0x00000080,
                        f_isRune              = 0x00000100,
                        f_isWritable          = 0x00000200,
                        f_isReadable          = 0x00000400,
                        f_isFluidContainer    = 0x00000800,
                        f_isSplash            = 0x00001000,
                        f_isBlocking          = 0x00002000,
                        f_isImmobile          = 0x00004000,
                        f_isMissileBlocking   = 0x00008000,
                        f_isPathBlocking      = 0x00010000,
                        f_isPickupable        = 0x00020000,
                        f_isHangable          = 0x00040000,
                        f_isHangableHorizontal= 0x00080000,
                        f_isHangableVertical  = 0x00100000,
                        f_isRotatable         = 0x00200000,
                        f_isLightSource       = 0x00400000,
                        f_isFloorChange       = 0x00800000,
                        f_isOffset            = 0x01000000,
                        f_isRaised            = 0x02000000,
                        f_isLayer             = 0x04000000,
                        f_isIdleAnimation     = 0x08000000,
                        f_isMiniMap           = 0x10000000,
                        f_isGround            = 0x20000000,
                };
        
                enum TibiaItemHelp {
                        h_isLadder            = 0x4C,
                        h_isSewer             = 0x4D,
                        h_isRopeSpot          = 0x4E,
                        h_isSwitch            = 0x4F,
                        h_isDoor              = 0x50,
                        h_isDoorWithLock      = 0x51,
                        h_isStairs            = 0x52,
                        h_isMailBox           = 0x53,
                        h_isDepot             = 0x54,
                        h_isTrash             = 0x55,
                        h_isHole              = 0x56,
                        h_isSpecialDescription= 0x57,
                        h_isReadOnly          = 0x58,
                };

                uint32_t _thingFlags;
                uint8_t  _helpByte;

                uint16_t _speed;
                uint16_t _writeLimit;
                uint16_t _readLimit;
                uint16_t _lightColor;
                uint16_t _lightRadius;
                uint16_t _offsetX;
                uint16_t _offsetY;
                uint16_t _raised;
                uint16_t _miniMapColor;
                uint16_t _id;
                //i dont keep any of the sprite data
};
        
class DatReader
{
        public:
                DatReader ();
                virtual ~DatReader ();
                //ThingData* getItemData (TItem* item);
                ThingData* getItemData (TWord16* itemId);
                ThingData* getItemData (uint16_t itemId);

                uint32_t getNIds ();

        private:
                ThingData** _things;
                uint32_t _nIds;
};

#endif

