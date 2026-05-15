// IAUSB: USB Library for C++
//
// Copyright (C) 2026 I-A-S (ias@iasoft.dev)
// Copyright (C) 2026 IASoft (PVT) LTD (contact@iasoft.dev)
//
// This source code is licensed under the PolyForm Noncommercial License 1.0.0.
// A copy of this license is included in the LICENSE file at the root of this project,
// and is also available at <https://polyformproject.org/licenses/noncommercial/1.0.0>.

#include <iausb_context.hpp>

namespace iausb
{
  auto Context_T::initialize() -> Result<void>
  {
    assert(!libusb_ctx);

    const auto init_result = libusb_init(&libusb_ctx);
    if (init_result < 0)
      return fail("Failed to initialize libusb context with error %d", init_result);

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
    }

    context->~Context_T();
    allocator->free(context, sizeof(Context_T), alignof(Context_T));
  }
} // namespace iausb