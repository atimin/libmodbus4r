begin 
  require 'rubygems'
rescue
end

require 'modbus4r'

mstr = ModBus::RTUMaster.new('/dev/ttyS0', # device
                             9600,         # baud: 9600, 19200, etc
                             "none",       # parity: "even", "odd", "none"
                             8,            # data bits: 5, 6, 7, 8
                             1)            # stop bits: 1, 2
mstr.connect
# Reads the coils status in the slave
puts mstr.read_coil_status(1,   #slave id
                           0,   #start address
                           10)  #number
# Reads the input status in the slave
puts mstr.read_input_status(1,  #slave id
                            0,  #start address
                            10) #number
# Reads the holding registers in the slave
puts mstr.read_holding_registers(1, #slave id
                            0,  #start address
                            5)   #number
# Reads the input registers in the slave
puts mstr.read_input_registers(1, #slave id
                            0,  #start address
                            5)  #number
# Turns a single coil in the slave
mstr.force_single_coil(1, #slave id
                        0, #coil address
                        true) #state

# Sets a value in one holding register in the slave
mstr.preset_single_register(1, #slave id
                        0, #reg address
                        0xa0a0) #value

# Sets\reset the coils in the slave
mstr.force_multiple_coils(1, #slave id
                        0, #start address
                        [true, false, true]) #data

#Copies the values in th slave
mstr.preset_multiple_registers(1, #slave id
                        0, #start address
                        [1, 0xdf, 2]) #data

mstr.close
