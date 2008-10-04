#include <stdio.h>
#include "pluginmanager.h"

PluginManager::PluginManager ()
{
        ids = new IdManager (100);
}

PluginManager::~PluginManager ()
{
        delete ids;
}

uint32_t PluginManager::addPlugin (Plugin* plugin)
{
        uint32_t id = ids->newId ();

        plist.insert (std::pair<uint32_t, Plugin> (id, *plugin));
        return id;
}

void PluginManager::deletePlugin (uint32_t pid)
{
        if (plist.erase (pid) == 0) {
                printf ("plugin delete error: plugin doesnt exist\n");
                return;
        }
        if (!ids->recycleId (pid)) {
                printf ("plugin delete error: ids not concurrent\n");
        }
}

uint32_t PluginManager::getPluginByName (const std::string& name)
{
        for (PluginList::iterator i; i != plist.end (); ++ i) {
                if (name == (*i).second.name ()) {
                        return (*i).first;
                }
        }
        return 0;
}

/************************************************************************
 * These functions provide the medium for the plugins to operate through
 * Note: all these functions work in terms of plugin id, the one
 *       notable function is send message which hides the recipricant
 *       id from the outside
 ************************************************************************/

//uint32_t PluginManager::sendMessage (uint32_t pid, const std::string& msg)
//{
        
