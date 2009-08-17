require 'modbus4r'

describe ModBus::RTUSlave do
  before(:all) do
    @sl = ModBus::RTUSlave.new('/dev/ttyS3', 9600, 'none', 8, 1, 1)
    @sl.start
  end

  it "should be start" do
    @sl.stoped?.should == false
  end

  after(:all) do
    @sl.stop unless @sl.stoped?
  end
end
