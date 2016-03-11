#include "genos/debug/debug.h"
#include "kernel/diag.h"

#include "hal/arch.h"

#include "utilxx/functype.h"
#include "genos/terminal/command_list.h"

void task(int, char**) 
{
	debug_print("task");
};

func_t<void,int,char**> fptr;

int main(){
arch_init();
diag_init();

central_cmdlist.add("task", task);


central_cmdlist.find("task", fptr);

char m[10];
strcpy(m, "task 0 678");

argvc_t argvc;
split_argv(m, argvc);

debug_print(argvc.argv[0]);dln;
debug_print(argvc.argv[1]);dln;
debug_print(argvc.argv[2]);dln;



}