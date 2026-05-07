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