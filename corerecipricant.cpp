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

CoreRecipricant::CoreRecipricant (Client* client)
{
        _client = client;
}

void CoreRecipricant::func (const Args& args)
{
        uint32_t consoleId = _client->getConsoleId ();
        Args::const_iterator i = args.begin ();
        uint32_t argc = args.size ();
        if (argc == 0) {
                return;
        }

        if (argc > 2) {
                i ++;
                if (*i == "load") {
                        loadPlugin (args, i, argc);
                } else if (*i == "unload") {
                        unloadPlugin (args, i, argc);
                }
        } else if (consoleId != 0) {
                _client->sendMessage (consoleId, 
                        "console 'core: usage'");
                _client->sendMessage (consoleId,
                        "console 'load \\'plugin path\\''");
                _client->sendMessage (consoleId,
                        "console 'unload \\'plugin name\\''");
        }
}

void CoreRecipricant::loadPlugin (const Args& args, Args::const_iterator i,
                                  uint32_t argc)
{
        uint32_t consoleId = _client->getConsoleId ();
        if (argc < 3) {
                _client->sendMessage (consoleId, 
                        "console 'core: load requires a path name'");
                return;
        }
        i ++;
        if (_client->pluginManager->addPlugin (*i) == 0) {
                std::string error = "console 'core: could not load " +
                                        *i + "'";
                _client->sendMessage (consoleId, error); 
        }
}
                
void CoreRecipricant::unloadPlugin (const Args& args, Args::const_iterator i,
                                    uint32_t argc)
{
        uint32_t consoleId = _client->getConsoleId ();
        if (argc < 3) {
                _client->sendMessage (consoleId, 
                        "console 'core: unload requires a plugin name'");
                return;
        }
        i ++;
        std::string error = "console 'core: could not unload plugin " + *i + "'";
        uint32_t pid = _client->getPluginByName (*i);
        if (pid == 0) {
                _client->sendMessage (consoleId, error);
                return;
        }
        if (!_client->pluginManager->deletePlugin (pid)) {
                _client->sendMessage (consoleId, error); 
        }
}
                
