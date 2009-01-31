#include <vector>

#include "autofish.h"
#include "tibiamessage.h"
#include "typeparser.h"

std::string _name;
uint32_t _pluginId;
Client *_client;

AutoFish fisher;

const uint16_t fishing_rod_id = 3483;
const uint16_t fish_tile_start = 4597;
const uint16_t fish_tile_end = 4602;

const uint16_t WATER_EFFECT = 2;

int32_t AutoFishRecipricant::func(const Args& args, Args& out)
{
	Args::const_iterator i = args.begin();

	if(args.size() == 1){
		if(*i == "autofish"){
			return fisher.Usage(out);
		}else{
			printf ("info error: got wrong message\n");
			return PLUGIN_FAILURE;
		}
	}

	i++;
	if(args.size() > 1){
		if(*i == "test"){
			fisher.Fish();
			return PLUGIN_FAILURE;
		}

		if(*i == "on"){
			fisher.Enable();
			fisher.Fish();
			return PLUGIN_FAILURE;
		}

		if(*i == "off"){
			fisher.Disable();
			return PLUGIN_FAILURE;
		}
	}

	return fisher.Usage(out);
}

void AutoFishEffectHook::func(TibiaMessage* tm, Client* client)
{
	static uint32_t last_cycle = 0;
	GRMMagicEffect* m = (GRMMagicEffect*) tm;

	if(fisher.Enabled() && last_cycle != client->getCycle()){
		last_cycle = client->getCycle();
		if(m->getEffect() == WATER_EFFECT){
			fisher.Fish();
		}
	}
}

void AutoFish::Load(uint32_t plugin_id, Client *client)
{
	m_plugin_id = plugin_id;
	m_client = client;
	m_rid = client->addRecipricant(plugin_id, new AutoFishRecipricant() );

        m_effect_hook_id = _client->addRecvReadHook(m_plugin_id, GRM_MAGIC_EFFECT_ID, new AutoFishEffectHook());
}

void AutoFish::Unload()
{
	m_client->deleteRecvReadHook(m_plugin_id, m_effect_hook_id);
	m_client->deleteRecipricant(m_plugin_id, m_rid);
}

int32_t AutoFish::Usage(Args& out)
{
	out.push_back("autofish on");
	out.push_back("autofish off");
	out.push_back("autofish test");

	return PLUGIN_FAILURE;
}

void AutoFish::Fish()
{
	uint16_t px = m_client->gstate->map->getCurPos().x;
	uint16_t py = m_client->gstate->map->getCurPos().y;
	uint8_t pz = m_client->gstate->map->getCurPos().z;

	std::vector<FishyTile> fishy_tiles;

	for(uint16_t x = px-5; x < px+5; x++){
		for(uint16_t y = py-5; y < py+5; y++){
			Item &itm = (Item&) (m_client->gstate->map->getTile(x,y,pz).getThing(0));
			uint16_t id = itm.getItemId();
			if(id >= fish_tile_start && id <= fish_tile_end){
				fishy_tiles.push_back(FishyTile(Pos(x,y,pz), id));
			}
		}
	}


	if(fishy_tiles.size() > 0){
		GSMessageList gsml;

		uint32_t fish_at = rand() % fishy_tiles.size();
		TPos fish_pos(fishy_tiles[fish_at].pos.x, fishy_tiles[fish_at].pos.y, fishy_tiles[fish_at].pos.z);

		gsml.add(new GSMUseItemWith(SLOT_AMMO, fishing_rod_id, fish_pos, fishy_tiles[fish_at].id, (uint8_t)0));

		m_client->sendToServer(gsml);
	}
}

void load(uint32_t id, Client *client)
{
	_name = "autofish";
	_pluginId = id;
	_client = client;
	
	fisher.Load(id, client);
}

void unload()
{
	fisher.Unload();
}

const std::string &name()
{
        return _name;
}