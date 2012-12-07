require 'mkmf'
create_header
create_makefile(File.basename(File.dirname(File.expand_path(__FILE__))), '.')
