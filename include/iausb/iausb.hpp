// IAUSB: USB Library for C++
//
// Copyright (C) 2026 I-A-S (ias@iasoft.dev)
// Copyright (C) 2026 IASoft (PVT) LTD (contact@iasoft.dev)
//
// This source code is licensed under the PolyForm Noncommercial License 1.0.0.
// A copy of this license is included in the LICENSE file at the root of this project,
// and is also available at <https://polyformproject.org/licenses/noncommercial/1.0.0>.

#pragma once

#include <iausb/definitions.hpp>

namespace iausb
{
  using Context = Box<Context_T, Deleter_Context_T<HeapAllocator>>;
  using Device = Box<Device_T, Deleter_Device_T<HeapAllocator>>;
  using Connection = Box<Connection_T, Deleter_Connection_T<HeapAllocator>>;

  auto create_context() -> Result<Context>;

  auto list_devices(Context &ctx) -> Result<Vec<Device>>;
  auto find_device(Context &ctx, const String &manufacturer_regex_pattern, const String &product_regex_pattern)
      -> Result<Device>;

  auto get_device_info(const Device &device) -> Result<DeviceInfo>;

  auto open_device(const Device &device, u16 interface) -> Result<Connection>;

  auto write(Connection &connection, u16 endpoint, Span<const u8> data, i32 timeout = 1000) -> Result<void>;
  auto read(Connection &connection, u16 endpoint, i32 max_length, i32 timeout = 1000) -> Result<Vec<u8>>;
} // namespace iausb
