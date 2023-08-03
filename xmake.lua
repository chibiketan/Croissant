-- xmake f -c -p mingw --cc=/mingw64/bin/clang.exe --cxx=/mingw64/bin/clang++.exe --ar=/mingw64/bin/llvm-ar.exe --ld=/mingw64/bin/clang++.exe --engine_static=n
-- besoin d'installer base-devel : pacman -S base-devel
-- xmake f -c -p mingw --cc=clang --cxx=clang++ --mode=debug
-- generate cmakefile
--  xmake project -k cmake

-- I looooooooove glfw...
-- xmake f -c -p mingw --toolchain=clang --ar=/mingw64/bin/llvm-ar.exe --mode=debug --mingw=C:/msys64/mingw64


-- MISSING : No generation for defines.hpp for Engine2
rule("shader.compile")
    set_extensions(".frag", ".vert")

    on_build_file(function(target, sourcefile, opt)
        import("core.project.depend")
        import("utils.progress") -- it only for v2.5.9, we need use print to show progress below v2.5.8

        -- replace .md with .html
        local targetfile = path.join(target:targetdir(),sourcefile .. ".spv")

        -- make sure build directory exists
        os.mkdir(path.directory(targetfile))



        -- only rebuild the file if its changed since last run
        depend.on_changed(function ()
            os.vrunv('glslangValidator', { '-V', '-o', targetfile, sourcefile })
            progress.show(opt.progress, "${color.build.object}compile shader %s", sourcefile)
        end, {files = sourcefile})
    end)
rule_end()

option("engine_static", { description = "Build engine as static lib", default = false })

add_rules("mode.coverage", "mode.debug", "mode.releasedbg", "mode.release")
set_allowedmodes("debug", "releasedbg", "release", "coverage")
set_defaultmode("debug")

add_requires("volk", {configs={header_only=true}})
add_requires("glfw =3.3.8", "glslang")
set_languages("cxx20")
set_warnings("allextra")

target("engine")
    if has_config("engine_static") then
        add_defines("CROISSANT_SHARED_LIB", { public = true })
        set_kind("shared")
    else
        set_kind("static")
    end

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
    add_files("vulkan/main.cpp")
    add_files("vulkan/Source/**.cpp")
    add_includedirs("vulkan/Include")
    add_deps("engine")
    add_packages("glfw", "volk", "glslang")
    add_rules("shader.compile")
    for _, filepath in pairs(os.files("vulkan/**.frag")) do
        -- compile shaders
        -- add_files(filepath, {rule = "nzsl.compile.shaders"})
        add_files(filepath)
    end
    for _, filepath in pairs(os.files("vulkan/**.vert")) do
        -- compile shaders
        -- add_files(filepath, {rule = "nzsl.compile.shaders"})
        add_files(filepath)
    end

target("get_config")
    set_kind("binary")
    add_files("Sources/GetConfig/Source/**.cpp")
    add_deps("engine")
    add_packages("volk")