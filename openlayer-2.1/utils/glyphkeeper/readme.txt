
           _____ _             _       _  __
          / ____| |           | |     | |/ /
         | |  __| |_   _ _ __ | |__   | ' / ___  ___ _ __   ___ _ __ 
         | | |_ | | | | | '_ \| '_ \  |  < / _ \/ _ \ '_ \ / _ \ '__|
         | |__| | | |_| | |_) | | | | | . \  __/  __/ |_) |  __/ |
          \_____|_|\__, | .__/|_| |_| |_|\_\___|\___| .__/ \___|_|
                    __/ | |                         | |
                   |___/|_|                         |_|      0.32


 Introduction
 ------------

Glyph Keeper is a text rendering library written in C. In can enable your
program to load a font from file, render character glyphs, and output them
to the target surface.

Glyph Keeper should work on any platform which has C compiler, FreeType,
and your target library (Allegro or SDL). See installation instructions in
"install.txt" file.

More information is available on Glyph Keeper web-site:
  http://kd.lab.nig.ac.jp/glyph-keeper/



 Files, included in this package:
 --------------------------------

behcnmark/   - Benchmark programs for testing Glyph Keeper performance.

docs/*       - Documentation.

examples/    - Glyph Keeper Examples.

fonts/       - Example fonts.

freetype/    - FreeType source code.

include/glyph.h  - Glyph Keeper header file,
                you may want to copy it to your compiler 'include' directory.

obj/         - Empty directory, where object and lib files will be produced.

src/         - Glyph Keeper source. If you compile Glyph Keeper manually,
                you only need to compile "src/glyph.c", as it #includes all
                other files.

authors.txt  - List of people contributed to Glyph Keeper development.

changes.txt  - Glyph Keeper version history.

install.txt  - Installation instructions.

license.txt  - Glyph Keeper license.

*.bat        - Batch files for compiling and installing Glyph Keeper.

Makefile.*   - Makefiles for compiling and installing Glyph Keeper.

quotes.txt   - What people say about Glyph Keeper.

releng.txt   - How to prepare a new release of Glyph Keeper.

readme.txt   - This file.

to-do.txt    - List of first priority things to do.
