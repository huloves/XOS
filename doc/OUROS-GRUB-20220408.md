# OUROS

## GRUB（Grand Unified Boot Loader）

### Multiboot Specification version 0.6.96

https://www.gnu.org/software/grub/manual/multiboot/multiboot.html

* Overview
* Terminology
* Specification
* Examples
* History
* Invoking mbchk: how to use the Multiboot checker
* index

#### 1 Multiboot Specification 介绍

本节粗略介绍一些关于 Multiboot Specification 的知识。请注意，这不是 Multiboot Specification 本身的内容。

* Motivation - 动机
* Architecture - 架构
* Operating systems - 操作系统
* Boot sources - 引导源
* Boot-time configuration - 引导时配置
* Convenience to operating systems - 对操作系统的便利性
* Boot modules - 引导模块

##### 1.1 Multiboot Specification 的背景

所有的操作系统都会创建自己的 boot loader。在一个机器上安装一个操作系统通常要安装一套全新的启动机制，每一个系统都有不同的 install-time 和 boot-time 用户接口。通过 chaining 机制让多个操作系统在一个机器上可靠的共存会变成一个噩梦。对于特定的操作系统，几乎没有或者说根本没有 boot loader 可供选择。如果操作系统自带的 boot loader 不能完全的满足你的要求，或者不能在你的机器上工作，那你完了，这个系统将很难被加载。

虽然我们可能无法在现有的专有操作系统上解决这个问题，对于 free operating system communities 的人来说，集思广益，为流行的 free operating system 解决这个问题应该不会太难。这就是 Multiboot Specification 的目的。基本上，它指定了 boot loader 和 操作系统之间的接口，这样做之后，符合要求的 boot loader 就能够加载符合要求的操作系统。该规范不指定 boot loader 应该如何工作，它只指定 boot loader 必须如何与正在加载的操作系统交互。

##### 1.2 目标架构（The target architecture）

该规范主要针对PC，因为它们最常见，并且它们拥有最多种类的操作系统和 boot loader。然而，在某些架构可能需要 boot specification 但还没有的情况下，该规范的变体也可以适用于它们，例如去掉X86专有的内容。

##### 1.3 目标操作系统（The target operating systems）

该规范面向 free 32-bit operating systems，这些操作系统可以被非常简单的修改就能支持该规范，而不用修改很多内容。该规范主要针对的特定操作系统是Linux、FreeBSD、NetBSD的内核。希望其他新兴的操作系统从一开始就采用它，从而能够立即利用现有的 boot loader。如果专有操作系统最终也采用该规范，那就太好了，但这可能是一个白日梦。

##### 1.4 引导源（boot sources）

应该编写兼容的 boot loader，从各种源（包括软盘、硬盘和网络）加载操作系统映像。

急于磁盘的 boot loader 可以使用各种技术在磁盘上寻找相关的操作系统映像和引导模块数据，例如通过解释特定文件系统（e.g. the BSD/Mach boot loader）、使用 precalculated *blocklists*（e.g. LILO）、从特定的*boot partition*加载（e.g. OS/2）、或者从其他操作系统加载（e.g. the VSTa boot code, which loads from DOS）。同样，基于网络的引导加载程序可以使用各种网络硬件和协议。

该规范希望 boot loader 能够创建支持多种加载机制，从而提高 boot loader 的可移植性、稳健性和用户友好性。

##### 1.5 在启动时配置操作系统（Configure an operating system at boot-time）

出于各种原因，用户通常需要在引导操作系统时动态地向操作系统提供一些配置信息。该规范不应规定 boot loader 如何获取这些配置信息，但是该规范应该为 boot loader 提供将这些信息传递给操作系统的标准方法。

##### 1.6 如何使操作系统更容易开发（How to make OS development easier）

操作系统映像应该容易生成。理想情况下，一个操作系统映像只是一个简单的 32-bit 可执行文件，该可执行文件格式可以是操作系统使用的任意文件格式（例如 ELF 文件格式）。操作系统映像应该可以像普通的可执行文件一样，可以被 nm 或者被反汇编*（nm是 GNU Binutils 二进制工具集的一员，用于显示目标文件中的符号。）*。专有工具不需要为操作系统映像而使用特殊的文件格式。如果讲一些工作从操作系统转移到 boot loader，这种选择可能更合适，因为因为 boot loader 消耗的内存通常可以在 boot loader 工作完成后再次使用，而OS映像中的每一位代码都必须保留在内存中*（这里应将 boot loader 和 OS image 的工作内容分开，各司其职）*。操作系统不必担心如何进入 32-bit 模式，因为模式转换通常应该在 boot loader 中进行，这样做以便于加载高于1MB边界的操作系统数据。如果强制操作系统完成模式转换，将会使创建 OS映像 变得更加困难。

不幸的是，在免费的类 Unix PC 操作系统中有很多种可执行文件格式，通常每个操作系统都有不同的格式。大多数操作系统使用 a.out 格式的变体，但是一些操作系统正在转向 ELF 文件格式。为了加载操作系统，boot loader 不必解释现行的所有不同类型的可执行文件，否则 boot loader 又将成为某一操作系统专属的软件。

该规范对这一问题采取了一个折中的解决方案。Multiboot 使 boot loader 加载映像时不必理解众多 a.out 变体或者其他可执行文件格式，兼容 Multiboot 的 OS映像 包含一个 magic Multiboot header（see [OS image format](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#OS-image-format)）。这个 magic header 不需要位于可执行文件的最开头，所以内核映像在兼容 Multiboot 的同时，仍然可以符合 a.out 变体格式或者其他可执行文件格式（例如ELF文件格式）。

##### 1.7 引导模块（Boot modules）

类似 Mach 和 VSTa 微内核，很多现代操作系统内核本身没有足够的机制使系统完全运行。它们需要在启动时存在其他软件模块才能访问设备、挂载文件系统等。这些附加模块可以与内核本身一起嵌入到主 OS 映像中，并且当操作系统接收到控制权时，生成的映像会由操作系统手动拆分。如果 boot loader 可以独立加载这些附加模块，通常会更灵活、更节省空间并且对操作系统和用户更方便。

因此，本规范应为 boot loader 提供一种标准方法，向操作系统指示加载了哪些辅助引导模块（boot modules）以及可以在哪里找到它们。boot loader 不必支持多个引导模块（boot modules），但强烈建议它们支持，因为没有它们，某些操作系统将无法引导。

#### 2 规范中专业术语的定义

* *must* - 必须

当任何 boot loader 或 OS映像 需要遵循规则时，我们使用术语 must — 要不然 boot loader 或 OS 映像不兼容 Multiboot。

* *should* - 应该

当建议任何 boot loader 或 OS映像 遵循规则时，我们使用术语 should，但它不是必须遵循规则。

* *may* - 可能

当允许任何 boot loader 或 OS映像 遵循规则时，我们使用术语可能。

* *boot loader*

任何程序或程序集为了在机器上运行，都会加载操作系统的映像。boot loader 本身可能包含几个阶段，但这是与本规范无关的实现细节。只有 boot loader 的最后阶段（最终将控制权转移到操作系统的阶段）必须遵循该规范中指定的规则，这样才能符合 Multiboot； boot loader 之前的阶段可以以最方便的方式设计。

* *OS image* - 操作系统映像

由 boot loader 加载到内存中，并转移控制以启动操作系统的初始二进制映像。操作系统映像通常是包含操作系统内核的可执行文件。

* *boot module* - 引导模块

与 OS映像 一起被 boot loader 加载到内存中的其他辅助文件，除了在调用时将它们的地址传递给操作系统之外，不做解释。

* *Multiboot-compliant* - Multiboot 兼容

为兼容 Multiboot，boot loader 或 OS映像 必须遵循的规则。当本规范指定了应该（should）或可能（may）的规则时，Multiboot-complaint 的 boot loader/OS映像 不是必须遵循该规则。

* *u8*

无符号 8-bit 数据类型。

* *u16*

无符号 16-bit 数据类型。因为目标架构是 little-endian的，所以 u16 采用 little-endian 编码。

* *u32*

无符号 32-bit 数据类型。little-endian 编码。

* u64

无符号 64-bit 数据类型。little-endian 编码。

#### 3 Multiboot Specification 的确切定义（The exact definitions of Multiboot Specification）

boot loader/OS映像接口有三个主要方面：

1. boot loader 看到的 **OS 映像的格式**。
2. boot loader 启动操作系统时的**机器状态**。
3. boot loader 传递给操作系统的**信息格式**。

##### 3.1 OS映像格式

对于特定的操作系统，OS映像可能是标准格式的普通32-bit可执行文件，它可以链接到非默认加载地址，以避免加载到 PC 的 I/O 区域或其他保留区域的顶部，当然 OS映像 不应该使用共享库或其他花哨的功能。

除了 OS 映像使用的头部之外，OS 映像还必须包含一个称为 Multiboot header 的附加头部。Multiboot header 必须完全包含在 OS 映像的前 8192 个字节中，并且必须是长字（32-bit）对齐的。一般来说，它应该尽早出现，并且可能在real executable header 之后嵌入到代码段的开头。

* [Header layout](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Header-layout)（header 布局）：Multiboot header 的布局
* [Header magic fields](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Header-magic-fields)（header magic 字段）：Multiboot header 的 magic 字段
* [Header address fields](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Header-address-fields)（header address 字段）
* [Header graphics fields](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Header-graphics-fields)（header graphics 字段）（标题图形字段）

###### 3.1.1 Multiboot header 布局

Multiboot header 的布局必须如下：

| Offset | Type | Field Name    | Note                |
| ------ | ---- | ------------- | ------------------- |
| 0      | u32  | magic         | required            |
| 4      | u32  | flags         | required            |
| 8      | u32  | checksum      | required            |
| 12     | u32  | header_addr   | if flags[16] is set |
| 16     | u32  | load_addr     | if flags[16] is set |
| 20     | u32  | load_end_addr | if flags[16] is set |
| 24     | u32  | bss_end_addr  | if flags[16] is set |
| 28     | u32  | entry_addr    | if flags[16] is set |
| 32     | u32  | mode_type     | if flags[2] is set  |
| 36     | u32  | width         | if flags[2] is set  |
| 40     | u32  | height        | if flags[2] is set  |
| 44     | u32  | depth         | if flags[2] is set  |

The fields ‘magic’, ‘flags’ and ‘checksum’ are defined in [Header magic fields](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Header-magic-fields), the fields ‘header_addr’, ‘load_addr’, ‘load_end_addr’, ‘bss_end_addr’ and ‘entry_addr’ are defined in [Header address fields](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Header-address-fields), and the fields ‘mode_type’, ‘width’, ‘height’ and ‘depth’ are defined in [Header graphics fields](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Header-graphics-fields).

###### 3.1.2 Multiboot header 的 magic fields（The magic fields of Multiboot header）

* ‘magic’

’magic‘ 字段是标识 header 的魔数，必须是十六进制值 0x1BADB002。

* ’flags‘

“flags”字段指定 OS 映像请求或要求 boot loader 的功能。第 0-15 位表示要求，如果 boot loader 看到这些位中的任何一个被设置了，但不理解标志或由于某种原因不能满足它指示的要求，必须通知用户并且无法加载OS映像；第 16-31 位表示可选功能，如果设置了此范围内的任何位，但 boot loader 不理解它们，可能会简单地忽略它们并照常进行。当然，OS映像中，“flags”字中所有尚未定义的位都必须设置为零。通过这种方式，“flags”字段用于版本控制以及简单的功能选择。

> 我的理解：当然，“flags”字中所有尚未定义的位都必须设置为零。

如果设置了“flags”字中的第 0 位，则与操作系统一起加载的所有**引导模块**必须在页面 (4KB) 边界上对齐。一些操作系统希望能够在启动期间将包含引导模块的页面直接映射到分页地址空间，因此需要引导模块进行页面对齐。

如果设置了“flags”字中的第 1 位，必须包括**可用内存信息**，通过 Multiboot infomation structure （see [Boot information format](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Boot-information-format)）得到 `mem_*` 字段指示的**可用内存信息**。如果 boot loader 能够传递**内存映射**（“mmap_*” fields），并且存在一个内存映射，那么该内存映射也可能包括在内。

如果设置了“flags”字中的第 2 位，则有关视频模式表（video mode table）的信息（see [Boot information format](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Boot-information-format)）必须可供内核使用。

如果设置了“flags”字中的第 16 位，那么 Multiboot header 中偏移量为 12-28 的字段是有效的，并且 boot loader 应该使用它们而不是实际可执行文件头中的字段来计算加载 OS 映像的位置。如果内核映像是 ELF 格式，则不需要提供此信息，但如果OS映像是 a.out 格式或其他格式，则必须（*must*）提供。兼容的 boot loader 必须能够加载 ELF 格式的映像，或能够加载 Multiboot header 中加载地址信息的映像；boot loader 也可以直接支持其他可执行格式，例如特定的 a.out 变体，但不是必须的。

* ‘checksum’

“checksum”字段是一个32位无符号值，当添加到其他 magic fidlds（i.e. ‘magic’ and ‘flags’）时，“checksum”32位无符号和必须为零。

###### 3.1.3 Multiboot header 的 address fields（The address fields of Multiboot header）

由“flags”字段的第 16 位启用的所有地址字段都是物理地址。每个的含义如下：

* header_addr

对应于 Multiboot header 的起始地址——应该加载 magic value 的物理内存地址。该字段用于同步 OS 映像偏移和物理内存地址之间的映射。

* lead_addr

text segment 的起始物理地址。OS 映像文件中开始加载的偏移量由找到的 header 的偏移量定义，减去（header_addr - load_addr）。load_addr 必须小于或等于 header_addr。

> 翻译的shit，原文：Contains the physical address of the beginning of the text segment. The offset in the OS image file at which to start loading is defined by the offset at which the header was found, minus (header_addr - load_addr). load_addr must be less than or equal to header_addr.

* load_end_addr

data segment 的尾物理地址。(load_end_addr - load_addr) 指定要加载多少数据。这意味着 text and data segment 在OS映像中必须是连续的，这也适用于现有的 a.out 可执行文件格式。如果该字段为零，则 boot loader 假定 text and data segment 占据整个 OS 映像文件。

* bss_end_addr

bss segment 的尾物理地址。boot loader 将此区域初始化为零，并保留其占用的内存，以避免将引导模块和其他与操作系统相关的数据放置在该区域中。

* entry_addr

为了开始运行操作系统，boot loader 应该跳转到的物理地址。

###### 3.1.4 Multiboot header 的 graphics fields（The graphics fields of Multiboot header）

所有图形字段都由“flags”的第 2 位启用。它们指定首选的图形模式。请注意，这只是操作系统映像推荐的模式。如果 boot loader 认为合适，boot loader 可以选择不同的模式。

每个的含义如下：

* mode_type

  “0”表示线性图形模式或“1”表示 EGA 标准文本模式。其他一切都为将来的扩展而保留。请注意，即使该字段包含“0”，引导加载程序也可以设置文本模式，或者即使该字段包含“1”，也可以设置视频模式。

* width

  列数。这在图形模式中以像素为单位，在文本模式中以字符为单位。零值表示操作系统映像 has no preference。

* height

  行数。这在图形模式中以像素为单位，在文本模式中以字符为单位。零值表示操作系统映像 has no preference。

* depth

  图形模式下每像素的位数，文本模式下为零。The value zero indicates that the OS image has no preference.

##### 3.2 机器状态（Machine state）

当 boot loader 调用 32 位操作系统时，机器必须具有以下状态：

* EAX

  必须包含魔数“0x2BADB002”；该值的存在向操作系统表明，它是由符合Multiboot的 boot loader 加载的（与可以加载操作系统的另一种 boot loader 不同）。

* EBX

  必须包含 boot loader 提供的 Multiboot information structure 32 位物理地址（physical address）。（see [Boot information format](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Boot-information-format)）

* cs

  必须是 32-bit 的 read/execute code segment，偏移量为“0”，限制为“0xFFFFFFFF”。确切的值是未定义的。

* DS

* ES

* FS

* GS

* SS

  必须是 32-bit 的 read/write data segment，偏移量为0，限制为“0xFFFFFFFF”。确切的值是未定义的。

* A20 gate

  必须使能。

* CR0

  位 31 (PG) 必须清零。必须设置位 0 (PE)。其他位都是未定义的。

* EFLAGS

  位 17 (VM) 必须清零。位 9 (IF) 必须清零。其他位都是未定义的。

所有其他处理器寄存器和标志位都未定义。这尤其包括：

* ESP

  OS映像必须在需要时立即创建自己的堆栈。

* GDTR

  即使段寄存器如上所述设置，“GDTR”可能无效，因此 OS映像 在设置自己的“GDT”之前不得加载任何段寄存器（即使只是重新加载相同的值！）。

* IDTR

  OS映像必须禁用中断，直到它建立自己的 IDT。

但是，其他机器状态应该由 boot loader 以正常工作顺序保留，即由 BIOS（或 DOS，如果 boot loader 从其运行）初始化。换句话说，操作系统应该能够在加载后进行 BIOS 调用等，只要它在这样做之前不覆盖 BIOS 数据结构。此外，boot loader 必须使用正常的 BIOS/DOS 值对 PIC 进行编程，即使 boot loader 在切换到 32 位模式期间更改了它们。

> 可编程中断控制器(Programmable Interrupt Controller)，也简称为PIC，是微处器与外设之间的中断处理的桥梁，由外设发出的中断请求需要中断控制器来进行处理。

##### 引导信息格式（Boot information format）

进入操作系统之后，EBX 寄存器包含 Multiboot information structure 的物理地址，boot loader 通过它向操作系统传达重要信息。操作系统可以选择使用或忽略结构的任何部分；boot loader 传递的所有信息仅供操作系统参考。

Multiboot information structure 及其相关子结构可以由引导加载程序放置在内存中的任何位置（当然，为内核和引导模块保留的内存除外）。操作系统有责任避免覆盖此内存（Multi boot information structure 及其相关子结构），直到它使用完毕。

Multiboot information structure 的格式（到目前为止已定义）如下：

```text
        +-------------------+
0       | flags             |    (required)
        +-------------------+
4       | mem_lower         |    (present if flags[0] is set)
8       | mem_upper         |    (present if flags[0] is set)
        +-------------------+
12      | boot_device       |    (present if flags[1] is set)
        +-------------------+
16      | cmdline           |    (present if flags[2] is set)
        +-------------------+
20      | mods_count        |    (present if flags[3] is set)
24      | mods_addr         |    (present if flags[3] is set)
        +-------------------+
28 - 40 | syms              |    (present if flags[4] or
        |                   |                flags[5] is set)
        +-------------------+
44      | mmap_length       |    (present if flags[6] is set)
48      | mmap_addr         |    (present if flags[6] is set)
        +-------------------+
52      | drives_length     |    (present if flags[7] is set)
56      | drives_addr       |    (present if flags[7] is set)
        +-------------------+
60      | config_table      |    (present if flags[8] is set)
        +-------------------+
64      | boot_loader_name  |    (present if flags[9] is set)
        +-------------------+
68      | apm_table         |    (present if flags[10] is set)
        +-------------------+
72      | vbe_control_info  |    (present if flags[11] is set)
76      | vbe_mode_info     |
80      | vbe_mode          |
82      | vbe_interface_seg |
84      | vbe_interface_off |
86      | vbe_interface_len |
        +-------------------+
88      | framebuffer_addr  |    (present if flags[12] is set)
96      | framebuffer_pitch |
100     | framebuffer_width |
104     | framebuffer_height|
108     | framebuffer_bpp   |
109     | framebuffer_type  |
110-115 | color_info        |
        +-------------------+
```

第一个长字表示 Multiboot information structure 中其他字段的存在和有效性。boot loader 必须将所有尚未定义的位设置为零。操作系统不理解的位都应该被忽略。因此，“flags”字段还可以用作版本指示器，允许将来扩展 Multiboot information structure 而不会破坏任何内容。

如果“flags”字中的位 0 被设置，则“mem_*”字段有效。‘mem_lower’ 和 ‘mem_upper’ 分别表示 lower and upper memoty 的数量，以 KB 为单位。Lower memory 从地址 0 开始，upper memory 从 1MB 开始。Lower memory 的最大可能值为 640 KB。upper memory 返回的最大值是第一个 upper memory hole 减去 1MB。这是一个不确定的值。

> 原文：‘mem_lower’ and ‘mem_upper’ indicate the amount of lower and upper memory, respectively, in kilobytes.
>
> 不懂 lower and upper memory

如果“flags”字中的位 1 被设置，则“boot_device”字段有效，并指示 boot loader 从哪个 BIOS 磁盘设备加载 OS 映像。如果操作系统映像不是从 BIOS 磁盘加载的，则该字段不得存在（必须清除第 3 位）。操作系统可以使用该字段作为确定其自己根设备（root device）的提示，但不是必须的。“boot_device”字段以四个 1 字节的子字段排列，如下所示：

```text
+-------+-------+-------+-------+
| part3 | part2 | part1 | drive |
+-------+-------+-------+-------+
Least significant             Most significant
```

最高有效字节包含 BIOS INT 0x13 low-level disk interface 所理解的 BIOS 驱动器号：例如0x00 表示第一张软盘或 0x80 表示第一块硬盘。

其余三个字节指定引导分区（boot partition）。‘part1’指定 *top-level* partition number，‘part2’指定 *sub-partition* in the top-level partition, etc。分区号（partition number）总是从零开始。Unused partition bytes 必须设置为 0xFF。例如，如果磁盘使用简单的一级 DOS 分区方案进行分区，则“part1”包含 DOS 分区号，“part2”和“part3”均为 0xFF。再举一个例子，如果一个磁盘首先被划分为 DOS 分区，然后使用 BSD 的 disklabel 策略将其中一个 DOS 分区细分为多个 BSD 分区，那么“part1”包含 DOS 分区号，“part2”包含在DOS分区中的 BSD 子分区号，“part3”为 0xFF。

如果设置了“flags”的位 2，则“cmdline”字段有效，并且包含要传递给内核的命令行的物理地址。命令行是一个普通的 C 风格的以零结尾的字符串。命令行的确切格式留给操作系统开发人员。通用 boot loader 应允许用户完全控制命令行，而不受映像名称等其他因素的影响。考虑到有特定负载的 boot loader 可以通过算法完全或部分生成命令行地址。

如果设置了“flags”的位 3，“mods”字段向内核指示与内核映像一起加载了哪些引导模块，以及可以在哪里找到它们。‘mods_count’ 包含加载的模块数；‘mods_addr’包含第一个模块结构的物理地址。‘mods_count’ 可能为零，表示没有加载引导模块，即使设置了“标志”的第 3 位。每个模块结构的格式如下：

```text
        +-------------------+
0       | mod_start         |
4       | mod_end           |
        +-------------------+
8       | string            |
        +-------------------+
12      | reserved (0)      |
        +-------------------+
```

前两个字段包含引导模块本身的开始和结束地址。“string”字段提供一个与特定引导模块关联的任意字符串，它是一个以零结尾的 ASCII 字符串，就像内核命令行一样。如果没有与模块关联的字符串，则“string”字段可能为 0。通常，该字符串可能是命令行（例如，如果操作系统将引导模块视为可执行程序），或路径名（例如，如果操作系统将引导模块视为文件系统中的文件），但它的具体用途取决于操作系统。“reserved”字段必须由 boot loader 设置为 0，并被操作系统忽略。

> 注意：Multiboot information structure 中"flags"字段的位 4 和 5 是互斥的。

如果“flags”字段中的位 4 被设置，那么 Multiboot information structure 中从字节 28 开始的以下字段是有效的：

```text
        +-------------------+
28      | tabsize           |
32      | strsize           |
36      | addr              |
40      | reserved (0)      |
        +-------------------+
```

这些指示可以从 a.out 内核映像中找到符号表（symbol table）的位置。‘addr’ 是 a.out 格式、 nlist 结构数组的大小（4 字节无符号长）的物理地址，紧随其后的是数组本身，然后是一组以零结尾的 ASCII 字符串的大小（4-byte unsigned long）（在这种情况下加上 sizeof(unsigned long)），最后是字符串集本身。'tabsize' 等于它的大小参数（found at the beginning of the symbol section），并且“strsize”等于符号表所引用的字符串表的大小参数（found at the beginning of the string section）。请注意，“tabsize”可能为 0，表示没有符号，即使设置了“flags”字中的 位 4。

> 原文：
>
> These indicate where the symbol table from an a.out kernel image can be found. ‘addr’ is the physical address of the size (4-byte unsigned long) of an array of a.out format *nlist* structures, followed immediately by the array itself, then the size (4-byte unsigned long) of a set of zero-terminated ASCII strings (plus sizeof(unsigned long) in this case), and finally the set of strings itself. ‘tabsize’ is equal to its size parameter (found at the beginning of the symbol section), and ‘strsize’ is equal to its size parameter (found at the beginning of the string section) of the following string table to which the symbol table refers. Note that ‘tabsize’ may be 0, indicating no symbols, even if bit 4 in the ‘flags’ word is set.

如果“flags”字中的位 5 被设置，则 Multiboot information structure 中从第 28 字节开始以下字段有效：

```text
        +-------------------+
28      | num               |
32      | size              |
36      | addr              |
40      | shndx             |
        +-------------------+
```

这些指示 ELF 内核的 section header table 在哪里，每个条目的大小、条目数以及用作名称索引的字符串表（the string table used as the index of names）。它们对应于程序头中可执行和可链接格式 (ELF) 规范中的“shdr_*”条目（“shdr_num”等）。加载所有节，ELF节头的物理地址字段表示节在内存中的位置（有关如何阅读 section header(s) 的详细信息，请参阅 i386 ELF 文档）。请注意，“shdr_num”可能为 0，表示没有符号，即使设置了“标志”字中的第 5 位。

如果“flags”字中的位 6 被设置，则“mmap_*”字段有效，并指示缓冲区的地址和长度，该缓冲区是 BIOS 提供的用于机器内存映射的缓冲区。‘mmap_addr’是缓冲区的地址，‘mmap_length’是缓冲区的总大小。缓冲区由以下一个或多个大小/结构对组成（“size”实际上用于跳到下一对）：

```text
        +-------------------+
-4      | size              |
        +-------------------+
0       | base_addr         |
8       | length            |
16      | type              |
        +-------------------+
```

其中“size”是关联结构（associated structure）的大小，以字节为单位的，可以大于最小值 20 字节。“base_addr”是起始地址。“length”是内存区域的大小，以字节为单位。‘type’ 表示的地址范围的种类，其中值 1 表示可用 RAM，值 3 表示保存 ACPI 信息的可用内存，值 4 表示在休眠时需要保留的保留内存，值 5 表示内存被有缺陷的 RAM 模块（defective RAM modules）占用，所有其他值当前表示保留区域。

提供的 map 保证列出所有正常使用的标准RAM。

如果“flags”中的位 7 被设置，则“drives_*”字段有效，并指示第一个驱动器结构（drive structure）的物理地址的地址和大小。“drives_addr”是地址，“drives_length”是驱动器结构的总大小。请注意，“drives_length”可能为零。每个驱动器结构的格式如下：

```text
        +-------------------+
0       | size              |
        +-------------------+
4       | drive_number      |
        +-------------------+
5       | drive_mode        |
        +-------------------+
6       | drive_cylinders   |
8       | drive_heads       |
9       | drive_sectors     |
        +-------------------+
10 - xx | drive_ports       |
        +-------------------+
```

“size”字段指定此结构的大小。大小因端口数量而异。请注意，由于对齐，大小可能不等于（10 + 2 * 端口数）。

“drive_number”字段包含 BIOS 驱动器号。“drive_mode”字段表示 boot loader 使用的访问模式。目前，定义了以下模式：

* 0

  CHS 模式（传统的柱面/磁头/扇区寻址模式）。

* 1

  LBA 模式（逻辑块寻址模式）。

“drive_cylinders”、“drive_heads”和“drive_sectors”这三个字段表示 BIOS 检测到的驱动器的几何形状。‘drive_cylinders’ 包含柱面的数量。 ‘drive_heads’包含磁头的数量。 ‘drive_sectors’包含每个磁道的扇区数。

“drive_ports”字段包含 BIOS 代码中用于驱动器的 I/O 端口数组。该数组由零个或多个无符号双字节整数组成，并以零结尾。请注意，数组可能包含任意数量的与驱动器实际无关的 I/O 端口（例如 DMA 控制器的端口）。

如果“flags”中的位 8 被设置，则“config_table”字段有效，并指示 BIOS 调用 GET CONFIGURATION 返回的 ROM 配置表的地址。如果 BIOS 调用失败，则表的大小必须为零。

如果“flags”中的位 9 被设置，则“boot_loader_name”字段有效，并且包含引导内核的 boot loader 名称的物理地址。该名称是一个普通的 C 风格的以零结尾的字符串。

如果“flags”中的位 10 被设置，“apm_table”字段是有效的，并且包含 APM 表的物理地址，所指向的 APM 表定义如下：

```text
        +----------------------+
0       | version              |
2       | cseg                 |
4       | offset               |
8       | cseg_16              |
10      | dseg                 |
12      | flags                |
14      | cseg_len             |
16      | cseg_16_len          |
18      | dseg_len             |
        +----------------------+
```

字段'version', 'cseg', 'offset', 'cseg_16', 'dseg', 'flags', 'cseg_len', 'cseg_16_len', 'dseg_len' 表示版本号，保护模式32位代码段, 入口点的偏移量, 保护模式 16 位代码段, 保护模式 16 位数据段, 标志, 保护模式 32 位代码段长度, 保护模式 16 位代码段长度和保护模式 16 位数据段的长度。只有“offset”字段为 4 个字节，其他为 2 个字节。有关详细信息，请参阅高级电源管理 (APM) BIOS 接口规范（[Advanced Power Management (APM) BIOS Interface Specification](http://www.microsoft.com/hwdev/busbios/amp_12.htm)）。

如果“flags”中的位 11 被设置，则 VBE 表可用。

The fields ‘vbe_control_info’ and ‘vbe_mode_info’ contain the physical addresses of VBE control information returned by the VBE Function 00h and VBE mode information returned by the VBE Function 01h, respectively.

The field ‘vbe_mode’ indicates current video mode in the format specified in VBE 3.0.

The rest fields ‘vbe_interface_seg’, ‘vbe_interface_off’, and ‘vbe_interface_len’ contain the table of a protected mode interface defined in VBE 2.0+. If this information is not available, those fields contain zero. Note that VBE 3.0 defines another protected mode interface which is incompatible with the old one. If you want to use the new protected mode interface, you will have to find the table yourself.

The fields for the graphics table are designed for VBE, but Multiboot boot loaders may simulate VBE on non-VBE modes, as if they were VBE modes.

如果“flags”中的位 12 被设置，则 FRAMEBUFFER 表（帧缓冲表）可用。

The field ‘framebuffer_addr’ contains framebuffer physical address. This field is 64-bit wide but bootloader *should* set it under 4 GiB if possible for compatibility with kernels which aren’t aware of PAE or AMD64. The field ‘framebuffer_pitch’ contains the framebuffer pitch in bytes. The fields ‘framebuffer_width’, ‘framebuffer_height’ contain the framebuffer dimensions in pixels. The field ‘framebuffer_bpp’ contains the number of bits per pixel. If ‘framebuffer_type’ is set to ‘0’ it means indexed color will be used. In this case color_info is defined as follows:

```text
        +----------------------------------+
110     | framebuffer_palette_addr         |
114     | framebuffer_palette_num_colors   |
        +----------------------------------+
```

‘framebuffer_palette_addr’ contains the address of the color palette, which is an array of color descriptors. Each color descriptor has the following structure:

```text
        +-------------+
0       | red_value   |
1       | green_value |
2       | blue_value  |
        +-------------+
```

If ‘framebuffer_type’ is set to ‘1’ it means direct RGB color will be used. Then color_type is defined as follows:

```text
        +----------------------------------+
110     | framebuffer_red_field_position   |
111     | framebuffer_red_mask_size        |
112     | framebuffer_green_field_position |
113     | framebuffer_green_mask_size      |
114     | framebuffer_blue_field_position  |
115     | framebuffer_blue_mask_size       |
        +----------------------------------+
```

If ‘framebuffer_type’ is set to ‘2’ it means EGA-standard text mode will be used. In this case ‘framebuffer_width’ and ‘framebuffer_height’ are expressed in characters instead of pixels. ‘framebuffer_bpp’ is equal to 16 (bits per character) and ‘framebuffer_pitch’ is expressed in bytes per text line. All further values of ‘framebuffer_type’ are reserved for future expansion.

#### 4 例子（Example）

注意：The following items are not part of the specification document, but are included for prospective operating system and boot loader writers.

* Notes on PC - PC 注意事项
* BIOS device mapping techniques - BIOS 设备映射技术
* Example OS code
* Example boot loader code

##### 4.1 PC 注意事项（Notes on PC）

参考 Multiboot information structure 中‘flags’参数的位 0，如果所讨论的 boot loader 使用较旧的 BIOS 接口，或者最新的接口不可用（参见关于位 6 的说明），则可能会报告最多 15 或 63 兆字节的内存。强烈建议 boot loader 执行彻底的内存探测。

参考 Multiboot information structure 中的“flags”参数的位 1，我们认识到，确定哪个 BIOS 驱动器映射到操作系统中的哪个设备驱动器是很重要的。人们对各种操作系统进行了大量修改，但没有解决这个问题，其中大多数在许多情况下都会崩溃。为了鼓励使用通用解决方案来解决这个问题，有 2 种 BIOS 设备映射技术（see [BIOS device mapping techniques](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#BIOS-device-mapping-techniques)）。

参考 Multiboot information structure 中‘flags’参数的位 6，需要注意的是，需要注意的是，此处使用的数据结构（以“BaseAddrLow”开头）是 INT 15h，AX=E820h 返回的数据——查询系统地址映射。有关更多信息，请参阅 GRUB 手册中的查询系统地址映射（[Query System Address Map](http://www.gnu.org/software/grub/manual/grub.html#Query-System-Address-Map)）。此处的接口旨在允许 boot loader 在未修改的情况下与 BIOS 接口的任何合理扩展一起工作，并根据需要传递任何额外的数据以供操作系统解释。

##### 4.2 BIOS 设备映射技术（BIOS device mapping techniques）

这两种技术都应该可以在任何 PC 操作系统上使用，并且不需要驱动程序本身的任何特殊支持。本节将展开详细解释，特别是 I/O 限制技术（I/O restriction technique）。

一般的方法是数据比较技术（the data comparison technique），该技术是快速而肮脏的解决方案。它在大多数情况下都有效，但并未涵盖所有情况，并且相对简单。

I/O限制技术（The I/O restriction technique）要复杂得多，但它有可能在所有情况下解决问题，并且在不是所有设备都有操作系统驱动程序时允许访问剩余的 BIOS 设备。

> The I/O restriction technique is much more complex, but it has potential to solve the problem under all conditions, plus allow access of the remaining BIOS devices when not all of them have operating system drivers.

* 数据比较技术（the data comparison technique）
* I/O限制技术（The I/O restriction technique）

###### 4.2.1 数据比较技术（the data comparison technique）

在激活任何设备驱动程序之前，请从每个磁盘上的相似扇区收集足够的数据，以便可以唯一识别每个磁盘。

激活设备驱动程序后，使用操作系统驱动程序比较驱动器中的数据。这种情况足以提供这样的映射。

问题：

1. 某些 BIOS 设备上的数据可能是相同的（因此从 BIOS 读取驱动器的部分信息时，应该有某种机制可以放弃）
2. 可能有额外的驱动器无法从 BIOS 访问，这些驱动器与BIOS使用的某些驱动器相同（因此也应该能够放弃）

###### 4.2.2 I/O限制技术（The I/O restriction technique）

这第一步可能是不必要的，首先为写入 PC RAM 的设备驱动程序创建写时复制映射。保留原始副本以供以后创建的干净 BIOS 虚拟机（the *clean BIOS virtual machine*）使用。

对于每个联机的设备驱动程序，通过以下方式确定哪些 BIOS 设备变得不可访问：

1. 创建一个干净的 BIOS 虚拟机（a *clean BIOS virtual machine*）。
2. 将 I/O 区域的 I/O 权限映射设置为无权限（不可读写），其中I/O区域是设备驱动程序声明的I/O区域。
3. 访问每个设备。
4. 记录那些设备成功，哪些设备尝试访问受限的 I/O 区域（hopefully, this will be an *xor* situation）。

对于每个设备驱动程序，被包含在多少个 BIOS 设备（此列表中不应有任何空白），应该很容易确定是控制器上的哪些设备。

> 原文：
>
> For each device driver, given how many of the BIOS devices were subsumed by it (there should be no gaps in this list), it should be easy to determine which devices on the controller these are.

通常，BIOS 编号的每个控制器最多有 2 个磁盘，但它们几乎总是从控制器上逻辑编号最低的设备开始计数。

> 原文：
>
> In general, you have at most 2 disks from each controller given BIOS numbers, but they pretty much always count from the lowest logically numbered devices on the controller.

##### 4.3 Example OS code

在这部分，the example Multiboot kernel `kernel` is included. The kernel 只是在屏幕上打印出 Multiboot information structure，所以可以利用 the kernel 去测试 Multiboot-compliant 的 boot loader 以及提供如何实现 Multiboot kernel 的参考。源文件可以在 Multiboot 源代码分发的目录 `doc` 下找到。

The kernel `kernel` consists of only three files: boot.S, kernel.c and multiboot.h. 汇编源文件 boot.S 是用 GAS（see [GNU assembler](http://sourceware.org/binutils/docs/as/index.html#Top) in The GNU assembler） 编写的，并且包含符合规范的 Multiboot information structure。当 Multiboot-compliant 的 boot loader 加载并运行时，该 boot loader 初始化堆栈指针和 `EFLAGS`，然后调用 `kernel.c` 中定义的函数 `cmain.c`。如果 `cmain` 返回到被调用者，然后它会显示一条消息，通知用户暂停状态并永远停止，直到您按下重置键。文件 `kernel.c` 包含函数 `cmain`，它检查 boot loader 传递的魔数是否有效等，以及调用一些在屏幕上打印消息的函数。文件 `multiboot.h` 定义了一些宏，例如 Multiboot header 的 magic number、Multiboot header structure 和 Multiboot information structure。

* multiboot.h
* boot.S
* kernel.c
* Other Multiboot kernels

###### 4.3.1 multiboot.h

这是文件 `multiboot.h` 中的源代码：

```c
/* multiboot.h - Multiboot header file. */
/* Copyright (C) 1999,2003,2007,2008,2009,2010  Free Software Foundation, Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL ANY
 *  DEVELOPER OR DISTRIBUTOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MULTIBOOT_HEADER
#define MULTIBOOT_HEADER 1

/* How many bytes from the start of the file we search for the header. */
#define MULTIBOOT_SEARCH                        8192
#define MULTIBOOT_HEADER_ALIGN                  4

/* The magic field should contain this. */
#define MULTIBOOT_HEADER_MAGIC                  0x1BADB002

/* This should be in %eax. */
#define MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002

/* Alignment of multiboot modules. */
#define MULTIBOOT_MOD_ALIGN                     0x00001000

/* Alignment of the multiboot info structure. */
#define MULTIBOOT_INFO_ALIGN                    0x00000004

/* Flags set in the ’flags’ member of the multiboot header. */

/* Align all boot modules on i386 page (4KB) boundaries. */
#define MULTIBOOT_PAGE_ALIGN                    0x00000001

/* Must pass memory information to OS. */
#define MULTIBOOT_MEMORY_INFO                   0x00000002

/* Must pass video information to OS. */
#define MULTIBOOT_VIDEO_MODE                    0x00000004

/* This flag indicates the use of the address fields in the header. */
#define MULTIBOOT_AOUT_KLUDGE                   0x00010000

/* Flags to be set in the ’flags’ member of the multiboot info structure. */

/* is there basic lower/upper memory information? */
#define MULTIBOOT_INFO_MEMORY                   0x00000001
/* is there a boot device set? */
#define MULTIBOOT_INFO_BOOTDEV                  0x00000002
/* is the command-line defined? */
#define MULTIBOOT_INFO_CMDLINE                  0x00000004
/* are there modules to do something with? */
#define MULTIBOOT_INFO_MODS                     0x00000008

/* These next two are mutually exclusive */

/* is there a symbol table loaded? */
#define MULTIBOOT_INFO_AOUT_SYMS                0x00000010
/* is there an ELF section header table? */
#define MULTIBOOT_INFO_ELF_SHDR                 0X00000020

/* is there a full memory map? */
#define MULTIBOOT_INFO_MEM_MAP                  0x00000040

/* Is there drive info? */
#define MULTIBOOT_INFO_DRIVE_INFO               0x00000080

/* Is there a config table? */
#define MULTIBOOT_INFO_CONFIG_TABLE             0x00000100

/* Is there a boot loader name? */
#define MULTIBOOT_INFO_BOOT_LOADER_NAME         0x00000200

/* Is there a APM table? */
#define MULTIBOOT_INFO_APM_TABLE                0x00000400

/* Is there video information? */
#define MULTIBOOT_INFO_VBE_INFO                 0x00000800
#define MULTIBOOT_INFO_FRAMEBUFFER_INFO         0x00001000

#ifndef ASM_FILE

typedef unsigned char           multiboot_uint8_t;
typedef unsigned short          multiboot_uint16_t;
typedef unsigned int            multiboot_uint32_t;
typedef unsigned long long      multiboot_uint64_t;

struct multiboot_header
{
  /* Must be MULTIBOOT_MAGIC - see above. */
  multiboot_uint32_t magic;

  /* Feature flags. */
  multiboot_uint32_t flags;

  /* The above fields plus this one must equal 0 mod 2^32. */
  multiboot_uint32_t checksum;

  /* These are only valid if MULTIBOOT_AOUT_KLUDGE is set. */
  multiboot_uint32_t header_addr;
  multiboot_uint32_t load_addr;
  multiboot_uint32_t load_end_addr;
  multiboot_uint32_t bss_end_addr;
  multiboot_uint32_t entry_addr;

  /* These are only valid if MULTIBOOT_VIDEO_MODE is set. */
  multiboot_uint32_t mode_type;
  multiboot_uint32_t width;
  multiboot_uint32_t height;
  multiboot_uint32_t depth;
};

/* The symbol table for a.out. */
struct multiboot_aout_symbol_table
{
  multiboot_uint32_t tabsize;
  multiboot_uint32_t strsize;
  multiboot_uint32_t addr;
  multiboot_uint32_t reserved;
};
typedef struct multiboot_aout_symbol_table multiboot_aout_symbol_table_t;

/* The section header table for ELF. */
struct multiboot_elf_section_header_table
{
  multiboot_uint32_t num;
  multiboot_uint32_t size;
  multiboot_uint32_t addr;
  multiboot_uint32_t shndx;
};
typedef struct multiboot_elf_section_header_table multiboot_elf_section_header_table_t;

struct multiboot_info
{
  /* Multiboot info version number */
  multiboot_uint32_t flags;

  /* Available memory from BIOS */
  multiboot_uint32_t mem_lower;
  multiboot_uint32_t mem_upper;

  /* "root" partition */
  multiboot_uint32_t boot_device;

  /* Kernel command line */
  multiboot_uint32_t cmdline;

  /* Boot-Module list */
  multiboot_uint32_t mods_count;
  multiboot_uint32_t mods_addr;

  union
  {
    multiboot_aout_symbol_table_t aout_sym;
    multiboot_elf_section_header_table_t elf_sec;
  } u;

  /* Memory Mapping buffer */
  multiboot_uint32_t mmap_length;
  multiboot_uint32_t mmap_addr;

  /* Drive Info buffer */
  multiboot_uint32_t drives_length;
  multiboot_uint32_t drives_addr;

  /* ROM configuration table */
  multiboot_uint32_t config_table;

  /* Boot Loader Name */
  multiboot_uint32_t boot_loader_name;

  /* APM table */
  multiboot_uint32_t apm_table;

  /* Video */
  multiboot_uint32_t vbe_control_info;
  multiboot_uint32_t vbe_mode_info;
  multiboot_uint16_t vbe_mode;
  multiboot_uint16_t vbe_interface_seg;
  multiboot_uint16_t vbe_interface_off;
  multiboot_uint16_t vbe_interface_len;

  multiboot_uint64_t framebuffer_addr;
  multiboot_uint32_t framebuffer_pitch;
  multiboot_uint32_t framebuffer_width;
  multiboot_uint32_t framebuffer_height;
  multiboot_uint8_t framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB     1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT     2
  multiboot_uint8_t framebuffer_type;
  union
  {
    struct
    {
      multiboot_uint32_t framebuffer_palette_addr;
      multiboot_uint16_t framebuffer_palette_num_colors;
    };
    struct
    {
      multiboot_uint8_t framebuffer_red_field_position;
      multiboot_uint8_t framebuffer_red_mask_size;
      multiboot_uint8_t framebuffer_green_field_position;
      multiboot_uint8_t framebuffer_green_mask_size;
      multiboot_uint8_t framebuffer_blue_field_position;
      multiboot_uint8_t framebuffer_blue_mask_size;
    };
  };
};
typedef struct multiboot_info multiboot_info_t;

struct multiboot_color
{
  multiboot_uint8_t red;
  multiboot_uint8_t green;
  multiboot_uint8_t blue;
};

struct multiboot_mmap_entry
{
  multiboot_uint32_t size;
  multiboot_uint64_t addr;
  multiboot_uint64_t len;
#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5
  multiboot_uint32_t type;
} __attribute__((packed));
typedef struct multiboot_mmap_entry multiboot_memory_map_t;

struct multiboot_mod_list
{
  /* the memory used goes from bytes ’mod_start’ to ’mod_end-1’ inclusive */
  multiboot_uint32_t mod_start;
  multiboot_uint32_t mod_end;

  /* Module command line */
  multiboot_uint32_t cmdline;

  /* padding to take it to 16 bytes (must be zero) */
  multiboot_uint32_t pad;
};
typedef struct multiboot_mod_list multiboot_module_t;

/* APM BIOS info. */
struct multiboot_apm_info
{
  multiboot_uint16_t version;
  multiboot_uint16_t cseg;
  multiboot_uint32_t offset;
  multiboot_uint16_t cseg_16;
  multiboot_uint16_t dseg;
  multiboot_uint16_t flags;
  multiboot_uint16_t cseg_len;
  multiboot_uint16_t cseg_16_len;
  multiboot_uint16_t dseg_len;
};

#endif /* ! ASM_FILE */

#endif /* ! MULTIBOOT_HEADER */
```

###### 4.3.2 boot.S

In the file `boot.S`:

```assembly
/* boot.S - bootstrap the kernel */
/* Copyright (C) 1999, 2001, 2010  Free Software Foundation, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define ASM_FILE        1
#include <multiboot.h>

/* C symbol format. HAVE_ASM_USCORE is defined by configure. */
#ifdef HAVE_ASM_USCORE
# define EXT_C(sym)                     _ ## sym
#else
# define EXT_C(sym)                     sym
#endif

/* The size of our stack (16KB). */
#define STACK_SIZE                      0x4000

/* The flags for the Multiboot header. */
#ifdef __ELF__
# define AOUT_KLUDGE 0
#else
# define AOUT_KLUDGE MULTIBOOT_AOUT_KLUDGE
#endif
#define MULTIBOOT_HEADER_FLAGS                  MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_VIDEO_MODE | AOUT_KLUDGE
        
        .text

        .globl  start, _start
start:
_start:
        jmp     multiboot_entry

        /* Align 32 bits boundary. */
        .align  4
        
        /* Multiboot header. */
multiboot_header:
        /* magic */
        .long   MULTIBOOT_HEADER_MAGIC
        /* flags */
        .long   MULTIBOOT_HEADER_FLAGS
        /* checksum */
        .long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
#ifndef __ELF__
        /* header_addr */
        .long   multiboot_header
        /* load_addr */
        .long   _start
        /* load_end_addr */
        .long   _edata
        /* bss_end_addr */
        .long   _end
        /* entry_addr */
        .long   multiboot_entry
#else /* ! __ELF__ */
        .long   0
        .long   0
        .long   0
        .long   0
        .long   0       
#endif /* __ELF__ */
        .long 0
        .long 1024
        .long 768
        .long 32

multiboot_entry:
        /* Initialize the stack pointer. */
        movl    $(stack + STACK_SIZE), %esp

        /* Reset EFLAGS. */
        pushl   $0
        popf

        /* Push the pointer to the Multiboot information structure. */
        pushl   %ebx
        /* Push the magic value. */
        pushl   %eax

        /* Now enter the C main function... */
        call    EXT_C(cmain)

        /* Halt. */
        pushl   $halt_message
        call    EXT_C(printf)
        
loop:   hlt
        jmp     loop

halt_message:
        .asciz  "Halted."

        /* Our stack area. */
        .comm   stack, STACK_SIZE
```

###### 4.3.3 kernel.c

And, in the file `kernel.c`:

```c
/* kernel.c - the C part of the kernel */
/* Copyright (C) 1999, 2010  Free Software Foundation, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <multiboot.h>

/* Macros. */

/* Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

/* Some screen stuff. */
/* The number of columns. */
#define COLUMNS                 80
/* The number of lines. */
#define LINES                   24
/* The attribute of an character. */
#define ATTRIBUTE               7
/* The video memory address. */
#define VIDEO                   0xB8000

/* Variables. */
/* Save the X position. */
static int xpos;
/* Save the Y position. */
static int ypos;
/* Point to the video memory. */
static volatile unsigned char *video;

/* Forward declarations. */
void cmain (unsigned long magic, unsigned long addr);
static void cls (void);
static void itoa (char *buf, int base, int d);
static void putchar (int c);
void printf (const char *format, ...);

/* Check if MAGIC is valid and print the Multiboot information structure
   pointed by ADDR. */
void
cmain (unsigned long magic, unsigned long addr)
{
  multiboot_info_t *mbi;
  
  /* Clear the screen. */
  cls ();

  /* Am I booted by a Multiboot-compliant boot loader? */
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
      printf ("Invalid magic number: 0x%x\n", (unsigned) magic);
      return;
    }

  /* Set MBI to the address of the Multiboot information structure. */
  mbi = (multiboot_info_t *) addr;

  /* Print out the flags. */
  printf ("flags = 0x%x\n", (unsigned) mbi->flags);

  /* Are mem_* valid? */
  if (CHECK_FLAG (mbi->flags, 0))
    printf ("mem_lower = %uKB, mem_upper = %uKB\n",
            (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

  /* Is boot_device valid? */
  if (CHECK_FLAG (mbi->flags, 1))
    printf ("boot_device = 0x%x\n", (unsigned) mbi->boot_device);
  
  /* Is the command line passed? */
  if (CHECK_FLAG (mbi->flags, 2))
    printf ("cmdline = %s\n", (char *) mbi->cmdline);

  /* Are mods_* valid? */
  if (CHECK_FLAG (mbi->flags, 3))
    {
      multiboot_module_t *mod;
      int i;
      
      printf ("mods_count = %d, mods_addr = 0x%x\n",
              (int) mbi->mods_count, (int) mbi->mods_addr);
      for (i = 0, mod = (multiboot_module_t *) mbi->mods_addr;
           i < mbi->mods_count;
           i++, mod++)
        printf (" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
                (unsigned) mod->mod_start,
                (unsigned) mod->mod_end,
                (char *) mod->cmdline);
    }

  /* Bits 4 and 5 are mutually exclusive! */
  if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
    {
      printf ("Both bits 4 and 5 are set.\n");
      return;
    }

  /* Is the symbol table of a.out valid? */
  if (CHECK_FLAG (mbi->flags, 4))
    {
      multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);
      
      printf ("multiboot_aout_symbol_table: tabsize = 0x%0x, "
              "strsize = 0x%x, addr = 0x%x\n",
              (unsigned) multiboot_aout_sym->tabsize,
              (unsigned) multiboot_aout_sym->strsize,
              (unsigned) multiboot_aout_sym->addr);
    }

  /* Is the section header table of ELF valid? */
  if (CHECK_FLAG (mbi->flags, 5))
    {
      multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

      printf ("multiboot_elf_sec: num = %u, size = 0x%x,"
              " addr = 0x%x, shndx = 0x%x\n",
              (unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
              (unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->shndx);
    }

  /* Are mmap_* valid? */
  if (CHECK_FLAG (mbi->flags, 6))
    {
      multiboot_memory_map_t *mmap;
      
      printf ("mmap_addr = 0x%x, mmap_length = 0x%x\n",
              (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
      for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
           (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
           mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
                                    + mmap->size + sizeof (mmap->size)))
        printf (" size = 0x%x, base_addr = 0x%x%08x,"
                " length = 0x%x%08x, type = 0x%x\n",
                (unsigned) mmap->size,
                (unsigned) (mmap->addr >> 32),
                (unsigned) (mmap->addr & 0xffffffff),
                (unsigned) (mmap->len >> 32),
                (unsigned) (mmap->len & 0xffffffff),
                (unsigned) mmap->type);
    }

  /* Draw diagonal blue line. */
  if (CHECK_FLAG (mbi->flags, 12))
    {
      multiboot_uint32_t color;
      unsigned i;
      void *fb = (void *) (unsigned long) mbi->framebuffer_addr;

      switch (mbi->framebuffer_type)
        {
        case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
          {
            unsigned best_distance, distance;
            struct multiboot_color *palette;
            
            palette = (struct multiboot_color *) mbi->framebuffer_palette_addr;

            color = 0;
            best_distance = 4*256*256;
            
            for (i = 0; i < mbi->framebuffer_palette_num_colors; i++)
              {
                distance = (0xff - palette[i].blue) * (0xff - palette[i].blue)
                  + palette[i].red * palette[i].red
                  + palette[i].green * palette[i].green;
                if (distance < best_distance)
                  {
                    color = i;
                    best_distance = distance;
                  }
              }
          }
          break;

        case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
          color = ((1 << mbi->framebuffer_blue_mask_size) - 1) 
            << mbi->framebuffer_blue_field_position;
          break;

        case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
          color = '\\' | 0x0100;
          break;

        default:
          color = 0xffffffff;
          break;
        }
      for (i = 0; i < mbi->framebuffer_width
             && i < mbi->framebuffer_height; i++)
        {
          switch (mbi->framebuffer_bpp)
            {
            case 8:
              {
                multiboot_uint8_t *pixel = fb + mbi->framebuffer_pitch * i + i;
                *pixel = color;
              }
              break;
            case 15:
            case 16:
              {
                multiboot_uint16_t *pixel
                  = fb + mbi->framebuffer_pitch * i + 2 * i;
                *pixel = color;
              }
              break;
            case 24:
              {
                multiboot_uint32_t *pixel
                  = fb + mbi->framebuffer_pitch * i + 3 * i;
                *pixel = (color & 0xffffff) | (*pixel & 0xff000000);
              }
              break;

            case 32:
              {
                multiboot_uint32_t *pixel
                  = fb + mbi->framebuffer_pitch * i + 4 * i;
                *pixel = color;
              }
              break;
            }
        }
    }

}    

/* Clear the screen and initialize VIDEO, XPOS and YPOS. */
static void
cls (void)
{
  int i;

  video = (unsigned char *) VIDEO;
  
  for (i = 0; i < COLUMNS * LINES * 2; i++)
    *(video + i) = 0;

  xpos = 0;
  ypos = 0;
}

/* Convert the integer D to a string and save the string in BUF. If
   BASE is equal to ’d’, interpret that D is decimal, and if BASE is
   equal to ’x’, interpret that D is hexadecimal. */
static void
itoa (char *buf, int base, int d)
{
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = d;
  int divisor = 10;
  
  /* If %d is specified and D is minus, put ‘-’ in the head. */
  if (base == 'd' && d < 0)
    {
      *p++ = '-';
      buf++;
      ud = -d;
    }
  else if (base == 'x')
    divisor = 16;

  /* Divide UD by DIVISOR until UD == 0. */
  do
    {
      int remainder = ud % divisor;
      
      *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
  while (ud /= divisor);

  /* Terminate BUF. */
  *p = 0;
  
  /* Reverse BUF. */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2)
    {
      char tmp = *p1;
      *p1 = *p2;
      *p2 = tmp;
      p1++;
      p2--;
    }
}

/* Put the character C on the screen. */
static void
putchar (int c)
{
  if (c == '\n' || c == '\r')
    {
    newline:
      xpos = 0;
      ypos++;
      if (ypos >= LINES)
        ypos = 0;
      return;
    }

  *(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
  *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;

  xpos++;
  if (xpos >= COLUMNS)
    goto newline;
}

/* Format a string and print it on the screen, just like the libc
   function printf. */
void
printf (const char *format, ...)
{
  char **arg = (char **) &format;
  int c;
  char buf[20];

  arg++;
  
  while ((c = *format++) != 0)
    {
      if (c != '%')
        putchar (c);
      else
        {
          char *p, *p2;
          int pad0 = 0, pad = 0;
          
          c = *format++;
          if (c == '0')
            {
              pad0 = 1;
              c = *format++;
            }

          if (c >= '0' && c <= '9')
            {
              pad = c - '0';
              c = *format++;
            }

          switch (c)
            {
            case 'd':
            case 'u':
            case 'x':
              itoa (buf, c, *((int *) arg++));
              p = buf;
              goto string;
              break;

            case 's':
              p = *arg++;
              if (! p)
                p = "(null)";

            string:
              for (p2 = p; *p2; p2++);
              for (; p2 < p + pad; p2++)
                putchar (pad0 ? '0' : ' ');
              while (*p)
                putchar (*p++);
              break;

            default:
              putchar (*((int *) arg++));
              break;
            }
        }
    }
}
```

###### 4.3.4 Other Multiboot kernels

Multiboot kernels 中应该提供有其他有用的信息。例如 GNU Mach 和 Fiasco http://os.inf.tu-dresden.de/fiasco/。而且，值得一提的是 OSKit http://www.cs.utah.edu/projects/flux/oskit/，它提供了一个支持规范的库。

##### 4.4 Example boot loader code

GNU GRUB（see [GRUB](http://www.gnu.org/software/grub/manual/grub.html#Top) in The GRUB manual）项目是一个兼容 Multiboot-compliant 的 boot loader，支持本规范中存在的所有必需和许多可选功能。尚未发布公开版本，但可从以下位置获取测试版本：

[ftp://alpha.gnu.org/gnu/grub](ftp://alpha.gnu.org/gnu/grub)

有关更多信息，请参见网页 http://www.gnu.org/software/grub/grub.html。

#### 5 本规范的变更日志（The change log of this specification）

- 0.7

  *Multiboot Standard* is renamed to *Multiboot Specification*.Graphics fields are added to Multiboot header.BIOS drive information, BIOS configuration table, the name of a boot loader, APM information, and graphics information are added to Multiboot information.Rewritten in Texinfo format.Rewritten, using more strict words.The maintainer changes to the GNU GRUB maintainer team [bug-grub@gnu.org](mailto:bug-grub@gnu.org), from Bryan Ford and Erich Stefan Boleyn.The byte order of the ‘boot_device’ in Multiboot information is reversed. This was a mistake.The offset of the address fields were wrong.The format is adapted to a newer Texinfo, and the version number is specified more explicitly in the title.

- 0.6

  A few wording changes.Header checksum.Classification of machine state passed to an operating system.

- 0.5

  Name change.

- 0.4

  Major changes plus HTMLification.

#### 6 调用 mbchk（Invoking mbchk）

程序 `mbchk` 检查 Multiboot kernel 的格式。我们建议在引导您自己的内核之前使用该程序。

`mbchk` 接受以下选项：

* --help

  Print a summary of the command-line options and exit.

* --version

  Print the version number of GRUB and exit.

* --quiet

  Suppress all normal output.
