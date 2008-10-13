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

#ifndef __CORE_RECIPRICANT_H
#define __CORE_RECIPRICANT_H

#include <stdint.h>
#include "messenger.h"

class Client;

class CoreRecipricant : public Recipricant
{
        public:
                CoreRecipricant (Client* client);
                virtual void func (const Args& args);
        private:
                void loadPlugin (const Args& args, Args::const_iterator i,
                                        uint32_t argc);
                void unloadPlugin (const Args& args, Args::const_iterator i,
                                        uint32_t argc);

                Client* _client;
};

#endif

