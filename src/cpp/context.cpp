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

#include <iausb_context.hpp>

namespace iausb
{
  auto Context_T::initialize() -> Result<void>
  {
    assert(!libusb_ctx);

    const auto init_result = libusb_init(&libusb_ctx);
    if (init_result < 0)
      return fail("Failed to initialize libusb context with error %d", init_result);

    auxid::get_thread_logger().trace("libusb context initialized successfully");

    return {};
  }

  auto __internal_destory_context(HeapAllocator *allocator, Context_T *context) -> void
  {
    if (!context)
      return;

    assert(context->libusb_ctx);

    if (context->libusb_ctx != nullptr)
    {
      libusb_exit(context->libusb_ctx);
      context->libusb_ctx = nullptr;
      auxid::get_thread_logger().trace("libusb context destroyed successfully");
    }

    context->~Context_T();
    allocator->free(context, sizeof(Context_T), alignof(Context_T));
  }
} // namespace iausb