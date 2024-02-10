# Template to get started with C++ projects
Currently, this template is considered for single binary projects. To add new DIY static/shared libraries appropriate changes to CMakeLists.txt are needed, although I will enrich the current project to include those features as well.

## Dependencies
The bare minimum requirements are:

* [Conan](https://github.com/conan-io/conan) >= 2.0.17
* [CMake](https://github.com/Kitware/CMake) >= 3.28.1

To install Conan, one can use [pip](https://github.com/pypa/pip), a Python package manager.
On Archlinux it is installed as:

```shell
sudo pacman -Sy python-pip
```

But the thing is, `pip` (at least on Arch/Manjaro), will refuse to install system-wide packages,
so one is either forced to use the system-wide package manager to manage Python packages e.g. `python-xxx`,
`xxx` is the package name, or create virtual a environment (virtualenv). virtualenv is always preferred.
To begin with virtualenv, first install its Python package:
 
```shell
sudo pacman -Sy python-virtualenv
```

Then, switch to this project directory, create a virtualenv named `venv`, and activate it:

```shell
python3 -m virtualenv venv
. ./venv/bin/activate
```

Hooray! You can use `pip` now. Let's finally install `Conan2`, and generate a default profile for it:

```shell
pip install --upgrade pip
pip install conan
conan profile detect
```

Now you're ready to install dependencies, generate presets for CMake, and finally configure and build the project.

## Configuration
First, we need to install dependencies, and generate a preset for `CMake`:

```shell
conan install . --output-folder=build --build=missing --profile=default
```

This command will place its output into the folder named `build` in the `cwd`, build missing dependencies,
and use the default `Conan` profile on your system.

When this is done, we're ready to configure the project:

```shell
cmake --preset conan-release
```

## Build
Why we've been doing all this for:

```shell
cmake --build ./build
```

To use multiple threads e.g. 24 in my case:

```shell
cmake --build ./build -- -j 24
```

Notice that anything after the `--` CMake will pass directly to the underlying builder e.g. make, Ninja, or whatever.

## ToDo
* [CMake] How to use custom generators e.g. Ninja
* [Conan] How to create and use custom profiles e.g. GCC_Release, Clang_Debug
* [Template] Add templates for the creation of static and shared libraries and their usage 
* [Template] Add templates for tests
* [Tools] Support for ccache
* [Tools] Support for distcc
* [Tools] Support for static analysis tools e.g. clang-tidy, cppcheck
* [Tools] Support for linkers other than the GNU ld e.g. mold
* [DevOps] CI/CD?

