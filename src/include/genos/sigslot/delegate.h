
//by Mirmik 2015

//Непереносимо. Только для G++

//Реализация делегатов.
//Работа с указателями на метод реализуется с помощью horrible_cast.

//В данной реализации делегат является шаблонным классом, что ограничивает
//его возможности одной сигнатурой. Однако, позволяет возвращать результат.

#ifndef GENOS_DELEGATE_H
	#define GENOS_DELEGATE_H
	
	#include "genos/gstl/horrible_cast.h"
	#include "genos/gstl/utility.h"		
	#include "genos/debug/debug.h"		
	
	class AbstractDelegate {};
			
		//Делегат. Шаблонный класс. 
		//Параметры шаблона задают сигнатуру делегируемой функции. 
		//@1 тип возвращаемый тип.
		//@2 - ... Типы параметров. Доступно неограниченное количество параметров.
		template<typename R ,typename ... Args>	
		class delegate
		{public:
			
			static constexpr uint8_t METHOD =0x01;
			static constexpr uint8_t FUNCTION =0x02;

			//Возможный вариант рассмотрения метода,
			//как функции, параметром которой предшествует указатель.
			//Сомнительная переносимость.
			static constexpr uint8_t EXTFUNCTION =0x03;
			
			using obj_t 		= AbstractDelegate*;			
			using mtd_t 		= R (AbstractDelegate::*)(Args ...);
			using fnc_t 		= R (*)(Args ...);
			using extfnc_t	 	= R (*)(void* ,Args ...);
			using absmemb_t		= gstd::pair<obj_t , mtd_t>;
			
			obj_t object;
			
			//Соответствует истине и будет работать только в G++
			union{
				mtd_t method;
				struct{
					fnc_t function;	
					fnc_t attributes;
				};
			};
			
			//Конструктор пустого делегата.		
			delegate() : object(0), method(0) {};
			

			//Конструктор копирования
			delegate(const delegate& d)
			{
				object = d.object;
				method = d.method;
			};
			
			delegate(volatile const delegate& d)
			{
				object = d.object;
				method = d.method;
			};
			

			//Конструктор перемещения
			delegate(delegate&& d)
			{
				object = d.object;
				method = d.method;
			};
			
			delegate& operator=(const delegate& d) 
			{
				object = d.object;
				method = d.method;
				return *this;
			};
			
			delegate& operator=(delegate&& d) 
			{
				object = d.object;
				method = d.method;
				return *this;
			};


			//Конструктор. Делегат метода класса. Ручная инициализация 
			//@1 указатель на объект, метод которого вызывается.
			//@2 указатель на метод.
			//Пример delegate<void, int> d(&a, &A::func);
			template <typename T1, typename T2>
			void set(T1* ptr_obj, R(T2::*mtd)(Args ...))
			{
				object = reinterpret_cast <obj_t> (ptr_obj);
				method = horrible_cast<mtd_t, R(T2::*)(Args ...)>(mtd);
			};
			
			void set(fnc_t func)
			{
				object = 0;
				attributes = 0;
				function = func;
			};
			
			void set(const absmemb_t& pr)
			{
				object = pr.first;
				method = pr.second;
			};	


			//BLACK_MAGIC
			void set_ext(extfnc_t func, void* obj)
			{
				object = reinterpret_cast<obj_t>(obj);
				attributes = 0;
				function = reinterpret_cast<fnc_t>(func);
			};
			///////////////


			//Конструктор. Делегат функции.
			//@1 указатель на функцию.
			delegate(fnc_t func) : attributes(0), object(0)
			{
				function = func;
			};	

			//Конструктор. Делегат метода класса. Ручная инициализация 
			//@1 указатель на объект, метод которого вызывается.
			//@2 указатель на метод.
			//Пример delegate<void, int> d(&a, &A::func);
			template <typename T1, typename T2>
			delegate(T1* ptr_obj, R(T2::*mtd)(Args ...))
			{
				set(ptr_obj, mtd);
			};	
			
			//Конструктор. Делегат метода класса. Для использования в delegate_method
			//@1 пара, состоящая из объекта и указателя на метод.
			//Вы можете использовать макрос method для создания пары.
			//Пример delegate<void, int> d(method(a, A::func));
			//template <typename T1, typename T2>
			delegate(absmemb_t&& pr)
			{
				set(pr);
			};	


			delegate& operator=(fnc_t func) 
			{
				set(func);	
				return *this;
			};

			delegate& operator=(const absmemb_t& pr) 
			{
				set(pr);	
				return *this;
			};

			//Осторожно, черная магия!!!
			//Конструктор. Делегат метода класса.
			//@1 указатель на объект, метод которого вызывается.
			//@2 мануальное задание указателя на метод класса. 
			//Пример delegate<void, int> d(method(a, 0x00010002)); 
			//(Смотри стандартную реализацию виртуальных функций)
			/*delegate(gstd::pair<void* , delegate_mtd_t> pr) 
			{
				object = reinterpret_cast <obj_t> (pr.first);
				method = horrible_cast<mtd_t, delegate_mtd_t>(pr.second);
			};	*/		
			
			
			
			
			//Вызов делегата. В зависимости от типа делегата вызывается одна
			//из двух реализаций. Оператору передаются параметры 
			//в соответствии с сигнатурой делегата.
			R operator()(Args ... arg) {

				//debug_printhex_uint32((uint32_t)object);dln;
				//while(1);

				//uint8_t type = attributes ? METHOD : object ? EXTFUNCTION : FUNCTION;
				uint8_t type = object ? METHOD : FUNCTION;
				switch (type)
				{
					case METHOD: 
					return (object->*method)(arg ...);
					
					case FUNCTION: 
					return function(arg ...);
					
				//	case EXTFUNCTION: 
					//debug_print("EXTFUNC");
				//	return (reinterpret_cast<extfnc_t>(function))(object, arg ...);
				};
			};
			
			//Сравнение делегатов.
			bool operator==(delegate<R ,Args ... > b)
			{return (method==b.method && function==b.function && object==b.object) ? true : false;};
		};	
		
		//Макрос для создания пары объект - метод.
		//#define d_member(obj, mtd) gstd::make_pair(&obj, &mtd)
		
		
	
	
	
		template<typename T, typename B>
		class change_basic{};
		
		
		template<typename T, typename B, typename R, typename ... V>
		class change_basic<T, R(B::*)(V...)>
		{public:
			using type =  R(T::*)(V...);
		};
		
		
		#define delegate_mtd(obj, mtd) gstd::make_pair(reinterpret_cast<AbstractDelegate*>(obj), \
		horrible_cast<typename change_basic<AbstractDelegate, decltype(mtd)>::type, decltype(mtd)>(mtd));
		
	//	#define set_delegate_method(d, obj, mtd) d = delegate_method(obj, mtd)
	//	#define set_delegate_function(d, func) d = func
		
		
		
#endif