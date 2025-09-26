add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")
set_languages("c++23")

add_includedirs("../")

target("chapter4")
    set_kind("binary")
    add_files("main.cpp")
    add_deps("chapter4_lib")
    set_targetdir("$(builddir)")
    set_rundir("$(projectdir)")

target("chapter4_lib")
    set_kind("static")
    add_files("*.cpp")
    add_headerfiles("*.h", "*.hpp")
    add_includedirs("../")
    set_targetdir("$(builddir)/lib")
