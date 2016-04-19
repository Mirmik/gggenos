#ifndef GENOS_TIMER_SERV
	#define GENOS_TIMER_SERV
	
	//Timeserv и Waitserv следует объединить. Поначалу
	//Можно даже не добовлять оптимизацию действия.
	//Предлагаю делать реализацию сервера исключительно на делегатах. 
	//Это не проблема сервера, кого и что он будет вызывать.
	//Для простейших случаев добавить безделегатный функционал.
	//Делегаты опционально застрабировать.
	//Сделать упрощенную сугубо таймерную функциональность.
	//Перейти на реалтаймовые делегаты.
	//Объявление делегатов в основном коде недопустимо.

	#include "genos/defs.h"
	
	#include "genos/time/basic_timer.h"
	#include "genos/datastruct/bits.h"
	#include "genos/datastruct/list.h"
	#include "hal/arch.h"
	
	
	#ifdef DEBUG_MODE
	#include "genos/debug/debug.h"
	#endif
	
	
	#ifdef PROCESS_MODE
	#include "genos/schedproc/schedee_base.h"
	#include "genos/schedproc/scheduler_base.h"
	//#include "genos/syscontext/syscontext.h"
	#endif 
	
	
	#ifdef DELEGATE_MODE
	#include "genos/sigslot/delegate2.h"
	template<typename T1, typename ... T2>
	using tmrserv_d = cdelegate<T1,T2...>;
	#endif 
	
		
		namespace tmr{
			//Типы таймера
			static constexpr uint8_t NULLER = 0x01;
			static constexpr uint8_t DELEGATE_VOID = 0x02;
			static constexpr uint8_t DELEGATE_INT = 0x03;
			static constexpr uint8_t DELEGATE_PTR = 0x04;
			static constexpr uint8_t PROC_WAIT = 0x05;
			static constexpr uint8_t FUNC_VOID = 0x06;
			
			//Маска типов.
			static constexpr uint8_t TMRTYPE = 0x0F;
			
			//Что сделать с таймером, если он сработал?
			static constexpr uint8_t ONCE = 0x00; 	//Удалить таймер
			static constexpr uint8_t ROUND = 0x10;	//Повторно его запустить на тот же интервал времени
			static constexpr uint8_t MANUAL = 0x20;	//Ничего не делать, ждать, что скажут снаружи
			//Маска стратегий
			static constexpr uint8_t STRATEGY = 0x30;
			
			//Флаг, используемый с таймером делегата, ссылающегося на указатель.
			//При удалении таймера вызывает delete по отношению к указателю.
			//(Использовать только при передаче структуры, созданной в качестве параметра). 
			//Не безопасно.
			static constexpr uint8_t DELETEINFO = 0x40;
			
			//Таймер выполнен и извлечен из списка. (В основном для MANUAL стратегии)
			//Позволяет контролировать работу таймера.
			static constexpr uint8_t DONE = 0x80;
		};
		
		class timer;
		
		/////////////////////////////////////////////////////////////////////////////////
		//timer_create создаёт таймер, который сработает через t единиц времени (для Arduino - микросекунда).
		//strategy устанавливается в одну из стратегий - ROUND, ONCE, MANUAL. Также сюда при необходимости передаётся флаш DELETEINFO.
		
		//Создание тестового таймера. Выводит слого таймер в режиме отладки.
		timer* timer_create(time_t t, uint8_t strategy);
		
		//Создание таймера, запускающего void void функцию.
		//timer* timer_create(void(*)(), time_t t, uint8_t strategy);
		
		
		//
		timer* timer_create(void(*ptr)(void), time_t t, uint8_t state);
		#ifdef DELEGATE_MODE
		//Создание таймера, запускающего void void делегат.
		timer* timer_create(tmrserv_d<void>, time_t t, uint8_t strategy);
		
		//Создание таймера, запускающего void int делегат с параметром.
		timer* timer_create(tmrserv_d<void, int> d, int param, time_t t, uint8_t strategy);
		
		//Создание таймера, запускающего void void* делегат с параметром.
		timer* timer_create(tmrserv_d<void, void*> d, void* param, time_t t, uint8_t strategy);
		#endif
		
		#ifdef PROCESS_MODE
		//Создание таймера, реализующего ожидание процесса.
		timer* timer_create_proc_wait
		(schedee_base* proc, scheduler_base* sched, time_t t, uint8_t strategy);
		///////////////////////////////////////////////////////////////////////////////
		#endif
		
		
		/////////////////////////////////////////////////////////////////////////////////
		//Функции для обработки таймеров через указатель на таймер.
		
		//Таймер  tmr сработает через t единиц времени.
		void timer_set(timer* tmr, time_t t);
		
		//Таймер tmr сработает через t единиц времени после окончания ранее установленного интерввала.
		void timer_set_bias(timer* tmr, time_t t);
		
		//Доступ к статусу.
		void timer_trait(timer* tmr, uint8_t state);
		uint8_t timer_trait(timer* tmr);
		
		//Мануальное удаление таймера.
		void delete_timer(timer* tsr);
		
		//Проверяет, выполнился ли таймер.
		bool timer_is_done(timer* tmr);
		
		
		//Основной метод, который проверяет таймеры на срабатывание.
		void timerserv_check();	
		
		//Внутренняя функция. Выполнение таймера.
		void timer_exec(timer* tsr);
		
		//Внутренняя функция. Реализует алгоритм добавления таймера в список.
		void put_timer_to_timerlist(timer* tsr);	
		
		
		
		//Класс таймера.
		class timer
		{public:	 
			basic_timer tmr;
			uint8_t trait;
			///////////////TODO///////////////
			char info[sizeof(size_t) * 4];
			//////////////////////////////////
			list_head tmr_list;
			uint16_t count;
			timer () : trait(0), count(0) {};
			
			//Обертки над функциями мануального управления, 
			//реализованные в виде метода класса.
			void set(time_t t) {timer_set(this, t);};
			void set_bias(time_t t){timer_set_bias(this, t);};
			void set_trait(uint8_t state){timer_trait(this, state);};
			uint8_t get_trait(){return timer_trait(this);};
			bool is_done(){return  timer_is_done(this);};
			void destroy(){delete_timer(this);};
		};
		
		//Список таймеров.
		extern list_head timer_list;
		
		
		////////////TODO///////////////
		#include "genos/time/sysclock.h"
		//typedef int32_t signed_time_t;
		static inline long//auto 
		get_time () {
		return millis();
		}; 
		/////////////////////////////////
		
		timer* msleep_subst(long int  a);
		timer* msleep_subst(timer* t, long int  a);
		timer* msleep_subst_bias(timer* t, long int a);
		timer* msleep_autom(long int a); 
		timer* msleep_autom_bias(timer* t, long int a);
		
	
	
	
	
#endif				
