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
