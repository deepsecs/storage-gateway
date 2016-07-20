// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: common.proto

#ifndef PROTOBUF_common_2eproto__INCLUDED
#define PROTOBUF_common_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)

namespace huawei {
namespace proto {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_common_2eproto();
void protobuf_AssignDesc_common_2eproto();
void protobuf_ShutdownFile_common_2eproto();


enum RESULT {
  DRS_UNKNOWN = 0,
  DRS_OK = 1,
  INTERNAL_ERROR = 2,
  RESULT_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  RESULT_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool RESULT_IsValid(int value);
const RESULT RESULT_MIN = DRS_UNKNOWN;
const RESULT RESULT_MAX = INTERNAL_ERROR;
const int RESULT_ARRAYSIZE = RESULT_MAX + 1;

const ::google::protobuf::EnumDescriptor* RESULT_descriptor();
inline const ::std::string& RESULT_Name(RESULT value) {
  return ::google::protobuf::internal::NameOfEnum(
    RESULT_descriptor(), value);
}
inline bool RESULT_Parse(
    const ::std::string& name, RESULT* value) {
  return ::google::protobuf::internal::ParseNamedEnum<RESULT>(
    RESULT_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace proto
}  // namespace huawei

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::huawei::proto::RESULT> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::huawei::proto::RESULT>() {
  return ::huawei::proto::RESULT_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_common_2eproto__INCLUDED
