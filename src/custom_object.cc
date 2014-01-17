// Copyright (c) 2014 GitHub, Inc. All rights reserved.

#include <string>

#include "arraysize.h"
#include "object_helper.h"

namespace {

NAN_GETTER(CustomAccessorGetter) {
  NanScope();
  Handle<Function> getter;
  if (!ObjectGet(args.Data()->ToObject(), "getter", &getter))
    return NanThrowError("Cannot find getter");

  Handle<Value> argv[] = { property };
  NanReturnValue(getter->Call(args.This(), arraysize(argv), argv));
}

NAN_SETTER(CustomAccessorSetter) {
  NanScope();
  Handle<Function> setter;
  if (!ObjectGet(args.Data()->ToObject(), "setter", &setter))
    return NanThrowError("Cannot find setter");

  Handle<Value> argv[] = { property, value };
  NanReturnValue(setter->Call(args.This(), arraysize(argv), argv));
}

bool SetObjectTemplate(Handle<ObjectTemplate> object_template,
                       Handle<Object> options,
                       std::string* error) {
  // accessor: { name: String, getter: Function, setter: Function }
  Handle<Object> accessor;
  if (ObjectGet(options, "accessor", &accessor)) {
    Handle<String> name;
    if (!ObjectGet(accessor, "name", &name)) {
      *error = "The 'name' is required when setting accessor";
      return false;
    }
    if (!accessor->Has(NanSymbol("getter"))) {
      *error = "The 'getter' is required when setting accessor";
      return false;
    }
    // The "setter" is optional.
    AccessorSetterCallback setter = NULL;
    if (accessor->Has(NanSymbol("setter")))
      setter = CustomAccessorSetter;

    object_template->SetAccessor(name, CustomAccessorGetter, setter, accessor);
  }

  return true;
}

NAN_METHOD(CreateConstructor) {
  NanScope();

  if (!args[0]->IsFunction() || !args[1]->IsObject())
    return NanThrowTypeError("Bad argument");

  NanReturnNull();
}

NAN_METHOD(CreateObject) {
  NanScope();

  if (!args[0]->IsObject())
    return NanThrowTypeError("Bad argument");

  Local<ObjectTemplate> object_template = ObjectTemplate::New();
  std::string error;
  if (!SetObjectTemplate(object_template, args[0]->ToObject(), &error))
    return NanThrowError(error.c_str());

  NanReturnValue(object_template->NewInstance());
}

void Init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "createConstructor", CreateConstructor);
  NODE_SET_METHOD(exports, "createObject", CreateObject);
}

}  // namespace

NODE_MODULE(custom_object, Init)
