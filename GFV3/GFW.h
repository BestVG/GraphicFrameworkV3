#pragma once
#include "include.h"

namespace GFW {

	SDL_Color GFW_GetRenderDrawColor(SDL_Renderer* renderer);
	void GFW_SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color);

	class Inst;


	struct Vector2D { int x, y; };
	struct FVector2D {
		float x, y;
		FVector2D() : x(0), y(0) {}
		FVector2D(float x, float y) : x(x), y(y) {}
		FVector2D(const Vector2D& vec) : x(static_cast<float>(vec.x)), y(static_cast<float>(vec.y)) {}
		FVector2D& operator=(const Vector2D& vec);
	};

	class Drawable {
	public:
		virtual void Draw(SDL_Renderer* renderer) = 0;
	};

	class Updatable {
	public:
		void Update(SDL_Renderer* renderer) { needsUpdate = false; DoUpdate(renderer); };
		bool NeedsUpdate() { return needsUpdate; }
		void RequestUpdate() { needsUpdate = true; }
	protected:
		virtual void DoUpdate(SDL_Renderer* renderer) = 0;
	private:
		bool needsUpdate = true;
	};

	namespace Points {

		struct SDL_Points {
			SDL_Points(const vector<Vector2D>& v);
			vector<SDL_Point> points;
			SDL_Points& operator=(const vector<Vector2D>& v);
		};

		struct Points : Drawable {
			vector<Vector2D> v;
			Vector2D midp;
			SDL_Color color;
			void DrawBounds(SDL_Renderer* renderer);
			void Draw(SDL_Renderer* renderer) { DrawBounds(renderer); }
		};

		class Polygon {
		public:
			virtual Points GetBounds() = 0;
			bool detectCollision(Points points);
			bool detectCollision(Polygon& poly);
		};

		Vector2D RotatePoint(Vector2D origin, Vector2D orginal_point, double angle);
		Points RotatePoints(Points p, double angle);
	}


	namespace Collision {
		bool detectCollision(Points::Points a, Points::Points b);
		bool checkshape_Dalg(Points::Points a, Points::Points b);
		bool checkshape_SATalg(Points::Points p1, Points::Points p2);
		Points::Points GetRectBounds(SDL_Rect rect);
		namespace Ray {
			typedef pair<Vector2D, Vector2D> Ray;
		}
	}

	namespace Image {
		struct Image : Points::Polygon, Drawable, Updatable {
			SDL_Rect rect;
			SDL_Texture* texture;
			Points::Points BoundingBox;
			double angle = 0.0;

			void UpdateBoundingBoxDefault() { BoundingBox = Collision::GetRectBounds(rect); }
			void SetPos(Vector2D pos) { rect.x = pos.x; rect.y = pos.y; }
			void DrawImage(SDL_Renderer* renderer);
			void SetSize(pair<int, int> s) { rect.w = s.first; rect.h = s.second; }
			int GetX() { return rect.x; }
			int GetY() { return rect.y; }
			int GetW() { return rect.w; }
			int GetH() { return rect.h; }
			Points::Points GetBounds() { return BoundingBox; }
			void Draw(SDL_Renderer* renderer) { DrawImage(renderer); }
			void DoUpdate(SDL_Renderer* renderer) { UpdateBoundingBoxDefault(); }
		};
		Image CreateImg(string img_path, SDL_Renderer* renderer);
	};

	namespace Text {

		class FontManager {
		public:
			FontManager() { DefaultPath = "./Fonts/"; }
			
			void LoadFont(string fontPath, int fsize, string fontName = "", int style = TTF_STYLE_NORMAL);
			void SetDefaultPath(string path) { DefaultPath = path; }
			TTF_Font* GetFont(string fontName) { return regF[fontName]; }
			TTF_Font* operator[](string fontName) { return GetFont(fontName);  }

		private:
			map<string, TTF_Font*> regF;
			string DefaultPath;
		};

		struct Text : Points::Polygon, Drawable, Updatable {
			string msg;
			Vector2D pos;
			TTF_Font* font;
			SDL_Color color = { 0, 0, 0, 255 };
			SDL_Rect rect;
			SDL_Texture* texture = nullptr;
			void UpdateTexture(SDL_Renderer* renderer);
			void DrawString(SDL_Renderer* renderer);
			pair<int, int> GetTextSize();
			Points::Points GetBounds();
			void Draw(SDL_Renderer* renderer) { DrawString(renderer); }
			void DoUpdate(SDL_Renderer* renderer) { UpdateTexture(renderer); }
		};

	}


	namespace Circle {
		struct Circle : Points::Polygon, Drawable, Updatable {
			Vector2D pos;
			int r;
			SDL_Color color = { 0, 0, 0, 255 };
			Points::Points BoundingBox;
			Points::Points GetBounds() { return BoundingBox; }
			void Draw(SDL_Renderer* renderer);
			void DoUpdate(SDL_Renderer* renderer);
		};
	};

	class Inst {
	public:
		Inst();

		void WRinit(string winT, int sw, int sh);

		void gerror_print();
		
		//do input stuff

		//bind(&test_proj::ex1, this) param 2
		void bind_input(SDL_EventType binding_event, function<void(void)> func);


		void unbind_input(SDL_EventType binding_event);

		void gInput();

		void pInput(SDL_Event e);
		//do input stuff
		
		//rendering stuff

		void prep();


		void pres();

		void DrawBounds(Points::Points bounds) { bounds.DrawBounds(renderer); };
		void DrawBounds(Points::Polygon& poly) { DrawBounds(poly.GetBounds()); };
		void DrawBounds(Points::Polygon& poly, SDL_Color color);
		void Draw(Drawable& drawable) { drawable.Draw(renderer); };
		void Update(Updatable& updatable) { if(updatable.NeedsUpdate()) updatable.Update(renderer); };
		void RequestUpdate(Updatable& updatable) { updatable.RequestUpdate(); }
		void PrepUpdate(Updatable& updatable) { queuedUpdates.push_back(&updatable); };
		void PrepUpdate(vector<Updatable*> updatables) { for(Updatable* updatable: updatables) queuedUpdates.push_back(updatable); };
		void UpdateAll() { for (Updatable* updatable : queuedUpdates) Update(*updatable); }

		void WindowBgColor(SDL_Color c) { backgroundColor = c; };

		//rendering stuff


		//entry point this acts as your main function
		virtual void ep() {};


		//frame_rate stuff
		void SetFrameStart() { FrameStart = SDL_GetTicks(); };
		void Delay();

		//frame_rate stuff

	protected:
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Event e;

		vector<pair<SDL_EventType, function<void(void)>>> inputs;
		bool running;
		int FPS;
		int FrameDelay;
		Uint32 FrameStart;
		int FrameTime;
		SDL_Color backgroundColor;
		vector<Updatable*> queuedUpdates;
		static Text::FontManager fontManager;
	};
	
	

}