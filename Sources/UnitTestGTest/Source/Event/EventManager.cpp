//#include "stdafx.hpp"
//#include "Event/EventManager.hpp"
//#include "gtest/gtest.h"
//#include "Core/DebugMemoryManager.hpp"
//#include <queue>
//
//class Event_Test : public Croissant::Event::Event
//{
//public:
//	Event_Test(const String&& type) : Event(std::move(type)) {}
//
//	virtual ~Event_Test() {}
//
//};
//
//class EventListener_Test : public Croissant::Event::IEventListener
//{
//public:
//
//	virtual void ProcessEvent( Croissant::Event::Event* evt ) 
//	{
//		m_events.push(evt->Name());
//	}
//
//	std::queue<String> m_events;
//};
//
//class EventManagerTest : public ::testing::Test
//{
//public:
//	virtual void SetUp()
//	{
//		m_manager = new Croissant::Event::EventManager();
//	}
//
//	virtual void TearDown()
//	{
//		delete m_manager;
//	}
//
//protected:
//	Croissant::Event::EventManager* m_manager;
//};
//
//TEST_F(EventManagerTest, AddEventWithPrioritySTARTFRAME)
//{
//	std::unique_ptr<Croissant::Event::Event> evt = std::unique_ptr<Croissant::Event::Event>(CROISSANT_NEW Event_Test(String("START_FRAME")));
//
//	m_manager->AddEvent(std::move(evt), Croissant::Event::EventPriority::START_FRAME);
//	ASSERT_EQ(1, m_manager->GetEventCount());
//}
//
//TEST_F(EventManagerTest, AddEventWithPriorityUSERACTION)
//{
//	std::unique_ptr<Croissant::Event::Event> evt = std::unique_ptr<Croissant::Event::Event>(CROISSANT_NEW Event_Test(String("USER_ACTION")));
//
//	m_manager->AddEvent(std::move(evt), Croissant::Event::EventPriority::USER_ACTION);
//	ASSERT_EQ(1, m_manager->GetEventCount());
//}
//
//TEST_F(EventManagerTest, AddEventWithPriorityINTERACTION)
//{
//	std::unique_ptr<Croissant::Event::Event> evt = std::unique_ptr<Croissant::Event::Event>(CROISSANT_NEW Event_Test(String("INTERACTION")));
//
//	m_manager->AddEvent(std::move(evt), Croissant::Event::EventPriority::INTERACTION);
//	ASSERT_EQ(1, m_manager->GetEventCount());
//}
//
//TEST_F(EventManagerTest, AddEventWithPriorityENDFRAME)
//{
//	std::unique_ptr<Croissant::Event::Event> evt(CROISSANT_NEW Event_Test(String("END_FRAME")));
//
//	m_manager->AddEvent(std::move(evt), Croissant::Event::EventPriority::END_FRAME);
//	ASSERT_EQ(1, m_manager->GetEventCount());
//}
//
//TEST_F(EventManagerTest, AddEventWithPriorityRENDER)
//{
//	std::unique_ptr<Croissant::Event::Event> evt = std::unique_ptr<Croissant::Event::Event>(CROISSANT_NEW Event_Test(String("RENDER")));
//
//	m_manager->AddEvent(std::move(evt), Croissant::Event::EventPriority::RENDER);
//	ASSERT_EQ(1, m_manager->GetEventCount());
//}
//
//TEST_F(EventManagerTest, EventExecutedInTheRightOrder)
//{
//	std::unique_ptr<Croissant::Event::Event> evt(CROISSANT_NEW Event_Test(String("RENDER")));
//	EventListener_Test listener;
//
//	m_manager->AddEvent(std::move(evt), Croissant::Event::EventPriority::RENDER);
//	evt.reset(CROISSANT_NEW Event_Test(String("END_FRAME")));
//	m_manager->AddEvent(std::move(evt), Croissant::Event::EventPriority::END_FRAME);
//	evt.reset(CROISSANT_NEW Event_Test(String("INTERACTION")));
//	m_manager->AddEvent(std::move(evt), Croissant::Event::EventPriority::INTERACTION);
//	evt.reset(CROISSANT_NEW Event_Test(String("START_FRAME")));
//	m_manager->AddEvent(std::move(evt), Croissant::Event::EventPriority::START_FRAME);
//	evt.reset(CROISSANT_NEW Event_Test(String("USER_ACTION")));
//	m_manager->AddEvent(std::move(evt), Croissant::Event::EventPriority::USER_ACTION);
//
//	m_manager->RegisterListener([&listener](Croissant::Event::Event* event) -> void {
//		listener.ProcessEvent(event);
//	});
//
//	ASSERT_EQ(5, m_manager->GetEventCount());
//	m_manager->ProcessEvents();
//	ASSERT_EQ(0, m_manager->GetEventCount());
//	ASSERT_STREQ("START_FRAME", listener.m_events.front().Str());
//	listener.m_events.pop();
//	ASSERT_STREQ("USER_ACTION", listener.m_events.front().Str());
//	listener.m_events.pop();
//	ASSERT_STREQ("INTERACTION", listener.m_events.front().Str());
//	listener.m_events.pop();
//	ASSERT_STREQ("END_FRAME", listener.m_events.front().Str());
//	listener.m_events.pop();
//	ASSERT_STREQ("RENDER", listener.m_events.front().Str());
//	listener.m_events.pop();
//	ASSERT_TRUE(listener.m_events.empty());
//	ASSERT_EQ(0, m_manager->GetEventCount());
//}
