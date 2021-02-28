#include "BoundsEditor.h"

void BoundsEditor::BoundsEditor::OnQuit()
{
	running = false;
}

void BoundsEditor::BoundsEditor::OnClick()
{
	cout << mouse_pos.x << ", " << mouse_pos.y << endl;

	mouse_count++;

	BoundingBox.v.push_back(mouse_pos);
	BoundingBox.v.push_back(mouse_pos);
}

void BoundsEditor::BoundsEditor::ep()
{
	WRinit("BoundsEditor", 1600, 900);
	bind_input(SDL_QUIT, bind(&BoundsEditor::OnQuit, this));
	bind_input(SDL_MOUSEBUTTONDOWN, bind(&BoundsEditor::OnClick, this));

	WindowBgColor({ 175,175,175,225 });

	BoundingBox.color = { 0, 0, 0 ,255 };

	while (running) {
		SetFrameStart();

		SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
		gInput();
		

		prep();

		if (mouse_count != 0) {
			BoundingBox.v[BoundingBox.v.size() - 1] = mouse_pos;
			DrawBounds(BoundingBox);
		}

		pres();

		Delay();
	}

	cleanup();
}

void BoundsEditor::BoundsEditor::cleanup()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
