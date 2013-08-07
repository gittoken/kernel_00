%top{

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "structures.h"
#include "defines.h"

char _user_ [MAX_NAME_SIZE] = ".";
char _group_ [MAX_NAME_SIZE] = ".";
char _file_ [MAX_PATH_SIZE] = "-";
char _directory_ [MAX_PATH_SIZE] = "-";

int _write_access_;
int _read_access_;
int _exec_access_;

struct file_rule* fr;

int _max_disk_space_;
int _max_mem_use_;
int _max_no_files_open_;
int _max_stack_size_;
int _max_cpu_;
int _max_no_process_;
int _max_no_files_created_;
char _process_name_ [25];
int _time_now_;
int _starting_with_;

struct user_rule ur[100];
int no_rules;
}

%{

void Init(){
    _user_[0] = '\0';
    _group_[0] = '\0';
    _file_[0] = '\0';
    _directory_[0] = '\0';

    _write_access_ = TRUE;
    _read_access_ = TRUE;
    _exec_access_ = TRUE;

        
    _max_disk_space_ = -1;
    _max_mem_use_ = -1;
    _max_no_files_open_ = -1;
    _max_stack_size_ = -1;
    _max_cpu_ = -1;
    _max_no_process_ = -1;
    _max_no_files_created_ = -1;
    _process_name_[0] = '.';
    _time_now_ = -1;
    _starting_with_ = -1;
    
    no_rules = 0;

}


int SaveConfig(){
    int result = TRUE;
    int i;
    
    FILE * save_file = fopen("config.txt","w");
    
    if(save_file == NULL){
	printf("File 'config.txt' cannot be found!\n");
	result = FALSE;
    }else{
	fprintf(save_file,"%i\n",no_rules);
	
	for(i = 0; i<no_rules; i++){
	    if(_user_[0] != '\0'){ // avem user
		fprintf(save_file,"user\n");
		fprintf(save_file,"%s", _user_);
	    }
	    else if(_group_[0] != '\0'){ // avem grup
		fprintf(save_file,"group\n");
		fprintf(save_file,"%s", _group_);
	    }
	    else
		return FALSE;
	}
	fclose(save_file);
    }
    return result;
}

int KeepRule(){
    if(_user_[0] != '\0'){ // avem user
	strcpy(ur[no_rules].name, _user_);
    }
    else if(_group_[0] != '\0'){ // avem grup
	strcpy(ur[no_rules].name, _group_);
    }
    else{
	printf("Eroare! User/Group not set!\n");
	return FALSE;
    }
    no_rules++;
}

int LoadConfig(){
    
    int result = TRUE;
    return result;
}

//-------------------------------------------
void ResetGroup(){
    bzero(_group_, MAX_NAME_SIZE);
    _group_[0] = '\0';
}


void ResetUser(){
    bzero(_user_, MAX_NAME_SIZE);
    _user_[0] = '\0';
}
//-------------------------------------------

void SetUser(){
    bzero(_user_, MAX_NAME_SIZE);
    strncpy(_user_, yytext, MAX_NAME_SIZE-1);
    ResetGroup();
}

void SetGroup(){
    bzero(_group_, MAX_NAME_SIZE);
    strncpy(_group_, yytext, MAX_NAME_SIZE-1);
    ResetUser();
}

//--------------------------------------------

int VerifyFile(char * file_name){
    int result = TRUE;
    printf("Opening : %s ...\n",file_name);
    FILE * file_test = fopen(file_name,"r");
    // find /. -name "SOA"
    
    if(file_test == NULL){
	printf("File %s cannot be found!\n",yytext);
	result = FALSE;
    }else{
	fclose(file_test);
    }
    return result;
}

void ResetFile(){
    bzero(_file_, MAX_PATH_SIZE);
    _file_[0] = '\0';
}


void ResetDirectory(){
    bzero(_directory_, MAX_PATH_SIZE);
    _directory_[0] = '\0';
}

int SetFile(){
    if(VerifyFile(yytext) == TRUE){
	bzero(_file_, MAX_PATH_SIZE);
	strncpy(_file_, yytext, MAX_PATH_SIZE);
	ResetDirectory();
	return TRUE;
    }
    return FALSE;
}


int SetDirectory(){
    if(VerifyFile(yytext) == TRUE){
	bzero(_directory_, MAX_PATH_SIZE);
	strncpy(_directory_, yytext, MAX_PATH_SIZE);
	ResetFile();
	return TRUE;
    }
    return FALSE;
}

void SetRestrictions(){
    _exec_access_ = TRUE;
    _write_access_ = TRUE;
    _read_access_ = TRUE;
    if(strstr(yytext,"x")!=NULL){
	_exec_access_ = FALSE;
    }
    if(strstr(yytext,"r")!=NULL){
	_read_access_ = FALSE;
    }
    if(strstr(yytext,"w")!=NULL){
	_write_access_ = FALSE;
    }

}

void List_current(){
    printf("User  : '%s'\n",_user_);
    printf("Group : '%s'\n",_group_);
}

void Clear(){
    Init();
    printf("Cleared!");
}

void start(){

    FILE *fpipe;
    char* command = "insmod ../kernel_mode_module/kernel_module.ko";
    if( !(fpipe = (FILE*)popen(command,"r"))){
	printf("ERROR - Cannot start module!\n");
    }
    return;
}

void stop(){
    FILE *fpipe;
    char* command = "rmmod kernel_module";
    if( !(fpipe = (FILE*)popen(command,"r"))){
	printf("ERROR - Cannot remove module!\n");
    }
    return;
}
%}


digit		[0-9]
number		{digit}+
char		[a-zA-Z]
space		[ \t]
notspace        [^" "]
user_           [a-zA-Z][{digit}{char}]*
group		{char}+

%x USER_
%x GROUP_
%x FILE_
%x DIRS_
%x GET_RESTRICTIONS_

%x DISK_SPACE_
%x MAX_MEM_USED_
%x MAX_NO_FILE_
%x MAX_STACK_SIZE_
%x MAX_CPU_
%x MAX_NO_PROCESS_
%x MAX_NO_FILES_
%x PER_PROCESS_
%x OPT_TIME_
%x OPT_DATE_

%x MGMT_LIST_
%x MGMT_DEL_

%%

user{space}+		{
			    //printf("USERUL = >%s< !",yytext);
			    BEGIN(USER_);
			}

delete{space}+		{
			    //printf("Numar del = >%s< !",yytext);
			    BEGIN(MGMT_DEL_);
			    
			}
list[\n]		{}

list{space}+		{
			    BEGIN(MGMT_LIST_);
			}

group{space}+		{
			    //printf("GRUPUL = >%s< !",yytext);
			    BEGIN(GROUP_);
			}
file{space}+		{
			    BEGIN(FILE_);
			}
dir{space}+		{
			    BEGIN(DIRS_);
			}
max_disk{space}+	{
			    BEGIN(DISK_SPACE_);
			}
max_mem{space}+		{
			    BEGIN(MAX_MEM_USED_);
			}
max_open{space}+	{
			    BEGIN(MAX_NO_FILE_);
			}
max_stack{space}+	{
			    BEGIN(MAX_STACK_SIZE_);
			}
max_cpu{space}+		{
			    BEGIN(MAX_CPU_);
			}
max_create{space}+	{
			    BEGIN(MAX_NO_FILES_);
			}
process{space}+		{
			    BEGIN(PER_PROCESS_);
			}

time_now{space}+	{
			    BEGIN(OPT_TIME_);
			}
time{space}+		{
			    BEGIN(OPT_DATE_);
			}

"exit"|"quit"		{
			    exit(0);
			}
start			{start();}
stop			{stop();}
save[\n]		{SaveConfig();}
keep			{KeepRule();}
load			{LoadConfig();}
update			{
			    syscall(SIGUSR2,"/SOA/1234");
			}
current			{List_current();}
clear			{Clear();}

[ \t]			{
			    printf("");
			}
[\n]			{
			    printf(">");
			}



<MGMT_DEL_>[^ \n\t]+/[ \t\n]	{
			    if(atoi(yytext) != 0)
				printf("Sterge numarul '%s'\n",yytext);
			    else
				printf("Variable NOT a number\n");
			    BEGIN(INITIAL);
			}
<MGMT_LIST_>[^ \n\t]+/[ \t\n]	{
			    int aux = atoi(yytext);
			    if((aux !=0)&&(aux<no_rules))
				printf("Rule no. : %s\n",yytext);
				//aux-1;
			    else{
				if (no_rules == 0){
				    printf("No rules available.\n");
				}else
				    printf("Please input a number between 1 and %i\n",no_rules);
			    }
			    BEGIN(INITIAL);
			}

<USER_>[^ \n\t]+	{
			    printf("Read user  : '%s'\n",yytext);
			    SetUser();
			    BEGIN(INITIAL);
			}		
<GROUP_>[^ \n\t]+	{
			    printf("Read group : '%s'\n",yytext);
			    SetGroup();
			    BEGIN(INITIAL);
			}

<FILE_>[^ \n\t]+	{
			    printf("Read file : '%s'\n",yytext);
			    if(SetFile() == TRUE)
				BEGIN(GET_RESTRICTIONS_);
			    else
				BEGIN(INITIAL);
			}
			

<DIRS_>[^ \n\t]+	{
			    printf("Read dir : '%s'\n",yytext);
			    SetDirectory();
			    BEGIN(INITIAL);
			}


<GET_RESTRICTIONS_>[ \t]*[x|r|w]+[ \t]*	{
			    SetRestrictions();
			    BEGIN(INITIAL);
			}

<MAX_MEM_USED_>[^ \n\t]+/[ \t\n] {
				    BEGIN(INITIAL);
				}

<MAX_NO_FILE_>[^ \n\t]+/[ \t\n]	{
			    if(atoi(yytext) != 0)
				printf("Max file no : '%s'\n",yytext);
			    else
				printf("Variable NOT a number\n");
			    BEGIN(INITIAL);
			}


<MAX_STACK_SIZE_>[^ \n\t]+/[ \t\n]	{
				    BEGIN(INITIAL);
				    }

<MAX_CPU_>[^ \n\t]+/[ \t\n]	{
				    BEGIN(INITIAL);
				}
<MAX_NO_PROCESS_>[^ \n\t]+/[ \t\n]	{
				    BEGIN(INITIAL);
				    }
<MAX_NO_FILES_>[^ \n\t]+/[ \t\n]	{
				    BEGIN(INITIAL);
				}
<PER_PROCESS_>[^ \n\t]+/[ \t\n]	{
				    BEGIN(INITIAL);
				}
<OPT_TIME_>[^ \n\t]+/[ \t\n]	{
				    BEGIN(INITIAL);
				}
<OPT_DATE_>[^ \n\t]+/[ \t\n]	{
				    BEGIN(INITIAL);
				}



<INITIAL>[^ \n\t]+	{printf("EROARE - Incorrect command!\n");}

%%

int main(int argc, char* argv[])
{
  Init();
  printf("Give me your input:\n>");
  yylex();
}

