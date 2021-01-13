
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
  0 .interp       0000001c  0000000000400238  0000000000400238  00000238  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  1 .note.ABI-tag 00000020  0000000000400254  0000000000400254  00000254  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  2 .note.gnu.build-id 00000024  0000000000400274  0000000000400274  00000274  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  3 .gnu.hash     00000038  0000000000400298  0000000000400298  00000298  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .dynsym       000000f0  00000000004002d0  00000000004002d0  000002d0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  5 .dynstr       00000079  00000000004003c0  00000000004003c0  000003c0  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  6 .gnu.version  00000014  000000000040043a  000000000040043a  0000043a  2**1
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  7 .gnu.version_r 00000020  0000000000400450  0000000000400450  00000450  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  8 .rela.dyn     00000018  0000000000400470  0000000000400470  00000470  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  9 .rela.plt     00000060  0000000000400488  0000000000400488  00000488  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 10 .init         0000001a  00000000004004e8  00000000004004e8  000004e8  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 11 .plt          00000050  0000000000400510  0000000000400510  00000510  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 12 .text         000001a2  0000000000400560  0000000000400560  00000560  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 13 .fini         00000009  0000000000400704  0000000000400704  00000704  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 14 .rodata       0000001d  0000000000400710  0000000000400710  00000710  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 15 .eh_frame_hdr 00000034  0000000000400730  0000000000400730  00000730  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 16 .eh_frame     000000ec  0000000000400768  0000000000400768  00000768  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 17 .init_array   00000008  0000000000600e00  0000000000600e00  00000e00  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 18 .fini_array   00000008  0000000000600e08  0000000000600e08  00000e08  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 19 .jcr          00000008  0000000000600e10  0000000000600e10  00000e10  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 20 .dynamic      000001e0  0000000000600e18  0000000000600e18  00000e18  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 21 .got          00000008  0000000000600ff8  0000000000600ff8  00000ff8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 22 .got.plt      00000038  0000000000601000  0000000000601000  00001000  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 23 .data         00000014  0000000000601038  0000000000601038  00001038  2**2
                  CONTENTS, ALLOC, LOAD, DATA
 24 .bss          0000000c  000000000060104c  000000000060104c  0000104c  2**2
                  ALLOC
 25 .comment      0000002d  0000000000000000  0000000000000000  0000104c  2**0
                  CONTENTS, READONLY
SYMBOL TABLE:
0000000000400238 l    d  .interp	0000000000000000              .interp
0000000000400254 l    d  .note.ABI-tag	0000000000000000              .note.ABI-tag
0000000000400274 l    d  .note.gnu.build-id	0000000000000000              .note.gnu.build-id
0000000000400298 l    d  .gnu.hash	0000000000000000              .gnu.hash
00000000004002d0 l    d  .dynsym	0000000000000000              .dynsym
00000000004003c0 l    d  .dynstr	0000000000000000              .dynstr
000000000040043a l    d  .gnu.version	0000000000000000              .gnu.version
0000000000400450 l    d  .gnu.version_r	0000000000000000              .gnu.version_r
0000000000400470 l    d  .rela.dyn	0000000000000000              .rela.dyn
0000000000400488 l    d  .rela.plt	0000000000000000              .rela.plt
00000000004004e8 l    d  .init	0000000000000000              .init
0000000000400510 l    d  .plt	0000000000000000              .plt
0000000000400560 l    d  .text	0000000000000000              .text
0000000000400704 l    d  .fini	0000000000000000              .fini
0000000000400710 l    d  .rodata	0000000000000000              .rodata
0000000000400730 l    d  .eh_frame_hdr	0000000000000000              .eh_frame_hdr
0000000000400768 l    d  .eh_frame	0000000000000000              .eh_frame
0000000000600e00 l    d  .init_array	0000000000000000              .init_array
0000000000600e08 l    d  .fini_array	0000000000000000              .fini_array
0000000000600e10 l    d  .jcr	0000000000000000              .jcr
0000000000600e18 l    d  .dynamic	0000000000000000              .dynamic
0000000000600ff8 l    d  .got	0000000000000000              .got
0000000000601000 l    d  .got.plt	0000000000000000              .got.plt
0000000000601038 l    d  .data	0000000000000000              .data
000000000060104c l    d  .bss	0000000000000000              .bss
0000000000000000 l    d  .comment	0000000000000000              .comment
0000000000000000 l    df *ABS*	0000000000000000              crtstuff.c
0000000000600e10 l     O .jcr	0000000000000000              __JCR_LIST__
0000000000400590 l     F .text	0000000000000000              deregister_tm_clones
00000000004005c0 l     F .text	0000000000000000              register_tm_clones
0000000000400600 l     F .text	0000000000000000              __do_global_dtors_aux
000000000060104c l     O .bss	0000000000000001              completed.6355
0000000000600e08 l     O .fini_array	0000000000000000              __do_global_dtors_aux_fini_array_entry
0000000000400620 l     F .text	0000000000000000              frame_dummy
0000000000600e00 l     O .init_array	0000000000000000              __frame_dummy_init_array_entry
0000000000000000 l    df *ABS*	0000000000000000              main2.c
0000000000000000 l    df *ABS*	0000000000000000              crtstuff.c
0000000000400850 l     O .eh_frame	0000000000000000              __FRAME_END__
0000000000600e10 l     O .jcr	0000000000000000              __JCR_END__
0000000000000000 l    df *ABS*	0000000000000000              
0000000000600e08 l       .init_array	0000000000000000              __init_array_end
0000000000600e18 l     O .dynamic	0000000000000000              _DYNAMIC
0000000000600e00 l       .init_array	0000000000000000              __init_array_start
0000000000400730 l       .eh_frame_hdr	0000000000000000              __GNU_EH_FRAME_HDR
0000000000601000 l     O .got.plt	0000000000000000              _GLOBAL_OFFSET_TABLE_
0000000000400700 g     F .text	0000000000000002              __libc_csu_fini
0000000000601038  w      .data	0000000000000000              data_start
000000000060104c g       .data	0000000000000000              _edata
0000000000601050 g     O .bss	0000000000000008              z
0000000000601044 g     O .data	0000000000000008              x
0000000000400704 g     F .fini	0000000000000000              _fini
0000000000000000       F *UND*	0000000000000000              printf@@GLIBC_2.2.5
0000000000000000       F *UND*	0000000000000000              __libc_start_main@@GLIBC_2.2.5
0000000000000000       F *UND*	0000000000000000              addvec
0000000000601038 g       .data	0000000000000000              __data_start
0000000000000000  w      *UND*	0000000000000000              __gmon_start__
0000000000400718 g     O .rodata	0000000000000000              .hidden __dso_handle
0000000000400710 g     O .rodata	0000000000000004              _IO_stdin_used
0000000000400690 g     F .text	0000000000000065              __libc_csu_init
0000000000601058 g       .bss	0000000000000000              _end
0000000000400560 g     F .text	0000000000000000              _start
000000000060103c g     O .data	0000000000000008              y
000000000060104c g       .bss	0000000000000000              __bss_start
000000000040064d g     F .text	0000000000000042              main
0000000000601050 g     O .data	0000000000000000              .hidden __TMC_END__
00000000004004e8 g     F .init	0000000000000000              _init



Disassembly of section .init:

00000000004004e8 <_init>:
  4004e8:	48 83 ec 08          	sub    $0x8,%rsp
  4004ec:	48 8b 05 05 0b 20 00 	mov    0x200b05(%rip),%rax        # 600ff8 <__gmon_start__>
  4004f3:	48 85 c0             	test   %rax,%rax
  4004f6:	74 05                	je     4004fd <_init+0x15>
  4004f8:	e8 53 00 00 00       	callq  400550 <__gmon_start__@plt>
  4004fd:	48 83 c4 08          	add    $0x8,%rsp
  400501:	c3                   	retq   

Disassembly of section .plt:

0000000000400510 <.plt>:
  400510:	ff 35 f2 0a 20 00    	pushq  0x200af2(%rip)        # 601008 <_GLOBAL_OFFSET_TABLE_+0x8>
  400516:	ff 25 f4 0a 20 00    	jmpq   *0x200af4(%rip)        # 601010 <_GLOBAL_OFFSET_TABLE_+0x10>
  40051c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400520 <printf@plt>:
  400520:	ff 25 f2 0a 20 00    	jmpq   *0x200af2(%rip)        # 601018 <printf@GLIBC_2.2.5>
  400526:	68 00 00 00 00       	pushq  $0x0
  40052b:	e9 e0 ff ff ff       	jmpq   400510 <.plt>

0000000000400530 <__libc_start_main@plt>:
  400530:	ff 25 ea 0a 20 00    	jmpq   *0x200aea(%rip)        # 601020 <__libc_start_main@GLIBC_2.2.5>
  400536:	68 01 00 00 00       	pushq  $0x1
  40053b:	e9 d0 ff ff ff       	jmpq   400510 <.plt>

0000000000400540 <addvec@plt>:
  400540:	ff 25 e2 0a 20 00    	jmpq   *0x200ae2(%rip)        # 601028 <addvec>
  400546:	68 02 00 00 00       	pushq  $0x2
  40054b:	e9 c0 ff ff ff       	jmpq   400510 <.plt>

0000000000400550 <__gmon_start__@plt>:
  400550:	ff 25 da 0a 20 00    	jmpq   *0x200ada(%rip)        # 601030 <__gmon_start__>
  400556:	68 03 00 00 00       	pushq  $0x3
  40055b:	e9 b0 ff ff ff       	jmpq   400510 <.plt>

Disassembly of section .text:

0000000000400560 <_start>:
  400560:	31 ed                	xor    %ebp,%ebp
  400562:	49 89 d1             	mov    %rdx,%r9
  400565:	5e                   	pop    %rsi
  400566:	48 89 e2             	mov    %rsp,%rdx
  400569:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  40056d:	50                   	push   %rax
  40056e:	54                   	push   %rsp
  40056f:	49 c7 c0 00 07 40 00 	mov    $0x400700,%r8
  400576:	48 c7 c1 90 06 40 00 	mov    $0x400690,%rcx
  40057d:	48 c7 c7 4d 06 40 00 	mov    $0x40064d,%rdi
  400584:	e8 a7 ff ff ff       	callq  400530 <__libc_start_main@plt>
  400589:	f4                   	hlt    
  40058a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000400590 <deregister_tm_clones>:
  400590:	b8 57 10 60 00       	mov    $0x601057,%eax
  400595:	55                   	push   %rbp
  400596:	48 2d 50 10 60 00    	sub    $0x601050,%rax
  40059c:	48 83 f8 0e          	cmp    $0xe,%rax
  4005a0:	48 89 e5             	mov    %rsp,%rbp
  4005a3:	77 02                	ja     4005a7 <deregister_tm_clones+0x17>
  4005a5:	5d                   	pop    %rbp
  4005a6:	c3                   	retq   
  4005a7:	b8 00 00 00 00       	mov    $0x0,%eax
  4005ac:	48 85 c0             	test   %rax,%rax
  4005af:	74 f4                	je     4005a5 <deregister_tm_clones+0x15>
  4005b1:	5d                   	pop    %rbp
  4005b2:	bf 50 10 60 00       	mov    $0x601050,%edi
  4005b7:	ff e0                	jmpq   *%rax
  4005b9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000004005c0 <register_tm_clones>:
  4005c0:	b8 50 10 60 00       	mov    $0x601050,%eax
  4005c5:	55                   	push   %rbp
  4005c6:	48 2d 50 10 60 00    	sub    $0x601050,%rax
  4005cc:	48 c1 f8 03          	sar    $0x3,%rax
  4005d0:	48 89 e5             	mov    %rsp,%rbp
  4005d3:	48 89 c2             	mov    %rax,%rdx
  4005d6:	48 c1 ea 3f          	shr    $0x3f,%rdx
  4005da:	48 01 d0             	add    %rdx,%rax
  4005dd:	48 d1 f8             	sar    %rax
  4005e0:	75 02                	jne    4005e4 <register_tm_clones+0x24>
  4005e2:	5d                   	pop    %rbp
  4005e3:	c3                   	retq   
  4005e4:	ba 00 00 00 00       	mov    $0x0,%edx
  4005e9:	48 85 d2             	test   %rdx,%rdx
  4005ec:	74 f4                	je     4005e2 <register_tm_clones+0x22>
  4005ee:	5d                   	pop    %rbp
  4005ef:	48 89 c6             	mov    %rax,%rsi
  4005f2:	bf 50 10 60 00       	mov    $0x601050,%edi
  4005f7:	ff e2                	jmpq   *%rdx
  4005f9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000400600 <__do_global_dtors_aux>:
  400600:	80 3d 45 0a 20 00 00 	cmpb   $0x0,0x200a45(%rip)        # 60104c <_edata>
  400607:	75 11                	jne    40061a <__do_global_dtors_aux+0x1a>
  400609:	55                   	push   %rbp
  40060a:	48 89 e5             	mov    %rsp,%rbp
  40060d:	e8 7e ff ff ff       	callq  400590 <deregister_tm_clones>
  400612:	5d                   	pop    %rbp
  400613:	c6 05 32 0a 20 00 01 	movb   $0x1,0x200a32(%rip)        # 60104c <_edata>
  40061a:	f3 c3                	repz retq 
  40061c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400620 <frame_dummy>:
  400620:	48 83 3d e8 07 20 00 	cmpq   $0x0,0x2007e8(%rip)        # 600e10 <__JCR_END__>
  400627:	00 
  400628:	74 1e                	je     400648 <frame_dummy+0x28>
  40062a:	b8 00 00 00 00       	mov    $0x0,%eax
  40062f:	48 85 c0             	test   %rax,%rax
  400632:	74 14                	je     400648 <frame_dummy+0x28>
  400634:	55                   	push   %rbp
  400635:	bf 10 0e 60 00       	mov    $0x600e10,%edi
  40063a:	48 89 e5             	mov    %rsp,%rbp
  40063d:	ff d0                	callq  *%rax
  40063f:	5d                   	pop    %rbp
  400640:	e9 7b ff ff ff       	jmpq   4005c0 <register_tm_clones>
  400645:	0f 1f 00             	nopl   (%rax)
  400648:	e9 73 ff ff ff       	jmpq   4005c0 <register_tm_clones>

000000000040064d <main>:
  40064d:	48 83 ec 08          	sub    $0x8,%rsp
  400651:	b9 02 00 00 00       	mov    $0x2,%ecx
  400656:	ba 50 10 60 00       	mov    $0x601050,%edx
  40065b:	be 3c 10 60 00       	mov    $0x60103c,%esi
  400660:	bf 44 10 60 00       	mov    $0x601044,%edi
  400665:	e8 d6 fe ff ff       	callq  400540 <addvec@plt>
  40066a:	8b 15 e4 09 20 00    	mov    0x2009e4(%rip),%edx        # 601054 <__TMC_END__+0x4>
  400670:	8b 35 da 09 20 00    	mov    0x2009da(%rip),%esi        # 601050 <__TMC_END__>
  400676:	bf 20 07 40 00       	mov    $0x400720,%edi
  40067b:	b8 00 00 00 00       	mov    $0x0,%eax
  400680:	e8 9b fe ff ff       	callq  400520 <printf@plt>
  400685:	b8 00 00 00 00       	mov    $0x0,%eax
  40068a:	48 83 c4 08          	add    $0x8,%rsp
  40068e:	c3                   	retq   
  40068f:	90                   	nop

0000000000400690 <__libc_csu_init>:
  400690:	41 57                	push   %r15
  400692:	41 89 ff             	mov    %edi,%r15d
  400695:	41 56                	push   %r14
  400697:	49 89 f6             	mov    %rsi,%r14
  40069a:	41 55                	push   %r13
  40069c:	49 89 d5             	mov    %rdx,%r13
  40069f:	41 54                	push   %r12
  4006a1:	4c 8d 25 58 07 20 00 	lea    0x200758(%rip),%r12        # 600e00 <__frame_dummy_init_array_entry>
  4006a8:	55                   	push   %rbp
  4006a9:	48 8d 2d 58 07 20 00 	lea    0x200758(%rip),%rbp        # 600e08 <__init_array_end>
  4006b0:	53                   	push   %rbx
  4006b1:	4c 29 e5             	sub    %r12,%rbp
  4006b4:	31 db                	xor    %ebx,%ebx
  4006b6:	48 c1 fd 03          	sar    $0x3,%rbp
  4006ba:	48 83 ec 08          	sub    $0x8,%rsp
  4006be:	e8 25 fe ff ff       	callq  4004e8 <_init>
  4006c3:	48 85 ed             	test   %rbp,%rbp
  4006c6:	74 1e                	je     4006e6 <__libc_csu_init+0x56>
  4006c8:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  4006cf:	00 
  4006d0:	4c 89 ea             	mov    %r13,%rdx
  4006d3:	4c 89 f6             	mov    %r14,%rsi
  4006d6:	44 89 ff             	mov    %r15d,%edi
  4006d9:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  4006dd:	48 83 c3 01          	add    $0x1,%rbx
  4006e1:	48 39 eb             	cmp    %rbp,%rbx
  4006e4:	75 ea                	jne    4006d0 <__libc_csu_init+0x40>
  4006e6:	48 83 c4 08          	add    $0x8,%rsp
  4006ea:	5b                   	pop    %rbx
  4006eb:	5d                   	pop    %rbp
  4006ec:	41 5c                	pop    %r12
  4006ee:	41 5d                	pop    %r13
  4006f0:	41 5e                	pop    %r14
  4006f2:	41 5f                	pop    %r15
  4006f4:	c3                   	retq   
  4006f5:	90                   	nop
  4006f6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  4006fd:	00 00 00 

0000000000400700 <__libc_csu_fini>:
  400700:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000400704 <_fini>:
  400704:	48 83 ec 08          	sub    $0x8,%rsp
  400708:	48 83 c4 08          	add    $0x8,%rsp
  40070c:	c3                   	retq   
