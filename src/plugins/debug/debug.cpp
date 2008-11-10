#include <stdio.h>
#include "debug.h"
#include "enums.h"

Debug debug;

void ShowMessage::func (TibiaMessage* tm, Client* _client)
{
        tm->show ();
}

void ShowPacket::func (const NetworkMessage& msg)
{
        msg.show ();
}

Args DebugRecipricant::func (const Args& args)
{
        Args ret;
        Args::const_iterator i = args.begin ();
        /* always four arguements, this makes things nice */
        if (args.size () >= 2) {
                if ((*i) != "debug") {
                        printf ("debug got wrong message\n");
                        return Args ();
                }
                i ++;
                if ((*i) == "disp" && args.size () == 4) {
                        i ++;
                        if ((*i) == "send") {
                                i ++;
                                if ((*i) == "pre") {
                                        ret.push_back (debug.pre_send_on ());
                                } else if ((*i) == "post") {
                                        ret.push_back (debug.post_send_on ());
                                } else {
                                        ret.push_back (debug.send_on (*i));
                                }
                        } else if ((*i) == "recv") {
                                i ++;
                                if ((*i) == "pre") {
                                        ret.push_back (debug.pre_recv_on ());
                                } else if ((*i) == "post") {
                                        ret.push_back (debug.post_recv_on ());
                                } else {
                                        ret.push_back (debug.recv_on (*i));
                                }
                        }
                } else if ((*i) == "undisp" && args.size () == 4) {
                        i ++;
                        if ((*i) == "send") {
                                i ++;
                                if ((*i) == "pre") {
                                        ret.push_back (debug.pre_send_off ());
                                } else if ((*i) == "post") {
                                        ret.push_back (debug.post_send_off ());
                                } else {
                                        ret.push_back (debug.send_off (*i));
                                }
                        } else if ((*i) == "recv") {
                                i ++;
                                if ((*i) == "pre") {
                                        ret.push_back (debug.pre_recv_off ());
                                } else if ((*i) == "post") {
                                        ret.push_back (debug.post_recv_off ());
                                } else {
                                        ret.push_back (debug.recv_off (*i));
                                }
                        }
                } else if ((*i) == "show" && args.size () >= 3) {
                        i ++;
                        if ((*i) == "tile" && args.size () == 6) {
                                i ++;
                                uint32_t x = strtol ((*i).c_str (), NULL, 0);
                                i ++;
                                uint32_t y = strtol ((*i).c_str (), NULL, 0);
                                i ++;
                                uint32_t z = strtol ((*i).c_str (), NULL, 0);
                                ret.push_back (debug.show_tile (x, y, z));
                        } else if ((*i) == "battlelist" && args.size () == 3) {
                                ret.push_back (debug.show_battlelist ());
                        } else if (((*i) == "inv" || (*i) == "inventory")
                                        && args.size () == 3) {
                                ret.push_back (debug.show_inventory ());
                        } else if (((*i) == "cont" || (*i) == "container")
                                        && args.size () == 4)
                        {
                                i ++;
                                ret.push_back (debug.show_container (*i));
                        }
                }
        } else {
                return debug.usage ();
        }
        return ret;
}

Args Debug::usage ()
{
        Args ret;
        ret.push_back ("debug: usage");
        ret.push_back ("debug disp [send|recv] [pre|post|messageid]");
        ret.push_back ("debug undisp [send|recv] [pre|post|messageid]");
        ret.push_back ("debug show tile x y z");
        ret.push_back ("debug show battlelist");
        return ret;
}

Debug::Debug ()
{
        /* initialize all hook ids to 0 */
        for (uint32_t i = 0; i < MAX_HOOKS; i ++) {
                _sendHooks[i] = 0;
                _recvHooks[i] = 0;
        }
        _sendpre = 0;
        _sendpost = 0;
        _recvpre = 0;
        _recvpost = 0;
}

std::string Debug::pre_send_on ()
{
        if (_sendpre == 0) {
                _sendpre = _client->addPreSendPacketHook (_pluginId, new ShowPacket ());
                if (_sendpre == 0) {
                        return "debug: disp send pre failed";
                } else {
                        return "debug: success!";
                }
        } else {
                return "debug: disp send pre is already on";
        }
}
                
std::string Debug::post_send_on ()
{
        if (_sendpost == 0) {
                _sendpost = _client->addPreSendPacketHook (_pluginId, new ShowPacket ());
                if (_sendpost == 0) {
                        return "debug: disp send post failed";
                } else {
                        return "debug: success!";
                }
        } else {
                return "debug: disp send post is already on";
        }
}
                
std::string Debug::send_on (const std::string& mid)
{
        uint32_t m = strtol (mid.c_str (), NULL, 0);
        if (!(0 <= m && m < MAX_HOOKS)) {
                return "debug: error message id out of range";
        }
        if (_sendHooks[m] == 0) {
                _sendHooks[m] = _client->addSendReadHook (_pluginId, m,
                                        new ShowMessage ());
                if (_sendHooks[m] == 0) {
                        return "debug: disp send failed";
                } else {
                        return "debug: success!";
                }
        } else {
                return "debug: disp " + mid + " is already on";
        }
}

std::string Debug::pre_recv_on ()
{
        if (_recvpre == 0) {
                _recvpre = _client->addPreRecvPacketHook (_pluginId, new ShowPacket ());
                if (_recvpre == 0) {
                        return "debug: disp recv pre failed";
                } else {
                        return "debug: success!";
                }
        } else {
                return "debug: disp recv pre is already on";
        }
}
                
std::string Debug::post_recv_on ()
{
        if (_recvpost == 0) {
                _recvpost = _client->addPreRecvPacketHook (_pluginId, new ShowPacket ());
                if (_recvpost == 0) {
                        return "debug: disp recv post failed";
                } else {
                        return "debug: success!";
                }
        } else {
                return "debug: disp recv post is already on";
        }
}
                
std::string Debug::recv_on (const std::string& mid)
{
        uint32_t m = strtol (mid.c_str (), NULL, 0);
        if (!(0 <= m && m < MAX_HOOKS)) {
                return "debug: error message id out of range";
        }
        if (_recvHooks[m] == 0) {
                _recvHooks[m] = _client->addRecvReadHook (_pluginId, m,
                                        new ShowMessage ());
                if (_recvHooks[m] == 0) {
                        return "debug: disp recv failed";
                } else {
                        return "debug: success!";
                }
        } else {
                return "debug: disp " + mid + " is already on";
        }
}

std::string Debug::pre_send_off ()
{
        if (_sendpre != 0) {
                _client->deletePreSendPacketHook (_pluginId, _sendpre);
                _sendpre = 0;
                return "debug: success!";
        } else {
                return "debug: disp send pre is not on";
        }
}
                
std::string Debug::post_send_off ()
{
        if (_sendpost != 0) {
                _client->deletePostSendPacketHook (_pluginId, _sendpost);
                _sendpost = 0;
                return "debug: success!";
        } else {
                return "debug: disp send post is not on";
        }
}
                
std::string Debug::send_off (const std::string& mid)
{
        uint32_t m = strtol (mid.c_str (), NULL, 0);
        if (!(0 <= m && m < MAX_HOOKS)) {
                return "debug: error message id out of range";
        }
        if (_sendHooks[m] != 0) {
                _client->deleteSendReadHook (_pluginId, _recvHooks[m]);
                _sendHooks[m] = 0;
                return "debug: success!";
        } else {
                return "debug: disp " + mid + " is not on";
        }
}
                
std::string Debug::pre_recv_off ()
{
        if (_recvpre != 0) {
                _client->deletePreRecvPacketHook (_pluginId, _recvpre);
                _recvpre = 0;
                return "debug: success!";
        } else {
                return "debug: disp recv pre is not on";
        }
}
                
std::string Debug::post_recv_off ()
{
        if (_recvpost != 0) {
                _client->deletePostRecvPacketHook (_pluginId, _recvpost);
                _recvpost = 0;
                return "debug: success!";
        } else {
                return "debug: disp recv post is not on";
        }
}
                
std::string Debug::recv_off (const std::string& mid)
{
        uint32_t m = strtol (mid.c_str (), NULL, 0);
        if (!(0 <= m && m < MAX_HOOKS)) {
                return "debug: error message id out of range";
        }
        if (_recvHooks[m] != 0) {
                _client->deleteRecvReadHook (_pluginId, _recvHooks[m]);
                _recvHooks[m] = 0;
                return "debug: success!";
        } else {
                return "debug: disp " + mid + " is not on";
        }
}

std::string Debug::show_tile (uint32_t x, uint32_t y, uint32_t z)
{
        Pos& p = _client->gstate->map->getCurPos ();
        Tile& t = _client->gstate->map->getTile (x + p.x, y + p.y, z + p.z);
        t.show ();

        std::string ret = "debug: showing tile ";
        char buffer [100];
        snprintf (buffer, 100, "%d %d %d", x + p.x, y + p.y, z + p.z);
        ret += buffer;
        return ret;
}
                
std::string Debug::show_battlelist ()
{
        _client->gstate->map->showCreatures ();

        return "debug: showing battlelist";
}

std::string Debug::show_container (const std::string& cid)
{
        uint32_t _cid = strtol (cid.c_str (), NULL, 0);
        _client->gstate->inventory->getContainer (_cid).show ();

        return "debug: showing container " + cid;
}
                
std::string Debug::show_inventory ()
{
        _client->gstate->inventory->showSlots ();
        return "debug: showing slots";
}
                
void Debug::iload (uint32_t pluginId, Client* client)
{
        _name = "debug";
        _pluginId = pluginId;
        _client = client;
        _rid = _client->addRecipricant (_pluginId, new DebugRecipricant ());
}

void Debug::iunload ()
{
        for (uint32_t i = 0; i != MAX_HOOKS; i ++) {
                if (_sendHooks[i] != 0) {
                        _client->deleteSendReadHook (_pluginId, _sendHooks[i]);
                }
                if (_recvHooks[i] != 0) {
                        _client->deleteRecvReadHook (_pluginId, _recvHooks[i]);
                }
        }
        if (_sendpre != 0) {
                _client->deletePreSendPacketHook (_pluginId, _sendpre);
        }
        if (_sendpost != 0) {
                _client->deletePostSendPacketHook (_pluginId, _sendpost);
        }
        if (_recvpre != 0) {
                _client->deletePreRecvPacketHook (_pluginId, _recvpre);
        }
        if (_recvpost != 0) {
                _client->deletePostRecvPacketHook (_pluginId, _recvpost);
        }
        _client->deleteRecipricant (_pluginId, _rid);
}

const std::string& Debug::iname ()
{
        return _name;
}

void load (uint32_t id, Client* _client)
{
        debug.iload (id, _client);
}

void unload ()
{
        debug.iunload ();
}

const std::string& name ()
{
        return debug.iname ();
}

