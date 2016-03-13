#include "genos/kernel/waitserver.h"
#include "util/bits.h"
#include "genos/time/sysclock.h"
#include "utilxx/stub.h"

WaitServer waitserver;

BasicWaiter::BasicWaiter() 	: 
_action(tg_error_stub<void, void*>), _action_data(nullptr), 
_trait(0)
{};


BasicWaiter::BasicWaiter( 
	delegate<void, void*> action, void* action_data,
	uint8_t trait) 	: 
_action(action), _action_data(action_data), 
_trait(trait)
{};

Waiter::Waiter( 
	delegate<void, void*> action, void* action_data,
	delegate<bool, void*> condition, void* condition_data,
	uint8_t trait) 	: 
BasicWaiter(action, action_data, trait), 
_condition(condition), _condition_data(condition_data)
{};

TimWaiter::TimWaiter (	
	delegate<void, void*> action, void* action_data,
	uint8_t trait) :
BasicWaiter(action, action_data, trait)
{};

TimWaiter::TimWaiter ( ) :
BasicWaiter()
{};

void WaitServer::waiter_put_to_list(Waiter* w)
{
	bits_clr(w->_trait, Waiter_DONE);
	list_move_tail(&w->lst, &wait_list);
};

void WaitServer::timwaiter_put_to_list(TimWaiter* t)
{
	list_head* it;
	list_for_each(it, &timer_list)
	{
		if (
		((time_t)list_entry(it, TimWaiter, lst)->_timer.finish() - 
		(time_t)t->_timer.finish() > 0) 
		|| 
		it==&timer_list
		)
		break;
	};
	bits_clr(t->_trait, TimWaiter_DONE);

	list_move_tail(&t->lst, &list_entry(it, TimWaiter, lst)->lst);
};

void WaitServer::postprocessing_timer(TimWaiter* t)
{
	if (bits_mask(t->_trait, TimWaiter_ROUND))
		{
			t->_timer.reset();
			timwaiter_put_to_list(t);
			return;
		};

	if (bits_mask(t->_trait, TimWaiter_AUTOMATIC_CREATED))
		{
			list_del(&t->lst); 	
			delete t;
		}
	else 
		{
			list_del(&t->lst); 	
			bits_set(t->_trait, TimWaiter_DONE);
		};
};

void WaitServer::postprocessing_waiter(Waiter* w)
{
	if (bits_mask(w->_trait, Waiter_NO_UNWAIT))
		{
			return;
		};

	if (bits_mask(w->_trait, Waiter_AUTOMATIC_CREATED))
		{
			list_del(&w->lst); 	
			delete w;
		}
	else 
		{
			list_del(&w->lst); 	
			bits_set(w->_trait, Waiter_DONE);
		};
};

void WaitServer::check_timers()
{
	//dpr("1");
	if (list_empty(&timer_list)) return;
	//dpr("2");
	TimWaiter* t = list_first_entry(&timer_list, TimWaiter, lst);
	if (t->_timer.check(millis())) 
	{
		t->_action(t->_action_data);
		postprocessing_timer(t);
	};			
};

void WaitServer::check_waiters()
{
	Waiter *w, *tmp;
	list_for_each_entry_safe(w, tmp, &wait_list, lst)
	{
		if (w->_condition(w->_condition_data)) 
		{
			w->_action(w->_action_data);
			postprocessing_waiter(w);
		};
	};
};

void WaitServer::check()
{
	check_waiters();
	check_timers();	
};

bool check_u8_flag(void* ptr)
{
	return *static_cast<uint8_t*>(ptr);
};

bool check_stream_available(void* ptr)
{
	//debug_printhex_uint8(static_cast<stream*>(ptr)->available());
	return static_cast<stream*>(ptr)->available();
};

void void_func_execute(void* ptr)
{
	reinterpret_cast<void(*)()>(ptr)();
};

void schedee_run(void* ptr)
{
	//debug_print("run!!!");
	schedee* sch = reinterpret_cast<schedee*>(ptr);
	current_scheduler()->schedee_set_running(sch);
};

void WaitServer::exec_on_u8flag(void(*f)(), uint8_t* flag)
{
	delegate<void, void*> a = void_func_execute;
	void* ad = reinterpret_cast<void*>(f);
	delegate<bool, void*> c = check_u8_flag;
	void* cd = static_cast<void*>(flag);
	uint8_t tr = Waiter_AUTOMATIC_CREATED;
	Waiter* w = new Waiter(a, ad, c, cd, tr);
	waiter_put_to_list(w); 		
};

TimWaiter* WaitServer::schedee_on_simple_timer(schedee* sch, time_t interval)
{
	delegate<void, void*> a = schedee_run;
	void* ad = reinterpret_cast<void*>(sch);
	uint8_t tr = TimWaiter_AUTOMATIC_CREATED;
	TimWaiter* t = new TimWaiter(a, ad, tr);
	t->_timer.set(millis(), interval);
	timwaiter_put_to_list(t); 
	return t;		
};

TimWaiter* WaitServer::delegate_on_simple_timer(delegate<void, void*> d, void* data, TimWaiter* timer, time_t interval)
{
	timer->_action = d;
	timer->_action_data = data;
	timer->_trait = 0;
	timer->_timer.set(millis(), interval);
	timwaiter_put_to_list(timer); 
	return timer;		
};

Waiter* WaitServer::schedee_on_stream_available(schedee* sch, stream* strm)
{
	delegate<void, void*> a = schedee_run;
	void* ad = reinterpret_cast<void*>(sch);
	delegate<bool, void*> c = check_stream_available;
	void* cd = static_cast<void*>(strm);
	uint8_t tr = Waiter_AUTOMATIC_CREATED;
	Waiter* w = new Waiter(a, ad, c, cd, tr);
	waiter_put_to_list(w); 
	return w;		
};

TimWaiter* WaitServer::schedee_on_bias_timer(schedee* sch, TimWaiter* timer, time_t interval)
{
	timer->_action = schedee_run;
	timer->_action_data = reinterpret_cast<void*>(sch);
	timer->_trait = 0;
	timer->_timer.set_bias(interval);
	timwaiter_put_to_list(timer); 
	return timer;		
};

void WaitServer::schedee_on_u8flag(schedee* sch, uint8_t* flag)
{
	delegate<void, void*> a = schedee_run;
	void* ad = reinterpret_cast<void*>(sch);
	delegate<bool, void*> c = check_u8_flag;
	void* cd = static_cast<void*>(flag);
	uint8_t tr = Waiter_AUTOMATIC_CREATED;
	Waiter* w = new Waiter(a, ad, c, cd, tr);
	waiter_put_to_list(w); 		
};

Waiter* wait_autom(uint8_t* flag)
{
	current_scheduler()->schedee_set_wait(current_schedee()); 
	waitserver.schedee_on_u8flag(current_schedee(), flag); 
}

Waiter* wait_autom(stream* strm)
{
	current_scheduler()->schedee_set_wait(current_schedee()); 
	return waitserver.schedee_on_stream_available(current_schedee(), strm); 
}

TimWaiter* msleep_autom(long int a) 
{ 
	current_scheduler()->schedee_set_wait(current_schedee()); 
	return waitserver.schedee_on_simple_timer(current_schedee(), a); 
}

TimWaiter* msleep_autom_bias(TimWaiter* timer, long int a) 
{ 
	current_scheduler()->schedee_set_wait(current_schedee()); 
	return waitserver.schedee_on_bias_timer(current_schedee(), timer, a); 
}