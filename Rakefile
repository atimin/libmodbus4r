require 'rbconfig'
require 'fileutils'
require 'rubygems'

begin
  require 'rubygems'
rescue Exception
end

begin
 require 'spec/rake/spectask'

 Spec::Rake::SpecTask.new do |t|
    t.spec_opts = ['-c', '-t 2.0']
    t.libs << 'lib'
    t.spec_files = FileList['spec/**/*_spec.rb']
    t.rcov = false 
  end
rescue Exception 
  puts 'RSpec not available. Install it with: sudo gem install rspec'
end

include Config
include FileUtils

sitedir = CONFIG['sitelibdir']

task :install => :compile do
  dest = File.join(sitedir, 'modbus4r')

  mkdir_p dest, :mode => 0755

  files = []
  cd('lib') { files = Dir['**/*.rb', '**/*.so'] }

  files.each do |fn|
    fn_dir = File.dirname(fn)
    target_dir = File.join(sitedir, fn_dir)
    mkdir target_dir unless File.exist?(target_dir)
    install(File.join('lib', fn), File.join(sitedir, fn), :mode => 0644, :verbose => true)
  end
  puts "Successfully installed libmodbus4r"
end

task :uninstall do
  cd(sitedir) do
    if File.exist?('modbus4r.rb')
      rmdir File.join(sitedir, 'modbus4r')
      rm 'modbus4r.rb'
      puts "Successfully uninstalled libmodbus4r"
    end
  end
end

begin 
  require 'rake/extensiontask'

  spec = Gem::Specification.new do |s|
    s.name = "libmodbus4r"
    s.version = "0.3.0"
    s.author  = 'Aleksey Timin'
    s.email = 'atimin@gmail.com'
    s.platform = Gem::Platform::RUBY
    s.summary = "Binding use *libmodbus* (free implementation of modbus for Linux\\MacOS) for Ruby."
    s.files = Dir['lib/**/*','examples/*.rb','spec/*.rb', 'ext/**/*']
    s.autorequire = "libmodbus4r"
    s.has_rdoc = true
    s.rdoc_options = ["--title", "libmodbus4r", "--inline-source", "--main", "README"]
    s.extra_rdoc_files = ["README", "LICENSE", "NEWS",]
    s.extensions = ['ext/modbus4r/extconf.rb'] 
    s.require_path = 'lib/modbus4r'
  end

  Rake::GemPackageTask.new(spec) do |pkg|
  end

  Rake::ExtensionTask.new('modbus4r', spec) do |ext|
    ext.lib_dir = 'lib/modbus4r'
  end
rescue
  puts 'rake-compiler not available. Install it with: sudo gem install rake-compiler'
end

task :spec => :compile

task :default => :spec
