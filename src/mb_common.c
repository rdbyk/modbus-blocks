#include "scicos_block4.h"
#include "modbus.h" // modbus_param_t
#include "mb_common.h"

// global modbus parameters
modbus_param_t mb_param;

// malloc block work data struct
mb_work_t* mb_malloc( int count ) 
{
	mb_work_t *work;
	
	work = ( mb_work_t* ) scicos_malloc( sizeof( mb_work_t ));
	if ( work == NULL ) return NULL;
	
	work->data = ( int* ) scicos_malloc( count * sizeof( int ));
	if ( work->data == NULL ) {
		scicos_free( work );
		return NULL;
	}
	
	work->count = count;
	
	return work;
}

// free block work data struct
void mb_free( mb_work_t *work )
{
	if ( work->data != NULL ) scicos_free( work->data );
	if ( work != NULL ) scicos_free( work );
}
