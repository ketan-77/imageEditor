# 🎨 ImageEditor

**ImageEditor** is a modular Win32-based image editing application built in C++. It demonstrates a component-based architecture using COM (Component Object Model) aggregation to encapsulate core image processing features and user interaction logging.

The project is split into three parts:

---

## 🧩 Components Overview

1. **Outer Component (Core Features)**
   - Contains the primary logic for image filtering and processing.
   - Exposes core COM interfaces to clients and aggregates the inner component.

2. **Inner Component (Logging & Extension)**
   - Responsible for user activity logging and analytics.
   - Aggregated by the outer component.
   - Built as a DLL and registered via a `.reg` file.

3. **Client Application (UI and Interaction)**
   - Provides a graphical interface for the user to interact with the image editor.
   - Relies on both inner and outer components via COM.

---

## ⚙️ Build Instructions

> Ensure you are using the **x64 Native Tools Command Prompt for Visual Studio** and have **CMake** installed.

---

### ✅ Step 1: Build the Outer Component

```sh
cd imageProcessing_outerComponent
mkdir build && cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
copy ..\build\Release\imageProcessingOuterComponent.dll C:\Windows\System32\
regedit /s imageProcessingOuterComponent.reg

### ✅ Step 2: Build the Inner Component

cd imageProcessing_innerComponent
mkdir build && cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
copy ..\build\Release\imageProcessingInnerComponent.dll C:\Windows\System32\

### ✅ Step 3: Build the Client Application
cd ../../imageProcessing_client
mkdir build && cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
cd /Release 
Win32App.exe

### 🛠️ Dependencies
CMake ≥ 3.10

Visual Studio 2022

Win32 API, COM, GDI

