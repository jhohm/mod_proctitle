/* The code for Apache1 can be found at dammit.lt */
/*
**  mod_proctitle.c -- Apache sample proctitle module
**  [Autogenerated via ``apxs -n proctitle -g]
**
**  To play with this sample module first compile it into a
**  DSO file and install it into Apache's modules directory
**  by running:
**
**    $ apxs -c -i mod_proctitle.c
**
**  Then activate it in Apache's httpd.conf file for instance
**  for the URL /proctitle in as follows:
**
**    #   httpd.conf
**    LoadModule proctitle_module modules/mod_proctitle.so
**
**  Then after restarting Apache via
**
**    $ apachectl restart
*/
#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"
extern char *ap_server_argv0;
char proctitle_buf[128];
/* The sample content handler */
static int proctitle_handler(request_rec *r)
{
   if (strcmp(r->handler, "proctitle")) {
       return DECLINED;
   }
   r->content_type = "text/html";
   if (!r->header_only)
       ap_rputs("The sample page from mod_proctitle.c\n", r);
   return OK;
}
static int proctitle_child_init(request_rec *r)
{
       strncpy(ap_server_argv0,"httpd: init-child",64);
       return DECLINED;
}
static int proctitle_clear(request_rec *r)
{
       strncpy(ap_server_argv0,"httpd: idle",64);
       return DECLINED;
}
static int proctitle_ft(request_rec *r)
{
       bzero(proctitle_buf,128);
       snprintf(proctitle_buf,127,"httpd: %16s [%s] %s",
               r->connection->remote_ip,r->hostname,
               r->the_request);
       strncpy(ap_server_argv0,proctitle_buf,128);
       return DECLINED;
}
static void proctitle_register_hooks(apr_pool_t *p)
{
   /* ap_hook_handler(proctitle_handler, NULL, NULL, APR_HOOK_MIDDLE); */
   ap_hook_child_init(proctitle_child_init, NULL, NULL, APR_HOOK_FIRST);
   ap_hook_translate_name(proctitle_ft,NULL,NULL, APR_HOOK_FIRST);
   ap_hook_log_transaction(proctitle_clear,NULL,NULL, APR_HOOK_LAST);
   strncpy(ap_server_argv0,"httpd: master",60);
}
/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA proctitle_module = {
   STANDARD20_MODULE_STUFF,
   NULL,                  /* create per-dir    config structures */
   NULL,                  /* merge  per-dir    config structures */
   NULL,                  /* create per-server config structures */
   NULL,                  /* merge  per-server config structures */
   NULL,                  /* table of config file commands       */
   proctitle_register_hooks  /* register hooks                      */
};