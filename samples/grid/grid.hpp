#pragma once

#include <ge/actor.hpp>
#include <ge/asset_manager.hpp>

#include <memory>
#include <random>
#include <vector>

#include "ticktimer.hpp"
#include "zombiespawner.hpp"

class piece;

class grid : public ge::actor
{
	glm::ivec2 m_size;
	std::mt19937 rand_gen;
	unsigned int z_count=0;
	unsigned int max_z = 5;
	const int z_spawn_delay = 1;
	bool spawning = false;
public:
	ticktimer* timer;
	int get_random(int lower, int higher)
	{
		std::uniform_int_distribution<> uniform_int(lower, higher);
		return uniform_int(rand_gen);
	}
	void initialize(glm::ivec2 size, float tps);

	std::vector<piece*> get_actors_from_coord(glm::ivec3 loc);

	void try_spawn_z();

	glm::ivec2 get_size() { return m_size; }
	void increment_z_count(bool positive)
	{
		if (positive)
			z_count++;
		else
		{
			z_count--;
			try_spawn_z();
		}
	}
	int get_z_count()
	{
		return z_count;
	}


    

};
