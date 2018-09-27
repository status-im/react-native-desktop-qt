/**
 * Copyright (c) 2016-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <jschelpers/JSCWrapper.h>

#include <mutex>

// Crash the app (with a descriptive stack trace) if a function that is not supported by
// the system JSC is called.
#define UNIMPLEMENTED_SYSTEM_JSC_FUNCTION(FUNC_NAME)            \
static void Unimplemented_##FUNC_NAME(void* args...) {}

UNIMPLEMENTED_SYSTEM_JSC_FUNCTION(JSEvaluateBytecodeBundle)
#if WITH_FBJSCEXTENSIONS
UNIMPLEMENTED_SYSTEM_JSC_FUNCTION(JSStringCreateWithUTF8CStringExpectAscii)
#endif
UNIMPLEMENTED_SYSTEM_JSC_FUNCTION(JSPokeSamplingProfiler)
UNIMPLEMENTED_SYSTEM_JSC_FUNCTION(JSStartSamplingProfilingOnMainJSCThread)

UNIMPLEMENTED_SYSTEM_JSC_FUNCTION(JSGlobalContextEnableDebugger)
UNIMPLEMENTED_SYSTEM_JSC_FUNCTION(JSGlobalContextDisableDebugger)

UNIMPLEMENTED_SYSTEM_JSC_FUNCTION(configureJSCForIOS)

UNIMPLEMENTED_SYSTEM_JSC_FUNCTION(FBJSContextStartGCTimers)

bool JSSamplingProfilerEnabled() {
  return false;
}

const int32_t JSNoBytecodeFileFormatVersion = -1;

void JSGlobalContextEnableDebugger(
  JSGlobalContextRef ctx, facebook::react::IInspector &globalInspector,
   const char *title, const std::function<bool()> &checkIsInspectedRemote) {}
void JSGlobalContextDisableDebugger(
  JSGlobalContextRef ctx, facebook::react::IInspector &globalInspector) {}

// This is used to substitute an alternate JSC implementation for
// testing. These calls must all be ABI compatible with the standard JSC.
JSValueRef JSEvaluateBytecodeBundle(JSContextRef, JSObjectRef, int, JSStringRef,
                                    JSValueRef *) {
  return JSValueRef();
}
void JSStartSamplingProfilingOnMainJSCThread(JSGlobalContextRef) {}
JSValueRef JSPokeSamplingProfiler(JSContextRef) { return JSValueRef(); }

namespace facebook {
namespace react {

static JSCWrapper s_systemWrapper = {};

const JSCWrapper* systemJSCWrapper() {
  // Note that this is not used on Android. All methods are statically linked instead.
  // Some fields are lazily initialized
  static std::once_flag flag;
  std::call_once(flag, []() {
    s_systemWrapper = {
      .JSGlobalContextCreateInGroup_ = JSGlobalContextCreateInGroup,
      .JSGlobalContextRelease_ = JSGlobalContextRelease,
      .JSGlobalContextSetName_ = JSGlobalContextSetName,

      .JSContextGetGlobalContext_ = JSContextGetGlobalContext,
      .JSContextGetGlobalObject_ = JSContextGetGlobalObject,
      .FBJSContextStartGCTimers_ =
        (decltype(&FBJSContextStartGCTimers))
        Unimplemented_FBJSContextStartGCTimers,

      .JSEvaluateScript_ = JSEvaluateScript,
      .JSEvaluateBytecodeBundle_ =
        (decltype(&JSEvaluateBytecodeBundle))
        Unimplemented_JSEvaluateBytecodeBundle,

      .JSStringCreateWithUTF8CString_ = JSStringCreateWithUTF8CString,
      //.JSStringCreateWithCFString_ = JSStringCreateWithCFString,
      #if WITH_FBJSCEXTENSIONS
      .JSStringCreateWithUTF8CStringExpectAscii =
        (decltype(&JSStringCreateWithUTF8CStringExpectAscii))
        Unimplemented_JSStringCreateWithUTF8CStringExpectAscii,
      #endif
      //.JSStringCopyCFString_ = JSStringCopyCFString,
      .JSStringGetCharactersPtr_ = JSStringGetCharactersPtr,
      .JSStringGetLength_ = JSStringGetLength,
      .JSStringGetMaximumUTF8CStringSize_ = JSStringGetMaximumUTF8CStringSize,
      .JSStringIsEqualToUTF8CString_ = JSStringIsEqualToUTF8CString,
      .JSStringRelease_ = JSStringRelease,
      .JSStringRetain_ = JSStringRetain,

      .JSClassCreate_ = JSClassCreate,
      .JSClassRetain_ = JSClassRetain,
      .JSClassRelease_ = JSClassRelease,

      .JSObjectCallAsConstructor_ = JSObjectCallAsConstructor,
      .JSObjectCallAsFunction_ = JSObjectCallAsFunction,
      .JSObjectGetPrivate_ = JSObjectGetPrivate,
      .JSObjectGetProperty_ = JSObjectGetProperty,
      .JSObjectGetPropertyAtIndex_ = JSObjectGetPropertyAtIndex,
      .JSObjectIsConstructor_ = JSObjectIsConstructor,
      .JSObjectIsFunction_ = JSObjectIsFunction,
      .JSObjectMake_ = JSObjectMake,
      .JSObjectMakeArray_ = JSObjectMakeArray,
      .JSObjectMakeDate_ = JSObjectMakeDate,
      .JSObjectMakeError_ = JSObjectMakeError,
      .JSObjectMakeFunctionWithCallback_ = JSObjectMakeFunctionWithCallback,
      .JSObjectSetPrivate_ = JSObjectSetPrivate,
      .JSObjectSetProperty_ = JSObjectSetProperty,
      .JSObjectSetPropertyAtIndex_ = JSObjectSetPropertyAtIndex,

      .JSObjectCopyPropertyNames_ = JSObjectCopyPropertyNames,
      .JSPropertyNameArrayGetCount_ = JSPropertyNameArrayGetCount,
      .JSPropertyNameArrayGetNameAtIndex_ = JSPropertyNameArrayGetNameAtIndex,
      .JSPropertyNameArrayRelease_ = JSPropertyNameArrayRelease,

      .JSValueCreateJSONString_ = JSValueCreateJSONString,
      .JSValueGetType_ = JSValueGetType,
      .JSValueMakeFromJSONString_ = JSValueMakeFromJSONString,
      .JSValueMakeBoolean_ = JSValueMakeBoolean,
      .JSValueMakeNull_ = JSValueMakeNull,
      .JSValueMakeNumber_ = JSValueMakeNumber,
      .JSValueMakeString_ = JSValueMakeString,
      .JSValueMakeUndefined_ = JSValueMakeUndefined,
      .JSValueProtect_ = JSValueProtect,
      .JSValueToBoolean_ = JSValueToBoolean,
      .JSValueToNumber_ = JSValueToNumber,
      .JSValueToObject_ = JSValueToObject,
      .JSValueToStringCopy_ = JSValueToStringCopy,
      .JSValueUnprotect_ = JSValueUnprotect,

      .JSSamplingProfilerEnabled_ = JSSamplingProfilerEnabled,
      .JSPokeSamplingProfiler_ =
        (decltype(&JSPokeSamplingProfiler))
        Unimplemented_JSPokeSamplingProfiler,
      .JSStartSamplingProfilingOnMainJSCThread_ =
        (decltype(&JSStartSamplingProfilingOnMainJSCThread))
        Unimplemented_JSStartSamplingProfilingOnMainJSCThread,

      .JSGlobalContextEnableDebugger_ =
        (decltype(&JSGlobalContextEnableDebugger))
        Unimplemented_JSGlobalContextEnableDebugger,
      .JSGlobalContextDisableDebugger_ =
        (decltype(&JSGlobalContextDisableDebugger))
        Unimplemented_JSGlobalContextDisableDebugger,

      .configureJSCForIOS_ =
        (decltype(&configureJSCForIOS))Unimplemented_configureJSCForIOS,

      //.JSContext_ = objc_getClass("JSContext"),
      //.JSValue_ = objc_getClass("JSValue"),

      .JSBytecodeFileFormatVersion = JSNoBytecodeFileFormatVersion,
    };
  });
  return &s_systemWrapper;
}

} }
