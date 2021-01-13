
libvector.so:     file format elf64-x86-64
libvector.so
architecture: i386:x86-64, flags 0x00000150:
HAS_SYMS, DYNAMIC, D_PAGED
start address 0x0000000000000610

Program Header:
    LOAD off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**21
         filesz 0x00000000000007ec memsz 0x00000000000007ec flags r-x
    LOAD off    0x0000000000000de8 vaddr 0x0000000000200de8 paddr 0x0000000000200de8 align 2**21
         filesz 0x0000000000000240 memsz 0x0000000000000250 flags rw-
 DYNAMIC off    0x0000000000000e08 vaddr 0x0000000000200e08 paddr 0x0000000000200e08 align 2**3
         filesz 0x00000000000001c0 memsz 0x00000000000001c0 flags rw-
    NOTE off    0x00000000000001c8 vaddr 0x00000000000001c8 paddr 0x00000000000001c8 align 2**2
         filesz 0x0000000000000024 memsz 0x0000000000000024 flags r--
EH_FRAME off    0x0000000000000754 vaddr 0x0000000000000754 paddr 0x0000000000000754 align 2**2
         filesz 0x0000000000000024 memsz 0x0000000000000024 flags r--
   STACK off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**4
         filesz 0x0000000000000000 memsz 0x0000000000000000 flags rw-
   RELRO off    0x0000000000000de8 vaddr 0x0000000000200de8 paddr 0x0000000000200de8 align 2**0
         filesz 0x0000000000000218 memsz 0x0000000000000218 flags r--

Dynamic Section:
  NEEDED               libc.so.6
  INIT                 0x00000000000005c0
  FINI                 0x0000000000000748
  INIT_ARRAY           0x0000000000200de8
  INIT_ARRAYSZ         0x0000000000000008
  FINI_ARRAY           0x0000000000200df0
  FINI_ARRAYSZ         0x0000000000000008
  GNU_HASH             0x00000000000001f0
  STRTAB               0x00000000000003a0
  SYMTAB               0x0000000000000238
  STRSZ                0x00000000000000c1
  SYMENT               0x0000000000000018
  PLTGOT               0x0000000000201000
  PLTRELSZ             0x0000000000000030
  PLTREL               0x0000000000000007
  JMPREL               0x0000000000000590
  RELA                 0x00000000000004a0
  RELASZ               0x00000000000000f0
  RELAENT              0x0000000000000018
  VERNEED              0x0000000000000480
  VERNEEDNUM           0x0000000000000001
  VERSYM               0x0000000000000462
  RELACOUNT            0x0000000000000003

Version References:
  required from libc.so.6:
    0x09691a75 0x00 02 GLIBC_2.2.5

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .note.gnu.build-id 00000024  00000000000001c8  00000000000001c8  000001c8  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  1 .gnu.hash     00000048  00000000000001f0  00000000000001f0  000001f0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  2 .dynsym       00000168  0000000000000238  0000000000000238  00000238  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  3 .dynstr       000000c1  00000000000003a0  00000000000003a0  000003a0  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .gnu.version  0000001e  0000000000000462  0000000000000462  00000462  2**1
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  5 .gnu.version_r 00000020  0000000000000480  0000000000000480  00000480  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  6 .rela.dyn     000000f0  00000000000004a0  00000000000004a0  000004a0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  7 .rela.plt     00000030  0000000000000590  0000000000000590  00000590  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  8 .init         0000001a  00000000000005c0  00000000000005c0  000005c0  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  9 .plt          00000030  00000000000005e0  00000000000005e0  000005e0  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 10 .text         00000138  0000000000000610  0000000000000610  00000610  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 11 .fini         00000009  0000000000000748  0000000000000748  00000748  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 12 .eh_frame_hdr 00000024  0000000000000754  0000000000000754  00000754  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 13 .eh_frame     00000074  0000000000000778  0000000000000778  00000778  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 14 .init_array   00000008  0000000000200de8  0000000000200de8  00000de8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 15 .fini_array   00000008  0000000000200df0  0000000000200df0  00000df0  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 16 .jcr          00000008  0000000000200df8  0000000000200df8  00000df8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 17 .data.rel.ro  00000008  0000000000200e00  0000000000200e00  00000e00  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 18 .dynamic      000001c0  0000000000200e08  0000000000200e08  00000e08  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 19 .got          00000038  0000000000200fc8  0000000000200fc8  00000fc8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 20 .got.plt      00000028  0000000000201000  0000000000201000  00001000  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 21 .bss          00000010  0000000000201028  0000000000201028  00001028  2**2
                  ALLOC
 22 .comment      0000002d  0000000000000000  0000000000000000  00001028  2**0
                  CONTENTS, READONLY
SYMBOL TABLE:
00000000000001c8 l    d  .note.gnu.build-id	0000000000000000              .note.gnu.build-id
00000000000001f0 l    d  .gnu.hash	0000000000000000              .gnu.hash
0000000000000238 l    d  .dynsym	0000000000000000              .dynsym
00000000000003a0 l    d  .dynstr	0000000000000000              .dynstr
0000000000000462 l    d  .gnu.version	0000000000000000              .gnu.version
0000000000000480 l    d  .gnu.version_r	0000000000000000              .gnu.version_r
00000000000004a0 l    d  .rela.dyn	0000000000000000              .rela.dyn
0000000000000590 l    d  .rela.plt	0000000000000000              .rela.plt
00000000000005c0 l    d  .init	0000000000000000              .init
00000000000005e0 l    d  .plt	0000000000000000              .plt
0000000000000610 l    d  .text	0000000000000000              .text
0000000000000748 l    d  .fini	0000000000000000              .fini
0000000000000754 l    d  .eh_frame_hdr	0000000000000000              .eh_frame_hdr
0000000000000778 l    d  .eh_frame	0000000000000000              .eh_frame
0000000000200de8 l    d  .init_array	0000000000000000              .init_array
0000000000200df0 l    d  .fini_array	0000000000000000              .fini_array
0000000000200df8 l    d  .jcr	0000000000000000              .jcr
0000000000200e00 l    d  .data.rel.ro	0000000000000000              .data.rel.ro
0000000000200e08 l    d  .dynamic	0000000000000000              .dynamic
0000000000200fc8 l    d  .got	0000000000000000              .got
0000000000201000 l    d  .got.plt	0000000000000000              .got.plt
0000000000201028 l    d  .bss	0000000000000000              .bss
0000000000000000 l    d  .comment	0000000000000000              .comment
0000000000000000 l    df *ABS*	0000000000000000              crtstuff.c
0000000000200df8 l     O .jcr	0000000000000000              __JCR_LIST__
0000000000000610 l     F .text	0000000000000000              deregister_tm_clones
0000000000000640 l     F .text	0000000000000000              register_tm_clones
0000000000000680 l     F .text	0000000000000000              __do_global_dtors_aux
0000000000201028 l     O .bss	0000000000000001              completed.6355
0000000000200df0 l     O .fini_array	0000000000000000              __do_global_dtors_aux_fini_array_entry
00000000000006c0 l     F .text	0000000000000000              frame_dummy
0000000000200de8 l     O .init_array	0000000000000000              __frame_dummy_init_array_entry
0000000000000000 l    df *ABS*	0000000000000000              addvec.c
0000000000000000 l    df *ABS*	0000000000000000              multvec.c
0000000000000000 l    df *ABS*	0000000000000000              crtstuff.c
00000000000007e8 l     O .eh_frame	0000000000000000              __FRAME_END__
0000000000200df8 l     O .jcr	0000000000000000              __JCR_END__
0000000000000000 l    df *ABS*	0000000000000000              
0000000000200e00 l     O .data.rel.ro	0000000000000000              __dso_handle
0000000000200e08 l     O .dynamic	0000000000000000              _DYNAMIC
0000000000000754 l       .eh_frame_hdr	0000000000000000              __GNU_EH_FRAME_HDR
0000000000201028 l     O .got.plt	0000000000000000              __TMC_END__
0000000000201000 l     O .got.plt	0000000000000000              _GLOBAL_OFFSET_TABLE_
0000000000000000  w      *UND*	0000000000000000              _ITM_deregisterTMCloneTable
0000000000201030 g     O .bss	0000000000000004              multcnt
0000000000201028 g       .got.plt	0000000000000000              _edata
0000000000000748 g     F .fini	0000000000000000              _fini
000000000000071e g     F .text	000000000000002a              multvec
00000000000006f5 g     F .text	0000000000000029              addvec
0000000000000000  w      *UND*	0000000000000000              __gmon_start__
0000000000201038 g       .bss	0000000000000000              _end
0000000000201028 g       .bss	0000000000000000              __bss_start
000000000020102c g     O .bss	0000000000000004              addcnt
0000000000000000  w      *UND*	0000000000000000              _Jv_RegisterClasses
0000000000000000  w      *UND*	0000000000000000              _ITM_registerTMCloneTable
0000000000000000  w    F *UND*	0000000000000000              __cxa_finalize@@GLIBC_2.2.5
00000000000005c0 g     F .init	0000000000000000              _init



Disassembly of section .init:

00000000000005c0 <_init>:
 5c0:	48 83 ec 08          	sub    $0x8,%rsp
 5c4:	48 8b 05 0d 0a 20 00 	mov    0x200a0d(%rip),%rax        # 200fd8 <__gmon_start__>
 5cb:	48 85 c0             	test   %rax,%rax
 5ce:	74 05                	je     5d5 <_init+0x15>
 5d0:	e8 1b 00 00 00       	callq  5f0 <__gmon_start__@plt>
 5d5:	48 83 c4 08          	add    $0x8,%rsp
 5d9:	c3                   	retq   

Disassembly of section .plt:

00000000000005e0 <.plt>:
 5e0:	ff 35 22 0a 20 00    	pushq  0x200a22(%rip)        # 201008 <_GLOBAL_OFFSET_TABLE_+0x8>
 5e6:	ff 25 24 0a 20 00    	jmpq   *0x200a24(%rip)        # 201010 <_GLOBAL_OFFSET_TABLE_+0x10>
 5ec:	0f 1f 40 00          	nopl   0x0(%rax)

00000000000005f0 <__gmon_start__@plt>:
 5f0:	ff 25 22 0a 20 00    	jmpq   *0x200a22(%rip)        # 201018 <__gmon_start__>
 5f6:	68 00 00 00 00       	pushq  $0x0
 5fb:	e9 e0 ff ff ff       	jmpq   5e0 <.plt>

0000000000000600 <__cxa_finalize@plt>:
 600:	ff 25 1a 0a 20 00    	jmpq   *0x200a1a(%rip)        # 201020 <__cxa_finalize@GLIBC_2.2.5>
 606:	68 01 00 00 00       	pushq  $0x1
 60b:	e9 d0 ff ff ff       	jmpq   5e0 <.plt>

Disassembly of section .text:

0000000000000610 <deregister_tm_clones>:
 610:	48 8d 05 18 0a 20 00 	lea    0x200a18(%rip),%rax        # 20102f <addcnt+0x3>
 617:	48 8d 3d 0a 0a 20 00 	lea    0x200a0a(%rip),%rdi        # 201028 <_edata>
 61e:	55                   	push   %rbp
 61f:	48 29 f8             	sub    %rdi,%rax
 622:	48 89 e5             	mov    %rsp,%rbp
 625:	48 83 f8 0e          	cmp    $0xe,%rax
 629:	77 02                	ja     62d <deregister_tm_clones+0x1d>
 62b:	5d                   	pop    %rbp
 62c:	c3                   	retq   
 62d:	48 8b 05 94 09 20 00 	mov    0x200994(%rip),%rax        # 200fc8 <_ITM_deregisterTMCloneTable>
 634:	48 85 c0             	test   %rax,%rax
 637:	74 f2                	je     62b <deregister_tm_clones+0x1b>
 639:	5d                   	pop    %rbp
 63a:	ff e0                	jmpq   *%rax
 63c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000000640 <register_tm_clones>:
 640:	48 8d 05 e1 09 20 00 	lea    0x2009e1(%rip),%rax        # 201028 <_edata>
 647:	48 8d 3d da 09 20 00 	lea    0x2009da(%rip),%rdi        # 201028 <_edata>
 64e:	55                   	push   %rbp
 64f:	48 29 f8             	sub    %rdi,%rax
 652:	48 89 e5             	mov    %rsp,%rbp
 655:	48 c1 f8 03          	sar    $0x3,%rax
 659:	48 89 c2             	mov    %rax,%rdx
 65c:	48 c1 ea 3f          	shr    $0x3f,%rdx
 660:	48 01 d0             	add    %rdx,%rax
 663:	48 d1 f8             	sar    %rax
 666:	75 02                	jne    66a <register_tm_clones+0x2a>
 668:	5d                   	pop    %rbp
 669:	c3                   	retq   
 66a:	48 8b 15 7f 09 20 00 	mov    0x20097f(%rip),%rdx        # 200ff0 <_ITM_registerTMCloneTable>
 671:	48 85 d2             	test   %rdx,%rdx
 674:	74 f2                	je     668 <register_tm_clones+0x28>
 676:	5d                   	pop    %rbp
 677:	48 89 c6             	mov    %rax,%rsi
 67a:	ff e2                	jmpq   *%rdx
 67c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000000680 <__do_global_dtors_aux>:
 680:	80 3d a1 09 20 00 00 	cmpb   $0x0,0x2009a1(%rip)        # 201028 <_edata>
 687:	75 27                	jne    6b0 <__do_global_dtors_aux+0x30>
 689:	48 83 3d 67 09 20 00 	cmpq   $0x0,0x200967(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 690:	00 
 691:	55                   	push   %rbp
 692:	48 89 e5             	mov    %rsp,%rbp
 695:	74 0c                	je     6a3 <__do_global_dtors_aux+0x23>
 697:	48 8d 3d 62 07 20 00 	lea    0x200762(%rip),%rdi        # 200e00 <__dso_handle>
 69e:	e8 5d ff ff ff       	callq  600 <__cxa_finalize@plt>
 6a3:	e8 68 ff ff ff       	callq  610 <deregister_tm_clones>
 6a8:	5d                   	pop    %rbp
 6a9:	c6 05 78 09 20 00 01 	movb   $0x1,0x200978(%rip)        # 201028 <_edata>
 6b0:	f3 c3                	repz retq 
 6b2:	0f 1f 40 00          	nopl   0x0(%rax)
 6b6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 6bd:	00 00 00 

00000000000006c0 <frame_dummy>:
 6c0:	48 83 3d 30 07 20 00 	cmpq   $0x0,0x200730(%rip)        # 200df8 <__JCR_END__>
 6c7:	00 
 6c8:	74 26                	je     6f0 <frame_dummy+0x30>
 6ca:	48 8b 05 17 09 20 00 	mov    0x200917(%rip),%rax        # 200fe8 <_Jv_RegisterClasses>
 6d1:	48 85 c0             	test   %rax,%rax
 6d4:	74 1a                	je     6f0 <frame_dummy+0x30>
 6d6:	55                   	push   %rbp
 6d7:	48 8d 3d 1a 07 20 00 	lea    0x20071a(%rip),%rdi        # 200df8 <__JCR_END__>
 6de:	48 89 e5             	mov    %rsp,%rbp
 6e1:	ff d0                	callq  *%rax
 6e3:	5d                   	pop    %rbp
 6e4:	e9 57 ff ff ff       	jmpq   640 <register_tm_clones>
 6e9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 6f0:	e9 4b ff ff ff       	jmpq   640 <register_tm_clones>

00000000000006f5 <addvec>:
 6f5:	48 8b 05 e4 08 20 00 	mov    0x2008e4(%rip),%rax        # 200fe0 <addcnt@@Base-0x4c>
 6fc:	83 00 01             	addl   $0x1,(%rax)
 6ff:	b8 00 00 00 00       	mov    $0x0,%eax
 704:	eb 12                	jmp    718 <addvec+0x23>
 706:	4c 63 c0             	movslq %eax,%r8
 709:	46 8b 0c 86          	mov    (%rsi,%r8,4),%r9d
 70d:	46 03 0c 87          	add    (%rdi,%r8,4),%r9d
 711:	46 89 0c 82          	mov    %r9d,(%rdx,%r8,4)
 715:	83 c0 01             	add    $0x1,%eax
 718:	39 c8                	cmp    %ecx,%eax
 71a:	7c ea                	jl     706 <addvec+0x11>
 71c:	f3 c3                	repz retq 

000000000000071e <multvec>:
 71e:	48 8b 05 ab 08 20 00 	mov    0x2008ab(%rip),%rax        # 200fd0 <multcnt@@Base-0x60>
 725:	83 00 01             	addl   $0x1,(%rax)
 728:	b8 00 00 00 00       	mov    $0x0,%eax
 72d:	eb 13                	jmp    742 <multvec+0x24>
 72f:	4c 63 c0             	movslq %eax,%r8
 732:	46 8b 0c 86          	mov    (%rsi,%r8,4),%r9d
 736:	46 0f af 0c 87       	imul   (%rdi,%r8,4),%r9d
 73b:	46 89 0c 82          	mov    %r9d,(%rdx,%r8,4)
 73f:	83 c0 01             	add    $0x1,%eax
 742:	39 c8                	cmp    %ecx,%eax
 744:	7c e9                	jl     72f <multvec+0x11>
 746:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000000748 <_fini>:
 748:	48 83 ec 08          	sub    $0x8,%rsp
 74c:	48 83 c4 08          	add    $0x8,%rsp
 750:	c3                   	retq   
