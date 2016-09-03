#include <assert.h>
#include <string.h>
#include <Windows.h>
#include <stdio.h>
#include <nanomsg/nn.h>
#include <nanomsg/bus.h>

int node( const int argc, const char **argv )
{
	int sock = nn_socket( AF_SP, NN_BUS );
	assert( sock >= 0 );
	assert( nn_bind( sock, argv[ 2 ] ) >= 0 );
	Sleep( 1000 ); // wait for connections
	if ( argc >= 3 )
	{
		int x = 3;
		for ( x; x<argc; x++ )
			assert( nn_connect( sock, argv[ x ] ) >= 0 );
	}
	Sleep( 1000 ); // wait for connections
	int to = 100;
	assert( nn_setsockopt( sock, NN_SOL_SOCKET, NN_RCVTIMEO, &to, sizeof( to ) ) >= 0 );
	// SEND
	int sz_n = strlen( argv[ 1 ] ) + 1; // '\0' too
	printf( "%s: SENDING '%s' ONTO BUS\n", argv[ 1 ], argv[ 1 ] );
	int send = nn_send( sock, argv[ 1 ], sz_n, 0 );
	assert( send == sz_n );
	while ( 1 )
	{
		// RECV
		char *buf = NULL;
		int recv = nn_recv( sock, &buf, NN_MSG, 0 );
		if ( recv >= 0 )
		{
			printf( "%s: RECEIVED '%s' FROM BUS\n", argv[ 1 ], buf );
			nn_freemsg( buf );
		}
	}
	return nn_shutdown( sock, 0 );
}

// usage: nanomsg_bus.exe node0 ipc:///tmp/node0.ipc ipc:///tmp/node1.ipc ipc:///tmp/node2.ipc
// usage: nanomsg_bus.exe node1 ipc:///tmp/node1.ipc ipc:///tmp/node2.ipc ipc:///tmp/node3.ipc
// usage: nanomsg_bus.exe node2 ipc:///tmp/node2.ipc ipc:///tmp/node3.ipc
// usage: nanomsg_bus.exe node3 ipc:///tmp/node3.ipc ipc:///tmp/node0.ipc
int main( const int argc, const char **argv )
{
	if ( argc >= 3 ) 
		node( argc, argv );
	return 0;
}