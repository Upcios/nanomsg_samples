#include <assert.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <stdio.h>
#include <nanomsg/nn.h>
#include <nanomsg/survey.h>

#define NODE0 "node0"
#define NODE1 "node1"
#define DATE   "DATE"

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
	int sock = nn_socket( AF_SP, NN_SURVEYOR );
	assert( sock >= 0 );
	assert( nn_bind( sock, url ) >= 0 );
	Sleep( 10000 ); // wait for connections
	int sz_d = strlen( DATE ) + 1; // '\0' too
	printf( "SERVER: SENDING DATE SURVEY REQUEST\n" );
	int bytes = nn_send( sock, DATE, sz_d, 0 );
	assert( bytes == sz_d );
	while ( 1 )
	{
		char *buf = NULL;
		int bytes = nn_recv( sock, &buf, NN_MSG, 0 );
		if ( bytes == ETIMEDOUT ) break;
		if ( bytes >= 0 )
		{
			printf( "SERVER: RECEIVED \"%s\" SURVEY RESPONSE\n", buf );
			nn_freemsg( buf );
		}
	}
	return nn_shutdown( sock, 0 );
}

int client( const char *url, const char *name )
{
	int sock = nn_socket( AF_SP, NN_RESPONDENT );
	assert( sock >= 0 );
	assert( nn_connect( sock, url ) >= 0 );
	while ( 1 )
	{
		char *buf = NULL;
		int bytes = nn_recv( sock, &buf, NN_MSG, 0 );
		if ( bytes >= 0 )
		{
			printf( "CLIENT (%s): RECEIVED \"%s\" SURVEY REQUEST\n", name, buf );
			nn_freemsg( buf );
			char *d = date();
			int sz_d = strlen( d ) + 1; // '\0' too
			printf( "CLIENT (%s): SENDING DATE SURVEY RESPONSE\n", name );
			int bytes = nn_send( sock, d, sz_d, 0 );
			assert( bytes == sz_d );
		}
	}
	return nn_shutdown( sock, 0 );
}

// usage: nanomsg_survey.exe node0
// usage: nanomsg_survey.exe node1 client0
// usage: nanomsg_survey.exe node1 client1
// usage: nanomsg_survey.exe node1 client2
// note: at least 1 client must be connected before the first server message (see Sleep of 10s)
// note: client1 can crash ?
int main( const int argc, const char **argv )
{
	if ( strncmp( NODE0, argv[ 1 ], strlen( NODE0 ) ) == 0 )
		return server( "ipc:///tmp/survey.ipc" );
	else if ( strncmp( NODE1, argv[ 1 ], strlen( NODE1 ) ) == 0 && argc > 1 )
		return client( "ipc:///tmp/survey.ipc", argv[ 2 ] );
	return 0;
}