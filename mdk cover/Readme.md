# mdk gcovr 使用

首先使用 mdk simulate，通过命令输出 gcoverage details，然后使用 gcocr 导出覆盖率测试报告。

## coverage 命令使用

| 语法                            | 描述                                                         | 备注 |
| ------------------------------- | ------------------------------------------------------------ | ---- |
| COVERAGE                        | 显示整个应用的信息                                           |      |
| COVERAGE \module [DETAILS]      | 显示选择的module信息，选项提供额外指令                       |      |
| COVERAGE \module\func [DETAILS] | 显示在module函数包含函数信息，选项提供额外的执行指令         |      |
| COVERAGE CLEAR                  | 清除所有测试覆盖的信息                                       |      |
| COVERAGE FLOW filespec          | 按照规顶打印程序运行信息，仅能模拟使用                       |      |
| COVERAGE GCOV * [DETAILS]       | 打印各个模块执行信息，生成 module.gcov 文件。选项提供条件指令信息 |      |
| COVERAGE GCOV module [DETAILS]  | 同上                                                         |      |
| COVERAGE LOAD path\filename     | 从二进制中加载代码覆盖信息，能将之前的测试片段与当前测试片段合并。值得注意的是应用 image 一定不能改变。 |      |
| COVERAGE MTB filename           |                                                              |      |
| COVERAGE SAVE path\filename     | 保存代码覆盖率信息的二进制文件（在写入前，该文件必须存在）。 |      |

## COVERAGE  GCOV 说明

在调试模式可以使用 mdk 命令窗口，可以输出 `coverage` 测试报告。

```shell
COVERAGE  GCOV module //generate the report for a single module

COVERAGE  GCOV *//generate the report for all modules

COVERAGE GCOV * DETAILS //generate the report for all modules with conditional instruction execution 
```

mdk 输出这些 modules.gcov 文件到 output folder 中，下面为样例 `test.gcov`，文件如下

```
     -:     0:Source:../main.c
     -:     0:Graph:
     -:     0:Runs:1
     -:     0:Programs:1
     -:     1:#include <stdio.h>
     -:     2:
     -:     3:int main (void)
     1:     4:{
     1:     5:  int i = 1;
     -:     6:
     1:     7:  while (i < 10) {
     9:     8:          if (i % 3 == 0)
     3:     9:                  printf ("%d is divisible by 3\n", i);
     9:    10:    if (i % 11 == 0)
######:    11:      printf ("%d is divisible by 11\n", i);
     9:    12:          i++;
     -:    13:    }
     1:    14:   return 0;
     -:    15:}
```

第一列表示是否执行与执行次数：数字表示执行次数，`-`表示不相关的代码部分，`######`表示没有执行；第二列表示源文件的行数；第三列表示实际代码。

使用命令 `COVERAGE GCOV * DETAILS` 将条件指令的覆盖率测试均导出来。

## 将覆盖信息转换成报告

根据 mdk help 介绍，转换成报告需要使用开源工具 `gcovr`，安装成功后，使用如下命令导成报告：

```
gcovr -v -gk --html --html-details -o report/coverage.html -s

-v：显示版本
-gk：使用 .gcov 数据分析，并且保留 .gcov(去掉 -k 表示删除)
--html-details：生成 html 覆盖报告文件
-o：打印文件名称
report/coverage.html：指定文件路径
-s：打印间段报告
```

