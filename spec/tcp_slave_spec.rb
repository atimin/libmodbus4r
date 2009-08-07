require 'modbus4r'
require 'timeout'

describe ModBus::TCPSlave do
  
  before(:all) do
    @sl = ModBus::TCPSlave.new('127.0.0.1', 1502, 1)
    @sl.start
  end

  it "should not stoped" do
    @sl.stoped?.should == false
  end

  it "should have stoped" do
    sl = ModBus::TCPSlave.new('127.0.0.1', 1512, 1)
    sl.start.stoped? == false
    sl.stop.stoped? == true
#    sl.start.stoped? == false
  end

  it "should open listen socket" do
    mstr = ModBus::TCPMaster.new('127.0.0.1', 1502)
    mstr.connect.closed?.should == false
    mstr.close
  end

  after(:all) do
    @sl.stop unless @sl.stoped?
  end

end
