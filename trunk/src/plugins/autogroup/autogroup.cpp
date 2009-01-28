#include <stdio.h>
#include "autogroup.h"
#include "enums.h"

#define BUFFER_SIZE 256 

AutoGroup group;

int32_t AutoGroupRecipricant::func (const Args& args, Args &out)
{
        Args::const_iterator i = args.begin ();
        if (args.size () == 1) {
                if (*i == "autogroup") {
                        return group.usage (out);
                } else {
                        printf ("info error: got wrong message\n");
                        return PLUGIN_FAILURE;
                }
        }

        i ++;
        if (args.size () > 1) {
                if (*i == "list") {
                        return group.list_containers (out);
                } else if (*i == "ids") {
                        return group.list_ids (out);
                } else if (*i == "all") {
                        out.push_back (group.all_ids ());
                } else if (*i == "none") {
                        out.push_back (group.no_ids ());
                } else if (*i == "add") {
                        i ++;
                        for (; i != args.end (); ++ i) {
                                out.push_back (group.add_id (*i));
                        }
                } else if (*i == "remove") {
                        i ++;
                        for (; i != args.end (); ++ i) {
                                out.push_back (group.remove_id (*i));
                        }
                } else {
                        return group.usage (out);
                }
        } else {
                return group.usage (out);
        }
        return PLUGIN_FAILURE;
}

void ContainerAddHook::func (TibiaMessage* tm, Client* client)
{
        GRMContainerAdd* ca = (GRMContainerAdd*)tm;

        InventoryState* inventory = client->gstate->inventory;

        uint32_t cid = ca->getCid ();

        if (!group.newContainerCycle (cid)) {
                return;
        }

        if (group.isSet (cid) != 0) {
                /* in this case we only need to sort the added item */
                if (ca->getItem ().getType () == TThing::xitem) {
                        /* note the protocol has already parsed this message */
                        const Container& c = inventory->getContainer (cid);
                        group.groupItem (c, cid, (XItem&)c.getThing (0), 0);
                }
        }
}

void ContainerOpenHook::func (TibiaMessage* tm, Client* client)
{
        GRMContainerOpen* co = (GRMContainerOpen*)tm;

        InventoryState* inventory = client->gstate->inventory;

        uint32_t cid = co->getCid ();
        if (!group.newContainerCycle (cid)) {
                return;
        }

        if (group.isSet (cid) != 0) {
                /* sort the entire container */
                group.groupContainer (inventory->getContainer (cid), cid);
        }
}

void ContainerUpdateHook::func (TibiaMessage* tm, Client* client)
{
        GRMContainerUpdate* cu = (GRMContainerUpdate*)tm;

        InventoryState* inventory = client->gstate->inventory;

        uint32_t cid = cu->getCid ();
        if (!group.newContainerCycle (cid)) {
                return;
        }

        if (group.isSet (cid) != 0) {
                /* sort the entire container */
                group.groupContainer (inventory->getContainer (cid), cid);
        }
}

bool AutoGroup::newContainerCycle (uint32_t cid)
{
        uint32_t cur = _client->getCycle ();
        if (_cycles[cid] == cur) {
                return false;
        } else {
                _cycles[cid] = cur;
                return true;
        }
}

bool AutoGroup::isSet (uint32_t cid)
{
        if (_cids.count (cid) != 0) {
                return true;
        } else {
                return false;
        }
}

int32_t AutoGroup::usage (Args &out)
{
        out.push_back ("autogroup list");
        out.push_back ("autogroup ids");
        out.push_back ("autogroup add x y z");
        out.push_back ("autogroup remove x y z");
        out.push_back ("autogroup all");
        out.push_back ("autogroup none");
        return PLUGIN_FAILURE;
}

int32_t AutoGroup::list_ids (Args &out)
{
        char tmp[BUFFER_SIZE];
        if (_cids.size () == 0) {
                out.push_back ("none");
        } else {
                CidSet::iterator i;
                for (i = _cids.begin (); i != _cids.end (); ++ i) {
                        snprintf (tmp, BUFFER_SIZE, "%d", *i);
                        out.push_back (tmp);
                }
        }
        return PLUGIN_SUCCESS;
}

int32_t AutoGroup::list_containers (Args &out)
{
        char tmp[BUFFER_SIZE];
        InventoryState* inventory = _client->gstate->inventory;

        for (int i = 0; i < MAX_CONTAINER; i ++) {
                Container& c = inventory->getContainer (i);
                if (c.isOpen ()) {
                        snprintf (tmp, BUFFER_SIZE, 
                                  "container %d: cap = %d size = %d id = %d",
                                  i,
                                  c.getItemId (),
                                  c.getCapacity (),
                                  c.getThingCount ()
                                 );
                        out.push_back (tmp);
                }
        }
        if (out.size () == 0) {
                out.push_back ("no open containers");
        }
        return PLUGIN_SUCCESS;
}

std::string AutoGroup::all_ids ()
{
        _cids.clear ();
        for (uint32_t i = 0; i < MAX_CONTAINER; i ++) {
                _cids.insert (i);
                const Container& c = _client->gstate->inventory->getContainer (i);
                if (c.isOpen ()) {
                        groupContainer (c, i);
                }
        
        }
        return "autogroup: all cids set";
}

std::string AutoGroup::no_ids ()
{
        _cids.clear ();
        return "autogroup: all cids cleared";
}

std::string AutoGroup::add_id (const std::string& cid)
{
        uint32_t _cid = strtol (cid.c_str (), NULL, 0);
        _cids.insert (_cid);
       
        const Container& c = _client->gstate->inventory->getContainer (_cid);
        if (c.isOpen ()) {
                groupContainer (c, _cid);
        }
        
        return "autogroup: added " + cid;
}

std::string AutoGroup::remove_id (const std::string& cid)
{
        if (_cids.erase (strtol (cid.c_str (), NULL, 0)) == 0) {
                return "autogroup: cid " + cid + " not set";
        } else {
                return "autogroup: removed " + cid;
        }
}

bool AutoGroup::groupContainer (const Container& c, uint32_t cid)
{
        /* note we will have to be smart here as the changes will not
         * take effect until after the packet has been sent.
         * What we will engage in is a strange but awesome form of
         * recursion over a network connection */
        DatReader* dat = _client->dat;

        uint32_t size = c.getThingCount ();
        for (uint32_t i = 0; i < size; i ++) {
                const Thing& thing = c.getThing (i);
                if (thing.getType () == Thing::t_xitem) {
                        const XItem& xitem = (XItem&)thing;
                        uint32_t itemid = xitem.getItemId ();
                        if (dat->getItemData (itemid).isStackable ()) {
                                if (xitem.getXByte () != 100) {
                                        if (groupItem (c, cid, xitem, i)) {
                                                return true;
                                        }
                                }
                        }
                }
        }
        return false;
}

bool AutoGroup::groupItem (const Container& c, 
                             uint32_t cid,
                             const XItem& x, 
                             uint32_t i)
{
        uint32_t size = c.getThingCount ();
        uint32_t xid = x.getItemId ();
        uint32_t xindex = i;

        for (i = i + 1; i < size; i ++) {
                const Thing& thing = c.getThing (i);
                if (thing.getType () == Thing::t_xitem) {
                        const XItem& xitem = (XItem&)thing;
                        if (xitem.getItemId () == xid 
                         && xitem.getXByte () != 100)
                        {
                                uint32_t count;
                                if (xitem.getXByte () + x.getXByte () > 100) {
                                        count = 100 - x.getXByte ();
                                } else {
                                        count = xitem.getXByte ();
                                }
                                i_moveItem (cid, xid, i, xindex, count);
                                return true;
                        }
                }
        }
        return false;
}

void AutoGroup::i_moveItem (uint32_t cid, 
                            uint32_t itemid,
                            uint32_t from, 
                            uint32_t to, 
                            uint32_t count)
{
        GSMessageList gsml;
        gsml.add (new GSMMoveItem (cid, from, itemid, cid, to, count));
        _client->sendToServer (gsml);
}

void AutoGroup::i_load (uint32_t pluginId, Client* client)
{
        _name = "autogroup";
        _pluginId = pluginId;
        _client = client;

        _addhookid = _client->addRecvReadHook (_pluginId, 
                                               GRM_CONTAINER_ADD_ID,
                                               new ContainerAddHook);
                                               
        _openhookid = _client->addRecvReadHook (_pluginId, 
                                                GRM_CONTAINER_OPEN_ID,
                                                new ContainerOpenHook);

        _updatehookid = _client->addRecvReadHook (_pluginId, 
                                                  GRM_CONTAINER_UPDATE_ID,
                                                  new ContainerUpdateHook);

        _rid = _client->addRecipricant (_pluginId, new AutoGroupRecipricant ());
}

void AutoGroup::i_unload ()
{
        printf ("blah1\n");
        _client->deleteRecvReadHook (_pluginId, _addhookid);
        printf ("blah2\n");
        _client->deleteRecvReadHook (_pluginId, _openhookid);
        printf ("blah3\n");
        _client->deleteRecvReadHook (_pluginId, _updatehookid);
        printf ("blah4\n");

        _client->deleteRecipricant (_pluginId, _rid);
        printf ("blah5\n");
}

const std::string& AutoGroup::i_name ()
{
        return _name;
}

void load (uint32_t id, Client* _client)
{
        group.i_load (id, _client);
}

void unload ()
{
        printf ("yeah1\n");
        group.i_unload ();
        printf ("yeah2\n");
}

const std::string& name ()
{
        return group.i_name ();
}

