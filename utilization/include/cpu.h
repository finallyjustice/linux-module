#ifndef __CPU_H__
#define __CPU_H__

unsigned long cpu_user_time(int c);
unsigned long cpu_nice_time(int c);
unsigned long cpu_sys_time(int c);
unsigned long cpu_idle_time(int c);
unsigned long cpu_iowait_time(int c);
unsigned long cpu_hirq_time(int c);
unsigned long cpu_sirq_time(int c);
unsigned long cpu_steal_time(int c);

#endif
