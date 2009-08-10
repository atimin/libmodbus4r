require 'rubygems'
spec = Gem::Specification.new do |s|
  s.name = "libmodbus4r"
  s.version = "0.2.1"
  s.author  = 'Aleksey Timin'
  s.platform = Gem::Platform::RUBY
  s.summary = "Binding use *libmodbus* (free implementation of modbus for Linux\\MacOS) for Ruby."
  s.files = Dir['lib/**/*','examples/*.rb','spec/*.rb', 'ext/*/*']
  s.autorequire = "libmodbus4r"
  s.has_rdoc = true
  s.rdoc_options = ["--title", "libmodbus4r", "--inline-source", "--main", "README"]
  s.extra_rdoc_files = ["README", "LICENSE", "NEWS", "TODO"]
  s.extensions = ['ext/extconf.rb'] 
end
