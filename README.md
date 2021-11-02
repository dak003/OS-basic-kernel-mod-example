# Generic Template
This repository contains some boilerplate code so you can develop your project
using Visual Studio Code or CLion if you so wish. This will allow you to debug
your software. You can also use the same boilerplate through SSH remote, like
you had to do on GL for earlier classes I believe. But this configuration is up
to you if you want it.

Finally, you can use it without any environment and a normal text editor (i.e.
emacs) if you so choose.

You can read a more in-depth explanation at [the course
website](https://www.csee.umbc.edu/courses/undergraduate/421/spring21/docs/vm/using-the-template.html).

# Usage
This boilerplate will work with either Make or CMake. The later is a hard
requirement for CLion to work so I made this template work with both so you can
move between IDEs if you need to. You can chose whichever you want if you don't
use CLion.

If you decide to use CMake you will add files in `CMakeLists.txt`, otherwise you
need to modify `Makefile`

### No editor
---
You can just create your .c and .h files as you normally would and just add them
to the Makefile. There are instructions in the comments inside the Makefile if
you do not know how to add a new .c file to the compilation. You can then just
compile with `make`.

### Visual Studio Code
---
You will have to install some extensions in visual studio code to make full use
of this biolerplate. You can search them by name:

* C/C++ (This is the standard extension by Microsoft)
* EditorConfig for VS Code (Made by EditorConfig)
* CMake (Made by twxs, if you want to use CMake)
* CMake Tools (Made by Microsoft again, if you want to use CMake)

If you open the debug tab in vscode you will see two configurations:
* (gdb) With Make
* (gdb) With CMake

You just have to select the one you wish to use then you can start debugging
by pressing F7. The rest works as any other graphical debugger

### CLion
---
For CLion you do not need to install anything other than CMake. We had you
install CMake when setting up your vm but if you don't have it you can install
it with:

`sudo apt install cmake`

Next you just go to  `File > Open` and select either the folder you cloned or
the `CMakeLists.txt` inside it. From then on you can use CLion as you normaly
would.

## Extra goodies
This repository also contains so formating rules so you can practice with the
way you write code in the kernel. During setup we had you install clang-format.
So by running the following command it will format all your code according to
the kernel's standards:

`clang-format -i ./src/*.{c,h}`

You will notice that the code is now wrapped at 80 characters. It is recommended
to not exceed that number. If you find your code frequently exceeding that, it
is a very clear indication that your code needs to be re-thought, and it may be
extremely nested.
