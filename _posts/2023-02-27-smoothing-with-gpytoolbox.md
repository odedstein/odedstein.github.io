---
author: Oded Stein
title: Smoothing with Gpytoolbox
category: Code
tags: python library opensource gpytoolbox
layout: blogpost
published: true
---


[Gpytoolbox](https://gpytoolbox.org) version 0.1.0 has just been released!
In this blog post I'm showing you how to use Gpytoolbox for a few basic
smoothing tasks you might want to do in everyday geometry processing.
Gpytoolbox is developed by me and Silvia Sellán to be an easy-to-use everyday
Python geometry processing library.

<!-- more -->


## Gpytoolbox v0.1.0

We just published [Gpytoolbox v0.1.0](https://gpytoolbox.org/0.1.0/)!
It is available to download on `pip`.
You can install it like this:
```
python -m pip install gpytoolbox
```

To celebrate the release, Silvia and I will be showing you how to do a few basic
geometry processing tasks with Gpytoolbox.
I will add the link to Silvia's post here when it is published.


## Smoothing with Gpytoolbox

One of the basic tasks in geometry processing is smoothing.
Today we will learn a few basic ways in which we can use the Laplacian operator
and Gpytoolbox in geometry processing.
This tutorial assumes some previous knowledge in computer graphics and numerical
mathematics.


### Loading and displaying a surface mesh

We will start with loading and displaying a simple mesh in gpytoolbox.
We are using Nick Sharp's [Polyscope](https://polyscope.run/py/) viewer to
display surfaces.
If you have not installed it yet, install it now:
```
python -m pip install polyscope
```

We will be working with the mesh of a falcon statue from a small mediterranean
island today ([copyright info](https://github.com/odedstein/meshes/blob/master/objects/falconstatue/falconstatue-info.md)).
Please download the falcon statue mesh from [here](/assets/blog/2023-02-27/falcon.obj)
and place it in the working directory.
Now we can start!

Gpytoolbox loads OBJ meshes with `read_mesh`, which returns two numpy arrays:
the vertex list `V` and the face list `F`.
Every row in `V` is a coordinate of a vertex in three-dimensional space,
and every row in `F` is a triangle consisting of three vertex indices into the
rows of `V`.[^1]
The triangle mesh is the collection of all triangles in `F`:
```python
import gpytoolbox as gpy

V,F = gpy.read_mesh('falcon.obj')
print(V.shape)
print(F.shape)
```
```
(25791, 3)
(51482, 3)
```
The falcon mesh has 25791 vertices and 51482 faces.

Let's display it!
We'll use Polyscope to display our mesh
(the relevant Polyscope doc is [here](https://polyscope.run/py/structures/surface_mesh/basics/)):
```python
import gpytoolbox as gpy
import polyscope as ps

V,F = gpy.read_mesh('falcon.obj')

ps.init()
ps.register_surface_mesh("falcon", V, F)
ps.show()
```
![The Maltese Falcon in Polyscope](/assets/blog/2023-02-27/falcon_load_display.png){: width="600"}


### Smoothing (fairing) a mesh

Now that we have loaded a mesh, let's try to smooth it via a simple procedure
called surface fairing.
We'll use a method like in
[this famous article](https://dl.acm.org/doi/10.1145/311535.311576).
In this method, the surface is smoothed by repeatedly solving a linear equation
that smoothes the surface a little bit each time.

We'll need two special geometric matrices for this, the *Laplacian* and the
*mass* matrix.
The Laplacian approximates the [Laplacian operator](https://en.wikipedia.org/wiki/Laplace_operator)
from calculus, the sum of the second derivatives in each dimension.
They can be constructed with the following two simple commands:
```python
import gpytoolbox as gpy

V,F = gpy.read_mesh('falcon.obj')
L = gpy.cotangent_laplacian(V,F)
M = gpy.massmatrix(V,F)
print(L.shape)
print(M.shape)
```
```
(25791, 25791)
(25791, 25791)
```
As we can see, both matrices have dimension `n x n`, where `n` is the number of
vertices in `V`.

The surface fairing algorithm works by repeatedly solving the following linear
equation, where `h` is a small number that quantifies how much we want to
smooth each step.
In matrix pseudocode, this is:
```
(M + h L) V1 = M V0
```

Using scipy sparse linear algebra, this is what that looks like in code:
```python
import gpytoolbox as gpy
import scipy as sp
import polyscope as ps

# Read our mesh
V,F = gpy.read_mesh('falcon.obj')

# Store initial V for display later
V0 = V

# Surface fairing
h = 2e-4
for i in range(3):
    L = gpy.cotangent_laplacian(V,F)
    M = gpy.massmatrix(V,F)
    V = sp.sparse.linalg.spsolve(M + h*L, M*V)

    ps.init()
    ps.register_surface_mesh("raw falcon", V0, F)
    ps.register_surface_mesh("smoothed falcon", V, F)
    ps.show()
```
![Falcon, before smoothing](/assets/blog/2023-02-27/falcon_smoothing_0.png){: width="600"}
![Falcon, after smoothing once](/assets/blog/2023-02-27/falcon_smoothing_1.png){: width="600"}
![Falcon, after smoothing twice](/assets/blog/2023-02-27/falcon_smoothing_2.png){: width="600"}
![Falcon, after smoothing thrice](/assets/blog/2023-02-27/falcon_smoothing_3.png){: width="600"}

That's how easy it is to smooth a mesh using the Laplacian!
If you want to smooth more, increase `h`.
If you want to smooth less, decrease `h`.


### Computing geodesic distances

A more complicated use for the Laplacian is to compute approximate geodesic
distances.
The geodesic distance are the shortest distance that somebody who walks on the
surface has to walk to get from point to point.

We wll be using the method described in 
[this article](https://dl.acm.org/doi/10.1145/2516971.2516977).
A slightly modified version of their algorithm is as follows.
Let `u0` be a function that is zero everywhere on the surface, except for the
spots from which we want to measure distance, where it is one.
Let `L` be the Laplacian matrix (as computed above), `M` the mass matrix,
`G` is the gradient matrix, and `a` a list of triangle areas.
They all approximate the corresponding operators from calculus.
In matrix pseudocode:
```
(M + h L) u1 = M u0
X = - normalize(Gu1)
L d = M G^T (a X)
```
On the last line, `L d = M G^T (a X)`, we have to make sure that the solution
`d` is zero at whatever point we are measuring the distance from.
The matrix `L` has a zero eigenvalue, and the equation will not be uniquely
solvable if we don't enforce this boundary condition.

We will use a different surface mesh for this, the fish
([copyright info](https://github.com/odedstein/meshes/blob/master/objects/fish/fish-info.md)).
Download the fish mesh from [here](/assets/blog/2023-02-27/fish.obj).

Using scipy sparse linear algebra, here is the implementation of the geodesics
in heat algorithm.
It is a straightforward implementation of the matrix equations above.
We enforce the boundary condition that the distance to the source
point `i` shall be zero with [`gpy.fixed_dof_solve`](https://gpytoolbox.org/0.1.0/fixed_dof_solve/):
```python
import gpytoolbox as gpy
import numpy as np
import scipy as sp
import polyscope as ps

# Read our mesh
V,F = gpy.read_mesh('fish.obj')

# Compute matrices
L = gpy.cotangent_laplacian(V,F)
M = gpy.massmatrix(V,F)
G = gpy.grad(V,F)
a = 0.5*gpy.doublearea(V,F)

# Select vertex to measure distance from and timestep size
t = 5e-2
i = 150
u0 = np.zeros(V.shape[0])
u0[i] = 1

# Execute algorithm
u1 = sp.sparse.linalg.spsolve(M + t*L, M*u0)
X = np.reshape(G*u1, (F.shape[0],3), order='F')
X /= - np.linalg.norm(X, axis=-1)[:,None]
Y = np.reshape(X*a[:,None], 3*F.shape[0], order='F')
d = gpy.fixed_dof_solve(L, M*G.transpose()*Y, k=np.array([i]), y=0.)

ps.init()
ps_mesh = ps.register_surface_mesh("fish", V, F)
ps_scalar = ps_mesh.add_scalar_quantity("d", d, enabled=True)
ps.show()
```

Here is the resulting image of the geodesic distance from our chosen point
to every other point on the fish surface.
![geodesic distance on a fish](/assets/blog/2023-02-27/fish_distance.png){: width="600"}


### Further reading

If you are interested in finding out a little bit more about the mathematical
background of the Laplacian, you might be interested in
[this SGP course of mine](https://odedstein.com/projects/sgp-2021-lap-bilap-course/).

[This excellent course](https://www.cs.cmu.edu/~kmcrane/Projects/DDG/)
by Keenan Crane also features an extensive introduction to the Laplacian in
geometry processing.


⠀[^1]: There is a small but important distinction between the OBJ file format
and Gpytoolbox:
OBJ uses the 1-indexing convention, where we count the entries of arrays
starting at 1.
Gpytoolbox uses Python's and numpy's 0-indexing convention, where we count the
entries of arrays starting at 0.
You do not need to worry about this when reading an OBJ file with `read_mesh`,
as the conversion is automatically done for you.
