#ifdef WIN32
        /* do windows stuff here */
#else
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
        _loaded = true;
#ifdef WIN32
        /* do windows stuff here */
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
        _load (pluginId, client);
        _client = client;
        _pluginId = pluginId;
        return true;
#endif
}
        
bool Plugin::unload ()
{
        if (!_loaded) {
                printf ("plugin unload error: plugin not loaded\n");
                return false;
        }
        _loaded = false;
        _unload ();

        /* now we do the dirty work if the plugin was to lazy */
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
                

#ifdef WIN32
        /* do windows stuff here */
#else
        if (dlclose (_handle) != 0) {
                printf ("plugin error: %s\n", dlerror());
                return false;
        }
        return true;
#endif
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

