#ifndef GENOS_delegate2
	#define GENOS_delegate2
	
	
	//Делегаты, на этапе компиляции создающие набор функций под все свои вызовы.
	
	
	template<class Result, typename ... Args>
	class cdelegate
	{
		public:
        typedef Result (*InvokeT)(void *, Args ...);              // указатель на функцию (Invoke())
        static  Result VoidFunc(Args ...) { return Result(); }
		private:
        void *_object;
        InvokeT _callback;
        template<class ObjectT, Result (ObjectT::* Func)(Args...)>
        static Result InvokeObj(void *object, Args ... args){
			return (static_cast<ObjectT*>(object)->* Func)(args ...);
		}
        template<Result (* Func)(Args ...)>
        static Result InvokeFunc(void* object, Args ... args){
			return Func(args ...);
		}
        cdelegate(void *obj, InvokeT func)
		: _object(obj), _callback(func) { }
		
		cdelegate(InvokeT func)
		: _object(0), _callback(func) { }
		
		public:
		
        cdelegate()
		: _object(0), _callback(&InvokeFunc<VoidFunc>)  { }
		
        // вызывает конструктор, 
        // для каждой функции класса создаёт свой Invoke (т.к. специализация шаблона!)
        // и передаёт указатель на Invoke как второй параметр ctor"а.
        template<class ObjectT, Result (ObjectT::* Func)(Args ...)> void set(ObjectT * object){
			_object = object; 
			_callback = &InvokeObj<ObjectT, Func>;
		}
        template<Result (* Func)(Args ...)> void set(){
			_object = 0;
			_callback = &InvokeFunc<Func>;
		}
		
		
		// вызывает конструктор, 
        // для каждой функции класса создаёт свой Invoke (т.к. специализация шаблона!)
        // и передаёт указатель на Invoke как второй параметр ctor"а.
        template<class ObjectT, Result (ObjectT::* Func)(Args ...)> 
		static cdelegate<Result, Args ...> make(ObjectT * object){
			return cdelegate(object, &InvokeObj<ObjectT, Func>);
		}
        template<Result (* Func)(Args ...)> 
		static cdelegate<Result, Args ...> make(){
			return cdelegate(&InvokeFunc<Func>);
		};
		
		
        // вызов Invoke и передача ему объекта для которого он был создан
        inline Result operator()(Args ... args)const{
			return _callback(_object, args ...);
		}
        InvokeT ToCFunction()const{
			return _callback;
		}
        void* Object()const{
			return _object;
		}
		
	};
	template <typename T, T ptr> 
	class delegate_creater{};
	
	template <typename T, typename R, typename ... V, R(T::*ptr)(V...)> 
	class delegate_creater<R(T::*)(V...), ptr>
	{public:
		using delegate_type = cdelegate<R, V...>;
		using basic_type = T;
	};
	
	template <typename R, typename ... V, R(*ptr)(V...)> 
	class delegate_creater<R(*)(V...), ptr>
	{public:
		using delegate_type = cdelegate<R, V...>;
	};
	
	
	#define makedelegate(obj, a) _makedelegate(decltype(a), a, obj)
	#define _makedelegate(ptrT, ptr, obj) delegate_creater<ptrT,ptr>::delegate_type::make<delegate_creater<ptrT,ptr>::basic_type, ptr>(obj)
	
	#define makedfunc(a)  _makedfunc(decltype(a), a)
	#define _makedfunc(ptrT, ptr) delegate_creater<ptrT,ptr>::delegate_type::make<ptr>()
	
	
#endif