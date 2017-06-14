#include "modbus.h" // modbus_param_t

// global modbus parameters
extern modbus_param_t mb_param;

// local block work data struct
typedef struct _mb_work_t {
	int slave;
	int addr;
	int count;
	int *data;
	void *func;
} mb_work_t;

// malloc & free block work data struct 
mb_work_t* mb_malloc( int count );
void mb_free( mb_work_t *work );
