"""
Cross-platform build script for Holy-Engine. Requires CMake and Vulkan Tools to be in PATH.
"""
import os
import platform
import shutil

name = "Holy Engine"
app_dir = os.getcwd()


def get_platform_file_separator():
    if platform.system() == "Windows":
        return "\\"
    else:
        return "/"


def build():
    try:
        shutil.rmtree("build")
    except:
        pass
    os.mkdir("build")
    os.chdir(os.getcwd() + get_platform_file_separator() + "build")

    print(os.system("cmake .." + get_platform_file_separator()))
    print(os.system("cmake --build ."))

    os.rename("main", name)

    print(os.system(f".{get_platform_file_separator()}\"{name}\""))

    os.chdir(app_dir)


def compile_shader(shader_name):
    print(os.system(f"glslc {shader_name} -o {shader_name}.spv"))


def compile_shaders():
    os.chdir(
        f"{app_dir}{get_platform_file_separator()}src{get_platform_file_separator()}shaders")

    dir_files = os.listdir()
    for file in dir_files:
        try:
            file.split("spv")[1]
            continue
        except:
            compile_shader(file)

    os.chdir(app_dir)


compile_shaders()
build()
