# XOS

## 环境配置

## 链接脚本

### 简单介绍

#### 链接配置

一些符号变量的定义、入口地址、输出格式等

```c
STACK_SIZE = 0X200;
OUTPUT_FORMAT(elf32-littlearm)
OUTPUT_ARCH(arm)
ENTRY(_start)
```

#### 内存布局定义

脚本中以MEMORY命令定义了存储空间，其中以ORIGIN定义地址空间的起始地址，LENGTH定义地址空间的长度。

```undefined
MEMORY
{
		FLASH (rx) : ORIGIN = 0, LENGTH = 64K
}
```

#### 段链接定义

脚本中以SECTIONS命令定义一些段（text、data、bss等段）链接分布。

```css
SECTIONS
{
    .text :
    {
      *(.text*)
    } > FLASH
}
```

.text段即代码段，* (.text*)指示将工程中所有目标文件的.text段链接到FLASH中

#### SECTION命令

```csharp
SECTIONS
{
       ...
      secname start BLOCK(align) (NOLOAD) : AT ( ldadr )
      { 
        contents 
      } >region :phdr =fill
      ...
}
```

这么多参数中，只有secname 和 contents 是必须的，即可简写成：

```undefined
SECTIONS
{
       ...
      secname :
      { 
        contents 
      } 
      ...
}
```

链接脚本本质就是描述输入和输出。secname表示输出文件的段，即输出文件中有哪些段。而contents就是描述输出文件的这个段从哪些文件里抽取而来，即输入文件，一般就是目标文件之类的。（段名自己定义,段名前后必须要有空格）

如下，将目标文件的数据段存放在输出文件的数据段（段名自己定义,段名前后必须要有空格）

```kotlin
SECTIONS
{
       ...
      .data :
      { 
        main.o（.data）
        *(.data)
      } 
      ...
}
```

其中 *(.data) 表示将所有的目标的.data段链接到输出文件.datad段中， 特别注意的是，之前链接的就不会再链接，这样做的目的是可以将某些特殊的目标文件链接到地址前面。

**其他参数**：

- start 

  表示将某个段强制链接到的地址。

- AT(ADDR)

  实现存放地址和加载地址不一致的功能，AT表示在文件中存放的位置，而在内存里呢，按照普通方式存储。

- region

  这个region就是前面说的MEMORY命令定义的位置信息。PROVIDE关键字

#### PROVIDE关键字

该关键字定义一个（目标文件内被引用但没定义）符号。相当于定义一个全局变量，其他C文件可以引用它。

```undefined
SECTIONS
{
    .text :
    {
        *(.text)
        _etext = .;
        PROVIDE(etext = .);
    }
}
```

如上，目标文件可以引用etext符号，其地址为定义为.text section之后的第一个字节的地址。C文件中引用。

```cpp
int main（）
{
    //引用该变量
    extern char  _etext;
    //...
}
```

#### KEEP关键字

在连接命令行内使用了选项–gc-sections后，连接器可能将某些它认为没用的section过滤掉，此时就有必要强制连接器保留一些特定的 section，可用KEEP()关键字达此目的。如KEEP(* (.text))或KEEP(SORT(*)(.text))

> 链接脚本学习自博客
>
> 作者：小王子_f27a
> 链接：https://www.jianshu.com/p/42823b3b7c8e
> 来源：简书
> 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

### 更全面的介绍

https://zhuanlan.zhihu.com/p/383729996

#### 设置入口点

程序执行的第一条指令称为入口点。你可以使用' ENTRY '链接脚本命令来设置入口点。参数是一个符号名:

```text
ENTRY ( symbol )
```

有几种方法可以设置入口点。链接器将依次尝试以下方法来设置入口点，并在其中一个成功时停止:

- -e 入口命令选项（**译注**：-e是链接器的参数）；
- 链接脚本中的ENTRY (symbol) 命令 ；
- 如果符号start定义了，那就是start符号的值；
- 如果存在.text节，那就是.text节的第一个字节的地址；
- 地址0

#### 输出节描述（Output section description）

````c
SECTION [ address ] [( type )] : [AT( LMA )]
  {
       output-sections-command
       output-sections-command
...
} [> region ] [: phdr : phdr ...] [=fillexp]
````

多数输出节不使用大部分可选的节属性。“SECTION”周围的空格是必需的，这样SECTION名是明确的。冒号和花括号也是必需的。换行符和其他空白是可选的。

每个output-sections-command 命令可以是如下：

- 一个符号赋值 (see [Simple assignments](https://link.zhihu.com/?target=https%3A//users.informatik.haw-hamburg.de/~krabat/FH-Labor/gnupro/5_GNUPro_Utilities/c_Using_LD/ldLinker_scripts.html%23Simple_assignments))
- 一个输入节描述(see [Input section description](https://link.zhihu.com/?target=https%3A//users.informatik.haw-hamburg.de/~krabat/FH-Labor/gnupro/5_GNUPro_Utilities/c_Using_LD/ldLinker_scripts.html%23Input_section_description))
- 要直接包含的数据值(see [Output section data](https://link.zhihu.com/?target=https%3A//users.informatik.haw-hamburg.de/~krabat/FH-Labor/gnupro/5_GNUPro_Utilities/c_Using_LD/ldLinker_scripts.html%23Output_section_data))
- 一个特殊的输出部分关键字(see [Output section keywords](https://link.zhihu.com/?target=https%3A//users.informatik.haw-hamburg.de/~krabat/FH-Labor/gnupro/5_GNUPro_Utilities/c_Using_LD/ldLinker_scripts.html%23Output_section_keywords))

#### 输出节名称（Output section name）

```c
SECTION [ address ] [( type )] : [AT( LMA )]
  {
       output-sections-command
       output-sections-command
...
} [> region ] [: phdr : phdr ...] [=fillexp]
```

输出节的名称是' section '。' section '必须满足输出格式的约束。在只支持有限数量的节的输出文件格式中，例如' a.out '，名称必须是该格式支持的名称之一(例如' a.out '，只允许' .text '， ' .data '或' .bss ')。如果输出格式支持任意数量的节，但是使用数字而不是名称(就像Oasys的情况一样)，则名称应该作为引用的数字字符串提供。section名称可以由任何字符序列组成，但是包含逗号等任何不寻常字符的名称必须用引号括起来。输出节名' /DISCARD/ '是特殊的 See [Output section discarding](https://link.zhihu.com/?target=https%3A//users.informatik.haw-hamburg.de/~krabat/FH-Labor/gnupro/5_GNUPro_Utilities/c_Using_LD/ldLinker_scripts.html%23Output_section_discarding).

#### 输出节地址（Output seciton address）

```c
SECTION [ address ] [( type )] : [AT( LMA )]
  {
       output-sections-command
       output-sections-command
...
} [> region ] [: phdr : phdr ...] [=fillexp]
```

“address”是输出节的VMA(虚拟内存地址)的表达式。如果你没有提供' address '，链接器将基于' region '(如果存在)设置它，或者以其他方式基于位置计数器的当前值。

如果你提供了' address '，输出节的地址将被精确地设置为该规范。如果你既不提供' address '也不提供' region '，那么输出节的地址将被设置为符合输出节的对齐要求的位置计数器的当前值。

输出节的对齐要求是输出节中包含的所有输入节中最严格的对齐方式。（**译注**：比如，一个输出节包括2个输入节，一个输入节是2字节对齐，一个输入节是4字节对齐，那么这个输出节就按照最严格的4字节对齐）

例如：

```text
.text . : { *(.text) }
```

和

```text
.text : { *(.text) }
```

有微妙的不同。第一个将把' .text '输出节的地址设置为位置计数器的当前值。第二个将其设置为位置计数器以' .text '输入节的最严格对齐方式对齐后的当前值。

“address”可以是任意的表达式。See [Expressions in linker scripts](https://link.zhihu.com/?target=https%3A//users.informatik.haw-hamburg.de/~krabat/FH-Labor/gnupro/5_GNUPro_Utilities/c_Using_LD/ldLinker_scripts.html%23Expressions_in%20_linker_scripts). 例如，如果你想在0x10字节的边界上对齐section，以便section地址的最低4位为0，你可以做如下声明:

```text
.text ALIGN(0x10) : { *(.text) }
```

这是因为' ALIGN '返回向上对齐到指定值的当前位置计数器。

为一个节指定' address '将改变位置计数器的值。

#### 输入节描述 Input section description

最常见的输出节命令是输入节描述，输入节描述是最基本的链接脚本操作。使用输出节告诉链接如何在内存中布局程序。使用输入节描述来告诉链接器如何将输入文件映射到内存布局中。

#### 输入节基础 Input section basics

输入节描述由一个文件名(可选)和一个用圆括号括起来的节名称列表组成。文件名和节名可以是通配符模式，这是我们要介绍的；see [Input section wildcard patterns](https://link.zhihu.com/?target=https%3A//users.informatik.haw-hamburg.de/~krabat/FH-Labor/gnupro/5_GNUPro_Utilities/c_Using_LD/ldLinker_scripts.html%23Input_section_wildcard_patterns). 最常见的输入节描述是在输出节中包含具有特定名称的所有输入节。例如，要包含所有输入' .text '部分，你可以这样写:

```text
*(.text)
```

这里的' * '是一个通配符，可以匹配任何文件名。

有两种方法可以包含多个节:

```text
*(.text .rdata)
*(.text) *(.rdata)
```

它们之间的区别在于' .text '和' .rdata '输入节出现在输出节的顺序。在第一个例子中，它们将混合在一起。在第二个例子中，所有的' .text '输入部分将出现在前面，然后是所有的' .rdata '输入部分。

你可以指定文件名来包含来自特定文件的节。如果一个或多个文件包含需要位于内存中特定位置的特殊数据，就可以这样做。例如:

```text
data.o(.data)
```

如果你使用的文件名没有包含一个节列表，那么输入文件中的所有节都将包含在输出节中。这种做法并不常见，但有时可能是有用的。例如:

```text
data.o
```

当你使用不包含任何通配符的文件名时，链接器将首先查看你是在链接命令行上还是在' INPUT '命令中指定了文件名。如果没有，链接器将尝试将该文件作为输入文件打开，就像它出现在命令行上一样。注意，这与' INPUT '命令不同，因为链接器不会在存档搜索路径中搜索文件。

#### 输入节通配符模式 Input section wildcard patterns

在输入区段描述中，文件名或节名或两者都可以是通配符模式。在许多示例中看到的 ' * ' 文件名是一个简单的文件名通配符模式。通配符模式类似于Unix shell使用的那些模式。

**' * '**

匹配任意数量的字符。

文件名通配符模式只匹配在命令行或' INPUT '命令中显式指定的文件。链接器不搜索目录以展开通配符。

如果一个文件名匹配多个通配符模式，或者如果一个文件名显式出现并且也由通配符模式匹配，链接器将使用链接脚本中的**第1个匹配**。例如，输入部分描述的序列可能是错误的，因为' data.o '规则将不会被使用（**译注**：也就是第2个规则被第1个短路了）:

```text
.data : { *(.data) }
.data1 : { data.o(.data) }
```

这个例子展示了如何使用通配符模式对文件进行分区。这个链接脚本指示链接器将所有的' .text '节放在'.text '中，将所有输入的'. bss '节放到输出的'. bss '节。链接器会将所有以大写字母开头的文件中的' .data '部分放在' .DATA '中;对于所有其他文件，链接器将在'. data '放置在'. data '节。

```text
SECTIONS {
  .text : { *(.text) }
  .DATA : { [A-Z]*(.data) }
  .data : { *(.data) }
  .bss : { *(.bss) }
}
```

#### 输入节示例 Input section example

下面的示例是一个完整的链接脚本。它告诉链接器从文件' all.o 中读取所有的节，并将它们放在输出节' outputa '的开头，outputa从位置' 0x10000 '开始。foo.o中的所有的 ' .input1 '节紧随其后，在相同的输出部分中。foo.o中的所有.input2节放入到输出节outputb中，接着就是foo1.o的.input1节。剩下的所有来自任意文件的.input1和input2节被输出到输出节outputc中。

```c
SECTIONS {
    outputa 0x10000 :
    {
        all.o
        foo.o (.input1)
    }
    outputb :
    {
        foo.o (.input2)
        foo1.o (.input1)
    }
    outputc :
    {
        *(.input1)
        *(.input2)
    }
}
```

#### 输出节数据（Output section data）

```c
SECTION [ address ] [( type )] : [AT( LMA )]
  {
       output-sections-command
       output-sections-command
...
} [> region ] [: phdr : phdr ...] [=fillexp]
```

你可以使用' BYTE '， ' SHORT '， ' LONG '， ' QUAD '或' SQUAD '作为输出节命令，在输出节中包含显式的若干字节的数据。每个关键字后面都有一个括号中的表达式，提供要存储的值;see [Expressions in linker scripts](https://link.zhihu.com/?target=https%3A//users.informatik.haw-hamburg.de/~krabat/FH-Labor/gnupro/5_GNUPro_Utilities/c_Using_LD/ldLinker_scripts.html%23Expressions_in%20_linker_scripts).表达式的值存储在位置计数器的当前值处。

' BYTE '， ' SHORT '， ' LONG '和' QUAD '命令分别存储1、2、4和8个字节。存储字节之后，位置计数器将按存储的字节数递增。例如，这里将存储字节1后面跟着符号' addr '的四个字节值:

```text
BYTE(1)
LONG(addr)
```

当使用64位主机或目标机时，' QUAD '和' SQUAD '是相同的;它们都存储8字节或64位的值。当主机和目标机都是32位时，表达式被计算为32位。在这种情况下，' QUAD '存储32位值0扩展到64位，而' SQUAD '存储32位值符号扩展到64位。

#### 输出节废弃 Output section discarding

链接器不会创建没有任何内容的输出节。这是为了方便引用可能出现在或不出现在任何输入文件中的输入节。例如:

```text
.foo { *(.foo) }
```

只有在至少一个输入文件中有' .foo '节时，才会在输出文件中创建' .foo '节。

如果使用输入节描述以外的任何东西作为输出节命令，例如符号赋值，那么输出节将始终被创建，即使没有匹配的输入节。特殊的输出节名' /DISCARD/ '可用于丢弃输入节。任何被分配给名为' /DISCARD/ '的输出节的输入节都不会包含在输出文件中。

#### 输出节加载地址 Output section LMA

```c
SECTION [ address ] [( type )] : [AT( LMA )]
  {
       output-sections-command
       output-sections-command
...
} [> region ] [: phdr : phdr ...] [=fillexp]
```

每个节都有一个虚拟地址（VMA）和加载地址（LMA）。see [Basic linker script concepts](https://link.zhihu.com/?target=https%3A//users.informatik.haw-hamburg.de/~krabat/FH-Labor/gnupro/5_GNUPro_Utilities/c_Using_LD/ldLinker_scripts.html%23Concepts). 可能出现在输出节描述中的地址表达式设置VMA。链接器通常将LMA设置为' VMA '。你可以通过使用' AT '关键字来改变LMA。' AT '关键字后面的LMA表达式指定该节的加载地址。==设计这个特性是为了使构建ROM镜像变得容易==。例如，下面的链接脚本创建了三个输出节:一个名为' .text '，开始于' 0x1000 '，一个名为' .mdata '，在' .text'的末尾加载，即使它的VMA是' 0x2000 '。还有一个节叫'.bss '，它在地址' 0x3000 '保存未初始化的数据。

```text
SECTIONS
 {
    .text 0x1000 : { 
        *(.text) _etext = . ; 
    }
    .mdata 0x2000 : AT(ADDR(.text) + SIZEOF(.text))
    { 
         _data = . ; 
         *(.data); 
         _edata = . ; 
    }   
    .bss 0x3000 :
    { 
         _bstart = . ; 
         *(.bss) *(COMMON) ; 
         _bend = . ;
     }
}
```

符号' _data '用值' 0x2000 '定义，这表明位置计数器保存的是VMA值，而不是LMA值。

使用这个链接脚本生成的程序的运行时初始化代码将包括类似下面的内容：将初始化的数据从ROM镜像复制到它的运行时地址。注意这段代码是如何利用链接脚本定义的符号的。

```c
extern char _etext, _data, _edata, _bstart, _bend;
char *src = &_etext; 
char *dst = &_data;

/* ROM has data at end of text; copy it. */
while (dst < &_edata) {
    *dst++ = *src++;
}

/* Zero bss */
for (dst = &_bstart; dst< &_bend; dst++)
    *dst = 0;
```

( **译注**：注意这里是取符号_etext的地址，不是符号本身的值，与链接脚本中的'_etext = .'直观上不一致。我们在可以这样理解 ‘_etext = .’ ：符号_etext所在的地址是当前的位置计数器（详见简单的赋值）。而_etext的值就是它所在地址上存的内容。我们要的就是该符号所在的地址。）

### 链接脚本实例

```c
ENTRY(start)   /* 入口地址 */
SECTIONS
{
	. = 0x100000;
	PROVIDE( _start = . );
	.text.init :
	{
		*(.text.init)
		. = ALIGN(4096);
	}
	.data.init :
	{
		*(.data.init)
		. = ALIGN(8192);
	}
	. += 0xC0000000;   /* 1100_0000_0000_0000_0000_0000_0000_0000, 2^31 + 2^30 = 3G */
	.text : AT(ADDR(.text) - 0xC0000000)
	{
		*(.text)
		. = ALIGN(4096);
	}
	.data : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
		*(.rodata)
		. = ALIGN(8192);
	}

	.bss : AT(ADDR(.bss) - 0xC0000000)
	{
		*(.bss)
		. = ALIGN(4096);
	}
	.stab : AT(ADDR(.stab) - 0xC0000000)
	{
		*(.stab)
		. = ALIGN(8192);
	}
	.data.init_task : AT(ADDR(.data.init_task) - 0xC0000000)
    {
        *(.data.init_task)
        . = ALIGN(8192);
    }
    PROVIDE( __initcall_start = .);
    .initcall.init : AT(ADDR(.initcall.init) - 0xC0000000)
    {
            *(.initcall.init)
            PROVIDE( __initcall_end = .);
            . = ALIGN(8192);
    }
        . += 0x2000;
	.stabstr : AT(ADDR(.stabstr) - 0xC0000000)
	{
		*(.stabstr)
	 	. = ALIGN(4096);
	}
	PROVIDE( _end = . );

	/DISCARD/ : { *(.comment) *(.eh_frame) }
}

```

