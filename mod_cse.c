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
#include "apr_strings.h"
static int cse_handler(request_rec* r){
	if(!r->handler || !strcasecmp(r->handler,"cse"))
		return DECLINED;
	char * command = NULL;

    if(strcmp("(null)",r->args)){
		asprintf(&command,"csegen %s '\"%s\"'",r->filename,r->args); 
	}else{
		asprintf(&command,"csegen %s \"\"",r->filename,r->args); 
	}
	if (r->method_number != M_GET) {
    /* We only accept GET and HEAD requests.
     * They are identical for the purposes of a content generator
     * Returning an HTTP error code causes Apache to return an
     * error page (ErrorDocument) to the client.
     */
    return HTTP_METHOD_NOT_ALLOWED;
  	}
	ap_set_content_type(r , "text/html");
	FILE *cmd = popen(command,"r");
	char * str;
	int maxsz = 32;
	int len = 0;
	str = malloc(sizeof(char));
	while(!feof(cmd)){
		str[len++]=fgetc(cmd);
		if(len == maxsz){
			str=realloc(str,sizeof(char)*maxsz*2);
			maxsz *=2;
		}
	}
	str[len]=0;
	ap_set_content_type(r , "text/html");
	ap_rputs(str,r);
	free(str);
	ap_set_content_type(r , "text/html");
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

