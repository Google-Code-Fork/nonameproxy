#include "mapstate.h"

Pos::Pos ()
{
        x = 0;
        y = 0;
        z = 0;
}

Pos::Pos (uint32_t xx, uint32_t yy, uint32_t zz)
{
        x = xx;
        y = yy;
        z = zz;
}

Pos MapState::getCurPos ()
{
        return _curPos;
}

void MapState::setCurPos (const Pos& pos)
{
        _curPos = pos;
}

