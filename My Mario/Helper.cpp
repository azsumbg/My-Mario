#include "pch.h"
#include "framework.h"
#include "Helper.h"

class CLOUDS :public INTERFACES
{
	public:

		CLOUDS(types __what, float _where_x, float _where_y) :INTERFACES(_where_x, _where_y, 1.0f, 1.0f, __what)
		{
			switch (__what)
			{
			case types::cloud1:
				NewDims(100.0f, 53.0f);
				speed = 0.5f;
				break;

			case types::cloud2:
				NewDims(80.0f, 42.0f);
				speed = 0.5f;
				break;
			}

			dir = dirs::left;
		}

		return_type Move() override
		{
			switch (dir)
			{
			case dirs::left:
				if (ex - speed >= 0)
				{
					x -= speed;
					SetDims();
					return return_type::R_IN;
				}
				break;

			case dirs::right:
				if (x + speed <= scr_width)
				{
					x += speed;
					SetDims();
					return return_type::R_IN;
				}
				break;

			default: return return_type::R_NOT_SUPPORTED;
			}
			return return_type::R_OUT;
		}
		
		return_type Jump(float targ_x = 0, float targ_y = 0) override
		{
			return return_type::R_NOT_SUPPORTED;
		}
		return_type Transform(types _in_what) override
		{
			return return_type::R_NOT_SUPPORTED;
		}

		int GetFrame() override
		{
			return static_cast<int>(return_type::R_NOT_SUPPORTED);
		}

		void Release() override
		{
			delete this;
		}
};

class MOUNTAINS :public INTERFACES
{
public:
	MOUNTAINS(types __what, float _where_x, float _where_y) :INTERFACES(_where_x, _where_y, 1.0f, 1.0f, __what)
	{
		switch (__what)
		{
		case types::mountain1:
			NewDims(300.0f, 164.0f);
			speed = 0.3f;
			break;

		case types::mountain2:
			NewDims(400.0f, 189.0f);
			speed = 0.2f;
			break;

		case types::mountain3:
			NewDims(400.0f, 351.0f);
			speed = 0.1f;
			break;
		}

		dir = dirs::left;
	}

	return_type Move() override
	{
		switch (dir)
		{
		case dirs::left:
			if (ex - speed >= 0)
			{
				x -= speed;
				SetDims();
				return return_type::R_IN;
			}
			break;

		case dirs::right:
			if (x + speed <= scr_width)
			{
				x += speed;
				SetDims();
				return return_type::R_IN;
			}
			break;

		default: return return_type::R_NOT_SUPPORTED;
		}
		return return_type::R_OUT;
	}

	return_type Jump(float targ_x = 0, float targ_y = 0) override
	{
		return return_type::R_NOT_SUPPORTED;
	}
	return_type Transform(types _in_what) override
	{
		return return_type::R_NOT_SUPPORTED;
	}

	int GetFrame() override
	{
		return static_cast<int>(return_type::R_NOT_SUPPORTED);
	}

	void Release() override
	{
		delete this;
	}
};

class FIELD : public INTERFACES
{
	public:

		FIELD(float _where_x = 0, float where_y = scr_height - 155.0f) :INTERFACES(_where_x,where_y,1000.0f,100.0f,types::field)
		{
			speed = 0.5f;
		}
		~FIELD() {};

		return_type Move() override
		{
			switch (dir)
			{
			case dirs::left:
				if (ex - speed >= 0)
				{
					x -= speed;
					SetDims();
					return return_type::R_IN;
				}
				break;

			case dirs::right:
				if (x + speed <= scr_width)
				{
					x += speed;
					SetDims();
					return return_type::R_IN;
				}
				break;

			default: return return_type::R_NOT_SUPPORTED;
			}
			return return_type::R_OUT;
		}

		return_type Jump(float targ_x = 0, float targ_y = 0) override
		{
			return return_type::R_NOT_SUPPORTED;
		}
		return_type Transform(types _in_what) override
		{
			return return_type::R_NOT_SUPPORTED;
		}

		int GetFrame() override
		{
			return static_cast<int>(return_type::R_NOT_SUPPORTED);
		}

		void Release() override
		{
			delete this;
		}

};

class COIN :public INTERFACES
{
	public:
		COIN(float _x, float _y) :INTERFACES(_x, _y, 10.0f, 10.0f, types::coin)
		{
			dir = dirs::stop;
		}
		~COIN() {};

		return_type Move() override
		{
			switch (dir)
			{
			case dirs::left:
				if (ex - speed >= 0)
				{
					x -= speed;
					SetDims();
					return return_type::R_IN;
				}
				break;

			case dirs::right:
				if (x + speed <= scr_width)
				{
					x += speed;
					SetDims();
					return return_type::R_IN;
				}
				break;

			default: return return_type::R_NOT_SUPPORTED;
			}
			return return_type::R_OUT;
		}

		return_type Jump(float targ_x = 0, float targ_y = 0) override
		{
			return return_type::R_NOT_SUPPORTED;
		}
		return_type Transform(types _in_what) override
		{
			return return_type::R_NOT_SUPPORTED;
		}

		int GetFrame() override
		{
			return static_cast<int>(return_type::R_NOT_SUPPORTED);
		}

		void Release() override
		{
			delete this;
		}

};

class BRICKS :public INTERFACES
{
public:

	BRICKS(float _x, float _y) :INTERFACES(_x, _y, 60.0f, 60.0f, types::brick)
	{
		dir = dirs::stop;
	}

	return_type Move() override
	{
		switch (dir)
		{
		case dirs::left:
			if (ex - speed >= 0)
			{
				x -= speed;
				SetDims();
				return return_type::R_IN;
			}
			break;

		case dirs::right:
			if (x + speed <= scr_width)
			{
				x += speed;
				SetDims();
				return return_type::R_IN;
			}
			break;

		default: return return_type::R_NOT_SUPPORTED;
		}
		return return_type::R_OUT;
	}

	return_type Jump(float targ_x = 0, float targ_y = 0) override
	{
		return return_type::R_NOT_SUPPORTED;
	}
	return_type Transform(types _in_what) override
	{
		if (_in_what == types::goldbrick)
		{
			type = types::goldbrick;
			return return_type::R_OK;
		}
		
		return return_type::R_NOT_SUPPORTED;
	}

	int GetFrame() override
	{
		return static_cast<int>(return_type::R_NOT_SUPPORTED);
	}

	void Release() override
	{
		delete this;
	}
	
};

class MUSHROOM :public INTERFACES
{
	public:

		MUSHROOM(float _x, float _y) :INTERFACES(_x, _y, 40.0f, 40.0f, types::mushroom)
		{
			dir = dirs::stop;
		}

		return_type Move() override
		{
			switch (dir)
			{
			case dirs::left:
				if (ex - speed >= 0)
				{
					x -= speed;
					SetDims();
					return return_type::R_IN;
				}
				break;

			case dirs::right:
				if (x + speed <= scr_width)
				{
					x += speed;
					SetDims();
					return return_type::R_IN;
				}
				break;

			default: return return_type::R_NOT_SUPPORTED;
			}
			return return_type::R_OUT;
		}

		return_type Jump(float targ_x = 0, float targ_y = 0) override
		{
			return return_type::R_NOT_SUPPORTED;
		}
		return_type Transform(types _in_what) override
		{
			return return_type::R_NOT_SUPPORTED;
		}

		int GetFrame() override
		{
			return static_cast<int>(return_type::R_NOT_SUPPORTED);
		}

		void Release() override
		{
			delete this;
		}

};

class MARIO :public INTERFACES
{
	public:

		MARIO() :INTERFACES(100.0f, scr_height - 155.0f, 50.0f, 55.0f, types::mario)
		{
			dir = dirs::right;
			state = states::stop;
			speed = 2.0f;
		}

		return_type Move() override
		{
			if (state == states::stop)return return_type::R_IN;
			switch (dir)
			{
			case dirs::left:
				if (x - speed >= 0)
				{
					x -= speed;
					SetDims();
					return return_type::R_IN;
				}
				else
				{
					return return_type::R_IN;
					break;
				}
				
				break;

			case dirs::right:
				if (ex + speed <= scr_width)
				{
					x += speed;
					SetDims();
					return return_type::R_IN;
				}
				else
				{
					return return_type::R_IN;
					break;
				}
				
				break;

			case dirs::down:
				y += speed;
				SetDims();
				if (ey > scr_height - 155.0f)return return_type::R_OUT;
				break;
			}
			return return_type::R_OUT;
		}
		
		return_type Jump(float targ_x, float targ_y) override
		{
			static line_graph line_info;

			if (state != states::jump_up && state != states::jump_down)
			{
				old_x = x;
				old_y = y;
				prev_dir = dir;
				dir = dirs::up;
				state = states::jump_up;
				line_info = GetYFromSlope(targ_x, targ_y);
			}
			else
			{
				if (dir == dirs::up)
				{
					if (y > targ_y)
					{
						if (prev_dir == dirs::right || prev_dir == dirs::stop)
						{
							if (old_x < targ_x) x += speed;
						}
						else if (prev_dir == dirs::left)
						{
							if (old_x > targ_x) x -= speed;
						}

						y = (line_info.slope * x) + line_info.intercept;

						SetDims();
						return return_type::R_IN;
					}
					else dir = dirs::down;
				}
				else if (dir == dirs::down)
				{
					state = states::jump_down;
					line_info = GetYFromSlope(targ_x, old_y);

					if (y < old_y)
					{
						if (prev_dir == dirs::right || prev_dir == dirs::stop)
						{

							if (x < targ_x) x += speed;

							y = (line_info.slope * x) + line_info.intercept;

							if (y >= old_y)
							{
								y = old_y;
								SetDims();
								state = states::stop;
								dir = dirs::stop;
								return return_type::R_IN;
							}
							SetDims();
						}
						else if (prev_dir == dirs::left)
						{
							if (x > targ_x)x -= speed;
							y = (line_info.slope * x) + line_info.intercept;

							if (y >= old_y)
							{
								y = old_y;
								SetDims();
								state = states::stop;
								dir = dirs::stop;
								return return_type::R_IN;
							}
							SetDims();
						}
					}
				}
			}
			return return_type::R_FAIL;
		}
		
		return_type Transform(types _in_what) override
		{
			return return_type::R_NOT_SUPPORTED;
		}
		
		int GetFrame() override
		{
			frame_delay--;
			if(frame_delay <= 0)
			{
				frame_delay = 5;
				frame_count++;
				if (frame_count > 11)frame_count = 0;
			}
			return frame_count;
		}

		void Release() override
		{
			delete this;
		}

};

class TURTLE :public INTERFACES
{
	public:

		TURTLE(float _x, float _y) :INTERFACES(_x, _y, 64.0f, 80.0f)
		{
			dir = dirs::left;
			state = states::move;
			speed = 1.0f;
		}

		~TURTLE() {};

		return_type Move() override
		{
			switch (dir)
			{
			case dirs::left:
				if (ex - speed >= 0)
				{
					x -= speed;
					SetDims();
					return return_type::R_IN;
				}
				else
				{
					dir = dirs::right;
					break;
				}
				break;

			case dirs::right:
				if (x + speed <= scr_width)
				{
					x += speed;
					SetDims();
					return return_type::R_IN;
				}
				else
				{
					dir = dirs::left;
					break;
				}
				break;

			case dirs::down:
				y += speed;
				SetDims();
				if (ey > scr_height - 155.0f)return return_type::R_OUT;
				break;
			}
			return return_type::R_OUT;
		}

		return_type Jump(float targ_x, float targ_y) override
		{
			return return_type::R_NOT_SUPPORTED;
		}

		return_type Transform(types _in_what) override
		{
			return return_type::R_NOT_SUPPORTED;
		}

		int GetFrame() override
		{
			frame_delay--;
			if (frame_delay <= 0)
			{
				frame_delay = 15;
				frame_count++;
				if (frame_count > 1)frame_count = 0;
			}
			return frame_count;
		}

		void Release() override
		{
			delete this;
		}
};


///////////////////////////////
obj_ptr iCreate(types _what, float where_x, float where_y)
{
	obj_ptr ret = nullptr;

	switch (_what)
	{
	case types::cloud1:
		ret = new CLOUDS(types::cloud1, where_x, where_y);
		break;

	case types::cloud2:
		ret = new CLOUDS(types::cloud2, where_x, where_y);
		break;

	case types::mountain1:
		ret = new MOUNTAINS(types::mountain1, where_x, where_y);
		break;

	case types::mountain2:
		ret = new MOUNTAINS(types::mountain2, where_x, where_y);
		break;

	case types::mountain3:
		ret = new MOUNTAINS(types::mountain3, where_x, where_y);
		break;

	case types::field:
		ret = new FIELD(where_x, where_y);
		break;

	case types::coin:
		ret = new COIN(where_x, where_y);
		break;

	case types::brick:
		ret = new BRICKS(where_x, where_y);
		break;

	case types::mushroom:
		ret = new MUSHROOM(where_x, where_y);
		break;

	case types::mario:
		ret = new MARIO();
		break;

	case types::turtle:
		ret = new TURTLE(where_x, where_y);
		break;
	}

	return ret;
}
