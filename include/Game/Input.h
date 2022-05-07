#pragma once
#include <Circe/Circe.h>
#include <memory>
#include <deque>
#include <functional>
#include <unordered_map>
#include <SDL2/SDL.h>

namespace Elysium
{

	using Key = unsigned int;

	class Input
	{
		public:
			Input();
			Input(const Input&) = delete;
			Input(Input&&) = delete;
			Input& operator=(const Input&)=delete;

			void pressKey(const Key key, const int state);

			/** Press virtual key (i.e. GUI button) **/
			void pressKey(const std::string& name, const int state);

			void moveMouse(const int x, const int y,
						   const int dx, const int dy);

			Circe::Emitter<int>& operator()(const Key key);				

			Circe::Emitter<int>& getKeyListener(const Key key);

			Circe::Emitter<Circe::Vec2>& getMouseMotion();

			bool isDown(const Key key);

			bool isDown(const std::string& virtualKey);

			bool isActive(const std::string& action);

			Circe::Vec2 getMousePosition() const;

			void clear();

			void poll();

			bool isTerminated() const;

			void addActionToKey(const Key key, const std::string& action);

			Key getKey(const std::string& name) const;

			static constexpr Key KEY_A = 97;
			static constexpr Key KEY_B = 98;
			static constexpr Key KEY_C = 99;
			static constexpr Key KEY_D = 100;
			static constexpr Key KEY_E = 101;
			static constexpr Key KEY_F = 102;
			static constexpr Key KEY_G = 103;
			static constexpr Key KEY_H = 104;
			static constexpr Key KEY_I = 105;
			static constexpr Key KEY_J = 106;
			static constexpr Key KEY_K = 107;
			static constexpr Key KEY_L = 108;
			static constexpr Key KEY_M = 109;
			static constexpr Key KEY_N = 110;
			static constexpr Key KEY_O = 111;
			static constexpr Key KEY_P = 112;
			static constexpr Key KEY_Q = 113;
			static constexpr Key KEY_R = 114;
			static constexpr Key KEY_S = 115;
			static constexpr Key KEY_T = 116;
			static constexpr Key KEY_U = 117;
			static constexpr Key KEY_V = 118;
			static constexpr Key KEY_W = 119;
			static constexpr Key KEY_X = 120;
			static constexpr Key KEY_Y = 121;
			static constexpr Key KEY_Z = 122;
			static constexpr Key KEY_0 = 123;
			static constexpr Key KEY_1 = 124;
			static constexpr Key KEY_2 = 125;
			static constexpr Key KEY_3 = 126;
			static constexpr Key KEY_4 = 127;
			static constexpr Key KEY_5 = 128;
			static constexpr Key KEY_6 = 129;
			static constexpr Key KEY_7 = 130;
			static constexpr Key KEY_8 = 131;
			static constexpr Key KEY_9 = 132;	
			static constexpr Key KEY_ENTER = 13;
			static constexpr Key KEY_ESCAPE = 27;
			static constexpr Key KEY_BACKSPACE = 8;
			static constexpr Key KEY_TAB = 9;
			static constexpr Key KEY_SPACE = 32;
			static constexpr Key KEY_MINUS = 1073741910;
			static constexpr Key KEY_EQUALS = 1073741911;
			static constexpr Key KEY_MOUSELEFT = 200;
			static constexpr Key KEY_MOUSERIGHT = 201;
		
		private:
			std::unordered_map<Key, Circe::Emitter<int>> m_emitters;
			Circe::Emitter<Circe::Vec2> m_mouseMotion;
			Circe::Vec2 m_mouseDx;
			Circe::Vec2 m_mousePosition;
			std::deque<Key> m_activeKeys;
			std::unordered_map<unsigned int, Key> m_actions;
			bool m_terminate;
			static bool m_instantiated;
	};
}
