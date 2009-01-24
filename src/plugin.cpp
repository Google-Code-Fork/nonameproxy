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

#ifndef WIN32
        #include <dlfcn.h>
#endif

#include "plugin.h"
#include "client.h"

Plugin::Plugin ()
{
        _recipricantId = 0;
        _loaded = false;
}

Plugin::~Plugin ()
{
        if (_loaded) {
                unload ();
        }
}

bool Plugin::load (uint32_t pluginId, const std::string& path, Client* client)
{
        if (_loaded) {
                printf ("plugin load error: plugin already loaded\n");
                return false;
        }
#ifdef WIN32
        if ((_handle = LoadLibrary (path.c_str ())) == NULL) {
                printf ("plugin error: could not open plugin: errno %d\n", 
                        GetLastError ());
                return false;
        }
        if ((_load = (loadfunc) GetProcAddress (_handle, "load")) == NULL) {
                printf ("plugin error: could not resolve symbol \"load\"\n");
                return false;
        }
        if ((_unload = (unloadfunc) GetProcAddress (_handle, "unload")) == NULL) {
                printf ("plugin error: could not resolve symbol \"unload\"\n");
                return false;
        }
        if ((_name = (namefunc) GetProcAddress (_handle, "name")) == NULL) {
                printf ("plugin error: could not resolve symbol \"name\"\n");
                return false;
        }
                
#else
        if ((_handle = dlopen (path.c_str (), RTLD_NOW)) == NULL) {
                
                printf ("plugin error: %s\n", dlerror());
                return false;
        }
        if ((_load = (loadfunc) dlsym (_handle, "load")) == NULL) {
                printf ("plugin error: could not resolve symbol \"load\"\n");
                return false;
        }
        if ((_unload = (unloadfunc) dlsym (_handle, "unload")) == NULL) {
                printf ("plugin error: could not resolve symbol \"unload\"\n");
                return false;
        }
        if ((_name = (namefunc) dlsym (_handle, "name")) == NULL) {
                printf ("plugin error: could not resolve symbol \"name\"\n");
                return false;
        }
#endif
        _loaded = true;
        _load (pluginId, client);
        _client = client;
        _pluginId = pluginId;
        return true;
}
        
bool Plugin::unload ()
{
        if (!_loaded) {
                printf ("plugin unload error: plugin not loaded\n");
                return false;
        }
        _loaded = false;
        printf ("yay1\n");
        _unload ();
        printf ("yay2\n");

        /* now we do the dirty work if the plugin was to lazy */
        if (_recipricantId != 0) {
                _client->deleteRecipricant (_pluginId, _recipricantId);
        }
        IdSet::iterator i;
        for (i = _rrhooks.begin (); i != _rrhooks.end (); ++ i) {
                _client->deleteRecvReadHook (_pluginId, (*i));
        }
        for (i = _rwhooks.begin (); i != _rwhooks.end (); ++ i) {
                _client->deleteRecvWriteHook (_pluginId, (*i));
        }
        for (i = _srhooks.begin (); i != _srhooks.end (); ++ i) {
                _client->deleteSendReadHook (_pluginId, (*i));
        }
        for (i = _swhooks.begin (); i != _swhooks.end (); ++ i) {
                _client->deleteSendWriteHook (_pluginId, (*i));
        }
        /* TODO add connection cleanum */
        /* TODO add packet hooks */
                

#ifdef WIN32
        printf ("yay\n");
        if (FreeLibrary (_handle) != 0) {
                printf ("plugin error: could not unload %s\n", 
                        name ().c_str ());
                return false;
        }
#else
        if (dlclose (_handle) != 0) {
                printf ("plugin error: %s\n", dlerror());
                return false;
        }
#endif
        return true;
}

const std::string& Plugin::name ()
{
        return _name ();
}

uint32_t Plugin::getRecipricantId ()
{
        return _recipricantId;
}

void Plugin::setRecipricantId (uint32_t rid)
{
        _recipricantId = rid;
}

void Plugin::addRecvReadHookId (uint32_t hid)
{
        std::pair <std::set<uint32_t>::iterator, bool> ret;
        ret = _rrhooks.insert (hid);
        if (!ret.second) {
                printf ("plugin error: addRecvReadHookId: ");
                printf ("hook id already exists\n");
        }
}

void Plugin::addRecvWriteHookId (uint32_t hid)
{
        std::pair <std::set<uint32_t>::iterator, bool> ret;
        ret = _rwhooks.insert (hid);
        if (!ret.second) {
                printf ("plugin error: addRecvWriteHookId: ");
                printf ("hook id already exists\n");
        }
}

void Plugin::addSendReadHookId (uint32_t hid)
{
        std::pair <std::set<uint32_t>::iterator, bool> ret;
        ret = _srhooks.insert (hid);
        if (!ret.second) {
                printf ("plugin error: addSendReadHookId: ");
                printf ("hook id already exists\n");
        }
}

void Plugin::addSendWriteHookId (uint32_t hid)
{
        std::pair <std::set<uint32_t>::iterator, bool> ret;
        ret = _swhooks.insert (hid);
        if (!ret.second) {
                printf ("plugin error: addSendWriteHookId: ");
                printf ("hook id already exists\n");
        }
}

void Plugin::deleteRecvReadHookId (uint32_t hid)
{
        if (_rrhooks.erase (hid) == 0) {
                printf ("plugin error: deleteRecvReadId: ");
                printf ("non existant hook id\n");
        }
}

void Plugin::deleteRecvWriteHookId (uint32_t hid)
{
        if (_rwhooks.erase (hid) == 0) {
                printf ("plugin error: deleteRecvWriteId: ");
                printf ("non existant hook id\n");
        }
}

void Plugin::deleteSendReadHookId (uint32_t hid)
{
        if (_srhooks.erase (hid) == 0) {
                printf ("plugin error: deleteSendReadId: ");
                printf ("non existant hook id\n");
        }
}

void Plugin::deleteSendWriteHookId (uint32_t hid)
{
        if (_swhooks.erase (hid) == 0) {
                printf ("plugin error: deleteSendWriteId: ");
                printf ("non existant hook id\n");
        }
}

void Plugin::addPreSendPacketHookId (uint32_t hid)
{
        std::pair <std::set<uint32_t>::iterator, bool> ret;
        ret = _preshooks.insert (hid);
        if (!ret.second) {
                printf ("plugin error: addPreSendPacketHookId: ");
                printf ("hook id already exists\n");
        }
}

void Plugin::addPostSendPacketHookId (uint32_t hid)
{
        std::pair <std::set<uint32_t>::iterator, bool> ret;
        ret = _postshooks.insert (hid);
        if (!ret.second) {
                printf ("plugin error: addPostSendPacketHookId: ");
                printf ("hook id already exists\n");
        }
}

void Plugin::addPreRecvPacketHookId (uint32_t hid)
{
        std::pair <std::set<uint32_t>::iterator, bool> ret;
        ret = _prerhooks.insert (hid);
        if (!ret.second) {
                printf ("plugin error: addPreRecvPacketHookId: ");
                printf ("hook id already exists\n");
        }
}

void Plugin::addPostRecvPacketHookId (uint32_t hid)
{
        std::pair <std::set<uint32_t>::iterator, bool> ret;
        ret = _postrhooks.insert (hid);
        if (!ret.second) {
                printf ("plugin error: addPostRecvPacketHookId: ");
                printf ("hook id already exists\n");
        }
}

void Plugin::deletePreSendPacketHookId (uint32_t hid)
{
        if (_preshooks.erase (hid) == 0) {
                printf ("plugin error: deletePreSendPacketHookId: ");
                printf ("non existant hook id\n");
        }
}

void Plugin::deletePostSendPacketHookId (uint32_t hid)
{
        if (_postshooks.erase (hid) == 0) {
                printf ("plugin error: deletePostSendPacketHookId: ");
                printf ("non existant hook id\n");
        }
}

void Plugin::deletePreRecvPacketHookId (uint32_t hid)
{
        if (_prerhooks.erase (hid) == 0) {
                printf ("plugin error: deletePreRecvPacketHookId: ");
                printf ("non existant hook id\n");
        }
}

void Plugin::deletePostRecvPacketHookId (uint32_t hid)
{
        if (_postrhooks.erase (hid) == 0) {
                printf ("plugin error: deletePostRecvPacketHookId: ");
                printf ("non existant hook id\n");
        }
}

bool Plugin::addConnectionId (uint32_t cid)
{
        std::pair <std::set<uint32_t>::iterator, bool> ret;
        ret = _connections.insert (cid);
        return ret.second;
}

bool Plugin::removeConnectionId (uint32_t hid)
{
        if (_connections.erase (hid) == 0) {
                return false;
        }
        return true;
}

/* protected fakein wrappers */
void _fakeload (uint32_t pluginId, Client* client) {};
void _fakeunload () {};

bool Plugin::fakeload (uint32_t pluginId, Client* client)
{
        if (_loaded) {
                printf ("plugin fakeload error: fakein already loaded\n");
                return false;
        }
        _loaded = true;

        _pluginId = pluginId;
        _client = client;

        _load = _fakeload;
        _unload = _fakeunload;
        return true;
}

/************************************************
 * Fakein
 ************************************************/
Fakein::Fakein (const std::string& pluginName)
{
        _name = pluginName;
}

bool Fakein::load (uint32_t pluginId, const std::string& path, Client* client)
{
        return Plugin::fakeload (pluginId, client);
}

const std::string& Fakein::name ()
{
        return _name;
}

