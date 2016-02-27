
//by Mirmik 2015

//Реализация sigslot функционала. 
//Библиотека реализует класс sigslot, представляющий собой
//монолист подключенных к слоту делегатов. Само собой,
//в наличии средства редактирования списка.
//Вызов sigslot приводит к последовательному выполнению
//всех подключенных делегатов.

//Библиотека использует динамическую память для хранения делегатов.
//Алгоритм работы медленней других реализаций, разворачивающихся
//на этапе компиляции. В данной реализации вся работа выполняется в 
//реалтайме.

#ifndef GENOS_SIGSLOT_H
	#define GENOS_SIGSLOT_H
	
	#include "genos/sigslot/delegate.h"	
	#include "genos/base_struct/monolist.h"		
	#include "genos/gstl/utility.h"	


	
	//Внутренняя структура. Монолист делегатов.
	template<typename OutputType ,typename ... VarTypes>
	class listed_delegate
	{
	using delegate = RealTimeDelegate::delegate;
	public:
		delegate<OutputType, VarTypes ... > 	dlg;
		monolist_head 							mlist;
	};
	
	
	//Клас слота. Шаблонный класс. 
	//Параметры шаблона задают сигнатуру делегируемых функций. 
	//@1 тип возвращаемый тип.
	//@2 - ... Типы параметров. Доступно неограниченное количество параметров. 
	template<typename OutputType ,typename ... VarTypes>
	class sigslot{
		
		using delegate = RealTimeDelegate::delegate;
		
		public:
		monolist_head head;
		sigslot() : head(&head) {};
		
		
		class Abstract{};
		typedef listed_delegate<OutputType, VarTypes ... > ldlg_t;
		typedef Abstract* delegate_obj_t;			
		typedef OutputType (Abstract::*delegate_mtd_t)(VarTypes ...);
		typedef OutputType (*delegate_fnc_t)(VarTypes ...);
		
		//Очистка списка.
		void clear()
		{
			monolist_head * pos, *n ,*p;
			monolist_for_each_safe(p, pos, n, &head)
			{
				__list_del(p, n);
				delete (monolist_entry(pos, ldlg_t , mlist));
			};
			
		};
		
		//Вызов слота. Оператору передаются параметры
		//в соответствии с сигнатурой. Возвращается 
		//значение последней вызванной функции. 
		//(Не знаю, зачем... Чтоб было.)
		template <typename T>
		T operator()(VarTypes ... arg) volatile 
		{
			T res;
			ldlg_t * pos;
			monolist_for_each_entry(pos,&head,mlist)
			{
				res = pos->dlg(arg ...);
			};		
			return res;
		};
		
		
		
		//Вызов слота. Оператору передаются параметры
		//в соответствии с сигнатурой. Особая реализация для
		//слотов, возвращающих void
		void operator()(VarTypes ... arg) volatile 
		{
			ldlg_t * pos;
			monolist_for_each_entry(pos,&head,mlist)
			{
				pos->dlg(arg ...);
			};		
		};
		
		
		
		//Далее внутренние функции. Реализуют добавление
		//новых делегатов в слот.
		
		void include (OutputType(*func)(VarTypes ...))
		{
			ldlg_t *l = new ldlg_t;
			l->dlg = func;
			ldlg_t * pos;
			monolist_last_entry(pos, &head, mlist);	
			monolist_add(&l->mlist, &pos->mlist);
		};
		
		
		void priority_include (OutputType(*func)(VarTypes ...))
		{
			ldlg_t *l = new ldlg_t;
			l->dlg = func;
			monolist_add(&l->mlist, &head);
		};
		
		template <typename T1, typename T2>
		void include (T1* ptr_obj, OutputType(T2::*mtd)(VarTypes ...))
		{
			ldlg_t *l = new ldlg_t;
			l->dlg = {ptr_obj, mtd};
			ldlg_t * pos;
			monolist_last_entry(pos, &head, mlist);	
			monolist_add(&l->mlist, &pos->mlist);
		};
		
		template <typename T1, typename T2>
		void priority_include (T1* ptr_obj, OutputType(T2::*mtd)(VarTypes ...))
		{
			ldlg_t *l = new ldlg_t;
			l->dlg = {ptr_obj, mtd};
			monolist_add(&l->mlist, &head);
		};
		
		//Добавление функции.
		//@1 указатель на функцию.
		sigslot& operator+= (OutputType(*func)(VarTypes ...)) {include(func);return *this;};	
		
		//Добавление пары-метода.
		//@1 пара, состоящая из объекта и указателя на метод.
		//Вы можете использовать макрос method для создания пары.
		template <typename T1, typename T2>
		sigslot& operator+= (std::pair<T1* , OutputType(T2::*)(VarTypes ...)> pr) 
		{include(pr.first, pr.second);return *this;};	
		
		//Осторожно, черная магия!!!
		//Добавление пары-метода.
		//@1 указатель на объект, метод которого вызывается.
		//@2 мануальное задание указателя на метод класса. 
		template <typename T1>
		sigslot& operator+= (std::pair<T1* , delegate_mtd_t> pr) 
		{
			include(pr.first, 
			horrible_cast<OutputType(Abstract::*)(VarTypes ...),delegate_mtd_t> (pr.second));
		return *this;
		};	
		
		
		//Приоритетное добавление функции. (Функция будет исполняться первой.)
		//@1 указатель на функцию.
		sigslot& operator!= (OutputType(*func)(VarTypes ...)) {priority_include(func);return *this;};	
		
		//Приоритетное добавление пары-метода. (Функция будет исполняться первой.)
		//@1 пара, состоящая из объекта и указателя на метод.
		//Вы можете использовать макрос method для создания пары.
		template <typename T1, typename T2>
		sigslot& operator!= (std::pair<T1* , OutputType(T2::*)(VarTypes ...)> pr) 
		{priority_include(pr.first, pr.second);return *this;};	
		
		//реализация удаления делегата.
		void drop(delegate<OutputType, VarTypes ... >  dlg)
		{
			monolist_head * pos, *n ,*p;
			ldlg_t* ptr;
			
			monolist_for_each_safe(p, pos, n, &head)
			{
				ptr = monolist_entry(pos, ldlg_t , mlist);
				if (dlg == ptr->dlg) 
				{
					__list_del(p, n);
					delete ptr;
					return;
				}
				else p = pos;
			};
		}
		
		//Удаление делегата. 
		//@1 функция, определяющая удаляемый делегат.
		//Если в списке два одинаковых делегата, удалён будет только один.
		sigslot& operator-= (OutputType(*func)(VarTypes ...)) 
		{drop(delegate<OutputType, VarTypes ...>(func));return *this;};	
		
		//Удаление делегата. 
		//@1 пара-метод, определяющая удаляемый делегат.
		//Если в списке два одинаковых делегата, удалён будет только один.
		template <typename T1, typename T2>
		sigslot& operator-= (std::pair<T1* , OutputType(T2::*)(VarTypes ...)> pr) 
		{drop(delegate<OutputType, VarTypes ...> (pr));return *this;};	
		
	};
	
	
	
	
	
#endif