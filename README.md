# ZJU CG 21fall Final Project
Final Project of ZJU Computer Graphic (by Wu Hongzhi)

Relation libs: glfw3 + glad + glm

based on C++

You need to configuration the OpenGL environment by yourself.
##### For linux
```
cd src
./run.sh
```

##### For windows
The Makefile has already prepared for user, but you need to change the file path due to difference between windows and linux.(e.g. delete "../" of all GLSL file)
```
make run
```

##### Shader的GLSL文件可能会存在编码问题，如果不能运行需要新建一个文件重新copy一下代码。


