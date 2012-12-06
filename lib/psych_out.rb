require "psych_out/version"

module PsychOut
  @@activated = false
  def self.activate!
    return if @@activated
    require 'psych_out/psych_out_ext'
    require 'psych_out/psych_out_emitter_ext'
    Psych::Emitter.send(:include, PsychOutEmitterExt)
    @@activated = true
  end
end

require 'yaml'
require 'psych'

