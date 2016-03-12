

#include "genos/schedproc/schedee_base.h"
#include "genos/schedproc/scheduler_base.h"

schedee* _current_schedee  = nullptr;
schedee* current_schedee(){return _current_schedee;};
void current_schedee(schedee* proc){_current_schedee = proc;};

scheduler* _current_scheduler = nullptr;
scheduler* current_scheduler(){return _current_scheduler;};
void current_scheduler(scheduler* _sched){_current_scheduler = _sched;};