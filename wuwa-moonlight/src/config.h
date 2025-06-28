#pragma once

namespace config
{
	namespace multihit
	{
		inline bool enabled = true;
		inline int max_hits = 100;
		inline int min_hits = 2;
		inline int hits = 3;
	}

	namespace godmode
	{
		inline bool enabled = true;
	}

	namespace esp
	{
		inline bool enable = false;
	}
	namespace binds
	{
		inline int menu_key = VK_INSERT;
		inline int quit_key = VK_F9;
	}
}