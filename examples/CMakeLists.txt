add_executable(example
    main.cpp
)

add_custom_target(examples
    COMMAND example
)
