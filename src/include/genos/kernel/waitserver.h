#ifndef GENOS_WAITSERVER
#define GENOS_WAITSERVER

#include "genos/container/dlist_head.h"
#include "genos/sigslot/delegate.h"
//#include "defines/time_t.h"
#include "genos/kernel/time.h"
#include "genos/time/basic_timer.h"
#include "genos/schedproc/schedee_base.h"
#include "genos/schedproc/scheduler_base.h"
#include "genos/io/stream.h"


constexpr uint8_t TimWaiter_DONE = 0x01;
constexpr uint8_t TimWaiter_AUTOMATIC_CREATED = 0x08;
constexpr uint8_t TimWaiter_ROUND = 0x10;

constexpr uint8_t Waiter_DONE = 0x01;
constexpr uint8_t Waiter_AUTOMATIC_CREATED = 0x08;
constexpr uint8_t Waiter_NO_UNWAIT = 0x10;

constexpr uint8_t NoneWaiter_DONE = 0x01;
constexpr uint8_t NoneWaiter_AUTOMATIC_CREATED = 0x08;
constexpr uint8_t NoneWaiter_NO_UNWAIT = 0x10;

//Базовый класс вэйтеров. 
//Поля action и action_data сохраняют данные о выполняемом действии.
//В поле trait сохраняются опции, связанные с выполнением.
struct BasicWaiter
{
	dlist_head lst;
	delegate<void, void*> _action;
	void* _action_data;
	uint8_t _trait;
	
	BasicWaiter( );
	BasicWaiter( delegate<void, void*> action, void* action_data,
		uint8_t trait);
};

struct Waiter : public BasicWaiter
{
	delegate<bool, void*> _condition;
	void* _condition_data;

	Waiter ();
	Waiter( delegate<void, void*> action, void* action_data,
		delegate<bool, void*> condition, void* condition_data,
		uint8_t trait);
};

struct TimWaiter : public BasicWaiter
{
	BasicTimer _timer;

	TimWaiter ();
	TimWaiter (	delegate<void, void*> action, void* action_data,
		uint8_t trait );
};

struct NoneWaiter : public BasicWaiter
{
	NoneWaiter ();
	NoneWaiter ( delegate<void, void*> action, void* action_data, 
		uint8_t trait );
};

class WaitServer 
{
private:
	dlist_head wait_list;
	dlist_head timer_list;
	dlist_head none_list;

	void postprocessing_waiter(Waiter* w);
	void postprocessing_timer(TimWaiter* t);
	void postprocessing_nonewaiter(NoneWaiter* nw);
public:
	
	void waiter_put_to_list(Waiter* w);
	void timwaiter_put_to_list(TimWaiter* t);
	void nonewaiter_put_to_list(NoneWaiter* w);

	void check();
	void check_waiters();
	void check_timers();

	void unwait(Waiter* w);
	void unwait(TimWaiter* t);
	void unwait(NoneWaiter* t);

	//void unwait(Waiter* w);
	//void unwait(TimWaiter* t);
	void force_end(NoneWaiter* t);

	//utility:
	void exec_on_u8flag(void(*f)(), uint8_t* flag);
	//void method_on_u8flag(void(*f)(), uint8_t* flag);

	Waiter* schedee_on_u8flag(schedee* sch, uint8_t* flag);
	TimWaiter* schedee_on_simple_timer(schedee* sch, time_t interval);
	TimWaiter* schedee_on_simple_timer(schedee* sch, TimWaiter* timer, time_t interval);
	TimWaiter* schedee_on_bias_timer(schedee* sch, TimWaiter* timer, time_t interval);

	TimWaiter* delegate_on_simple_timer(delegate<void, void*>, void* data, TimWaiter* timer, time_t interval);

	NoneWaiter* schedee_on_external(schedee* sch);

	Waiter* schedee_on_stream_available(schedee* sch, stream* strm);

};

//Стандартные функции проверки условий.
bool check_u8_flag(void* ptr);
bool check_stream(void* ptr);

extern WaitServer waitserver;

Waiter* wait_autom(uint8_t* flag);
Waiter* wait_autom(stream* strm);
TimWaiter* msleep_autom(long int a);
TimWaiter* msleep_autom_bias(TimWaiter* timer, long int a);

TimWaiter* msleep(long int a);
TimWaiter* msleep_bias(TimWaiter* timer, long int a);
Waiter* wait(uint8_t* flag);
Waiter* wait(volatile uint8_t* flag);
Waiter* wait(stream* strm);


#endif