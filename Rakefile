require 'rbconfig'
require 'fileutils'

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

task :install => :make do
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
end

task :make do
  cd 'ext' do
    ruby 'extconf.rb'
    sh 'make'
    mv 'modbus4r.so', '../lib/modbus4r/'
  end
end
