#pragma once

#include "defines.h"

struct boolean{
    int value;
}* boolean;


struct file_rule{
    char _file_name_ [MAX_PATH_SIZE]; // name with whole path
    int _type_; // file or directory
    int _write_access_; // True(1) = has access / False(0) = access restricted
    int _read_access_;
    int _execute_access_;
}* __file_rule;

struct process{
    char name[20];
    int allow;
    int cpu_load;
};


struct user_rule{
    char name[30];
    struct file_rule fr[10];
    int max_disk_space;
    int max_mem_used;
    int max_open_files;
    int max_stack;
    int max_cpu;
    int max_processes;
    int max_files;
    struct process ps[20];
    
    int time_span;
    int time_start;
    
};
