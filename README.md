Compilation instructions

* Ubuntu (24.04)
```
sudo apt install build-essential
sudo apt install libglew-dev
sudo apt install libglfw3
sudo apt install libglfw3-dev
sudo apt install libfreetype6-dev
sudo apt install libcglm-dev
git clone https://github.com/Carterz5/Cnake.git

cd Cnake
make

```

* Windows

install MSYS2 https://www.msys2.org/
```
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-glew
pacman -S mingw-w64-ucrt-x86_64-glfw
pacman -S mingw-w64-ucrt-x86_64-cglm
pacman -S mingw-w64-ucrt-x86_64-freetype
pacman -S mingw-w64-ucrt-x86_64-zlib
pacman -S mingw-w64-ucrt-x86_64-harfbuzz
pacman -S mingw-w64-ucrt-x86_64-bzip2
pacman -S mingw-w64-ucrt-x86_64-graphite2
pacman -S make
pacman -S git

git clone https://github.com/Carterz5/Cnake.git

cd Cnake
create bin folder

make
```
