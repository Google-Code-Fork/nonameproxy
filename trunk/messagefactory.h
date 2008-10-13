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

#ifndef __MESSAGE_FACTORY_H
#define __MESSAGE_FACTORY_H

class TibiaMessage;
class NetworkMessage;
class GameState;
class DatReader;

//A Message factory takes an unecrypted network message and processes it
//getMessage will return a ptr to the next TibiaMessage or return NULL
//if packet has been completely read. When its deconstructor is called
//it will also call delete the NetworkMessage, so after assigning a
//networkmessage to a factory do not use it again

class MessageFactory
{
        public:
                virtual ~MessageFactory () {}

                virtual TibiaMessage* getMessage () = 0;
                virtual bool isEnd () = 0;
};

class LSMessageFactory : public MessageFactory
{
        public:
                LSMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                virtual ~LSMessageFactory ();

                virtual TibiaMessage* getMessage ();
                virtual bool isEnd ();
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;
};

class LRMessageFactory : public MessageFactory
{
        public:
                LRMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                virtual ~LRMessageFactory ();

                virtual TibiaMessage* getMessage ();
                virtual bool isEnd ();
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;
};

class GSMessageFactory : public MessageFactory
{
        public:
                GSMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                virtual ~GSMessageFactory ();

                virtual TibiaMessage* getMessage ();
                virtual bool isEnd ();
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;
};

class GRMessageFactory : public MessageFactory
{
        public:
                GRMessageFactory (NetworkMessage* msg,
                                        GameState* gs,
                                        DatReader* dat);
                virtual ~GRMessageFactory ();

                virtual TibiaMessage* getMessage ();
                virtual bool isEnd ();
        private:
                NetworkMessage* _msg;
                GameState* _gs;
                DatReader* _dat;
};
#endif

