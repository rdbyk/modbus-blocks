#include "scicos_block4.h"
#include "block_common.h"
#include "mb_common.h"
#include "modbus.h" // libmodbus

void mb_init_tcp( scicos_block *block, int flag )
{
	int* ipar = NULL;
	
	char *ip = NULL;
	uint16_t port;
	
	switch( flag ) {
			
		case Initialization:
			block_debug( "Start Initialization:\n" );
			
			ipar = GetIparPtrs( block );
			
			// ip address
			ip = block_first_string( &( ipar[2] ));
			if ( ip == NULL ) {
				scicos_free( ip );
				block_error( -16, "scicos_malloc failed!\n" );
				return;
			}
			block_debug( "ip = %s\n", ip );
			
			// tcp port
			port = ipar[0];
			block_debug( "port = %i\n", port );
			
			// initialize parameters
			modbus_init_tcp( &mb_param, ip, port );
			scicos_free( ip );
			
			// set debug mode?
			if ( ipar[1] > 0 ) {
				block_debug( "libmodbus debug mode!\n" );
				modbus_set_debug( &mb_param, TRUE );
			}	
			// connect
			if ( modbus_connect( &mb_param ) != 0 ) {
				block_error( -16, "modbus_connect failed!\n" );
				return;
			}
		break;
		
		case Ending:
			block_debug( "Start Ending:\n" );
			modbus_close( &mb_param );
		break;
	}
}
