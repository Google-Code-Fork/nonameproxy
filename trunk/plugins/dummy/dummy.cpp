#include <stdio.h>
#include "dummy.h"
#include "enums.h"

uint32_t pluginId;
uint32_t rid;
uint32_t chickenHookId;

Client* clientPtr;
std::string pluginName;

TibiaMessage* ChickenHook::func (TibiaMessage* tm, Client* client)
{
        GRMSpeak* speak = (GRMSpeak*)tm;

        const TSpeak& orig = speak->getSpeak ();
        if (orig.getSpeakType () != TSpeak::pub) {
                /* if the speak isnt a public one dont do anything */
                return tm;
        }

        /* because speak has a derived class structure this will be a bit
         * more complicated than usual */

        const TPublicSpeak& oldPublic = (TPublicSpeak&)orig;
        /* look at the header files for the definitions */
        TPublicSpeak squark (oldPublic.getU1 (),
                             oldPublic.getName (),
                             oldPublic.getLevel (),
                             oldPublic.getType (),
                             oldPublic.getPos (),
                             "Squark");
        
        /* delete the old message and return a new one, this is how a 
         * write hook works. The new proxy will replace the old message
         * with the new one */
        delete tm;
        GRMSpeak* newSpeak = new GRMSpeak (squark);
        return newSpeak;
}

void TestRecipricant::func (const Args& args)
{
        Args::const_iterator i;
        for (i = args.begin (); i != args.end (); ++ i) {
                printf ("%s\n", (*i).c_str ());
        }
}

void load (uint32_t id, Client* client)
{
        pluginId = id;
        printf ("loading id = %d\n", pluginId);
        pluginName = "dummy";
        clientPtr = client;

        /* new we have to load the plugin
         * The pluginId is required for resource management.
         * Something which i think is pretty cool is that when a plugin
         * is unloaded it will automatically free any resources
         * ie. hooks, connections, and the messaging hooks, even if the
         * plugin doesnt do it by itself, which its meant to */

        chickenHookId = clientPtr->addRecvWriteHook (pluginId, GRM_SPEAK_ID,
                                                        new ChickenHook);
        rid = clientPtr->addRecipricant (pluginId, new TestRecipricant);
}

void unload ()
{
        printf ("unloading\n");
        clientPtr->deleteRecvWriteHook (pluginId, chickenHookId);
        clientPtr->deleteRecipricant (pluginId, rid);
}

const std::string& name ()
{
        return pluginName;
}

