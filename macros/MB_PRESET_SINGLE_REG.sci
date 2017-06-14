function [x,y,typ]=MB_PRESET_SINGLE_REG(job,arg1,arg2)

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
      [ok,slave,addr,exprs]=..
      getvalue('Set MB_PRESET_SINGLE_REG block parameters',..
               ['Slave ID';'Address'],..
               list('vec',1,'vec',1),exprs)
      if ok then
        model.ipar(1:2)=[slave;addr]
        graphics.exprs=exprs
        x.graphics=graphics
        x.model=model
      end
      break
    end

  case 'define' then
    // default parameters
    slave=1
    addr=0
    
    // private parameter
    fcode=hex2dec('06') // modbus function code
    
    // computational function
    model=scicos_model()
    model.sim=list('mb_write',4)
  
    // inputs
    model.in=1
    model.in2=[]
    model.intyp=7 // uint16

    // outputs
    model.out=[]
    model.out2=[]
    model.outtyp=1

    // event ports
    model.evtin=1
    model.evtout=[]

    // states
    model.state=[]
    model.dstate=[]
    model.odstate=list()
    
    // parameters
    model.ipar=[slave;addr;fcode]
    model.rpar=[]
    model.opar=list()
  
    model.blocktype='d'
    model.firing=[]
    model.dep_ut=[%f,%f]
    model.label=''
    model.nzcross=0
    model.nmode=0
    model.equations=list()

    exprs=[sci2exp(slave);sci2exp(addr)];
    
    gr_i=list(["xstringb(orig(1)+sz(1)*0.1,orig(2)+sz(2)*0.6,[""Modbus""],sz(1)*0.8,sz(2)*0.3,''fill'');";..
               "xstringb(orig(1)+sz(1)*0.1,orig(2)+sz(2)*0.1,[""0x06""],sz(1)*0.8,sz(2)*0.3,''fill'');"],8)

    x=standard_define([2,2],model,exprs,gr_i)
    x.graphics.id="Preset Single Register"
end

endfunction
