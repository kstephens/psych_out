# -*- encoding: utf-8 -*-
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'psych_out/version'

Gem::Specification.new do |gem|
  gem.name          = "psych_out"
  gem.version       = PsychOut::VERSION
  gem.authors       = ["Kurt Stephens"]
  gem.email         = ["ks.github@enova.com"]
  gem.description   = %q{Control Psych internals.}
  gem.summary       = %q{Control Psych internals.}
  gem.homepage      = "http://github.com/kstephens/psych_out"

  gem.files         = `git ls-files`.split($/)
  gem.executables   = gem.files.grep(%r{^bin/}).map{ |f| File.basename(f) }
  gem.test_files    = gem.files.grep(%r{^(test|spec|features)/})
  gem.require_paths = ["lib"]
end
