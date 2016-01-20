#include "genos/subst/subst.h"
#include "genos/stack/static_stack.h"
#include "genos/debug/debug.h"

#include "aiop2560.h"

thread first, second , third;
static_stack<300> stck1;
static_stack<300> stck2;

void loop() {
  red_on();
  change_thread(&second);
  red_off();
  change_thread(&second);
}

void loop2() {
  green_change();
  change_thread(&third);
};

void loop3() {
  debug_delay(200000);
  change_thread(&first);
};

void setup() {
  red_mode_out();
  green_mode_out();
  
  init_current_thread(&first, RAMEND);
  set_stack_thread(&second, stck1.top());
  set_stack_thread(&third,  stck2.top());
  
  if (!fork_thread(&second)) while (1) loop2();
  if (!fork_thread(&third))  while (1) loop3();
};

