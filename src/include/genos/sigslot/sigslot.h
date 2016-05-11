
//by Mirmik 2015

//Реализация sigslot функционала. 
//Библиотека реализует класс sigslot, представляющий собой
//монолист подключенных к слоту делегатов.
//Вызов sigslot приводит к последовательному выполнению
//всех подключенных делегатов.
//Библиотека использует динамическую память для хранения делегатов.

#ifndef GENOS_SIGSLOT_H
#define GENOS_SIGSLOT_H

#include "genos.h"
#include "genos/sigslot/delegate.h"	
#include "genos/container/mvector.h"		
#include "genos/gstl/utility.h"	
#include "genos/gstl/algorithm.h"

namespace genos {

template<typename R ,typename ... Args>
class sigslot{
	public:

	using dlg_t = delegate<R, Args ... >;
	using obj_t = AbstractDelegate*;			
	using mtd_t = R (AbstractDelegate::*)(Args ...);
	using fnc_t = R (*)(Args ...);
	using absmemb_t	= gstd::pair<obj_t , mtd_t>;

	mvector<dlg_t> vect;

	sigslot() : vect() {};
	
	void operator()(Args ... args) 
	{
		for(auto &d : vect)
		{
			d(args ...);
		};		
	};
	
	unsigned int length() const
	{
		return vect.length();
	}

	void include (const dlg_t& d)
	{
		vect.push_back(d);
	};
	
	void include (const fnc_t& func)
	{
		vect.push_back(dlg_t(func));
	};
	
	template <typename T1, typename T2>
	void include (T1* obj, R(T2::*mtd)(Args ...))
	{
		vect.push_back(dlg_t(delegate_mtd(obj, mtd)));
	};
	
	void include (absmemb_t&& pr)
	{
		vect.push_back(dlg_t(pr));
	};
	
	void priority_include (const dlg_t& d)
	{
		vect.push_front(d);
	};
	
	void priority_include (const fnc_t& func)
	{
		vect.push_front(dlg_t(func));
	};
	
	template <typename T1, typename T2>
	void priority_include (T1* obj, R(T2::*mtd)(Args ...))
	{
		vect.push_front(dlg_t(delegate_mtd(obj, mtd)));
	};
	
	void priority_include (absmemb_t&& pr)
	{
		vect.push_front(dlg_t(pr));
	};
		
	sigslot& operator+= (dlg_t&& dlg) 
	{
		include(dlg);
		return *this;
	}	
	
	sigslot& operator+= (const dlg_t& dlg) 
	{
		include(dlg);
		return *this;
	}	

	sigslot& operator+= (const fnc_t& func) 
	{
		include(func);
		return *this;
	}	
	
	sigslot& operator+= (absmemb_t&& pr) 
	{
		include(gstd::move(pr));
		return *this;
	};	

	sigslot& operator!= (dlg_t&& dlg) 
	{
		priority_include(dlg);
		return *this;
	}	

	sigslot& operator!= (const dlg_t& dlg) 
	{
		priority_include(dlg);
		return *this;
	}	

	sigslot& operator!= (const fnc_t& func) 
	{
		priority_include(func);
		return *this;
	}	
	
	sigslot& operator!= (absmemb_t&& pr) 
	{
		priority_include(gstd::move(pr));
		return *this;
	};

	unsigned char remove(const dlg_t& dlg)
	{
		auto begin = vect.begin();
		auto end = vect.end(); 
		auto it = gstd::find(begin, end, dlg);		
		if (it == end) return 0; 
		vect.remove(it);
		return 1;
	};

	unsigned char remove(const fnc_t& func)
	{
		remove(dlg_t(func));
	};

	unsigned char remove(absmemb_t&& mtd)
	{
		remove(dlg_t(mtd));
	};

	sigslot& operator-= (const dlg_t& dlg) 
	{
		remove(dlg);
		return *this;
	}	

	sigslot& operator-= (dlg_t&& dlg) 
	{
		remove(dlg);
		return *this;
	}	

	sigslot& operator-= (const fnc_t& func) 
	{
		remove(func);
		return *this;
	}	
	
	sigslot& operator-= (absmemb_t&& mtd) 
	{
		remove(gstd::move(mtd));
		return *this;
	};	

	void clear()
	{
		vect.invalidate();
	};

	string to_info()
	{
		string str;
		str.reserve(128);
		str << "sigslot:\r\n" <<
		"length: " << length() << "\r\n";
		return str;
	};
};

};

#endif