// IAUSB: USB Library for C++
//
// Copyright (C) 2026 I-A-S (ias@iasoft.dev)
// Copyright (C) 2026 IASoft (PVT) LTD (contact@iasoft.dev)
//
// This source code is licensed under the PolyForm Noncommercial License 1.0.0.
// A copy of this license is included in the LICENSE file at the root of this project,
// and is also available at <https://polyformproject.org/licenses/noncommercial/1.0.0>.

#pragma once

#include <auxid/auxid.hpp>
#include <auxid/containers/vec.hpp>
#include <auxid/memory/box.hpp>

namespace iausb
{
  using namespace au;
  using namespace au::memory;

  struct Context_T;
  struct Device_T;
  struct Connection_T;

  auto __internal_destory_context(HeapAllocator *allocator, Context_T *context) -> void;
  auto __internal_destory_device(HeapAllocator *allocator, Device_T *device) -> void;
  auto __internal_destory_connection(HeapAllocator *allocator, Connection_T *connection) -> void;

  AU_DECLARE_CUSTOM_DELETER(Context_T, __internal_destory_context);
  AU_DECLARE_CUSTOM_DELETER(Device_T, __internal_destory_device);
  AU_DECLARE_CUSTOM_DELETER(Connection_T, __internal_destory_connection);

  struct DeviceInfo
  {
    String product;
    String manufacturer;
    String serial_number;

    u8 bus_number;
    u8 address;
    u16 vendor_id;
    u16 product_id;
  };
} // namespace iausb