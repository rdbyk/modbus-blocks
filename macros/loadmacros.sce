mode(-1)
pathL=get_absolute_file_path('loadmacros.sce')
disp('Loading macros in ' +pathL)
load(pathL+'/lib')

disp('Adding Scicos palette ' +pathL+'ModbusBlocks.cosf')
scicos_pal($+1,:)=['ModbusBlocks',pathL+'ModbusBlocks.cosf']

clear pathL
