Shader Based Image Manipulation with the Annotated Shading Language (ASL)
=========================================================================

This project was done as part of the Computer Graphics module by Prof. Dr. Mario
Botsch at the Bielefeld University in the summer term of 2011. It is an image
manipulation program which implements its algorithms in shader programs running
on the GPU.

We aimed to make this image manipulation program flexible and extensible.  
Therefore, we decided to save the source code of the shaders in their own files 
which will be loaded and compiled at runtime. This makes it easy to add new 
algorithms: just write a shader and you can load it from the program.

However, it would be nice if one could control the various parameters of the 
algorithms with GUI controls. But a normal shader program does not provide the 
information what the purpose and valid range of a parameter is. This is the 
point were the Annotated Shading Language (ASL) comes into play.

The Annotated Shading Language allows to add information like description and
parameter range of a parameter to the program. Because all ASL elements are
written within comments, the shader program can still be compiled without using
an ASL shader compiler.

Because ASL was developed only as part of a rather small project, there is still 
room for many enhancements. For example, at the moment it is only possible to 
annotate uniforms and whole files, but not per vertex parameters. We had no need 
for this function as we only used fragment shaders which do not have per vertex 
parameters.

If you want to try out ASL for yourself, take a look into the `doc` folder where
you will find a documentation of ASL.

Requirements
------------

- [Qt](http://qt.nokia.com/) 4.6 (or newer)
- [OpenGL](http://www.opengl.org/) 2.0 (or newer)
- [flex](http://flex.sourceforge.net/)
- [Bison](http://www.gnu.org/s/bison/)

The program should be platform independent. However, the final version was only
tested with Mac OS X 10.6 (Snow Leopard) at the time of writing this.

Known Problems
--------------

- The program will not work with Mac OS X 10.7 (Lion). Qt messes up the
  initialization of the OpenGL context on that platform. Presumably because Lion
  introduced OpenGL 3. The problem may be fixed with Qt versions > 4.7. However,
  it still persisted in the beta version of Qt 4.8.

Authors
-------

- [Jan Gosmann](http://www.hyper-world.de)
- Denis John
- Markus Kastrop

The project has been supervised by Matthias Heinrich and Prof. Dr. Mario Botsch.

