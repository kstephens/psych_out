require "bundler/gem_tasks"
gem 'rspec'
require 'rspec/core/rake_task'
require 'rbconfig'

desc "default => all"
task :default => :all

desc "all => [ :ext, :test ]"
task :all => [ :ext, :test ]

desc "Run all tests."
task :test => [ :spec ]

desc "Build extensions."
task :ext do
  c = RbConfig::CONFIG
  # require 'pp'; pp c
  Dir["ext/*/"].sort.each do | d |
    d.chomp!("/")
    Dir.chdir(d) do
      puts "  # building #{d}"
      # load "extconf.rb"
      sh "ruby extconf.rb"
      sh "make"
    end
    f = File.basename(d) << "." << c['DLEXT']
    cp "#{d}/#{f}", "lib/psych_out"
  end
end

desc "Run specs."
RSpec::Core::RakeTask.new(:spec) do |t|
  t.pattern = "./spec/**/*_spec.rb" # don't need this, it's default.
  # Put spec opts in a file named .rspec in root
end
