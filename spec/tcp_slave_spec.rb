require 'modbus4r'
require 'timeout'

describe ModBus::TCPSlave do
  
  before(:all) do
    @sl = ModBus::TCPSlave.new('127.0.0.1', 1502, 1)
    @sl.start
  end

  it "should have slave id" do
    sl = ModBus::TCPSlave.new('127.0.0.1', 1502, 5)
    sl.id.should == 5
    @sl.id.should == 1
  end

  it "should not stoped" do
    @sl.stoped?.should == false
  end

  it "should have stoped" do
    sl = ModBus::TCPSlave.new('127.0.0.1', 1502, 1)
    sl.start.stoped? == false
    sl.stop.stoped? == true
  end

  after(:all) do
    @sl.stop unless @sl.stoped?
  end

end
