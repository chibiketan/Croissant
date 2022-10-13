-- xmake f -c -p mingw --mingw=D:/logiciels/msys64/mingw64/ --cc=clang --cxx=clang++
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


add_requires("volk", "glfw =3.3.5", "glslang")
set_languages("cxx20")
set_warnings("allextra")

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