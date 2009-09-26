require 'mkmf'

printf "checking for ruby version... "
STDOUT.flush
ruby_ver = RUBY_VERSION
puts ruby_ver
$CFLAGS += " -DRUBY_1_8" if ruby_ver.to_f == 1.8

create_makefile 'modbus4r'
