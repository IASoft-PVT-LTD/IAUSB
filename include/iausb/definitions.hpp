// IAUSB: USB Library for C++
// Copyright (C) 2026 I-A-S (ias@iasoft.dev)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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