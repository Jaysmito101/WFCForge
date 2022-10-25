# This script provides helper functions

import os
import sys

def UpdateSubmodules():
    os.system("git submodule update --recursive --remote")

def InitSubmodules():
    os.system("git submodule update --init --recursive")

def GenerateProject():
    os.system("cmake -S ./ -B ./build")

def BuildProject():
    os.system("cmake --build ./build")

def PrintHelp():
    print("VoxoGen3D Helper Script\n")
    print("Usage:")
    print("\tpython Helper.py help      - Prints this help message")
    print("\tpython Helper.py initsm    - Initiates the submodules")
    print("\tpython Helper.py updatesm  - Updates the submodules")
    print("\tpython Helper.py generate  - Generate CMake project files")
    print("\tpython Helper.py build     - Build the project")

def Main():
    argc = len(sys.argv)
    if argc == 2:
        argv = sys.argv[1].lower()
        if argv == "help":
            PrintHelp()
        elif argv == "initsm":
            InitSubmodules()
        elif argv == "updatesm":
            UpdateSubmodules()
        elif argv == "generate":
            GenerateProject()
        elif argv == "build":
            BuildProject()
        else:
            print("Unknown command : " + argv + "\n")
            PrintHelp()
    else:
        PrintHelp()    

if __name__ == "__main__":
    Main()
    