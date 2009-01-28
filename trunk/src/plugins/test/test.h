#ifndef __TEST
#define __TEST

#include <string>
#include <set>
#include <stdint.h>

#include "client.h"

/******************************************************************************
 * test evaluates a series of condions and returns "1" if they are true
 * and "0" if they are false
 *
 * test is inspired by unix test, and there are several things to understand
 *
 * 1: everything is evaluated as a string
 *      if the first character of an arg is numeric, it is numeric string
 *      if the first character is alpha, then it is just a plain string
 *      if the string is surrounded by `, then the string value is the 
 *              result of executing the string in the console
 * 
 * : the following operators return true if
 *      ( EXPRESSION )          EXPRESSION is true
 *      ! EXPRESSION            EXPRESSION is false
 *
 *      EXPRESSION1 -a EXPRESSION2
 *                              EXPRESSION1 and EXPRESSION2 are true
 *      
 *      EXPRESSION1 -o EXPRESSION2
 *                              EXPRESSION1 or EXPRESSION2 is true
 *      
 *      -n STRING               string length is not 0
 *      -z STRING               string length is 0
 *                              only works with '' and empty Args
 *
 *      STRING1 = STRING2       strings are equal
 *      STRING1 != STRING2      strings are not equal
 *
 * Note: here the numeric value of strings are compared. ie '12 3' == '12blah'
 *      INTEGER1 -eq INTEGER2   INTEGER1 is equal to INTEGER 2
 *      INTEGER1 -ge INTEGER2   INTEGER1 is greater than or equal to INTEGER2
 *      INTEGER1 -gt INTEGER2   INTEGER1 is greater than INTEGER2
 *      INTEGER1 -le INTEGER2   INTEGER1 is less than or equal to INTEGER2
 *      INTEGER1 -lt INTEGER2   INTEGER1 is less than INTEGER2
 *      INTEGER1 -ne INTEGER2   INTEGER1 is not equal to INTEGER2
 *
 * 
 * Usage:
 *      test expression
 *
 * Example:
 *      test ( '`info hp`' -lt 200 ) -a ( '`info mana`' -gt 20 )
 *
 * Note: spaces are important
 *****************************************************************************/

class TestRecipricant : public Recipricant
{
        public:
                virtual int32_t func (const Args &args, Args &out);
};

class Test
{
        public:
                int32_t test (const Args &args, Args &out);

                void i_load (uint32_t id, Client *client);
                void i_unload ();
                const std::string &i_name ();

        private:
                uint32_t        _pluginId;
                uint32_t        _rid;

                Client          *_client;

                std::string     _name;
};

extern "C"
{
        void load (uint32_t id, Client *client);
        void unload ();
        const std::string &name ();
}

#endif

