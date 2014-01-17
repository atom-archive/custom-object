// Copyright (c) 2014 GitHub, Inc. All rights reserved.

#include "nan.h"
using namespace v8;

inline bool ObjectGet(Handle<Object> object,
                      const char* key,
                      Handle<Object>* value) {
  Handle<Value> v = object->Get(NanSymbol(key));
  if (!v->IsObject())
    return false;
  *value = v->ToObject();
  return true;
}

inline bool ObjectGet(Handle<Object> object,
                      const char* key,
                      Handle<String>* value) {
  Handle<Value> v = object->Get(NanSymbol(key));
  if (!v->IsString())
    return false;
  *value = v->ToString();
  return true;
}

inline bool ObjectGet(Handle<Object> object,
                      const char* key,
                      Handle<Function>* value) {
  Handle<Value> v = object->Get(NanSymbol(key));
  if (!v->IsFunction())
    return false;
  *value = Handle<Function>::Cast(v);
  return true;
}

inline bool ObjectGet(Handle<Object> object,
                      const char* key,
                      Handle<Array>* value) {
  Handle<Value> v = object->Get(NanSymbol(key));
  if (!v->IsArray())
    return false;
  *value = Handle<Array>::Cast(v);
  return true;
}
