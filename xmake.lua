set_project("jSTL")
set_version("1.0.0")
set_languages("cxx17")
set_toolchains("clang")
add_rules("mode.debug", "mode.release")

target("jSTL")
    set_kind("shared")
    add_files("src/**.cpp")
    add_includedirs("include", {public = true})
    add_defines("JSTL_STD_VER=17", {public = true})
    if is_plat("linux") then
        add_syslinks("pthread")
    end

target("test_headers")
    set_kind("binary")
    add_files("test/test_headers.cpp")
    add_deps("jSTL")

target("test_thread")
    set_kind("binary")
    add_files("test/test_thread.cpp")
    add_deps("jSTL")

target("test_vector")
    set_kind("binary")
    add_files("test/test_vector.cpp")
    add_deps("jSTL")