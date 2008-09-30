#include <errno.h>
#include <stdio.h>

#include "datreader.h"
#include "safefile.h"

#define ID_OFFSET 100 

/***************************************************
 * ThingData
 ***************************************************/

ThingData::ThingData (SafeFile* datfile, uint16_t id)
{
        _id = id;
        _thingFlags = 0;
        _helpByte = 0;
        _speed = 0;
        _writeLimit = 0;
        _readLimit = 0;
        _lightColor = 0;
        _lightRadius = 0;
        _offsetX = 0;
        _offsetY = 0;
        _raised = 0;
        _miniMapColor = 0;

        //read the ops until we reach 0xFF
        uint8_t op = 0xFF;
        datfile->read (&op, 1);

        while (op != 0xFF) { 
                parseOp (datfile, op);
                datfile->read (&op, 1);
        }

        //skip over the sprites
        uint8_t width, height, blendframes, xdiv, ydiv, rare, animations;
        datfile->read (&width, 1);
        datfile->read (&height, 1);
        if (width > 1 || height > 1)
        {
            datfile->seek (1, SafeFile::s_from_current);
        }
        datfile->read (&blendframes, 1);
        datfile->read (&xdiv, 1);
        datfile->read (&ydiv, 1);
        datfile->read (&rare, 1);
        datfile->read (&animations, 1);
        
        uint32_t spritecount =  width * 
                                height * 
                                blendframes * 
                                xdiv * 
                                ydiv * 
                                rare * 
                                animations;

        datfile->seek (2 * spritecount, SafeFile::s_from_current);
}

void ThingData::show ()
{
        printf ("id = %d {\n", _id);

        if (isTopOrder1 ()) printf ("\tisTopOrder1\n");
        if (isTopOrder2 ()) printf ("\tisTopOrder2\n");
        if (isTopOrder3 ()) printf ("\tisTopOrder3\n");
        if (isContainer ()) printf ("\tisContainer\n");
        if (isStackable ()) printf ("\tisStackable\n");
        if (isCorpse ()) printf ("\tisCorpse\n");
        if (isUsable ()) printf ("\tisUsable\n");
        if (isRune ()) printf ("\tisRune\n");
        if (isWritable ()) printf ("\tisWritable\n");
        if (isReadable ()) printf ("\tisReadable\n");
        if (isFluidContainer ()) printf ("\tisFluidContainer\n");
        if (isSplash ()) printf ("\tisSplash\n");
        if (isBlocking ()) printf ("\tisBlocking\n");
        if (isImmobile ()) printf ("\tisImmobile\n");
        if (isMissileBlocking ()) printf ("\tisMissileBlocking\n");
        if (isPathBlocking ()) printf ("\tisPathBlocking\n");
        if (isPickupable ()) printf ("\tisPickupable\n");
        if (isHangable ()) printf ("\tisHangable\n");
        if (isHangableHorizontal ()) printf ("\tisHangableHorizontal\n");
        if (isHangableVertical ()) printf ("\tisHangableVertical\n");
        if (isRotatable ()) printf ("\tisRotatable\n");
        if (isLightSource ()) printf ("\tisLightSource\n");
        if (isFloorChange ()) printf ("\tisFloorChange\n");
        if (isOffset ()) printf ("\tisOffset\n");
        if (isRaised ()) printf ("\tisRaised\n");
        if (isIdleAnimation ()) printf ("\tisIdleAnimation\n");
        if (isMiniMap ()) printf ("\tisMiniMap\n");
        if (isGround ()) printf ("\tisGround\n");
        if (isLadder ()) printf ("\tisLadder\n");
        if (isSewer ()) printf ("\tisSewer\n");
        if (isRopeSpot ()) printf ("\tisRopeSpot\n");
        if (isSwitch ()) printf ("\tisSwitch\n");
        if (isDoor ()) printf ("\tisDoor\n");
        if (isDoorWithLock ()) printf ("\tisDoorWithLock\n");
        if (isStairs ()) printf ("\tisStairs\n");
        if (isMailBox ()) printf ("\tisMailBox\n");
        if (isDepot ()) printf ("\tisDepot\n");
        if (isTrash ()) printf ("\tisTrash\n");
        if (isHole ()) printf ("\tisHole\n");
        if (isSpecialDescription ()) printf ("\tisSpecialDescription\n");
        if (isReadOnly ()) printf ("\tisReadOnly\n");

        printf ("}\n");
}

bool ThingData::isTopOrder1 () {
        return _thingFlags & f_isTopOrder1;
}

bool ThingData::isTopOrder2 () {
        return _thingFlags & f_isTopOrder2;
}

bool ThingData::isTopOrder3 () {
        return _thingFlags & f_isTopOrder3;
}

bool ThingData::isContainer () {
        return _thingFlags & f_isContainer;
}

bool ThingData::isStackable () {
        return _thingFlags & f_isStackable;
}

bool ThingData::isCorpse () {
        return _thingFlags & f_isCorpse;
}

bool ThingData::isUsable () {
        return _thingFlags & f_isUsable;
}

bool ThingData::isRune () {
        return _thingFlags & f_isRune;
}

bool ThingData::isWritable () {
        return _thingFlags & f_isWritable;
}

bool ThingData::isReadable () {
        return _thingFlags & f_isReadable;
}

bool ThingData::isFluidContainer () {
        return _thingFlags & f_isFluidContainer;
}

bool ThingData::isSplash () {
        return _thingFlags & f_isSplash;
}

bool ThingData::isBlocking () {
        return _thingFlags & f_isBlocking;
}

bool ThingData::isImmobile () {
        return _thingFlags & f_isImmobile;
}

bool ThingData::isMissileBlocking () {
        return _thingFlags & f_isMissileBlocking;
}

bool ThingData::isPathBlocking () {
        return _thingFlags & f_isPathBlocking;
}

bool ThingData::isPickupable () {
        return _thingFlags & f_isPickupable;
}

bool ThingData::isHangable () {
        return _thingFlags & f_isHangable;
}

bool ThingData::isHangableHorizontal () {
        return _thingFlags & f_isHangableHorizontal;
}

bool ThingData::isHangableVertical () {
        return _thingFlags & f_isHangableVertical;
}

bool ThingData::isRotatable () {
        return _thingFlags & f_isRotatable;
}

bool ThingData::isLightSource () {
        return _thingFlags & f_isLightSource;
}

bool ThingData::isFloorChange () {
        return _thingFlags & f_isFloorChange;
}

bool ThingData::isOffset () {
        return _thingFlags & f_isOffset;
}

bool ThingData::isRaised () {
        return _thingFlags & f_isRaised;
}

bool ThingData::isIdleAnimation () {
        return _thingFlags & f_isIdleAnimation;
}

bool ThingData::isMiniMap () {
        return _thingFlags & f_isMiniMap;
}

bool ThingData::isGround () {
        return _thingFlags & f_isGround;
}

bool ThingData::isLadder () {
        return _helpByte == h_isLadder;
}

bool ThingData::isSewer () {
        return _helpByte == h_isSewer;
}

bool ThingData::isRopeSpot () {
        return _helpByte == h_isRopeSpot;
}

bool ThingData::isSwitch () {
        return _helpByte == h_isSwitch;
}

bool ThingData::isDoor () {
        return _helpByte == h_isDoor;
}

bool ThingData::isDoorWithLock () {
        return _helpByte == h_isDoorWithLock;
}

bool ThingData::isStairs () {
        return _helpByte == h_isStairs;
}

bool ThingData::isMailBox () {
        return _helpByte == h_isMailBox;
}

bool ThingData::isDepot () {
        return _helpByte == h_isDepot;
}

bool ThingData::isTrash () {
        return _helpByte == h_isTrash;
}

bool ThingData::isHole () {
        return _helpByte == h_isHole;
}

bool ThingData::isSpecialDescription () {
        return _helpByte == h_isSpecialDescription;
}

bool ThingData::isReadOnly () {
        return _helpByte == h_isReadOnly;
}

TWord16* ThingData::getSpeed ()
{
        return new TWord16 (_speed);
}

TWord16* ThingData::getWriteLimit ()
{
        return new TWord16 (_writeLimit);
}

TWord16* ThingData::getReadLimit ()
{
        return new TWord16 (_readLimit);
}

TItemLight* ThingData::getLight ()
{
        return new TItemLight (_lightColor, _lightRadius);
}

TWord16* ThingData::getOffsetX ()
{
        return new TWord16 (_offsetX);
}

TWord16* ThingData::getOffsetY ()
{
        return new TWord16 (_offsetY);
}

TWord16* ThingData::getRaised ()
{
        return new TWord16 (_raised);
}

TWord16* ThingData::getMiniMap ()
{
        return new TWord16 (_miniMapColor);
}

bool ThingData::isXItem ()
{
       return isStackable () || isRune () || isFluidContainer () || isSplash ();
}

void ThingData::parseOp (SafeFile* datfile, uint8_t op)
{
        switch(op)
        {
                case 0x00: //ground speed
                        datfile->read (&_speed, 2);
                        break;
                case 0x01: //toporder 1
                        _thingFlags |= f_isTopOrder1;
                        break;
                case 0x02: //toporder 2
                        _thingFlags |= f_isTopOrder2;
                        break;
                case 0x03: //toporder 3
                        _thingFlags |= f_isTopOrder3;
                        break;
                case 0x04: //isContainer
                        _thingFlags |= f_isContainer;
                        break;
                case 0x05: //isStackable
                        _thingFlags |= f_isStackable;
                        break;
                case 0x06: //isCorpse
                        _thingFlags |= f_isCorpse;
                        break;
                case 0x07: //isUseable
                        _thingFlags |= f_isUsable;
                        break;
                case 0x08: //isRune
                        _thingFlags |= f_isRune;
                        break;
                case 0x09: //isWriteable
                        _thingFlags |= f_isWritable;
                        datfile->read (&_writeLimit, 2);
                        break;
                case 0x0A: //isReadable
                        _thingFlags |= f_isReadable;
                        datfile->read (&_readLimit, 2);
                        break;
                case 0x0B: //isFluidContainer
                        _thingFlags |= f_isFluidContainer;
                        break;
                case 0x0C: //isSplash
                        _thingFlags |= f_isSplash;
                        break;
                case 0x0D: //isBlocking
                        _thingFlags |= f_isBlocking;
                        break;
                case 0x0E: //isImmobile
                        _thingFlags |= f_isImmobile;
                        break;
                case 0x0F: //isMissileBlocking
                        _thingFlags |= f_isMissileBlocking;
                        break;
                case 0x10: //isPathBlocking
                        _thingFlags |= f_isPathBlocking;
                        break;
                case 0x11: //isPickupable
                        _thingFlags |= f_isPickupable;
                        break;
                case 0x12: //isHangable
                        _thingFlags |= f_isHangable;
                        break;
                case 0x13: //isHangableHorizontal
                        _thingFlags |= f_isHangableHorizontal;
                        break;
                case 0x14: //isHangableVertical
                        _thingFlags |= f_isHangableVertical;
                        break;
                case 0x15: //isRotatable
                        _thingFlags |= f_isRotatable;
                        break;
                case 0x16: //isLightSource
                        _thingFlags |= f_isLightSource;
                        datfile->read (&_lightColor, 2);
                        datfile->read (&_lightRadius, 2);
                        break;
                case 0x18: //isFloorChange
                        _thingFlags |= f_isFloorChange;
                        break;
                case 0x19: //isOffset
                        _thingFlags |= f_isOffset;
                        datfile->read (&_offsetX, 2);
                        datfile->read (&_offsetY, 2);
                        break;
                case 0x1A: //isRaised
                        _thingFlags |= f_isRaised;
                        datfile->read (&_raised, 2);
                        break;
                case 0x1B: //isLayer
                        _thingFlags |= f_isLayer;
                        break;
                case 0x1C: //isIdleAnimation
                        _thingFlags |= f_isIdleAnimation;
                        break;
                case 0x1D: //isMinimap
                        _thingFlags |= f_isMiniMap;
                        datfile->read(&_miniMapColor, 2);
                        break;
                case 0x1E: //hasHelp
                        datfile->read(&_helpByte, 1);
                        break;
                case 0x1F:
                        _thingFlags |= f_isGround;
                        break;
        }
}

/***************************************************
 * ThingData
 ***************************************************/

DatReader::DatReader ()
{
        SafeFile* datfile = new SafeFile ("Tibia.dat", SafeFile::m_read_b);
        
        //seek past the dat sig
        datfile->seek (4, SafeFile::s_from_current);
        
        uint16_t nItems, nCreatures, nEffects, nMissile;
        datfile->read (&nItems, 2);
        datfile->read (&nCreatures, 2);
        datfile->read (&nEffects, 2);
        datfile->read (&nMissile, 2);

        _nIds = nItems + nCreatures + nEffects + nMissile - (ID_OFFSET - 1);

        
        _things = new ThingData*[_nIds];

        for (uint32_t i = 0; i < _nIds; i ++) {
                _things[i] = new ThingData (datfile, i + ID_OFFSET);
        }

        datfile->close ();
        delete datfile;
}

DatReader::~DatReader ()
{
        for (uint32_t i = 0; i < _nIds; i ++) {
                delete _things[i];
        }
        delete[] _things;
}

ThingData* DatReader::getItemData (TWord16* itemId)
{
        return (getItemData (itemId->getVal ()));
}
        
ThingData* DatReader::getItemData (uint16_t itemId)
{
        uint32_t index = itemId - ID_OFFSET;
        if (0 <= index && index < _nIds) {
                return _things[index];
        } else {
                printf ("error: item id out of range");
                return NULL;
        }
}

uint32_t DatReader::getNIds ()
{
        return _nIds;
}

