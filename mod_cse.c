#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_log.h"
#include "http_main.h"
#include "http_protocol.h"
#include "http_request.h"
#include "util_script.h"
#include "http_connection.h"

#include "apr_strings.h"

static int cse_handler(request_rec* r){
	if(!r->handler || strcmp(r->handler,"cse"))
		return DECLINED;
	char * command = NULL;

	command = asprintf(&null,"csegen %s %s",r->filename,r->args);
	ap_set_content_type(r,"text/plain");
	ap_rputs(command);
	return OK;
}
