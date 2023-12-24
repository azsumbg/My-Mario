#pragma once

#ifdef HELPER_EXPORTS
#define HELPER_API _declspec(dllexport)
#else 
#define HELPER_API _declspec(dllimport)
#endif

constexpr float scr_width = 1016.0;
constexpr float scr_height = 739.0f;

enum class dirs { stop = 0, up = 1, down = 2, left = 3, right = 4 };
enum class types {
	no_type = -1, mario = 0, turtle = 1, brick = 2, cloud1 = 3, cloud2 = 4, field = 5, goldbrick = 6,
	mountain1 = 7, mountain2 = 8, mountain3 = 9, mushroom = 10, coin = 11, bullet = 12
};
enum class return_type { R_OK = 0, R_FAIL = 1, R_NOT_SUPPORTED = 2, R_IN = 3, R_OUT = 4 };
enum class states { stop = 0, move = 1, jump_up = 2, jump_down = 3, hit = 4, run = 5, fall = 6 };
struct line_graph
{
	float slope = 0;
	float intercept = 0;
};

class HELPER_API ATOMS
{
	protected:
		float width = 0;
		float height = 0;
		
		line_graph GetYFromSlope(float _target_x, float _target_y) const
		{
			line_graph ret_graph;
			float slope = 0;
			float intercept = 0;
			
			if (_target_x - x == 0) _target_x--;

			slope = (_target_y - y) / (_target_x - x);
			intercept = y - (slope * x);

			ret_graph.slope = slope;
			ret_graph.intercept = intercept;

			return ret_graph;
		}

	public:
		float x = 0;
		float ex = 0;
		float y = 0;
		float ey = 0;
		
		float old_x = 0;
		float old_y = 0;

		
		dirs dir = dirs::stop;
		dirs prev_dir = dirs::stop;

		ATOMS(float _x, float _y, float _width, float _height)
		{
			x = _x;
			y = _y;
			width = _width;
			height = _height;
			ex = x + width;
			ey = y + height;
		}

		virtual ~ATOMS() {};

		void SetDims()
		{
			ex = x + width;
			ey = y + height;
		}

		void NewDims(float _width, float _height)
		{
			width = _width;
			height = _height;
			ex = x + width;
			ey = y + height;
		}

		virtual void Release()
		{
			delete this;
		}
};

class HELPER_API INTERFACES :public ATOMS
{
	protected:
		int count = 0;
		int frame_count = 0;
		int frame_delay = 5;


	public:
		types type = types::no_type;
		states state = states::stop;
		float speed = 0;
		
		INTERFACES(float _x, float _y, float _width = 1.0f, float _height = 1.0f, types _type = types::no_type) 
			:ATOMS(_x, _y, _width, _height)
		{
			type = _type;
			count++;
		}

		virtual ~INTERFACES()
		{
			count--;
		};

		int GetCount() const
		{
			return count;
		}

		virtual return_type Move() = 0;
		virtual return_type Jump(float targ_x, float targ_y) = 0;
		virtual return_type Transform(types _in_what) = 0;
		virtual int GetFrame() = 0;

		virtual void Release() = 0;
};

typedef INTERFACES* obj_ptr;

extern HELPER_API obj_ptr iCreate(types _what, float where_x, float where_y);