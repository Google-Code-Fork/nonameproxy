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

#ifndef __DAT_READER_H
#define __DAT_READER_H

#define ORDER_GROUND 0
#define ORDER_CREATURE 4

#include "tibiatypes.h"

class SafeFile;

class ThingData
{
        public:
                /* a ThingData will read itself from the dat file */
                ThingData (SafeFile* datfile, uint16_t id);
                void show () const;

                bool isTopOrder1 () const;
                bool isTopOrder2 () const;
                bool isTopOrder3 () const;
                bool isContainer () const;
                bool isStackable () const;
                bool isCorpse () const;
                bool isUsable () const;
                bool isRune () const;
                bool isWritable () const;
                bool isReadable () const;
                bool isFluidContainer () const;
                bool isSplash () const;
                bool isBlocking () const;
                bool isImmobile () const;
                bool isMissileBlocking () const;
                bool isPathBlocking () const;
                bool isPickupable () const;
                bool isHangable () const;
                bool isHangableHorizontal () const;
                bool isHangableVertical () const;
                bool isRotatable () const;
                bool isLightSource () const;
                bool isFloorChange () const;
                bool isOffset () const;
                bool isRaised () const;
                bool isIdleAnimation () const;
                bool isMiniMap () const;
                bool isGround () const;
                bool isLadder () const;
                bool isSewer () const;
                bool isRopeSpot () const;
                bool isSwitch () const;
                bool isDoor () const;
                bool isDoorWithLock () const;
                bool isStairs () const;
                bool isMailBox () const;
                bool isDepot () const;
                bool isTrash () const;
                bool isHole () const;
                bool isSpecialDescription () const;
                bool isReadOnly () const;
                /* these functions actually return something */
                uint32_t getSpeed () const;
                uint32_t getWriteLimit () const;
                uint32_t getReadLimit () const;
                uint32_t getLightColor () const;
                uint32_t getLightRadius () const;
                uint32_t getOffsetX () const;
                uint32_t getOffsetY () const;
                uint32_t getRaised () const;
                uint32_t getMiniMap () const;

                /* determines if an item has an extra byte */
                bool isXItem () const;

                /* determines an items order */
                uint32_t getOrder () const;

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

                uint32_t _speed;
                uint32_t _writeLimit;
                uint32_t _readLimit;
                uint32_t _lightColor;
                uint32_t _lightRadius;
                uint32_t _offsetX;
                uint32_t _offsetY;
                uint32_t _raised;
                uint32_t _miniMapColor;
                uint32_t _id;
                /* i dont keep any of the sprite data */
};
        
class DatReader
{
        public:
                DatReader ();
                virtual ~DatReader ();
                const ThingData& getItemData (uint16_t itemId) const;

                uint32_t getNIds () const;

        private:
                ThingData** _things;
                uint32_t _nIds;
};

#endif

