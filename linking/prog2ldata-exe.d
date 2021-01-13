
prog2l:     file format elf64-x86-64
prog2l
architecture: i386:x86-64, flags 0x00000112:
EXEC_P, HAS_SYMS, D_PAGED
start address 0x0000000000400560

Program Header:
    PHDR off    0x0000000000000040 vaddr 0x0000000000400040 paddr 0x0000000000400040 align 2**3
         filesz 0x00000000000001f8 memsz 0x00000000000001f8 flags r-x
  INTERP off    0x0000000000000238 vaddr 0x0000000000400238 paddr 0x0000000000400238 align 2**0
         filesz 0x000000000000001c memsz 0x000000000000001c flags r--
    LOAD off    0x0000000000000000 vaddr 0x0000000000400000 paddr 0x0000000000400000 align 2**21
         filesz 0x0000000000000854 memsz 0x0000000000000854 flags r-x
    LOAD off    0x0000000000000e00 vaddr 0x0000000000600e00 paddr 0x0000000000600e00 align 2**21
         filesz 0x000000000000024c memsz 0x0000000000000258 flags rw-
 DYNAMIC off    0x0000000000000e18 vaddr 0x0000000000600e18 paddr 0x0000000000600e18 align 2**3
         filesz 0x00000000000001e0 memsz 0x00000000000001e0 flags rw-
    NOTE off    0x0000000000000254 vaddr 0x0000000000400254 paddr 0x0000000000400254 align 2**2
         filesz 0x0000000000000044 memsz 0x0000000000000044 flags r--
EH_FRAME off    0x0000000000000730 vaddr 0x0000000000400730 paddr 0x0000000000400730 align 2**2
         filesz 0x0000000000000034 memsz 0x0000000000000034 flags r--
   STACK off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**4
         filesz 0x0000000000000000 memsz 0x0000000000000000 flags rw-
   RELRO off    0x0000000000000e00 vaddr 0x0000000000600e00 paddr 0x0000000000600e00 align 2**0
         filesz 0x0000000000000200 memsz 0x0000000000000200 flags r--

Dynamic Section:
  NEEDED               ./libvector.so
  NEEDED               libc.so.6
  INIT                 0x00000000004004e8
  FINI                 0x0000000000400704
  INIT_ARRAY           0x0000000000600e00
  INIT_ARRAYSZ         0x0000000000000008
  FINI_ARRAY           0x0000000000600e08
  FINI_ARRAYSZ         0x0000000000000008
  GNU_HASH             0x0000000000400298
  STRTAB               0x00000000004003c0
  SYMTAB               0x00000000004002d0
  STRSZ                0x0000000000000079
  SYMENT               0x0000000000000018
  DEBUG                0x0000000000000000
  PLTGOT               0x0000000000601000
  PLTRELSZ             0x0000000000000060
  PLTREL               0x0000000000000007
  JMPREL               0x0000000000400488
  RELA                 0x0000000000400470
  RELASZ               0x0000000000000018
  RELAENT              0x0000000000000018
  VERNEED              0x0000000000400450
  VERNEEDNUM           0x0000000000000001
  VERSYM               0x000000000040043a

Version References:
  required from libc.so.6:
    0x09691a75 0x00 02 GLIBC_2.2.5

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
 21 .got          00000008  0000000000600ff8  0000000000600ff8  00000ff8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 23 .data         00000014  0000000000601038  0000000000601038  00001038  2**2
                  CONTENTS, ALLOC, LOAD, DATA
SYMBOL TABLE:
0000000000600ff8 l    d  .got	0000000000000000              .got
0000000000601038 l    d  .data	0000000000000000              .data
0000000000601038  w      .data	0000000000000000              data_start
000000000060104c g       .data	0000000000000000              _edata
0000000000601044 g     O .data	0000000000000008              x
0000000000601038 g       .data	0000000000000000              __data_start
000000000060103c g     O .data	0000000000000008              y
0000000000601050 g     O .data	0000000000000000              .hidden __TMC_END__


Contents of section .got:
 600ff8 00000000 00000000                    ........        
Contents of section .data:
 601038 00000000 03000000 04000000 01000000  ................
 601048 02000000                             ....            
