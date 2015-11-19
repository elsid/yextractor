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
    -lgtest
    -lgmock
)

target_link_libraries(yextractor_test ${LIBRARIES})

if(NOT TARGET check)
    add_custom_target(check)
endif()

add_test(yextractor_test yextractor_test)
add_dependencies(check yextractor_test)
