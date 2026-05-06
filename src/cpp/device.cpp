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

#include <iausb_device.hpp>

namespace iausb
{
  auto __internal_destory_device(HeapAllocator *allocator, Device_T *device) -> void
  {
    if (!device)
      return;

    assert(device->libusb_device);

    if (device->libusb_device != nullptr)
    {
      libusb_unref_device(device->libusb_device);
      device->libusb_device = nullptr;
    }

    device->~Device_T();
    allocator->free(device, sizeof(Device_T), alignof(Device_T));
  }
} // namespace iausb