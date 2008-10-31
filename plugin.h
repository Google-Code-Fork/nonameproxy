/*****************************************************************************
 * noname proxy
 *****************************************************************************
 *
 *****************************************************************************
 * This program is free software; you can redistribute it and*or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *****************************************************************************/

#ifndef __PLUGIN_H
#define __PLUGIN_H

#include <stdint.h>
#include <string>
#include <set>

class Client;

typedef const std::string& (*namefunc) ();
typedef void (*loadfunc) (uint32_t, Client*);
typedef void (*unloadfunc) ();

typedef std::set <uint32_t> IdSet;

class Plugin
{
        public:
                Plugin ();
                virtual ~Plugin ();

                virtual bool load (uint32_t pluginId, const std::string& path, 
                                Client* client);

                virtual bool unload ();
                virtual const std::string& name ();

                uint32_t getRecipricantId ();
                void     setRecipricantId (uint32_t rid);

                void     addRecvReadHookId (uint32_t hid);
                void     addRecvWriteHookId (uint32_t hid);
                void     addSendReadHookId (uint32_t hid);
                void     addSendWriteHookId (uint32_t hid);

                void     deleteRecvReadHookId (uint32_t hid);
                void     deleteRecvWriteHookId (uint32_t hid);
                void     deleteSendReadHookId (uint32_t hid);
                void     deleteSendWriteHookId (uint32_t hid);

                void     addPreSendPacketHookId (uint32_t hid);
                void     addPostSendPacketHookId (uint32_t hid);

                void     addPreRecvPacketHookId (uint32_t hid);
                void     addPostRecvPacketHookId (uint32_t hid);

                void     deletePreSendPacketHookId (uint32_t hid);
                void     deletePostSendPacketHookId (uint32_t hid);

                void     deletePreRecvPacketHookId (uint32_t hid);
                void     deletePostRecvPacketHookId (uint32_t hid);

                bool     addConnectionId (uint32_t cid);
                bool     removeConnectionId (uint32_t cid);
        
        protected:
                bool fakeload (uint32_t pluginId, Client* client);
                bool fakeunload ();
                bool _loaded;

        private:
#ifdef WIN32
#else
                void* _handle;
#endif
                /* we keep the client for cleanup purposes */
                Client* _client;

                namefunc   _name;
                loadfunc   _load;
                unloadfunc _unload;

                uint32_t _pluginId;
                uint32_t _recipricantId;
                IdSet _rrhooks;
                IdSet _rwhooks;
                IdSet _srhooks;
                IdSet _swhooks;
                IdSet _preshooks;
                IdSet _postshooks;
                IdSet _prerhooks;
                IdSet _postrhooks;
                IdSet _connections;
};

/* the core has a fakein which allows it to act like a plugin */
class Fakein : public Plugin
{
        public:
                Fakein (const std::string& pluginName);
                virtual bool load (uint32_t pluginId, const std::string& path, 
                                Client* client);

                virtual const std::string& name ();

        private:
                std::string _name;
};
        
#endif

