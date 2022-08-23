llvm_map_components_to_libnames(llvm_libs bitwriter core ipo irreader instcombine instrumentation target linker analysis scalaropts support )
function(add_unit_test)
    string(REPLACE ";" "-" test_name "${ARGV}")
    string(REPLACE ";" "/" test_path "${ARGV}")
    set(test_build_target "test-core-${test_name}")
    add_executable(${test_build_target} "${test_path}.cpp")
    target_link_libraries(${test_build_target}
            ${SVF_LIB} ${llvm_libs} ${Boost_LIBRARIES})

    add_dependencies(loop-analysis-tests ${test_build_target})

    add_test(NAME "core-${test_name}" COMMAND ${test_build_target})
endfunction()

add_unit_test("loopTest")