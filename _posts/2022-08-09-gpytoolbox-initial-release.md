---
author: Oded Stein
title: Initial release of Gpytoolbox - A Python Geometry Processing Toolbox
category: Code
tags: python library opensource gpytoolbox
layout: blogpost
published: true
---


Silvia Sellán and I are creating a geometry processing library in Python
called [Gpytoolbox](https://gpytoolbox.org).
Gpytoolbox is a general-purpose geometry processing library written for direct
use in Python with numpy and scipy datatypes.
Use it for your next geometry processing project!

<!-- more -->


## Silvia and I are releasing a geometry processing library!

[Gpytoolbox](https://gpytoolbox.org) is a new general-purpose geometry
processing library in Python.
If you write geometry applications in Python with
[numpy](https://numpy.org) and [scipy](https://scipy.org), there are a lot of
everyday tasks you do not want to reimplement by yourself.
In our new Python library, Gpytoolbox, Silvia Sellán and I aim to provide
accessible and reliable tools for you to quickly implement your Python geometry
applications.

The functions in Gpytoolbox are written in vectorized Python with numpy where
possible, and call into custom C++ binding where required for performance
reasons.
You, the user, never have to use any C++.
All functions are useable from their Python interface exclusively, and behave
like normal Python functions -- just like in numpy.
Every function comes with a detailed documentation.

Gpytoolbox is licensed with the MIT license.
Certain functions in Gpytoolbox depend on GPL-licensed code.
These functions are segregated into a separate module, `gpytoolbox.copyleft`,
and the MIT-licensed rest of the library can be used without importing this
module.

There are a few people to acknowledge for our library.
- Silvia and I are both avid users of
[gptoolbox](https://github.com/alecjacobson/gptoolbox)
in MATLAB, Alec Jacobson's geometry processing library.
The structure of Gpytoolbox is based on gptoolbox -- we want to provide a
Python geometry processing library that's as convenient and all-encompassing as
gptoolbox.
- [NumPy](https://numpy.org) is a library that makes high-performance linear
algebra operations available to people via a simple Python interface.
We want to provide high-performance geometry processing functionality with
a very simple interface to make geometry processing as accessible as NumPy has
made linear algebra.
Furthermore, NumPy provides an excellent documentation.
We strive towards having a documentation as good as NumPy's.
- Nicholas Sharp of [Polyscope](https://polyscope.run/py/) was instrumental in
helping us with the infrastructure side of creating and releasing a Python
library.

Gpytoolbox is available through Python pip, and you can install it like this:
```
python -m pip install gpytoolbox
```

Import it and start using it!
```python
import gpytoolbox as gpy

V,F = gpy.read_mesh('mesh.obj')
area = 0.5*sum(gpy.doublearea(V,F))
print(f"My mesh has total area {area}.")
```

The documentation can be found on [our website](https://gpytoolbox.org), in the
left sidebar.

