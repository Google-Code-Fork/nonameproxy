#include "pos.h"

Pos::Pos ()
{
        x = 0;
        y = 0;
        z = 0;
}

Pos::Pos (uint32_t _x, uint32_t _y, uint32_t _z)
{
        x = _x;
        y = _y;
        z = _z;
}

const bool Pos::operator== (const Pos& a) const
{
        if (x == a.x && a.y == y) {
                return true;
        } else {
                return false;
        }
}

const bool Pos::operator!= (const Pos& a) const
{
        return ! operator== (a);
}

const bool Pos::operator< (const Pos& a) const
{
        Pos tmp = *this;
        return POS_2_INDEX (tmp) < POS_2_INDEX (a);
}

