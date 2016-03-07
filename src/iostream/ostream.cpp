
#include "genos/iostream/iostream.h"
#include "genos/iostream/iostream_minimaly.h"
#include <genos/types.h>

namespace genos{
	
	
	ostream& endl 			(ostream& os)			{os.print("\r\n");os.flush();					return os;};;
	ostream& hex 			(ostream& os)			{os.setf(ios_base::hex,ios_base::basefield);	return os;};;
	ostream& oct 			(ostream& os)			{os.setf(ios_base::oct,ios_base::basefield);	return os;};;
	ostream& dec 			(ostream& os)			{os.setf(ios_base::dec,ios_base::basefield);	return os;};;
	ostream& usebase 		(ostream& os)			{os.setf(ios_base::usebase,ios_base::basefield);return os;};;
	ostream& right 			(ostream& os)			{os.setf(ios_base::right,ios_base::adjustfield);return os;};;
	ostream& left 			(ostream& os)			{os.setf(ios_base::left,ios_base::adjustfield);	return os;};;
	ostream& showpos 		(ostream& os)			{os.setf(ios_base::showpos);					return os;};;
	ostream& showbase 		(ostream& os)			{os.setf(ios_base::showbase);					return os;};;
	ostream& alldigits 		(ostream& os)			{os.setf(ios_base::alldigits);					return os;};;
	ostream& noshowpos 		(ostream& os)			{os.clrf(ios_base::showpos);					return os;};;
	ostream& noshowbase 	(ostream& os)			{os.clrf(ios_base::showbase);					return os;};;
	ostream& noboolalpha 	(ostream& os)			{os.clrf(ios_base::boolalpha);					return os;};;
	ostream& noalldigits 	(ostream& os)			{os.clrf(ios_base::alldigits);					return os;};;
	ostream& reset 			(ostream& os)			{os.reset_flags();								return os;};;
	ostream& bin			(ostream& os)			{os.setf(ios_base::bin,ios_base::basefield);	return os;};;
	ostream& flush			(ostream& os)			{os.flush();									return os;};;
	
	ostream_minimaly& endl 	(ostream_minimaly& os)	{os.print("\r\n");								return os;};;
	
	
	
	
};