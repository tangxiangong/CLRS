add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")
set_languages("c++23")

add_includedirs("src")

target("CLRS")
    set_kind("binary")
    add_files("src/main.cpp")
    add_headerfiles("src/utils.hpp", "src/error.hpp", "src/matrix.hpp")

includes("src/chapter2", "src/chapter4")
