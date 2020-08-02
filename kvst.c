/* var.c
 *
 * Instructions at the bottom of this file
 */
#define _GNU_SOURCE 1
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <jansson.h>
#include <flux/core.h>

const char default_service_name[] = "var";
static uint32_t rank, size;



int mod_main (flux_t *h, int argc, char **argv)
{
    flux_future_t *f;

    // Get rank via flux_get_rank() and print results to stderr.
    flux_get_rank(h, &rank);
    flux_get_size(h, &size);
    flux_log(NULL, LOG_NOTICE, "QQQ Hello from rank %" PRIu32 " of %" PRIu32 ".\n", rank, size);
    

    // Create the kvs namespace.  Nevermind.  Doesn't work.
    /*
    f = flux_kvs_namespace_create( h, "variorum", FLUX_USERID_UNKNOWN, 0 );
    if (!f || flux_future_get (f, NULL) < 0) { flux_log_error (h, "flux_service_register"); }
    flux_future_destroy (f);
    */

    // Start the service.
    flux_reactor_run (flux_get_reactor (h), 0);

    // Unregister service.
    f = flux_service_unregister (h, default_service_name);    
    if (!f || flux_future_get (f, NULL) < 0) { flux_log_error (h, "flux_service_register"); }
    flux_future_destroy (f);

    return 0;
}

MOD_NAME ("var");
