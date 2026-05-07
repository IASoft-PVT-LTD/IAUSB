set(SRC_FILES
    "cpp/main.cpp"

    "cpp/iausb_test.cpp"
)

add_executable(TestSuite ${SRC_FILES})

target_link_libraries(TestSuite PRIVATE iausb)

if(MSVC)
    target_compile_options(TestSuite PRIVATE /EHsc)
else()
    target_compile_options(TestSuite PRIVATE -fexceptions)
endif()

add_test(NAME TestSuite COMMAND TestSuite)
