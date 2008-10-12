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
                
