#!/usr/bin/env python3
import sbuildr

import glob
import os

cppstdlib = sbuildr.Library("stdc++")
project = sbuildr.Project()

project.interfaces(filter(os.path.isfile, glob.glob(os.path.join("include", "**", "*"), recursive=True)))

for source in glob.iglob(os.path.join("test", "*.cpp"), recursive=True):
    project.test(os.path.splitext(os.path.basename(source))[0], sources=[source], libs=[cppstdlib])

project.export()
