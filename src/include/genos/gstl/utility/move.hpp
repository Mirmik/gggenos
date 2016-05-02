template< class T >
typename gstd::remove_reference<T>::type&& move( T&& t )
{
	return static_cast<typename remove_reference<T>::type&&>(t);
}
	
template<class S> S&& forward(typename remove_reference<S>::type& a) 
{
	return static_cast<S&&>(a);
}

template<class S> S&& forward(typename remove_reference<S>::type&& a)
{
	return static_cast<S&&>(a);
} 

/*
namespace genos{

template<typename T> typename gstd::remove_reference<T>::type* pointer  (T &&v) { return &v; }
template<typename T> typename gstd::remove_reference<T>::type& reference(T &&v) { return  v; }
template<typename T> typename gstd::remove_reference<T>::type&& temporary(T &&v){return  gstd::move(v);}

*/	