add_library(iausb STATIC 
    cpp/iausb.cpp
    cpp/context.cpp
    cpp/device.cpp
    cpp/connection.cpp
)

target_include_directories(iausb PUBLIC
    $<BUILD_INTERFACE:${iausb_ROOT}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(iausb PRIVATE hpp)

target_link_libraries(iausb PUBLIC libauxid)
target_link_libraries(iausb PRIVATE 
    usb-1.0    
    auxid_platform_standard
)
