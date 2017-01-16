#pragma once

#ifndef SPIKE_HPP
#define SPIKE_HPP

#include "piece.hpp"
#include "gridtick_interface.hpp"
#include "damagable.hpp"

#include <ge/mesh.hpp>
#include <ge/mesh_actor.hpp>
#include <ge/texture_asset.hpp>
#include "grid.hpp"

class spike : public piece
{
	ge::mesh_actor* mesh;

	boost::signals2::scoped_connection die_connect;

public:
	void initialize(glm::uvec3 location)
	{
		piece::initialize(location);
		add_interface<spike, gridtick_interface>();
		mesh = ge::actor::factory<ge::mesh_actor>(this, "turret/turret.meshsettings").get();
		mesh->m_mesh_settings.m_material.set_parameter("Texture", m_runtime->m_asset_manager.get_asset<ge::texture_asset>("spike.texture"));

		die_connect = sig_die.connect([](piece* p) {
			p->set_parent(NULL);
		});
	}
	void tick_grid()
	{
		glm::ivec3 myLocation = get_grid_location();
		auto actors = m_grid->get_actors_from_coord(glm::ivec3(myLocation.x,myLocation.y,2));
		for (int x = 0; x < actors.size(); x++)
		{
			auto d = actors[x]->get_interface_storage<damagable>();
			if (d)
			{
				d->damage(10);
			}
		}
	}
};

#endif  // TURRET_HPP
