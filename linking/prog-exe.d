
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
  0 .interp       0000001c  0000000000400238  0000000000400238  00000238  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  1 .note.ABI-tag 00000020  0000000000400254  0000000000400254  00000254  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  2 .note.gnu.build-id 00000024  0000000000400274  0000000000400274  00000274  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  3 .gnu.hash     0000001c  0000000000400298  0000000000400298  00000298  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .dynsym       00000048  00000000004002b8  00000000004002b8  000002b8  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  5 .dynstr       00000038  0000000000400300  0000000000400300  00000300  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  6 .gnu.version  00000006  0000000000400338  0000000000400338  00000338  2**1
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  7 .gnu.version_r 00000020  0000000000400340  0000000000400340  00000340  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  8 .rela.dyn     00000018  0000000000400360  0000000000400360  00000360  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  9 .rela.plt     00000030  0000000000400378  0000000000400378  00000378  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 10 .init         0000001a  00000000004003a8  00000000004003a8  000003a8  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 11 .plt          00000030  00000000004003d0  00000000004003d0  000003d0  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 12 .text         00000192  0000000000400400  0000000000400400  00000400  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 13 .fini         00000009  0000000000400594  0000000000400594  00000594  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 14 .rodata       00000010  00000000004005a0  00000000004005a0  000005a0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 15 .eh_frame_hdr 0000003c  00000000004005b0  00000000004005b0  000005b0  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 16 .eh_frame     00000104  00000000004005f0  00000000004005f0  000005f0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 17 .init_array   00000008  0000000000600e10  0000000000600e10  00000e10  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 18 .fini_array   00000008  0000000000600e18  0000000000600e18  00000e18  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 19 .jcr          00000008  0000000000600e20  0000000000600e20  00000e20  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 20 .dynamic      000001d0  0000000000600e28  0000000000600e28  00000e28  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 21 .got          00000008  0000000000600ff8  0000000000600ff8  00000ff8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 22 .got.plt      00000028  0000000000601000  0000000000601000  00001000  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 23 .data         0000000c  0000000000601028  0000000000601028  00001028  2**2
                  CONTENTS, ALLOC, LOAD, DATA
 24 .bss          00000004  0000000000601034  0000000000601034  00001034  2**0
                  ALLOC
 25 .comment      0000002d  0000000000000000  0000000000000000  00001034  2**0
                  CONTENTS, READONLY
SYMBOL TABLE:
0000000000400238 l    d  .interp	0000000000000000              .interp
0000000000400254 l    d  .note.ABI-tag	0000000000000000              .note.ABI-tag
0000000000400274 l    d  .note.gnu.build-id	0000000000000000              .note.gnu.build-id
0000000000400298 l    d  .gnu.hash	0000000000000000              .gnu.hash
00000000004002b8 l    d  .dynsym	0000000000000000              .dynsym
0000000000400300 l    d  .dynstr	0000000000000000              .dynstr
0000000000400338 l    d  .gnu.version	0000000000000000              .gnu.version
0000000000400340 l    d  .gnu.version_r	0000000000000000              .gnu.version_r
0000000000400360 l    d  .rela.dyn	0000000000000000              .rela.dyn
0000000000400378 l    d  .rela.plt	0000000000000000              .rela.plt
00000000004003a8 l    d  .init	0000000000000000              .init
00000000004003d0 l    d  .plt	0000000000000000              .plt
0000000000400400 l    d  .text	0000000000000000              .text
0000000000400594 l    d  .fini	0000000000000000              .fini
00000000004005a0 l    d  .rodata	0000000000000000              .rodata
00000000004005b0 l    d  .eh_frame_hdr	0000000000000000              .eh_frame_hdr
00000000004005f0 l    d  .eh_frame	0000000000000000              .eh_frame
0000000000600e10 l    d  .init_array	0000000000000000              .init_array
0000000000600e18 l    d  .fini_array	0000000000000000              .fini_array
0000000000600e20 l    d  .jcr	0000000000000000              .jcr
0000000000600e28 l    d  .dynamic	0000000000000000              .dynamic
0000000000600ff8 l    d  .got	0000000000000000              .got
0000000000601000 l    d  .got.plt	0000000000000000              .got.plt
0000000000601028 l    d  .data	0000000000000000              .data
0000000000601034 l    d  .bss	0000000000000000              .bss
0000000000000000 l    d  .comment	0000000000000000              .comment
0000000000000000 l    df *ABS*	0000000000000000              crtstuff.c
0000000000600e20 l     O .jcr	0000000000000000              __JCR_LIST__
0000000000400430 l     F .text	0000000000000000              deregister_tm_clones
0000000000400460 l     F .text	0000000000000000              register_tm_clones
00000000004004a0 l     F .text	0000000000000000              __do_global_dtors_aux
0000000000601034 l     O .bss	0000000000000001              completed.6355
0000000000600e18 l     O .fini_array	0000000000000000              __do_global_dtors_aux_fini_array_entry
00000000004004c0 l     F .text	0000000000000000              frame_dummy
0000000000600e10 l     O .init_array	0000000000000000              __frame_dummy_init_array_entry
0000000000000000 l    df *ABS*	0000000000000000              main.c
0000000000000000 l    df *ABS*	0000000000000000              sum.c
0000000000000000 l    df *ABS*	0000000000000000              crtstuff.c
00000000004006f0 l     O .eh_frame	0000000000000000              __FRAME_END__
0000000000600e20 l     O .jcr	0000000000000000              __JCR_END__
0000000000000000 l    df *ABS*	0000000000000000              
0000000000600e18 l       .init_array	0000000000000000              __init_array_end
0000000000600e28 l     O .dynamic	0000000000000000              _DYNAMIC
0000000000600e10 l       .init_array	0000000000000000              __init_array_start
00000000004005b0 l       .eh_frame_hdr	0000000000000000              __GNU_EH_FRAME_HDR
0000000000601000 l     O .got.plt	0000000000000000              _GLOBAL_OFFSET_TABLE_
0000000000400590 g     F .text	0000000000000002              __libc_csu_fini
0000000000601028  w      .data	0000000000000000              data_start
000000000060102c g     O .data	0000000000000008              array
0000000000601034 g       .data	0000000000000000              _edata
0000000000400594 g     F .fini	0000000000000000              _fini
0000000000000000       F *UND*	0000000000000000              __libc_start_main@@GLIBC_2.2.5
0000000000601028 g       .data	0000000000000000              __data_start
0000000000000000  w      *UND*	0000000000000000              __gmon_start__
00000000004005a8 g     O .rodata	0000000000000000              .hidden __dso_handle
0000000000400505 g     F .text	000000000000001b              sum
00000000004005a0 g     O .rodata	0000000000000004              _IO_stdin_used
0000000000400520 g     F .text	0000000000000065              __libc_csu_init
0000000000601038 g       .bss	0000000000000000              _end
0000000000400400 g     F .text	0000000000000000              _start
0000000000601034 g       .bss	0000000000000000              __bss_start
00000000004004ed g     F .text	0000000000000018              main
0000000000601038 g     O .data	0000000000000000              .hidden __TMC_END__
00000000004003a8 g     F .init	0000000000000000              _init



Disassembly of section .init:

00000000004003a8 <_init>:
  4003a8:	48 83 ec 08          	sub    $0x8,%rsp
  4003ac:	48 8b 05 45 0c 20 00 	mov    0x200c45(%rip),%rax        # 600ff8 <__gmon_start__>
  4003b3:	48 85 c0             	test   %rax,%rax
  4003b6:	74 05                	je     4003bd <_init+0x15>
  4003b8:	e8 33 00 00 00       	callq  4003f0 <__gmon_start__@plt>
  4003bd:	48 83 c4 08          	add    $0x8,%rsp
  4003c1:	c3                   	retq   

Disassembly of section .plt:

00000000004003d0 <.plt>:
  4003d0:	ff 35 32 0c 20 00    	pushq  0x200c32(%rip)        # 601008 <_GLOBAL_OFFSET_TABLE_+0x8>
  4003d6:	ff 25 34 0c 20 00    	jmpq   *0x200c34(%rip)        # 601010 <_GLOBAL_OFFSET_TABLE_+0x10>
  4003dc:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004003e0 <__libc_start_main@plt>:
  4003e0:	ff 25 32 0c 20 00    	jmpq   *0x200c32(%rip)        # 601018 <__libc_start_main@GLIBC_2.2.5>
  4003e6:	68 00 00 00 00       	pushq  $0x0
  4003eb:	e9 e0 ff ff ff       	jmpq   4003d0 <.plt>

00000000004003f0 <__gmon_start__@plt>:
  4003f0:	ff 25 2a 0c 20 00    	jmpq   *0x200c2a(%rip)        # 601020 <__gmon_start__>
  4003f6:	68 01 00 00 00       	pushq  $0x1
  4003fb:	e9 d0 ff ff ff       	jmpq   4003d0 <.plt>

Disassembly of section .text:

0000000000400400 <_start>:
  400400:	31 ed                	xor    %ebp,%ebp
  400402:	49 89 d1             	mov    %rdx,%r9
  400405:	5e                   	pop    %rsi
  400406:	48 89 e2             	mov    %rsp,%rdx
  400409:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  40040d:	50                   	push   %rax
  40040e:	54                   	push   %rsp
  40040f:	49 c7 c0 90 05 40 00 	mov    $0x400590,%r8
  400416:	48 c7 c1 20 05 40 00 	mov    $0x400520,%rcx
  40041d:	48 c7 c7 ed 04 40 00 	mov    $0x4004ed,%rdi
  400424:	e8 b7 ff ff ff       	callq  4003e0 <__libc_start_main@plt>
  400429:	f4                   	hlt    
  40042a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000400430 <deregister_tm_clones>:
  400430:	b8 3f 10 60 00       	mov    $0x60103f,%eax
  400435:	55                   	push   %rbp
  400436:	48 2d 38 10 60 00    	sub    $0x601038,%rax
  40043c:	48 83 f8 0e          	cmp    $0xe,%rax
  400440:	48 89 e5             	mov    %rsp,%rbp
  400443:	77 02                	ja     400447 <deregister_tm_clones+0x17>
  400445:	5d                   	pop    %rbp
  400446:	c3                   	retq   
  400447:	b8 00 00 00 00       	mov    $0x0,%eax
  40044c:	48 85 c0             	test   %rax,%rax
  40044f:	74 f4                	je     400445 <deregister_tm_clones+0x15>
  400451:	5d                   	pop    %rbp
  400452:	bf 38 10 60 00       	mov    $0x601038,%edi
  400457:	ff e0                	jmpq   *%rax
  400459:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000400460 <register_tm_clones>:
  400460:	b8 38 10 60 00       	mov    $0x601038,%eax
  400465:	55                   	push   %rbp
  400466:	48 2d 38 10 60 00    	sub    $0x601038,%rax
  40046c:	48 c1 f8 03          	sar    $0x3,%rax
  400470:	48 89 e5             	mov    %rsp,%rbp
  400473:	48 89 c2             	mov    %rax,%rdx
  400476:	48 c1 ea 3f          	shr    $0x3f,%rdx
  40047a:	48 01 d0             	add    %rdx,%rax
  40047d:	48 d1 f8             	sar    %rax
  400480:	75 02                	jne    400484 <register_tm_clones+0x24>
  400482:	5d                   	pop    %rbp
  400483:	c3                   	retq   
  400484:	ba 00 00 00 00       	mov    $0x0,%edx
  400489:	48 85 d2             	test   %rdx,%rdx
  40048c:	74 f4                	je     400482 <register_tm_clones+0x22>
  40048e:	5d                   	pop    %rbp
  40048f:	48 89 c6             	mov    %rax,%rsi
  400492:	bf 38 10 60 00       	mov    $0x601038,%edi
  400497:	ff e2                	jmpq   *%rdx
  400499:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000004004a0 <__do_global_dtors_aux>:
  4004a0:	80 3d 8d 0b 20 00 00 	cmpb   $0x0,0x200b8d(%rip)        # 601034 <_edata>
  4004a7:	75 11                	jne    4004ba <__do_global_dtors_aux+0x1a>
  4004a9:	55                   	push   %rbp
  4004aa:	48 89 e5             	mov    %rsp,%rbp
  4004ad:	e8 7e ff ff ff       	callq  400430 <deregister_tm_clones>
  4004b2:	5d                   	pop    %rbp
  4004b3:	c6 05 7a 0b 20 00 01 	movb   $0x1,0x200b7a(%rip)        # 601034 <_edata>
  4004ba:	f3 c3                	repz retq 
  4004bc:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004004c0 <frame_dummy>:
  4004c0:	48 83 3d 58 09 20 00 	cmpq   $0x0,0x200958(%rip)        # 600e20 <__JCR_END__>
  4004c7:	00 
  4004c8:	74 1e                	je     4004e8 <frame_dummy+0x28>
  4004ca:	b8 00 00 00 00       	mov    $0x0,%eax
  4004cf:	48 85 c0             	test   %rax,%rax
  4004d2:	74 14                	je     4004e8 <frame_dummy+0x28>
  4004d4:	55                   	push   %rbp
  4004d5:	bf 20 0e 60 00       	mov    $0x600e20,%edi
  4004da:	48 89 e5             	mov    %rsp,%rbp
  4004dd:	ff d0                	callq  *%rax
  4004df:	5d                   	pop    %rbp
  4004e0:	e9 7b ff ff ff       	jmpq   400460 <register_tm_clones>
  4004e5:	0f 1f 00             	nopl   (%rax)
  4004e8:	e9 73 ff ff ff       	jmpq   400460 <register_tm_clones>

00000000004004ed <main>:
  4004ed:	48 83 ec 08          	sub    $0x8,%rsp
  4004f1:	be 02 00 00 00       	mov    $0x2,%esi
  4004f6:	bf 2c 10 60 00       	mov    $0x60102c,%edi
  4004fb:	e8 05 00 00 00       	callq  400505 <sum>
  400500:	48 83 c4 08          	add    $0x8,%rsp
  400504:	c3                   	retq   

0000000000400505 <sum>:
  400505:	b8 00 00 00 00       	mov    $0x0,%eax
  40050a:	ba 00 00 00 00       	mov    $0x0,%edx
  40050f:	eb 09                	jmp    40051a <sum+0x15>
  400511:	48 63 ca             	movslq %edx,%rcx
  400514:	03 04 8f             	add    (%rdi,%rcx,4),%eax
  400517:	83 c2 01             	add    $0x1,%edx
  40051a:	39 f2                	cmp    %esi,%edx
  40051c:	7c f3                	jl     400511 <sum+0xc>
  40051e:	f3 c3                	repz retq 

0000000000400520 <__libc_csu_init>:
  400520:	41 57                	push   %r15
  400522:	41 89 ff             	mov    %edi,%r15d
  400525:	41 56                	push   %r14
  400527:	49 89 f6             	mov    %rsi,%r14
  40052a:	41 55                	push   %r13
  40052c:	49 89 d5             	mov    %rdx,%r13
  40052f:	41 54                	push   %r12
  400531:	4c 8d 25 d8 08 20 00 	lea    0x2008d8(%rip),%r12        # 600e10 <__frame_dummy_init_array_entry>
  400538:	55                   	push   %rbp
  400539:	48 8d 2d d8 08 20 00 	lea    0x2008d8(%rip),%rbp        # 600e18 <__init_array_end>
  400540:	53                   	push   %rbx
  400541:	4c 29 e5             	sub    %r12,%rbp
  400544:	31 db                	xor    %ebx,%ebx
  400546:	48 c1 fd 03          	sar    $0x3,%rbp
  40054a:	48 83 ec 08          	sub    $0x8,%rsp
  40054e:	e8 55 fe ff ff       	callq  4003a8 <_init>
  400553:	48 85 ed             	test   %rbp,%rbp
  400556:	74 1e                	je     400576 <__libc_csu_init+0x56>
  400558:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  40055f:	00 
  400560:	4c 89 ea             	mov    %r13,%rdx
  400563:	4c 89 f6             	mov    %r14,%rsi
  400566:	44 89 ff             	mov    %r15d,%edi
  400569:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  40056d:	48 83 c3 01          	add    $0x1,%rbx
  400571:	48 39 eb             	cmp    %rbp,%rbx
  400574:	75 ea                	jne    400560 <__libc_csu_init+0x40>
  400576:	48 83 c4 08          	add    $0x8,%rsp
  40057a:	5b                   	pop    %rbx
  40057b:	5d                   	pop    %rbp
  40057c:	41 5c                	pop    %r12
  40057e:	41 5d                	pop    %r13
  400580:	41 5e                	pop    %r14
  400582:	41 5f                	pop    %r15
  400584:	c3                   	retq   
  400585:	90                   	nop
  400586:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  40058d:	00 00 00 

0000000000400590 <__libc_csu_fini>:
  400590:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000400594 <_fini>:
  400594:	48 83 ec 08          	sub    $0x8,%rsp
  400598:	48 83 c4 08          	add    $0x8,%rsp
  40059c:	c3                   	retq   
