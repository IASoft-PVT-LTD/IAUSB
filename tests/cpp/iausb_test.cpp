#include <iausb/iausb.hpp>
#include <auxid/utils/test.hpp>

using namespace au;

AUT_BEGIN_BLOCK(iausb, basic_tests)

auto test_create_context() -> bool
{
  auto ctx_result = iausb::create_context();
  AUT_CHECK(ctx_result.is_ok());

  AUT_CHECK(ctx_result.unwrap() != nullptr);

  return true;
}

auto test_list_devices() -> bool
{
  auto ctx_result = iausb::create_context();
  AUT_CHECK(ctx_result.is_ok());

  auto devices_result = iausb::list_devices(ctx_result.unwrap());
  AUT_CHECK(devices_result.is_ok());

  return true;
}

auto test_find_device_not_found() -> bool
{
  auto ctx_result = iausb::create_context();
  AUT_CHECK(ctx_result.is_ok());

  // Try to find a nonexistent device, should return an error
  auto device_result = iausb::find_device(ctx_result.unwrap(), "NonExistentManufacturerXYZ", "NonExistentProductABC");
  AUT_CHECK(device_result.is_err());

  return true;
}

AUT_BEGIN_TEST_LIST()
AUT_ADD_TEST(test_create_context);
AUT_ADD_TEST(test_list_devices);
AUT_ADD_TEST(test_find_device_not_found);
AUT_END_TEST_LIST()

AUT_END_BLOCK()

AUT_REGISTER_ENTRY(iausb, basic_tests);
