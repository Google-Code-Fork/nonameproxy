#ifndef AUTOFISHER_H_
#define AUTOFISHER_H_

#include <string>
#include <set>
#include <stdint.h>

#include "tibiamessage.h"
#include "networkmessage.h"
#include "hook.h"
#include "packethook.h"
#include "client.h"

struct FishyTile
{
	FishyTile(const Pos& p, const uint16_t tid): pos(p), id(tid){}
	Pos pos;
	uint16_t id;
};

class AutoFishRecipricant : public Recipricant
{
	public:
		int32_t func (const Args& args, Args &out);
};

class AutoFishEffectHook : public ReadHook
{
	public:
		void func(TibiaMessage* tm, Client* client);
};

class AutoFish
{
	public:
		AutoFish();
		void Load(uint32_t plugin_id, Client *client);
		void Unload();

		int32_t Usage(Args& out);

		void Fish();
	
		bool Enabled() const;
		void Enable();
		void Disable();

	private:
		Client* m_client;
		uint32_t m_rid;
		uint32_t m_plugin_id;
		uint32_t m_effect_hook_id;
		bool m_enabled;
};

AutoFish::AutoFish() : m_enabled(false) {}
inline bool AutoFish::Enabled() const { return m_enabled; }
inline void AutoFish::Enable(){ m_enabled = true;}
inline void AutoFish::Disable(){ m_enabled = false;}

extern "C"
{
        void load (uint32_t id, Client *client);
        void unload ();
        const std::string &name ();
}

#endif

