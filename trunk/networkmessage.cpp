#include <string.h>
#include <stdio.h>
#include "networkmessage.h"

NetworkMessage::NetworkMessage (uint32_t size)
{
        _buffer = new uint8_t[size];
        _curpos = 0;
        _size = size;
}

NetworkMessage::NetworkMessage (uint32_t size, uint8_t* buffer)
{
        _buffer = buffer;
        _curpos = 0;
        _size = size;
}

NetworkMessage::~NetworkMessage ()
{
        delete[] _buffer;
}

uint8_t* NetworkMessage::getBuffer () {
        return _buffer;
}

bool NetworkMessage::getU8 (uint8_t& val)
{
        if (_curpos + 1 > _size) {
                return false;
        }
        memcpy (&_buffer, &val, 1);
        _curpos += 1;
        return true;
}

bool NetworkMessage::getU16 (uint16_t& val)
{
        if (_curpos + 2 > _size) {
                return false;
        }
        memcpy (&_buffer[_curpos], &val, 2);
        _curpos += 2;
        return true;
}

bool NetworkMessage::getU32 (uint32_t& val)
{
        if (_curpos + 4 > _size) {
                return false;
        }
        memcpy (&_buffer[_curpos], &val, 4);
        _curpos += 4;
        return true;
}

bool NetworkMessage::getN (uint8_t* dest, uint32_t n)
{
        if (_curpos + n > _size) {
                return false;
        }
        memcpy (dest, &_buffer[_curpos], n);
        _curpos += n;
        return true;
}

bool NetworkMessage::putU8 (uint8_t val)
{
        if (_curpos + 1 > _size) {
                return false;
        }
        memcpy (&val, &_buffer[_curpos], 1);
        _curpos += 1;
        return true;
}
        
bool NetworkMessage::putU16 (uint16_t val)
{
        if (_curpos + 2 > _size) {
                return false;
        }
        memcpy (&val, &_buffer[_curpos], 2);
        _curpos += 2;
        return true;
}
        
bool NetworkMessage::putU32 (uint32_t val)
{
        if (_curpos + 4 > _size) {
                return false;
        }
        memcpy (&val, &_buffer[_curpos], 4);
        _curpos += 4;
        return true;
}

bool NetworkMessage::putN (uint8_t* src, uint32_t n)
{
        if (_curpos + n > _size) {
                return false;
        }
        memcpy (src, &_buffer[_curpos], n);
        _curpos += n;
        return true;
}

void NetworkMessage::show () {
        for (uint32_t i = 0; i <= _size / 16; i ++) {
                for (uint32_t ii = 0; ii < 16; ii++) {
                        uint32_t p = i * 16 + ii;
                        if (p < _size) {
                                printf ("%02X ", _buffer[p]);
                        } else {
                                printf ("   ");
                        }
                }
                printf ("        ");
                for (uint32_t ii = 0; ii < 16; ii++) {
                        uint32_t p = i * 16 + ii;
                        if (p < _size) {
                                if (' ' <= _buffer[p] && _buffer[p] <= '~') {
                                        printf ("%c", _buffer[i * 16 + ii]);
                                } else {
                                        printf (".");
                                }
                        } else {
                                printf (" ");
                        }
                }
                printf ("\n");
        }
}

