// IAUSB: USB Library for C++
//
// Copyright (C) 2026 I-A-S (ias@iasoft.dev)
// Copyright (C) 2026 IASoft (PVT) LTD (contact@iasoft.dev)
//
// This source code is licensed under the PolyForm Noncommercial License 1.0.0.
// A copy of this license is included in the LICENSE file at the root of this project,
// and is also available at <https://polyformproject.org/licenses/noncommercial/1.0.0>.

#include <iausb_connection.hpp>

namespace iausb
{
  auto __internal_destory_connection(HeapAllocator *allocator, Connection_T *connection) -> void
  {
    if (!connection)
      return;

    assert(connection->libusb_handle);

    if (connection->libusb_handle != nullptr)
    {
      libusb_release_interface(connection->libusb_handle, connection->interface);
      libusb_close(connection->libusb_handle);
      connection->libusb_handle = nullptr;
    }

    connection->~Connection_T();
    allocator->free(connection, sizeof(Connection_T), alignof(Connection_T));
  }
} // namespace iausb