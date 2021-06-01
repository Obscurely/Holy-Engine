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
    shutil.rmtree("build")
    os.mkdir("build")
    os.chdir(os.getcwd() + get_platform_file_separator() + "build")

    print(os.system("cmake .." + get_platform_file_separator()))
    print(os.system("cmake --build ."))

    os.rename("main", name)

    print(os.system(f".{get_platform_file_separator()}\"{name}\""))

    os.chdir(app_dir)


build()
