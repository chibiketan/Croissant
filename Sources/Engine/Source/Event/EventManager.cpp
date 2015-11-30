#include "stdafx.hpp"
#include "Event/EventManager.hpp"
#include <map>
#include <vector>

#define QUICK_ACCESS_LIST_SIZE 5

namespace Croissant
{
	namespace Event
	{
		typedef std::vector<ListenerDelegate> ListenerList;
		typedef std::map<String, ListenerList*> ListenerDictionnary;

		class EventManager::EventManagerImpl : private Croissant::Core::NonCopyable
		{
		public:
			EventManagerImpl();
			~EventManagerImpl();
			void AddEvent(std::unique_ptr<Event> event, EventPriority priority);
			void RegisterListener(const String& category, ListenerDelegate listener);
			void UnregisterListener(const String& category, ListenerDelegate listener);
			std::uint32_t GetEventCount() const;
			void ProcessEvents();

		private:
			struct Node
			{
				std::unique_ptr<Event> item;
				EventPriority priority;
				Node* prev;
				Node* next;
			};

			Node* GetLastOfSection(EventPriority priority) const;
			Node* GetLastNodeBeforeSection(EventPriority priority) const;
			std::uint32_t m_eventCount;
			Node* m_list;
			ListenerDictionnary m_listenersDictionnary;
			Node* m_listQuickAccess[QUICK_ACCESS_LIST_SIZE];
		};

		EventManager::EventManagerImpl::EventManagerImpl()
			: m_eventCount(0), m_list(nullptr)
		{
			for (int i = 0; i < QUICK_ACCESS_LIST_SIZE; ++i)
			{
				m_listQuickAccess[i] = nullptr;
			}
		}

		EventManager::EventManagerImpl::~EventManagerImpl()
		{
			Node* node = m_list;
			Node* next = nullptr;

			if (nullptr != node)
			{
				do
				{
					next = node->next;
					CROISSANT_DELETE(node);
					node = next;
				} while (nullptr != node);

				m_list = nullptr;
			}

			for (auto &val : m_listenersDictionnary)
			{
				if (nullptr != val.second)
				{
					CROISSANT_DELETE(val.second);
				}
			}

			m_listenersDictionnary.clear();
		}

		void EventManager::EventManagerImpl::AddEvent(std::unique_ptr<Event> event, EventPriority priority)
		{
			assert(nullptr != event);
			Node* node = CROISSANT_NEW Node();

			node->item = std::move(event);
			node->priority = priority;
			if (nullptr != m_list)
			{
				Node* lastNode = GetLastOfSection(priority);

				if (nullptr == lastNode)
				{
					lastNode = GetLastNodeBeforeSection(priority);
					m_listQuickAccess[(int)priority] = node;
				}

				if (nullptr != lastNode)
				{
					node->prev = lastNode;
					node->next = lastNode->next;
					lastNode->next = node;
					if (nullptr != node->next)
					{
						node->next->prev = node;
					}
				}
				else
				{
					node->prev = nullptr;
					node->next = m_list;
					m_list->prev = node;
					m_list = node;
				}
			}
			else
			{
				node->prev = nullptr;
				node->next = nullptr;
				m_list = node;
				m_listQuickAccess[(int)priority] = node;
			}

			++m_eventCount;
		}

		std::uint32_t EventManager::EventManagerImpl::GetEventCount() const
		{
			return m_eventCount;
		}

		EventManager::EventManagerImpl::Node* EventManager::EventManagerImpl::GetLastOfSection(EventPriority priority) const
		{
			Node* node = m_listQuickAccess[(int)priority];

			while (nullptr != node && node->next != NULL && node->next->priority == priority)
			{
				node = node->next;
			}

			return node;
		}

		EventManager::EventManagerImpl::Node* EventManager::EventManagerImpl::GetLastNodeBeforeSection(EventPriority priority) const
		{
			int index = (int)priority;
			Node* node = nullptr;

			while (--index >= 0 && node == nullptr)
			{
				node = GetLastOfSection((EventPriority)index);
			}

			return node;
		}

		void EventManager::EventManagerImpl::RegisterListener(const String& category, ListenerDelegate listener)
		{
			ListenerList* listeners = m_listenersDictionnary[category];

			if (nullptr == listeners)
			{
				listeners = CROISSANT_NEW ListenerList();
				m_listenersDictionnary[category] = listeners;
			}

			listeners->push_back(listener);
		}

		//void EventManager::EventManagerImpl::UnregisterListener(const std::string& category, ListenerDelegate listener)
		void EventManager::EventManagerImpl::UnregisterListener(const String& category, Croissant::Core::Delegate<std::function<void (Croissant::Event::Event*)>> listener)
		{
			ListenerList* listeners = m_listenersDictionnary[category];

			if (nullptr == listeners)
			{
				return;
			}

			auto begin = listeners->begin();
			auto end = listeners->end();

			while (begin != end)
			{
				if ((*begin) == listener)
				{
					listeners->erase(begin);
					break;
				}

				++begin;
			}
		}

		void EventManager::EventManagerImpl::ProcessEvents()
		{
			while (nullptr != m_list)
			{
				String index;
				Node* node = m_list;

				if (nullptr != node->next)
				{
					if (node->next->priority == node->priority)
					{
						m_listQuickAccess[(int)node->priority] = node->next;
					}
					else
					{
						m_listQuickAccess[(int)node->priority] = nullptr;
					}

					node->next->prev = nullptr;
				}
				else
				{
					m_listQuickAccess[(int)node->priority] = nullptr;
				}

				m_list = node->next;
				for (int i = 0; i < 2; ++i)
				{
					index = i == 0 ? node->item->Name() : GENERIC_EVENT_TYPE;
					ListenerList* listeners = m_listenersDictionnary[index];

					if (nullptr == listeners)
					{
						continue;
					}

					for (auto &func : (*listeners))
					{
						//(*begin)->ProcessEvent(node->item.get());
						func(node->item.get());
					}
				}

				CROISSANT_DELETE(node);
				--m_eventCount;
			}
		}

		// --------------------------------------------- !EventManagerImpl

		EventManager::EventManager()
		{
			m_impl = CROISSANT_NEW EventManagerImpl();
		}

		EventManager::~EventManager()
		{
			if (nullptr != m_impl)
			{
				CROISSANT_DELETE(m_impl);
				m_impl = nullptr;
			}
		}

		std::uint32_t EventManager::GetEventCount() const
		{
			return m_impl->GetEventCount();
		}

		void EventManager::AddEvent( std::unique_ptr<Event> event, EventPriority priority )
		{
			m_impl->AddEvent(std::move(event), priority);
		}

		void EventManager::RegisterListener(ListenerDelegate listener)
		{
			m_impl->RegisterListener("", listener);
		}

		void EventManager::RegisterListener(const String& category, ListenerDelegate listener)
		{
			assert(category.Length() > 0);
			m_impl->RegisterListener(category, listener);
		}

		void EventManager::UnregisterListener(ListenerDelegate listener)
		{
			m_impl->UnregisterListener("", listener);
		}

		void EventManager::UnregisterListener(const String& category, ListenerDelegate listener)
		{
			assert(category.Length() > 0);
			m_impl->UnregisterListener(category, listener);
		}

		void EventManager::ProcessEvents()
		{
			m_impl->ProcessEvents();
		}

	}
}
