# solar
Solar system visualization implemented in C99.

## Dependencies
### Common
* [glfw3](https://github.com/glfw/glfw)
* [OpenGL](https://opengl.org/)
### System-specific
#### Linux
* build-essential or equivalent (GCC, Make)
* GLEW
#### macOS
* Xcode Command Line Tools
* Homebrew (recommended for GLFW installation: `brew install glfw`)
> [!NOTE]
> macOS natively supports OpenGL up to version 4.1
#### Windows
* Microsoft Visual Studio (2019 or newer recommended) with C development tools
* vcpkg (recommended for GLFW installation: `vcpkg install glfw3`)
* GLEW

## Compiling
#### Linux/macOS
```
git clone https://github.com/filipswiszcz/solar.git
cd solar
make build
```
#### Windows
```
vcpkg integrate install
```
> [!NOTE]
> Above command is a one-time setup. It integrates vcpkg with Visual Studio to automatically use installed libraries.
```
git clone https://github.com/filipswiszcz/solar.git
Open project in `Microsoft Visual Studio` and build
```