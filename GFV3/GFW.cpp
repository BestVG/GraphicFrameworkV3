#include "GFW.h"

GFW::Inst::Inst()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		exit(1);
	}
	running = true;
	cout << "Done with SDL Init" << endl;

	FPS = 60;
	FrameDelay = 100 / FPS;
}

void GFW::Inst::WRinit(string winT, int sw, int sh)
{
	window = SDL_CreateWindow(winT.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sw, sh, 0);
	
	if (!window) {
		gerror_print();
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer) {
		gerror_print();
		exit(1);
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void GFW::Inst::gerror_print()
{
	cout << "error: " << SDL_GetError() << endl;
}

void GFW::Inst::bind_input(SDL_EventType binding_event, function<void(void)> func)
{
	inputs.push_back({ binding_event, func });
}

void GFW::Inst::unbind_input(SDL_EventType binding_event)
{
	int event_index = -1;
	for (unsigned int i = 0; i < inputs.size(); i++) {
		if (inputs[i].first == binding_event) {
			event_index = i;
			break;
		}
	}

	if (event_index != -1) {
		inputs.erase(inputs.begin() + event_index);
	}
	else {
		cout << "no input exists" << endl;
	}

}

void GFW::Inst::gInput()
{
	if (SDL_PollEvent(&e)) {
		pInput(e);
	}
}

void GFW::Inst::pInput(SDL_Event e)
{
	for (pair<int, function<void(void)>> input : inputs) {

		if (e.type == input.first) {
			input.second();
		}
	}
}

void GFW::Inst::prep()
{
	SDL_RenderClear(renderer);
}

void GFW::Inst::pres()
{
	SDL_RenderPresent(renderer);
}

GFW::Image GFW::Inst::CreateImg(string img_path)
{
	Image img;
	SDL_Surface* temp = IMG_Load(img_path.c_str());

	img.texture = SDL_CreateTextureFromSurface(renderer, temp);

	SDL_FreeSurface(temp);

	int width, height;

	SDL_QueryTexture(img.texture, nullptr, nullptr, &width, &height);

	img.rect.w = width;
	img.rect.h = height;


	return img;
}

void GFW::Inst::Delay()
{
	FrameTime = SDL_GetTicks() - FrameStart;

	if (FrameDelay > FrameTime) {
		SDL_Delay(FrameDelay - FrameTime);
	}
}


GFW::Points::Points GFW::Collision::GetRectBounds(SDL_Rect rect)
{
	Points::Points ret;

	ret.v.push_back({rect.x, rect.y});
	ret.v.push_back({rect.x + rect.w, rect.y});
	ret.v.push_back({rect.x + rect.w, rect.y + rect.h});
	ret.v.push_back({rect.x, rect.y + rect.h});
	ret.midp.x = rect.x + (rect.w / 2);
	ret.midp.y = rect.y + (rect.h / 2);

	return ret;
}



bool GFW::Collision::detectCollision(Points::Points a, Points::Points b)
{
	future<bool> check1 = async(checkshape, a, b);
	future<bool> check2 = async(checkshape, b, a);

	if (check1.get() || check2.get()) {
		return true;
	}

	return false;
}

//credit goes to javidx9
bool GFW::Collision::checkshape(Points::Points p1, Points::Points p2)
{
	for (int p = 0; p < p1.v.size(); p++) {
		Vector2D line_r1s = p1.midp;
		Vector2D line_r1e = p1.v[p];

		for (int q = 0; q < p2.v.size(); q++) {
			Vector2D line_r2s = p2.v[q];
			Vector2D line_r2e = p2.v[(q + 1) % p2.v.size()];


			float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
			float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
			float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

			if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
			{
				return true;
			}


		}


	}
	return false;
}

void GFW::Collision::DrawCollisionBounds(Points::Points bounds, SDL_Color color, SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (int i = 0; i < bounds.v.size() - 1; i++) {
		SDL_RenderDrawLine(renderer, bounds.v[i].x, bounds.v[i].y, bounds.v[i + 1].x, bounds.v[i + 1].y);
	}
	SDL_RenderDrawLine(renderer, bounds.v[bounds.v.size() - 1].x, bounds.v[bounds.v.size() - 1].y, bounds.v[0].x, bounds.v[0].y);
}
 
GFW::Vector2D GFW::Points::RotatePoint(Vector2D origin, Vector2D orginal_point, double angle)
{
	Vector2D newpoint;

	angle = (angle) * (M_PI / 180);
	newpoint.x = round(cos(angle) * (orginal_point.x - origin.x) - sin(angle) * (orginal_point.y - origin.y) + origin.x);
	newpoint.y = round(sin(angle) * (orginal_point.x - origin.x) + cos(angle) * (orginal_point.y - origin.y) + origin.y);

	return newpoint;
}

GFW::Points::Points GFW::Points::RotatePoints(Points p, double angle)
{
	Points newpoints;
	newpoints.midp = p.midp;

	for (Vector2D point : p.v) {
		newpoints.v.push_back(RotatePoint(p.midp, point, angle));
	}

	return newpoints;
}

void GFW::Image::DrawImage(SDL_Renderer* renderer)
{
	SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
}
