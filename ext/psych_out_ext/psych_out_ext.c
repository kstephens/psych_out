#include <ruby.h>
#include <yaml.h>

static VALUE check(VALUE self) {
  return rb_str_new2("ok");
}

void Init_psych_out_ext(void) {
  VALUE module = rb_define_module("PsychOutExt");
  rb_define_singleton_method(module, "check", check, 0);
}
