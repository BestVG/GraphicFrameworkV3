 #include "test.h"


void test_proj::test_proj::MovePlayer()
{
	Vector2D pos_modifers = {0, 0};
	int speed = 5;

	if (KeyPressed[SDL_SCANCODE_W]) {
		pos_modifers.y = -(speed);
	}
	if (KeyPressed[SDL_SCANCODE_S]) {
		pos_modifers.y = speed;
	}
	if (KeyPressed[SDL_SCANCODE_A]) {
		pos_modifers.x = -(speed);
	}
	if (KeyPressed[SDL_SCANCODE_D]) {
		pos_modifers.x = speed;
	}
	if (KeyPressed[SDL_SCANCODE_E]) {
		img1.angle += 3;
	}
	if (KeyPressed[SDL_SCANCODE_Q]) {
		img1.angle -= 3;
	}

	img1.rect.x += pos_modifers.x;
	img1.rect.y += pos_modifers.y;
}

void test_proj::test_proj::OnQuit()
{
	running = false;
}

void test_proj::test_proj::init()
{
	WRinit("Noobers", 1000, 700);

	bind_input(SDL_QUIT, bind(&test_proj::OnQuit, this));

	key_inputs = bind(&test_proj::MovePlayer, this);



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
	Update(txt);
	txt.pos.x = 500 - txt.GetTextSize().first / 2;
	txt.pos.y = 30;

	c1.pos = { 300, 300 };
	c1.r = 50;
}


void test_proj::test_proj::ep()
{
	init();

	//bg color
	WindowBgColor({ 175,175,175,225 });
	//bg color

	PrepUpdate({ img1, img2, txt, c1 });

	while (running) {

		SetFrameStart();

		//input
		gInput();
		//input

		//update
		RequestUpdate(img1);
		UpdateAll();
		img1.BoundingBox = Points::RotatePoints(img1.BoundingBox, img1.angle);
		if (img1.detectCollision(img2)) {
			img1.BoundingBox.color = {255, 0, 0, 255};
		}
		else {
			img1.BoundingBox.color = {0, 255, 0, 255};
		}
		c1.color = img1.BoundingBox.color;
		//update

	

		prep();

		//render stuff here
		Draw(img1);
		DrawBounds(img1);
		Draw(img2);
		Draw(txt);
		Draw(c1);
		DrawBounds(c1, {0, 0, 0, 255});
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
