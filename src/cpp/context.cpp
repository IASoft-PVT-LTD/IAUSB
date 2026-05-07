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