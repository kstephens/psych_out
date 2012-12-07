require "psych_out/version"

module PsychOut
  @@activated = false
  def self.activate!
    return if @@activated
    require 'psych_out/psych_out_ext'
    require 'psych_out/psych_out_emitter_ext'
    PsychOutEmitterExt.module_eval do
      attr_accessor :output_encoding
    end
    Psych::Emitter.send(:include, PsychOutEmitterExt)
    @@activated = true
  end
end

require 'yaml'
require 'psych'

