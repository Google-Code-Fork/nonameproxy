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

#include "corerecipricant.h"
#include "client.h"

CoreRecipricant::CoreRecipricant (Client *client)
{
        _client = client;
}

int32_t CoreRecipricant::func (const Args &args, Args &out)
{
        Args::const_iterator i = args.begin ();
        uint32_t argc = args.size ();
        if (argc > 2) {
                i ++;
                if (*i == "load") {
                        return loadPlugin (args, out, i, argc);
                } else if (*i == "unload") {
                        return unloadPlugin (args, out, i, argc);
                }
        }
        return usage (out);
}

int32_t CoreRecipricant::loadPlugin (const Args &args, Args &out,
                                     Args::const_iterator i, uint32_t argc)
{
        if (argc < 3) {
                out.push_back ("core: load requires a path name");
                return PLUGIN_FAILURE;
        }
        i ++;
        if (_client->pluginManager->addPlugin (*i) == 0) {
                out.push_back ("core: could not load " + *i);
                return PLUGIN_FAILURE;
        }
        out.push_back ("core: plugin " + *i + " successfully loaded");
        return PLUGIN_SUCCESS;
}
                
int32_t CoreRecipricant::unloadPlugin (const Args &args, Args &out,
                                       Args::const_iterator i, uint32_t argc)
{
        if (argc < 3) {
                out.push_back ("core: unload requires a plugin name");
                return PLUGIN_FAILURE;
        }
        i ++;
        uint32_t pid = _client->getPluginByName (*i);
        if (pid == 0) {
                out.push_back ("core: could not find plugin " + *i);
                return PLUGIN_FAILURE;
        }
        if (!_client->pluginManager->deletePlugin (pid)) {
                out.push_back ("core: could not unload plugin " + *i);
                return PLUGIN_FAILURE;
        }
        out.push_back ("core: " + *i + " successfully unloaded");
        return PLUGIN_SUCCESS;
}
                
int32_t CoreRecipricant::usage (Args &out)
{
        Args ret;
        out.push_back ("core: usage");
        out.push_back ("load 'plugin path'");
        out.push_back ("unload 'plugin name'");
        return PLUGIN_FAILURE;
}

