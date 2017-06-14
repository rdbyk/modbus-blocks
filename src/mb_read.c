#include "scicos_block4.h"
#include "block_common.h"
#include "mb_common.h"
#include "modbus.h" // libmodbus

// pointer to libmodus function 0x01, 0x02, 0x03 or 0x04
typedef int (*mb_read_func_p)( modbus_param_t*, int, int, int, int*);

void mb_read( scicos_block *block, int flag )
{
	int *ipar = NULL;
	mb_work_t *work = NULL;
	SCSUINT16_COP *out  = NULL;
	
	int count = 0;
	int i = 0;
	
	switch( flag ) {
			
		case Initialization:
			block_debug( "Start Initialization:\n" );
			
			ipar = GetIparPtrs( block );
			
			// malloc work & set work->count
			work = mb_malloc( ipar[2] );
			if ( work ==  NULL ) {
				block_error( -16, "mb_malloc failed!\n" );
				return;
			}
			
			// slave
			work->slave = ipar[0];
			
			// start address
			work->addr = ipar[1];
			
			// modbus function
			switch ( ipar[3] ) {
				case 0x01:
					work->func = &read_coil_status;
				break;
				case 0x02:
					work->func = &read_input_status;
				break;
				case 0x03:
					work->func = &read_holding_registers;
				break;
				case 0x04:
					work->func = &read_input_registers;
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
			
			// call modbus function
			count = (( mb_read_func_p ) work->func )( &mb_param,
			                                          work->slave,
                                                      work->addr, 
                                                      work->count,
                                                      work->data);
                                            
            // actually read?                        
			if ( count != work->count ) {
				block_error( -2, "count != work->count\n" );
				return;
			}
			
			// block output
			out = Getuint16OutPortPtrs( block, 1 );
			if ( out == NULL ) {
				block_error( -16, "Getuint16OutPortsPtrs failed!\n" );
				return;
			}
			
			// int -> uint16
			for ( i = 0; i < count; i++ )
				out[i] = ( SCSUINT16_COP )( work->data[i] ); 
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
