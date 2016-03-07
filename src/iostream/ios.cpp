


#include <genos/iostream/ios.h>

namespace genos {

static constexpr ios_base::fmtflags startflags = 
ios_base::right | ios_base::showbase | ios_base::boolalpha | ios_base::usebase;

ios_base::ios_base() : _width(0), _base(10) , _flags(nullflags) {reset_flags();};
			
void ios_base::reset_flags ( void ) {_flags = startflags; _width=0; _base=10; };

ios_base::fmtflags ios_base::flags ( ) const {return _flags;};
ios_base::fmtflags ios_base::flags ( ios_base::fmtflags fmtfl ) {ios_base::fmtflags temp = _flags; _flags = fmtfl; return temp;};
void ios_base::setf ( ios_base::fmtflags f ) {_flags = _flags | f ;};
void ios_base::clrf ( ios_base::fmtflags f ) {_flags = _flags & ~f;};
bool ios_base::getf ( ios_base::fmtflags f ) {return (_flags & f)!=0;};
void ios_base::setf ( ios_base::fmtflags fmtfl, ios_base::fmtflags mask ) {_flags=(fmtfl&mask)|(_flags&~mask);};


};