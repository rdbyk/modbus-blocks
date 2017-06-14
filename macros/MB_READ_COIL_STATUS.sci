function [x,y,typ]=MB_READ_COIL_STATUS(job,arg1,arg2)

x=[];y=[];typ=[]

select job
  
  case 'plot' then
    standard_draw(arg1)
    
  case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1)
    
  case 'getoutputs' then
    [x,y,typ]=standard_outputs(arg1)
    
  case 'getorigin' then
    [x,y]=standard_origin(arg1)

  case 'set' then
    x=arg1
    graphics=arg1.graphics
    exprs=graphics.exprs
    model=arg1.model
    while %t do
      [ok,slave,addr,count,exprs]=..
      getvalue('Set MB_READ_COIL_STATUS block parameters',..
               ['Slave ID';'Start Address';'Coil Count'],..
               list('vec',1,'vec',1,'vec',1),exprs)
      if ok then
        model.out=count
        model.ipar(1:3)=[slave;addr;count]
        graphics.exprs=exprs
        x.graphics=graphics
        x.model=model
      end
      break
    end

  case 'define' then
    // default (public) parameters
    slave=1
    addr=0
    count=1
    
    // private parameter
    fcode=hex2dec('01') // modbus function code
    
    // computational function
    model=scicos_model()
    model.sim=list('mb_read',4)
  
    // inputs
    model.in=[]
    model.in2=[]
    model.intyp=1

    // outputs
    model.out=count
    model.out2=[]
    model.outtyp=7 // uint16

    // event ports
    model.evtin=1
    model.evtout=[]

    // states
    model.state=[]
    model.dstate=[]
    model.odstate=list()
    
    // parameters
    model.ipar=[slave;addr;count;fcode]
    model.rpar=[]
    model.opar=list()
  
    model.blocktype='d'
    model.firing=[]
    model.dep_ut=[%f,%f]
    model.label=''
    model.nzcross=0
    model.nmode=0
    model.equations=list()

    exprs=[sci2exp(slave);sci2exp(addr);sci2exp(count)];
    
    gr_i=list(["xstringb(orig(1)+sz(1)*0.1,orig(2)+sz(2)*0.6,[""Modbus""],sz(1)*0.8,sz(2)*0.3,''fill'');";..
               "xstringb(orig(1)+sz(1)*0.1,orig(2)+sz(2)*0.1,[""0x01""],sz(1)*0.8,sz(2)*0.3,''fill'');"],8)

    x=standard_define([2,2],model,exprs,gr_i)
    x.graphics.id="Read Coil Status"
end

endfunction

