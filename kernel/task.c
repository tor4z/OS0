#include <sys/task.h>
#include <kio.h>
#include <string.h>
#include <klib.h>
#include "global.h"


static void task_a();
static void task_b();
static void task_c();


void setup_tasks()
{
    strcpy(tasks[0].name, "taskA");
    tasks[0].ptr = (uint32_t)task_a;
    tasks[0].stack_size = STACK_SIZE;
    tasks[0].dpl = DPL1;
    tasks[0].rpl = RPL1;
}


void setup_utasks()
{
    strcpy(user_tasks[0].name, "taskB");
    user_tasks[0].ptr = (uint32_t)task_b;
    user_tasks[0].stack_size = STACK_SIZE;
    user_tasks[0].dpl = DPL3;
    user_tasks[0].rpl = RPL3;

    strcpy(user_tasks[1].name, "taskC");
    user_tasks[1].ptr = (uint32_t)task_c;
    user_tasks[1].stack_size = STACK_SIZE;
    user_tasks[1].dpl = DPL3;
    user_tasks[1].rpl = RPL3;
}


static void dummy_sleep()
{
    uint32_t n = 99999;
    while (n)
        --n;
}


static void task_a()
{
    while (1)
    {
        kputs("task a, sys task");
        dummy_sleep();
    }
}


static void task_b()
{
    while(1)
    {
        kputs("task b, a user task");
        dummy_sleep();
    }
}


static void task_c()
{
    while (1)
    {
        kputs("task c, a user task");
        dummy_sleep();
    }
}
