
//by Mirmik 2015

//Непереносимо. Только для G++

//Реализация делегатов.
//Работа с указателями на метод реализуется с помощью horrible_cast.

//В данной реализации делегат является шаблонным классом, что ограничивает
//его возможности одной сигнатурой. Однако, позволяет возвращать результат.

#ifndef GENOS_FUTURE_DELEGATE_H
#define GENOS_FUTURE_DELEGATE_H
	
#include "genos/gstl/horrible_cast.h"
#include "genos/gstl/utility.h"		
#include "genos/debug/debug.h"		
	
class AbstractDelegate {};

template<typename R ,typename ... Args>	
class fastdelegate;
	
//Делегат. Шаблонный класс. 
//Параметры шаблона задают сигнатуру делегируемой функции. 
//@1 тип возвращаемый тип.
//@2 - ... Типы параметров. Доступно неограниченное количество параметров.
template<typename R ,typename ... Args>	
class delegate
{public:
	
	//EXTFUNC - Возможный вариант рассмотрения метода,
	//как функции, параметром которой предшествует указатель.
	//Сомнительная переносимость.
	enum { METHOD, FUNCTION, EXTFUNCTION };
			
	using obj_t 		= AbstractDelegate*;			
	using mtd_t 		= R (AbstractDelegate::*)(Args ...);
	using fnc_t 		= R (*)(Args ...);
	using extfnc_t	 	= R (*)(void* ,Args ...);
	using absmemb_t		= gstd::pair<obj_t , mtd_t>;


	//Соответствует истине и будет работать только в G++
	union method_union
	{
		mtd_t method;
		struct{
			fnc_t function;	
			fnc_t attributes;
		};
	};

	obj_t object;
	method_union method;
			
	//Конструктор пустого делегата.		
	delegate() : object(0) 
	{
		method.method = 0;
	};
			

	//Конструктор копирования
	delegate(const delegate& d)
	{
		object = d.object;
		method.method = d.method.method;
	};
			
	//Бывает, что и с volatile делегатами приходится 
	//работать.
	delegate(volatile const delegate& d)
	: delegate(remove_volatile(d)) {};
			

	//Конструктор перемещения
	//delegate(delegate&& d)
	//{
	//	object = d.object;
	//	method = d.method;
	//};
			
	delegate& operator=(const delegate& d) 
	{
		object = d.object;
		method.method = d.method.method;
		return *this;
	};
			
	//delegate& operator=(delegate&& d) 
	//{
	//	object = d.object;
	//	method = d.method;
	//	return *this;
	//};

	delegate(const fastdelegate<R,Args...>& fd)
	: object(fd.object)
	{
		method.attributes = 0;
		method.function = reinterpret_cast<fnc_t>(fd.extfunction);
	};

	//Конструктор. Делегат метода класса. Ручная инициализация 
	//@1 указатель на объект, метод которого вызывается.
	//@2 указатель на метод.
	//Пример delegate<void, int> d(&a, &A::func);
	template <typename T1, typename T2>
	void set(T1* ptr_obj, R(T2::*mtd)(Args ...))
	{
		object = reinterpret_cast <obj_t> (ptr_obj);
		method.method = horrible_cast<mtd_t, R(T2::*)(Args ...)>(mtd);
	};
			
	void set(fnc_t func)
	{
		object = 0;
		method.attributes = 0;
		method.function = func;
	};
			
	void set(const absmemb_t& pr)
	{
		object = pr.first;
		method.method = pr.second;
	};	


	//BLACK_MAGIC
	void set_ext(extfnc_t func, void* obj)
	{
		object = reinterpret_cast<obj_t>(obj);
		method.attributes = 0;
		method.function = reinterpret_cast<fnc_t>(func);
	};
	///////////////


	//Конструктор. Делегат функции.
	//@1 указатель на функцию.
	delegate(fnc_t func) : object(0)
	{
		method.function = func;
		method.attributes = 0;
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
		uint8_t type = object ? METHOD : FUNCTION;
		switch (type)
		{
			case METHOD: 
			return (object->*method.method)(arg ...);
			
			case FUNCTION: 
			return method.function(arg ...);
			
		//	case EXTFUNCTION: 
		//	return (reinterpret_cast<extfnc_t>(function))(object, arg ...);
		};
	};

	//Сравнение делегатов.
	bool operator==(delegate<R ,Args ... > b)
		{
			return method.method==b.method.method && object==b.object;
		};
};	
		
		//Макрос для создания пары объект - метод.
		//#define d_member(obj, mtd) gstd::make_pair(&obj, &mtd)
		
		
template<typename R ,typename ... Args>	
class fastdelegate
{
	using obj_t 		= AbstractDelegate*;			
	using mtd_t 		= R (AbstractDelegate::*)(Args ...);
	using fnc_t 		= R (*)(Args ...);
	using extfnc_t	 	= R (*)(void* ,Args ...);
	using absmemb_t		= gstd::pair<obj_t , mtd_t>;

	//Соответствует истине и будет работать только в G++
	union method_union
	{
		mtd_t method;
		struct{
			fnc_t function;	
			fnc_t attributes;
		};
	};

public:
	obj_t object;
	extfnc_t extfunction;

public:
	fastdelegate(const fastdelegate& d)
	: object(d.object), extfunction(d.extfunction) {};

	fastdelegate(absmemb_t&& pr)
	{
		object = pr.first;
		extfunction = reinterpret_cast<extfnc_t>(
			horrible_cast<method_union,mtd_t>(pr.second).function
			);
	};

	R operator()(Args ... arg) 
	{ 
		return extfunction(object, arg ...);
	};
};

template<typename R, typename ... Args>
class callback
{
	using fnc_t = R (*)(Args ...);
	fnc_t function;

public:
	callback(fnc_t func): function(func) {};

	R operator()(Args ... args)
	{
		return function(args ...);
	};
};

template<typename T, typename B>
class change_basic{};
	
template<typename T, typename B, typename R, typename ... V>
class change_basic<T, R(B::*)(V...)>
{public:
	using type =  R(T::*)(V...);
};
		
#define delegate_mtd(obj, mtd) gstd::make_pair(reinterpret_cast<AbstractDelegate*>(obj), \
horrible_cast<typename change_basic<AbstractDelegate, decltype(mtd)>::type, decltype(mtd)>(mtd));		
		
#endif