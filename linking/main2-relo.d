
main2.o:     file format elf64-x86-64
main2.o
architecture: i386:x86-64, flags 0x00000011:
HAS_RELOC, HAS_SYMS
start address 0x0000000000000000

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         00000042  0000000000000000  0000000000000000  00000040  2**0
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  1 .data         00000010  0000000000000000  0000000000000000  00000084  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000000  0000000000000000  0000000000000000  00000094  2**0
                  ALLOC
  3 .rodata.str1.1 0000000d  0000000000000000  0000000000000000  00000094  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .comment      0000002e  0000000000000000  0000000000000000  000000a1  2**0
                  CONTENTS, READONLY
  5 .note.GNU-stack 00000000  0000000000000000  0000000000000000  000000cf  2**0
                  CONTENTS, READONLY
  6 .eh_frame     00000030  0000000000000000  0000000000000000  000000d0  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
SYMBOL TABLE:
0000000000000000 l    df *ABS*	0000000000000000 main2.c
0000000000000000 l    d  .text	0000000000000000 .text
0000000000000000 l    d  .data	0000000000000000 .data
0000000000000000 l    d  .bss	0000000000000000 .bss
0000000000000000 l    d  .rodata.str1.1	0000000000000000 .rodata.str1.1
0000000000000000 l    d  .note.GNU-stack	0000000000000000 .note.GNU-stack
0000000000000000 l    d  .eh_frame	0000000000000000 .eh_frame
0000000000000000 l    d  .comment	0000000000000000 .comment
0000000000000000 g     F .text	0000000000000042 main
0000000000000008       O *COM*	0000000000000004 z
0000000000000000 g     O .data	0000000000000008 y
0000000000000008 g     O .data	0000000000000008 x
0000000000000000         *UND*	0000000000000000 addvec
0000000000000000         *UND*	0000000000000000 printf



Disassembly of section .text:

0000000000000000 <main>:
   0:	48 83 ec 08          	sub    $0x8,%rsp
   4:	b9 02 00 00 00       	mov    $0x2,%ecx
   9:	ba 00 00 00 00       	mov    $0x0,%edx
			a: R_X86_64_32	z
   e:	be 00 00 00 00       	mov    $0x0,%esi
			f: R_X86_64_32	y
  13:	bf 00 00 00 00       	mov    $0x0,%edi
			14: R_X86_64_32	x
  18:	e8 00 00 00 00       	callq  1d <main+0x1d>
			19: R_X86_64_PC32	addvec-0x4
  1d:	8b 15 00 00 00 00    	mov    0x0(%rip),%edx        # 23 <main+0x23>
			1f: R_X86_64_PC32	z
  23:	8b 35 00 00 00 00    	mov    0x0(%rip),%esi        # 29 <main+0x29>
			25: R_X86_64_PC32	z-0x4
  29:	bf 00 00 00 00       	mov    $0x0,%edi
			2a: R_X86_64_32	.rodata.str1.1
  2e:	b8 00 00 00 00       	mov    $0x0,%eax
  33:	e8 00 00 00 00       	callq  38 <main+0x38>
			34: R_X86_64_PC32	printf-0x4
  38:	b8 00 00 00 00       	mov    $0x0,%eax
  3d:	48 83 c4 08          	add    $0x8,%rsp
  41:	c3                   	retq   
