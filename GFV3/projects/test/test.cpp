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
		img1.angle -= 5;
		break;

	case SDLK_e:
		img1.angle += 5;
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
	WRinit("Noobers", 1000, 700);

	bind_input(SDL_QUIT, bind(&test_proj::OnQuit, this));

	bind_input(SDL_KEYDOWN, bind(&test_proj::MovePlayer, this));

	string img_path = "./projects/test/images/DefaultProfilePicture.png";


	img1 = Image::CreateImg(img_path, renderer);
	img1.SetPos({100, 100});
	img1.SetSize({img1.GetW()/2, img1.GetH()/2});

	img2 = Image::CreateImg(img_path, renderer);
	img2.SetPos({ 400, 400 });
	img2.SetSize({img2.GetW()/2, img2.GetH()/2});

	fontManager.LoadFont("HighlandGothicFLF.ttf", 20, "noober");

	txt.font = fontManager["noober"];
	txt.msg = "Noobers";
	txt.pos.x = 500 - txt.GetTextSize().first / 2;
	txt.pos.y = 30;
}


void test_proj::test_proj::ep()
{
	init();

	//bg color
	WindowBgColor({ 175,175,175,225 });
	//bg color

	while (running) {

		SetFrameStart();

		//input
		gInput();
		//input


		//update
		Update(img1);
		Update(img2);
		Update(txt);

		img1.BoundingBox = Points::RotatePoints(img1.BoundingBox, img1.angle);

		if (img1.detectCollision(img2)) {
			img1.BoundingBox.color = {255, 0, 0, 255};
		}
		else {
			img1.BoundingBox.color = {0, 255, 0, 255};
		}
		//update

		prep();

		//render stuff here
		Draw(img1);
		DrawBounds(img1);
		Draw(img2);
		Draw(txt);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawLine(renderer, 300, 300, 300, 330);
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
