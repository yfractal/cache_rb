# frozen_string_literal: true

require "bundler/gem_tasks"
require "rspec/core/rake_task"

RSpec::Core::RakeTask.new(:spec)

task :compile do
  puts "Compiling extension"
  `cd ext && make clean`
  `cd ext && ruby extconf.rb`
  `cd ext && make`
  puts "Compiled"
end

task default: :spec
