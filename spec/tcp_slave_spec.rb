require 'modbus4r'
require 'timeout'

describe ModBus::TCPSlave do
  
  before(:each) do
    @sl = ModBus::TCPSlave.new('127.0.0.1', 1502, 1)
    @sl.start
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
    mstr = ModBus::TCPMaster.new('127.0.0.1', 1502, 1)
    mstr.connect

    @sl.coil_status.should == []
    @sl.coil_status = [false, true, false]
    
    mstr.read_coil_status(0, 3).should == [false, true, false]
    mstr.force_single_coil(1, false)
     
    @sl.coil_status.should == [false, false ,false]
     
    mstr.force_multiple_coils(0, [true, true, true])

    @sl.coil_status.should == [true, true, true]

    @sl.coil_status[1] = false
    @sl.coil_status.should == [true, false, true]
    mstr.read_coil_status(0, 3).should == [true, false, true]
  end

  it "should have input status" do
    mstr = ModBus::TCPMaster.new('127.0.0.1', 1502, 1)
    mstr.connect
    
    @sl.input_status.should == []
    @sl.input_status = [false, false, false]
    @sl.input_status.should == [false, false, false]

    mstr.read_input_status(0, 3).should == [false, false, false]

    @sl.input_status[1] = true
    @sl.input_status.should == [false, true, false]

    mstr.read_input_status(0, 3).should  == [false, true, false]
  end

  after(:each) do
    @sl.stop unless @sl.stoped?
  end

end
