#pragma once
#include <Circe/Circe.h>
#include <memory>
#include <functional>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "Event.h"

namespace Elysium
{
	class Input
	{
		public:
			Input();
			Input(const Input&) = delete;
			Input(Input&&) = delete;
			Input& operator=(const Input&)=delete;

			void pressKey(const int key, const int state);

			void moveMouse(const int x, const int y,
						   const int dx, const int dy);

			Emitter<int>& operator()(const int key);				

			Emitter<int>& getKeyListener(const int key);

			Emitter<Circe::Vec2>& getMouseMotion();

			bool isDown(const int key);

			Circe::Vec2 getMousePosition() const;

			void poll();

			bool isTerminated() const;

			static constexpr int KEY_A = 97;
			static constexpr int KEY_B = 98;
			static constexpr int KEY_C = 99;
			static constexpr int KEY_D = 100;
			static constexpr int KEY_E = 101;
			static constexpr int KEY_F = 102;
			static constexpr int KEY_G = 103;
			static constexpr int KEY_H = 104;
			static constexpr int KEY_I = 105;
			static constexpr int KEY_J = 106;
			static constexpr int KEY_K = 107;
			static constexpr int KEY_L = 108;
			static constexpr int KEY_M = 109;
			static constexpr int KEY_N = 110;
			static constexpr int KEY_O = 111;
			static constexpr int KEY_P = 112;
			static constexpr int KEY_Q = 113;
			static constexpr int KEY_R = 114;
			static constexpr int KEY_S = 115;
			static constexpr int KEY_T = 116;
			static constexpr int KEY_U = 117;
			static constexpr int KEY_V = 118;
			static constexpr int KEY_W = 119;
			static constexpr int KEY_X = 120;
			static constexpr int KEY_Y = 121;
			static constexpr int KEY_Z = 122;
			static constexpr int KEY_0 = 123;
			static constexpr int KEY_1 = 124;
			static constexpr int KEY_2 = 125;
			static constexpr int KEY_3 = 126;
			static constexpr int KEY_4 = 127;
			static constexpr int KEY_5 = 128;
			static constexpr int KEY_6 = 129;
			static constexpr int KEY_7 = 130;
			static constexpr int KEY_8 = 131;
			static constexpr int KEY_9 = 132;	
			static constexpr int KEY_ENTER = 13;
			static constexpr int KEY_ESCAPE = 27;
			static constexpr int KEY_BACKSPACE = 8;
			static constexpr int KEY_TAB = 9;
			static constexpr int KEY_SPACE = 32;
			static constexpr int KEY_MINUS = 1073741910;
			static constexpr int KEY_EQUALS = 1073741911;
			static constexpr int KEY_MOUSELEFT = 200;
			static constexpr int KEY_MOUSERIGHT = 201;
		
		private:
			std::unordered_map<int, Emitter<int>> m_emitters;
			Emitter<Circe::Vec2> m_mouseMotion;
			Circe::Vec2 m_mouseDx;
			Circe::Vec2 m_mousePosition;
			std::unordered_map<int, int> m_keyState;
			bool m_terminate;
			static bool m_instantiated;
	};
}
