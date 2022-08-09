#!/usr/bin/env python
# -*- coding: utf-8 -*-
from subprocess import run

CC = ["gcc"]
flags = ["-g", "-lSDL2main", "-lSDL2", "-lSDL2_ttf", "-Wall"]
files = ["./src/basic-collision.c",
         "./src/core.c",
         "./src/draw.c",
         "./src/game.c",
         "./src/text.c",
         "./src/audio.c"
         ]
include = ["./include"]

output = ["./build/test"]

run(["echo", "Compiling"])
text = run(CC + ["-o"] + output + ["-I"] + include + files + flags)
run(["echo", str(text)])
if text.returncode != 0:
  run(["echo", "Compilation failed"])
  exit(text.returncode)

run(["echo", "Debugging"])
run(["gdb" ,"./test","--quiet", "--eval-command=run", "--batch"], cwd="./build")

#run(["echo", "Running Build"])
#run(["./test"], cwd="./build")
