#ifndef GODOT_MOD_TEMPLATE_TEMPLATE_H
#define GODOT_MOD_TEMPLATE_TEMPLATE_H

#include <godot_cpp/classes/sprite2d.hpp>

namespace godot {
	class GDTemplate : public Sprite2D {
		GDCLASS(GDTemplate, Sprite2D)
	
	private:
		double time_passed;
	
	protected:
		static void _bind_methods();
	
	public:
		GDTemplate();
		~GDTemplate();
		
		void _process(double delta) override;
	};
}


#endif //GODOT_MOD_TEMPLATE_TEMPLATE_H
