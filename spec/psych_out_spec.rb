require 'psych_out'

describe 'PsychOut' do
  it "should load PsychOutExt" do
    PsychOut.activate!
    PsychOutExt.check.should == 'ok'
  end

  it "should load PsychOutEmitterExt" do
    PsychOut.activate!
    Psych::Emitter.instance_methods.include?(:set_utf8).should == true
    PsychOutEmitterExt.class.should == Module
  end

  it "should handle scalar_with_encoding." do
    PsychOut.activate!

    require 'socket'
    str = Socket.gethostname
    str.encoding.inspect.should == "#<Encoding:ASCII-8BIT>"

    yaml = YAML.dump(str)
    yaml.should == "--- !binary |-\n  dHJhbnM0\n"

    Psych::Emitter.class_eval do
      alias :scalar :scalar_with_encoding
    end

    io = StringIO.new
    visitor = Psych::Visitors::YAMLTree.new({})
    def visitor.binary? str; false; end
    visitor << str
=begin
    emitter = visitor.instance_variable_get('@handler')
    emitter.class.should == nil
    emitter.extend(PsychOutEmitterExt)
    def emitter.scalar *args
      scalar_with_encoding *args
    end
=end
    visitor.tree.yaml io
    io.string.should == ''
  end
end
