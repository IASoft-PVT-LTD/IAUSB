<div align="center">
  <img src="logo.png" alt="IAUSB Logo" width="190" style="padding-bottom: 8px;"/>
  <br/>

  <img src="https://img.shields.io/badge/license-Apache v2-darkblue.svg" alt="License"/>
  <img src="https://img.shields.io/badge/platform-Win64 | Linux-darkred.svg" alt="Platform"/>
  <img src="https://img.shields.io/badge/compiler-MSVC | Clang-darkgreen.svg" alt="Compiler"/>

  <p style="padding-top: 0.2rem;">
    <b>IAUSB: USB Library for C++</b>
  </p>
</div>

IAUSB is a USB library for C++. It is built on top of libusb and supports compilation with native MSVC and Clang. It leverages the Auxid platform for memory management and error handling.

### Core features

- **Modern C++ API:** Clean, memory-safe abstraction over `libusb`.
- **Memory Safety:** Custom allocation support and automatic resource cleanup via `auxid`'s `Box` and `Deleter` mechanisms.
- **Robust Error Handling:** Uses `au::Result` instead of exceptions for predictable control flow.
- **Device Discovery:** Easily list devices or find specific ones using `std::regex` for manufacturer and product names.
- **Cross-Platform:** Works on Windows and Linux, supporting both MSVC and Clang.

## Quick start (CMake)

You can easily integrate IAUSB into your project using CMake's `FetchContent`:

```cmake
include(FetchContent)

FetchContent_Declare(
    iausb
    GIT_REPOSITORY https://github.com/I-A-S/iausb.git
    GIT_TAG        main # Or a specific release tag
)
FetchContent_MakeAvailable(iausb)

# Link against your target
target_link_libraries(your_target PRIVATE iausb)
```

### Example

Here is a simple example demonstrating how to find a device (e.g., an Altera USB-Blaster), open a connection, and perform a bulk write:

```cpp
#include <iausb/iausb.hpp>

using namespace au;

auto app_main() -> Result<void>
{
  // 1. Initialize the USB context
  AU_TRY_VAR(context, iausb::create_context());

  // 2. Find the device using Regex (Manufacturer: "Altera", Product: "USB-Blaster")
  AU_TRY_VAR(blaster_device, iausb::find_device(context, "Altera", "USB-Blaster"));

  // 3. Open the device and claim interface 0
  AU_TRY_VAR(connection, iausb::open_device(blaster_device, 0));

  // 4. Perform a bulk write to endpoint 0x02
  AU_TRY_DISCARD(iausb::write(connection, 0x02, {1 << 5}));

  return {};
}

int main(int argc, char *argv[])
{
  // Initialize the Auxid platform main thread
  auxid::MainThreadGuard _main_thread_guard;
  
  const auto result = app_main();
  if (!result)
  {
    auxid::get_thread_logger().error("Application failed with error: %s", result.error().c_str());
    return 1;
  }
  
  return 0;
}
```

## License

Copyright © 2026 IASoft (PVT LTD). Licensed under the [Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0).

> [!NOTE]
> While IAUSB is released under the Apache 2.0 license, the dependency `libftdi` is licensed under the LGPL v2.1.
> `libftdi` is built and linked as a dynamic library by the CMake files in this repository by default.
> This means your code will not be subject to the LGPL's copyleft requirements for derivative works, as long as you do not link `libftdi` statically.
