// debug macro
#ifdef BLOCK_DEBUG
#include <stdio.h>
#define block_debug(fmt, ...) \
sciprint("DEBUG:%s: ",__FUNCTION__); \
sciprint(fmt, ## __VA_ARGS__)
#else
#define block_debug(fmt, ...)
#endif // BLOCK_DEBUG

// error macro
#define block_error( err, fmt, ...) \
sciprint("ERROR:%s: ",__FUNCTION__); \
sciprint(fmt, ## __VA_ARGS__); \
set_block_error( err )

// encoded string parameters
char* block_first_string( int *data );
char* block_next_string( void );
