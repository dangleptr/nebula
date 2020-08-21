/* Copyright (c) 2020 vesoft inc. All rights reserved.
 *
 * obj source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */
#pragma once

#include <thrift/lib/cpp2/GeneratedSerializationCodeHelper.h>
#include <thrift/lib/cpp2/gen/module_types_tcc.h>
#include <thrift/lib/cpp2/protocol/ProtocolReaderStructReadState.h>
#include "base/VertexID.h"

namespace apache {
namespace thrift {

namespace detail {
template <>
struct TccStructTraits<nebula::VertexID> {
    static void translateFieldName(
        folly::StringPiece _fname,
        int16_t& fid,
        apache::thrift::protocol::TType& _ftype) {
        if (_fname == "first") {
            fid = 1;
            _ftype = apache::thrift::protocol::T_I64;
        } else if (_fname == "second") {
            fid = 2;
            _ftype = apache::thrift::protocol::T_I64;
        }
    }
};

}  // namespace detail

template <>
inline void Cpp2Ops<nebula::VertexID>::clear(nebula::VertexID* obj) {
  return obj->clear();
}

template <>
inline constexpr apache::thrift::protocol::TType Cpp2Ops<nebula::VertexID>::thriftType() {
  return apache::thrift::protocol::T_STRUCT;
}

template <>
template <class Protocol> uint32_t Cpp2Ops<nebula::VertexID>::write(Protocol* proto,
                                                                    nebula::VertexID const* obj) {
    uint32_t xfer = 0;
    xfer += proto->writeStructBegin("VertexID");
    xfer += proto->writeFieldBegin("first", apache::thrift::protocol::T_I64, 1);
    xfer += detail::pm::protocol_methods<type_class::integral, int64_t>::write(*proto, obj->first);
    xfer += proto->writeFieldEnd();
    xfer += proto->writeFieldBegin("second", apache::thrift::protocol::T_I64, 2);
    xfer += detail::pm::protocol_methods<type_class::integral, int64_t>::write(*proto, obj->second);
    xfer += proto->writeFieldEnd();
    xfer += proto->writeFieldStop();
    xfer += proto->writeStructEnd();
    return xfer;
}

template <>
template <class Protocol> void Cpp2Ops< nebula::VertexID>::read(Protocol* proto,
                                                                nebula::VertexID* obj) {
    apache::thrift::detail::ProtocolReaderStructReadState<Protocol> _readState;
    _readState.readStructBegin(proto);

    using apache::thrift::TProtocolException;

    if (UNLIKELY(!_readState.advanceToNextField(
          proto,
          0,
          1,
          apache::thrift::protocol::T_I64))) {
        goto _loop;
    }

_readField_first:
    {
        detail::pm::protocol_methods<type_class::integral, int64_t>::read(*proto, obj->first);
    }

    if (UNLIKELY(!_readState.advanceToNextField(
          proto,
          1,
          2,
          apache::thrift::protocol::T_I64))) {
        goto _loop;
    }

_readField_second:
    {
        detail::pm::protocol_methods<type_class::integral, int64_t>::read(*proto, obj->second);
    }

    if (UNLIKELY(!_readState.advanceToNextField(
          proto,
          2,
          0,
          apache::thrift::protocol::T_STOP))) {
        goto _loop;
    }

_end:
    _readState.readStructEnd(proto);

    return;

_loop:
    if (_readState.fieldType == apache::thrift::protocol::T_STOP) {
        goto _end;
    }

    if (proto->kUsesFieldNames()) {
        detail::TccStructTraits<nebula::VertexID>::translateFieldName(_readState.fieldName(),
                                                                      _readState.fieldId,
                                                                      _readState.fieldType);
    }

    switch (_readState.fieldId) {
        case 1: {
            if (LIKELY(_readState.fieldType == apache::thrift::protocol::T_I64)) {
                goto _readField_first;
            } else {
                goto _skip;
            }
        }
        case 2: {
            if (LIKELY(_readState.fieldType == apache::thrift::protocol::T_I64)) {
                goto _readField_second;
            } else {
                goto _skip;
            }
        }
        default: {
_skip:
            proto->skip(_readState.fieldType);
            _readState.readFieldEnd(proto);
            _readState.readFieldBeginNoInline(proto);
            goto _loop;
        }
    }
}

template <>
template <class Protocol>
uint32_t Cpp2Ops< nebula::VertexID>::serializedSize(Protocol const* proto,
                                                    nebula::VertexID const* obj) {
    uint32_t xfer = 0;
    xfer += proto->serializedStructSize("VertexID");

    xfer += proto->serializedFieldSize("first", apache::thrift::protocol::T_I64, 1);
    xfer += detail::pm::protocol_methods<type_class::integral, int64_t>
                    ::serializedSize<false>(*proto, obj->first);
    xfer += proto->serializedFieldSize("second", apache::thrift::protocol::T_I64, 2);
    xfer += detail::pm::protocol_methods<type_class::integral, int64_t>
                            ::serializedSize<false>(*proto, obj->second);
    xfer += proto->serializedSizeStop();
    return xfer;
}

template <>
template <class Protocol>
uint32_t Cpp2Ops< nebula::VertexID>::serializedSizeZC(Protocol const* proto,
                                                      nebula::VertexID const* obj) {
    uint32_t xfer = 0;
    xfer += proto->serializedStructSize("VertexID");

    xfer += proto->serializedFieldSize("first", apache::thrift::protocol::T_I64, 1);
    xfer += detail::pm::protocol_methods<type_class::integral, int64_t>
                    ::serializedSize<false>(*proto, obj->first);

    xfer += proto->serializedFieldSize("second", apache::thrift::protocol::T_I64, 2);
    xfer += detail::pm::protocol_methods<type_class::integral, int64_t>
                    ::serializedSize<false>(*proto, obj->second);

    xfer += proto->serializedSizeStop();
    return xfer;
}

}  // namespace thrift
}  // namespace apache
