#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_log.h"
#include "http_main.h"
#include "http_protocol.h"
#include "http_request.h"
#include "util_script.h"
#include "http_connection.h"
#include <stdlib.h>
#include <stdio.h>
#include "apr_strings.h"
#define COMPDIR "/tmp/csedir"
int generate = 0;
char* read_entire_file(FILE* cmd)
{
	char * str;
	int maxsz = 1;
	int len = 0;
	str = malloc(sizeof(char)*maxsz);
	char b[1];
	while(fread(b,1,1,cmd)){
		str[len++]=b[0];
		if(len >= maxsz-1){
			str=(char*)realloc(str,sizeof(char)*maxsz*2);
			maxsz *=2;
		}

	}
	str[len]=0;
	return str;
}
char * full_path(char* tmpdir, char*filedir)
{
	int len = strlen(tmpdir)+strlen(filedir);
	char * new_path = malloc(len + 1);
	new_path[len]=0;
	new_path[0]=0;
	strcat(new_path,tmpdir);
	strcat(new_path,filedir);
	return new_path;
}
int file_exists(char * filename)
{
	FILE * file;
	file = fopen(filename,"r");
		if(file)
	{
		fclose(file);
		return 1;
	}
	return 0;
}
int old(char* filename)
{

	char * ffpath = full_path(filename,".md5");
	char * tspath = full_path(COMPDIR,ffpath);
	free(ffpath);
	int ol = 1;
	if (file_exists(tspath) && file_exists(filename))
	{
		FILE * f = fopen(tspath,"r");
		char * inner = read_entire_file(f);
		fclose(f);
		inner[strlen(inner)-1]=0;
		struct stat file_stat;
		stat(filename,&file_stat);
		fprintf(stderr,"%s:%d %s:%d\n",tspath,atoi(inner),filename,file_stat.st_mtime);
		fflush(stderr);
		if(file_stat.st_mtime<=atoi(inner))
		{
			ol = 0;
		}
		else
		{
			ol = 1;
		}
		free(inner);
	}
	free(tspath);
	return ol;
}
static int cse_handler(request_rec* r,const char** data){
	if(!r->handler || strcmp(r->handler,"cse"))
		return DECLINED;
	char*  command;
	if (r->method_number != M_GET) 
	{
		return HTTP_METHOD_NOT_ALLOWED;
	}
	generate = old(r->filename);
	char* de = full_path(COMPDIR,r->filename);
	char* depname = full_path(de,".dep");
	if(!file_exists(depname))
	{
		generate = 1;
	}
	else
	{
		char* dee = read_entire_file(fopen(depname,"r"));
		dee[strlen(dee)-1]=0;
		int maxsz = 1;
		char * sub =(char*) malloc(maxsz);;
		int i;
		int len = strlen(dee);
		int pos = 0;
		for(i = 0; i < len; i++)
		{
			if(dee[i]!=',')
			{
				sub[pos++] = dee[i];
				if(pos >= maxsz-1)
				{
					sub = (char*)realloc(sub,maxsz*2);
					maxsz*=2;
				}
			}else
			{
				sub[pos]=0;
				if(old(sub))
				{
					generate = 1;
				}
				free(sub);
				pos = 0;
				maxsz=1;
				sub =malloc(maxsz);
			}
		}
		free(dee);
	}
	free(de);
	free(depname);
	if(!generate)
	{
	}
	if(generate)
	{
		if(r->args==NULL)
		{
			asprintf(&command,"csegen %s '\"\"' 2>&1",r->filename); 
		}else
		{
			asprintf(&command,"csegen %s '\"%s\"' 2>&1",r->filename,r->args); 
		}
	}
	else
	{
		char * t = full_path(COMPDIR,r->filename);
		int len = strlen(t)+ 2;
		t = (char *)realloc(t,len+1);
		t[len-2]=0;
		strcat(t,".e");
		t[len]=0;
		if(r->args==NULL)
		{
			asprintf(&command,"%s '\"\"' 2>&1",t);
		}else
		{
			asprintf(&command,"%s '\"%s\"' 2>&1",t,r->args);
		}
		free(t);

	}
	ap_set_content_type(r , "text/html");
	FILE *cmd = popen(command,"r");
	char* str = read_entire_file(cmd);
	if(generate)
		ap_rputs("<!--GENERATED-->\n",r);
	ap_rputs(str,r);
	free(str);
	free(command);
	return OK;
}
static void register_hooks(apr_pool_t* pool)
{
	ap_hook_handler(cse_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

module AP_MODULE_DECLARE_DATA cse_module = {
	STANDARD20_MODULE_STUFF,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	register_hooks
};

