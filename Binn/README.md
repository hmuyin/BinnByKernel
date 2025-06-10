这是一个内核版本的 Binn
原项目：https://github.com/liteserver/binn

基于该项目的代码进行了修改，主要是为了适配内核版本。

修改内容包括：
1. 移除了与用户空间相关的头文件。
2. 修改了内存分配函数，使用内核的 ExAllocatePool2 和 ExFreePool。
3. 手动实现了内核下用的 realloc ==> ReallocateMemory
4. 增加内核下需要用到的 _atoi64，atoi，atof 以及 strdup ==> Rt_strdup，stricmp ==> Rt_stricmp，strnicmp ==> Rt_strnicmp 以及替换源文件中的对应函数
5. 增加源文件中所需要用到的一些定义

其他：
所有增加的函数都在 inplace.c 中实现。
其他使用与用户空间的代码保持一致。

具体使用方法请参考原项目的 README.md。
