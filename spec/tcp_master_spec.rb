require 'modbus4r'

describe ModBus::TCPMaster do

  before(:all) do
    @mstr = ModBus::TCPMaster.new('127.0.0.1', 1502, 1)
    @mstr.connect
  end

  it "should raise exception if have'n connected with slave" do
    mstr = ModBus::TCPMaster.new('127.0.0.1', 1503, 1)
    mstr.closed?.should == true
    lambda { mstr.connect }.should raise_error(ModBus::Errors::ModBusError, "Connection refused")
  end

  it "should connect with slave" do
    @mstr.closed?.should == false
  end

  # Read coil status
  it "should read coil status" do
    @mstr.read_coil_status(0, 4).should == [false, false, false, false]
  end

  it "should raise exception if illegal data address" do
   lambda { @mstr.read_coil_status(0, 501) }.should raise_error(ModBus::Errors::IllegalDataAddress, "Illegal data address (-2)")
  end


  it "should raise exception if too many data" do
   lambda { @mstr.read_coil_status(0, 0x07D1) }.should raise_error(ModBus::Errors::ModBusError, "Too many data (-15)")
  end

  # Read input status
  it "should read discrete inputs" do
    @mstr.read_input_status(0, 4).should == [false, false, false, false]
  end

  it "should raise exception if illegal data address" do
   lambda { @mstr.read_input_status(0, 501) }.should raise_error(ModBus::Errors::IllegalDataAddress, "Illegal data address (-2)")
  end


  it "should raise exception if too many data" do
   lambda { @mstr.read_input_status(0, 0x07D1) }.should raise_error(ModBus::Errors::ModBusError, "Too many data (-15)")
  end

  # Read holding registers
  it "should read discrete inputs" do
    @mstr.read_holding_registers(0, 4).should == [0, 0, 0, 0]
  end

  it "should raise exception if illegal data address" do
   lambda { @mstr.read_holding_registers(402, 99) }.should raise_error(ModBus::Errors::IllegalDataAddress, "Illegal data address (-2)")
  end


  it "should raise exception if too many data" do
   lambda { @mstr.read_holding_registers(0, 0x007E) }.should raise_error(ModBus::Errors::ModBusError, "Too many data (-15)")
  end

  # Read input registers
  it "should read discrete inputs" do
    @mstr.read_input_registers(0, 4).should == [0, 0, 0, 0]
  end

  it "should raise exception if illegal data address" do
   lambda { @mstr.read_input_registers(402, 99) }.should raise_error(ModBus::Errors::IllegalDataAddress, "Illegal data address (-2)")
  end

  it "should raise exception if too many data" do
   lambda { @mstr.read_input_registers(0, 0x007E) }.should raise_error(ModBus::Errors::ModBusError, "Too many data (-15)")
  end

  # Force single coil
  it "should force single coil" do
    @mstr.force_single_coil(4, true).should == @mstr
    @mstr.read_coil_status(4, 4).should == [true, false, false, false] 
  end

  it "should raise exception if illegal data address" do
   lambda { @mstr.force_single_coil(501, true) }.should raise_error(ModBus::Errors::IllegalDataAddress, "Illegal data address (-2)")
  end

  # Preset single register
  it "should preset single register" do
    @mstr.preset_single_register(4, 0x0AA0).should == @mstr
    @mstr.read_holding_registers(4, 1).should == [0x0AA0] 
  end

  it "should raise exception if illegal data address" do
   lambda { @mstr.preset_single_register(501, 0x0AA0) }.should raise_error(ModBus::Errors::IllegalDataAddress, "Illegal data address (-2)")
  end

  # Force multiple coils
  it "should force multiple coils" do
    @mstr.force_multiple_coils(14, [false, true, false, true]).should == @mstr
    @mstr.read_coil_status(13, 5).should == [false, false, true, false, true] 
  end

  it "should raise exception if illegal data address" do
   lambda { @mstr.force_multiple_coils(501, [true, false]) }.should raise_error(ModBus::Errors::IllegalDataAddress, "Illegal data address (-2)")
  end

  # Preset multiple registers
  it "should preset multiple registers" do
    @mstr.preset_multiple_registers(14, [1, 2, 3, 0xAACC]).should == @mstr
    @mstr.read_holding_registers(13, 5).should == [0, 1, 2, 3, 0xAACC] 
  end

  it "should raise exception if illegal data address" do
   lambda { @mstr.preset_multiple_registers(501, [1, 2]) }.should raise_error(ModBus::Errors::IllegalDataAddress, "Illegal data address (-2)")
  end

  after(:all) do
    @mstr.close unless @mstr.closed?
  end

end
