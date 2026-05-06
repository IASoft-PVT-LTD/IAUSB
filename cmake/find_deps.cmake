include(FetchContent)

FetchContent_Declare(
  libftdi
  GIT_REPOSITORY https://github.com/I-A-S/libftdi1-cmake
  GIT_TAG main
  SYSTEM
)

FetchContent_MakeAvailable(libftdi)
