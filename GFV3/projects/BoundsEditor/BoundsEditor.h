#pragma once

#include "../../GFW.h"

using namespace GFW;

//project namespace
namespace BoundsEditor {

	class BoundsEditor : public Inst {
	public:
		void OnQuit();
		void OnClick();
		void ep();

		void cleanup();

	private:
		Points::Points BoundingBox;

		bool draw_point = false;
		int mouse_count;
		Vector2D mouse_pos;
	};


}