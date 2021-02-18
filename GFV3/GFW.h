#pragma once
#include "include.h"

namespace GFW {

	class Inst;


	struct Vector2D { int x, y; };
	struct FVector2D {
		float x, y;
		void convertVector2D(Vector2D vec) { x = static_cast<float>(vec.x); y = static_cast<float>(vec.y); };
	};

	namespace Points {
		struct Points {
			vector<Vector2D> v;
			Vector2D midp;
		};

		Vector2D RotatePoint(Vector2D origin, Vector2D orginal_point, double angle);
		Points RotatePoints(Points p, double angle);
	}


	namespace Collision {
		bool detectCollision(Points::Points a, Points::Points b);
		bool checkshape_Dalg(Points::Points a, Points::Points b);
		bool checkshape_SATalg(Points::Points p1, Points::Points p2);
		void DrawCollisionBounds(Points::Points bounds, SDL_Color color, SDL_Renderer* renderer);
		Points::Points GetRectBounds(SDL_Rect rect);
		namespace Ray {
			typedef pair<Vector2D, Vector2D> Ray;
		}
	}

	namespace Image {
		struct Image {
			SDL_Rect rect;
			SDL_Texture* texture;
			Points::Points BoundingBox;
			double angle = 0.0;

			void UpdateBoundingBoxDefault() { BoundingBox = Collision::GetRectBounds(rect); };
			void SetPos(Vector2D pos) { rect.x = pos.x; rect.y = pos.y; };
			void DrawImage(SDL_Renderer* renderer);
			void SetSize(pair<int, int> s) { rect.w = s.first; rect.h = s.second; };
			int GetX() { return rect.x; };
			int GetY() { return rect.y; };
			int GetW() { return rect.w; };
			int GetH() { return rect.h; };
		};
		Image CreateImg(string img_path, SDL_Renderer* renderer);
	};

	namespace Text {

		class FontManager {
		public:
			FontManager() { DefaultPath = "./Fonts/"; };
			
			void LoadFont(string fn, int fsize, string ckey = "", int style = TTF_STYLE_NORMAL);
			void SetDefaultPath(string path) { DefaultPath = path; };
			TTF_Font* GetFont(string key) { return regF[key]; };

		private:
			map<string, TTF_Font*> regF;
			string DefaultPath;
		};

		struct Text {
			string msg;
			Vector2D pos;
			TTF_Font* font;
			SDL_Color color = { 0, 0, 0, 255 };
			void DrawString(SDL_Renderer* renderer);
			pair <int, int> GetTextSize();
		};

	}


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


		

		void WindowBgColor(SDL_Color color) { SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); };

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
		Text::FontManager fm;
	};
	
	

}
