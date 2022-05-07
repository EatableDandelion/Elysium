#pragma once
#include <memory>
#include <functional>

namespace Circe
{
	/**
	Inspired by:
	https://medium.com/brakulla/signal-slot-implementation-part-1-adb458454f05
	*/
	/** How to use:
	Emitter<int> emitter;
	Listener<int> listener = 
					std::make_shared<ListenerObject<int>>();
	listener->setCallback([](int input)
			{
				std::cout << input << std::endl;
			});
	emitter.addListener(listener);

	int a = 10;
	emitter.broadcast(a);
	//Prints "10"
	**/

	template<typename ...Args>
	using Callback = std::function<void(Args...)>;

	template<typename ...Args>
	class ListenerObject
	{
		public:
			ListenerObject(const ListenerObject&) = delete;
			ListenerObject(ListenerObject&&) = delete;
			ListenerObject& operator=(const ListenerObject&) = delete;

			ListenerObject():m_hasCallback(false)
			{}
				
			ListenerObject(const Callback<Args...>& callback)
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
	using Listener = std::shared_ptr<ListenerObject<Args...>>;

	template<typename ...Args>
	class Emitter
	{
		using WeakListener = std::weak_ptr<ListenerObject<Args...>>;

		public:
			Emitter()
			{}

			~Emitter()
			{
				removeListeners();	
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

			void addListener(const Listener<Args...> listener)
			{
				m_listeners.push_back(WeakListener(listener));
			}	

			void removeListener(const Listener<Args...> listener)
			{
				for(auto it = m_listeners.begin(); it != m_listeners.end();)
				{
					if(*it == listener)
						it = m_listeners.erase();
					else
						++it;
				}	
			}

			void removeListeners()
			{
				m_listeners.clear();	
			}

		private:
			std::vector<WeakListener> m_listeners;
	};
}
