include(FetchContent)

FetchContent_Declare(
  LibUSB1
  GIT_REPOSITORY https://github.com/libusb/libusb-cmake
  GIT_TAG main
  SYSTEM
)

FetchContent_MakeAvailable(LibUSB1)

if(WIN32 AND CMAKE_C_COMPILER_ID MATCHES "Clang" AND TARGET usb-1.0)
  target_compile_options(usb-1.0 PRIVATE
    -Wno-unused-value
    -Wno-pragma-pack
    -Wno-deprecated-declarations
  )
endif()
