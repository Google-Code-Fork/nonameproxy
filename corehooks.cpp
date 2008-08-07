#include <string>
#include "corehooks.h"
#include "tibiamessage.h"

TibiaMessage* HWCharacterList::func (TibiaMessage* tm, GameState* gs)
{
        LRMCharacterList* cl = (LRMCharacterList*)tm;
        TCharacterList* fixedCharList = new TCharacterList ();
        TCharacter*  newChar;
        CharList charlist = cl->getCharList ()->getCharList ();
        for (CharList::iterator i = charlist.begin ();
                i != charlist.end (); ++ i) {
                const std::string name = (*i)->getName ();
                const std::string world = (*i)->getWorld ();
                newChar = new TCharacter (name, world, 0x0100007F, 7331);
                fixedCharList->addChar (newChar);
        }

        LRMCharacterList* newcl
                = new LRMCharacterList (fixedCharList, cl->getDaysPrem ());
        //because we do not return the original TibiaMessage we must delete it
        delete cl;
        return newcl;
}

                
        

