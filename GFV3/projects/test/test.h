#pragma once

#include "../../GFW.h"

using namespace GFW;

//project namespace
namespace test_proj {

	class test_proj : public Inst {
	public:
		void MovePlayer();
		void OnQuit();
		void init();
		void ep();

		void cleanup();

	private:
		Image::Image img1, img2;
		Text::Text txt;
		Circle::Circle c1;
	};


}
