#pragma once


	enum class KEY_TYPE
	{
		UP = VK_UP,
		DOWN = VK_DOWN,
		LEFT = VK_LEFT,
		RIGHT = VK_RIGHT,

		Q = 'Q',
		E = 'E',

		A = 'A',
		S = 'S',
		D = 'D',
		F = 'F',
	};

	enum class KEY_STATE
	{
		NONE,
		PRESS,
		DOWN,
		UP,
		END,
	};

	enum
	{
		KEY_TYPE_COUNT = static_cast<int32>(UINT8_MAX + 1),
		KEY_STATE_COUNT = static_cast<int32>(KEY_STATE::END),
	};

	class Input
	{
		DECLARE_SINGLE(Input);

	public:
		void Init(HWND hwnd);
		void Update();

		// when key hold
		bool GetButton(KEY_TYPE key) { return GetState(key) == KEY_STATE::PRESS; }
		// when key pressed down once
		bool GetButtonDown(KEY_TYPE key) { return GetState(key) == KEY_STATE::DOWN; }
		// when key pressed up after pressed down
		bool GetButtonUp(KEY_TYPE key) { return GetState(key) == KEY_STATE::UP; }

	private:
		inline KEY_STATE GetState(KEY_TYPE key) { return _states[static_cast<uint8>(key)]; }

	private:
		HWND _hwnd;
		vector<KEY_STATE> _states;
	};
