
add_requires("volk", "glfw =3.3.5")
set_languages("cxx20")

target("engine")
    set_kind("static")
    add_files("Sources/Engine2/Source/**.cpp")
    add_includedirs("Sources/Engine2/Include", {public = true})
    if(is_plat("mingw", "windows")) then
        add_defines("ENGINE_EXPORTS")
        remove_files("Sources/Engine2/Source/**.Linux.cpp")
        remove_files("Sources/Engine2/Source/**.uni.cpp")
        add_syslinks("opengl32", "glu32", "gdi32")
    end
    if(is_plat("linux")) then
        remove_files("Sources/Engine2/Source/**.Windows.cpp")
        remove_files("Sources/Engine2/Source/**.win.cpp")
        remove_files("Sources/Engine2/Source/**.Linux.cpp")
        remove_files("Sources/Engine2/Source/**.uni.cpp")
    end
    add_packages("volk")


target("vulkan_test")
    set_kind("binary")
    add_files("vulkan/**.cpp")
    add_deps("engine")
    add_packages("glfw", "volk")

target("get_config")
    set_kind("binary")
    add_files("Sources/GetConfig/Source/**.cpp")
    add_deps("engine")
    add_packages("volk")