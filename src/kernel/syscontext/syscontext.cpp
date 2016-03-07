#include "syscontext/syscontext.h"

syscontext* _current_syscontext;


	syscontext* current_syscontext(){return _current_syscontext;};
	void current_syscontext(syscontext* _s){_current_syscontext = _s;};