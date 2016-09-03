#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>

#define NODE0 "node0"
#define NODE1 "node1"

char *date()
{
	time_t raw = time( &raw );
	struct tm *info = localtime( &raw );
	char *text = asctime( info );
	text[ strlen( text ) - 1 ] = '\0'; // remove '\n'
	return text;
}

int server( const char *url )
{
	int sock = nn_socket( AF_SP, NN_PUB );
	assert( sock >= 0 );
	assert( nn_bind( sock, url ) >= 0 );
	while ( 1 )
	{
		char *d = date();
		int sz_d = strlen( d ) + 1; // '\0' too
		printf( "SERVER: PUBLISHING DATE %s\n", d );
		int bytes = nn_send( sock, d, sz_d, 0 );
		assert( bytes == sz_d );
		Sleep( 1000 );
	}
	return nn_shutdown( sock, 0 );
}

int client( const char *url, const char *name )
{
	int sock = nn_socket( AF_SP, NN_SUB );
	assert( sock >= 0 );
	// TODO learn more about publishing/subscribe keys
	assert( nn_setsockopt( sock, NN_SUB, NN_SUB_SUBSCRIBE, "", 0 ) >= 0 );
	assert( nn_connect( sock, url ) >= 0 );
	while ( 1 )
	{
		char *buf = NULL;
		int bytes = nn_recv( sock, &buf, NN_MSG, 0 );
		assert( bytes >= 0 );
		printf( "CLIENT (%s): RECEIVED %s\n", name, buf );
		nn_freemsg( buf );
	}
	return nn_shutdown( sock, 0 );
}

// usage: nanomsg_pubsub.exe node0
// usage: nanomsg_pubsub.exe node1 client0
// usage: nanomsg_pubsub.exe node1 client1
// usage: nanomsg_pubsub.exe node1 client2
int main( const int argc, const char **argv )
{
	if ( strncmp( NODE0, argv[ 1 ], strlen( NODE0 ) ) == 0 )
		return server( "ipc:///tmp/pubsub.ipc" );
	else if ( strncmp( NODE1, argv[ 1 ], strlen( NODE1 ) ) == 0 && argc > 1 )
		return client( "ipc:///tmp/pubsub.ipc", argv[ 2 ] );
	return 0;
}