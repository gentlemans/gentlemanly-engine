#pragma once

#include <anax/System.hpp>

#include <Box2D/Box2D.h>

#include "ge/transform_component.hpp"
#include "ge/velocity_component.hpp"

namespace ge {

	// inits the b2Body
	struct location_system : anax::System<anax::Requires<transform_component>> 
	{
		virtual void onEntityAdded(Entity& ent) override
		{
			b2BodyDef bdef;
			
			
			ent.getComponent<transform_component>.body = world.createBody();
		}
		
		b2World world{0.f, 0.f};
	};
	
	struct physics_system : anax::System<anax::Requires<transform_component, velocity_component>>
	{
		
	};
	
}

