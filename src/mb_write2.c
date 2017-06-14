#include "scicos_block4.h"
#include "block_common.h"
#include "mb_common.h"
#include "modbus.h" // libmodbus

// pointer to libmodus function 0x0F or 0x10
typedef int (*mb_write2_func_p)( modbus_param_t*, int, int, int, int*);

void mb_write2( scicos_block *block, int flag )
{
	int *ipar = NULL;
	mb_work_t *work = NULL;
	int count = 0;
	SCSUINT16_COP *in = NULL;
	int i = 0;
	
	switch( flag ) {
			
		case Initialization:
			block_debug( "Start Initialization:\n" );
			
			ipar = GetIparPtrs( block );
			
			// malloc work & set work->count
			work = mb_malloc( GetInPortRows( block, 1));
			if ( work ==  NULL ) {
				block_error( -16, "mb_malloc failed!\n" );
				return;
			}
			block_debug( "count = %i\n", work->count );
			
			// slave
			work->slave = ipar[0];
			
			// start address
			work->addr = ipar[1];
			
			// modbus function
			switch ( ipar[2] ) {
				case 0x0F:
					work->func = &force_multiple_coils;
				break;
				case 0x10:
					work->func = &preset_multiple_registers;
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
			for ( i = 0; i < work->count; i++ )
				work->data[i] = ( int )( in[i] ); 
				
			// call modbus function
			count = ((mb_write2_func_p ) work->func )( &mb_param,
			                                           work->slave,
                                                       work->addr, 
                                                       work->count,
                                                       work->data );
                                            
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
