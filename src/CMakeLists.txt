llvm_map_components_to_libnames(llvm_libs bitwriter core ipo irreader instcombine instrumentation target linker analysis scalaropts support )
file (GLOB SOURCES
   *.cpp
)
add_executable(loopa ${SOURCES})

target_link_libraries(loopa ${SVF_LIB} ${llvm_libs})

target_link_libraries(loopa ${Z3_DIR}/bin/libz3.a )
set_target_properties( loopa PROPERTIES
                       RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

include_directories(.)

enable_testing()
add_custom_target(loop-analysis-tests)
add_subdirectory(test EXCLUDE_FROM_ALL)
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_custom_target(check
            COMMAND ${CMAKE_CTEST_COMMAND}
            DEPENDS loop-analysis-tests)
endif()