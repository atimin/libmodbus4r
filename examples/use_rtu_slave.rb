begin 
  require 'rubygems'
rescue
end

require 'modbus4r'

sl = ModBus::RTUSlave.new('/dev/ttyS0', # device
                             9600,         # baud: 9600, 19200, etc
                             "none",       # parity: "even", "odd", "none"
                             8,            # data bits: 5, 6, 7, 8
                             1,            # stop bits: 1, 2
                             1)            # slave id

sl.coil_status = [false, false, false, false, false] 
sl.input_status = [false, false, false, false, false]
sl.holding_registers = [0, 0, 0, 0, 0, 0, 0, 0]
sl.input_registers = [0, 0, 0, 0, 0, 0, 0, 0]

sl.start 

sl.join

sl.stop
