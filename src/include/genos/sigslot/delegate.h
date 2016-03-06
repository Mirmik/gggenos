
//by Mirmik 2015

//Непереносимо. Только для G++

//Реализация делегатов.
//Работа с указателями на метод реализуется с помощью horrible_cast.

//В данной реализации делегат является шаблонным классом, что ограничивает
//его возможности одной сигнатурой. Однако, позволяет возвращать результат.

#ifndef GENOS_DELEGATE_H
	#define GENOS_DELEGATE_H
	
	#include "genos/sigslot/horrible_cast.h"
//	#include "genos/platform/delegate.h"	
	#include "gstl/utility.h"		
	#include "genos/debug/debug.h"		
	
	class AbstractDelegate{};
			
		//Делегат. Шаблонный класс. 
		//Параметры шаблона задают сигнатуру делегируемой функции. 
		//@1 тип возвращаемый тип.
		//@2 - ... Типы параметров. Доступно неограниченное количество параметров.
		template<typename R ,typename ... Args>	
		class rtdelegate
		{public:
			
			static constexpr uint8_t METHOD =0x01;
			static constexpr uint8_t FUNCTION =0x02;
			static constexpr uint8_t EXTFUNCTION =0x03;
			
			using obj_t 		= AbstractDelegate*;			
			using mtd_t 		= R (AbstractDelegate::*)(Args ...);
			using fnc_t 		= R (*)(Args ...);
			using extfnc_t	 	= R (*)(void* ,Args ...);
			
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
			rtdelegate() : object(0), method(0) {};
			
			rtdelegate(const rtdelegate& d)
			{
				object = d.object;
				method = d.method;
			};
			
			rtdelegate(rtdelegate&& d)
			{
				object = d.object;
				method = d.method;
			};
			
			rtdelegate& operator=(const rtdelegate& d) 
			{
				object = d.object;
				method = d.method;
				return *this;
			};
			
			rtdelegate& operator=(rtdelegate&& d) 
			{
				object = d.object;
				method = d.method;
				return *this;
			};
			
			//Конструктор. Делегат метода класса. Ручная инициализация 
			//@1 указатель на объект, метод которого вызывается.
			//@2 указатель на метод.
			//Пример rtdelegate<void, int> d(&a, &A::func);
			template <typename T1, typename T2>
			rtdelegate(T1* ptr_obj, R(T2::*mtd)(Args ...))
			{
				object = reinterpret_cast <obj_t> (ptr_obj);
				method = horrible_cast<mtd_t, R(T2::*)(Args ...)>(mtd);
			};	
			
			//Конструктор. Делегат метода класса. Ручная инициализация 
			//@1 указатель на объект, метод которого вызывается.
			//@2 указатель на метод.
			//Пример rtdelegate<void, int> d(&a, &A::func);
			template <typename T1, typename T2>
			void set(T1* ptr_obj, R(T2::*mtd)(Args ...))
			{
				object = reinterpret_cast <obj_t> (ptr_obj);
				method = horrible_cast<mtd_t, R(T2::*)(Args ...)>(mtd);
			};
			
			//Конструктор. Делегат метода класса. Для использования в delegate_method
			//@1 пара, состоящая из объекта и указателя на метод.
			//Вы можете использовать макрос method для создания пары.
			//Пример rtdelegate<void, int> d(method(a, A::func));
			//template <typename T1, typename T2>
			rtdelegate(std::pair<obj_t , mtd_t>&& pr)
			{
				object = pr.first;
				method = pr.second;
			};	
			
			//Осторожно, черная магия!!!
			//Конструктор. Делегат метода класса.
			//@1 указатель на объект, метод которого вызывается.
			//@2 мануальное задание указателя на метод класса. 
			//Пример rtdelegate<void, int> d(method(a, 0x00010002)); 
			//(Смотри стандартную реализацию виртуальных функций)
			rtdelegate(std::pair<void* , delegate_mtd_t> pr) 
			{
				object = reinterpret_cast <obj_t> (pr.first);
				method = horrible_cast<mtd_t, delegate_mtd_t>(pr.second);
			};			
			
			
			
			
			//Конструктор. Делегат функции.
			//@1 указатель на функцию.
			rtdelegate(fnc_t func) : attributes(0), object(0)
			{
				function = func;
			};	
			
			rtdelegate& operator=(fnc_t func) 
			{
				object = 0;
				attributes = 0;
				function = func;	
				return *this;
			};
			
			
			//Вызов делегата. В зависимости от типа делегата вызывается одна
			//из двух реализаций. Оператору передаются параметры 
			//в соответствии с сигнатурой делегата.
			R operator()(Args ... arg) {
				//uint8_t type = attributes ? METHOD : object ? EXTFUNCTION : FUNCTION;
				uint8_t type = object ? METHOD : FUNCTION;
				switch (type)
				{
					case METHOD: 
					return (object->*method)(arg ...);
					
					case FUNCTION: 
					return function(arg ...);
					
					//case EXTFUNCTION: 
					//return (reinterpret_cast<extfnc_t>(function))(object, arg ...);
				};
			};
			
			//Сравнение делегатов.
			bool operator==(rtdelegate<R ,Args ... > b)
			{return (method==b.method && function==b.function && object==b.object) ? true : false;};
		};	
		
		//Макрос для создания пары объект - метод.
		//#define method(obj, mtd) std::make_pair(&obj, &mtd)
		
		
	
	
	
	template<typename T, typename B>
		class change_basic{};
		
		
		template<typename T, typename B, typename R, typename ... V>
		class change_basic<T, R(B::*)(V...)>
		{public:
			using type =  R(T::*)(V...);
		};
		
		
	//	#define delegate_method(obj, mtd) std::make_pair(reinterpret_cast<AbstractDelegate*>(obj),\
		horrible_cast<typename change_basic<AbstractDelegate, decltype(mtd)>::type, decltype(mtd)>(mtd));
		
	//	#define set_delegate_method(d, obj, mtd) d = delegate_method(obj, mtd)
	//	#define set_delegate_function(d, func) d = func
		
		
		
#endif