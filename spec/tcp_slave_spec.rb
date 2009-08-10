require 'modbus4r'
require 'timeout'

describe ModBus::TCPSlave do
  
  before(:each) do
    @sl = ModBus::TCPSlave.new('127.0.0.1', 1502, 1)
    @sl.start
    @mstr = ModBus::TCPMaster.new('127.0.0.1', 1502, 1)
    @mstr.connect
  end

  it "should have stoped" do
    sl = ModBus::TCPSlave.new('127.0.0.1', 1512, 1)
    sl.start.stoped? == false
    sl.stop.stoped? == true
    sl.start.stoped? == false
  end

  it "should open listen socket" do
    mstr = ModBus::TCPMaster.new('127.0.0.1', 1502, 1)
    mstr.connect.closed?.should == false
    mstr.close

    @sl.stop
    lambda { mstr.connect }.should raise_error(ModBus::Errors::ModBusError)
  end


  it "should have coil status" do
    @sl.coil_status.should == []
    @sl.coil_status = [false, true, false]
    
    @mstr.read_coil_status(0, 3).should == [false, true, false]
    @mstr.force_single_coil(1, false)
     
    @sl.coil_status.should == [false, false ,false]
     
    @mstr.force_multiple_coils(0, [true, true, true])

    @sl.coil_status.should == [true, true, true]

    @sl.coil_status[1] = false
    @sl.coil_status.should == [true, false, true]
    @mstr.read_coil_status(0, 3).should == [true, false, true]
  end

  it "should have input status" do
    @sl.input_status.should == []
    @sl.input_status = [false, false, false]
    @sl.input_status.should == [false, false, false]

    @mstr.read_input_status(0, 3).should == [false, false, false]

    @sl.input_status[1] = true
    @sl.input_status.should == [false, true, false]

    @mstr.read_input_status(0, 3).should  == [false, true, false]
  end

  it "should have input status" do
    @sl.holding_registers.should == []
    @sl.holding_registers = [0, 0, 0]

    @mstr.read_holding_registers(0, 3).should == [0, 0, 0]
    @mstr.preset_single_register(1, 0x0AA0)
    
    @sl.holding_registers.should == [0, 0x0AA0, 0]

    @mstr.preset_multiple_registers(0, [1, 2, 3])

    @sl.holding_registers.should == [1, 2, 3]
    @sl.holding_registers[2] = 0xABCD

    @mstr.read_holding_registers(0, 3) == [1, 2, 0xABCD]

  end

  after(:each) do
    @sl.stop unless @sl.stoped?
  end

end
