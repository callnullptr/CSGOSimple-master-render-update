#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include "imgui/imgui.h"

#define A( s ) #s
#define OPTION(type, var, val) Var<type> var = {A(var), val}

template <typename T = bool>
class Var {
public:
	std::string name;
	std::shared_ptr<T> value;
	int32_t size;
	Var(std::string name, T v) : name(name) {
		value = std::make_shared<T>(v);
		size = sizeof(T);
	}
	operator T() { return *value; }
	operator T*() { return &*value; }
	operator T() const { return *value; }
	//operator T*() const { return value; }
};

class Options
{
public:
		// 
		// ESP
		// 
		OPTION(bool, esp_enabled, false);
		OPTION(bool, esp_enemies_only, false);
		OPTION(bool, esp_player_boxes, false);
		OPTION(bool, esp_player_names, false);
		OPTION(bool, esp_player_health, false);
		OPTION(bool, esp_player_armour, false);
		OPTION(bool, esp_player_weapons, false);
		OPTION(bool, esp_player_snaplines, false);
		OPTION(bool, esp_crosshair, false);
		OPTION(bool, esp_dropped_weapons, false);
		OPTION(bool, esp_defuse_kit, false);
		OPTION(bool, esp_planted_c4, false);
		OPTION(bool, esp_items, false);

		// 
		// GLOW
		// 
		OPTION(bool, glow_enabled, false);
		OPTION(bool, glow_enemies_only, false);
		OPTION(bool, glow_players, false);
		OPTION(bool, glow_chickens, false);
		OPTION(bool, glow_c4_carrier, false);
		OPTION(bool, glow_planted_c4, false);
		OPTION(bool, glow_defuse_kits, false);
		OPTION(bool, glow_weapons, false);

		//
		// CHAMS
		//
		OPTION(bool, chams_player_enabled, false);
		OPTION(bool, chams_player_enemies_only, false);
		OPTION(bool, chams_player_wireframe, false);
		OPTION(bool, chams_player_flat, false);
		OPTION(bool, chams_player_ignorez, false);
		OPTION(bool, chams_player_glass, false);
		OPTION(bool, chams_arms_enabled, false);
		OPTION(bool, chams_arms_wireframe, false);
		OPTION(bool, chams_arms_flat, false);
		OPTION(bool, chams_arms_ignorez, false);
		OPTION(bool, chams_arms_glass, false);

		//
		// MISC
		//
		OPTION(bool, misc_bhop, false);
		OPTION(bool, misc_no_hands, false);
		OPTION(bool, misc_thirdperson, false);
		OPTION(bool, misc_showranks, true);
		OPTION(bool, misc_watermark, true);
		OPTION(float, misc_thirdperson_dist, 50.f);
		OPTION(int, viewmodel_fov, 68);
		OPTION(float, mat_ambient_light_r, 0.0f);
		OPTION(float, mat_ambient_light_g, 0.0f);
		OPTION(float, mat_ambient_light_b, 0.0f);

		// 
		// COLORS
		// 
		OPTION(ImColor, color_esp_ally_visible, ImColor(0, 128, 255));
		OPTION(ImColor, color_esp_enemy_visible, ImColor(255, 0, 0));
		OPTION(ImColor, color_esp_ally_occluded, ImColor(0, 128, 255));
		OPTION(ImColor, color_esp_enemy_occluded, ImColor(255, 0, 0));
		OPTION(ImColor, color_esp_crosshair, ImColor(255, 255, 255));
		OPTION(ImColor, color_esp_weapons, ImColor(128, 0, 128));
		OPTION(ImColor, color_esp_defuse, ImColor(0, 128, 255));
		OPTION(ImColor, color_esp_c4, ImColor(255, 255, 0));
		OPTION(ImColor, color_esp_item, ImColor(255, 255, 255));

		OPTION(ImColor, color_glow_ally, ImColor(0, 128, 255));
		OPTION(ImColor, color_glow_enemy, ImColor(255, 0, 0));
		OPTION(ImColor, color_glow_chickens, ImColor(0, 128, 0));
		OPTION(ImColor, color_glow_c4_carrier, ImColor(255, 255, 0));
		OPTION(ImColor, color_glow_planted_c4, ImColor(128, 0, 128));
		OPTION(ImColor, color_glow_defuse, ImColor(255, 255, 255));
		OPTION(ImColor, color_glow_weapons, ImColor(255, 128, 0));

		OPTION(ImColor, color_chams_player_ally_visible, ImColor(0, 128, 255));
		OPTION(ImColor, color_chams_player_ally_occluded, ImColor(0, 255, 128));
		OPTION(ImColor, color_chams_player_enemy_visible, ImColor(255, 0, 0));
		OPTION(ImColor, color_chams_player_enemy_occluded, ImColor(255, 128, 0));
		OPTION(ImColor, color_chams_arms_visible, ImColor(0, 128, 255));
		OPTION(ImColor, color_chams_arms_occluded, ImColor(0, 128, 255));
		OPTION(ImColor, color_watermark, ImColor(0, 128, 255)); // no menu config cuz its useless
};

inline Options g_Options;
inline bool   g_Unload;
