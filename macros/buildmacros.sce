mode(-1)
toolboxname='modbusblocks'
pathB=get_absolute_file_path('buildmacros.sce')
disp('Building macros in ' +pathB)
genlib(toolboxname+'lib',pathB,%t)

disp('Building Scicos palette ...')
blocks=['MB_INIT_TCP.sci';..
        'MB_INIT_RTU.sci';..
        'MB_READ_COIL_STATUS.sci';..
        'MB_READ_INPUT_STATUS.sci';..
        'MB_READ_HOLDING_REGS.sci';..
        'MB_READ_INPUT_REGS.sci';..
        'MB_FORCE_SINGLE_COIL.sci';..
        'MB_PRESET_SINGLE_REG.sci';..
        'MB_FORCE_MULTIPLE_COILS.sc';..
        'MB_PRESET_MULTIPLE_REGS.sci']
load(SCI+'/macros/scicos/lib')
exec(SCI+'/macros/util/create_palette.sci',-1)
build_palette(blocks,pathB,'ModbusBlocks')

clear pathB genlib toolboxname blocks
