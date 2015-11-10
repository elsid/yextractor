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
    add_custom_target(check COMMAND yextractor_test)
else()
    add_custom_target(run_yextractor_test COMMAND yextractor_test)
    set_property(TARGET run_yextractor_test PROPERTY EXCLUDE_FROM_ALL TRUE)
    add_dependencies(check run_yextractor_test)
endif()
