/*****************************************************************************
  *noname proxy
 *****************************************************************************
 *
 *****************************************************************************
  *This program is free software; you can redistribute it and*or
  *modify it under the terms of the GNU General Public License
  *as published by the Free Software Foundation; either version 2
  *of the License, or (at your option) any later version.
 *
  *This program is distributed in the hope that it will be useful,
  *but WITHOUT ANY WARRANTY; without even the implied warranty of
  *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *GNU General Public License for more details.
 *
  *You should have received a copy of the GNU General Public License
  *along with this program; if not, write to the Free Software Foundation,
  *Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *****************************************************************************/

#ifndef __MESSENGER_H
#define __MESSENGER_H

#include <stdint.h>
#include <map>
#include <string>

#include "idmanager.h"

/**
 * define EXIT_FAILURE and EXIT_SUCCESS
 * these are already defined so we use slightly different names
 */

#define PLUGIN_SUCCESS 0
#define PLUGIN_FAILURE 1

typedef std::string Arg;
typedef std::list<Arg> Args;

/***************************************************************************
 * ArgsParser will split a string into tokens. When someone sends a message
 * the string they send will be tokenised and then the recipricant will
 * recieve a list of args.
 * To excape a character, use \ (note in c you will need \\ to excape the \)
 * To send a string with spaces, use a ' on either side of the string
 * Examples:
 *      abc def ghi             -> |abc|def|ghi|
 *      'abc def' ghi           -> |abc def|ghi|
 *      ab'c def g'hi           -> |abc def ghi|
 *      ' a b c '               -> | a b c |
 *      \a\b\c\ \d\e\f          -> |abc def|
 *      a\'b d\\e               -> |a'b|d\e| 
 ***************************************************************************/

class Client;
class PluginManager;

class ArgsParser
{
        public:
                ArgsParser (const std::string &msg, Client *client);
                const Args &getArgs ();
        private:
                bool addToken ();

                Args args;
                const std::string &_msg;
                std::string::const_iterator i;

                uint32_t start;
                uint32_t length;
                bool quotes;
                bool excape;

                Client *_client;
};

class Recipricant
{
        public:
                virtual ~Recipricant () {};
                virtual int32_t func (const Args &args, Args &out) = 0;
};

typedef std::map<uint32_t, Recipricant*> RecipricantList;

class Messenger
{
        public:
                Messenger (Client *client);
                virtual ~Messenger ();

                /* Add hook returns a unique id for the hook just added */
                uint32_t addRecipricant    (Recipricant *recipricant);
                void     deleteRecipricant (uint32_t rid);

                int32_t sendMessage (uint32_t rid, const std::string &msg,
                                     Args &out);
                int32_t broadcastMessage (const std::string &msg, Args &out);

        private:
                RecipricantList rlist;
                IdManager *ids;
                Client *_client;
};

#endif

