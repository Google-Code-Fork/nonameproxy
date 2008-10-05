#include <stdio.h>
#include "messenger.h"

Messenger::Messenger ()
{
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

void Messenger::sendMessage (uint32_t rid, const std::string& msg)
{
        RecipricantList::iterator i = rlist.find (rid);

        if (i == rlist.end ()) {
                printf ("messenger send error: recipricant does not exist\n");
        } else {
                ArgsParser ap (msg);
                (*i).second->func (ap.getArgs ());
        }
}

void Messenger::broadcastMessage (const std::string& msg)
{
        ArgsParser ap (msg);
        const Args& args = ap.getArgs ();
        for (RecipricantList::iterator i; i != rlist.end (); ++ i) {
                (*i).second->func (args);
        }
}

/*****************************************************************
 * ArgsParser
 * i agree, its ugly :(
 *****************************************************************/
ArgsParser::ArgsParser (const std::string& msg) : _msg (msg)
{
        i = _msg.begin ();

        start = 0;
        length = 0;
        quotes = false;
        excape = false;

        while (addToken ());
}

bool ArgsParser::addToken ()
{

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

        std::string token;
        do {
                excape = false;
                while (i != _msg.end ()) 
                {
                        if ((isspace (*i) && !quotes) || *i == '\'' || *i == '\\') {
                                break;
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
