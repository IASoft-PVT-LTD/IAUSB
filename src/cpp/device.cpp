// IAUSB: USB Library for C++
//
// Copyright (C) 2026 I-A-S (ias@iasoft.dev)
// Copyright (C) 2026 IASoft (PVT) LTD (contact@iasoft.dev)
//
// This source code is licensed under the PolyForm Noncommercial License 1.0.0.
// A copy of this license is included in the LICENSE file at the root of this project,
// and is also available at <https://polyformproject.org/licenses/noncommercial/1.0.0>.

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