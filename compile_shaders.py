"""
Cross-platform shader compile script. Requires OpenGL to be in PATH (for glslc).
"""
import os
import platform

app_dir = os.getcwd()


def get_platform_file_separator():
    if platform.system() == "Windows":
        return "\\"
    else:
        return "/"


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


compile_shaders()
