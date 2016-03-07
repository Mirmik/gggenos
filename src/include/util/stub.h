#ifndef GENOS_STUB_H
#define GENOS_STUB_H

static void do_nothing(){};

#include "genos/debug/debug.h"
static void error_stub(){debug_panic("error_stub");};

#endif