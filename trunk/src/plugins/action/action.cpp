#include <stdio.h>
#include "action.h"

Action action;

Args ActionRecipricant::func (const Args& args)
{
        Args::const_iterator i = args.begin ();
        if (args.size () == 0) {
                printf ("action error: got empty message\n");
                return Args ();
        }
        if (args.size () == 1) {
                if (*i == "action") {
                        return action.usage ();
                } else {
                        printf ("action error: got wrong message\n");
                        return Args ();
                }
        }
        i ++;

        if (*i == "say" && args.size () > 2) {
                i ++;
                action.send_say (*i);
                return Args ();
        }
        return action.usage ();
}

Args Action::usage ()
{
        Args ret;
        ret.push_back ("action say 'string'");
        return ret;
}

void Action::send_say (const std::string& msg)
{
        GSMSpeak* speak = new GSMSpeak(1, msg);

        GSMessageList gsml;
        gsml.add (speak);

        _client->sendToServer (gsml);
}
        
void Action::i_load (uint32_t pluginId, Client* client)
{
        _name = "action";
        _pluginId = pluginId;
        _client = client;

        _rid = _client->addRecipricant (_pluginId, new ActionRecipricant ());
}

void Action::i_unload ()
{
        _client->deleteRecipricant (_pluginId, _rid);
}

const std::string& Action::i_name ()
{
        return _name;
}

void load (uint32_t id, Client* _client)
{
        action.i_load (id, _client);
}

void unload ()
{
        action.i_unload ();
}

const std::string& name ()
{
        return action.i_name ();
}

