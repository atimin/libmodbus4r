require 'modbus4r'

describe ModBus::RTUMaster do

  before(:all) do
    @mstr = ModBus::RTUMaster.new('/dev/ttyS3', 9600, 'none', 8, 1)
    @mstr.connect
  end

  it "should be connect" do
    @mstr.closed?.should == false
  end

  after(:all) do
    @mstr.close unless @mstr.closed?
  end

end
    


