#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <functional>

namespace Circe{
	
	using namespace std;
	
	template<class T> class CallbackObserver;
	template<class T> class ObserverNode;
	template<class T> class Observable;
	
	/*
	Example 1:
	
	Observable<Mat<2>> subject({1.0f, 0.0f,  0.0f, 1.0f});

	shared_ptr<bool> canary=make_shared<bool>();
	subject.addObserver([](const Mat<2>& f){cout << f << endl;}, canary);
	
	subject=Mat<2>({2.0f, 0.0f, 2.0f, 1.0f});
	
	*/

	template<class T>
	class CallbackObserver
	{
		public:
			CallbackObserver()
			{}
		
			CallbackObserver(const function<void(const T&)>& functor, shared_ptr<void> ptr):m_functor(functor), m_canary(weak_ptr<void>(ptr))
			{}
			
			void react(const T& value)
			{
				m_functor(value);
			}
			
			bool isAlive()
			{
				return m_canary.lock()!=0;
			}
			
		private:
			function<void(const T&)> m_functor;
			weak_ptr<void> m_canary;
	};
	
	//Internal class, node of the linkedlist containing the observers.
	template<class T>
	class ObserverNode
	{
		public:
			ObserverNode()
			{
				id=0;
			}
			
			ObserverNode(ObserverNode<T>&& node)
			{
				m_observer = node.m_observer;
				m_next=move(node.m_next);
				id=node.id;
			}
			
			ObserverNode(const function<void(const T&)>& functor, const shared_ptr<void> ptr, const int& id):m_observer(functor, ptr), id(id)
			{}
			
			~ObserverNode()
			{
				cout << "Deleting node " << id <<endl;
			}
			
			int addObserver(const function<void(const T&)>& functor, const shared_ptr<void> ptr)
			{
				if(m_next==nullptr)
				{
					m_next=make_unique<ObserverNode<T>>(functor, ptr, id+1);
					
				}
				else
				{
					m_next->addObserver(functor, ptr);
				}
				return id+1;
			}
			
			void removeObserver(const int& id)
			{	
				if(m_next->id == id)
				{		
					removeNext();
				}
				else
				{
					if(!m_next)return;//check for nullptr
					m_next->removeObserver(id);
				}
			}
			
			void notify(const T& value)//only called from the head or outside
			{
				if(m_next)//check if nullptr
				{
					CallbackObserver<T> nextObserver = m_next->m_observer;
					if(nextObserver.isAlive())//check if the next one is valid, to know if you should remove it. Checking the next instead of the current doesnt verify the head, which is what we want.
					{	
						nextObserver.react(value);
						m_next->notify(value);	
					}
					else
					{
						removeNext();
						notify(value);
					}
				}
			}
			
			void printAll()
			{
				cout << "Node " << id << endl;
				if(m_next)
				{
					m_next->printAll();
				}
			}
			
		private:
			CallbackObserver<T> m_observer;					//Observer callback wrapper
			unique_ptr<ObserverNode<T>> m_next; 			//Pointer to the next observer wrapper
			int id;
			
			void removeNext()
			{
				m_next=move(m_next->m_next);
			}
	};
	
	//A wrapper that allows the observers to be notified of its change;
	template<class T>
	class Observable : public ObserverNode<T>
	{
		public:
			Observable(Observable<T>&& observable)
			{
				m_value=observable.m_value;
			}
		
			template<typename ...Args>
			Observable(Args&&... args):m_value(forward<Args>(args)...)
			{}
			
			void set(const T& newValue)
			{
				m_value=newValue;
				notify();
			}
			
			T get() const
			{
				return m_value;
			}
			
			void notify()
			{
				ObserverNode<T>::notify(m_value);
			}
			
			void operator+=(const T& b)
			{
				m_value+=b;
				notify();
			}
			
			void operator-=(const T& b)
			{
				m_value-=b;
				notify();
			}
			
			void operator*=(const float& b)
			{
				m_value*=b;
				notify();
			}
			
			T& operator=(const T& b)
			{
				m_value=b;
				notify();
				return m_value;
			}

		private:
			T m_value;			
	};
}
