#include "tutorial1.h"
#include "tibiamessage.h"
#include "typeparser.h"

/**
 * Here we store the plugins name, pluginId, and its pointer to
 * the client where they are safe and sound
 */

std::string _name;
uint32_t    _pluginId;
Client      *_client;

void load (uint32_t id, Client *client)
{
        _name = "hello";
        _pluginId = id;
        _client = client;

        /**
         * Now we want to do something interesting
         * Tibias packets are made up of many messages , and in nonameproxy
         * you work with message lists, not individual messages. It is true
         * however that many times the packet will only contain a single
         * message. None the less, to send a message, you must use a message
         * list.
         * There are two kinds of message lists,
         * GSMessageList
         * GRMessageList
         *
         * These stand for game send message list and game recv message list
         * There are also login message lists, but you wont be using those.
         * Only messages to be sent to the server may be put into an
         * GSMessageList, and only messages to be sent to the client may
         * be placed in GRMessageList. Because we only want to send messages
         * To ourself, and not to the server, we will use a GRMessageList
         */

         GRMessageList grml;

         /**
          * Now we will make a GRMSpeak message. GRM stands for game recv
          * message. Likewise GSM stands for game send message. To get an
          * understanding of tibias messages, you will have to look through
          * tibiamessage.h until I document all the messages, which could
          * take a long time. Usually its pretty easy to understand though
          *
          * A GRMSpeak takes 6 parameters
          * unknown, always 0
          * player name
          * player level
          * message type
          * position of player
          * message
          *
          * There are actually 3 different types of speak, but the one we
          * will use is public speak. Things like message type are defined
          * in enums.h in terms you can actually understand
          */

        /**
         * First however we need to find out our players position. To do
         * this we query the game state. The game state is part of the
         * client and is divided into several sub categories. In this case
         * we only need mapstate
         */

        Pos playerPos = _client->gstate->map->getCurPos ();

        /**
         * Now we can declare the GRMSpeak. Note that GRMSpeak must be
         * declared on the heap using new. When it is put into the
         * message list, it will be destroyed automatically so you dont
         * have to worry about it any more
         *
         * There is one last wrinkle, the data types use for sending
         * and recving over the network are good for this purpose, but
         * bad for general purpose. For that reason, we must convert a
         * Pos into a TPos before we can use it. You will get used to
         * this concept, and the good news it isn't needed very frequently
         * but positions are one such thing. To convert from a
         * Type to a TType (all tibia types begin with T) you will need a
         * TypeParser class and use the toTPos function
         */

        TypeParser tp;

        GRMSpeak *speak = new GRMSpeak ((uint32_t)0,
                                        "Bot",
                                        (uint16_t)1337,
                                        SPEAK_SAY,
                                        tp.toTPos (playerPos),
                                        "hello world");

        /**
         * Now we add it to the GRMessageList
         */
        grml.add (speak);

        /**
         * And finally send it to the server
         */
        _client->sendToClient (grml);
}

void unload ()
{
        GRMessageList grml;
        Pos playerPos = _client->gstate->map->getCurPos ();
        TypeParser tp;
        GRMSpeak *speak = new GRMSpeak ((uint32_t)0,
                                        "Bot",
                                        (uint16_t)1337,
                                        SPEAK_SAY,
                                        tp.toTPos (playerPos),
                                        "bye bye world");
        grml.add (speak);
        _client->sendToClient (grml);
}

const std::string &name()
{
        /**
         * This is just a simple function to return the plugin's name
         */
        return _name;
}

