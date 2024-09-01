include(FetchContent)

macro(link_glad TARGET ACCESS TAG)
    FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad
        GIT_TAG ${TAG}
        GIT_SHALLOW TRUE
        SOURCE_SUBDIR cmake
    )

    FetchContent_MakeAvailable(glad)

    glad_add_library(glad REPRODUCIBLE QUIET API gl:core=4.1)

    target_include_directories(${TARGET} ${ACCESS} ${glad_SOURCE_DIR}/include)
    target_link_libraries(${TARGET} ${ACCESS} glad)

    # add_dependencies(${TARGET} glad)
endmacro()