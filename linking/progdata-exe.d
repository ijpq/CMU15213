
prog:     file format elf64-x86-64
prog
architecture: i386:x86-64, flags 0x00000112:
EXEC_P, HAS_SYMS, D_PAGED
start address 0x0000000000400400

Program Header:
    PHDR off    0x0000000000000040 vaddr 0x0000000000400040 paddr 0x0000000000400040 align 2**3
         filesz 0x00000000000001f8 memsz 0x00000000000001f8 flags r-x
  INTERP off    0x0000000000000238 vaddr 0x0000000000400238 paddr 0x0000000000400238 align 2**0
         filesz 0x000000000000001c memsz 0x000000000000001c flags r--
    LOAD off    0x0000000000000000 vaddr 0x0000000000400000 paddr 0x0000000000400000 align 2**21
         filesz 0x00000000000006f4 memsz 0x00000000000006f4 flags r-x
    LOAD off    0x0000000000000e10 vaddr 0x0000000000600e10 paddr 0x0000000000600e10 align 2**21
         filesz 0x0000000000000224 memsz 0x0000000000000228 flags rw-
 DYNAMIC off    0x0000000000000e28 vaddr 0x0000000000600e28 paddr 0x0000000000600e28 align 2**3
         filesz 0x00000000000001d0 memsz 0x00000000000001d0 flags rw-
    NOTE off    0x0000000000000254 vaddr 0x0000000000400254 paddr 0x0000000000400254 align 2**2
         filesz 0x0000000000000044 memsz 0x0000000000000044 flags r--
EH_FRAME off    0x00000000000005b0 vaddr 0x00000000004005b0 paddr 0x00000000004005b0 align 2**2
         filesz 0x000000000000003c memsz 0x000000000000003c flags r--
   STACK off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**4
         filesz 0x0000000000000000 memsz 0x0000000000000000 flags rw-
   RELRO off    0x0000000000000e10 vaddr 0x0000000000600e10 paddr 0x0000000000600e10 align 2**0
         filesz 0x00000000000001f0 memsz 0x00000000000001f0 flags r--

Dynamic Section:
  NEEDED               libc.so.6
  INIT                 0x00000000004003a8
  FINI                 0x0000000000400594
  INIT_ARRAY           0x0000000000600e10
  INIT_ARRAYSZ         0x0000000000000008
  FINI_ARRAY           0x0000000000600e18
  FINI_ARRAYSZ         0x0000000000000008
  GNU_HASH             0x0000000000400298
  STRTAB               0x0000000000400300
  SYMTAB               0x00000000004002b8
  STRSZ                0x0000000000000038
  SYMENT               0x0000000000000018
  DEBUG                0x0000000000000000
  PLTGOT               0x0000000000601000
  PLTRELSZ             0x0000000000000030
  PLTREL               0x0000000000000007
  JMPREL               0x0000000000400378
  RELA                 0x0000000000400360
  RELASZ               0x0000000000000018
  RELAENT              0x0000000000000018
  VERNEED              0x0000000000400340
  VERNEEDNUM           0x0000000000000001
  VERSYM               0x0000000000400338

Version References:
  required from libc.so.6:
    0x09691a75 0x00 02 GLIBC_2.2.5

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
 23 .data         0000000c  0000000000601028  0000000000601028  00001028  2**2
                  CONTENTS, ALLOC, LOAD, DATA
SYMBOL TABLE:
0000000000601028 l    d  .data	0000000000000000              .data
0000000000601028  w      .data	0000000000000000              data_start
000000000060102c g     O .data	0000000000000008              array
0000000000601034 g       .data	0000000000000000              _edata
0000000000601028 g       .data	0000000000000000              __data_start
0000000000601038 g     O .data	0000000000000000              .hidden __TMC_END__



Disassembly of section .data:

0000000000601028 <__data_start>:
  601028:	00 00                	add    %al,(%rax)
	...

000000000060102c <array>:
  60102c:	01 00 00 00 02 00 00 00                             ........
