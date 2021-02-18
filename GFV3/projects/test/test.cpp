#include "test.h"


void test_proj::test_proj::MovePlayer()
{
	switch (e.key.keysym.sym)
	{

	case SDLK_w:
		img1.rect.y -= 10;
		break;

	case SDLK_s:
		img1.rect.y += 10;
		break;

	case SDLK_a:
		img1.rect.x -= 10;
		break;

	case SDLK_d:
		img1.rect.x += 10;
		break;

	case SDLK_q:
		img1.angle += 5;
		break;

	case SDLK_e:
		img1.angle -= 5;
		break;

	default:
		break;
	}
}

void test_proj::test_proj::OnQuit()
{
	running = false;
}

void test_proj::test_proj::init()
{
	WRinit("RYANS GAY", 1000, 700);

	bind_input(SDL_QUIT, bind(&test_proj::OnQuit, this));

	bind_input(SDL_KEYDOWN, bind(&test_proj::MovePlayer, this));

	string img_path = "./projects/test/images/DefaultProfilePicture.png";


	img1 = Image::CreateImg(img_path, renderer);
	img1.SetPos({100, 100});
	img1.SetSize({img1.GetW()/2, img1.GetH()/2});

	img2 = Image::CreateImg(img_path, renderer);
	img2.SetPos({ 400, 400 });
	img2.SetSize({img2.GetW()/2, img2.GetH()/2});
}


void test_proj::test_proj::ep()
{
	init();
	SDL_Color color;
	while (running) {

		SetFrameStart();

		//input
		gInput();
		//input


		//update
		img2.UpdateBoundingBoxDefault();
		img1.UpdateBoundingBoxDefault();
		img1.BoundingBox = Points::RotatePoints(img1.BoundingBox, img1.angle);

		if (Collision::detectCollision(img1.BoundingBox, img2.BoundingBox)) {
			color = {255, 0, 0, 255};
		}
		else {
			color = {0, 255, 0, 255};
		}
		//update

		//bg color
		WindowBgColor({225,225,225,225});
		//bg color

		prep();

		//render stuff here
		img1.DrawImage(renderer);
		Collision::DrawCollisionBounds(img1.BoundingBox, color, renderer);
		img2.DrawImage(renderer);
		//render stuff here

		pres();


		//do frame delays
		Delay();
	}

	cleanup();
}

void test_proj::test_proj::cleanup()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
