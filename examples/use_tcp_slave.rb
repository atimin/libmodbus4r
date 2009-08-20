begin 
  require 'rubygems'
rescue
end

require 'modbus4r'

sl = ModBus::TCPSlave.new('127.0.0.1', #ip-address
                            1502,      #port
                            1)         #slave id

sl.coil_status = [false, false, false, false, false] 
sl.input_status = [false, false, false, false, false]
sl.holding_registers = [0, 0, 0, 0, 0, 0, 0, 0]
sl.input_registers = [0, 0, 0, 0, 0, 0, 0, 0]

sl.start 

sl.join

sl.stop
