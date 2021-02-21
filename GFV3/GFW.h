#pragma once
#include "include.h"

/**
 *  \brief A common namespace for all GraphicFramework classes and functions
 */
namespace GFW {

	/**
	 *  \brief Retrieves the color used for drawing operations
	 * 
	 *  \param renderer the renderer to query
	 * 
	 *  \return the current draw color of the given ::SDL_Renderer as a ::SDL_Color
	 * 
	 *  \sa GFW_SetRenderDrawColor()
	 */
	SDL_Color GFW_GetRenderDrawColor(SDL_Renderer* renderer);

	/**
	 *  \brief Sets the color used for drawing operations
	 * 
	 *  \param renderer the renderer on which to set the color
	 * 
	 *  \param color the ::SDL_Color to use as the new drawing color
	 * 
	 *  \sa GFW_GetRenderDrawColor()
	 */
	void GFW_SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color);

	/**
	 *  \brief Auto-converts between objects and their respective, pointer types
	 */
	template<typename T>
	class Pointer {
	public:
		Pointer(const T& t): p(const_cast<T*>(&t)) {}
		Pointer& operator=(const T& t) { p = &t; }
		operator T*() { return p; }
	private:
		T* p;
	};

	class Inst;


	struct Vector2D { int x, y; };
	struct FVector2D {
		float x, y;
		FVector2D() : x(0), y(0) {}
		FVector2D(float x, float y) : x(x), y(y) {}
		FVector2D(const Vector2D& vec) : x(static_cast<float>(vec.x)), y(static_cast<float>(vec.y)) {}
		FVector2D& operator=(const Vector2D& vec);
	};

	/**
	 *  \brief A common interface for classes which support drawing of themselves onto a ::SDL_Renderer
	 */
	class Drawable {
	public:
		/**
		 *  \brief Draws this object onto the given renderer
		 * 
		 *  \param renderer the renderer on which to draw
		 */
		virtual void Draw(SDL_Renderer* renderer) = 0;
	};

	/**
	 *  \brief A common interface for classes which require optional updating of their member values
	 */
	class Updatable {
	public:
		/**
		 *  \brief Updates the member values of this object and marks this object as updated
		 * 
		 *  \param renderer a ::SDL_Renderer representing the current rendering context
		 * 
		 *  \sa NeedsUpdate()
		 */
		void Update(SDL_Renderer* renderer) { needsUpdate = false; DoUpdate(renderer); };

		/**
		 *  \return Whether this object has requested that that its Update function be called
		 * 
		 *  \sa RequestUpdate()
		 *  \sa Update()
		 */
		bool NeedsUpdate() { return needsUpdate; }
		
		/**
		 *  \brief Requests that this object be updated when applicable
		 * 
		 *  \sa Update()
		 *  \sa NeedsUpdate()
		 */
		void RequestUpdate() { needsUpdate = true; }
	protected:
		/**
		 *  \brief Preforms the required updates on this object's member values
		 * 
		 *  \param renderer a ::SDL_Renderer representing the current rendering context
		 * 
		 *  \sa Update()
		 */
		virtual void DoUpdate(SDL_Renderer* renderer) = 0;
	private:
		bool needsUpdate = true;
	};

	/**
	 *  \brief Contains classes and functions for storing and working with points and shapes
	 */
	namespace Points {

		/**
		 *  \brief Represents a collection of ::SDL_Point objects and performs automatic coversion from vectors of ::Vector2D object
		 */
		struct SDL_Points {
			SDL_Points(const vector<Vector2D>& v);
			vector<SDL_Point> points;
			SDL_Points& operator=(const vector<Vector2D>& v);
		};

		/**
		 *  \brief Represents a collection of ::Vector2D objects which form the bouning box of an object
		 */
		struct Points : Drawable {
			/**
			 *  \brief A vector containing points which define the vertices of a shape
			 */
			vector<Vector2D> v;
			/**
			 *  \brief A Vector2D object representing the midpoint of a shape
			 */
			Vector2D midp;
			/**
			 *  \brief A ::SDL_Color which will be used to draw these points if requested
			 */
			SDL_Color color;

			Points() {}
			Points(vector<Vector2D> v, Vector2D midp) : v(v), midp(midp) {}
			Points(vector<Vector2D> v, Vector2D midp, SDL_Color color) : v(v), midp(midp), color(color) {}
			Points(const SDL_Rect& rect);
			Points(const SDL_Rect& rect, const SDL_Color& c) : Points(rect) { color = c; }
			Points(const Vector2D& vec);
			Points(const Vector2D& vec, const SDL_Color& c) : Points(vec) { color = c; }
			Points& operator=(const SDL_Rect& rect);
			Points& operator=(const Vector2D& vec);

			/**
			 *  \brief Draws these points onto the given renderer using the color stored in color
			 * 
			 *  \param renderer the ::SDL_Renderer on which to draw these points
			 * 
			 *  \sa color
			 */
			void Draw(SDL_Renderer* renderer);
		};

		/**
		 *  \brief Represents a convex polygon which is defined by a ::Points::Points object
		 */
		class Polygon {
		public:
			/**
			 *  \return A Points object representing the bounding box of this Polygon
			 */
			virtual Points GetBounds() = 0;

			/**
			 *  \brief detects if this polygon intersects the polygon formed by the given ::Points::Points object
			 */
			bool detectCollision(Points points);

			/**
			 *  \brief detects if this polygon intersects the given Polygon
			 */
			bool detectCollision(Polygon& poly);
		};

		/**
		 *  \brief Rotates a Vector2D around a specified origin Vector2D
		 * 
		 *  \param origin the origin point around which to rotate
		 *  \param orginal_point the point to rotate
		 *  \param angle the angle (in degrees) to rotate the point (counter-clockwise)
		 * 
		 *  \return the rotated point
		 * 
		 *  \sa RotatePoints()
		 */
		Vector2D RotatePoint(Vector2D origin, Vector2D orginal_point, double angle);

		/**
		 *  \brief Rotates the given Points around their midpoint
		 *
		 *  \param p the Points to rotate
		 *  \param angle the angle (in degrees) to rotate the Points (counter-clockwise)
		 *
		 *  \return the rotated Points
		 *
		 *  \sa RotatePoint()
		 */
		Points RotatePoints(Points p, double angle);

		/**
		 *  \param rect the SDL_Rect from which to get the bounds
		 * 
		 *  \return a Points object based on the bounds of the given SDL_Rect
		 */
		Points GetRectBounds(SDL_Rect rect);
	}

	/**
	 *  \brief Contains functions for detecting collisions between Points
	 *  
	 *  \sa Points
	 */
	namespace Collision {
		/**
		 *  \return whether the two Points objects intercect
		 */
		bool detectCollision(Points::Points a, Points::Points b);

		/**
		 *  \return whether the two Points objects intercect using the DIAG Algorithm
		 */
		bool checkshape_Dalg(Points::Points a, Points::Points b);

		/**
		 *  \return whether the two Points objects intercect using the SAT Algorithm
		 */
		bool checkshape_SATalg(Points::Points p1, Points::Points p2);
	}

	/**
	 *  \brief Contains classes and functions for working with images
	 */
	namespace Image {
		/**
		 *  \brief Represents an image
		 */
		struct Image : Points::Polygon, Drawable, Updatable {
			/**
			 *  \brief A SDL_Rect representing the rectangle containing this image
			 */
			SDL_Rect rect;
			/**
			 *  \brief A SDL_Texture storing the data contained by this image
			 */
			SDL_Texture* texture;
			/**
			 *  \brief A Points object representing the bounds of this image
			 */
			Points::Points BoundingBox;
			/**
			 *  \brief An angle which this image will be rotated by when being rendered
			 */
			double angle = 0.0;

			/**
			 *  \brief Updates the rect member of this image to move its top-left corner to the given position
			 * 
			 *  \param pos the new position of the top-left corner
			 * 
			 *  \sa GetX()
			 *  \sa GetY()
			 */
			void SetPos(Vector2D pos) { rect.x = pos.x; rect.y = pos.y; }

			/**
			 *  \brief Sets the width and height of this image
			 *
			 *  \param s a std::pair of the form { newWidth, newHeight }
			 * 
			 *  \sa GetW()
			 *  \sa GetH()
			 */
			void SetSize(pair<int, int> s) { rect.w = s.first; rect.h = s.second; }

			/**
			 *  \return the X position of the top-left corner where this image will be rendered
			 * 
			 *  \sa SetPos()
			 */
			int GetX() { return rect.x; }

			/**
			 *  \return the Y position of the top-left corner where this image will be rendered
			 * 
			 *  \sa SetPos()
			 */
			int GetY() { return rect.y; }

			/**
			 *  \return the width of this image
			 *
			 *  \sa SetSize()
			 */
			int GetW() { return rect.w; }

			/**
			 *  \return the height of this image
			 *
			 *  \sa SetSize()
			 */
			int GetH() { return rect.h; }

			Points::Points GetBounds() { return BoundingBox; }

			/**
			 *  \brief Draws this image at the position specified by rect and with the given angle onto the given renderer
			 *
			 *  \param renderer the SDL_Renderer on which to draw this image
			 */
			void Draw(SDL_Renderer* renderer);

			/**
			 *  \brief Updates the bounding box of this image to correspond to its rect member value
			 */
			void DoUpdate(SDL_Renderer* renderer) { BoundingBox = rect; }
		};

		/**
		 *  \brief creates an image from the given file path
		 * 
		 *  \param img_path a path to the file from which to load the image
		 *  \param renderer a SDL_Renderer representing the current rendering context
		 * 
		 *  \return an Image representing the image stored in the given file
		 */
		Image CreateImg(string img_path, SDL_Renderer* renderer);
	}


	/**
	 *  \brief Contains classes and functions for working with text
	 */
	namespace Text {

		/**
		 *  \brief Manages the loading and storage of fonts
		 */
		class FontManager {
		public:
			FontManager() { DefaultPath = "./Fonts/"; }

			/**
			 *  \brief Loads the given font from a file
			 * 
			 *  \param fontPath the path to the font file relative to DefaultPath
			 *  \param fsize the requested size of the loaded font
			 *  \param fontName a user-defined name used to access the font (if unspecified will be fontPath - .ttf)
			 *  \param style the requested style of the loaded font (should be flags of the type TTF_STYLE_*)
			 * 
			 *  \sa GetFont()
			 *  \sa UnloadFont()
			 */
			void LoadFont(string fontPath, int fsize, string fontName = "", int style = TTF_STYLE_NORMAL);

			/**
			 *  \brief Sets the root font path (DefaultPath)
			 * 
			 *  \param path the new path
			 */
			void SetDefaultPath(string path) { DefaultPath = path; }

			/**
			 *  \brief Unloads the given font from SDL_TTF
			 * 
			 *  \param fontName the fontName of the font to unload
			 * 
			 *  \sa LoadFont()
			 */
			void UnloadFont(string fontName) { TTF_CloseFont(GetFont(fontName)); regF.erase(fontName); }
			
			/**
			 *  \return the font with the given fontName
			 * 
			 *  \sa LoadFont()
			 */
			TTF_Font* GetFont(string fontName) { return regF[fontName]; }

			/**
			 *  \return the font with the given fontName
			 *
			 *  \sa LoadFont()
			 */
			TTF_Font* operator[](string fontName) { return GetFont(fontName);  }

		private:
			map<string, TTF_Font*> regF;
			string DefaultPath;
		};

		/**
		 *  \brief Represents a renderable string of text
		 */
		struct Text : Points::Polygon, Drawable, Updatable {
			/**
			 *  \brief The text which will be rendered
			 */
			string msg;
			/**
			 *  \brief A Vector2D representing the top-left corner of where the text will be rendered
			 */
			Vector2D pos;
			/**
			 *  \brief The font which will be used to render the text
			 */
			TTF_Font* font;
			/**
			 *  \brief The color in which the text will be rendered
			 */
			SDL_Color color = { 0, 0, 0, 255 };
			/**
			 *  \brief A pair representing the width and height of this text object of the form {width, height}
			 */
			pair<int, int> size;
			/**
			 *  \brief A SDL_Texture containing the rendered text
			 */
			SDL_Texture* texture = nullptr;
			/**
			 *  \brief The angle at which the text will be rendered
			 */
			double angle = 0;

			/**
			 *  \return The width and hight of this Text as a std::pair of the form { width, height }
			 */
			pair<int, int> GetTextSize() { return size; }

			Points::Points GetBounds();

			/**
			 *  \brief Renders this Text onto the given renderer
			 *
			 *  \param renderer the SDL_Renderer on which to render this Text
			 */
			void Draw(SDL_Renderer* renderer);

			/**
			 *  \brief Re-renderes the text onto the texture member
			 *
			 *  \param renderer a SDL_Renderer representing the current rendering context
			 */
			void DoUpdate(SDL_Renderer* renderer);
		};

	}

	/**
	 *  \brief Contains classes for working with circles
	 */
	namespace Circle {
		/**
		 *  \brief Represents a circle
		 */
		struct Circle : Points::Polygon, Drawable, Updatable {
			/**
			 *  \brief A Vector2D representing the center of this Circle
			 */
			Vector2D pos;
			/**
			 *  \brief The radius of this Circle
			 */
			int r;
			/**
			 *  \brief The SDL_Color in which this Circle will be rendered
			 */
			SDL_Color color = { 0, 0, 0, 255 };
			/**
			 *  \brief A Points object containing a polygon which approximates the bounding box of the Circle
			 */
			Points::Points BoundingBox;

			Points::Points GetBounds() { return BoundingBox; }

			/**
			 *  \brief Draws this circle onto the given renderer
			 *
			 *  \param renderer the SDL_Renderer on which to render this circle
			 */
			void Draw(SDL_Renderer* renderer);

			/**
			 *  \brief Updates the BoundingBox of this circle to reflect the current position and radius
			 */
			void DoUpdate(SDL_Renderer* renderer);
		};
	}


	/**
	 *  \brief A class representing a window instance
	 */
	class Inst {
	public:
		Inst();

		/**
		 *  \brief Initializes this instance by creating the window and renderer
		 * 
		 *  \param winT the title of the window
		 *  \param sw the width of the window
		 *  \param sh the height of the window
		 */
		void WRinit(string winT, int sw, int sh);

		/**
		 *  \brief prints the last SDL_Error to cerr
		 */
		void gerror_print();
		
		//do input stuff

		/**
		 *  \brief Requests that the given function be called when the given event type is recieved (ex. bind_input(SDL_QUIT, bind(&example_project::OnQuit, this));)
		 * 
		 *  \param binding_event the event type of which to be notified
		 *  \param func the function to call
		 */
		void bind_input(SDL_EventType binding_event, function<void(void)> func); 

		/**
		 *  \brief Unbinds all ballback functions of the given event type
		 * 
		 *  \param binding_event the event type to unbind
		 */
		void unbind_input(SDL_EventType binding_event);

		/**
		 *  \brief Polls the SDL_Event queue and calls pInput with the result (if any)
		 */
		void gInput();

		/**
		 *  \brief Processes the given SDL_Event by notifying all bound event handlers
		 * 
		 *  \param e the event to process
		 */
		void pInput(SDL_Event e);
		//do input stuff
		
		//rendering stuff

		/**
		 *  \brief Prepares the rendering enviornment for the start of a rendering cycle by, among other things, clearing the renderer
		 */
		void prep();

		/**
		 *  \brief Completes a rendering cycle by, among other things, calling SDL_RenderPresent to render the frame
		 */
		void pres();


		/**
		 *  \brief Draws the polygon formed by the given Points object
		 * 
		 *  \param bounds the Points to draw
		 */
		void DrawBounds(Points::Points bounds) { bounds.Draw(renderer); };

		/**
		 *  \brief Draws the outline of the given Polygon
		 * 
		 *  \param poly the Polygon to draw
		 */
		void DrawBounds(Points::Polygon& poly) { DrawBounds(poly.GetBounds()); };

		/**
		 *  \brief Draws the outline of the given Polygon
		 *
		 *  \param poly the Polygon to draw
		 *  \param color the color in which to draw the polygon
		 */
		void DrawBounds(Points::Polygon& poly, SDL_Color color);

		/**
		 *  \brief Calls the draw function on the given Drawable with the current renderer
		 * 
		 *  \param drawable the Drawable to draw
		 */
		void Draw(Drawable& drawable) { drawable.Draw(renderer); };

		/**
		 *  \brief Calls the Update function on the given Updatable if its NeedsUpdate function returns true
		 * 
		 *  \param updatable the Updatable to update
		 */
		void Update(Updatable& updatable) { if(updatable.NeedsUpdate()) updatable.Update(renderer); };

		/**
		 *  \brief Calls the RequestUpdate function of the given Updatable
		 * 
		 *  \param updatable the Updatable on which to request an update
		 */
		void RequestUpdate(Updatable& updatable) { updatable.RequestUpdate(); }

		/**
		 *  \brief Adds the given Updatable to be passed to Update when UpdateAll is called
		 * 
		 *  \param updatable the Updatable to add
		 * 
		 *  \sa Update()
		 *  \sa UpdateAll()
		 *  \sa StopUpdate()
		 */
		void PrepUpdate(Updatable& updatable) { queuedUpdates.push_back(&updatable); };

		/**
		 *  \brief Adds the given Updatables to be passed to Update when UpdateAll is called
		 *
		 *  \param updatables the Updatables to add
		 *
		 *  \sa Update()
		 *  \sa UpdateAll()
		 *  \sa StopUpdate()
		 */
		void PrepUpdate(vector<Pointer<Updatable>> updatables) { for(Updatable* updatable: updatables) PrepUpdate(*updatable); };

		/**
		 *  \brief Calls Update with all Updatables passed to PrepUpdate
		 * 
		 *  \sa Update()
		 *  \sa PrepUpdate()
		 *  \sa StopUpdate()
		 */
		void UpdateAll() { for (Updatable* updatable : queuedUpdates) Update(*updatable); }

		/**
		 *  \brief Stops the given Updatable from being passed to Update by UpdateAll
		 * 
		 *  \param updatable the Updatable to remove
		 * 
		 *  \sa Update()
		 *  \sa PrepUpdate()
		 *  \sa UpdateAll()
		 */
		void StopUpdate(Updatable& updatable) { queuedUpdates.erase(find(queuedUpdates.begin(), queuedUpdates.end(), &updatable)); }

		/**
		 *  \brief Stops the given Updatables from being passed to Update by UpdateAll
		 *
		 *  \param updatables the Updatables to remove
		 *
		 *  \sa Update()
		 *  \sa PrepUpdate()
		 *  \sa UpdateAll()
		 */
		void StopUpdate(vector<Pointer<Updatable>> updatables) { for (Updatable* updatable : updatables) StopUpdate(*updatable); }

		/**
		 *  \brief Sets the window background color
		 * 
		 *  \param c the new background color
		 */
		void WindowBgColor(SDL_Color c) { backgroundColor = c; };

		//rendering stuff


		//entry point this acts as your main function
		/**
		 *  \brief Represents an optional entrypoint function for the program
		 */
		virtual void ep() {};


		//frame_rate stuff
		/** 
		 *  \brief Marks the start of a new frame by loading SDL_GetTicks into FrameStart
		 */
		void SetFrameStart() { FrameStart = SDL_GetTicks(); };

		/**
		 *  \brief Performs a delay (if applicable) to ensure that the frame is updated at the rate specified in FPS
		 */
		void Delay();

		//frame_rate stuff

	protected:
		/**
		 *  \brief The SDL_Window which is controlled by this Inst
		 */
		SDL_Window* window;
		/**
		 *  \brief A SDL_Renderer which can be used to draw onto the window
		 */
		SDL_Renderer* renderer;
		/**
		 *  \brief A temporary storage variable for SDL_Events
		 */
		SDL_Event e;
		/**
		 *  \brief A vector of callback functions responsible for handling SDL_Events
		 */
		vector<pair<SDL_EventType, function<void(void)>>> inputs;
		/**
		 *  \brief A function which processes key inputs
		 */
		function<void(void)> key_inputs;
		/**
		 *  \brief Whether the program is running
		 */
		bool running;
		/**
		 *  \brief The FPS at which to update the frame
		 * 
		 *  \sa Delay()
		 */
		int FPS;
		/**
		 *  \brief The delay (in milliseconds) between frames required to maintain the given FPS
		 */
		int FrameDelay;
		/**
		 *  \brief The start time (from SDL_GetTicks) of the last rendering frame 
		 * 
		 *  \sa SetFrameStart()
		 */
		Uint32 FrameStart;
		/**
		 *  \brief The keyboard state
		 */
		const Uint8* KeyPressed;
		/**
		 *  \brief The elapsed time of the last render frame
		 */
		int FrameTime;
		/**
		 *  \brief The background color of the frame
		 */
		SDL_Color backgroundColor;
		/**
		 *  \brief The Updatables which will be passed to Update when UpdateAll is called
		 */
		vector<Updatable*> queuedUpdates;
		/**
		 *  \brief A common instance of FontManager for all instances
		 */
		static Text::FontManager fontManager;
	};

}
