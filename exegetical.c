/* exegetical.c
 *
 * Test with:  flux python -c 'import flux; h=flux.Flux(); print(h.rpc("exegetical.foo", { "test": 42 }).get())'
 */
#define _GNU_SOURCE 1
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <jansson.h>
#include <flux/core.h>

#define MY_MOD_NAME "exegetical"

const char default_service_name[] = MY_MOD_NAME;
static uint32_t rank, size;
static const int NO_FLAGS = 0;
#define HELLO flux_log(h, LOG_CRIT, "%s:%d rank %" PRIu32 " size %" PRIu32 ".\n", __FILE__, __LINE__, rank, size)

static void foo_cb (flux_t *h, flux_msg_handler_t *mh, const flux_msg_t *msg, void *arg){

	const void *data;
	int size, rc;


	rc = flux_msg_get_payload (msg, &data, &size);
	if (-1 == rc){
		flux_log_error (h, "echo_cb: flux_msg_get_payload");
		rc = flux_respond_error (h, msg, errno, "flux_msg_get_payload failed");
		if( -1 == rc ){
			flux_log_error (h, "flux_respond_error");
		}
	}

	rc = flux_respond_raw (h, msg, data, size);
	if (-1 == rc){
		flux_log_error (h, "echo_cb: flux_respond_raw");
		rc = flux_respond_error (h, msg, errno, "flux_respond_raw failed");
		if( -1  == rc ){
			flux_log_error (h, "flux_respond_error");
		}
	}
	return;
}

int mod_main (flux_t *h, int argc, char **argv)
{
	int rc;

	// Get rank via flux_get_rank() and print results to stderr.
	flux_get_rank(h, &rank);
	flux_get_size(h, &size);
	flux_log(h, LOG_CRIT, "%s:%d QQQ Hello from rank %" PRIu32 " of %" PRIu32 ".\n", __FILE__, __LINE__, rank, size);

	// Set up a message handler.
	struct flux_match match = FLUX_MATCH_REQUEST;
	match.topic_glob = "exegetical.foo";
	flux_msg_handler_t *mh = flux_msg_handler_create (h, match, foo_cb, NULL);
	assert( mh );
	flux_msg_handler_start(mh);

	// Start the reactor.  Does not return from flux_reactor_run() until the module is unloaded.
	flux_reactor_t *exegetical_reactor = flux_get_reactor(h);
	assert( exegetical_reactor );
	rc = flux_reactor_run ( exegetical_reactor, NO_FLAGS );
	assert( -1 != rc );

	// Unregister the service.
	flux_future_t *kvs_unregister_future;
	kvs_unregister_future = flux_service_unregister (h, default_service_name);    
	assert( NULL != kvs_unregister_future );
	rc = flux_future_wait_for( kvs_unregister_future, 10.0 );
	assert( rc != -1 );
	flux_future_destroy ( kvs_unregister_future );

	return 0;
}

MOD_NAME (MY_MOD_NAME);
