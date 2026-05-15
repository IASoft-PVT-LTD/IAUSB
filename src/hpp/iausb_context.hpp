// IAUSB: USB Library for C++
//
// Copyright (C) 2026 I-A-S (ias@iasoft.dev)
// Copyright (C) 2026 IASoft (PVT) LTD (contact@iasoft.dev)
//
// This source code is licensed under the PolyForm Noncommercial License 1.0.0.
// A copy of this license is included in the LICENSE file at the root of this project,
// and is also available at <https://polyformproject.org/licenses/noncommercial/1.0.0>.

#pragma once

#include <iausb/iausb.hpp>

#include <libusb.h>

namespace iausb
{
  struct Context_T
  {
    libusb_context *libusb_ctx{};

    auto initialize() -> Result<void>;
  };
} // namespace iausb