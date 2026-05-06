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
      auxid::get_thread_logger().trace("libusb connection closed successfully");
    }

    connection->~Connection_T();
    allocator->free(connection, sizeof(Connection_T), alignof(Connection_T));
  }
} // namespace iausb