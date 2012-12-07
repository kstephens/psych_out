#include <ruby.h>
#include <yaml.h>


static void emit(yaml_emitter_t * emitter, yaml_event_t * event)
{
    if(!yaml_emitter_emit(emitter, event))
	rb_raise(rb_eRuntimeError, "%s", emitter->problem);
}

#ifdef HAVE_RUBY_ENCODING_H
#include <ruby/encoding.h>

static ID id_output_encoding;

static rb_encoding *output_encoding(VALUE self)
{
  rb_encoding *encoding = 0;
  VALUE output_encoding = rb_funcall(self, id_output_encoding, 0);
  if ( ! NIL_P(output_encoding) )
    encoding = rb_enc_get(output_encoding);
  return encoding;
}
#endif

static VALUE scalar_with_encoding(
	VALUE self,
	VALUE value,
	VALUE anchor,
	VALUE tag,
	VALUE plain,
	VALUE quoted,
	VALUE style
	) {
    yaml_emitter_t * emitter;
    yaml_event_t event;
#ifdef HAVE_RUBY_ENCODING_H
    rb_encoding *encoding;
#endif
    Data_Get_Struct(self, yaml_emitter_t, emitter);

    Check_Type(value, T_STRING);

#ifdef HAVE_RUBY_ENCODING_H
    encoding = output_encoding(self);

    fprintf(stderr, "  %p encoding=%p style=%d plain=%p quoted=%p\n", 
            (void*) self, 
            encoding, 
            NUM2INT(style),
            (void*) plain,
            (void*) quoted);

#if 0
    yaml_emitter_set_unicode(emitter, 0);
    yaml_emitter_set_encoding(emitter, YAML_ANY_ENCODING);
#endif 0

    if ( encoding )
      value = rb_str_export_to_enc(value, encoding);

    if(!NIL_P(anchor)) {
	Check_Type(anchor, T_STRING);
        if ( encoding )
          anchor = rb_str_export_to_enc(anchor, encoding);
    }

    if(!NIL_P(tag)) {
	Check_Type(tag, T_STRING);
        if ( encoding )
          tag = rb_str_export_to_enc(tag, encoding);
    }
#endif

    yaml_scalar_event_initialize(
	    &event,
	    (yaml_char_t *)(NIL_P(anchor) ? NULL : StringValuePtr(anchor)),
	    (yaml_char_t *)(NIL_P(tag) ? NULL : StringValuePtr(tag)),
	    (yaml_char_t*)StringValuePtr(value),
	    (int)RSTRING_LEN(value),
	    plain ? 1 : 0,
	    quoted ? 1 : 0,
	    (yaml_scalar_style_t)NUM2INT(style)
	    );

    emit(emitter, &event);

    return self;
}


#define INT_SETTER(F)                                   \
  static VALUE F(VALUE self, VALUE x) {                 \
    yaml_emitter_t * emitter;                           \
    Data_Get_Struct(self, yaml_emitter_t, emitter);     \
    yaml_emitter_##F(emitter, NUM2INT(x));              \
    return self;                                        \
  }

// INT_SETTER(set_indent);
// INT_SETTER(set_width);
// INT_SETTER(set_canonical);
INT_SETTER(set_unicode);
INT_SETTER(set_encoding);
INT_SETTER(set_break);

#undef INT_SETTER

void Init_psych_out_emitter_ext(void) {
  VALUE module = rb_define_module("PsychOutEmitterExt");

  rb_define_method(module, "scalar_with_encoding", scalar_with_encoding, 6);

#define INT_SETTER(F) \
  rb_define_method(module, #F, F, 2)

  // INT_SETTER(set_indent);
  // INT_SETTER(set_width);
  // INT_SETTER(set_canonical);
  INT_SETTER(set_unicode);
  INT_SETTER(set_encoding);
  INT_SETTER(set_break);

#undef INT_SETTER

#define INT_CONSTANT(X) \
  rb_define_const(module, #X, INT2NUM(X))
  
  INT_CONSTANT(YAML_ANY_ENCODING);
  INT_CONSTANT(YAML_UTF8_ENCODING);
  INT_CONSTANT(YAML_UTF16LE_ENCODING);
  INT_CONSTANT(YAML_UTF16BE_ENCODING);

  INT_CONSTANT(YAML_ANY_BREAK);
  INT_CONSTANT(YAML_CR_BREAK);
  INT_CONSTANT(YAML_LN_BREAK);
  INT_CONSTANT(YAML_CRLN_BREAK);

#undef INT_CONSTANT

  id_output_encoding = rb_intern("output_encoding");
}

