// IAUSB: USB Library for C++
//
// Copyright (C) 2026 I-A-S (ias@iasoft.dev)
// Copyright (C) 2026 IASoft (PVT) LTD (contact@iasoft.dev)
//
// This source code is licensed under the PolyForm Noncommercial License 1.0.0.
// A copy of this license is included in the LICENSE file at the root of this project,
// and is also available at <https://polyformproject.org/licenses/noncommercial/1.0.0>.

#include <iausb_context.hpp>
#include <iausb_device.hpp>
#include <iausb_connection.hpp>

#include <regex>

namespace iausb
{
  auto create_context() -> Result<Context>
  {
    HeapAllocator alloc;
    void *mem = alloc.alloc(sizeof(Context_T), alignof(Context_T));
    if (!mem)
      return fail("failed to allocate context");

    const auto context = new (mem) Context_T();
    const auto init_res = context->initialize();
    if (!init_res)
    {
      context->~Context_T();
      alloc.free(context, sizeof(Context_T), alignof(Context_T));
      return fail(init_res.error());
    }

    return Box<Context_T, Deleter_Context_T<HeapAllocator>>{context, Deleter_Context_T<HeapAllocator>{alloc}};
  }

  auto list_devices(Context &ctx) -> Result<Vec<Device>>
  {
    libusb_device **device_list = nullptr;
    const auto device_count = libusb_get_device_list(ctx->libusb_ctx, &device_list);
    if (device_count < 0)
    {
      return fail("libusb_get_device_list failed");
    }

    Vec<Device> devices;
    devices.reserve(static_cast<size_t>(device_count));

    HeapAllocator alloc;
    for (ssize_t i = 0; i < device_count; ++i)
    {
      void *mem = alloc.alloc(sizeof(Device_T), alignof(Device_T));
      if (!mem)
      {
        libusb_free_device_list(device_list, 0);
        return fail("failed to allocate device");
      }
      devices.push_back(Box<Device_T, Deleter_Device_T<HeapAllocator>>{new (mem) Device_T{device_list[i]},
                                                                       Deleter_Device_T<HeapAllocator>{alloc}});
    }

    libusb_free_device_list(device_list, 0);

    return devices;
  }

  auto find_device(Context &ctx, const String &manufacturer_regex_pattern, const String &product_regex_pattern)
      -> Result<Device>
  {
    std::regex manufacturer_regex;
    bool check_manufacturer = !manufacturer_regex_pattern.empty();
    if (check_manufacturer)
    {
      manufacturer_regex = std::regex(manufacturer_regex_pattern.c_str());
    }

    std::regex product_regex;
    bool check_product = !product_regex_pattern.empty();
    if (check_product)
    {
      product_regex = std::regex(product_regex_pattern.c_str());
    }

    AU_TRY_VAR(devices, list_devices(ctx));

    for (auto &device : devices)
    {
      const auto info = get_device_info(device);
      if (!info)
        continue;

      bool manufacturer_match = true;
      if (check_manufacturer)
      {
        manufacturer_match = std::regex_search(info->manufacturer.c_str(), manufacturer_regex);
      }

      bool product_match = true;
      if (check_product)
      {
        product_match = std::regex_search(info->product.c_str(), product_regex);
      }

      if (manufacturer_match && product_match)
      {
        return std::move(device);
      }
    }

    return fail("Failed to find a device matching '%s' and '%s'", manufacturer_regex_pattern.c_str(),
                product_regex_pattern.c_str());
  }

  auto get_device_info(const Device &device) -> Result<DeviceInfo>
  {
    static const auto get_string_descriptor_ascii = [](libusb_device_handle *handle, u8 string_index) -> String {
      if (string_index == 0)
        return {};
      unsigned char text[256] = {0};
      const auto result = libusb_get_string_descriptor_ascii(handle, string_index, text, sizeof(text));
      if (result <= 0)
        return {};
      return String(reinterpret_cast<const char *>(text), static_cast<usize>(result));
    };

    libusb_device_descriptor descriptor = {};
    if (libusb_get_device_descriptor(device->libusb_device, &descriptor) != 0)
      return fail("libusb_get_device_descriptor failed");

    DeviceInfo info{};

    libusb_device_handle *handle = nullptr;
    if (libusb_open(device->libusb_device, &handle) == 0 && handle)
    {
      info.product = get_string_descriptor_ascii(handle, descriptor.iProduct);
      info.manufacturer = get_string_descriptor_ascii(handle, descriptor.iManufacturer);
      info.serial_number = get_string_descriptor_ascii(handle, descriptor.iSerialNumber);
      libusb_close(handle);
    }

    info.bus_number = libusb_get_bus_number(device->libusb_device);
    info.address = libusb_get_device_address(device->libusb_device);
    info.vendor_id = descriptor.idVendor;
    info.product_id = descriptor.idProduct;

    return info;
  }

  auto open_device(const Device &device, u16 interface) -> Result<Connection>
  {
    libusb_device_handle *handle = nullptr;
    if (libusb_open(device->libusb_device, &handle) != 0 || !handle)
      return fail("libusb_open failed");

    if (libusb_kernel_driver_active(handle, interface) == 1)
    {
      if (libusb_detach_kernel_driver(handle, interface) != 0)
      {
        libusb_close(handle);
        return fail("libusb_detach_kernel_driver failed");
      }
    }

    if (libusb_claim_interface(handle, interface) != 0)
    {
      libusb_close(handle);
      return fail("libusb_claim_interface failed");
    }

    HeapAllocator alloc;
    void *mem = alloc.alloc(sizeof(Connection_T), alignof(Connection_T));
    if (!mem)
    {
      libusb_release_interface(handle, interface);
      libusb_close(handle);
      return fail("failed to allocate connection");
    }

    return Box<Connection_T, Deleter_Connection_T<HeapAllocator>>{new (mem) Connection_T{interface, handle},
                                                                  Deleter_Connection_T<HeapAllocator>{alloc}};
  }

  auto write(Connection &connection, u16 endpoint, Span<const u8> data, i32 timeout) -> Result<void>
  {
    i32 transferred = 0;
    const auto t = libusb_bulk_transfer(connection->libusb_handle, endpoint, const_cast<u8 *>(data.data()),
                                        static_cast<int>(data.size()), &transferred, timeout);
    if (t != 0 || transferred != static_cast<int>(data.size()))
      return fail("libusb_bulk_transfer failed");
    return {};
  }

  auto read(Connection &connection, u16 endpoint, i32 max_length, i32 timeout) -> Result<Vec<u8>>
  {
    Vec<u8> data;
    data.resize(max_length);

    i32 transferred = 0;
    const auto t =
        libusb_bulk_transfer(connection->libusb_handle, endpoint, data.data(), max_length, &transferred, timeout);

    if (t != 0 || transferred > max_length)
      return fail("libusb_bulk_transfer failed");

    data.resize(transferred);

    return data;
  }
} // namespace iausb
