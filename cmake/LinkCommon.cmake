include(FetchContent)

macro(link_chk_common TARGET ACCESS TAG)
    FetchContent_Declare(
        chk_common
        GIT_REPOSITORY https://github.com/chikinrasshu/chk_common
        GIT_TAG ${TAG}
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(chk_common)

    target_include_directories(${TARGET} ${ACCESS} ${chk_common_SOURCE_DIR}/include)
    target_link_libraries(${TARGET} ${ACCESS} chk_common)

    add_dependencies(${TARGET} chk_common)
endmacro()