if(NOT TARGET googletest)
    include(ExternalProject)

    set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/external)

    if(NOT GOOGLETEST_REPOSITORY)
        set(GOOGLETEST_REPOSITORY https://github.com/google/googletest.git)
    endif()

    ExternalProject_Add(
        googletest
        GIT_REPOSITORY ${GOOGLETEST_REPOSITORY}
        TIMEOUT 1
        INSTALL_COMMAND ""
        UPDATE_COMMAND ""
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON
    )

    ExternalProject_Get_Property(googletest source_dir)
    include_directories(SYSTEM ${source_dir}/googletest/include)
    include_directories(SYSTEM ${source_dir}/googlemock/include)

    ExternalProject_Get_Property(googletest binary_dir)
    link_directories(${binary_dir}/googlemock)
endif()

add_executable(yextractor_test
    main.cpp
    flat.cpp
    extractor.cpp
    exclude.cpp
    parameter.cpp
    to_string.cpp
)

set(LIBRARIES
    -pthread
    gmock
)

target_link_libraries(yextractor_test ${LIBRARIES})
add_dependencies(yextractor_test googletest)

if(NOT TARGET check)
    add_custom_target(check)
endif()

add_test(yextractor_test yextractor_test)
add_dependencies(check yextractor_test)
