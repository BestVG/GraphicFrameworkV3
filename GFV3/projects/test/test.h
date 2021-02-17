#pragma once

#include "../../GFW.h"


//project namespace
namespace test_proj {

	class test_proj : public GFW::Inst {
	public:
		void MovePlayer();
		void OnQuit();
		void init();
		void ep();

		void cleanup();

	private:
		GFW::Image img1, img2;
	};


}
