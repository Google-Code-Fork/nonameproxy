#include <errno.h>
#include <stdio.h>

#include "datreader.h"
#include "safefile.h"

/***************************************************
 * ThingData
 ***************************************************/

ThingData::ThingData (SafeFile* datfile)
{
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

        uint32_t _nIds = nItems + nCreatures + nEffects + nMissile;
        
        _things = new ThingData*[_nIds];

        for (uint32_t i = 0; i < _nIds; i ++) {
                _things[i] = new ThingData (datfile);
        }

        datfile->close ();
}

DatReader::~DatReader ()
{
        for (uint32_t i = 0; i < _nIds; i ++) {
                delete _things[_nIds + 1];
        }
        delete[] _things;
}

