// IAUSB: USB Library for C++
// Copyright (C) 2026 I-A-S (ias@iasoft.dev)
// Copyright (C) 2026 IASoft (PVT) LTD (contact@iasoft.dev)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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