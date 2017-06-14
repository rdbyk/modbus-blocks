#include "scicos_block4.h"

// pointer to next encoded string parameter
static int* next_data = NULL;

// decode first string parameter
char* block_first_string( int *data )
{
	int len = 0;
	char *str = NULL;
	
	int i;
	
	if ( data == NULL ) return NULL;
	
	// length wo/ '\0'
	len = data[0];
	
	// malloc
	str = ( char* ) scicos_malloc((len + 1) * sizeof( char ));
	if ( str == NULL ) return NULL;
	
	// decode
	for( i = 0; i < len; i++ )
		str[i] = ( char ) data[i+1];
	str[i] = '\0';
	
	// next string
	next_data = &( data[i+1] );
	
	return str;
}

// decode next (2nd, 3rd, ...) string parameter
char* block_next_string( void )
{	
	return block_first_string( next_data );
}
