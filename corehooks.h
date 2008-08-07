#ifndef __CORE_HOOKS_H
#define __CORE_HOOKS_H

#include <stdint.h>
#include "hook.h"

//hook naming should be HW for a write hook and HR for a read hook,
//followed by the message name

class HWCharacterList : public WriteHook
{
        public:
                virtual TibiaMessage* func (TibiaMessage* tm, GameState* gs);
};

#endif


