#include "scicos_block4.h"
#include "block_common.h"
#include "mb_common.h"
#include "modbus.h" // libmodbus

// pointer to libmodus function 0x05 or 0x06
typedef int (*mb_write_func_p)( modbus_param_t*, int, int, int);

void mb_write( scicos_block *block, int flag )
{
	int *ipar = NULL;
	mb_work_t *work = NULL;
	int count = 0;
	SCSUINT16_COP *in = NULL;
	
	switch( flag ) {
			
		case Initialization:
			block_debug( "Start Initialization:\n" );
			
			ipar = GetIparPtrs( block );
			
			// malloc work
			// work->count = 1 (single coil/register)
			work = mb_malloc( 1 );
			if ( work ==  NULL ) {
				block_error( -16, "mb_malloc failed!\n" );
				return;
			}
			
			// slave
			work->slave = ipar[0];
			
			// address
			work->addr = ipar[1];
			
			// modbus function
			switch ( ipar[2] ) {
				case 0x05:
					work->func = &force_single_coil;
				break;
				case 0x06:
					work->func = &preset_single_register;
				break;
				default:
					work->func = NULL;
					block_error( -16, "illegal function code\n");
					return;
				break;
			}
            
            // save work
			GetWorkPtrs( block ) = work;
		break;
		
		case OutputUpdate:
			block_debug( "Start OutputUpdate:\n" );
			
			// get work
			work = GetWorkPtrs( block );
			
			// block input
			in = Getuint16InPortPtrs( block, 1 );
			if ( in == NULL ) {
				block_error( -16, "Getuint16InPortsPtrs failed!\n" );
				return;
			}
			
			// uint16 -> int
			work->data[0] = ( int )( in[0] ); 
				
			// call modbus function
			count = (( mb_write_func_p ) work->func )( &mb_param,
			                                           work->slave,
			                                           work->addr, 
			                                           work->data[0] );
                                            
            // actually written?                        
			if ( count != work->count ) {
				block_error( -2, "count != work->count\n" );
				return;
			}
		break;
		
		case Ending:
			block_debug( "Start Ending:\n" );
			
			// get work
			work = GetWorkPtrs( block );
			
			// free work
			mb_free( work );
		break;
	}
}
