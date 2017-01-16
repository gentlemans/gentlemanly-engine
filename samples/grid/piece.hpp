#ifndef GE_PIECE_HPP
#define GE_PIECE_HPP

#pragma once

#include <boost/signals2.hpp>

#include <ge/actor.hpp>

class grid;

class piece : public ge::actor
{
public:
	struct stats {
		float health;
		float damage;
		float speed;
		float regen;
	};
    enum Directions { NORTH = 0, EAST = 3, SOUTH = 2, WEST = 1, NONE = 5};
private:
	stats inital;
	stats now;
protected:
	Directions my_direction = NORTH;
public:
	grid* m_grid;
	int m_level;
	std::array<std::vector<piece*>, 4> checkNearbySquares(glm::ivec2 myLocation);
    std::vector<std::vector<piece*>> squares_in_direction(glm::ivec2 myLocation, Directions direction, int range);

    boost::signals2::signal<
        void(piece* p, glm::ivec3 new_loc, glm::ivec3 old_loc)
    > sig_move;

    boost::signals2::signal<
        void(piece* p, float amt)
    > sig_damaged;

	boost::signals2::signal<
		void(piece*)
	> sig_die;

	void initialize(glm::ivec3 loc);
	void set_grid_location(glm::ivec3 loc)
	{
        glm::ivec3 old = get_grid_location();

		set_relative_location({loc.x, loc.y});
		m_level = loc.z;

        sig_move(this, get_grid_location(), old);
	}
	glm::ivec3 get_grid_location() const
	{
		return {int(get_relative_location().x), int(get_relative_location().y), m_level};
	}
	Directions get_direction_to (glm::ivec2 initial, glm::ivec2 final)
	{
		int changeX = final.x - initial.x;
		int changeY = final.y - initial.y;
		Directions togo = NONE;
		if (changeX != 0 && changeY != 0)
			togo = NONE;
		if (changeX == 0 && changeY == 0)
			togo = NONE;
		if (changeY > 0)
			togo = NORTH;
		if (changeY < 0)
			togo = SOUTH;
		if (changeX > 0)
			togo = EAST;
		if (changeX < 0)
			togo = WEST;
		return togo;
	}
	void rotate(Directions direction)
	{
		if (direction == NONE)
			return;
		set_relative_rotation(direction*glm::half_pi<float>());
		my_direction = direction;
	}
};

#endif  // GE_PIECE_HPP
