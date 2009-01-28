#include <stdio.h>
#include "action.h"

Action action;

int32_t ActionRecipricant::func (const Args &args, Args &out)
{
        Args::const_iterator i = args.begin ();
        if (args.size () == 0) {
                printf ("action error: got empty message\n");
                return PLUGIN_FAILURE;
        }
        if (args.size () == 1) {
                if (*i == "action") {
                        return action.usage (out);
                } else {
                        printf ("action error: got wrong message\n");
                        return PLUGIN_FAILURE;
                }
        }
        i ++;

        if (*i == "say" && args.size () > 2) {
                i ++;
                action.send_say (*i);
                return PLUGIN_SUCCESS;
        }
        return action.usage (out);
}

int32_t Action::usage (Args &out)
{
        out.push_back ("action say 'string'");
        return PLUGIN_FAILURE;
}

void Action::send_say (const std::string &msg)
{
        GSMSpeak *speak = new GSMSpeak(1, msg);

        GSMessageList gsml;
        gsml.add (speak);

        _client->sendToServer (gsml);
}
        
void Action::i_load (uint32_t pluginId, Client *client)
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

const std::string &Action::i_name ()
{
        return _name;
}

void load (uint32_t id, Client *_client)
{
        action.i_load (id, _client);
}

void unload ()
{
        action.i_unload ();
}

const std::string &name ()
{
        return action.i_name ();
}

