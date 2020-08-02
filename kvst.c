/* var.c
 *
 * Instructions at the bottom of this file
 */
#define _GNU_SOURCE 1
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <jansson.h>
#include <flux/core.h>

const char default_service_name[] = "fluxt";
static uint32_t rank, size;
static const int NO_FLAGS = 0;
static const char const * DEFAULT_NAMESPACE = NULL;
#define HELLO flux_log(NULL, LOG_NOTICE, "%s:%d rank %" PRIu32 " size %" PRIu32 ".\n", __FILE__, __LINE__, rank, size)

int mod_main (flux_t *h, int argc, char **argv)
{
	int rc;

	// Get rank via flux_get_rank() and print results to stderr.
	flux_get_rank(h, &rank);
	flux_get_size(h, &size);
	flux_log(NULL, LOG_NOTICE, "%s:%d QQQ Hello from rank %" PRIu32 " of %" PRIu32 ".\n", __FILE__, __LINE__, rank, size);
	HELLO;
    

	// Create the kvs namespace.  Nevermind.  Doesn't work.
	/*
	f = flux_kvs_namespace_create( h, "variorum", FLUX_USERID_UNKNOWN, 0 );
	if (!f || flux_future_get (f, NULL) < 0) { flux_log_error (h, "flux_service_register"); }
	flux_future_destroy (f);
	*/

	// Start the service.
	flux_reactor_t *kvs_reactor = flux_get_reactor(h);
	HELLO;
	assert( NULL != kvs_reactor );
	HELLO;
	rc = flux_reactor_run ( kvs_reactor, NO_FLAGS );
	HELLO;
	assert( -1 != rc );
	HELLO;

	// Unregister service.
	flux_future_t *kvs_unregister_future;
	HELLO;
	kvs_unregister_future = flux_service_unregister (h, default_service_name);    
	HELLO;
	flux_log(NULL, LOG_NOTICE, "QQQ rank %" PRIu32 " of %" PRIu32 " waiting for confirmation of unregistration.\n", rank, size);
	HELLO;
	assert( NULL != kvs_unregister_future );
	HELLO;
	rc = flux_future_wait_for( kvs_unregister_future, 10.0 );
	HELLO;
	assert( rc != -1 );
	HELLO;
	flux_future_destroy ( kvs_unregister_future );
	HELLO;
	

	// Allocate the kvs transaction
	flux_kvs_txn_t *kvs_txn = flux_kvs_txn_create();
	HELLO;
	assert( NULL != kvs_txn );
	HELLO;

	// Create an entry from the kvs
	rc = flux_kvs_txn_put( kvs_txn, NO_FLAGS, "mykey", "myvalue" );
	HELLO;
	assert( -1 != rc );
	HELLO;

	// Commit the key+value.
	flux_future_t *kvs_future = flux_kvs_commit( h, DEFAULT_NAMESPACE, NO_FLAGS, kvs_txn );
	HELLO;
	assert( NULL != kvs_future );
	HELLO;

	// Wait for confirmation
	flux_log(NULL, LOG_NOTICE, "QQQ rank %" PRIu32 " of %" PRIu32 " waiting for confirmation of commit.\n", rank, size);
	HELLO;
	rc = flux_future_wait_for( kvs_future, 10.0 );
	HELLO;
	assert( rc != -1 );
	HELLO;
	flux_log(NULL, LOG_NOTICE, "QQQ rank %" PRIu32 " of %" PRIu32 " commit confirmed.\n", rank, size);
	HELLO;

	// Destroy our future.
	flux_future_destroy( kvs_future );
	HELLO;

	return 0;
}

MOD_NAME ("fluxt");
