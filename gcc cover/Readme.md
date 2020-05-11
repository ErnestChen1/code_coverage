# gcc cover 测试说明

使用象 gcov 或 gprof 这样的分析器，可以找到一些基本的性能统计数据： 
* 每一行代码执行的频率是多少 
* 实际执行了哪些行代码，配合测试用例达到满意的覆盖率和预期工作 
* 每段代码使用了多少计算时间，从而找到热点优化代码 
* gcov 创建一个 sourcefile.gcov 的日志文件，此文件标识源文件sourcefile.c每一行执行的次数,您可以与gprof一起使用这些日志文件来帮助优化程序的性能。gprof提供了您可以使用的时间信息以及从gcov获得的信息。

gvoc 工具主要做以下工作：

- 1) 编译前，在编译器中加入编译器参数-fprofile-arcs -ftest-coverage；
- 2) 源码经过编译预处理，然后编译成汇编文件，在生成汇编文件的同时完成插桩。插桩是在生成汇编文件的阶段完成的，因此插桩是汇编时候的插桩，每个桩点插入3~4条汇编语句，直接插入生成的*.s文件中，最后汇编文件汇编生成目标文件，生成可执行文件；并且生成关联BB和ARC的.gcno文件；
- 3) 执行可执行文件，在运行过程中之前插入桩点负责收集程序的执行信息。所谓桩点，其实就是一个变量，内存中的一个格子，对应的代码执行一次，则其值增加一次；
- 4) 生成.gcda文件，其中有BB和ARC的执行统计次数等，由此经过加工可得到覆盖率。

## 安装工具 	

### 安装 gcc 编译器套件
使用命令： `sudo apt install gcc`。(假如无法安装需要[更换国内源](https://blog.csdn.net/zyx_ly/article/details/89372558)）

#### -c 编译、汇编指定的源文件

命令格式：`$gcc -c test.c`

`-c`选项表示编译、汇编指定的源文件（也就是编译源文件），但是不进行链接。使用`-c`选项可以将每一个源文件编译成对应的目标文件。

如果不使用 `c` 选项，则仅仅生成一个可执行文件，没有目标文件。

#### -o

命令格式：`[infile] -o [outfile]`

[infile] 表示输入文件（也即要处理的文件），它可以是源文件，也可以是汇编文件或者是目标文件；[outfile] 表示输出文件（也即处理的结果），它可以是预处理文件、目标文件、可执行文件等。

[infile] 和 [outfile] 可以是一个文件，也可以是一组文件：

- 如果 [infile] 是一组文件，那么就表示有多个输入文件；
- 如果 [outfile] 是一组文件，那么就表示有多个输出文件。

如果不使用 -o 选项，那么将采用默认的输出文件，默认名称为 `a.out`。

```c
将源文件作为输入文件，将可执行文件作为输出文件，也即完整地编译整个程序：
$ gcc main.c func.c -o app.out
将 main.c 和 func.c 两个源文件编译成一个可执行文件，其名字为 app.out。如果不使用 -o 选项，那么将生成名字为 a.out 的可执行文件。
```

#### -E 保留预处理文件

命令格式：`$ gcc -E test.c`

选项**保留预处理器**的输出文件，以用于诊断代码。

```c
用 -o 输出到某个文件

$ gcc -E circle.c -o circle.i

表示把预处理的结果导出到 circle.i 文件。
```

#### -C 

命令格式：`$ gcc -E -C circle.c -o circle.c`

阻止预处理器删除源文件和头文件中的注释

#### -S 生成汇编文件

果没有指定输出文件名，那么采用`-S`选项的 GCC 编译过程会为每个被编译的输入文件生成以`.s`作为后缀的汇编语言文件。

```shell
$ gcc -S circle.c
编译器预处理 circle.c，将其翻译成汇编语言，并将结果存储在 circle.s 文件中。

如果想把C语言变量的名称作为汇编语言语句中的注释，可以加上-fverbose-asm选项：
$ gcc -S -fverbose-asm circle.c
```

#### -l 手动添加链接库

```shell
$ gcc main.c -o main.out -lm
数学库的文件名是 libm.a, 前缀lib和后缀.a是标准的，m是基本名称，GCC 会在-l选项后紧跟着的基本名称的基础上自动添加这些前缀、后缀，本例中，基本名称为 m。
在支持动态链接的系统上，GCC 自动使用在 Darwin 上的共享链接库 libm.so 或 libm.dylib。
```

### 安装 lcov（GCC 测试覆盖率的前端图形展示工具）

使用命令：`sudo apt install lcov`

- -d 项目路径，即.gcda .gcno所在的路径
- -a 合并（归并）多个lcov生成的info文件
- -c 捕获，也即收集代码运行后所产生的统计计数信息
- --external 捕获其它目录产生的统计计数文件
- -i/--initial 初始化所有的覆盖率信息，作为基准数据
- -o 生成处理后的文件
- -r/--remove 移除不需要关注的覆盖率信息文件
- -z 重置所有执行程序所产生的统计信息为0

## 编译

使用命令：`gcc -fprofile-arcs -ftest-coverage test.c -o test`

* -ftest-coverage：在编译的时候产生.gcno文件，它包含了重建基本块图和相应的块的源码的行号的信息。 
* -fprofile-arcs：在**运行编译过的程序**的时候，会产生.gcda文件，它包含了弧跳变的次数等运行信息。
* test.c： 表示输入文件
* test： 表示生成的文件

## 收集代码运行信息

运行生成的文件 `test`，使用命令`./test`，生成文件 `test.gcda`。

## 生成 gcov 代码覆盖报告

运行 test.c 文件，使用命令 `gcov test.c`，生成文件 `test.c.gcov`，即为 gcov 代码测试报告文件。

## 生成图形化报告

根据 test.c.gcov 文件，使用 lcov 工具生成图形化报告，命令为 `lcov -c -d . -o test.info`。

从 lcov 覆盖率测试数据中生成 HTML 可视化文件，使用命令 `genhtml -o report test.info`，其中 report 表示生成文件的目录。

## 参考

  [覆盖率测试参考文章](https://blog.csdn.net/yanxiangyfg/article/details/80989680?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1)