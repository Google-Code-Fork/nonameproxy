#include <stdio.h>
#include "map.h"
#include "enums.h"

#define BUFFER_SIZE 256 

Map map;

Args MapRecipricant::func (const Args& args)
{
        Args::const_iterator i = args.begin ();
        if (args.size () == 1) {
                if (*i == "map") {
                        return map.usage ();
                } else {
                        printf ("info error: got wrong message\n");
                        return Args ();
                }
        }
        Args ret;
        i ++;
        if (args.size () > 1) {
                if (*i == "walk") {
                        map.local.walk ();
                } else if (*i == "set" && args.size () == 5) {
                        i ++;
                        std::string out = "target = (" + *i + ",";
                        uint32_t x = strtol ((*i).c_str (), NULL, 0);
                        i ++;
                        out += (*i + ",");
                        uint32_t y = strtol ((*i).c_str (), NULL, 0);
                        i ++;
                        out += (*i + ")");
                        uint32_t z = strtol ((*i).c_str (), NULL, 0);
                        map.local.set_target (x, y, z);
                        ret.push_back (out);
                }
        } else {
                return map.usage ();
        }
        return ret;
}

void StopHook::func (TibiaMessage* tm, Client* client)
{
        map.set_state (Map::s_pause);
}

Map::Map ()
{
        for (uint32_t i = 0; i < MAX_STOP_HOOK; i ++) {
                _stophooks[i] = 0;
        }
        _autodisable = false;
        _state = s_stop;
        _statecycle = 0;
}

Args Map::usage ()
{
        Args ret;
        ret.push_back ("map walk");
        return ret;
}

std::string r_set_state (const std::string& state)
{
        bool res;
        if (state == "pause") {
                res = map.set_state (Map::s_pause);
        } else if (state == "play") {
                res = map.set_state (Map::s_play);
        } else if (state == "stop") {
                res = map.set_state (Map::s_stop);
        } else {
                return "unreconized state " + state;
        }
        if (res) {
                return state;
        } else {
                return "failed: only one state change allowed per cycle";
        }
}
                
std::string r_get_state ()
{
        if (map.get_state () == Map::s_play) {
                return "play";
        } else if (map.get_state () == Map::s_stop) {
                return "stop";
        } else if (map.get_state () == Map::s_pause) {
                return "pause";
        } else {
                printf ("map error: funny state type\n");
                return "error";
        }
}

bool Map::set_state (state_t state)
{
        uint32_t cycle = _client->getCycle ();
        if (_statecycle == cycle) {
                return false;
        } else {
                _statecycle = cycle;
                _state = state;
                return true;
        }
}

Map::state_t Map::get_state ()
{
        return _state;
}

bool Map::get_autodisable ()
{
        return _autodisable;
}

bool Map::enable_autodisable ()
{
        if (_autodisable == true) {
                return false;
        }

        if (MAX_STOP_HOOK != 15) {
                /* if i add another hook, i want to be reminded */
                printf ("MAX_STOP_HOOK != 15\n");
                return false;
        }

        _stophooks[0] = _client->addSendReadHook (_pluginId, 
                                                 GSM_AUTO_WALK_ID,
                                                 new StopHook);
        _stophooks[1] = _client->addSendReadHook (_pluginId, 
                                                 GSM_AUTO_WALK_CANCEL_ID,
                                                 new StopHook);
        _stophooks[2] = _client->addSendReadHook (_pluginId, 
                                                 GSM_MOVE_NORTH_ID,
                                                 new StopHook);
        _stophooks[3] = _client->addSendReadHook (_pluginId, 
                                                 GSM_MOVE_EAST_ID,
                                                 new StopHook);
        _stophooks[4] = _client->addSendReadHook (_pluginId, 
                                                 GSM_MOVE_SOUTH_ID,
                                                 new StopHook);
        _stophooks[5] = _client->addSendReadHook (_pluginId, 
                                                 GSM_MOVE_WEST_ID,
                                                 new StopHook);
        _stophooks[6] = _client->addSendReadHook (_pluginId, 
                                                 GSM_MOVE_NE_ID,
                                                 new StopHook);
        _stophooks[7] = _client->addSendReadHook (_pluginId, 
                                                 GSM_MOVE_SE_ID,
                                                 new StopHook);
        _stophooks[8] = _client->addSendReadHook (_pluginId, 
                                                 GSM_MOVE_SW_ID,
                                                 new StopHook);
        _stophooks[9] = _client->addSendReadHook (_pluginId, 
                                                 GSM_MOVE_NW_ID,
                                                 new StopHook);
        _stophooks[10] = _client->addSendReadHook (_pluginId, 
                                                  GSM_TURN_NORTH_ID,
                                                  new StopHook);
        _stophooks[11] = _client->addSendReadHook (_pluginId, 
                                                  GSM_TURN_EAST_ID,
                                                  new StopHook);
        _stophooks[12] = _client->addSendReadHook (_pluginId, 
                                                  GSM_TURN_SOUTH_ID,
                                                  new StopHook);
        _stophooks[13] = _client->addSendReadHook (_pluginId, 
                                                  GSM_TURN_WEST_ID,
                                                  new StopHook);
        _stophooks[14] = _client->addSendReadHook (_pluginId, 
                                                  GSM_CANCEL_MOVE_ID,
                                                  new StopHook);

        _autodisable = true;
        return true;
}

bool Map::disable_autodisable ()
{
        if (_autodisable == false) {
                return false;
        }
        for (uint32_t i = 0; i < MAX_STOP_HOOK; i ++) {
                _client->deleteSendReadHook (_pluginId, _stophooks[i]);
                _stophooks[i] = 0;
        }
        _autodisable = false;
        return true;
}

void Map::i_load (uint32_t pluginId, Client* client)
{
        _name = "map";
        _pluginId = pluginId;
        _client = client;

        set_state (s_stop);
        enable_autodisable ();

        _rid = _client->addRecipricant (_pluginId, new MapRecipricant ());

        local.i_load (pluginId, client);
}

void Map::i_unload ()
{
        local.i_unload ();

        if (get_autodisable ()) {
                disable_autodisable ();
        }
        _client->deleteRecipricant (_pluginId, _rid);
}

const std::string& Map::i_name ()
{
        return _name;
}

void load (uint32_t id, Client* _client)
{
        map.i_load (id, _client);
}

void unload ()
{
        map.i_unload ();
}

const std::string& name ()
{
        return map.i_name ();
}

