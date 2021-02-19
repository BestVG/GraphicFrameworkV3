#include "GFW.h"

GFW::Text::FontManager GFW::Inst::fontManager = GFW::Text::FontManager();

SDL_Color GFW::GFW_GetRenderDrawColor(SDL_Renderer* renderer) {
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	return { r, g, b, a };
}

void GFW::GFW_SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

GFW::Inst::Inst()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		exit(1);
	}
	running = true;
	cout << "Done with SDL Init" << endl;

	FPS = 60;
	FrameDelay = 1000 / FPS;

	if (TTF_Init() == -1) {
		exit(1);
	}
	cout << "Done with TTF Init" << endl;
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
	GFW_SetRenderDrawColor(renderer, backgroundColor);
	SDL_RenderClear(renderer);
}

void GFW::Inst::pres()
{
	SDL_RenderPresent(renderer);
}

GFW::Image::Image GFW::Image::CreateImg(string img_path, SDL_Renderer* renderer)
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
	bool check1 = checkshape_SATalg(a, b);
	bool check2 = checkshape_SATalg(b, a);

	return check1 && check2;
}

//credit goes to javidx9
//this algoritim still has same bugs
bool GFW::Collision::checkshape_Dalg(Points::Points p1, Points::Points p2)
{
	for (int p = 0; p < p1.v.size(); p++) {
		FVector2D line_r1s;
		FVector2D line_r1e;
		line_r1s = p1.midp;
		line_r1e = p1.v[p];

		for (int q = 0; q < p2.v.size(); q++) {
			FVector2D line_r2s;
			FVector2D line_r2e;

			line_r2s = p2.v[q];
			line_r2e = p2.v[(q + 1) % p2.v.size()];


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

//credit goes to javidx9
//use this it works
bool GFW::Collision::checkshape_SATalg(Points::Points p1, Points::Points p2)
{
	for (int a = 0; a < p1.v.size(); a++)
	{
		int b = (a + 1) % p1.v.size();
		FVector2D axisProj = { static_cast<float>(-(p1.v[b].y - p1.v[a].y)), static_cast<float>(p1.v[b].x - p1.v[a].x) };
		float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
		axisProj = { axisProj.x / d, axisProj.y / d};

		float min_r1 = INFINITY, max_r1 = -INFINITY;
		for (int p = 0; p < p1.v.size(); p++)
		{
			float q = (p1.v[p].x * axisProj.x + p1.v[p].y * axisProj.y);
			min_r1 = min(min_r1, q);
			max_r1 = max(max_r1, q);
		}

		float min_r2 = INFINITY, max_r2 = -INFINITY;
		for (int p = 0; p < p2.v.size(); p++)
		{
			float q = (p2.v[p].x * axisProj.x + p2.v[p].y * axisProj.y);
			min_r2 = min(min_r2, q);
			max_r2 = max(max_r2, q);
		}

		if (!(max_r2 >= min_r1 && max_r1 >= min_r2)) {
			return false;
		}
	}
	return true;
}

void GFW::Points::Points::DrawBounds(SDL_Renderer* renderer)
{
	GFW_SetRenderDrawColor(renderer, color);

	SDL_Points p = v;

	SDL_RenderDrawLines(renderer, &p.points[0], p.points.size());

	//for (int i = 0; i < v.size() - 1; i++) {
		//SDL_RenderDrawLine(renderer, v[i].x, v[i].y, v[i + 1].x, v[i + 1].y);
	//}
	//SDL_RenderDrawLine(renderer, v[v.size() - 1].x, v[v.size() - 1].y, v[0].x, v[0].y);
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

void GFW::Image::Image::DrawImage(SDL_Renderer* renderer)
{
	SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
}

void GFW::Text::FontManager::LoadFont(string fontPath, int fsize, string fontName, int style)
{
	if (fontPath == "") {
		fontName = fontPath.substr(0, fontPath.find_last_of('.'));
	}

	string path = DefaultPath + fontPath;

	TTF_Font* font = TTF_OpenFont(path.c_str(), fsize);

	TTF_SetFontStyle(font, style);

	regF[fontName] = font;
}

void GFW::Text::Text::UpdateTexture(SDL_Renderer* renderer)
{
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
	}

	SDL_Surface* surface;

	surface = TTF_RenderText_Blended(font, msg.c_str(), color);

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	rect.x = pos.x;
	rect.y = pos.y;

	SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
}

void GFW::Text::Text::DrawString(SDL_Renderer* renderer)
{
	if (texture != nullptr) {
		SDL_RenderCopy(renderer, texture, NULL, &rect);
	}
}

pair<int, int> GFW::Text::Text::GetTextSize()
{
	int w, h;
	TTF_SizeText(font, msg.c_str(), &w, &h);
	return {w, h};
}

GFW::Points::Points GFW::Text::Text::GetBounds() {
	pair<int, int> size = GetTextSize();
	return Collision::GetRectBounds({ pos.x, pos.y, size.first, size.second });
};

bool GFW::Points::Polygon::detectCollision(Points points) {
	return Collision::detectCollision(GetBounds(), points);
}

bool GFW::Points::Polygon::detectCollision(Polygon& poly) {
	return Collision::detectCollision(GetBounds(), poly.GetBounds());
}

GFW::FVector2D& GFW::FVector2D::operator=(const Vector2D& vec) {
	x = static_cast<float>(vec.x);
	y = static_cast<float>(vec.y);
	return *this;
};

void GFW::Inst::DrawBounds(Points::Polygon& poly, SDL_Color color) {
	Points::Points points = poly.GetBounds();
	points.color = color;
	DrawBounds(points);
}


GFW::Points::SDL_Points::SDL_Points(const vector<Vector2D>& v)
{
	for (Vector2D p : v) {
		points.push_back({ p.x, p.y });
	}
	points.push_back(points[0]);
}

GFW::Points::SDL_Points& GFW::Points::SDL_Points::operator=(const vector<Vector2D>& v)
{
	points.clear();
	for (Vector2D p : v) {
		points.push_back({p.x, p.y});
	}

	points.push_back(points[0]);

	return *this;
}

void GFW::Circle::Circle::Update(SDL_Renderer* renderer)
{
	double inc = (M_PI / 180);
	double max = (M_PI * 2);

	double angle = 0;

	BoundingBox.midp = pos;
	BoundingBox.v.clear();
	while (angle < max) {
		BoundingBox.v.push_back({int( round( r * cos(angle) ) + pos.x), int(round(r * sin(angle)) + pos.y) });
		angle += inc;
	}

}
