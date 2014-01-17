// Copyright (c) 2014 GitHub, Inc. All rights reserved.

#include <string>
#include <vector>

#include "object_helper.h"

namespace {

#define DEFINE_NONE_PARAMETER_FUNC(name) \
  NanScope(); \
  Handle<Function> func; \
  Handle<Object> data = args.Data()->ToObject(); \
  if (!ObjectGet(data, name, &func)) \
    return NanThrowError("Cannot find " name); \
  Handle<Value> argv[] = { data->Get(NanSymbol("data")) }; \
  Handle<Value> return_value = func->Call(args.This(), 1, argv)

#define DEFINE_ONE_PARAMETER_FUNC(name, key) \
  NanScope(); \
  Handle<Function> func; \
  Handle<Object> data = args.Data()->ToObject(); \
  if (!ObjectGet(args.Data()->ToObject(), name, &func)) \
    return NanThrowError("Cannot find " name); \
  Handle<Value> argv[] = { key, data->Get(NanSymbol("data")) }; \
  Handle<Value> return_value = func->Call(args.This(), 2, argv)

#define DEFINE_TWO_PARAMETERS_FUNC(name, key, value) \
  NanScope(); \
  Handle<Function> func; \
  Handle<Object> data = args.Data()->ToObject(); \
  if (!ObjectGet(args.Data()->ToObject(), name, &func)) \
    return NanThrowError("Cannot find " name); \
  Handle<Value> argv[] = { key, value, data->Get(NanSymbol("data")) }; \
  Handle<Value> return_value = func->Call(args.This(), 3, argv)

NAN_INDEX_GETTER(CustomIndexGetter) {
  DEFINE_ONE_PARAMETER_FUNC("getter", Integer::New(index));
  NanReturnValue(return_value);
}

NAN_INDEX_SETTER(CustomIndexSetter) {
  DEFINE_TWO_PARAMETERS_FUNC("setter", Integer::New(index), value);
  NanReturnValue(return_value);
}

NAN_INDEX_QUERY(CustomIndexQuery) {
  DEFINE_ONE_PARAMETER_FUNC("query", Integer::New(index));
  NanReturnValue(return_value->ToInteger());
}

NAN_INDEX_DELETER(CustomIndexDeleter) {
  DEFINE_ONE_PARAMETER_FUNC("deleter", Integer::New(index));
  NanReturnValue(return_value->ToBoolean());
}

NAN_INDEX_ENUMERATOR(CustomIndexEnumerator) {
  DEFINE_NONE_PARAMETER_FUNC("enumerator");
  NanReturnValue(Handle<Array>::Cast(return_value));
}

NAN_GETTER(CustomAccessorGetter) {
  DEFINE_ONE_PARAMETER_FUNC("getter", property);
  NanReturnValue(return_value);
}

NAN_SETTER(CustomAccessorSetter) {
  DEFINE_TWO_PARAMETERS_FUNC("setter", property, value);
  NanReturnValue(return_value);
}

NAN_METHOD(CustomFunctionCallback) {
  NanScope();
  Handle<Function> constructor = Handle<Function>::Cast(args.Data());
  std::vector<Handle<Value> > argv(args.Length());
  for (size_t i = 0; i < argv.size(); ++i)
    argv[i] = args[i];
  NanReturnValue(constructor->Call(args.This(), argv.size(), argv.data()));
}

bool SetObjectTemplate(Handle<ObjectTemplate> object_template,
                       Handle<Object> options,
                       std::string* error) {
  // accessor: [ { name: String, getter: Function, setter: Function } ]
  Handle<Array> accessor_array;
  if (ObjectGet(options, "accessor", &accessor_array)) {
    for (uint32_t i = 0; i < accessor_array->Length(); ++i) {
      Handle<Object> accessor = accessor_array->Get(i)->ToObject();
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

      object_template->SetAccessor(
          name, CustomAccessorGetter, setter, accessor);
    }
  }

  // index: { getter: Function, setter: Function, query: Function,
  //          deleter: Function, enumerator: Function }
  Handle<Object> index;
  if (ObjectGet(options, "index", &index)) {
    if (!index->Has(NanSymbol("getter"))) {
      *error = "The 'getter' is required when setting index";
      return false;
    }
    IndexedPropertySetterCallback setter = NULL;
    if (index->Has(NanSymbol("setter")))
      setter = CustomIndexSetter;
    IndexedPropertyQueryCallback query = NULL;
    if (index->Has(NanSymbol("query")))
      query = CustomIndexQuery;
    IndexedPropertyDeleterCallback deleter = NULL;
    if (index->Has(NanSymbol("deleter")))
      deleter = CustomIndexDeleter;
    IndexedPropertyEnumeratorCallback enumerator = NULL;
    if (index->Has(NanSymbol("enumerator")))
      enumerator = CustomIndexEnumerator;

    object_template->SetIndexedPropertyHandler(
        CustomIndexGetter, setter, query, deleter, enumerator, index);
  }

  return true;
}

NAN_METHOD(CreateConstructor) {
  NanScope();

  if (!args[0]->IsFunction() || !args[1]->IsObject())
    return NanThrowTypeError("Bad argument");

  // Extract args.
  Handle<Function> constructor = Handle<Function>::Cast(args[0]);
  Handle<Object> options = args[1]->ToObject();

  // Create a new FunctionTemplate which will call passed callback.
  Handle<FunctionTemplate> function_template = FunctionTemplate::New(
      CustomFunctionCallback, constructor);
  Handle<Value> function_name = constructor->GetName();
  function_template->SetClassName(function_name->ToString());

  // Modify the FunctionTemplate's InstanceTemplate.
  std::string error;
  if (!SetObjectTemplate(function_template->InstanceTemplate(),
                         options, &error))
    return NanThrowError(error.c_str());

  NanReturnValue(function_template->GetFunction());
}

NAN_METHOD(CreateObject) {
  NanScope();

  if (!args[0]->IsObject())
    return NanThrowTypeError("Bad argument");

  // Create a new ObjectTemplate based on the passed handler.
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
