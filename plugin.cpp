#ifdef WIN32
        /* do windows stuff here */
#else
        #include <dlfcn.h>
#endif

#include "plugin.h"

Plugin::Plugin ()
{
        _recipricantId = 0;
}

Plugin::~Plugin ()
{
        unload ();
}

bool Plugin::load (uint32_t pluginId, const std::string& path, Client* client)
{
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
        printf ("yay\n");
        _load (pluginId, client);
        _client = client;
        return true;
#endif
}
        
bool Plugin::unload ()
{
        _unload ();

        if (dlclose (_handle) != 0) {
                printf ("plugin error: %s\n", dlerror());
                return false;
        }
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

bool Plugin::addHookId (uint32_t hid)
{
        std::pair <std::set<uint32_t>::iterator, bool> ret;
        ret = _hooks.insert (hid);
        return ret.second;
}

bool Plugin::removeHookId (uint32_t hid)
{
        if (_hooks.erase (hid) == 0) {
                return false;
        }
        return true;
}

bool Plugin::addConnectionId (uint32_t cid)
{
        std::pair <std::set<uint32_t>::iterator, bool> ret;
        ret = _connections.insert (cid);
        return ret.second;
}

bool Plugin::removeConnectionId (uint32_t hid)
{
        if (_hooks.erase (hid) == 0) {
                return false;
        }
        return true;
}

