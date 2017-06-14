#include "scicos_block4.h"
#include "block_common.h"
#include "mb_common.h"
#include "modbus.h" // libmodbus

void mb_init_rtu( scicos_block *block, int flag )
{
	int* ipar = NULL;
	
	char *device = NULL;
	char *parity = NULL;
	int baud = 0;
	int data = 0;
	int stop = 0;
	
	switch( flag ) {
			
		case Initialization:
			block_debug( "Start Initialization:\n" );
			
			ipar = GetIparPtrs( block );
			
			// serial device
			device = block_first_string( &( ipar[4] ));
			if ( device == NULL ) {
				scicos_free( device );
				block_error( -16, "scicos_malloc failed!\n" );
				return;
			}
			block_debug( "device = %s\n", device );
			
			// baud rate
			baud = ipar[0];
			block_debug( "baud = %i\n", baud );
			
			// parity
			parity = block_next_string();
			if ( parity == NULL ) {
				scicos_free( parity );
				block_error( -16, "scicos_malloc failed!\n" );
				return;
			}
			block_debug( "parity = %s\n", parity );
			
			// data bits
			data = ipar[1];
			block_debug( "data = %i\n", data );
			
			// stop bits
			stop = ipar[2];
			block_debug( "stop = %i\n", stop );
			
			// initialize parameters
			modbus_init_rtu( &mb_param,
			                 device, baud, parity, data, stop );
			scicos_free( device );
			scicos_free( parity );
			
			// set debug mode?
			if ( ipar[3] > 0 ) {
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
