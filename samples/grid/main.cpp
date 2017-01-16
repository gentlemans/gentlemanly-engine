#include <ge/input_subsystem.hpp>
#include <ge/mesh_actor.hpp>
#include <ge/rocket_document_asset.hpp>
#include <ge/rocket_input_consumer.hpp>
#include <ge/rocket_subsystem.hpp>
#include <ge/runtime.hpp>
#include <ge/sdl_subsystem.hpp>
#include <ge/timer_subsystem.hpp>
#include <ge/renderable.hpp>

#include <glm/glm.hpp>

#include <cmath>
#include <memory>
#include "grid.hpp"
#include "gridtick_interface.hpp"
#include "damagable.hpp"
#include "piece.hpp"
#include "turret.hpp"
#include "zombie.hpp"
#include "zombiespawner.hpp"
#include "spike.hpp"


#include <Rocket/Debugger/Debugger.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

using namespace ge;

struct hud : actor {
  
  grid* g;
  Rocket::Core::ElementDocument* rdoc;
  Rocket::Core::ElementText* text;
  
  void initialize(grid* gr, Rocket::Core::ElementDocument* doc){
    g = gr;
    rdoc = doc;
    add_interface<hud, gridtick_interface>();
    
    auto zcount = doc->GetElementById("zcount");
    text = rdoc->CreateTextNode("0");
    zcount->AppendChild(text);
  }
  
  void tick_grid() {
    int zcount = g->get_z_count();
    
    text->SetText(std::to_string(zcount).c_str());
  }
  
};

struct myIC : input_consumer<myIC> {

	myIC(runtime* rt) : input_consumer(rt) {
        steal_input();
	}

	bool handle_input(ge::input_event ev) {
        static int i = 0;
		std::cout << i++ << std::endl;
		return true;
	}

};

int main()
{
	runtime r;
	
	r.m_asset_manager.add_asset_path("data/");
	r.add_subsystem<input_subsystem>({});
	r.add_subsystem<timer_subsystem>({});
	auto& sdl = r.add_subsystem<sdl_subsystem>(sdl_subsystem::config{"Example!", {1024, 720}});
	auto& rocket = r.add_subsystem<rocket_subsystem>({});

	r.register_interface<renderable>();
	r.register_interface<gridtick_interface>();
	r.register_interface<damagable>();
	
	// load UI
	auto doc = r.m_asset_manager.get_asset<rocket_document_asset>("gridui/doc.rocketdocument");
	doc->Show();

	auto root = actor::root_factory(&r);

    auto camera = actor::factory<camera_actor>(root.get(), 13.f);

	sdl.set_background_color({.2f, .2f, .2f});
	sdl.set_camera(camera.get());
	r.set_root_actor(root.get());

	// initialize the grid
    auto g = actor::factory<grid>(root.get(), glm::ivec2{11, 11}, 2.f);
	actor::factory<spike>(g.get(), glm::ivec3(4, 4, 1));
	actor::factory<spike>(g.get(), glm::ivec3(4, 6, 1));
	actor::factory<spike>(g.get(), glm::ivec3(6, 4, 1));
	actor::factory<spike>(g.get(), glm::ivec3(6, 6, 1));
	actor::factory<turret>(g.get(), glm::ivec3(4, 5, 2));  // ->set_relative_rotation(glm::half_pi<float>());
	for (int x = 0; x < 12; x++) {
		actor::factory<zombiespawner>(g.get(), glm::ivec3(-1, x, 2));
		actor::factory<zombiespawner>(g.get(), glm::ivec3(11, x-1, 2));
		actor::factory<zombiespawner>(g.get(), glm::ivec3(x-1, -1, 2));
		actor::factory<zombiespawner>(g.get(), glm::ivec3(x, 11, 2));
	}
	g->try_spawn_z();

//    Rocket::Debugger::Initialise(rocket.m_context);
//    Rocket::Debugger::SetVisible(true);

    myIC otherIC{&r};

	rocket_input_consumer ic{&r};
	ic.steal_input();

    actor::factory<hud>(root.get(), g.get(), doc.get());
    
#ifdef EMSCRIPTEN
	emscripten_set_main_loop_arg(
		[](void* run_ptr) {
			runtime* runt = (runtime*)run_ptr;

			runt->tick();
		},
		&r, 0, true);
#else
	while (r.tick())
		;
#endif
	
	int& a = *(int*)0;
}
