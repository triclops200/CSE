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


static int cse_handler(request_rec* r,const char** data){
	if(!r->handler || strcmp(r->handler,"cse"))
		return DECLINED;
	char*  command;

	if(r->args==NULL){
		asprintf(&command,"csegen %s '\"\"' 2>&1",r->filename,r->args); 
	}else{
		asprintf(&command,"csegen %s '\"%s\"' 2>&1",r->filename,r->args); 
	}
	if (r->method_number != M_GET && r->method_number !=M_POST) {
		/* We only accept GET and HEAD requests.
		 * They are identical for the purposes of a content generator
		 * Returning an HTTP error code causes Apache to return an
		 * error page (ErrorDocument) to the client.
		 */
		return HTTP_METHOD_NOT_ALLOWED;
	}
	else{
	}
	ap_set_content_type(r , "text/html");
	FILE *cmd = popen(command,"r");
	char * str;
	int maxsz = 32;
	int len = 0;
	fprintf(stderr,"Here\n");
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
	ap_rputs(str,r);
	free(str);
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

