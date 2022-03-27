#pragma once
#include <memory>
#include <functional>

namespace Elysium
{
//Inspired by:
//https://medium.com/brakulla/signal-slot-implementation-part-1-adb458454f05

	template<typename ...Args>
	using Callback = std::function<void(Args...)>;

	template<typename ...Args>
	class Listener
	{
		public:
			Listener(const Listener&) = delete;
			Listener(Listener&&) = delete;
			Listener& operator=(const Listener&) = delete;

			Listener():m_hasCallback(false)
			{}
				
			Listener(const Callback<Args...>& callback)
			{
				setCallback(callback);
			}

			void onEvent(const Args... args)
			{
				if(m_hasCallback)
					m_callback(args...);
			}	

			void setCallback(const Callback<Args...>& callback)
			{
				m_callback = callback;
				m_hasCallback = true;
			}

		private:
			bool m_hasCallback = false;
			Callback<Args...> m_callback;
	};

	template<typename ...Args>
	using SharedListener = std::shared_ptr<Listener<Args...>>;

	template<typename ...Args>
	class Emitter
	{
		using WeakListener = std::weak_ptr<Listener<Args...>>;

		public:
			Emitter()
			{}

			~Emitter()
			{
				for(auto it = m_listeners.begin(); it != m_listeners.end();)
				{
					it = m_listeners.erase(it);
				}
			}

			void broadcast(const Args... args)
			{
				for(auto it = m_listeners.begin(); it != m_listeners.end();)
				{
					if(auto listener = it->lock())
					{
						listener->onEvent(args...);
						++it;		
					}
					else
					{
						it = m_listeners.erase(it);
					}
				}
			}

			void addListener(const SharedListener<Args...> listener)
			{
				m_listeners.push_back(WeakListener(listener));
			}	

			void removeListener(const SharedListener<Args...> listener)
			{
				for(auto it = m_listeners.begin(); it != m_listeners.end();)
				{
					if(*it == listener)
						it = m_listeners.erase();
					else
						++it;
				}	
			}

		private:
			std::vector<WeakListener> m_listeners;
	};
	
}
