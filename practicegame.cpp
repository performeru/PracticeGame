#include <iostream>

const char StageData[]{ "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########" };

const int StageWidth{ 8 }, StageHeight{ 5 };

enum Object
{
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BOX,
	OBJ_BOX_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,

	OBJ_UNKNOWN,
};

void initialize(Object* state, int Width, int Height, const char* stageData);
void draw(const Object* state, int Width, int Height);
void update(Object* state, char input, int Width, int Height);
bool isClear(const Object* state, int Width, int Height);

int main()
{
	Object* state = new Object[StageWidth * StageHeight];

	initialize(state, StageWidth, StageHeight, StageData);

	while (true)
	{
		draw(state, StageWidth, StageHeight);

		if (isClear(state, StageWidth, StageHeight))
		{
			break;
		}

		std::cout << "a: left d: right w: up s: down // 커맨드를 입력하세요" << std::endl;
		char input;
		std::cin >> input;

		update(state, input, StageWidth, StageHeight);
	}

	std::cout << "You Win!" << std::endl;

	delete[] state;
}

void initialize(Object* state, int Width, int Height, const char* stageData)
{
	const char* d = stageData;
	int x = 0;
	int y = 0;

	while (*d != '\0')
	{
		Object t;

		switch (*d)
		{
		case '#':
			t = OBJ_WALL;
			break;
		case ' ':
			t = OBJ_SPACE;
			break;
		case 'o':
			t = OBJ_BOX;
			break;
		case 'O':
			t = OBJ_BOX_GOAL;
			break;
		case '.':
			t = OBJ_GOAL;
			break;
		case 'p':
			t = OBJ_MAN;
			break;
		case 'P':
			t = OBJ_MAN_ON_GOAL;
			break;
		case '\n':
			x = 0;
			++y;
			t = OBJ_UNKNOWN;
			break;
		default:
			t = OBJ_UNKNOWN;
			break;
		}

		++d;

		if (t != OBJ_UNKNOWN)
		{
			state[y * Width + x] = t;
			++x;
		}
	}
}

void draw(const Object* state, int Width, int Height)
{
	const char font[]{ ' ', '#', ',', 'o', 'O', 'p', 'P' };

	for (int y = 0; y < Height; ++y)
	{
		for (int x = 0; x < Width; ++x)
		{
			Object O = state[y * Width + x];
			std::cout << font[O];
		}

		std::cout << std::endl;
	}
}

void update(Object* state, char input, int Width, int Height)
{
	int dx = 0;
	int dy = 0;

	switch (input)
	{
	case 'a':
		dx = -1;
		break;
	case 'd':
		dx = 1;
		break;
	case 'w':
		dy = -1;
		break;
	case 's':
		dy = 1;
		break;
	default:
		break;
	}

	int i = -1;
	for (i = 0; i < Width * Height; ++i)
	{
		if (state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL)
		{
			break;
		}
	}

	int x = i % Width;
	int y = i / Width;

	int tx = x + dx;
	int ty = y + dy;

	if (tx < 0 || ty < 0 || tx >= Width || ty >= Height)
	{
		return;
	}

	int p = y * Width + x;
	int tp = ty * Width + tx;

	if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL)
	{
		state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
		state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
	}
	else if (state[tp] == OBJ_BOX || state[tp] == OBJ_BOX_GOAL)
	{
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		if (tx2 < 0 || ty2 < 0 || tx2 >= Width || ty2 >= Height)
		{
			return;
		}

		int tp2 = ty2 * Width + tx2;

		if (state[tp2] == OBJ_SPACE || state[tp2] == OBJ_GOAL)
		{
			state[tp2] = (state[tp2] == OBJ_GOAL) ? OBJ_BOX_GOAL : OBJ_BOX;
			state[tp] = (state[tp] == OBJ_BOX_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

bool isClear(const Object* state, int Width, int Height)
{
	for (int i = 0; i < Width * Height; ++i)
	{
		if (state[i] == OBJ_BOX)
		{
			return false;
		}
	}

	return true;
}