# DX11-Practice

## 開發環境
* Visual Studio 2019

## Third party
* ImGui v1.78 - https://github.com/ocornut/imgui
* Open Asset Import Library (assimp) v4.1.0 - https://github.com/assimp/assimp/releases/tag/v4.1.0

## Issues
* 2020/09/05 - model transform 的 position xy 正負方向相反?
    * Fixed: 因為 project matrix 中沒有把fov轉換為弧度量