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

#include <stdio.h>
#include "messenger.h"
#include "client.h"
#include "pluginmanager.h"

Messenger::Messenger (Client* client)
{
        _client = client;
        ids = new IdManager (100);
}

Messenger::~Messenger ()
{ 
        RecipricantList::iterator i;
        for (i = rlist.begin (); i != rlist.end (); ++ i) {
                delete (*i).second;
        }
        delete ids;
}

uint32_t Messenger::addRecipricant (Recipricant* recipricant)
{
        uint32_t id = ids->newId ();

        rlist.insert (std::pair<uint32_t, Recipricant*> (id, recipricant));
        return id;
}

void Messenger::deleteRecipricant (uint32_t rid)
{
        RecipricantList::iterator i = rlist.find (rid);
        if (i == rlist.end ()) {
                printf ("messenger delete error: recipricant doesnt exist\n");
                return;
        }
        delete (*i).second;
        if (!ids->recycleId (rid)) {
                printf ("messenger delete error: ids not concurrent\n");
        }
        rlist.erase (rid);
}

Args Messenger::sendMessage (uint32_t rid, const std::string& msg)
{
        RecipricantList::iterator i = rlist.find (rid);

        if (i == rlist.end ()) {
                printf ("messenger send error: recipricant does not exist\n");
                return Args ();
        } else {
                ArgsParser ap (msg, _client);
                return (*i).second->func (ap.getArgs ());
        }
}

Args Messenger::broadcastMessage (const std::string& msg)
{
        ArgsParser ap (msg, _client);
        const Args& args = ap.getArgs ();
        for (RecipricantList::iterator i; i != rlist.end (); ++ i) {
                return (*i).second->func (args);
        }
        return Args ();
}

/*****************************************************************
 * ArgsParser
 * i agree, its ugly :(
 *****************************************************************/
ArgsParser::ArgsParser (const std::string& msg, Client* client) : _msg (msg)
{
        _client = client;

        i = _msg.begin ();

        start = 0;
        length = 0;
        quotes = false;
        excape = false;

        while (addToken ());
}

bool ArgsParser::addToken ()
{
        if (i == _msg.end ()) {
                return false;
        }

        std::string token;
        /* back ticks get substituted for their console output */
        if (*i == '`' && !quotes) {
                /* get rid of first tick */
                start ++;
                length --;
                while (i != _msg.end ()) {
                        i ++;
                        length ++;
                        if (*i == '`') {
                                break;
                        }
                }
                std::string tmp;
                if (i == _msg.end ()) {
                        printf ("argsparser warning: unterminated `\n");
                        tmp = _msg.substr (start, length);
                } else {
                        i ++;
                        tmp = _msg.substr (start, length);
                        length ++;
                }

                start += length;
                length = 0;

                /* now we get the console output */
                Args output = _client->broadcastMessage (tmp);

                if (output.size () != 0) {
                        token = output.front ();
                        Args::iterator ii = output.begin ();
                        ii ++;
                        for (; ii != output.end (); ++ ii) {
                                token += " ";
                                token += *ii;
                        }
                }
                Args::iterator ii = output.begin ();
                args.push_back (token);
                return true;
        }

        /* skip white space */
        while (i != _msg.end ()) {
                if (!isspace (*i)) break;
                start ++;
                i ++;
        }
        if (i == _msg.end ())
        {
                return false;
        }

        do {
                excape = false;
                while (i != _msg.end ()) 
                {
                        if ((isspace (*i) && !quotes) || *i == '\'' || *i == '\\' 
                                || (*i == '`' && !quotes)) 
                        {
                                if (*i == '`') {
                                        /* we don't want to add an empty string */
                                        return true;
                                } else {
                                        break;
                                }
                        }
                        length ++;
                        i ++;
                }

                token.append (_msg.substr (start, length));
                start += length;
                length = 0;
                if (*i == '\\') {
                        i ++;
                        if (i != _msg.end ()) {
                                token.append (1, *i);
                                i ++;
                                start += 2;
                                excape = true;
                        } else {
                                printf ("argsparser warning: trailing '\\'\n");
                                /* if we get here its possible our earlier
                                 * white space test has been avoided */
                                if (token.length () == 0) {
                                        return false;
                                }
                                break;
                        }
                }
                        
                if (*i == '\'') {
                        start ++;
                        quotes = !quotes;
                        i ++;
                        excape = true;
                }
                if (*i == '`' && !quotes) {
                        break;
                }
        } while ((quotes || excape) && i != _msg.end ());
        if (quotes) {
                printf ("argsparser warning: unterminated '\n");
        }
        args.push_back (token);
        return true;
}

const Args& ArgsParser::getArgs ()
{
        return args;
}
