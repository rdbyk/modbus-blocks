names = ['mb_init_tcp',..
         'mb_init_rtu',..
         'mb_read',..
         'mb_write',..
         'mb_write2'];

files = ['block_common.o',..
         'mb_common.o',..
         'mb_init_tcp.o',..
         'mb_init_rtu.o',..
         'mb_read.o',..
         'mb_write.o',..
         'mb_write2.o'];

ldflags="-L. -lmodbus";

cflags="-DBLOCK_DEBUG";

ilib_for_link(names,files,[],'c',...
              'Makelib','loader.sce','scsmodbus',ldflags,cflags);
