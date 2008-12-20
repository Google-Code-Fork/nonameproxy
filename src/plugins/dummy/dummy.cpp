#include <stdio.h>
#include "dummy.h"
#include "enums.h"

uint32_t _pluginId;
uint32_t _rid;
uint32_t _hookId;

Client* _client;
std::string _pluginName;

TibiaMessage* ChickenHook::func (TibiaMessage* tm, Client* client)
{
        GRMSpeak* speak = (GRMSpeak*)tm;

        if (speak->getSpeakType () != GRMSpeak::pub) {
                /* if the speak isnt a public one dont do anything */
                return tm;
        }

        /* look at the header files for the definitions in tibiamessage.h
         * we will make a new speak message to replace the original */
        GRMSpeak* squark = new GRMSpeak (speak->getU1 (),
                                         speak->getName (),
                                         speak->getLevel (),
                                         speak->getType (),
                                         speak->getPos (),
                                         "Squark");
                                         
        /* delete the old message and return a new one, this is how a 
         * write hook works. The new proxy will replace the old message
         * with the new one */
        delete tm;
        return squark;
}

/* String parsing is generally a messy subject. To make it easier
 * the arguement string has already been split up for you. Args is
 * a list of strings, which if you are familar with c is similar
 * to argv. */
Args ChickenRecipricant::func (const Args& args)
{
        /* we must also return a list of args. a good way to do this is
         * to declare some args at the beginning of the recipricant and
         * add to it as we go. */
        Args ret;
        Args::const_iterator i = args.begin ();
        if (*i != "dummy") {
                /* the first arguement should always be the plugin name
                 * so if it is != dummy print a debug message */
                printf ("dummy error: got wrong message\n");

                /* an empty arguement can be used to represent an error */
                return Args ();
        }
        if (args.size () < 2) {
                /* in this case we have too few arguements, so we return
                 * a usage message to the caller */
                ret.push_back ("dummy usage:");
                ret.push_back ("on");
                ret.push_back ("off");
                return ret;
        }
        /* move to the next arguement */
        i ++;
        if (*i == "on") {
                /* an id in general is 0 if an error has occured. in other
                 * words 0 implies the plugin is not loaded */
                if (_hookId == 0) {
                        /* to turn it on we simply add the hook
                         * when setting a hook the client needs to know 3 things
                         * 1. the pluginId to associate the hook with
                         * 2. the id of the message to hook (from enums.h)
                         * 3. a pointer to a hook */
                        _hookId = _client->addRecvWriteHook (_pluginId,
                                                             GRM_SPEAK_ID,
                                                             new ChickenHook);
                        ret.push_back ("chicken hook on");
                } else {
                        ret.push_back ("chicken hook already on");
                }
        } else if (*i == "off") {
                if (_hookId != 0) {
                        /* removing a hook is similar to setting the hook */
                        _client->deleteRecvWriteHook (_pluginId, _hookId);
                        ret.push_back ("chicken hook off");
                } else {
                        ret.push_back ("chicken hook not on");
                }
        } else {
                /* its better to put usage into its own function
                 * so we dont need to repeat ourselves */
                ret.push_back ("dummy usage:");
                ret.push_back ("on");
                ret.push_back ("off");
        }
        return ret;
}

/* load is called by the client at start up. It passes a pointer to the client
 * which is used for controlling the client. It also passes the plugin id,
 * which is needed when communicating with the client */
void load (uint32_t id, Client* client)
{
        _pluginId = id;
        _pluginName = "dummy";
        _client = client;

        /* the hook starts off unloaded */
        _hookId = 0;

        /* each plugin can have at most one recipricant. A recipricant
         * provides access to the plugin communication system, which is
         * arguably the most powerful feature of the proxy. In our case
         * we will use it for communicating between the console and the
         * dummy plugin */
        _rid = client->addRecipricant (_pluginId, new ChickenRecipricant);
}

/* unload is called by the client when unloading the plugin. here we should
 * delete all of the hooks and recipricant we have set. the plugin is
 * actually smart enough to clean up if you forget, but its much better to
 * just delete the hooks when you have the chance */
void unload ()
{
        /* if the hook is still set we must delete it */
        if (_hookId != 0) {
                _client->deleteRecvWriteHook (_pluginId, _hookId);
        }
        _client->deleteRecipricant (_pluginId, _rid);
}

/* each plugin must have a unique name, the core will not load two plugins
 * with the same name */
const std::string& name ()
{
        return _pluginName;
}

