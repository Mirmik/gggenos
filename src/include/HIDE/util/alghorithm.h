#ifndef UTIL_ALGHORITHM
#define UTIL_ALGHORITHM

//Адаптация библиотеки алгоритмов в стиле С.

//TODO Там еще много агоритмов :).


//Правда, если все элементы вернут истину от предиката.
#define alghoritm_all_of(it, first, last, pred)\
	({\
		bool ret = true;\
		for(it = first; it != last; ++it) if(!pred(*it))\
			{\
			ret = false;\
			break;\
			}\
		ret;\
		})

//Правда, если хоть один элемент вернет истину от предиката.
#define alghoritm_any_of(it, first, last, pred)\
	({\
		bool ret = false;\
		for(it = first; it != last; ++it) if(pred(*it))\
			{\
			ret = true;\
			break;\
			}\
		ret;\
		})

//Правда, если ни один элемент не вернет истины от предиката.
#define alghoritm_none_of(it, first, last, pred)\
	({\
		bool ret = true;\
		for(it = first; it != last; ++it) if(!pred(*it))\
			{\
			ret = false;\
			break;\
			}\
		ret;\
		})

//Выполнить для каждого.
#define alghoritm_for_each(it, first, last, invoke)	\
	({for(it = first; it != last; ++it) invoke(*it);})

//Найти первый эквивалентный.
#define alghoritm_find(it, first, last, val)\
	({for(it = first; it != last; ++it) if (*it == val) break;})

//Найти первый, возвращающий истину от предиката.
#define alghoritm_find_if(it, first, last, pred)\
	({for(it = first; it != last; ++it) if (pred(*it)) break;})

//Найти первый, возвращающий ложь от предиката.
#define alghoritm_find_if_not(it, first, last, pred)\
	({for(it = first; it != last; ++it) if (!pred(*it)) break;})

//Посчитать число эквивалентных.
#define alghoritm_count(it, first, last, val)\
	({size_t ret = 0; for(it = first; it != last; ++it) if (*it == val) ret++; ret;})

//Посчитать число возвращающих истину от предиката.
#define alghoritm_count_if(it, first, last, pred)\
	({size_t ret = 0; for(it = first; it != last; ++it) if (pred(*it)) ret++; ret;})


/*
#define alghoritm_find_end(it1, it2, first1, last1, first2, last2)\
	({
typeof(first1) ret = last1;
if (first2 == last2) goto _ret;

while (first1 != last1)
{
	it1 = first1;
	it2 = first2;
	while (*it1 == *it2) {  // or: while (pred(*it1,*it2)) for version (2)
		++it1; ++it2;
		if (it2 == last2) { ret = first1; break; }
		if (it1 == last1) return ret;
	}
	++first1;
};
_ret: ret;
})*/



#endif
