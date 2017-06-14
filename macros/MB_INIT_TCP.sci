function [x,y,typ]=MB_INIT_TCP(job,arg1,arg2)

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
      [ok,IP,port,debugMode,exprs]=..
      getvalue('Set MB_INIT_TCP block parameters',..
               ['IP Address';'TCP Port';'Debug Mode (No=0/Yes=1)'],..
               list('str',1,'vec',1,'vec',1),exprs)
      if ok then
        model.ipar=[port;debugMode;length(IP);ascii(IP)']
        graphics.exprs=exprs
        x.graphics=graphics
        x.model=model
      end
      break
    end

  case 'define' then
    // default parameters
    IP='127.0.0.1'
    port=502
    debugMode=1
    
    // computational function
    model=scicos_model()
    model.sim=list('mb_init_tcp',4)
  
    // inputs
    model.in=[]
    model.in2=[]
    model.intyp=1

    // outputs
    model.out=[]
    model.out2=[]
    model.outtyp=1

    // event ports
    model.evtin=[]
    model.evtout=[]

    // states
    model.state=[]
    model.dstate=[]
    model.odstate=list()
    
    // parameters
    model.ipar=[port;debugMode;length(IP);ascii(IP)']
    model.rpar=[]
    model.opar=list()
  
    model.blocktype='d'
    model.firing=[]
    model.dep_ut=[%f,%f]
    model.label=''
    model.nzcross=0
    model.nmode=0
    model.equations=list()

    exprs=[IP;sci2exp(port);sci2exp(debugMode)];
    
    gr_i=list(["xstringb(orig(1)+sz(1)*0.1,orig(2)+sz(2)*0.6,[""Modbus""],sz(1)*0.8,sz(2)*0.3,''fill'');";..
               "xstringb(orig(1)+sz(1)*0.1,orig(2)+sz(2)*0.1,[""TCP""],sz(1)*0.8,sz(2)*0.4,''fill'');"],8)

    x=standard_define([2,2],model,exprs,gr_i)
    x.graphics.id="TCP Connection"
end

endfunction
