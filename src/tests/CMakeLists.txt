find_package(Boost REQUIRED)

add_executable(test
    main.cpp
    flat.cpp
    extractor.cpp)

set(LIBRARIES
    -pthread
    -lgtest
    -lgmock)

target_link_libraries(test ${LIBRARIES})

add_custom_target(check
    COMMAND test
)
