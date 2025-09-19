add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")
set_languages("c++23")

add_includedirs("../")

target("chapter2")
    set_kind("binary")
    add_files("main.cpp")
    add_deps("chapter2_lib")
    set_targetdir("$(builddir)")
    set_rundir("$(projectdir)")

target("chapter2_lib")
    set_kind("static")
    add_files("*.cpp")
    add_headerfiles("*.h", "*.hpp")
    add_includedirs("../")
    set_targetdir("$(builddir)/lib")
