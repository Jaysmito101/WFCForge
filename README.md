<br/>
<p align="center">
    <img src="https://raw.githubusercontent.com/Jaysmito101/WFCForge/main/logo.png" border="0"></
</p>

<br/>
<p align="center">
  <img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/Jaysmito101/WFCForge?style=for-the-badge">
  <img alt="Lines of code" src="https://img.shields.io/tokei/lines/github/Jaysmito101/WFCForge?style=for-the-badge">
  <img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/w/Jaysmito101/WFCForge?style=for-the-badge">
    <br>
    <img alt="Maintenance" src="https://img.shields.io/maintenance/yes/2024?style=for-the-badge">
    <a href="https://patreon.com/jaysmito101"><img src="https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%3Fusername%3Djaysmito101%26type%3Dpledges&style=for-the-badge" alt="Support me on Patreon" /></a>
</p>
<br/>

# WFCForge

WFCForge is a procedural as well as a custom game art generator! This is based on [Wave Function Collapse](https://github.com/mxgmn/WaveFunctionCollapse).

## Tutorial : <https://youtu.be/SFqquYiNGzg>

## Screenshots

<img alt = "Screenshot" src="./Images/ss4.gif" width=1000 />
<img alt = "Screenshot" src="./Images/ss5.gif" width=1000 />
<img alt = "Screenshot" src="./Images/ss0.png" width=1000 />
<img alt = "Screenshot" src="./Images/ss1.png" width=1000 />
<img alt = "Screenshot" src="./Images/ss2.png" width=1000 />
<img alt = "Screenshot" src="./Images/ss3.png" width=1000 />

## Outputs

<img alt = "Output" src="./Images/rs1.png" width=1000 />
<img alt = "Output" src="./Images/rs0.png" width=1000 />
<img alt = "Output" src="./Images/rs2.png" width=1000 />

## Download: <https://github.com/Jaysmito101/WFCForge/releases/download/v0.1/WFCForge.Win64.zip>

![Views](https://dynamic-badges.maxalpha.repl.co/views?id=Jaysmito101.WFCForge&style=for-the-badge&color=blue)

## Building from source

### Linux

1. Install dependencies
   - Build chain: cmake, gcc (should be 13 to support C++20 format header)
   - Project deps: opengl (libgl1-mesa-dev)
   - GLFW deps: libxrandr-dev, libxinerama-dev, libxcursor-dev, libxi-dev
2. Run the [Helper.py](Helper.py) script as listed in the help output:
   1. Initialize & update the submodules:

      ```bash
      python Helper.py initsm && python Helper.py updatesm
      ```

   2. Generate the cmake files:

      ```bash
      python Helper.py generate
      ```

   3. Build the project:

      ```bash
      python Helper.py build
      ```
