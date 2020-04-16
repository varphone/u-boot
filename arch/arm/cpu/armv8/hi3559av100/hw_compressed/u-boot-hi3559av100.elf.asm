
u-boot-hi3559av100.elf:     file format elf64-littleaarch64


Disassembly of section .text:

0000000048700000 <__image_copy_start>:
    48700000:	14000a1c 	b	48702870 <reset>
    48700004:	deadbeef 	.inst	0xdeadbeef ; undefined
    48700008:	deadbeef 	.inst	0xdeadbeef ; undefined
    4870000c:	deadbeef 	.inst	0xdeadbeef ; undefined
    48700010:	deadbeef 	.inst	0xdeadbeef ; undefined
    48700014:	deadbeef 	.inst	0xdeadbeef ; undefined
    48700018:	deadbeef 	.inst	0xdeadbeef ; undefined
    4870001c:	deadbeef 	.inst	0xdeadbeef ; undefined
    48700020:	deadbeef 	.inst	0xdeadbeef ; undefined
    48700024:	deadbeef 	.inst	0xdeadbeef ; undefined
    48700028:	deadbeef 	.inst	0xdeadbeef ; undefined
    4870002c:	deadbeef 	.inst	0xdeadbeef ; undefined
    48700030:	deadbeef 	.inst	0xdeadbeef ; undefined
    48700034:	deadbeef 	.inst	0xdeadbeef ; undefined
    48700038:	deadbeef 	.inst	0xdeadbeef ; undefined
    4870003c:	deadbeef 	.inst	0xdeadbeef ; undefined

0000000048700040 <__blank_zone_start>:
	...

0000000048702840 <_blank_zone_start>:
    48702840:	48700040 	.word	0x48700040
    48702844:	00000000 	.word	0x00000000

0000000048702848 <_blank_zone_end>:
    48702848:	48702840 	.word	0x48702840
    4870284c:	00000000 	.word	0x00000000

0000000048702850 <_TEXT_BASE>:
    48702850:	48700000 	.word	0x48700000
    48702854:	00000000 	.word	0x00000000

0000000048702858 <_end_ofs>:
    48702858:	0005bda8 	.word	0x0005bda8
    4870285c:	00000000 	.word	0x00000000

0000000048702860 <_bss_start_ofs>:
    48702860:	0005bda8 	.word	0x0005bda8
    48702864:	00000000 	.word	0x00000000

0000000048702868 <_bss_end_ofs>:
    48702868:	0005bda8 	.word	0x0005bda8
    4870286c:	00000000 	.word	0x00000000

0000000048702870 <reset>:
    48702870:	10000f40 	adr	x0, 48702a58 <vectors>
    48702874:	d5384241 	mrs	x1, currentel
    48702878:	f100303f 	cmp	x1, #0xc
    4870287c:	540000a0 	b.eq	48702890 <reset+0x20>  // b.none
    48702880:	f100203f 	cmp	x1, #0x8
    48702884:	54000160 	b.eq	487028b0 <reset+0x40>  // b.none
    48702888:	f100103f 	cmp	x1, #0x4
    4870288c:	540001a0 	b.eq	487028c0 <reset+0x50>  // b.none
    48702890:	d51ec000 	msr	vbar_el3, x0
    48702894:	d53e1100 	mrs	x0, scr_el3
    48702898:	b2400c00 	orr	x0, x0, #0xf
    4870289c:	d51e1100 	msr	scr_el3, x0
    487028a0:	d51e115f 	msr	cptr_el3, xzr
    487028a4:	580036e0 	ldr	x0, 48702f80 <Str_SystemSartup+0x28>
    487028a8:	d51be000 	msr	cntfrq_el0, x0
    487028ac:	14000008 	b	487028cc <reset+0x5c>
    487028b0:	d51cc000 	msr	vbar_el2, x0
    487028b4:	d2867fe0 	mov	x0, #0x33ff                	// #13311
    487028b8:	d51c1140 	msr	cptr_el2, x0
    487028bc:	14000004 	b	487028cc <reset+0x5c>
    487028c0:	d518c000 	msr	vbar_el1, x0
    487028c4:	d2a00600 	mov	x0, #0x300000              	// #3145728
    487028c8:	d5181040 	msr	cpacr_el1, x0
    487028cc:	580035e0 	ldr	x0, 48702f88 <Str_SystemSartup+0x30>
    487028d0:	b9414001 	ldr	w1, [x0, #320]
    487028d4:	180034e2 	ldr	w2, 48702f70 <Str_SystemSartup+0x18>
    487028d8:	6b02003f 	cmp	w1, w2
    487028dc:	540004a1 	b.ne	48702970 <normal_start_flow>  // b.any
    487028e0:	910003e1 	mov	x1, sp
    487028e4:	b9014001 	str	w1, [x0, #320]

00000000487028e8 <ziju_flow>:
    487028e8:	58003542 	ldr	x2, 48702f90 <Str_SystemSartup+0x38>
    487028ec:	927cec5f 	and	sp, x2, #0xfffffffffffffff0
    487028f0:	58fffa80 	ldr	x0, 48702840 <_blank_zone_start>
    487028f4:	58fffae1 	ldr	x1, 48702850 <_TEXT_BASE>
    487028f8:	cb010000 	sub	x0, x0, x1
    487028fc:	10feb821 	adr	x1, 48700000 <__image_copy_start>
    48702900:	8b010000 	add	x0, x0, x1
    48702904:	d2800001 	mov	x1, #0x0                   	// #0
    48702908:	940001d5 	bl	4870305c <init_registers>
    4870290c:	940002e7 	bl	487034a8 <start_ddr_training>
    48702910:	580033c0 	ldr	x0, 48702f88 <Str_SystemSartup+0x30>
    48702914:	b9408c01 	ldr	w1, [x0, #140]
    48702918:	53087c21 	lsr	w1, w1, #8
    4870291c:	12000021 	and	w1, w1, #0x1
    48702920:	7100043f 	cmp	w1, #0x1
    48702924:	540000c0 	b.eq	4870293c <pcie_slave_boot>  // b.none

0000000048702928 <return_to_bootrom>:
    48702928:	b9414001 	ldr	w1, [x0, #320]
    4870292c:	9100003f 	mov	sp, x1
    48702930:	b9414401 	ldr	w1, [x0, #324]
    48702934:	aa0103fe 	mov	x30, x1
    48702938:	d65f03c0 	ret

000000004870293c <pcie_slave_boot>:
    4870293c:	b9403001 	ldr	w1, [x0, #48]
    48702940:	12000021 	and	w1, w1, #0x1
    48702944:	7100043f 	cmp	w1, #0x1
    48702948:	54ffff00 	b.eq	48702928 <return_to_bootrom>  // b.none
    4870294c:	18003141 	ldr	w1, 48702f74 <Str_SystemSartup+0x1c>
    48702950:	b9013401 	str	w1, [x0, #308]

0000000048702954 <pcie_slave_hold>:
    48702954:	580031a0 	ldr	x0, 48702f88 <Str_SystemSartup+0x30>
    48702958:	b9413400 	ldr	w0, [x0, #308]
    4870295c:	180030e1 	ldr	w1, 48702f78 <Str_SystemSartup+0x20>
    48702960:	6b01001f 	cmp	w0, w1
    48702964:	54ffff81 	b.ne	48702954 <pcie_slave_hold>  // b.any
    48702968:	5800319e 	ldr	x30, 48702f98 <Str_SystemSartup+0x40>
    4870296c:	d65f03c0 	ret

0000000048702970 <normal_start_flow>:
    48702970:	58003180 	ldr	x0, 48702fa0 <Str_SystemSartup+0x48>
    48702974:	927cec1f 	and	sp, x0, #0xfffffffffffffff0
    48702978:	9400101e 	bl	487069f0 <uart_early_init>
    4870297c:	10002ee0 	adr	x0, 48702f58 <Str_SystemSartup>
    48702980:	94001034 	bl	48706a50 <uart_early_puts>
    48702984:	940013df 	bl	48707900 <icache_enable>

0000000048702988 <running_addr_check>:
    48702988:	10000000 	adr	x0, 48702988 <running_addr_check>
    4870298c:	d35cfc00 	lsr	x0, x0, #28
    48702990:	f100101f 	cmp	x0, #0x4
    48702994:	540003aa 	b.ge	48702a08 <master_cpu>  // b.tcont
    48702998:	58fff540 	ldr	x0, 48702840 <_blank_zone_start>
    4870299c:	58fff5a1 	ldr	x1, 48702850 <_TEXT_BASE>
    487029a0:	cb010000 	sub	x0, x0, x1
    487029a4:	10feb2e1 	adr	x1, 48700000 <__image_copy_start>
    487029a8:	8b010000 	add	x0, x0, x1
    487029ac:	d2800001 	mov	x1, #0x0                   	// #0
    487029b0:	940001ab 	bl	4870305c <init_registers>
    487029b4:	940002bd 	bl	487034a8 <start_ddr_training>

00000000487029b8 <check_boot_mode>:
    487029b8:	58002e80 	ldr	x0, 48702f88 <Str_SystemSartup+0x30>
    487029bc:	b9408c00 	ldr	w0, [x0, #140]
    487029c0:	53047c06 	lsr	w6, w0, #4
    487029c4:	120004c6 	and	w6, w6, #0x3
    487029c8:	710008df 	cmp	w6, #0x2
    487029cc:	540000e1 	b.ne	487029e8 <ufs_boot>  // b.any

00000000487029d0 <emmc_boot>:
    487029d0:	58fff400 	ldr	x0, 48702850 <_TEXT_BASE>
    487029d4:	58002ea1 	ldr	x1, 48702fa8 <Str_SystemSartup+0x50>
    487029d8:	58002ec2 	ldr	x2, 48702fb0 <Str_SystemSartup+0x58>
    487029dc:	cb010041 	sub	x1, x2, x1
    487029e0:	94001248 	bl	48707300 <emmc_boot_read>
    487029e4:	14000018 	b	48702a44 <jump_to_ddr>

00000000487029e8 <ufs_boot>:
    487029e8:	71000cdf 	cmp	w6, #0x3
    487029ec:	540000e1 	b.ne	48702a08 <master_cpu>  // b.any
    487029f0:	58fff300 	ldr	x0, 48702850 <_TEXT_BASE>
    487029f4:	58002da1 	ldr	x1, 48702fa8 <Str_SystemSartup+0x50>
    487029f8:	58002dc2 	ldr	x2, 48702fb0 <Str_SystemSartup+0x58>
    487029fc:	cb010041 	sub	x1, x2, x1
    48702a00:	9400114f 	bl	48706f3c <ufs_boot_read>
    48702a04:	14000010 	b	48702a44 <jump_to_ddr>

0000000048702a08 <master_cpu>:
    48702a08:	10feafc0 	adr	x0, 48700000 <__image_copy_start>
    48702a0c:	58002ce1 	ldr	x1, 48702fa8 <Str_SystemSartup+0x50>
    48702a10:	58002d02 	ldr	x2, 48702fb0 <Str_SystemSartup+0x58>
    48702a14:	eb010042 	subs	x2, x2, x1
    48702a18:	540000e0 	b.eq	48702a34 <clear_remap>  // b.none
    48702a1c:	58fff1a1 	ldr	x1, 48702850 <_TEXT_BASE>
    48702a20:	8b010042 	add	x2, x2, x1

0000000048702a24 <copy_loop>:
    48702a24:	a8c12c0a 	ldp	x10, x11, [x0], #16
    48702a28:	a8812c2a 	stp	x10, x11, [x1], #16
    48702a2c:	eb02003f 	cmp	x1, x2
    48702a30:	54ffffa3 	b.cc	48702a24 <copy_loop>  // b.lo, b.ul, b.last

0000000048702a34 <clear_remap>:
    48702a34:	58002aa1 	ldr	x1, 48702f88 <Str_SystemSartup+0x30>
    48702a38:	b9400020 	ldr	w0, [x1]
    48702a3c:	32180000 	orr	w0, w0, #0x100
    48702a40:	b9000020 	str	w0, [x1]

0000000048702a44 <jump_to_ddr>:
    48702a44:	10000060 	adr	x0, 48702a50 <_start_armboot>
    48702a48:	f940001e 	ldr	x30, [x0]
    48702a4c:	d65f03c0 	ret

0000000048702a50 <_start_armboot>:
    48702a50:	4870783c 	.word	0x4870783c
    48702a54:	00000000 	.word	0x00000000

0000000048702a58 <vectors>:
    48702a58:	1400000f 	b	48702a94 <_do_bad_sync>
    48702a5c:	d503201f 	nop
    48702a60:	1400002f 	b	48702b1c <_do_bad_irq>
    48702a64:	d503201f 	nop
    48702a68:	1400004f 	b	48702ba4 <_do_bad_fiq>
    48702a6c:	d503201f 	nop
    48702a70:	1400006f 	b	48702c2c <_do_bad_error>
    48702a74:	d503201f 	nop
    48702a78:	1400008f 	b	48702cb4 <_do_sync>
    48702a7c:	d503201f 	nop
    48702a80:	140000af 	b	48702d3c <_do_irq>
    48702a84:	d503201f 	nop
    48702a88:	140000cf 	b	48702dc4 <_do_fiq>
    48702a8c:	d503201f 	nop
    48702a90:	140000ef 	b	48702e4c <_do_error>

0000000048702a94 <_do_bad_sync>:
    48702a94:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48702a98:	a9bf73fb 	stp	x27, x28, [sp, #-16]!
    48702a9c:	a9bf6bf9 	stp	x25, x26, [sp, #-16]!
    48702aa0:	a9bf63f7 	stp	x23, x24, [sp, #-16]!
    48702aa4:	a9bf5bf5 	stp	x21, x22, [sp, #-16]!
    48702aa8:	a9bf53f3 	stp	x19, x20, [sp, #-16]!
    48702aac:	a9bf4bf1 	stp	x17, x18, [sp, #-16]!
    48702ab0:	a9bf43ef 	stp	x15, x16, [sp, #-16]!
    48702ab4:	a9bf3bed 	stp	x13, x14, [sp, #-16]!
    48702ab8:	a9bf33eb 	stp	x11, x12, [sp, #-16]!
    48702abc:	a9bf2be9 	stp	x9, x10, [sp, #-16]!
    48702ac0:	a9bf23e7 	stp	x7, x8, [sp, #-16]!
    48702ac4:	a9bf1be5 	stp	x5, x6, [sp, #-16]!
    48702ac8:	a9bf13e3 	stp	x3, x4, [sp, #-16]!
    48702acc:	a9bf0be1 	stp	x1, x2, [sp, #-16]!
    48702ad0:	d538424b 	mrs	x11, currentel
    48702ad4:	f100317f 	cmp	x11, #0xc
    48702ad8:	540000a0 	b.eq	48702aec <_do_bad_sync+0x58>  // b.none
    48702adc:	f100217f 	cmp	x11, #0x8
    48702ae0:	540000c0 	b.eq	48702af8 <_do_bad_sync+0x64>  // b.none
    48702ae4:	f100117f 	cmp	x11, #0x4
    48702ae8:	540000e0 	b.eq	48702b04 <_do_bad_sync+0x70>  // b.none
    48702aec:	d53e5201 	mrs	x1, esr_el3
    48702af0:	d53e4022 	mrs	x2, elr_el3
    48702af4:	14000006 	b	48702b0c <_do_bad_sync+0x78>
    48702af8:	d53c5201 	mrs	x1, esr_el2
    48702afc:	d53c4022 	mrs	x2, elr_el2
    48702b00:	14000003 	b	48702b0c <_do_bad_sync+0x78>
    48702b04:	d5385201 	mrs	x1, esr_el1
    48702b08:	d5384022 	mrs	x2, elr_el1
    48702b0c:	a9bf03e2 	stp	x2, x0, [sp, #-16]!
    48702b10:	910003e0 	mov	x0, sp
    48702b14:	94001398 	bl	48707974 <do_bad_sync>
    48702b18:	140000ef 	b	48702ed4 <exception_exit>

0000000048702b1c <_do_bad_irq>:
    48702b1c:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48702b20:	a9bf73fb 	stp	x27, x28, [sp, #-16]!
    48702b24:	a9bf6bf9 	stp	x25, x26, [sp, #-16]!
    48702b28:	a9bf63f7 	stp	x23, x24, [sp, #-16]!
    48702b2c:	a9bf5bf5 	stp	x21, x22, [sp, #-16]!
    48702b30:	a9bf53f3 	stp	x19, x20, [sp, #-16]!
    48702b34:	a9bf4bf1 	stp	x17, x18, [sp, #-16]!
    48702b38:	a9bf43ef 	stp	x15, x16, [sp, #-16]!
    48702b3c:	a9bf3bed 	stp	x13, x14, [sp, #-16]!
    48702b40:	a9bf33eb 	stp	x11, x12, [sp, #-16]!
    48702b44:	a9bf2be9 	stp	x9, x10, [sp, #-16]!
    48702b48:	a9bf23e7 	stp	x7, x8, [sp, #-16]!
    48702b4c:	a9bf1be5 	stp	x5, x6, [sp, #-16]!
    48702b50:	a9bf13e3 	stp	x3, x4, [sp, #-16]!
    48702b54:	a9bf0be1 	stp	x1, x2, [sp, #-16]!
    48702b58:	d538424b 	mrs	x11, currentel
    48702b5c:	f100317f 	cmp	x11, #0xc
    48702b60:	540000a0 	b.eq	48702b74 <_do_bad_irq+0x58>  // b.none
    48702b64:	f100217f 	cmp	x11, #0x8
    48702b68:	540000c0 	b.eq	48702b80 <_do_bad_irq+0x64>  // b.none
    48702b6c:	f100117f 	cmp	x11, #0x4
    48702b70:	540000e0 	b.eq	48702b8c <_do_bad_irq+0x70>  // b.none
    48702b74:	d53e5201 	mrs	x1, esr_el3
    48702b78:	d53e4022 	mrs	x2, elr_el3
    48702b7c:	14000006 	b	48702b94 <_do_bad_irq+0x78>
    48702b80:	d53c5201 	mrs	x1, esr_el2
    48702b84:	d53c4022 	mrs	x2, elr_el2
    48702b88:	14000003 	b	48702b94 <_do_bad_irq+0x78>
    48702b8c:	d5385201 	mrs	x1, esr_el1
    48702b90:	d5384022 	mrs	x2, elr_el1
    48702b94:	a9bf03e2 	stp	x2, x0, [sp, #-16]!
    48702b98:	910003e0 	mov	x0, sp
    48702b9c:	940013ad 	bl	48707a50 <do_bad_irq>
    48702ba0:	140000cd 	b	48702ed4 <exception_exit>

0000000048702ba4 <_do_bad_fiq>:
    48702ba4:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48702ba8:	a9bf73fb 	stp	x27, x28, [sp, #-16]!
    48702bac:	a9bf6bf9 	stp	x25, x26, [sp, #-16]!
    48702bb0:	a9bf63f7 	stp	x23, x24, [sp, #-16]!
    48702bb4:	a9bf5bf5 	stp	x21, x22, [sp, #-16]!
    48702bb8:	a9bf53f3 	stp	x19, x20, [sp, #-16]!
    48702bbc:	a9bf4bf1 	stp	x17, x18, [sp, #-16]!
    48702bc0:	a9bf43ef 	stp	x15, x16, [sp, #-16]!
    48702bc4:	a9bf3bed 	stp	x13, x14, [sp, #-16]!
    48702bc8:	a9bf33eb 	stp	x11, x12, [sp, #-16]!
    48702bcc:	a9bf2be9 	stp	x9, x10, [sp, #-16]!
    48702bd0:	a9bf23e7 	stp	x7, x8, [sp, #-16]!
    48702bd4:	a9bf1be5 	stp	x5, x6, [sp, #-16]!
    48702bd8:	a9bf13e3 	stp	x3, x4, [sp, #-16]!
    48702bdc:	a9bf0be1 	stp	x1, x2, [sp, #-16]!
    48702be0:	d538424b 	mrs	x11, currentel
    48702be4:	f100317f 	cmp	x11, #0xc
    48702be8:	540000a0 	b.eq	48702bfc <_do_bad_fiq+0x58>  // b.none
    48702bec:	f100217f 	cmp	x11, #0x8
    48702bf0:	540000c0 	b.eq	48702c08 <_do_bad_fiq+0x64>  // b.none
    48702bf4:	f100117f 	cmp	x11, #0x4
    48702bf8:	540000e0 	b.eq	48702c14 <_do_bad_fiq+0x70>  // b.none
    48702bfc:	d53e5201 	mrs	x1, esr_el3
    48702c00:	d53e4022 	mrs	x2, elr_el3
    48702c04:	14000006 	b	48702c1c <_do_bad_fiq+0x78>
    48702c08:	d53c5201 	mrs	x1, esr_el2
    48702c0c:	d53c4022 	mrs	x2, elr_el2
    48702c10:	14000003 	b	48702c1c <_do_bad_fiq+0x78>
    48702c14:	d5385201 	mrs	x1, esr_el1
    48702c18:	d5384022 	mrs	x2, elr_el1
    48702c1c:	a9bf03e2 	stp	x2, x0, [sp, #-16]!
    48702c20:	910003e0 	mov	x0, sp
    48702c24:	94001380 	bl	48707a24 <do_bad_fiq>
    48702c28:	140000ab 	b	48702ed4 <exception_exit>

0000000048702c2c <_do_bad_error>:
    48702c2c:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48702c30:	a9bf73fb 	stp	x27, x28, [sp, #-16]!
    48702c34:	a9bf6bf9 	stp	x25, x26, [sp, #-16]!
    48702c38:	a9bf63f7 	stp	x23, x24, [sp, #-16]!
    48702c3c:	a9bf5bf5 	stp	x21, x22, [sp, #-16]!
    48702c40:	a9bf53f3 	stp	x19, x20, [sp, #-16]!
    48702c44:	a9bf4bf1 	stp	x17, x18, [sp, #-16]!
    48702c48:	a9bf43ef 	stp	x15, x16, [sp, #-16]!
    48702c4c:	a9bf3bed 	stp	x13, x14, [sp, #-16]!
    48702c50:	a9bf33eb 	stp	x11, x12, [sp, #-16]!
    48702c54:	a9bf2be9 	stp	x9, x10, [sp, #-16]!
    48702c58:	a9bf23e7 	stp	x7, x8, [sp, #-16]!
    48702c5c:	a9bf1be5 	stp	x5, x6, [sp, #-16]!
    48702c60:	a9bf13e3 	stp	x3, x4, [sp, #-16]!
    48702c64:	a9bf0be1 	stp	x1, x2, [sp, #-16]!
    48702c68:	d538424b 	mrs	x11, currentel
    48702c6c:	f100317f 	cmp	x11, #0xc
    48702c70:	540000a0 	b.eq	48702c84 <_do_bad_error+0x58>  // b.none
    48702c74:	f100217f 	cmp	x11, #0x8
    48702c78:	540000c0 	b.eq	48702c90 <_do_bad_error+0x64>  // b.none
    48702c7c:	f100117f 	cmp	x11, #0x4
    48702c80:	540000e0 	b.eq	48702c9c <_do_bad_error+0x70>  // b.none
    48702c84:	d53e5201 	mrs	x1, esr_el3
    48702c88:	d53e4022 	mrs	x2, elr_el3
    48702c8c:	14000006 	b	48702ca4 <_do_bad_error+0x78>
    48702c90:	d53c5201 	mrs	x1, esr_el2
    48702c94:	d53c4022 	mrs	x2, elr_el2
    48702c98:	14000003 	b	48702ca4 <_do_bad_error+0x78>
    48702c9c:	d5385201 	mrs	x1, esr_el1
    48702ca0:	d5384022 	mrs	x2, elr_el1
    48702ca4:	a9bf03e2 	stp	x2, x0, [sp, #-16]!
    48702ca8:	910003e0 	mov	x0, sp
    48702cac:	94001348 	bl	487079cc <do_bad_error>
    48702cb0:	14000089 	b	48702ed4 <exception_exit>

0000000048702cb4 <_do_sync>:
    48702cb4:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48702cb8:	a9bf73fb 	stp	x27, x28, [sp, #-16]!
    48702cbc:	a9bf6bf9 	stp	x25, x26, [sp, #-16]!
    48702cc0:	a9bf63f7 	stp	x23, x24, [sp, #-16]!
    48702cc4:	a9bf5bf5 	stp	x21, x22, [sp, #-16]!
    48702cc8:	a9bf53f3 	stp	x19, x20, [sp, #-16]!
    48702ccc:	a9bf4bf1 	stp	x17, x18, [sp, #-16]!
    48702cd0:	a9bf43ef 	stp	x15, x16, [sp, #-16]!
    48702cd4:	a9bf3bed 	stp	x13, x14, [sp, #-16]!
    48702cd8:	a9bf33eb 	stp	x11, x12, [sp, #-16]!
    48702cdc:	a9bf2be9 	stp	x9, x10, [sp, #-16]!
    48702ce0:	a9bf23e7 	stp	x7, x8, [sp, #-16]!
    48702ce4:	a9bf1be5 	stp	x5, x6, [sp, #-16]!
    48702ce8:	a9bf13e3 	stp	x3, x4, [sp, #-16]!
    48702cec:	a9bf0be1 	stp	x1, x2, [sp, #-16]!
    48702cf0:	d538424b 	mrs	x11, currentel
    48702cf4:	f100317f 	cmp	x11, #0xc
    48702cf8:	540000a0 	b.eq	48702d0c <_do_sync+0x58>  // b.none
    48702cfc:	f100217f 	cmp	x11, #0x8
    48702d00:	540000c0 	b.eq	48702d18 <_do_sync+0x64>  // b.none
    48702d04:	f100117f 	cmp	x11, #0x4
    48702d08:	540000e0 	b.eq	48702d24 <_do_sync+0x70>  // b.none
    48702d0c:	d53e5201 	mrs	x1, esr_el3
    48702d10:	d53e4022 	mrs	x2, elr_el3
    48702d14:	14000006 	b	48702d2c <_do_sync+0x78>
    48702d18:	d53c5201 	mrs	x1, esr_el2
    48702d1c:	d53c4022 	mrs	x2, elr_el2
    48702d20:	14000003 	b	48702d2c <_do_sync+0x78>
    48702d24:	d5385201 	mrs	x1, esr_el1
    48702d28:	d5384022 	mrs	x2, elr_el1
    48702d2c:	a9bf03e2 	stp	x2, x0, [sp, #-16]!
    48702d30:	910003e0 	mov	x0, sp
    48702d34:	9400131b 	bl	487079a0 <do_sync>
    48702d38:	14000067 	b	48702ed4 <exception_exit>

0000000048702d3c <_do_irq>:
    48702d3c:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48702d40:	a9bf73fb 	stp	x27, x28, [sp, #-16]!
    48702d44:	a9bf6bf9 	stp	x25, x26, [sp, #-16]!
    48702d48:	a9bf63f7 	stp	x23, x24, [sp, #-16]!
    48702d4c:	a9bf5bf5 	stp	x21, x22, [sp, #-16]!
    48702d50:	a9bf53f3 	stp	x19, x20, [sp, #-16]!
    48702d54:	a9bf4bf1 	stp	x17, x18, [sp, #-16]!
    48702d58:	a9bf43ef 	stp	x15, x16, [sp, #-16]!
    48702d5c:	a9bf3bed 	stp	x13, x14, [sp, #-16]!
    48702d60:	a9bf33eb 	stp	x11, x12, [sp, #-16]!
    48702d64:	a9bf2be9 	stp	x9, x10, [sp, #-16]!
    48702d68:	a9bf23e7 	stp	x7, x8, [sp, #-16]!
    48702d6c:	a9bf1be5 	stp	x5, x6, [sp, #-16]!
    48702d70:	a9bf13e3 	stp	x3, x4, [sp, #-16]!
    48702d74:	a9bf0be1 	stp	x1, x2, [sp, #-16]!
    48702d78:	d538424b 	mrs	x11, currentel
    48702d7c:	f100317f 	cmp	x11, #0xc
    48702d80:	540000a0 	b.eq	48702d94 <_do_irq+0x58>  // b.none
    48702d84:	f100217f 	cmp	x11, #0x8
    48702d88:	540000c0 	b.eq	48702da0 <_do_irq+0x64>  // b.none
    48702d8c:	f100117f 	cmp	x11, #0x4
    48702d90:	540000e0 	b.eq	48702dac <_do_irq+0x70>  // b.none
    48702d94:	d53e5201 	mrs	x1, esr_el3
    48702d98:	d53e4022 	mrs	x2, elr_el3
    48702d9c:	14000006 	b	48702db4 <_do_irq+0x78>
    48702da0:	d53c5201 	mrs	x1, esr_el2
    48702da4:	d53c4022 	mrs	x2, elr_el2
    48702da8:	14000003 	b	48702db4 <_do_irq+0x78>
    48702dac:	d5385201 	mrs	x1, esr_el1
    48702db0:	d5384022 	mrs	x2, elr_el1
    48702db4:	a9bf03e2 	stp	x2, x0, [sp, #-16]!
    48702db8:	910003e0 	mov	x0, sp
    48702dbc:	9400133b 	bl	48707aa8 <do_irq>
    48702dc0:	14000045 	b	48702ed4 <exception_exit>

0000000048702dc4 <_do_fiq>:
    48702dc4:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48702dc8:	a9bf73fb 	stp	x27, x28, [sp, #-16]!
    48702dcc:	a9bf6bf9 	stp	x25, x26, [sp, #-16]!
    48702dd0:	a9bf63f7 	stp	x23, x24, [sp, #-16]!
    48702dd4:	a9bf5bf5 	stp	x21, x22, [sp, #-16]!
    48702dd8:	a9bf53f3 	stp	x19, x20, [sp, #-16]!
    48702ddc:	a9bf4bf1 	stp	x17, x18, [sp, #-16]!
    48702de0:	a9bf43ef 	stp	x15, x16, [sp, #-16]!
    48702de4:	a9bf3bed 	stp	x13, x14, [sp, #-16]!
    48702de8:	a9bf33eb 	stp	x11, x12, [sp, #-16]!
    48702dec:	a9bf2be9 	stp	x9, x10, [sp, #-16]!
    48702df0:	a9bf23e7 	stp	x7, x8, [sp, #-16]!
    48702df4:	a9bf1be5 	stp	x5, x6, [sp, #-16]!
    48702df8:	a9bf13e3 	stp	x3, x4, [sp, #-16]!
    48702dfc:	a9bf0be1 	stp	x1, x2, [sp, #-16]!
    48702e00:	d538424b 	mrs	x11, currentel
    48702e04:	f100317f 	cmp	x11, #0xc
    48702e08:	540000a0 	b.eq	48702e1c <_do_fiq+0x58>  // b.none
    48702e0c:	f100217f 	cmp	x11, #0x8
    48702e10:	540000c0 	b.eq	48702e28 <_do_fiq+0x64>  // b.none
    48702e14:	f100117f 	cmp	x11, #0x4
    48702e18:	540000e0 	b.eq	48702e34 <_do_fiq+0x70>  // b.none
    48702e1c:	d53e5201 	mrs	x1, esr_el3
    48702e20:	d53e4022 	mrs	x2, elr_el3
    48702e24:	14000006 	b	48702e3c <_do_fiq+0x78>
    48702e28:	d53c5201 	mrs	x1, esr_el2
    48702e2c:	d53c4022 	mrs	x2, elr_el2
    48702e30:	14000003 	b	48702e3c <_do_fiq+0x78>
    48702e34:	d5385201 	mrs	x1, esr_el1
    48702e38:	d5384022 	mrs	x2, elr_el1
    48702e3c:	a9bf03e2 	stp	x2, x0, [sp, #-16]!
    48702e40:	910003e0 	mov	x0, sp
    48702e44:	9400130e 	bl	48707a7c <do_fiq>
    48702e48:	14000023 	b	48702ed4 <exception_exit>

0000000048702e4c <_do_error>:
    48702e4c:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48702e50:	a9bf73fb 	stp	x27, x28, [sp, #-16]!
    48702e54:	a9bf6bf9 	stp	x25, x26, [sp, #-16]!
    48702e58:	a9bf63f7 	stp	x23, x24, [sp, #-16]!
    48702e5c:	a9bf5bf5 	stp	x21, x22, [sp, #-16]!
    48702e60:	a9bf53f3 	stp	x19, x20, [sp, #-16]!
    48702e64:	a9bf4bf1 	stp	x17, x18, [sp, #-16]!
    48702e68:	a9bf43ef 	stp	x15, x16, [sp, #-16]!
    48702e6c:	a9bf3bed 	stp	x13, x14, [sp, #-16]!
    48702e70:	a9bf33eb 	stp	x11, x12, [sp, #-16]!
    48702e74:	a9bf2be9 	stp	x9, x10, [sp, #-16]!
    48702e78:	a9bf23e7 	stp	x7, x8, [sp, #-16]!
    48702e7c:	a9bf1be5 	stp	x5, x6, [sp, #-16]!
    48702e80:	a9bf13e3 	stp	x3, x4, [sp, #-16]!
    48702e84:	a9bf0be1 	stp	x1, x2, [sp, #-16]!
    48702e88:	d538424b 	mrs	x11, currentel
    48702e8c:	f100317f 	cmp	x11, #0xc
    48702e90:	540000a0 	b.eq	48702ea4 <_do_error+0x58>  // b.none
    48702e94:	f100217f 	cmp	x11, #0x8
    48702e98:	540000c0 	b.eq	48702eb0 <_do_error+0x64>  // b.none
    48702e9c:	f100117f 	cmp	x11, #0x4
    48702ea0:	540000e0 	b.eq	48702ebc <_do_error+0x70>  // b.none
    48702ea4:	d53e5201 	mrs	x1, esr_el3
    48702ea8:	d53e4022 	mrs	x2, elr_el3
    48702eac:	14000006 	b	48702ec4 <_do_error+0x78>
    48702eb0:	d53c5201 	mrs	x1, esr_el2
    48702eb4:	d53c4022 	mrs	x2, elr_el2
    48702eb8:	14000003 	b	48702ec4 <_do_error+0x78>
    48702ebc:	d5385201 	mrs	x1, esr_el1
    48702ec0:	d5384022 	mrs	x2, elr_el1
    48702ec4:	a9bf03e2 	stp	x2, x0, [sp, #-16]!
    48702ec8:	910003e0 	mov	x0, sp
    48702ecc:	940012cb 	bl	487079f8 <do_error>
    48702ed0:	14000001 	b	48702ed4 <exception_exit>

0000000048702ed4 <exception_exit>:
    48702ed4:	a8c103e2 	ldp	x2, x0, [sp], #16
    48702ed8:	d538424b 	mrs	x11, currentel
    48702edc:	f100317f 	cmp	x11, #0xc
    48702ee0:	540000a0 	b.eq	48702ef4 <exception_exit+0x20>  // b.none
    48702ee4:	f100217f 	cmp	x11, #0x8
    48702ee8:	540000a0 	b.eq	48702efc <exception_exit+0x28>  // b.none
    48702eec:	f100117f 	cmp	x11, #0x4
    48702ef0:	540000a0 	b.eq	48702f04 <exception_exit+0x30>  // b.none
    48702ef4:	d51e4022 	msr	elr_el3, x2
    48702ef8:	14000004 	b	48702f08 <exception_exit+0x34>
    48702efc:	d51c4022 	msr	elr_el2, x2
    48702f00:	14000002 	b	48702f08 <exception_exit+0x34>
    48702f04:	d5184022 	msr	elr_el1, x2
    48702f08:	a8c10be1 	ldp	x1, x2, [sp], #16
    48702f0c:	a8c113e3 	ldp	x3, x4, [sp], #16
    48702f10:	a8c11be5 	ldp	x5, x6, [sp], #16
    48702f14:	a8c123e7 	ldp	x7, x8, [sp], #16
    48702f18:	a8c12be9 	ldp	x9, x10, [sp], #16
    48702f1c:	a8c133eb 	ldp	x11, x12, [sp], #16
    48702f20:	a8c13bed 	ldp	x13, x14, [sp], #16
    48702f24:	a8c143ef 	ldp	x15, x16, [sp], #16
    48702f28:	a8c14bf1 	ldp	x17, x18, [sp], #16
    48702f2c:	a8c153f3 	ldp	x19, x20, [sp], #16
    48702f30:	a8c15bf5 	ldp	x21, x22, [sp], #16
    48702f34:	a8c163f7 	ldp	x23, x24, [sp], #16
    48702f38:	a8c16bf9 	ldp	x25, x26, [sp], #16
    48702f3c:	a8c173fb 	ldp	x27, x28, [sp], #16
    48702f40:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48702f44:	d69f03e0 	eret

0000000048702f48 <__asm_invalidate_icache_all>:
    48702f48:	d508711f 	ic	ialluis
    48702f4c:	d5033fdf 	isb
    48702f50:	d65f03c0 	ret
    48702f54:	d503201f 	nop

0000000048702f58 <Str_SystemSartup>:
    48702f58:	0a0d0a0d 	.word	0x0a0d0a0d
    48702f5c:	74737953 	.word	0x74737953
    48702f60:	73206d65 	.word	0x73206d65
    48702f64:	74726174 	.word	0x74726174
    48702f68:	0a0d7075 	.word	0x0a0d7075
    48702f6c:	00          	.byte	0x00
    48702f6d:	75000000 	.inst	0x75000000 ; undefined
    48702f71:	6a          	.byte	0x6a
    48702f72:	7a69      	.short	0x7a69
    48702f74:	dceff002 	.word	0xdceff002
    48702f78:	bcdff003 	.word	0xbcdff003
    48702f7c:	00000000 	.inst	0x00000000 ; undefined
    48702f80:	01800000 	.word	0x01800000
    48702f84:	00000000 	.word	0x00000000
    48702f88:	12020000 	.word	0x12020000
    48702f8c:	00000000 	.word	0x00000000
    48702f90:	08009000 	.word	0x08009000
    48702f94:	00000000 	.word	0x00000000
    48702f98:	41000000 	.word	0x41000000
    48702f9c:	00000000 	.word	0x00000000
    48702fa0:	08004000 	.word	0x08004000
    48702fa4:	00000000 	.word	0x00000000
    48702fa8:	48700000 	.word	0x48700000
    48702fac:	00000000 	.word	0x00000000
    48702fb0:	4875bda8 	.word	0x4875bda8
    48702fb4:	00000000 	.word	0x00000000

0000000048702fb8 <reg_read>:
    48702fb8:	b9400c02 	ldr	w2, [x0, #12]
    48702fbc:	b9400003 	ldr	w3, [x0]
    48702fc0:	531b7c44 	lsr	w4, w2, #27
    48702fc4:	d3535c42 	ubfx	x2, x2, #19, #5
    48702fc8:	11000442 	add	w2, w2, #0x1
    48702fcc:	b9400063 	ldr	w3, [x3]
    48702fd0:	7100805f 	cmp	w2, #0x20
    48702fd4:	540000c0 	b.eq	48702fec <reg_read+0x34>  // b.none
    48702fd8:	1ac42463 	lsr	w3, w3, w4
    48702fdc:	52800024 	mov	w4, #0x1                   	// #1
    48702fe0:	1ac22082 	lsl	w2, w4, w2
    48702fe4:	51000442 	sub	w2, w2, #0x1
    48702fe8:	0a020063 	and	w3, w3, w2
    48702fec:	b9400400 	ldr	w0, [x0, #4]
    48702ff0:	6b03001f 	cmp	w0, w3
    48702ff4:	1a9f07e0 	cset	w0, ne  // ne = any
    48702ff8:	b9000020 	str	w0, [x1]
    48702ffc:	d65f03c0 	ret

0000000048703000 <reg_write>:
    48703000:	29410805 	ldp	w5, w2, [x0, #8]
    48703004:	29400c06 	ldp	w6, w3, [x0]
    48703008:	d3431c44 	ubfx	x4, x2, #3, #5
    4870300c:	11000484 	add	w4, w4, #0x1
    48703010:	b94000c1 	ldr	w1, [x6]
    48703014:	7100809f 	cmp	w4, #0x20
    48703018:	54000101 	b.ne	48703038 <reg_write+0x38>  // b.any
    4870301c:	2a0303e1 	mov	w1, w3
    48703020:	b90000c1 	str	w1, [x6]
    48703024:	d503201f 	nop
    48703028:	510004a5 	sub	w5, w5, #0x1
    4870302c:	310004bf 	cmn	w5, #0x1
    48703030:	54ffffa1 	b.ne	48703024 <reg_write+0x24>  // b.any
    48703034:	d65f03c0 	ret
    48703038:	52800020 	mov	w0, #0x1                   	// #1
    4870303c:	d34b3c42 	ubfx	x2, x2, #11, #5
    48703040:	1ac42000 	lsl	w0, w0, w4
    48703044:	51000400 	sub	w0, w0, #0x1
    48703048:	1ac22000 	lsl	w0, w0, w2
    4870304c:	1ac22062 	lsl	w2, w3, w2
    48703050:	0a200021 	bic	w1, w1, w0
    48703054:	2a020021 	orr	w1, w1, w2
    48703058:	17fffff2 	b	48703020 <reg_write+0x20>

000000004870305c <init_registers>:
    4870305c:	52800007 	mov	w7, #0x0                   	// #0
    48703060:	aa0003e8 	mov	x8, x0
    48703064:	d37c7ce0 	ubfiz	x0, x7, #4, #32
    48703068:	2a0103ea 	mov	w10, w1
    4870306c:	8b000106 	add	x6, x8, x0
    48703070:	b8606901 	ldr	w1, [x8, x0]
    48703074:	294100c5 	ldp	w5, w0, [x6, #8]
    48703078:	350000a1 	cbnz	w1, 4870308c <init_registers+0x30>
    4870307c:	b94004c1 	ldr	w1, [x6, #4]
    48703080:	35000061 	cbnz	w1, 4870308c <init_registers+0x30>
    48703084:	35000045 	cbnz	w5, 4870308c <init_registers+0x30>
    48703088:	34000720 	cbz	w0, 4870316c <init_registers+0x110>
    4870308c:	a9be7bfd 	stp	x29, x30, [sp, #-32]!
    48703090:	910003fd 	mov	x29, sp
    48703094:	14000016 	b	487030ec <init_registers+0x90>
    48703098:	36880360 	tbz	w0, #17, 48703104 <init_registers+0xa8>
    4870309c:	910073a1 	add	x1, x29, #0x1c
    487030a0:	aa0603e0 	mov	x0, x6
    487030a4:	97ffffc5 	bl	48702fb8 <reg_read>
    487030a8:	d503201f 	nop
    487030ac:	b9401fa0 	ldr	w0, [x29, #28]
    487030b0:	35ffff60 	cbnz	w0, 4870309c <init_registers+0x40>
    487030b4:	d503201f 	nop
    487030b8:	510004a5 	sub	w5, w5, #0x1
    487030bc:	310004bf 	cmn	w5, #0x1
    487030c0:	54ffffa1 	b.ne	487030b4 <init_registers+0x58>  // b.any
    487030c4:	110004e7 	add	w7, w7, #0x1
    487030c8:	d37c7ce0 	ubfiz	x0, x7, #4, #32
    487030cc:	8b000106 	add	x6, x8, x0
    487030d0:	b8606901 	ldr	w1, [x8, x0]
    487030d4:	294100c5 	ldp	w5, w0, [x6, #8]
    487030d8:	350000a1 	cbnz	w1, 487030ec <init_registers+0x90>
    487030dc:	b94004c1 	ldr	w1, [x6, #4]
    487030e0:	35000061 	cbnz	w1, 487030ec <init_registers+0x90>
    487030e4:	35000045 	cbnz	w5, 487030ec <init_registers+0x90>
    487030e8:	340003c0 	cbz	w0, 48703160 <init_registers+0x104>
    487030ec:	b9001fbf 	str	wzr, [x29, #28]
    487030f0:	3400014a 	cbz	w10, 48703118 <init_registers+0xbc>
    487030f4:	360ffd20 	tbz	w0, #1, 48703098 <init_registers+0x3c>
    487030f8:	aa0603e0 	mov	x0, x6
    487030fc:	97ffffc1 	bl	48703000 <reg_write>
    48703100:	17fffff1 	b	487030c4 <init_registers+0x68>
    48703104:	d503201f 	nop
    48703108:	510004a5 	sub	w5, w5, #0x1
    4870310c:	310004bf 	cmn	w5, #0x1
    48703110:	54ffffa1 	b.ne	48703104 <init_registers+0xa8>  // b.any
    48703114:	17ffffec 	b	487030c4 <init_registers+0x68>
    48703118:	3717ff00 	tbnz	w0, #2, 487030f8 <init_registers+0x9c>
    4870311c:	36900180 	tbz	w0, #18, 4870314c <init_registers+0xf0>
    48703120:	910073a1 	add	x1, x29, #0x1c
    48703124:	aa0603e0 	mov	x0, x6
    48703128:	97ffffa4 	bl	48702fb8 <reg_read>
    4870312c:	d503201f 	nop
    48703130:	b9401fa0 	ldr	w0, [x29, #28]
    48703134:	35ffff60 	cbnz	w0, 48703120 <init_registers+0xc4>
    48703138:	d503201f 	nop
    4870313c:	510004a5 	sub	w5, w5, #0x1
    48703140:	310004bf 	cmn	w5, #0x1
    48703144:	54ffffa1 	b.ne	48703138 <init_registers+0xdc>  // b.any
    48703148:	17ffffdf 	b	487030c4 <init_registers+0x68>
    4870314c:	d503201f 	nop
    48703150:	510004a5 	sub	w5, w5, #0x1
    48703154:	310004bf 	cmn	w5, #0x1
    48703158:	54ffffa1 	b.ne	4870314c <init_registers+0xf0>  // b.any
    4870315c:	17ffffda 	b	487030c4 <init_registers+0x68>
    48703160:	d503201f 	nop
    48703164:	a8c27bfd 	ldp	x29, x30, [sp], #32
    48703168:	d65f03c0 	ret
    4870316c:	d503201f 	nop
    48703170:	d65f03c0 	ret

0000000048703174 <delay>:
    48703174:	d10043ff 	sub	sp, sp, #0x10
    48703178:	52800c81 	mov	w1, #0x64                  	// #100
    4870317c:	1b017c00 	mul	w0, w0, w1
    48703180:	b9000fff 	str	wzr, [sp, #12]
    48703184:	b9400fe1 	ldr	w1, [sp, #12]
    48703188:	6b01001f 	cmp	w0, w1
    4870318c:	54000068 	b.hi	48703198 <delay+0x24>  // b.pmore
    48703190:	910043ff 	add	sp, sp, #0x10
    48703194:	d65f03c0 	ret
    48703198:	d503201f 	nop
    4870319c:	b9400fe1 	ldr	w1, [sp, #12]
    487031a0:	11000421 	add	w1, w1, #0x1
    487031a4:	b9000fe1 	str	w1, [sp, #12]
    487031a8:	17fffff7 	b	48703184 <delay+0x10>

00000000487031ac <hpm_value_avg>:
    487031ac:	d2800001 	mov	x1, #0x0                   	// #0
    487031b0:	52800002 	mov	w2, #0x0                   	// #0
    487031b4:	b8616803 	ldr	w3, [x0, x1]
    487031b8:	91001021 	add	x1, x1, #0x4
    487031bc:	f100403f 	cmp	x1, #0x10
    487031c0:	0b030042 	add	w2, w2, w3
    487031c4:	54ffff81 	b.ne	487031b4 <hpm_value_avg+0x8>  // b.any
    487031c8:	53057c40 	lsr	w0, w2, #5
    487031cc:	d65f03c0 	ret

00000000487031d0 <calc_volt_regval>:
    487031d0:	51000423 	sub	w3, w1, #0x1
    487031d4:	6b01001f 	cmp	w0, w1
    487031d8:	1a802060 	csel	w0, w3, w0, cs  // cs = hs, nlast
    487031dc:	52803403 	mov	w3, #0x1a0                 	// #416
    487031e0:	6b02001f 	cmp	w0, w2
    487031e4:	1a828400 	csinc	w0, w0, w2, hi  // hi = pmore
    487031e8:	4b020022 	sub	w2, w1, w2
    487031ec:	4b000021 	sub	w1, w1, w0
    487031f0:	11000440 	add	w0, w2, #0x1
    487031f4:	53017c00 	lsr	w0, w0, #1
    487031f8:	1b030020 	madd	w0, w1, w3, w0
    487031fc:	129cc141 	mov	w1, #0xffff19f5            	// #-58891
    48703200:	1ac20800 	udiv	w0, w0, w2
    48703204:	53103c00 	lsl	w0, w0, #16
    48703208:	0b010000 	add	w0, w0, w1
    4870320c:	d65f03c0 	ret

0000000048703210 <ddr_scramb_start>:
    48703210:	d2801103 	mov	x3, #0x88                  	// #136
    48703214:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48703218:	f2a24063 	movk	x3, #0x1203, lsl #16
    4870321c:	d2801182 	mov	x2, #0x8c                  	// #140
    48703220:	910003fd 	mov	x29, sp
    48703224:	f2a24062 	movk	x2, #0x1203, lsl #16
    48703228:	b9000060 	str	w0, [x3]
    4870322c:	d2801200 	mov	x0, #0x90                  	// #144
    48703230:	f2a24060 	movk	x0, #0x1203, lsl #16
    48703234:	b9000041 	str	w1, [x2]
    48703238:	52800201 	mov	w1, #0x10                  	// #16
    4870323c:	b900001f 	str	wzr, [x0]
    48703240:	b9000001 	str	w1, [x0]
    48703244:	52802000 	mov	w0, #0x100                 	// #256
    48703248:	97ffffcb 	bl	48703174 <delay>
    4870324c:	b900007f 	str	wzr, [x3]
    48703250:	b900005f 	str	wzr, [x2]
    48703254:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48703258:	d65f03c0 	ret

000000004870325c <get_random_num>:
    4870325c:	d2804101 	mov	x1, #0x208                 	// #520
    48703260:	f2a20461 	movk	x1, #0x1023, lsl #16
    48703264:	b9400020 	ldr	w0, [x1]
    48703268:	d3483c00 	ubfx	x0, x0, #8, #8
    4870326c:	34ffffc0 	cbz	w0, 48703264 <get_random_num+0x8>
    48703270:	d2804080 	mov	x0, #0x204                 	// #516
    48703274:	f2a20460 	movk	x0, #0x1023, lsl #16
    48703278:	b9400000 	ldr	w0, [x0]
    4870327c:	d65f03c0 	ret

0000000048703280 <trng_init>:
    48703280:	d2803281 	mov	x1, #0x194                 	// #404
    48703284:	f2a24021 	movk	x1, #0x1201, lsl #16
    48703288:	b9400020 	ldr	w0, [x1]
    4870328c:	32130000 	orr	w0, w0, #0x2000
    48703290:	b9000020 	str	w0, [x1]
    48703294:	d2804000 	mov	x0, #0x200                 	// #512
    48703298:	f2a20460 	movk	x0, #0x1023, lsl #16
    4870329c:	52800141 	mov	w1, #0xa                   	// #10
    487032a0:	b9000001 	str	w1, [x0]
    487032a4:	d65f03c0 	ret

00000000487032a8 <trng_deinit>:
    487032a8:	d2803281 	mov	x1, #0x194                 	// #404
    487032ac:	f2a24021 	movk	x1, #0x1201, lsl #16
    487032b0:	b9400020 	ldr	w0, [x1]
    487032b4:	12127800 	and	w0, w0, #0xffffdfff
    487032b8:	b9000020 	str	w0, [x1]
    487032bc:	d65f03c0 	ret

00000000487032c0 <ddr_scramb>:
    487032c0:	d2900a00 	mov	x0, #0x8050                	// #32848
    487032c4:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    487032c8:	f2a240c0 	movk	x0, #0x1206, lsl #16
    487032cc:	910003fd 	mov	x29, sp
    487032d0:	b9400004 	ldr	w4, [x0]
    487032d4:	b9500005 	ldr	w5, [x0, #4096]
    487032d8:	b9600006 	ldr	w6, [x0, #8192]
    487032dc:	72000c84 	ands	w4, w4, #0xf
    487032e0:	b9700007 	ldr	w7, [x0, #12288]
    487032e4:	12000ca5 	and	w5, w5, #0xf
    487032e8:	12000cc6 	and	w6, w6, #0xf
    487032ec:	12000ce7 	and	w7, w7, #0xf
    487032f0:	540000a0 	b.eq	48703304 <ddr_scramb+0x44>  // b.none
    487032f4:	d2900000 	mov	x0, #0x8000                	// #32768
    487032f8:	52800021 	mov	w1, #0x1                   	// #1
    487032fc:	f2a240c0 	movk	x0, #0x1206, lsl #16
    48703300:	b9000001 	str	w1, [x0]
    48703304:	340000a5 	cbz	w5, 48703318 <ddr_scramb+0x58>
    48703308:	d2920000 	mov	x0, #0x9000                	// #36864
    4870330c:	52800021 	mov	w1, #0x1                   	// #1
    48703310:	f2a240c0 	movk	x0, #0x1206, lsl #16
    48703314:	b9000001 	str	w1, [x0]
    48703318:	340000a6 	cbz	w6, 4870332c <ddr_scramb+0x6c>
    4870331c:	d2940000 	mov	x0, #0xa000                	// #40960
    48703320:	52800021 	mov	w1, #0x1                   	// #1
    48703324:	f2a240c0 	movk	x0, #0x1206, lsl #16
    48703328:	b9000001 	str	w1, [x0]
    4870332c:	340000a7 	cbz	w7, 48703340 <ddr_scramb+0x80>
    48703330:	d2960000 	mov	x0, #0xb000                	// #45056
    48703334:	52800021 	mov	w1, #0x1                   	// #1
    48703338:	f2a240c0 	movk	x0, #0x1206, lsl #16
    4870333c:	b9000001 	str	w1, [x0]
    48703340:	d2905283 	mov	x3, #0x8294                	// #33428
    48703344:	d2925288 	mov	x8, #0x9294                	// #37524
    48703348:	d294528a 	mov	x10, #0xa294                	// #41620
    4870334c:	d296528b 	mov	x11, #0xb294                	// #45716
    48703350:	f2a240c3 	movk	x3, #0x1206, lsl #16
    48703354:	f2a240c8 	movk	x8, #0x1206, lsl #16
    48703358:	f2a240ca 	movk	x10, #0x1206, lsl #16
    4870335c:	f2a240cb 	movk	x11, #0x1206, lsl #16
    48703360:	52800020 	mov	w0, #0x1                   	// #1
    48703364:	34000064 	cbz	w4, 48703370 <ddr_scramb+0xb0>
    48703368:	b9400060 	ldr	w0, [x3]
    4870336c:	12000000 	and	w0, w0, #0x1
    48703370:	52800021 	mov	w1, #0x1                   	// #1
    48703374:	34000065 	cbz	w5, 48703380 <ddr_scramb+0xc0>
    48703378:	b9400101 	ldr	w1, [x8]
    4870337c:	12000021 	and	w1, w1, #0x1
    48703380:	0a010000 	and	w0, w0, w1
    48703384:	52800021 	mov	w1, #0x1                   	// #1
    48703388:	34000066 	cbz	w6, 48703394 <ddr_scramb+0xd4>
    4870338c:	b9400141 	ldr	w1, [x10]
    48703390:	12000021 	and	w1, w1, #0x1
    48703394:	52800022 	mov	w2, #0x1                   	// #1
    48703398:	34000067 	cbz	w7, 487033a4 <ddr_scramb+0xe4>
    4870339c:	b9400162 	ldr	w2, [x11]
    487033a0:	12000042 	and	w2, w2, #0x1
    487033a4:	0a020021 	and	w1, w1, w2
    487033a8:	6a00003f 	tst	w1, w0
    487033ac:	54fffda0 	b.eq	48703360 <ddr_scramb+0xa0>  // b.none
    487033b0:	97ffffb4 	bl	48703280 <trng_init>
    487033b4:	97ffffaa 	bl	4870325c <get_random_num>
    487033b8:	2a0003e2 	mov	w2, w0
    487033bc:	97ffffa8 	bl	4870325c <get_random_num>
    487033c0:	2a0003e1 	mov	w1, w0
    487033c4:	2a0203e0 	mov	w0, w2
    487033c8:	97ffff92 	bl	48703210 <ddr_scramb_start>
    487033cc:	97ffffa4 	bl	4870325c <get_random_num>
    487033d0:	97ffffa3 	bl	4870325c <get_random_num>
    487033d4:	97ffffa2 	bl	4870325c <get_random_num>
    487033d8:	97ffffa1 	bl	4870325c <get_random_num>
    487033dc:	97ffffb3 	bl	487032a8 <trng_deinit>
    487033e0:	340000a4 	cbz	w4, 487033f4 <ddr_scramb+0x134>
    487033e4:	d2900000 	mov	x0, #0x8000                	// #32768
    487033e8:	52800041 	mov	w1, #0x2                   	// #2
    487033ec:	f2a240c0 	movk	x0, #0x1206, lsl #16
    487033f0:	b9000001 	str	w1, [x0]
    487033f4:	340000a5 	cbz	w5, 48703408 <ddr_scramb+0x148>
    487033f8:	d2920000 	mov	x0, #0x9000                	// #36864
    487033fc:	52800041 	mov	w1, #0x2                   	// #2
    48703400:	f2a240c0 	movk	x0, #0x1206, lsl #16
    48703404:	b9000001 	str	w1, [x0]
    48703408:	340000a6 	cbz	w6, 4870341c <ddr_scramb+0x15c>
    4870340c:	d2940000 	mov	x0, #0xa000                	// #40960
    48703410:	52800041 	mov	w1, #0x2                   	// #2
    48703414:	f2a240c0 	movk	x0, #0x1206, lsl #16
    48703418:	b9000001 	str	w1, [x0]
    4870341c:	340000a7 	cbz	w7, 48703430 <ddr_scramb+0x170>
    48703420:	d2960000 	mov	x0, #0xb000                	// #45056
    48703424:	52800041 	mov	w1, #0x2                   	// #2
    48703428:	f2a240c0 	movk	x0, #0x1206, lsl #16
    4870342c:	b9000001 	str	w1, [x0]
    48703430:	d2905283 	mov	x3, #0x8294                	// #33428
    48703434:	d2925288 	mov	x8, #0x9294                	// #37524
    48703438:	d294528a 	mov	x10, #0xa294                	// #41620
    4870343c:	d296528b 	mov	x11, #0xb294                	// #45716
    48703440:	f2a240c3 	movk	x3, #0x1206, lsl #16
    48703444:	f2a240c8 	movk	x8, #0x1206, lsl #16
    48703448:	f2a240ca 	movk	x10, #0x1206, lsl #16
    4870344c:	f2a240cb 	movk	x11, #0x1206, lsl #16
    48703450:	52800000 	mov	w0, #0x0                   	// #0
    48703454:	34000064 	cbz	w4, 48703460 <ddr_scramb+0x1a0>
    48703458:	b9400060 	ldr	w0, [x3]
    4870345c:	12000000 	and	w0, w0, #0x1
    48703460:	52800001 	mov	w1, #0x0                   	// #0
    48703464:	34000065 	cbz	w5, 48703470 <ddr_scramb+0x1b0>
    48703468:	b9400101 	ldr	w1, [x8]
    4870346c:	12000021 	and	w1, w1, #0x1
    48703470:	2a010001 	orr	w1, w0, w1
    48703474:	52800000 	mov	w0, #0x0                   	// #0
    48703478:	34000066 	cbz	w6, 48703484 <ddr_scramb+0x1c4>
    4870347c:	b9400140 	ldr	w0, [x10]
    48703480:	12000000 	and	w0, w0, #0x1
    48703484:	52800002 	mov	w2, #0x0                   	// #0
    48703488:	34000067 	cbz	w7, 48703494 <ddr_scramb+0x1d4>
    4870348c:	b9400162 	ldr	w2, [x11]
    48703490:	12000042 	and	w2, w2, #0x1
    48703494:	2a020000 	orr	w0, w0, w2
    48703498:	2a010000 	orr	w0, w0, w1
    4870349c:	35fffda0 	cbnz	w0, 48703450 <ddr_scramb+0x190>
    487034a0:	a8c17bfd 	ldp	x29, x30, [sp], #16
    487034a4:	d65f03c0 	ret

00000000487034a8 <start_ddr_training>:
    487034a8:	d2822081 	mov	x1, #0x1104                	// #4356
    487034ac:	a9bb7bfd 	stp	x29, x30, [sp, #-80]!
    487034b0:	f2a3b041 	movk	x1, #0x1d82, lsl #16
    487034b4:	d2824107 	mov	x7, #0x1208                	// #4616
    487034b8:	910003fd 	mov	x29, sp
    487034bc:	d2824186 	mov	x6, #0x120c                	// #4620
    487034c0:	d2824305 	mov	x5, #0x1218                	// #4632
    487034c4:	b9400020 	ldr	w0, [x1]
    487034c8:	d2824384 	mov	x4, #0x121c                	// #4636
    487034cc:	d2824503 	mov	x3, #0x1228                	// #4648
    487034d0:	f2a24067 	movk	x7, #0x1203, lsl #16
    487034d4:	121c7800 	and	w0, w0, #0xfffffff7
    487034d8:	f2a24066 	movk	x6, #0x1203, lsl #16
    487034dc:	b9000020 	str	w0, [x1]
    487034e0:	d2801601 	mov	x1, #0xb0                  	// #176
    487034e4:	f2a24061 	movk	x1, #0x1203, lsl #16
    487034e8:	f2a24065 	movk	x5, #0x1203, lsl #16
    487034ec:	f2a24064 	movk	x4, #0x1203, lsl #16
    487034f0:	f2a24063 	movk	x3, #0x1203, lsl #16
    487034f4:	b9400020 	ldr	w0, [x1]
    487034f8:	320f0002 	orr	w2, w0, #0x20000
    487034fc:	120f7800 	and	w0, w0, #0xfffeffff
    48703500:	b9000022 	str	w2, [x1]
    48703504:	320f0002 	orr	w2, w0, #0x20000
    48703508:	d2803380 	mov	x0, #0x19c                 	// #412
    4870350c:	b9000022 	str	w2, [x1]
    48703510:	f2a24020 	movk	x0, #0x1201, lsl #16
    48703514:	52a0ffe1 	mov	w1, #0x7ff0000             	// #134152192
    48703518:	52800102 	mov	w2, #0x8                   	// #8
    4870351c:	b9000001 	str	w1, [x0]
    48703520:	d2824001 	mov	x1, #0x1200                	// #4608
    48703524:	f2a24061 	movk	x1, #0x1203, lsl #16
    48703528:	52800020 	mov	w0, #0x1                   	// #1
    4870352c:	72ac0040 	movk	w0, #0x6002, lsl #16
    48703530:	b9000020 	str	w0, [x1]
    48703534:	b9001020 	str	w0, [x1, #16]
    48703538:	b9002020 	str	w0, [x1, #32]
    4870353c:	b9003020 	str	w0, [x1, #48]
    48703540:	29027fbf 	stp	wzr, wzr, [x29, #16]
    48703544:	29037fbf 	stp	wzr, wzr, [x29, #24]
    48703548:	29047fbf 	stp	wzr, wzr, [x29, #32]
    4870354c:	29057fbf 	stp	wzr, wzr, [x29, #40]
    48703550:	29067fbf 	stp	wzr, wzr, [x29, #48]
    48703554:	29077fbf 	stp	wzr, wzr, [x29, #56]
    48703558:	29087fbf 	stp	wzr, wzr, [x29, #64]
    4870355c:	29097fbf 	stp	wzr, wzr, [x29, #72]
    48703560:	528000a0 	mov	w0, #0x5                   	// #5
    48703564:	97ffff04 	bl	48703174 <delay>
    48703568:	b94000e0 	ldr	w0, [x7]
    4870356c:	b94037a1 	ldr	w1, [x29, #52]
    48703570:	71000442 	subs	w2, w2, #0x1
    48703574:	d3506408 	ubfx	x8, x0, #16, #10
    48703578:	12002400 	and	w0, w0, #0x3ff
    4870357c:	0b080021 	add	w1, w1, w8
    48703580:	b90037a1 	str	w1, [x29, #52]
    48703584:	b94033a1 	ldr	w1, [x29, #48]
    48703588:	0b000020 	add	w0, w1, w0
    4870358c:	b9403fa1 	ldr	w1, [x29, #60]
    48703590:	b90033a0 	str	w0, [x29, #48]
    48703594:	b94000c0 	ldr	w0, [x6]
    48703598:	d3506408 	ubfx	x8, x0, #16, #10
    4870359c:	12002400 	and	w0, w0, #0x3ff
    487035a0:	0b080021 	add	w1, w1, w8
    487035a4:	b9003fa1 	str	w1, [x29, #60]
    487035a8:	b9403ba1 	ldr	w1, [x29, #56]
    487035ac:	0b000020 	add	w0, w1, w0
    487035b0:	b94027a1 	ldr	w1, [x29, #36]
    487035b4:	b9003ba0 	str	w0, [x29, #56]
    487035b8:	b94000a0 	ldr	w0, [x5]
    487035bc:	d3506408 	ubfx	x8, x0, #16, #10
    487035c0:	12002400 	and	w0, w0, #0x3ff
    487035c4:	0b080021 	add	w1, w1, w8
    487035c8:	b90027a1 	str	w1, [x29, #36]
    487035cc:	b94023a1 	ldr	w1, [x29, #32]
    487035d0:	0b000020 	add	w0, w1, w0
    487035d4:	b9402fa1 	ldr	w1, [x29, #44]
    487035d8:	b90023a0 	str	w0, [x29, #32]
    487035dc:	b9400080 	ldr	w0, [x4]
    487035e0:	d3506408 	ubfx	x8, x0, #16, #10
    487035e4:	12002400 	and	w0, w0, #0x3ff
    487035e8:	0b080021 	add	w1, w1, w8
    487035ec:	b9002fa1 	str	w1, [x29, #44]
    487035f0:	b9402ba1 	ldr	w1, [x29, #40]
    487035f4:	0b000020 	add	w0, w1, w0
    487035f8:	b94047a1 	ldr	w1, [x29, #68]
    487035fc:	b9002ba0 	str	w0, [x29, #40]
    48703600:	b9400060 	ldr	w0, [x3]
    48703604:	d3506408 	ubfx	x8, x0, #16, #10
    48703608:	12002400 	and	w0, w0, #0x3ff
    4870360c:	0b080021 	add	w1, w1, w8
    48703610:	b90047a1 	str	w1, [x29, #68]
    48703614:	b94043a1 	ldr	w1, [x29, #64]
    48703618:	0b000020 	add	w0, w1, w0
    4870361c:	b9404fa1 	ldr	w1, [x29, #76]
    48703620:	b90043a0 	str	w0, [x29, #64]
    48703624:	d2824580 	mov	x0, #0x122c                	// #4652
    48703628:	f2a24060 	movk	x0, #0x1203, lsl #16
    4870362c:	b9400000 	ldr	w0, [x0]
    48703630:	d3506408 	ubfx	x8, x0, #16, #10
    48703634:	12002400 	and	w0, w0, #0x3ff
    48703638:	0b080021 	add	w1, w1, w8
    4870363c:	b9004fa1 	str	w1, [x29, #76]
    48703640:	b9404ba1 	ldr	w1, [x29, #72]
    48703644:	0b000020 	add	w0, w1, w0
    48703648:	b94017a1 	ldr	w1, [x29, #20]
    4870364c:	b9004ba0 	str	w0, [x29, #72]
    48703650:	d2824700 	mov	x0, #0x1238                	// #4664
    48703654:	f2a24060 	movk	x0, #0x1203, lsl #16
    48703658:	b9400000 	ldr	w0, [x0]
    4870365c:	d3506408 	ubfx	x8, x0, #16, #10
    48703660:	12002400 	and	w0, w0, #0x3ff
    48703664:	0b080021 	add	w1, w1, w8
    48703668:	b90017a1 	str	w1, [x29, #20]
    4870366c:	b94013a1 	ldr	w1, [x29, #16]
    48703670:	0b000020 	add	w0, w1, w0
    48703674:	b9401fa1 	ldr	w1, [x29, #28]
    48703678:	b90013a0 	str	w0, [x29, #16]
    4870367c:	d2824780 	mov	x0, #0x123c                	// #4668
    48703680:	f2a24060 	movk	x0, #0x1203, lsl #16
    48703684:	b9400000 	ldr	w0, [x0]
    48703688:	d3506408 	ubfx	x8, x0, #16, #10
    4870368c:	12002400 	and	w0, w0, #0x3ff
    48703690:	0b080021 	add	w1, w1, w8
    48703694:	b9001fa1 	str	w1, [x29, #28]
    48703698:	b9401ba1 	ldr	w1, [x29, #24]
    4870369c:	0b000020 	add	w0, w1, w0
    487036a0:	b9001ba0 	str	w0, [x29, #24]
    487036a4:	54fff5e1 	b.ne	48703560 <start_ddr_training+0xb8>  // b.any
    487036a8:	910043a0 	add	x0, x29, #0x10
    487036ac:	97fffec0 	bl	487031ac <hpm_value_avg>
    487036b0:	2a0003eb 	mov	w11, w0
    487036b4:	910083a0 	add	x0, x29, #0x20
    487036b8:	97fffebd 	bl	487031ac <hpm_value_avg>
    487036bc:	2a0003ea 	mov	w10, w0
    487036c0:	9100c3a0 	add	x0, x29, #0x30
    487036c4:	97fffeba 	bl	487031ac <hpm_value_avg>
    487036c8:	2a0003e7 	mov	w7, w0
    487036cc:	910103a0 	add	x0, x29, #0x40
    487036d0:	97fffeb7 	bl	487031ac <hpm_value_avg>
    487036d4:	d2803381 	mov	x1, #0x19c                 	// #412
    487036d8:	f2a24021 	movk	x1, #0x1201, lsl #16
    487036dc:	b900003f 	str	wzr, [x1]
    487036e0:	d2801601 	mov	x1, #0xb0                  	// #176
    487036e4:	f2a24061 	movk	x1, #0x1203, lsl #16
    487036e8:	b9400022 	ldr	w2, [x1]
    487036ec:	32100043 	orr	w3, w2, #0x10000
    487036f0:	120e7842 	and	w2, w2, #0xfffdffff
    487036f4:	b9000023 	str	w3, [x1]
    487036f8:	32100043 	orr	w3, w2, #0x10000
    487036fc:	d2822082 	mov	x2, #0x1104                	// #4356
    48703700:	b9000023 	str	w3, [x1]
    48703704:	f2a3b042 	movk	x2, #0x1d82, lsl #16
    48703708:	b9400041 	ldr	w1, [x2]
    4870370c:	321d0021 	orr	w1, w1, #0x8
    48703710:	b9000041 	str	w1, [x2]
    48703714:	d2800f01 	mov	x1, #0x78                  	// #120
    48703718:	f2a24061 	movk	x1, #0x1203, lsl #16
    4870371c:	b9400022 	ldr	w2, [x1]
    48703720:	528014a1 	mov	w1, #0xa5                  	// #165
    48703724:	12002442 	and	w2, w2, #0x3ff
    48703728:	5101d042 	sub	w2, w2, #0x74
    4870372c:	1b017c42 	mul	w2, w2, w1
    48703730:	528064c1 	mov	w1, #0x326                 	// #806
    48703734:	1ac10c42 	sdiv	w2, w2, w1
    48703738:	d2800501 	mov	x1, #0x28                  	// #40
    4870373c:	f2a204a1 	movk	x1, #0x1025, lsl #16
    48703740:	b9400023 	ldr	w3, [x1]
    48703744:	5100a042 	sub	w2, w2, #0x28
    48703748:	b9400c26 	ldr	w6, [x1, #12]
    4870374c:	d2800601 	mov	x1, #0x30                  	// #48
    48703750:	f2a204a1 	movk	x1, #0x1025, lsl #16
    48703754:	7100005f 	cmp	w2, #0x0
    48703758:	b9400025 	ldr	w5, [x1]
    4870375c:	d2800581 	mov	x1, #0x2c                  	// #44
    48703760:	f2a204a1 	movk	x1, #0x1025, lsl #16
    48703764:	b9400024 	ldr	w4, [x1]
    48703768:	5400134c 	b.gt	487039d0 <start_ddr_training+0x528>
    4870376c:	51000d4a 	sub	w10, w10, #0x3
    48703770:	51001808 	sub	w8, w0, #0x6
    48703774:	510018e7 	sub	w7, w7, #0x6
    48703778:	51001161 	sub	w1, w11, #0x4
    4870377c:	340000a3 	cbz	w3, 48703790 <start_ddr_training+0x2e8>
    48703780:	11002860 	add	w0, w3, #0xa
    48703784:	51001463 	sub	w3, w3, #0x5
    48703788:	6b01001f 	cmp	w0, w1
    4870378c:	1a813061 	csel	w1, w3, w1, cc  // cc = lo, ul, last
    48703790:	34000086 	cbz	w6, 487037a0 <start_ddr_training+0x2f8>
    48703794:	11003cc0 	add	w0, w6, #0xf
    48703798:	6b07001f 	cmp	w0, w7
    4870379c:	1a8620e7 	csel	w7, w7, w6, cs  // cs = hs, nlast
    487037a0:	34000085 	cbz	w5, 487037b0 <start_ddr_training+0x308>
    487037a4:	11003ca0 	add	w0, w5, #0xf
    487037a8:	6b08001f 	cmp	w0, w8
    487037ac:	1a852108 	csel	w8, w8, w5, cs  // cs = hs, nlast
    487037b0:	34000084 	cbz	w4, 487037c0 <start_ddr_training+0x318>
    487037b4:	11003c80 	add	w0, w4, #0xf
    487037b8:	6b0a001f 	cmp	w0, w10
    487037bc:	1a84214a 	csel	w10, w10, w4, cs  // cs = hs, nlast
    487037c0:	d2800980 	mov	x0, #0x4c                  	// #76
    487037c4:	52800022 	mov	w2, #0x1                   	// #1
    487037c8:	f2a204a0 	movk	x0, #0x1025, lsl #16
    487037cc:	5280014b 	mov	w11, #0xa                   	// #10
    487037d0:	b9400004 	ldr	w4, [x0]
    487037d4:	d3504880 	ubfx	x0, x4, #16, #3
    487037d8:	d3545886 	ubfx	x6, x4, #20, #3
    487037dc:	f26d009f 	tst	x4, #0x80000
    487037e0:	5a820443 	cneg	w3, w2, ne  // ne = any
    487037e4:	f269009f 	tst	x4, #0x800000
    487037e8:	1b037c00 	mul	w0, w0, w3
    487037ec:	5a820443 	cneg	w3, w2, ne  // ne = any
    487037f0:	f265009f 	tst	x4, #0x8000000
    487037f4:	5a82044c 	cneg	w12, w2, ne  // ne = any
    487037f8:	7100009f 	cmp	w4, #0x0
    487037fc:	1b037cc6 	mul	w6, w6, w3
    48703800:	d3586883 	ubfx	x3, x4, #24, #3
    48703804:	5a82a445 	cneg	w5, w2, lt  // lt = tstop
    48703808:	52800182 	mov	w2, #0xc                   	// #12
    4870380c:	1b0c7c63 	mul	w3, w3, w12
    48703810:	1b0b7ccd 	mul	w13, w6, w11
    48703814:	1b0b7c6c 	mul	w12, w3, w11
    48703818:	52867203 	mov	w3, #0x3390                	// #13200
    4870381c:	1b028c21 	msub	w1, w1, w2, w3
    48703820:	52807083 	mov	w3, #0x384                 	// #900
    48703824:	52804b62 	mov	w2, #0x25b                 	// #603
    48703828:	1acb0821 	udiv	w1, w1, w11
    4870382c:	1b0b0400 	madd	w0, w0, w11, w1
    48703830:	52806901 	mov	w1, #0x348                 	// #840
    48703834:	710d201f 	cmp	w0, #0x348
    48703838:	1a812000 	csel	w0, w0, w1, cs  // cs = hs, nlast
    4870383c:	528075e1 	mov	w1, #0x3af                 	// #943
    48703840:	710e101f 	cmp	w0, #0x384
    48703844:	1a839000 	csel	w0, w0, w3, ls  // ls = plast
    48703848:	97fffe62 	bl	487031d0 <calc_volt_regval>
    4870384c:	d2800c01 	mov	x1, #0x60                  	// #96
    48703850:	52800166 	mov	w6, #0xb                   	// #11
    48703854:	f2a24061 	movk	x1, #0x1203, lsl #16
    48703858:	52804b62 	mov	w2, #0x25b                 	// #603
    4870385c:	b9000020 	str	w0, [x1]
    48703860:	5285f640 	mov	w0, #0x2fb2                	// #12210
    48703864:	528074e1 	mov	w1, #0x3a7                 	// #935
    48703868:	1b068146 	msub	w6, w10, w6, w0
    4870386c:	52806180 	mov	w0, #0x30c                 	// #780
    48703870:	1acb08c6 	udiv	w6, w6, w11
    48703874:	0b0d00c6 	add	w6, w6, w13
    48703878:	710c30df 	cmp	w6, #0x30c
    4870387c:	1a8020c6 	csel	w6, w6, w0, cs  // cs = hs, nlast
    48703880:	52806e00 	mov	w0, #0x370                 	// #880
    48703884:	710dc0df 	cmp	w6, #0x370
    48703888:	1a8090c0 	csel	w0, w6, w0, ls  // ls = plast
    4870388c:	97fffe51 	bl	487031d0 <calc_volt_regval>
    48703890:	d35c7884 	ubfx	x4, x4, #28, #3
    48703894:	d2800d01 	mov	x1, #0x68                  	// #104
    48703898:	52804aa2 	mov	w2, #0x255                 	// #597
    4870389c:	f2a24061 	movk	x1, #0x1203, lsl #16
    487038a0:	1b057c84 	mul	w4, w4, w5
    487038a4:	528001c5 	mov	w5, #0xe                   	// #14
    487038a8:	b9000020 	str	w0, [x1]
    487038ac:	52876980 	mov	w0, #0x3b4c                	// #15180
    487038b0:	528086c1 	mov	w1, #0x436                 	// #1078
    487038b4:	1b0580e0 	msub	w0, w7, w5, w0
    487038b8:	1acb0800 	udiv	w0, w0, w11
    487038bc:	1b0b0084 	madd	w4, w4, w11, w0
    487038c0:	52806f00 	mov	w0, #0x378                 	// #888
    487038c4:	710de09f 	cmp	w4, #0x378
    487038c8:	1a802084 	csel	w4, w4, w0, cs  // cs = hs, nlast
    487038cc:	52807b80 	mov	w0, #0x3dc                 	// #988
    487038d0:	710f709f 	cmp	w4, #0x3dc
    487038d4:	1a809080 	csel	w0, w4, w0, ls  // ls = plast
    487038d8:	97fffe3e 	bl	487031d0 <calc_volt_regval>
    487038dc:	5286ec83 	mov	w3, #0x3764                	// #14180
    487038e0:	d2800d81 	mov	x1, #0x6c                  	// #108
    487038e4:	f2a24061 	movk	x1, #0x1203, lsl #16
    487038e8:	52804b62 	mov	w2, #0x25b                 	// #603
    487038ec:	1b058d03 	msub	w3, w8, w5, w3
    487038f0:	b9000020 	str	w0, [x1]
    487038f4:	52806b40 	mov	w0, #0x35a                 	// #858
    487038f8:	1acb0863 	udiv	w3, w3, w11
    487038fc:	528075e1 	mov	w1, #0x3af                 	// #943
    48703900:	0b0c0063 	add	w3, w3, w12
    48703904:	710d687f 	cmp	w3, #0x35a
    48703908:	1a802063 	csel	w3, w3, w0, cs  // cs = hs, nlast
    4870390c:	528076c0 	mov	w0, #0x3b6                 	// #950
    48703910:	710ed87f 	cmp	w3, #0x3b6
    48703914:	1a809060 	csel	w0, w3, w0, ls  // ls = plast
    48703918:	97fffe2e 	bl	487031d0 <calc_volt_regval>
    4870391c:	d2800c81 	mov	x1, #0x64                  	// #100
    48703920:	f2a24061 	movk	x1, #0x1203, lsl #16
    48703924:	b9000020 	str	w0, [x1]
    48703928:	52802300 	mov	w0, #0x118                 	// #280
    4870392c:	97fffe12 	bl	48703174 <delay>
    48703930:	d2806080 	mov	x0, #0x304                 	// #772
    48703934:	3204c7e1 	mov	w1, #0x30303030            	// #808464432
    48703938:	f2a24040 	movk	x0, #0x1202, lsl #16
    4870393c:	b900000a 	str	w10, [x0]
    48703940:	b9000408 	str	w8, [x0, #4]
    48703944:	b9000807 	str	w7, [x0, #8]
    48703948:	d2802a00 	mov	x0, #0x150                 	// #336
    4870394c:	f2a24040 	movk	x0, #0x1202, lsl #16
    48703950:	b9000001 	str	w1, [x0]
    48703954:	d2800000 	mov	x0, #0x0                   	// #0
    48703958:	94000c06 	bl	48706970 <ddr_hw_training_if>
    4870395c:	94000c0e 	bl	48706994 <ddr_cmd_site_save>
    48703960:	d2800000 	mov	x0, #0x0                   	// #0
    48703964:	94000c02 	bl	4870696c <ddr_sw_training_if>
    48703968:	94000c15 	bl	487069bc <ddr_cmd_site_restore>
    4870396c:	d2980580 	mov	x0, #0xc02c                	// #49196
    48703970:	d2900501 	mov	x1, #0x8028                	// #32808
    48703974:	f2a240c0 	movk	x0, #0x1206, lsl #16
    48703978:	f2a240c1 	movk	x1, #0x1206, lsl #16
    4870397c:	b9400000 	ldr	w0, [x0]
    48703980:	12000c00 	and	w0, w0, #0xf
    48703984:	7100181f 	cmp	w0, #0x6
    48703988:	52808020 	mov	w0, #0x401                 	// #1025
    4870398c:	b9000020 	str	w0, [x1]
    48703990:	54000041 	b.ne	48703998 <start_ddr_training+0x4f0>  // b.any
    48703994:	b9100020 	str	w0, [x1, #4096]
    48703998:	d29c0580 	mov	x0, #0xe02c                	// #57388
    4870399c:	f2a240c0 	movk	x0, #0x1206, lsl #16
    487039a0:	b9400000 	ldr	w0, [x0]
    487039a4:	12000c00 	and	w0, w0, #0xf
    487039a8:	7100181f 	cmp	w0, #0x6
    487039ac:	52808020 	mov	w0, #0x401                 	// #1025
    487039b0:	54000241 	b.ne	487039f8 <start_ddr_training+0x550>  // b.any
    487039b4:	d2940501 	mov	x1, #0xa028                	// #41000
    487039b8:	f2a240c1 	movk	x1, #0x1206, lsl #16
    487039bc:	b9000020 	str	w0, [x1]
    487039c0:	b9100020 	str	w0, [x1, #4096]
    487039c4:	97fffe3f 	bl	487032c0 <ddr_scramb>
    487039c8:	a8c57bfd 	ldp	x29, x30, [sp], #80
    487039cc:	d65f03c0 	ret
    487039d0:	7101185f 	cmp	w2, #0x46
    487039d4:	540000cd 	b.le	487039ec <start_ddr_training+0x544>
    487039d8:	11000d4a 	add	w10, w10, #0x3
    487039dc:	11000808 	add	w8, w0, #0x2
    487039e0:	110014e7 	add	w7, w7, #0x5
    487039e4:	11000961 	add	w1, w11, #0x2
    487039e8:	17ffff65 	b	4870377c <start_ddr_training+0x2d4>
    487039ec:	2a0003e8 	mov	w8, w0
    487039f0:	2a0b03e1 	mov	w1, w11
    487039f4:	17ffff62 	b	4870377c <start_ddr_training+0x2d4>
    487039f8:	d2920501 	mov	x1, #0x9028                	// #36904
    487039fc:	f2a240c1 	movk	x1, #0x1206, lsl #16
    48703a00:	b9000020 	str	w0, [x1]
    48703a04:	17fffff0 	b	487039c4 <start_ddr_training+0x51c>

0000000048703a08 <ddr_adjust_get_average>:
    48703a08:	b9409c01 	ldr	w1, [x0, #156]
    48703a0c:	b9409003 	ldr	w3, [x0, #144]
    48703a10:	b9407c02 	ldr	w2, [x0, #124]
    48703a14:	b9408c00 	ldr	w0, [x0, #140]
    48703a18:	7100081f 	cmp	w0, #0x2
    48703a1c:	53165420 	lsl	w0, w1, #10
    48703a20:	53196061 	lsl	w1, w3, #7
    48703a24:	540000e1 	b.ne	48703a40 <ddr_adjust_get_average+0x38>  // b.any
    48703a28:	0b010000 	add	w0, w0, w1
    48703a2c:	2a0203e2 	mov	w2, w2
    48703a30:	11086000 	add	w0, w0, #0x218
    48703a34:	b8626800 	ldr	w0, [x0, x2]
    48703a38:	d3411c00 	ubfx	x0, x0, #1, #7
    48703a3c:	d65f03c0 	ret
    48703a40:	0b010000 	add	w0, w0, w1
    48703a44:	2a0203e2 	mov	w2, w2
    48703a48:	11087001 	add	w1, w0, #0x21c
    48703a4c:	11088000 	add	w0, w0, #0x220
    48703a50:	b8626821 	ldr	w1, [x1, x2]
    48703a54:	b8626802 	ldr	w2, [x0, x2]
    48703a58:	d3493c23 	ubfx	x3, x1, #9, #7
    48703a5c:	d3411c20 	ubfx	x0, x1, #1, #7
    48703a60:	0b030000 	add	w0, w0, w3
    48703a64:	53197c43 	lsr	w3, w2, #25
    48703a68:	0b416463 	add	w3, w3, w1, lsr #25
    48703a6c:	d3515c21 	ubfx	x1, x1, #17, #7
    48703a70:	0b030000 	add	w0, w0, w3
    48703a74:	d3411c43 	ubfx	x3, x2, #1, #7
    48703a78:	0b030021 	add	w1, w1, w3
    48703a7c:	0b010000 	add	w0, w0, w1
    48703a80:	d3493c41 	ubfx	x1, x2, #9, #7
    48703a84:	d3515c42 	ubfx	x2, x2, #17, #7
    48703a88:	0b020022 	add	w2, w1, w2
    48703a8c:	0b020000 	add	w0, w0, w2
    48703a90:	53037c00 	lsr	w0, w0, #3
    48703a94:	d65f03c0 	ret

0000000048703a98 <ddr_dcc_get_min_win>:
    48703a98:	937a7c21 	sbfiz	x1, x1, #6, #32
    48703a9c:	8b010000 	add	x0, x0, x1
    48703aa0:	29540003 	ldp	w3, w0, [x0, #160]
    48703aa4:	d3412401 	ubfx	x1, x0, #1, #9
    48703aa8:	d3412462 	ubfx	x2, x3, #1, #9
    48703aac:	d3516400 	ubfx	x0, x0, #17, #9
    48703ab0:	d3516463 	ubfx	x3, x3, #17, #9
    48703ab4:	4b030042 	sub	w2, w2, w3
    48703ab8:	4b000020 	sub	w0, w1, w0
    48703abc:	6b00005f 	cmp	w2, w0
    48703ac0:	1a809040 	csel	w0, w2, w0, ls  // ls = plast
    48703ac4:	d65f03c0 	ret

0000000048703ac8 <ddrtr_memcpy>:
    48703ac8:	2a0203e2 	mov	w2, w2
    48703acc:	d2800003 	mov	x3, #0x0                   	// #0
    48703ad0:	eb02007f 	cmp	x3, x2
    48703ad4:	54000041 	b.ne	48703adc <ddrtr_memcpy+0x14>  // b.any
    48703ad8:	d65f03c0 	ret
    48703adc:	38636824 	ldrb	w4, [x1, x3]
    48703ae0:	38236804 	strb	w4, [x0, x3]
    48703ae4:	91000463 	add	x3, x3, #0x1
    48703ae8:	17fffffa 	b	48703ad0 <ddrtr_memcpy+0x8>

0000000048703aec <ddrtr_memset>:
    48703aec:	2a0203e2 	mov	w2, w2
    48703af0:	d2800003 	mov	x3, #0x0                   	// #0
    48703af4:	eb02007f 	cmp	x3, x2
    48703af8:	54000041 	b.ne	48703b00 <ddrtr_memset+0x14>  // b.any
    48703afc:	d65f03c0 	ret
    48703b00:	38236801 	strb	w1, [x0, x3]
    48703b04:	91000463 	add	x3, x3, #0x1
    48703b08:	17fffffb 	b	48703af4 <ddrtr_memset+0x8>

0000000048703b0c <ddr_training_by_dmc>:
    48703b0c:	f9405801 	ldr	x1, [x0, #176]
    48703b10:	b5000041 	cbnz	x1, 48703b18 <ddr_training_by_dmc+0xc>
    48703b14:	14000b2f 	b	487067d0 <ddr_training_boot_func>
    48703b18:	52800000 	mov	w0, #0x0                   	// #0
    48703b1c:	d65f03c0 	ret

0000000048703b20 <ddr_training_by_rank>:
    48703b20:	a9bc7bfd 	stp	x29, x30, [sp, #-64]!
    48703b24:	910003fd 	mov	x29, sp
    48703b28:	a90153f3 	stp	x19, x20, [sp, #16]
    48703b2c:	aa0003f3 	mov	x19, x0
    48703b30:	a9025bf5 	stp	x21, x22, [sp, #32]
    48703b34:	52800014 	mov	w20, #0x0                   	// #0
    48703b38:	52800015 	mov	w21, #0x0                   	// #0
    48703b3c:	d2800796 	mov	x22, #0x3c                  	// #60
    48703b40:	b9409c02 	ldr	w2, [x0, #156]
    48703b44:	f9001bf7 	str	x23, [sp, #48]
    48703b48:	d2800197 	mov	x23, #0xc                   	// #12
    48703b4c:	b9407c01 	ldr	w1, [x0, #124]
    48703b50:	b9404820 	ldr	w0, [x1, #72]
    48703b54:	121c6c00 	and	w0, w0, #0xfffffff0
    48703b58:	2a020000 	orr	w0, w0, w2
    48703b5c:	b9004820 	str	w0, [x1, #72]
    48703b60:	b9407e62 	ldr	w2, [x19, #124]
    48703b64:	b9409e60 	ldr	w0, [x19, #156]
    48703b68:	b940c841 	ldr	w1, [x2, #200]
    48703b6c:	12007821 	and	w1, w1, #0x7fffffff
    48703b70:	2a007c20 	orr	w0, w1, w0, lsl #31
    48703b74:	b900c840 	str	w0, [x2, #200]
    48703b78:	b9409a60 	ldr	w0, [x19, #152]
    48703b7c:	9b167c01 	mul	x1, x0, x22
    48703b80:	8b010260 	add	x0, x19, x1
    48703b84:	b9400800 	ldr	w0, [x0, #8]
    48703b88:	6b14001f 	cmp	w0, w20
    48703b8c:	540000e8 	b.hi	48703ba8 <ddr_training_by_rank+0x88>  // b.pmore
    48703b90:	2a1503e0 	mov	w0, w21
    48703b94:	f9401bf7 	ldr	x23, [sp, #48]
    48703b98:	a94153f3 	ldp	x19, x20, [sp, #16]
    48703b9c:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48703ba0:	a8c47bfd 	ldp	x29, x30, [sp], #64
    48703ba4:	d65f03c0 	ret
    48703ba8:	93407e80 	sxtw	x0, w20
    48703bac:	b900a274 	str	w20, [x19, #160]
    48703bb0:	11000694 	add	w20, w20, #0x1
    48703bb4:	9b170400 	madd	x0, x0, x23, x1
    48703bb8:	8b000260 	add	x0, x19, x0
    48703bbc:	b9401401 	ldr	w1, [x0, #20]
    48703bc0:	b9008261 	str	w1, [x19, #128]
    48703bc4:	b9401c00 	ldr	w0, [x0, #28]
    48703bc8:	b9008a60 	str	w0, [x19, #136]
    48703bcc:	aa1303e0 	mov	x0, x19
    48703bd0:	97ffffcf 	bl	48703b0c <ddr_training_by_dmc>
    48703bd4:	0b0002b5 	add	w21, w21, w0
    48703bd8:	17ffffe8 	b	48703b78 <ddr_training_by_rank+0x58>

0000000048703bdc <ddr_training_by_phy>:
    48703bdc:	a9bc7bfd 	stp	x29, x30, [sp, #-64]!
    48703be0:	d2800781 	mov	x1, #0x3c                  	// #60
    48703be4:	910003fd 	mov	x29, sp
    48703be8:	a90153f3 	stp	x19, x20, [sp, #16]
    48703bec:	aa0003f3 	mov	x19, x0
    48703bf0:	a9025bf5 	stp	x21, x22, [sp, #32]
    48703bf4:	aa0103f5 	mov	x21, x1
    48703bf8:	a90363f7 	stp	x23, x24, [sp, #48]
    48703bfc:	52800037 	mov	w23, #0x1                   	// #1
    48703c00:	52800014 	mov	w20, #0x0                   	// #0
    48703c04:	52800016 	mov	w22, #0x0                   	// #0
    48703c08:	b9409800 	ldr	w0, [x0, #152]
    48703c0c:	1ac022f7 	lsl	w23, w23, w0
    48703c10:	2a0003e0 	mov	w0, w0
    48703c14:	9b014c00 	madd	x0, x0, x1, x19
    48703c18:	b9400c18 	ldr	w24, [x0, #12]
    48703c1c:	6b18029f 	cmp	w20, w24
    48703c20:	540000e1 	b.ne	48703c3c <ddr_training_by_phy+0x60>  // b.any
    48703c24:	2a1603e0 	mov	w0, w22
    48703c28:	a94153f3 	ldp	x19, x20, [sp, #16]
    48703c2c:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48703c30:	a94363f7 	ldp	x23, x24, [sp, #48]
    48703c34:	a8c47bfd 	ldp	x29, x30, [sp], #64
    48703c38:	d65f03c0 	ret
    48703c3c:	b9409a61 	ldr	w1, [x19, #152]
    48703c40:	93407e80 	sxtw	x0, w20
    48703c44:	91001000 	add	x0, x0, #0x4
    48703c48:	b9009e74 	str	w20, [x19, #156]
    48703c4c:	9b157c21 	mul	x1, x1, x21
    48703c50:	8b000c20 	add	x0, x1, x0, lsl #3
    48703c54:	8b000260 	add	x0, x19, x0
    48703c58:	b9400c00 	ldr	w0, [x0, #12]
    48703c5c:	6a0002ff 	tst	w23, w0
    48703c60:	b9008660 	str	w0, [x19, #132]
    48703c64:	54000060 	b.eq	48703c70 <ddr_training_by_phy+0x94>  // b.none
    48703c68:	11000694 	add	w20, w20, #0x1
    48703c6c:	17ffffec 	b	48703c1c <ddr_training_by_phy+0x40>
    48703c70:	aa1303e0 	mov	x0, x19
    48703c74:	97ffffab 	bl	48703b20 <ddr_training_by_rank>
    48703c78:	0b0002d6 	add	w22, w22, w0
    48703c7c:	17fffffb 	b	48703c68 <ddr_training_by_phy+0x8c>

0000000048703c80 <ddr_training_all>:
    48703c80:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
    48703c84:	910003fd 	mov	x29, sp
    48703c88:	a90153f3 	stp	x19, x20, [sp, #16]
    48703c8c:	aa0003f4 	mov	x20, x0
    48703c90:	a9025bf5 	stp	x21, x22, [sp, #32]
    48703c94:	d2800013 	mov	x19, #0x0                   	// #0
    48703c98:	52800015 	mov	w21, #0x0                   	// #0
    48703c9c:	d2800796 	mov	x22, #0x3c                  	// #60
    48703ca0:	b9407a80 	ldr	w0, [x20, #120]
    48703ca4:	6b13001f 	cmp	w0, w19
    48703ca8:	540000c8 	b.hi	48703cc0 <ddr_training_all+0x40>  // b.pmore
    48703cac:	2a1503e0 	mov	w0, w21
    48703cb0:	a94153f3 	ldp	x19, x20, [sp, #16]
    48703cb4:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48703cb8:	a8c37bfd 	ldp	x29, x30, [sp], #48
    48703cbc:	d65f03c0 	ret
    48703cc0:	b9009a93 	str	w19, [x20, #152]
    48703cc4:	9b167e60 	mul	x0, x19, x22
    48703cc8:	91000673 	add	x19, x19, #0x1
    48703ccc:	b8606a80 	ldr	w0, [x20, x0]
    48703cd0:	b9007e80 	str	w0, [x20, #124]
    48703cd4:	aa1403e0 	mov	x0, x20
    48703cd8:	97ffffc1 	bl	48703bdc <ddr_training_by_phy>
    48703cdc:	0b0002b5 	add	w21, w21, w0
    48703ce0:	17fffff0 	b	48703ca0 <ddr_training_all+0x20>

0000000048703ce4 <ddr_training_cfg_set_rank>:
    48703ce4:	52800022 	mov	w2, #0x1                   	// #1
    48703ce8:	d2801401 	mov	x1, #0xa0                  	// #160
    48703cec:	f2a24041 	movk	x1, #0x1202, lsl #16
    48703cf0:	b9000c02 	str	w2, [x0, #12]
    48703cf4:	b9400023 	ldr	w3, [x1]
    48703cf8:	b9002c03 	str	w3, [x0, #44]
    48703cfc:	d2801203 	mov	x3, #0x90                  	// #144
    48703d00:	f2a24043 	movk	x3, #0x1202, lsl #16
    48703d04:	b9400063 	ldr	w3, [x3]
    48703d08:	b9003003 	str	w3, [x0, #48]
    48703d0c:	d2801483 	mov	x3, #0xa4                  	// #164
    48703d10:	f2a24043 	movk	x3, #0x1202, lsl #16
    48703d14:	b9400063 	ldr	w3, [x3]
    48703d18:	b9003403 	str	w3, [x0, #52]
    48703d1c:	d2801283 	mov	x3, #0x94                  	// #148
    48703d20:	f2a24043 	movk	x3, #0x1202, lsl #16
    48703d24:	b9400064 	ldr	w4, [x3]
    48703d28:	b9003804 	str	w4, [x0, #56]
    48703d2c:	b9400063 	ldr	w3, [x3]
    48703d30:	34000063 	cbz	w3, 48703d3c <ddr_training_cfg_set_rank+0x58>
    48703d34:	52800043 	mov	w3, #0x2                   	// #2
    48703d38:	b9000c03 	str	w3, [x0, #12]
    48703d3c:	b9004802 	str	w2, [x0, #72]
    48703d40:	b9400021 	ldr	w1, [x1]
    48703d44:	b9006801 	str	w1, [x0, #104]
    48703d48:	d2801301 	mov	x1, #0x98                  	// #152
    48703d4c:	f2a24041 	movk	x1, #0x1202, lsl #16
    48703d50:	b9400021 	ldr	w1, [x1]
    48703d54:	b9006c01 	str	w1, [x0, #108]
    48703d58:	d2801481 	mov	x1, #0xa4                  	// #164
    48703d5c:	f2a24041 	movk	x1, #0x1202, lsl #16
    48703d60:	b9400021 	ldr	w1, [x1]
    48703d64:	b9007001 	str	w1, [x0, #112]
    48703d68:	d2801381 	mov	x1, #0x9c                  	// #156
    48703d6c:	f2a24041 	movk	x1, #0x1202, lsl #16
    48703d70:	b9400022 	ldr	w2, [x1]
    48703d74:	b9007402 	str	w2, [x0, #116]
    48703d78:	b9400021 	ldr	w1, [x1]
    48703d7c:	34000061 	cbz	w1, 48703d88 <ddr_training_cfg_set_rank+0xa4>
    48703d80:	52800041 	mov	w1, #0x2                   	// #2
    48703d84:	b9004801 	str	w1, [x0, #72]
    48703d88:	d65f03c0 	ret

0000000048703d8c <ddr_training_cfg_set_phy>:
    48703d8c:	52800041 	mov	w1, #0x2                   	// #2
    48703d90:	b9007801 	str	w1, [x0, #120]
    48703d94:	52980001 	mov	w1, #0xc000                	// #49152
    48703d98:	72a240c1 	movk	w1, #0x1206, lsl #16
    48703d9c:	b9000001 	str	w1, [x0]
    48703da0:	d2980581 	mov	x1, #0xc02c                	// #49196
    48703da4:	f2a240c1 	movk	x1, #0x1206, lsl #16
    48703da8:	b9400021 	ldr	w1, [x1]
    48703dac:	12000c21 	and	w1, w1, #0xf
    48703db0:	b9000401 	str	w1, [x0, #4]
    48703db4:	529c0001 	mov	w1, #0xe000                	// #57344
    48703db8:	72a240c1 	movk	w1, #0x1206, lsl #16
    48703dbc:	b9003c01 	str	w1, [x0, #60]
    48703dc0:	d29c0581 	mov	x1, #0xe02c                	// #57388
    48703dc4:	f2a240c1 	movk	x1, #0x1206, lsl #16
    48703dc8:	b9400021 	ldr	w1, [x1]
    48703dcc:	12000c21 	and	w1, w1, #0xf
    48703dd0:	b9004001 	str	w1, [x0, #64]
    48703dd4:	d65f03c0 	ret

0000000048703dd8 <ddr_training_set_timing>:
    48703dd8:	52807d22 	mov	w2, #0x3e9                 	// #1001
    48703ddc:	71000442 	subs	w2, w2, #0x1
    48703de0:	540000e1 	b.ne	48703dfc <ddr_training_set_timing+0x24>  // b.any
    48703de4:	2a0003e0 	mov	w0, w0
    48703de8:	b9010801 	str	w1, [x0, #264]
    48703dec:	52807d20 	mov	w0, #0x3e9                 	// #1001
    48703df0:	71000400 	subs	w0, w0, #0x1
    48703df4:	54000081 	b.ne	48703e04 <ddr_training_set_timing+0x2c>  // b.any
    48703df8:	d65f03c0 	ret
    48703dfc:	d503201f 	nop
    48703e00:	17fffff7 	b	48703ddc <ddr_training_set_timing+0x4>
    48703e04:	d503201f 	nop
    48703e08:	17fffffa 	b	48703df0 <ddr_training_set_timing+0x18>

0000000048703e0c <ddr_training_save_timing>:
    48703e0c:	aa0103e6 	mov	x6, x1
    48703e10:	b9409801 	ldr	w1, [x0, #152]
    48703e14:	d2800282 	mov	x2, #0x14                  	// #20
    48703e18:	52800783 	mov	w3, #0x3c                  	// #60
    48703e1c:	d2800784 	mov	x4, #0x3c                  	// #60
    48703e20:	d2800005 	mov	x5, #0x0                   	// #0
    48703e24:	9ba30823 	umaddl	x3, w1, w3, x2
    48703e28:	2a0103e1 	mov	w1, w1
    48703e2c:	8b030003 	add	x3, x0, x3
    48703e30:	9b040024 	madd	x4, x1, x4, x0
    48703e34:	b9400880 	ldr	w0, [x4, #8]
    48703e38:	6b05001f 	cmp	w0, w5
    48703e3c:	54000048 	b.hi	48703e44 <ddr_training_save_timing+0x38>  // b.pmore
    48703e40:	d65f03c0 	ret
    48703e44:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48703e48:	910003fd 	mov	x29, sp
    48703e4c:	b9400060 	ldr	w0, [x3]
    48703e50:	b9410801 	ldr	w1, [x0, #264]
    48703e54:	b82578c1 	str	w1, [x6, x5, lsl #2]
    48703e58:	12144c21 	and	w1, w1, #0xfffff000
    48703e5c:	b840c460 	ldr	w0, [x3], #12
    48703e60:	97ffffde 	bl	48703dd8 <ddr_training_set_timing>
    48703e64:	b9400880 	ldr	w0, [x4, #8]
    48703e68:	910004a5 	add	x5, x5, #0x1
    48703e6c:	6b05001f 	cmp	w0, w5
    48703e70:	54fffee8 	b.hi	48703e4c <ddr_training_save_timing+0x40>  // b.pmore
    48703e74:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48703e78:	d65f03c0 	ret

0000000048703e7c <ddr_vref_set>:
    48703e7c:	b9408c02 	ldr	w2, [x0, #140]
    48703e80:	7100045f 	cmp	w2, #0x1
    48703e84:	54000341 	b.ne	48703eec <ddr_vref_set+0x70>  // b.any
    48703e88:	b9409c04 	ldr	w4, [x0, #156]
    48703e8c:	b9407c03 	ldr	w3, [x0, #124]
    48703e90:	b9409002 	ldr	w2, [x0, #144]
    48703e94:	350001e4 	cbnz	w4, 48703ed0 <ddr_vref_set+0x54>
    48703e98:	53196042 	lsl	w2, w2, #7
    48703e9c:	1109d042 	add	w2, w2, #0x274
    48703ea0:	b8636844 	ldr	w4, [x2, x3]
    48703ea4:	12196084 	and	w4, w4, #0xffffff80
    48703ea8:	2a010081 	orr	w1, w4, w1
    48703eac:	b8236841 	str	w1, [x2, x3]
    48703eb0:	b9409c03 	ldr	w3, [x0, #156]
    48703eb4:	b9409002 	ldr	w2, [x0, #144]
    48703eb8:	b9407c00 	ldr	w0, [x0, #124]
    48703ebc:	53165463 	lsl	w3, w3, #10
    48703ec0:	0b021c62 	add	w2, w3, w2, lsl #7
    48703ec4:	110bd042 	add	w2, w2, #0x2f4
    48703ec8:	b8206841 	str	w1, [x2, x0]
    48703ecc:	d65f03c0 	ret
    48703ed0:	53165484 	lsl	w4, w4, #10
    48703ed4:	0b021c82 	add	w2, w4, w2, lsl #7
    48703ed8:	1109d042 	add	w2, w2, #0x274
    48703edc:	b8636844 	ldr	w4, [x2, x3]
    48703ee0:	12076c84 	and	w4, w4, #0xfe1fffff
    48703ee4:	2a015481 	orr	w1, w4, w1, lsl #21
    48703ee8:	17fffff1 	b	48703eac <ddr_vref_set+0x30>
    48703eec:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
    48703ef0:	910003fd 	mov	x29, sp
    48703ef4:	a90153f3 	stp	x19, x20, [sp, #16]
    48703ef8:	aa0003f3 	mov	x19, x0
    48703efc:	f90013f5 	str	x21, [sp, #32]
    48703f00:	2a0103f4 	mov	w20, w1
    48703f04:	b9408000 	ldr	w0, [x0, #128]
    48703f08:	b9410815 	ldr	w21, [x0, #264]
    48703f0c:	12144ea1 	and	w1, w21, #0xfffff000
    48703f10:	97ffffb2 	bl	48703dd8 <ddr_training_set_timing>
    48703f14:	b9407e61 	ldr	w1, [x19, #124]
    48703f18:	b940c020 	ldr	w0, [x1, #192]
    48703f1c:	32010000 	orr	w0, w0, #0x80000000
    48703f20:	b900c020 	str	w0, [x1, #192]
    48703f24:	b9409260 	ldr	w0, [x19, #144]
    48703f28:	b9407e66 	ldr	w6, [x19, #124]
    48703f2c:	53196000 	lsl	w0, w0, #7
    48703f30:	1109c000 	add	w0, w0, #0x270
    48703f34:	b940c4c2 	ldr	w2, [x6, #196]
    48703f38:	b8666801 	ldr	w1, [x0, x6]
    48703f3c:	32010040 	orr	w0, w2, #0x80000000
    48703f40:	b900c4c0 	str	w0, [x6, #196]
    48703f44:	121a6421 	and	w1, w1, #0xffffffc0
    48703f48:	b9409260 	ldr	w0, [x19, #144]
    48703f4c:	2a140021 	orr	w1, w1, w20
    48703f50:	b9407e66 	ldr	w6, [x19, #124]
    48703f54:	53196000 	lsl	w0, w0, #7
    48703f58:	1109c000 	add	w0, w0, #0x270
    48703f5c:	b8266801 	str	w1, [x0, x6]
    48703f60:	52800021 	mov	w1, #0x1                   	// #1
    48703f64:	72a00081 	movk	w1, #0x4, lsl #16
    48703f68:	b9407e60 	ldr	w0, [x19, #124]
    48703f6c:	b9000401 	str	w1, [x0, #4]
    48703f70:	b9407e60 	ldr	w0, [x19, #124]
    48703f74:	91001006 	add	x6, x0, #0x4
    48703f78:	b94000c1 	ldr	w1, [x6]
    48703f7c:	3707ffe1 	tbnz	w1, #0, 48703f78 <ddr_vref_set+0xfc>
    48703f80:	12007842 	and	w2, w2, #0x7fffffff
    48703f84:	b900c402 	str	w2, [x0, #196]
    48703f88:	b9409260 	ldr	w0, [x19, #144]
    48703f8c:	b9407e66 	ldr	w6, [x19, #124]
    48703f90:	53196000 	lsl	w0, w0, #7
    48703f94:	1109c000 	add	w0, w0, #0x270
    48703f98:	b940c4c2 	ldr	w2, [x6, #196]
    48703f9c:	b8666801 	ldr	w1, [x0, x6]
    48703fa0:	32010040 	orr	w0, w2, #0x80000000
    48703fa4:	b900c4c0 	str	w0, [x6, #196]
    48703fa8:	121a6421 	and	w1, w1, #0xffffffc0
    48703fac:	b9409260 	ldr	w0, [x19, #144]
    48703fb0:	2a140021 	orr	w1, w1, w20
    48703fb4:	b9407e66 	ldr	w6, [x19, #124]
    48703fb8:	53196000 	lsl	w0, w0, #7
    48703fbc:	1109c000 	add	w0, w0, #0x270
    48703fc0:	b8266801 	str	w1, [x0, x6]
    48703fc4:	52800021 	mov	w1, #0x1                   	// #1
    48703fc8:	72a00081 	movk	w1, #0x4, lsl #16
    48703fcc:	b9407e60 	ldr	w0, [x19, #124]
    48703fd0:	b9000401 	str	w1, [x0, #4]
    48703fd4:	b9407e60 	ldr	w0, [x19, #124]
    48703fd8:	91001006 	add	x6, x0, #0x4
    48703fdc:	b94000c1 	ldr	w1, [x6]
    48703fe0:	3707ffe1 	tbnz	w1, #0, 48703fdc <ddr_vref_set+0x160>
    48703fe4:	12007842 	and	w2, w2, #0x7fffffff
    48703fe8:	b900c402 	str	w2, [x0, #196]
    48703fec:	b9407e61 	ldr	w1, [x19, #124]
    48703ff0:	b940c020 	ldr	w0, [x1, #192]
    48703ff4:	12007800 	and	w0, w0, #0x7fffffff
    48703ff8:	b900c020 	str	w0, [x1, #192]
    48703ffc:	b9409260 	ldr	w0, [x19, #144]
    48704000:	b9407e66 	ldr	w6, [x19, #124]
    48704004:	53196000 	lsl	w0, w0, #7
    48704008:	1109c000 	add	w0, w0, #0x270
    4870400c:	b940c4c2 	ldr	w2, [x6, #196]
    48704010:	b8666801 	ldr	w1, [x0, x6]
    48704014:	32010040 	orr	w0, w2, #0x80000000
    48704018:	b900c4c0 	str	w0, [x6, #196]
    4870401c:	121a6421 	and	w1, w1, #0xffffffc0
    48704020:	b9409260 	ldr	w0, [x19, #144]
    48704024:	2a140024 	orr	w4, w1, w20
    48704028:	b9407e66 	ldr	w6, [x19, #124]
    4870402c:	52800021 	mov	w1, #0x1                   	// #1
    48704030:	72a00081 	movk	w1, #0x4, lsl #16
    48704034:	53196000 	lsl	w0, w0, #7
    48704038:	1109c000 	add	w0, w0, #0x270
    4870403c:	b8266804 	str	w4, [x0, x6]
    48704040:	b9407e60 	ldr	w0, [x19, #124]
    48704044:	b9000401 	str	w1, [x0, #4]
    48704048:	b9407e60 	ldr	w0, [x19, #124]
    4870404c:	91001004 	add	x4, x0, #0x4
    48704050:	b9400081 	ldr	w1, [x4]
    48704054:	3707ffe1 	tbnz	w1, #0, 48704050 <ddr_vref_set+0x1d4>
    48704058:	12007842 	and	w2, w2, #0x7fffffff
    4870405c:	2a1503e1 	mov	w1, w21
    48704060:	b900c402 	str	w2, [x0, #196]
    48704064:	b9408260 	ldr	w0, [x19, #128]
    48704068:	f94013f5 	ldr	x21, [sp, #32]
    4870406c:	a94153f3 	ldp	x19, x20, [sp, #16]
    48704070:	a8c37bfd 	ldp	x29, x30, [sp], #48
    48704074:	17ffff59 	b	48703dd8 <ddr_training_set_timing>

0000000048704078 <ddr_training_stat>:
    48704078:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
    4870407c:	910003fd 	mov	x29, sp
    48704080:	a90153f3 	stp	x19, x20, [sp, #16]
    48704084:	2a0203f4 	mov	w20, w2
    48704088:	a9025bf5 	stp	x21, x22, [sp, #32]
    4870408c:	2a0303f3 	mov	w19, w3
    48704090:	2a0003f6 	mov	w22, w0
    48704094:	2a0103f5 	mov	w21, w1
    48704098:	94000a3b 	bl	48706984 <ddr_training_error>
    4870409c:	d2806000 	mov	x0, #0x300                 	// #768
    487040a0:	f2a24040 	movk	x0, #0x1202, lsl #16
    487040a4:	b9400000 	ldr	w0, [x0]
    487040a8:	35000260 	cbnz	w0, 487040f4 <ddr_training_stat+0x7c>
    487040ac:	2a1603e0 	mov	w0, w22
    487040b0:	34000115 	cbz	w21, 487040d0 <ddr_training_stat+0x58>
    487040b4:	52980001 	mov	w1, #0xc000                	// #49152
    487040b8:	52840002 	mov	w2, #0x2000                	// #8192
    487040bc:	72a240c1 	movk	w1, #0x1206, lsl #16
    487040c0:	6b0102bf 	cmp	w21, w1
    487040c4:	52820001 	mov	w1, #0x1000                	// #4096
    487040c8:	1a820021 	csel	w1, w1, w2, eq  // eq = none
    487040cc:	2a0102c0 	orr	w0, w22, w1
    487040d0:	2a146001 	orr	w1, w0, w20, lsl #24
    487040d4:	3100069f 	cmn	w20, #0x1
    487040d8:	1a801020 	csel	w0, w1, w0, ne  // ne = any
    487040dc:	3100067f 	cmn	w19, #0x1
    487040e0:	2a135001 	orr	w1, w0, w19, lsl #20
    487040e4:	1a801020 	csel	w0, w1, w0, ne  // ne = any
    487040e8:	d2806001 	mov	x1, #0x300                 	// #768
    487040ec:	f2a24041 	movk	x1, #0x1202, lsl #16
    487040f0:	b9000020 	str	w0, [x1]
    487040f4:	a94153f3 	ldp	x19, x20, [sp, #16]
    487040f8:	a9425bf5 	ldp	x21, x22, [sp, #32]
    487040fc:	a8c37bfd 	ldp	x29, x30, [sp], #48
    48704100:	d65f03c0 	ret

0000000048704104 <ddr_training_ctrl_easr>:
    48704104:	a9bc7bfd 	stp	x29, x30, [sp, #-64]!
    48704108:	d2800282 	mov	x2, #0x14                  	// #20
    4870410c:	910003fd 	mov	x29, sp
    48704110:	a90153f3 	stp	x19, x20, [sp, #16]
    48704114:	52800793 	mov	w19, #0x3c                  	// #60
    48704118:	a9025bf5 	stp	x21, x22, [sp, #32]
    4870411c:	2a0103f5 	mov	w21, w1
    48704120:	f9001bf7 	str	x23, [sp, #48]
    48704124:	d2800794 	mov	x20, #0x3c                  	// #60
    48704128:	52800016 	mov	w22, #0x0                   	// #0
    4870412c:	52800017 	mov	w23, #0x0                   	// #0
    48704130:	b9409801 	ldr	w1, [x0, #152]
    48704134:	9bb30833 	umaddl	x19, w1, w19, x2
    48704138:	2a0103e1 	mov	w1, w1
    4870413c:	8b130013 	add	x19, x0, x19
    48704140:	9b140034 	madd	x20, x1, x20, x0
    48704144:	b9400a80 	ldr	w0, [x20, #8]
    48704148:	6b17001f 	cmp	w0, w23
    4870414c:	540000e8 	b.hi	48704168 <ddr_training_ctrl_easr+0x64>  // b.pmore
    48704150:	2a1603e0 	mov	w0, w22
    48704154:	f9401bf7 	ldr	x23, [sp, #48]
    48704158:	a94153f3 	ldp	x19, x20, [sp, #16]
    4870415c:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48704160:	a8c47bfd 	ldp	x29, x30, [sp], #64
    48704164:	d65f03c0 	ret
    48704168:	71000abf 	cmp	w21, #0x2
    4870416c:	b9400261 	ldr	w1, [x19]
    48704170:	54000221 	b.ne	487041b4 <ddr_training_ctrl_easr+0xb0>  // b.any
    48704174:	2a0103e1 	mov	w1, w1
    48704178:	52800000 	mov	w0, #0x0                   	// #0
    4870417c:	b9000035 	str	w21, [x1]
    48704180:	910a5021 	add	x1, x1, #0x294
    48704184:	b9400022 	ldr	w2, [x1]
    48704188:	11000400 	add	w0, w0, #0x1
    4870418c:	360002c2 	tbz	w2, #0, 487041e4 <ddr_training_ctrl_easr+0xe0>
    48704190:	3100041f 	cmn	w0, #0x1
    48704194:	54ffff81 	b.ne	48704184 <ddr_training_ctrl_easr+0x80>  // b.any
    48704198:	12800003 	mov	w3, #0xffffffff            	// #-1
    4870419c:	52800200 	mov	w0, #0x10                  	// #16
    487041a0:	2a0303e2 	mov	w2, w3
    487041a4:	2a0303e1 	mov	w1, w3
    487041a8:	97ffffb4 	bl	48704078 <ddr_training_stat>
    487041ac:	12800000 	mov	w0, #0xffffffff            	// #-1
    487041b0:	1400000e 	b	487041e8 <ddr_training_ctrl_easr+0xe4>
    487041b4:	710006bf 	cmp	w21, #0x1
    487041b8:	54ffff01 	b.ne	48704198 <ddr_training_ctrl_easr+0x94>  // b.any
    487041bc:	2a0103e1 	mov	w1, w1
    487041c0:	52800000 	mov	w0, #0x0                   	// #0
    487041c4:	b9000035 	str	w21, [x1]
    487041c8:	910a5021 	add	x1, x1, #0x294
    487041cc:	b9400022 	ldr	w2, [x1]
    487041d0:	11000400 	add	w0, w0, #0x1
    487041d4:	37000082 	tbnz	w2, #0, 487041e4 <ddr_training_ctrl_easr+0xe0>
    487041d8:	3100041f 	cmn	w0, #0x1
    487041dc:	54ffff81 	b.ne	487041cc <ddr_training_ctrl_easr+0xc8>  // b.any
    487041e0:	17ffffee 	b	48704198 <ddr_training_ctrl_easr+0x94>
    487041e4:	52800000 	mov	w0, #0x0                   	// #0
    487041e8:	0b0002d6 	add	w22, w22, w0
    487041ec:	110006f7 	add	w23, w23, #0x1
    487041f0:	91003273 	add	x19, x19, #0xc
    487041f4:	17ffffd4 	b	48704144 <ddr_training_ctrl_easr+0x40>

00000000487041f8 <ddr_hw_training_process>:
    487041f8:	a9bc7bfd 	stp	x29, x30, [sp, #-64]!
    487041fc:	910003fd 	mov	x29, sp
    48704200:	a9025bf5 	stp	x21, x22, [sp, #32]
    48704204:	a90153f3 	stp	x19, x20, [sp, #16]
    48704208:	f9001bf7 	str	x23, [sp, #48]
    4870420c:	b9407c16 	ldr	w22, [x0, #124]
    48704210:	2a1603f5 	mov	w21, w22
    48704214:	b94006a2 	ldr	w2, [x21, #4]
    48704218:	35000101 	cbnz	w1, 48704238 <ddr_hw_training_process+0x40>
    4870421c:	52800013 	mov	w19, #0x0                   	// #0
    48704220:	2a1303e0 	mov	w0, w19
    48704224:	f9401bf7 	ldr	x23, [sp, #48]
    48704228:	a94153f3 	ldp	x19, x20, [sp, #16]
    4870422c:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48704230:	a8c47bfd 	ldp	x29, x30, [sp], #64
    48704234:	d65f03c0 	ret
    48704238:	2a0103f4 	mov	w20, w1
    4870423c:	32000021 	orr	w1, w1, #0x1
    48704240:	2a020021 	orr	w1, w1, w2
    48704244:	910012b7 	add	x23, x21, #0x4
    48704248:	b90006a1 	str	w1, [x21, #4]
    4870424c:	376001b4 	tbnz	w20, #12, 48704280 <ddr_hw_training_process+0x88>
    48704250:	52800013 	mov	w19, #0x0                   	// #0
    48704254:	b94002e0 	ldr	w0, [x23]
    48704258:	11000673 	add	w19, w19, #0x1
    4870425c:	360001c0 	tbz	w0, #0, 48704294 <ddr_hw_training_process+0x9c>
    48704260:	3100067f 	cmn	w19, #0x1
    48704264:	54ffff81 	b.ne	48704254 <ddr_hw_training_process+0x5c>  // b.any
    48704268:	b9400aa3 	ldr	w3, [x21, #8]
    4870426c:	2a1403e2 	mov	w2, w20
    48704270:	2a1603e1 	mov	w1, w22
    48704274:	52800200 	mov	w0, #0x10                  	// #16
    48704278:	97ffff80 	bl	48704078 <ddr_training_stat>
    4870427c:	17ffffe9 	b	48704220 <ddr_hw_training_process+0x28>
    48704280:	52800041 	mov	w1, #0x2                   	// #2
    48704284:	12800013 	mov	w19, #0xffffffff            	// #-1
    48704288:	97ffff9f 	bl	48704104 <ddr_training_ctrl_easr>
    4870428c:	34fffe20 	cbz	w0, 48704250 <ddr_hw_training_process+0x58>
    48704290:	17ffffe4 	b	48704220 <ddr_hw_training_process+0x28>
    48704294:	b9400aa0 	ldr	w0, [x21, #8]
    48704298:	34fffc20 	cbz	w0, 4870421c <ddr_hw_training_process+0x24>
    4870429c:	b9400aa3 	ldr	w3, [x21, #8]
    487042a0:	2a1403e2 	mov	w2, w20
    487042a4:	2a1603e1 	mov	w1, w22
    487042a8:	52800200 	mov	w0, #0x10                  	// #16
    487042ac:	12800013 	mov	w19, #0xffffffff            	// #-1
    487042b0:	97ffff72 	bl	48704078 <ddr_training_stat>
    487042b4:	17ffffdb 	b	48704220 <ddr_hw_training_process+0x28>

00000000487042b8 <ddr_training_check_bypass>:
    487042b8:	b9408400 	ldr	w0, [x0, #132]
    487042bc:	6a00003f 	tst	w1, w0
    487042c0:	1a9f07e0 	cset	w0, ne  // ne = any
    487042c4:	d65f03c0 	ret

00000000487042c8 <ddr_training_phy_disable>:
    487042c8:	52800000 	mov	w0, #0x0                   	// #0
    487042cc:	d65f03c0 	ret

00000000487042d0 <ddr_training_switch_axi>:
    487042d0:	b9409803 	ldr	w3, [x0, #152]
    487042d4:	d2800782 	mov	x2, #0x3c                  	// #60
    487042d8:	2a0303e1 	mov	w1, w3
    487042dc:	9b020021 	madd	x1, x1, x2, x0
    487042e0:	b9400421 	ldr	w1, [x1, #4]
    487042e4:	7100183f 	cmp	w1, #0x6
    487042e8:	54000061 	b.ne	487042f4 <ddr_training_switch_axi+0x24>  // b.any
    487042ec:	b940a001 	ldr	w1, [x0, #160]
    487042f0:	0b030423 	add	w3, w1, w3, lsl #1
    487042f4:	d2802081 	mov	x1, #0x104                 	// #260
    487042f8:	f2a240c1 	movk	x1, #0x1206, lsl #16
    487042fc:	b9400022 	ldr	w2, [x1]
    48704300:	121c6c42 	and	w2, w2, #0xfffffff0
    48704304:	321e0042 	orr	w2, w2, #0x4
    48704308:	2a030042 	orr	w2, w2, w3
    4870430c:	b9000022 	str	w2, [x1]
    48704310:	d2802282 	mov	x2, #0x114                 	// #276
    48704314:	f2a240c2 	movk	x2, #0x1206, lsl #16
    48704318:	b9400041 	ldr	w1, [x2]
    4870431c:	121c6c21 	and	w1, w1, #0xfffffff0
    48704320:	321e0021 	orr	w1, w1, #0x4
    48704324:	2a030021 	orr	w1, w1, w3
    48704328:	b9000041 	str	w1, [x2]
    4870432c:	b9409c01 	ldr	w1, [x0, #156]
    48704330:	7100043f 	cmp	w1, #0x1
    48704334:	540000a1 	b.ne	48704348 <ddr_training_switch_axi+0x78>  // b.any
    48704338:	b9408001 	ldr	w1, [x0, #128]
    4870433c:	b9406020 	ldr	w0, [x1, #96]
    48704340:	121d7000 	and	w0, w0, #0xfffffff8
    48704344:	b9006020 	str	w0, [x1, #96]
    48704348:	d65f03c0 	ret

000000004870434c <ddr_phy_cfg_update>:
    4870434c:	2a0003e0 	mov	w0, w0
    48704350:	b9407001 	ldr	w1, [x0, #112]
    48704354:	320d0022 	orr	w2, w1, #0x80000
    48704358:	120c7821 	and	w1, w1, #0xfff7ffff
    4870435c:	b9007002 	str	w2, [x0, #112]
    48704360:	b9007001 	str	w1, [x0, #112]
    48704364:	b9400401 	ldr	w1, [x0, #4]
    48704368:	32110022 	orr	w2, w1, #0x8000
    4870436c:	12107821 	and	w1, w1, #0xffff7fff
    48704370:	b9000402 	str	w2, [x0, #4]
    48704374:	b9000401 	str	w1, [x0, #4]
    48704378:	d5033f9f 	dsb	sy
    4870437c:	d65f03c0 	ret

0000000048704380 <ddr_training_save_reg>:
    48704380:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48704384:	aa0003e4 	mov	x4, x0
    48704388:	aa0103e3 	mov	x3, x1
    4870438c:	7104005f 	cmp	w2, #0x100
    48704390:	910003fd 	mov	x29, sp
    48704394:	b9408000 	ldr	w0, [x0, #128]
    48704398:	b9407c86 	ldr	w6, [x4, #124]
    4870439c:	2a0003e5 	mov	w5, w0
    487043a0:	b94108a1 	ldr	w1, [x5, #264]
    487043a4:	b9000061 	str	w1, [x3]
    487043a8:	b94028a4 	ldr	w4, [x5, #40]
    487043ac:	b9000464 	str	w4, [x3, #4]
    487043b0:	2a0603e4 	mov	w4, w6
    487043b4:	b9407087 	ldr	w7, [x4, #112]
    487043b8:	b9001067 	str	w7, [x3, #16]
    487043bc:	b9506487 	ldr	w7, [x4, #4196]
    487043c0:	b9001467 	str	w7, [x3, #20]
    487043c4:	b9506487 	ldr	w7, [x4, #4196]
    487043c8:	b9001467 	str	w7, [x3, #20]
    487043cc:	54000460 	b.eq	48704458 <ddr_training_save_reg+0xd8>  // b.none
    487043d0:	540003c8 	b.hi	48704448 <ddr_training_save_reg+0xc8>  // b.pmore
    487043d4:	7100405f 	cmp	w2, #0x10
    487043d8:	54000061 	b.ne	487043e4 <ddr_training_save_reg+0x64>  // b.any
    487043dc:	12144c21 	and	w1, w1, #0xfffff000
    487043e0:	97fffe7e 	bl	48703dd8 <ddr_training_set_timing>
    487043e4:	b9400460 	ldr	w0, [x3, #4]
    487043e8:	121f7800 	and	w0, w0, #0xfffffffe
    487043ec:	b90028a0 	str	w0, [x5, #40]
    487043f0:	b9401060 	ldr	w0, [x3, #16]
    487043f4:	120f7800 	and	w0, w0, #0xfffeffff
    487043f8:	b9007080 	str	w0, [x4, #112]
    487043fc:	b9408480 	ldr	w0, [x4, #132]
    48704400:	b9001860 	str	w0, [x3, #24]
    48704404:	12057000 	and	w0, w0, #0xf8ffffff
    48704408:	b9008480 	str	w0, [x4, #132]
    4870440c:	d2802080 	mov	x0, #0x104                 	// #260
    48704410:	f2a240c0 	movk	x0, #0x1206, lsl #16
    48704414:	b9400000 	ldr	w0, [x0]
    48704418:	b9001c60 	str	w0, [x3, #28]
    4870441c:	d2802280 	mov	x0, #0x114                 	// #276
    48704420:	f2a240c0 	movk	x0, #0x1206, lsl #16
    48704424:	b9400000 	ldr	w0, [x0]
    48704428:	b9002060 	str	w0, [x3, #32]
    4870442c:	b94060a0 	ldr	w0, [x5, #96]
    48704430:	b9002460 	str	w0, [x3, #36]
    48704434:	2a0603e0 	mov	w0, w6
    48704438:	97ffffc5 	bl	4870434c <ddr_phy_cfg_update>
    4870443c:	d5033f9f 	dsb	sy
    48704440:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48704444:	d65f03c0 	ret
    48704448:	7144005f 	cmp	w2, #0x100, lsl #12
    4870444c:	540000a0 	b.eq	48704460 <ddr_training_save_reg+0xe0>  // b.none
    48704450:	7160005f 	cmp	w2, #0x800, lsl #12
    48704454:	17ffffe1 	b	487043d8 <ddr_training_save_reg+0x58>
    48704458:	12144c21 	and	w1, w1, #0xfffff000
    4870445c:	97fffe5f 	bl	48703dd8 <ddr_training_set_timing>
    48704460:	b9402c80 	ldr	w0, [x4, #44]
    48704464:	372ffc00 	tbnz	w0, #5, 487043e4 <ddr_training_save_reg+0x64>
    48704468:	b910649f 	str	wzr, [x4, #4196]
    4870446c:	17ffffde 	b	487043e4 <ddr_training_save_reg+0x64>

0000000048704470 <ddr_training_restore_reg>:
    48704470:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48704474:	aa0103e3 	mov	x3, x1
    48704478:	910003fd 	mov	x29, sp
    4870447c:	294f9005 	ldp	w5, w4, [x0, #124]
    48704480:	b9400021 	ldr	w1, [x1]
    48704484:	2a0403e0 	mov	w0, w4
    48704488:	97fffe54 	bl	48703dd8 <ddr_training_set_timing>
    4870448c:	2a0403e4 	mov	w4, w4
    48704490:	b9400460 	ldr	w0, [x3, #4]
    48704494:	b9002880 	str	w0, [x4, #40]
    48704498:	2a0503e0 	mov	w0, w5
    4870449c:	b9401061 	ldr	w1, [x3, #16]
    487044a0:	b9007001 	str	w1, [x0, #112]
    487044a4:	b9402c01 	ldr	w1, [x0, #44]
    487044a8:	37280061 	tbnz	w1, #5, 487044b4 <ddr_training_restore_reg+0x44>
    487044ac:	b9401461 	ldr	w1, [x3, #20]
    487044b0:	b9106401 	str	w1, [x0, #4196]
    487044b4:	b9401861 	ldr	w1, [x3, #24]
    487044b8:	b9008401 	str	w1, [x0, #132]
    487044bc:	d2802080 	mov	x0, #0x104                 	// #260
    487044c0:	f2a240c0 	movk	x0, #0x1206, lsl #16
    487044c4:	b9401c61 	ldr	w1, [x3, #28]
    487044c8:	b9000001 	str	w1, [x0]
    487044cc:	b9402061 	ldr	w1, [x3, #32]
    487044d0:	b9001001 	str	w1, [x0, #16]
    487044d4:	b9402460 	ldr	w0, [x3, #36]
    487044d8:	b9006080 	str	w0, [x4, #96]
    487044dc:	2a0503e0 	mov	w0, w5
    487044e0:	97ffff9b 	bl	4870434c <ddr_phy_cfg_update>
    487044e4:	d5033f9f 	dsb	sy
    487044e8:	a8c17bfd 	ldp	x29, x30, [sp], #16
    487044ec:	d65f03c0 	ret

00000000487044f0 <ddr_phy_set_dq_bdl>:
    487044f0:	29520c04 	ldp	w4, w3, [x0, #144]
    487044f4:	b9409c02 	ldr	w2, [x0, #156]
    487044f8:	b9407c06 	ldr	w6, [x0, #124]
    487044fc:	b9408c00 	ldr	w0, [x0, #140]
    48704500:	53196084 	lsl	w4, w4, #7
    48704504:	12000865 	and	w5, w3, #0x7
    48704508:	7100081f 	cmp	w0, #0x2
    4870450c:	53165442 	lsl	w2, w2, #10
    48704510:	0b040042 	add	w2, w2, w4
    48704514:	54000241 	b.ne	4870455c <ddr_phy_set_dq_bdl+0x6c>  // b.any
    48704518:	11084040 	add	w0, w2, #0x210
    4870451c:	11085042 	add	w2, w2, #0x214
    48704520:	71000cbf 	cmp	w5, #0x3
    48704524:	531d0463 	ubfiz	w3, w3, #3, #2
    48704528:	9a808042 	csel	x2, x2, x0, hi  // hi = pmore
    4870452c:	2a0603e0 	mov	w0, w6
    48704530:	52801fe5 	mov	w5, #0xff                  	// #255
    48704534:	12001821 	and	w1, w1, #0x7f
    48704538:	1ac320a5 	lsl	w5, w5, w3
    4870453c:	11000463 	add	w3, w3, #0x1
    48704540:	b8626804 	ldr	w4, [x0, x2]
    48704544:	1ac32023 	lsl	w3, w1, w3
    48704548:	0a250084 	bic	w4, w4, w5
    4870454c:	2a030084 	orr	w4, w4, w3
    48704550:	b8226804 	str	w4, [x0, x2]
    48704554:	2a0603e0 	mov	w0, w6
    48704558:	17ffff7d 	b	4870434c <ddr_phy_cfg_update>
    4870455c:	11087040 	add	w0, w2, #0x21c
    48704560:	11088042 	add	w2, w2, #0x220
    48704564:	17ffffef 	b	48704520 <ddr_phy_set_dq_bdl+0x30>

0000000048704568 <ddr_phy_get_dq_bdl>:
    48704568:	29518c05 	ldp	w5, w3, [x0, #140]
    4870456c:	b9409c01 	ldr	w1, [x0, #156]
    48704570:	b9409402 	ldr	w2, [x0, #148]
    48704574:	710008bf 	cmp	w5, #0x2
    48704578:	53196063 	lsl	w3, w3, #7
    4870457c:	12000844 	and	w4, w2, #0x7
    48704580:	53165421 	lsl	w1, w1, #10
    48704584:	0b030021 	add	w1, w1, w3
    48704588:	54000181 	b.ne	487045b8 <ddr_phy_get_dq_bdl+0x50>  // b.any
    4870458c:	11084023 	add	w3, w1, #0x210
    48704590:	11085021 	add	w1, w1, #0x214
    48704594:	71000c9f 	cmp	w4, #0x3
    48704598:	b9407c00 	ldr	w0, [x0, #124]
    4870459c:	9a838021 	csel	x1, x1, x3, hi  // hi = pmore
    487045a0:	531d0442 	ubfiz	w2, w2, #3, #2
    487045a4:	11000442 	add	w2, w2, #0x1
    487045a8:	b8616800 	ldr	w0, [x0, x1]
    487045ac:	1ac22400 	lsr	w0, w0, w2
    487045b0:	12001800 	and	w0, w0, #0x7f
    487045b4:	d65f03c0 	ret
    487045b8:	11087023 	add	w3, w1, #0x21c
    487045bc:	11088021 	add	w1, w1, #0x220
    487045c0:	17fffff5 	b	48704594 <ddr_phy_get_dq_bdl+0x2c>

00000000487045c4 <ddr_rdqs_sync_rank_rdq>:
    487045c4:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    487045c8:	aa0003e7 	mov	x7, x0
    487045cc:	2a0103eb 	mov	w11, w1
    487045d0:	52800fe2 	mov	w2, #0x7f                  	// #127
    487045d4:	910003fd 	mov	x29, sp
    487045d8:	b9407c04 	ldr	w4, [x0, #124]
    487045dc:	2a0203ea 	mov	w10, w2
    487045e0:	b9409000 	ldr	w0, [x0, #144]
    487045e4:	52800008 	mov	w8, #0x0                   	// #0
    487045e8:	b9409ce1 	ldr	w1, [x7, #156]
    487045ec:	53196000 	lsl	w0, w0, #7
    487045f0:	0b012801 	add	w1, w0, w1, lsl #10
    487045f4:	11089021 	add	w1, w1, #0x224
    487045f8:	b8646823 	ldr	w3, [x1, x4]
    487045fc:	d3411c60 	ubfx	x0, x3, #1, #7
    48704600:	12186063 	and	w3, w3, #0xffffff01
    48704604:	2b0b0000 	adds	w0, w0, w11
    48704608:	1a9f5000 	csel	w0, w0, wzr, pl  // pl = nfrst
    4870460c:	6b02001f 	cmp	w0, w2
    48704610:	1a82d000 	csel	w0, w0, w2, le
    48704614:	2a000460 	orr	w0, w3, w0, lsl #1
    48704618:	b8246820 	str	w0, [x1, x4]
    4870461c:	aa0703e0 	mov	x0, x7
    48704620:	b90094e8 	str	w8, [x7, #148]
    48704624:	97ffffd1 	bl	48704568 <ddr_phy_get_dq_bdl>
    48704628:	2b0b0000 	adds	w0, w0, w11
    4870462c:	1a9f5000 	csel	w0, w0, wzr, pl  // pl = nfrst
    48704630:	7101fc1f 	cmp	w0, #0x7f
    48704634:	1a8ad001 	csel	w1, w0, w10, le
    48704638:	aa0703e0 	mov	x0, x7
    4870463c:	97ffffad 	bl	487044f0 <ddr_phy_set_dq_bdl>
    48704640:	11000508 	add	w8, w8, #0x1
    48704644:	7100211f 	cmp	w8, #0x8
    48704648:	54fffea1 	b.ne	4870461c <ddr_rdqs_sync_rank_rdq+0x58>  // b.any
    4870464c:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48704650:	d65f03c0 	ret

0000000048704654 <ddr_adjust_set_val>:
    48704654:	a9be7bfd 	stp	x29, x30, [sp, #-32]!
    48704658:	2a0103ef 	mov	w15, w1
    4870465c:	aa0003ec 	mov	x12, x0
    48704660:	910003fd 	mov	x29, sp
    48704664:	a90153f3 	stp	x19, x20, [sp, #16]
    48704668:	29518414 	ldp	w20, w1, [x0, #140]
    4870466c:	b9407c02 	ldr	w2, [x0, #124]
    48704670:	b9409c0d 	ldr	w13, [x0, #156]
    48704674:	7100069f 	cmp	w20, #0x1
    48704678:	53196021 	lsl	w1, w1, #7
    4870467c:	540003e1 	b.ne	487046f8 <ddr_adjust_set_val+0xa4>  // b.any
    48704680:	1108b021 	add	w1, w1, #0x22c
    48704684:	b8626833 	ldr	w19, [x1, x2]
    48704688:	b862682e 	ldr	w14, [x1, x2]
    4870468c:	d34125ce 	ubfx	x14, x14, #1, #9
    48704690:	4b0e01ee 	sub	w14, w15, w14
    48704694:	2a0e03e1 	mov	w1, w14
    48704698:	97ffffcb 	bl	487045c4 <ddr_rdqs_sync_rank_rdq>
    4870469c:	b9409980 	ldr	w0, [x12, #152]
    487046a0:	d2800781 	mov	x1, #0x3c                  	// #60
    487046a4:	9b013000 	madd	x0, x0, x1, x12
    487046a8:	b9400c00 	ldr	w0, [x0, #12]
    487046ac:	7100041f 	cmp	w0, #0x1
    487046b0:	540000e0 	b.eq	487046cc <ddr_adjust_set_val+0x78>  // b.none
    487046b4:	4b0d0294 	sub	w20, w20, w13
    487046b8:	2a0e03e1 	mov	w1, w14
    487046bc:	aa0c03e0 	mov	x0, x12
    487046c0:	b9009d94 	str	w20, [x12, #156]
    487046c4:	97ffffc0 	bl	487045c4 <ddr_rdqs_sync_rank_rdq>
    487046c8:	b9009d8d 	str	w13, [x12, #156]
    487046cc:	b9409180 	ldr	w0, [x12, #144]
    487046d0:	12165a73 	and	w19, w19, #0xfffffc01
    487046d4:	b9407d81 	ldr	w1, [x12, #124]
    487046d8:	2a0f066f 	orr	w15, w19, w15, lsl #1
    487046dc:	53196000 	lsl	w0, w0, #7
    487046e0:	1108b000 	add	w0, w0, #0x22c
    487046e4:	b821680f 	str	w15, [x0, x1]
    487046e8:	a94153f3 	ldp	x19, x20, [sp, #16]
    487046ec:	b9407d80 	ldr	w0, [x12, #124]
    487046f0:	a8c27bfd 	ldp	x29, x30, [sp], #32
    487046f4:	17ffff16 	b	4870434c <ddr_phy_cfg_update>
    487046f8:	0b0d282d 	add	w13, w1, w13, lsl #10
    487046fc:	1108d1ad 	add	w13, w13, #0x234
    48704700:	b86269a0 	ldr	w0, [x13, x2]
    48704704:	12106000 	and	w0, w0, #0xffff01ff
    48704708:	2a0f240f 	orr	w15, w0, w15, lsl #9
    4870470c:	b82269af 	str	w15, [x13, x2]
    48704710:	17fffff6 	b	487046e8 <ddr_adjust_set_val+0x94>

0000000048704714 <ddr_phy_get_byte_num>:
    48704714:	2a0003e0 	mov	w0, w0
    48704718:	52800081 	mov	w1, #0x4                   	// #4
    4870471c:	b9405000 	ldr	w0, [x0, #80]
    48704720:	53047c00 	lsr	w0, w0, #4
    48704724:	531f0400 	ubfiz	w0, w0, #1, #2
    48704728:	7100101f 	cmp	w0, #0x4
    4870472c:	1a819000 	csel	w0, w0, w1, ls  // ls = plast
    48704730:	d65f03c0 	ret

0000000048704734 <ddr_training_cfg_set_dmc>:
    48704734:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48704738:	aa0003e2 	mov	x2, x0
    4870473c:	d2801501 	mov	x1, #0xa8                  	// #168
    48704740:	910003fd 	mov	x29, sp
    48704744:	b9400400 	ldr	w0, [x0, #4]
    48704748:	f2a24041 	movk	x1, #0x1202, lsl #16
    4870474c:	7100181f 	cmp	w0, #0x6
    48704750:	52900000 	mov	w0, #0x8000                	// #32768
    48704754:	72a240c0 	movk	w0, #0x1206, lsl #16
    48704758:	540005c1 	b.ne	48704810 <ddr_training_cfg_set_dmc+0xdc>  // b.any
    4870475c:	52800043 	mov	w3, #0x2                   	// #2
    48704760:	b9000843 	str	w3, [x2, #8]
    48704764:	b9400023 	ldr	w3, [x1]
    48704768:	b9001440 	str	w0, [x2, #20]
    4870476c:	12003c61 	and	w1, w3, #0xffff
    48704770:	b9001c41 	str	w1, [x2, #28]
    48704774:	97ffffe8 	bl	48704714 <ddr_phy_get_byte_num>
    48704778:	b9001840 	str	w0, [x2, #24]
    4870477c:	52920000 	mov	w0, #0x9000                	// #36864
    48704780:	53107c63 	lsr	w3, w3, #16
    48704784:	72a240c0 	movk	w0, #0x1206, lsl #16
    48704788:	b9002843 	str	w3, [x2, #40]
    4870478c:	b9002040 	str	w0, [x2, #32]
    48704790:	97ffffe1 	bl	48704714 <ddr_phy_get_byte_num>
    48704794:	b9401841 	ldr	w1, [x2, #24]
    48704798:	b9002440 	str	w0, [x2, #36]
    4870479c:	0b000020 	add	w0, w1, w0
    487047a0:	b9001040 	str	w0, [x2, #16]
    487047a4:	d2801581 	mov	x1, #0xac                  	// #172
    487047a8:	b9404040 	ldr	w0, [x2, #64]
    487047ac:	f2a24041 	movk	x1, #0x1202, lsl #16
    487047b0:	7100181f 	cmp	w0, #0x6
    487047b4:	540003e1 	b.ne	48704830 <ddr_training_cfg_set_dmc+0xfc>  // b.any
    487047b8:	52800040 	mov	w0, #0x2                   	// #2
    487047bc:	b9004440 	str	w0, [x2, #68]
    487047c0:	52940000 	mov	w0, #0xa000                	// #40960
    487047c4:	72a240c0 	movk	w0, #0x1206, lsl #16
    487047c8:	b9400023 	ldr	w3, [x1]
    487047cc:	b9005040 	str	w0, [x2, #80]
    487047d0:	12003c61 	and	w1, w3, #0xffff
    487047d4:	b9005841 	str	w1, [x2, #88]
    487047d8:	97ffffcf 	bl	48704714 <ddr_phy_get_byte_num>
    487047dc:	b9005440 	str	w0, [x2, #84]
    487047e0:	52960000 	mov	w0, #0xb000                	// #45056
    487047e4:	53107c63 	lsr	w3, w3, #16
    487047e8:	72a240c0 	movk	w0, #0x1206, lsl #16
    487047ec:	b9006443 	str	w3, [x2, #100]
    487047f0:	b9005c40 	str	w0, [x2, #92]
    487047f4:	97ffffc8 	bl	48704714 <ddr_phy_get_byte_num>
    487047f8:	b9405441 	ldr	w1, [x2, #84]
    487047fc:	b9006040 	str	w0, [x2, #96]
    48704800:	0b000020 	add	w0, w1, w0
    48704804:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48704808:	b9004c40 	str	w0, [x2, #76]
    4870480c:	d65f03c0 	ret
    48704810:	52800023 	mov	w3, #0x1                   	// #1
    48704814:	b9001440 	str	w0, [x2, #20]
    48704818:	b9000843 	str	w3, [x2, #8]
    4870481c:	b9400021 	ldr	w1, [x1]
    48704820:	b9001c41 	str	w1, [x2, #28]
    48704824:	97ffffbc 	bl	48704714 <ddr_phy_get_byte_num>
    48704828:	b9001840 	str	w0, [x2, #24]
    4870482c:	17ffffdd 	b	487047a0 <ddr_training_cfg_set_dmc+0x6c>
    48704830:	52800020 	mov	w0, #0x1                   	// #1
    48704834:	b9004440 	str	w0, [x2, #68]
    48704838:	52920000 	mov	w0, #0x9000                	// #36864
    4870483c:	72a240c0 	movk	w0, #0x1206, lsl #16
    48704840:	b9005040 	str	w0, [x2, #80]
    48704844:	b9400021 	ldr	w1, [x1]
    48704848:	b9005841 	str	w1, [x2, #88]
    4870484c:	97ffffb2 	bl	48704714 <ddr_phy_get_byte_num>
    48704850:	b9005440 	str	w0, [x2, #84]
    48704854:	17ffffec 	b	48704804 <ddr_training_cfg_set_dmc+0xd0>

0000000048704858 <ddr_training_cfg_init>:
    48704858:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    4870485c:	aa0003e4 	mov	x4, x0
    48704860:	52801802 	mov	w2, #0xc0                  	// #192
    48704864:	52800001 	mov	w1, #0x0                   	// #0
    48704868:	910003fd 	mov	x29, sp
    4870486c:	97fffca0 	bl	48703aec <ddrtr_memset>
    48704870:	aa0403e0 	mov	x0, x4
    48704874:	97fffd46 	bl	48703d8c <ddr_training_cfg_set_phy>
    48704878:	aa0403e0 	mov	x0, x4
    4870487c:	97ffffae 	bl	48704734 <ddr_training_cfg_set_dmc>
    48704880:	aa0403e0 	mov	x0, x4
    48704884:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48704888:	17fffd17 	b	48703ce4 <ddr_training_cfg_set_rank>

000000004870488c <ddr_ddrt_init>:
    4870488c:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
    48704890:	910003fd 	mov	x29, sp
    48704894:	a90153f3 	stp	x19, x20, [sp, #16]
    48704898:	aa0003f3 	mov	x19, x0
    4870489c:	f90013f5 	str	x21, [sp, #32]
    487048a0:	d2800014 	mov	x20, #0x0                   	// #0
    487048a4:	2a0103f5 	mov	w21, w1
    487048a8:	b9409c00 	ldr	w0, [x0, #156]
    487048ac:	7100041f 	cmp	w0, #0x1
    487048b0:	540001c1 	b.ne	487048e8 <ddr_ddrt_init+0x5c>  // b.any
    487048b4:	b9408262 	ldr	w2, [x19, #128]
    487048b8:	d2800034 	mov	x20, #0x1                   	// #1
    487048bc:	b9405040 	ldr	w0, [x2, #80]
    487048c0:	b9406042 	ldr	w2, [x2, #96]
    487048c4:	d3441400 	ubfx	x0, x0, #4, #2
    487048c8:	d3482441 	ubfx	x1, x2, #8, #2
    487048cc:	12000843 	and	w3, w2, #0x7
    487048d0:	0b010000 	add	w0, w0, w1
    487048d4:	11005463 	add	w3, w3, #0x15
    487048d8:	0b030000 	add	w0, w0, w3
    487048dc:	d3441842 	ubfx	x2, x2, #4, #3
    487048e0:	0b020000 	add	w0, w0, w2
    487048e4:	9ac02294 	lsl	x20, x20, x0
    487048e8:	b9408260 	ldr	w0, [x19, #128]
    487048ec:	52802a41 	mov	w1, #0x152                 	// #338
    487048f0:	b9405000 	ldr	w0, [x0, #80]
    487048f4:	d3441400 	ubfx	x0, x0, #4, #2
    487048f8:	51000400 	sub	w0, w0, #0x1
    487048fc:	2a003020 	orr	w0, w1, w0, lsl #12
    48704900:	d2800181 	mov	x1, #0xc                   	// #12
    48704904:	f2a24841 	movk	x1, #0x1242, lsl #16
    48704908:	b9000020 	str	w0, [x1]
    4870490c:	d2800380 	mov	x0, #0x1c                  	// #28
    48704910:	f2a24840 	movk	x0, #0x1242, lsl #16
    48704914:	52a20001 	mov	w1, #0x10000000            	// #268435456
    48704918:	b9000001 	str	w1, [x0]
    4870491c:	94000818 	bl	4870697c <ddr_ddrt_get_test_addr>
    48704920:	0b140000 	add	w0, w0, w20
    48704924:	d2800401 	mov	x1, #0x20                  	// #32
    48704928:	710006bf 	cmp	w21, #0x1
    4870492c:	f2a24841 	movk	x1, #0x1242, lsl #16
    48704930:	53027c00 	lsr	w0, w0, #2
    48704934:	b9000020 	str	w0, [x1]
    48704938:	d2800700 	mov	x0, #0x38                  	// #56
    4870493c:	f2a24840 	movk	x0, #0x1242, lsl #16
    48704940:	528dada1 	mov	w1, #0x6d6d                	// #28013
    48704944:	72adada1 	movk	w1, #0x6d6d, lsl #16
    48704948:	b9000001 	str	w1, [x0]
    4870494c:	d2800100 	mov	x0, #0x8                   	// #8
    48704950:	f2a24840 	movk	x0, #0x1242, lsl #16
    48704954:	540001a1 	b.ne	48704988 <ddr_ddrt_init+0xfc>  // b.any
    48704958:	52800861 	mov	w1, #0x43                  	// #67
    4870495c:	b9000001 	str	w1, [x0]
    48704960:	528ab541 	mov	w1, #0x55aa                	// #21930
    48704964:	b900081f 	str	wzr, [x0, #8]
    48704968:	72aab541 	movk	w1, #0x55aa, lsl #16
    4870496c:	b9000c1f 	str	wzr, [x0, #12]
    48704970:	b900101f 	str	wzr, [x0, #16]
    48704974:	b9002801 	str	w1, [x0, #40]
    48704978:	a94153f3 	ldp	x19, x20, [sp, #16]
    4870497c:	f94013f5 	ldr	x21, [sp, #32]
    48704980:	a8c37bfd 	ldp	x29, x30, [sp], #48
    48704984:	d65f03c0 	ret
    48704988:	528009e1 	mov	w1, #0x4f                  	// #79
    4870498c:	d2800782 	mov	x2, #0x3c                  	// #60
    48704990:	b9000001 	str	w1, [x0]
    48704994:	b9409a61 	ldr	w1, [x19, #152]
    48704998:	b940a260 	ldr	w0, [x19, #160]
    4870499c:	9b027c21 	mul	x1, x1, x2
    487049a0:	d2800182 	mov	x2, #0xc                   	// #12
    487049a4:	9b020400 	madd	x0, x0, x2, x1
    487049a8:	8b000273 	add	x19, x19, x0
    487049ac:	d2800600 	mov	x0, #0x30                  	// #48
    487049b0:	f2a24840 	movk	x0, #0x1242, lsl #16
    487049b4:	b9401e61 	ldr	w1, [x19, #28]
    487049b8:	b9000001 	str	w1, [x0]
    487049bc:	d2800200 	mov	x0, #0x10                  	// #16
    487049c0:	f2a24840 	movk	x0, #0x1242, lsl #16
    487049c4:	52800fe1 	mov	w1, #0x7f                  	// #127
    487049c8:	b9000001 	str	w1, [x0]
    487049cc:	12800001 	mov	w1, #0xffffffff            	// #-1
    487049d0:	b9000401 	str	w1, [x0, #4]
    487049d4:	b900081f 	str	wzr, [x0, #8]
    487049d8:	17ffffe8 	b	48704978 <ddr_ddrt_init+0xec>

00000000487049dc <ddr_ddrt_test>:
    487049dc:	d2a24843 	mov	x3, #0x12420000            	// #306315264
    487049e0:	32000004 	orr	w4, w0, #0x1
    487049e4:	b9000064 	str	w4, [x3]
    487049e8:	d2800084 	mov	x4, #0x4                   	// #4
    487049ec:	f2a24844 	movk	x4, #0x1242, lsl #16
    487049f0:	b900009f 	str	wzr, [x4]
    487049f4:	d5033f9f 	dsb	sy
    487049f8:	52800003 	mov	w3, #0x0                   	// #0
    487049fc:	b9400085 	ldr	w5, [x4]
    48704a00:	11000463 	add	w3, w3, #0x1
    48704a04:	370001a5 	tbnz	w5, #0, 48704a38 <ddr_ddrt_test+0x5c>
    48704a08:	710fa07f 	cmp	w3, #0x3e8
    48704a0c:	54ffff81 	b.ne	487049fc <ddr_ddrt_test+0x20>  // b.any
    48704a10:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48704a14:	12800003 	mov	w3, #0xffffffff            	// #-1
    48704a18:	2a0303e2 	mov	w2, w3
    48704a1c:	52800001 	mov	w1, #0x0                   	// #0
    48704a20:	910003fd 	mov	x29, sp
    48704a24:	52800100 	mov	w0, #0x8                   	// #8
    48704a28:	97fffd94 	bl	48704078 <ddr_training_stat>
    48704a2c:	12800000 	mov	w0, #0xffffffff            	// #-1
    48704a30:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48704a34:	d65f03c0 	ret
    48704a38:	710fa07f 	cmp	w3, #0x3e8
    48704a3c:	54fffea0 	b.eq	48704a10 <ddr_ddrt_test+0x34>  // b.none
    48704a40:	12180403 	and	w3, w0, #0x300
    48704a44:	52800000 	mov	w0, #0x0                   	// #0
    48704a48:	7108007f 	cmp	w3, #0x200
    48704a4c:	54000680 	b.eq	48704b1c <ddr_ddrt_test+0x140>  // b.none
    48704a50:	37080665 	tbnz	w5, #1, 48704b1c <ddr_ddrt_test+0x140>
    48704a54:	3100045f 	cmn	w2, #0x1
    48704a58:	54000360 	b.eq	48704ac4 <ddr_ddrt_test+0xe8>  // b.none
    48704a5c:	d2801000 	mov	x0, #0x80                  	// #128
    48704a60:	52800023 	mov	w3, #0x1                   	// #1
    48704a64:	f2a24840 	movk	x0, #0x1242, lsl #16
    48704a68:	b9400004 	ldr	w4, [x0]
    48704a6c:	0b010c40 	add	w0, w2, w1, lsl #3
    48704a70:	1ac02063 	lsl	w3, w3, w0
    48704a74:	12800000 	mov	w0, #0xffffffff            	// #-1
    48704a78:	6a04007f 	tst	w3, w4
    48704a7c:	54000501 	b.ne	48704b1c <ddr_ddrt_test+0x140>  // b.any
    48704a80:	71000c5f 	cmp	w2, #0x3
    48704a84:	531d7043 	lsl	w3, w2, #3
    48704a88:	5400006d 	b.le	48704a94 <ddr_ddrt_test+0xb8>
    48704a8c:	51001043 	sub	w3, w2, #0x4
    48704a90:	531d7063 	lsl	w3, w3, #3
    48704a94:	13027c40 	asr	w0, w2, #2
    48704a98:	0b010400 	add	w0, w0, w1, lsl #1
    48704a9c:	d2a24841 	mov	x1, #0x12420000            	// #306315264
    48704aa0:	531e7400 	lsl	w0, w0, #2
    48704aa4:	11018000 	add	w0, w0, #0x60
    48704aa8:	93407c00 	sxtw	x0, w0
    48704aac:	b8616802 	ldr	w2, [x0, x1]
    48704ab0:	52801fe0 	mov	w0, #0xff                  	// #255
    48704ab4:	1ac32003 	lsl	w3, w0, w3
    48704ab8:	6a02007f 	tst	w3, w2
    48704abc:	5a9f03e0 	csetm	w0, ne  // ne = any
    48704ac0:	d65f03c0 	ret
    48704ac4:	3100043f 	cmn	w1, #0x1
    48704ac8:	540002a0 	b.eq	48704b1c <ddr_ddrt_test+0x140>  // b.none
    48704acc:	d2801000 	mov	x0, #0x80                  	// #128
    48704ad0:	52801fe3 	mov	w3, #0xff                  	// #255
    48704ad4:	f2a24840 	movk	x0, #0x1242, lsl #16
    48704ad8:	1ac12063 	lsl	w3, w3, w1
    48704adc:	b9400004 	ldr	w4, [x0]
    48704ae0:	2a0203e0 	mov	w0, w2
    48704ae4:	6a04007f 	tst	w3, w4
    48704ae8:	540001a1 	b.ne	48704b1c <ddr_ddrt_test+0x140>  // b.any
    48704aec:	531d7020 	lsl	w0, w1, #3
    48704af0:	d2a24843 	mov	x3, #0x12420000            	// #306315264
    48704af4:	11018000 	add	w0, w0, #0x60
    48704af8:	93407c00 	sxtw	x0, w0
    48704afc:	b8636802 	ldr	w2, [x0, x3]
    48704b00:	531d7020 	lsl	w0, w1, #3
    48704b04:	11019000 	add	w0, w0, #0x64
    48704b08:	93407c00 	sxtw	x0, w0
    48704b0c:	b8636800 	ldr	w0, [x0, x3]
    48704b10:	2b00005f 	cmn	w2, w0
    48704b14:	5a9f03e0 	csetm	w0, ne  // ne = any
    48704b18:	d65f03c0 	ret
    48704b1c:	d65f03c0 	ret

0000000048704b20 <ddr_dataeye_check_dq>:
    48704b20:	b940a801 	ldr	w1, [x0, #168]
    48704b24:	7100043f 	cmp	w1, #0x1
    48704b28:	54000441 	b.ne	48704bb0 <ddr_dataeye_check_dq+0x90>  // b.any
    48704b2c:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
    48704b30:	910003fd 	mov	x29, sp
    48704b34:	a90153f3 	stp	x19, x20, [sp, #16]
    48704b38:	aa0003f3 	mov	x19, x0
    48704b3c:	f90013f5 	str	x21, [sp, #32]
    48704b40:	d2800615 	mov	x21, #0x30                  	// #48
    48704b44:	f2a24855 	movk	x21, #0x1242, lsl #16
    48704b48:	b9409014 	ldr	w20, [x0, #144]
    48704b4c:	b940a000 	ldr	w0, [x0, #160]
    48704b50:	b90002bf 	str	wzr, [x21]
    48704b54:	b9409662 	ldr	w2, [x19, #148]
    48704b58:	531f7800 	lsl	w0, w0, #1
    48704b5c:	4b000281 	sub	w1, w20, w0
    48704b60:	6b00029f 	cmp	w20, w0
    48704b64:	1a942034 	csel	w20, w1, w20, cs  // cs = hs, nlast
    48704b68:	52800000 	mov	w0, #0x0                   	// #0
    48704b6c:	2a1403e1 	mov	w1, w20
    48704b70:	97ffff9b 	bl	487049dc <ddr_ddrt_test>
    48704b74:	12800001 	mov	w1, #0xffffffff            	// #-1
    48704b78:	35000120 	cbnz	w0, 48704b9c <ddr_dataeye_check_dq+0x7c>
    48704b7c:	b9408a60 	ldr	w0, [x19, #136]
    48704b80:	2a1403e1 	mov	w1, w20
    48704b84:	b90002a0 	str	w0, [x21]
    48704b88:	52840000 	mov	w0, #0x2000                	// #8192
    48704b8c:	b9409662 	ldr	w2, [x19, #148]
    48704b90:	97ffff93 	bl	487049dc <ddr_ddrt_test>
    48704b94:	7100001f 	cmp	w0, #0x0
    48704b98:	5a9f03e1 	csetm	w1, ne  // ne = any
    48704b9c:	2a0103e0 	mov	w0, w1
    48704ba0:	f94013f5 	ldr	x21, [sp, #32]
    48704ba4:	a94153f3 	ldp	x19, x20, [sp, #16]
    48704ba8:	a8c37bfd 	ldp	x29, x30, [sp], #48
    48704bac:	d65f03c0 	ret
    48704bb0:	52800001 	mov	w1, #0x0                   	// #0
    48704bb4:	2a0103e0 	mov	w0, w1
    48704bb8:	d65f03c0 	ret

0000000048704bbc <ddr_dataeye_search_dq>:
    48704bbc:	a9bb7bfd 	stp	x29, x30, [sp, #-80]!
    48704bc0:	910003fd 	mov	x29, sp
    48704bc4:	a90363f7 	stp	x23, x24, [sp, #48]
    48704bc8:	2a0103f8 	mov	w24, w1
    48704bcc:	a9046bf9 	stp	x25, x26, [sp, #64]
    48704bd0:	2a0303f7 	mov	w23, w3
    48704bd4:	a90153f3 	stp	x19, x20, [sp, #16]
    48704bd8:	aa0203f9 	mov	x25, x2
    48704bdc:	a9025bf5 	stp	x21, x22, [sp, #32]
    48704be0:	aa0403f4 	mov	x20, x4
    48704be4:	2a0003f5 	mov	w21, w0
    48704be8:	528000da 	mov	w26, #0x6                   	// #6
    48704bec:	4b150313 	sub	w19, w24, w21
    48704bf0:	aa1403e0 	mov	x0, x20
    48704bf4:	0b5306b3 	add	w19, w21, w19, lsr #1
    48704bf8:	2a1303e1 	mov	w1, w19
    48704bfc:	97fffe3d 	bl	487044f0 <ddr_phy_set_dq_bdl>
    48704c00:	aa1403e0 	mov	x0, x20
    48704c04:	97ffffc7 	bl	48704b20 <ddr_dataeye_check_dq>
    48704c08:	71000aff 	cmp	w23, #0x2
    48704c0c:	540001a0 	b.eq	48704c40 <ddr_dataeye_search_dq+0x84>  // b.none
    48704c10:	710012ff 	cmp	w23, #0x4
    48704c14:	54000240 	b.eq	48704c5c <ddr_dataeye_search_dq+0xa0>  // b.none
    48704c18:	710006ff 	cmp	w23, #0x1
    48704c1c:	1a9f0356 	csel	w22, w26, wzr, eq  // eq = none
    48704c20:	35000260 	cbnz	w0, 48704c6c <ddr_dataeye_search_dq+0xb0>
    48704c24:	b9000333 	str	w19, [x25]
    48704c28:	a94153f3 	ldp	x19, x20, [sp, #16]
    48704c2c:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48704c30:	a94363f7 	ldp	x23, x24, [sp, #48]
    48704c34:	a9446bf9 	ldp	x25, x26, [sp, #64]
    48704c38:	a8c57bfd 	ldp	x29, x30, [sp], #80
    48704c3c:	d65f03c0 	ret
    48704c40:	35000140 	cbnz	w0, 48704c68 <ddr_dataeye_search_dq+0xac>
    48704c44:	2a1503e1 	mov	w1, w21
    48704c48:	aa1403e0 	mov	x0, x20
    48704c4c:	97fffe29 	bl	487044f0 <ddr_phy_set_dq_bdl>
    48704c50:	aa1403e0 	mov	x0, x20
    48704c54:	97ffffb3 	bl	48704b20 <ddr_dataeye_check_dq>
    48704c58:	17fffff3 	b	48704c24 <ddr_dataeye_search_dq+0x68>
    48704c5c:	35000220 	cbnz	w0, 48704ca0 <ddr_dataeye_search_dq+0xe4>
    48704c60:	2a1803e1 	mov	w1, w24
    48704c64:	17fffff9 	b	48704c48 <ddr_dataeye_search_dq+0x8c>
    48704c68:	52800096 	mov	w22, #0x4                   	// #4
    48704c6c:	6b1302bf 	cmp	w21, w19
    48704c70:	7a581264 	ccmp	w19, w24, #0x4, ne  // ne = any
    48704c74:	54fffda0 	b.eq	48704c28 <ddr_dataeye_search_dq+0x6c>  // b.none
    48704c78:	360800f6 	tbz	w22, #1, 48704c94 <ddr_dataeye_search_dq+0xd8>
    48704c7c:	aa1403e4 	mov	x4, x20
    48704c80:	2a1703e3 	mov	w3, w23
    48704c84:	aa1903e2 	mov	x2, x25
    48704c88:	2a1303e1 	mov	w1, w19
    48704c8c:	2a1503e0 	mov	w0, w21
    48704c90:	97ffffcb 	bl	48704bbc <ddr_dataeye_search_dq>
    48704c94:	2a1303f5 	mov	w21, w19
    48704c98:	3617fc96 	tbz	w22, #2, 48704c28 <ddr_dataeye_search_dq+0x6c>
    48704c9c:	17ffffd4 	b	48704bec <ddr_dataeye_search_dq+0x30>
    48704ca0:	52800056 	mov	w22, #0x2                   	// #2
    48704ca4:	17fffff2 	b	48704c6c <ddr_dataeye_search_dq+0xb0>

0000000048704ca8 <ddr_dataeye_deskew>:
    48704ca8:	a9b87bfd 	stp	x29, x30, [sp, #-128]!
    48704cac:	910003fd 	mov	x29, sp
    48704cb0:	a9025bf5 	stp	x21, x22, [sp, #32]
    48704cb4:	aa0103f6 	mov	x22, x1
    48704cb8:	a90153f3 	stp	x19, x20, [sp, #16]
    48704cbc:	aa0003f3 	mov	x19, x0
    48704cc0:	a90363f7 	stp	x23, x24, [sp, #48]
    48704cc4:	52800015 	mov	w21, #0x0                   	// #0
    48704cc8:	a9046bf9 	stp	x25, x26, [sp, #64]
    48704ccc:	52800017 	mov	w23, #0x0                   	// #0
    48704cd0:	a90573fb 	stp	x27, x28, [sp, #80]
    48704cd4:	52800019 	mov	w25, #0x0                   	// #0
    48704cd8:	b9409018 	ldr	w24, [x0, #144]
    48704cdc:	b901003f 	str	wzr, [x1, #256]
    48704ce0:	531d7300 	lsl	w0, w24, #3
    48704ce4:	b9006fa0 	str	w0, [x29, #108]
    48704ce8:	b9406fa0 	ldr	w0, [x29, #108]
    48704cec:	b9009675 	str	w21, [x19, #148]
    48704cf0:	0b0002ba 	add	w26, w21, w0
    48704cf4:	aa1303e0 	mov	x0, x19
    48704cf8:	97fffe1c 	bl	48704568 <ddr_phy_get_dq_bdl>
    48704cfc:	2a0003fc 	mov	w28, w0
    48704d00:	29520274 	ldp	w20, w0, [x19, #144]
    48704d04:	0b140c14 	add	w20, w0, w20, lsl #3
    48704d08:	aa1303e0 	mov	x0, x19
    48704d0c:	97fffe17 	bl	48704568 <ddr_phy_get_dq_bdl>
    48704d10:	2a0003fb 	mov	w27, w0
    48704d14:	aa1303e0 	mov	x0, x19
    48704d18:	b90077bb 	str	w27, [x29, #116]
    48704d1c:	97ffff81 	bl	48704b20 <ddr_dataeye_check_dq>
    48704d20:	34000660 	cbz	w0, 48704dec <ddr_dataeye_deskew+0x144>
    48704d24:	12800000 	mov	w0, #0xffffffff            	// #-1
    48704d28:	aa1303e4 	mov	x4, x19
    48704d2c:	9101d3a2 	add	x2, x29, #0x74
    48704d30:	52800023 	mov	w3, #0x1                   	// #1
    48704d34:	b90077a0 	str	w0, [x29, #116]
    48704d38:	52800fe1 	mov	w1, #0x7f                  	// #127
    48704d3c:	52800000 	mov	w0, #0x0                   	// #0
    48704d40:	97ffff9f 	bl	48704bbc <ddr_dataeye_search_dq>
    48704d44:	b94077a0 	ldr	w0, [x29, #116]
    48704d48:	3100041f 	cmn	w0, #0x1
    48704d4c:	54000501 	b.ne	48704dec <ddr_dataeye_deskew+0x144>  // b.any
    48704d50:	d37e7e94 	ubfiz	x20, x20, #2, #32
    48704d54:	2a1b03e1 	mov	w1, w27
    48704d58:	8b1402c0 	add	x0, x22, x20
    48704d5c:	b8346adf 	str	wzr, [x22, x20]
    48704d60:	b900801f 	str	wzr, [x0, #128]
    48704d64:	aa1303e0 	mov	x0, x19
    48704d68:	97fffde2 	bl	487044f0 <ddr_phy_set_dq_bdl>
    48704d6c:	8b3a4ada 	add	x26, x22, w26, uxtw #2
    48704d70:	b9408347 	ldr	w7, [x26, #128]
    48704d74:	53107ce8 	lsr	w8, w7, #16
    48704d78:	71001d1f 	cmp	w8, #0x7
    48704d7c:	54000b88 	b.hi	48704eec <ddr_dataeye_deskew+0x244>  // b.pmore
    48704d80:	35000a39 	cbnz	w25, 48704ec4 <ddr_dataeye_deskew+0x21c>
    48704d84:	510006b5 	sub	w21, w21, #0x1
    48704d88:	52800039 	mov	w25, #0x1                   	// #1
    48704d8c:	110006b5 	add	w21, w21, #0x1
    48704d90:	71001ebf 	cmp	w21, #0x7
    48704d94:	54fffaad 	b.le	48704ce8 <ddr_dataeye_deskew+0x40>
    48704d98:	b9408e60 	ldr	w0, [x19, #140]
    48704d9c:	7100081f 	cmp	w0, #0x2
    48704da0:	54000121 	b.ne	48704dc4 <ddr_dataeye_deskew+0x11c>  // b.any
    48704da4:	b9409e60 	ldr	w0, [x19, #156]
    48704da8:	53196318 	lsl	w24, w24, #7
    48704dac:	53037ef7 	lsr	w23, w23, #3
    48704db0:	531f1af7 	ubfiz	w23, w23, #1, #7
    48704db4:	0b002b18 	add	w24, w24, w0, lsl #10
    48704db8:	b9407e60 	ldr	w0, [x19, #124]
    48704dbc:	11086318 	add	w24, w24, #0x218
    48704dc0:	b8206b17 	str	w23, [x24, x0]
    48704dc4:	b9407e60 	ldr	w0, [x19, #124]
    48704dc8:	97fffd61 	bl	4870434c <ddr_phy_cfg_update>
    48704dcc:	52800000 	mov	w0, #0x0                   	// #0
    48704dd0:	a94153f3 	ldp	x19, x20, [sp, #16]
    48704dd4:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48704dd8:	a94363f7 	ldp	x23, x24, [sp, #48]
    48704ddc:	a9446bf9 	ldp	x25, x26, [sp, #64]
    48704de0:	a94573fb 	ldp	x27, x28, [sp, #80]
    48704de4:	a8c87bfd 	ldp	x29, x30, [sp], #128
    48704de8:	d65f03c0 	ret
    48704dec:	b94077a1 	ldr	w1, [x29, #116]
    48704df0:	aa1303e4 	mov	x4, x19
    48704df4:	9101e3a2 	add	x2, x29, #0x78
    48704df8:	52800043 	mov	w3, #0x2                   	// #2
    48704dfc:	52800000 	mov	w0, #0x0                   	// #0
    48704e00:	b9007ba1 	str	w1, [x29, #120]
    48704e04:	97ffff6e 	bl	48704bbc <ddr_dataeye_search_dq>
    48704e08:	b9407ba1 	ldr	w1, [x29, #120]
    48704e0c:	7100003f 	cmp	w1, #0x0
    48704e10:	5400016d 	b.le	48704e3c <ddr_dataeye_deskew+0x194>
    48704e14:	51000421 	sub	w1, w1, #0x1
    48704e18:	aa1303e0 	mov	x0, x19
    48704e1c:	b9007ba1 	str	w1, [x29, #120]
    48704e20:	97fffdb4 	bl	487044f0 <ddr_phy_set_dq_bdl>
    48704e24:	aa1303e0 	mov	x0, x19
    48704e28:	97ffff3e 	bl	48704b20 <ddr_dataeye_check_dq>
    48704e2c:	34fffee0 	cbz	w0, 48704e08 <ddr_dataeye_deskew+0x160>
    48704e30:	b9407ba0 	ldr	w0, [x29, #120]
    48704e34:	11000400 	add	w0, w0, #0x1
    48704e38:	b9007ba0 	str	w0, [x29, #120]
    48704e3c:	b94077a0 	ldr	w0, [x29, #116]
    48704e40:	aa1303e4 	mov	x4, x19
    48704e44:	9101f3a2 	add	x2, x29, #0x7c
    48704e48:	52800083 	mov	w3, #0x4                   	// #4
    48704e4c:	52800fe1 	mov	w1, #0x7f                  	// #127
    48704e50:	b9007fa0 	str	w0, [x29, #124]
    48704e54:	97ffff5a 	bl	48704bbc <ddr_dataeye_search_dq>
    48704e58:	b9407fa1 	ldr	w1, [x29, #124]
    48704e5c:	7101f83f 	cmp	w1, #0x7e
    48704e60:	5400016c 	b.gt	48704e8c <ddr_dataeye_deskew+0x1e4>
    48704e64:	11000421 	add	w1, w1, #0x1
    48704e68:	aa1303e0 	mov	x0, x19
    48704e6c:	b9007fa1 	str	w1, [x29, #124]
    48704e70:	97fffda0 	bl	487044f0 <ddr_phy_set_dq_bdl>
    48704e74:	aa1303e0 	mov	x0, x19
    48704e78:	97ffff2a 	bl	48704b20 <ddr_dataeye_check_dq>
    48704e7c:	34fffee0 	cbz	w0, 48704e58 <ddr_dataeye_deskew+0x1b0>
    48704e80:	b9407fa0 	ldr	w0, [x29, #124]
    48704e84:	51000400 	sub	w0, w0, #0x1
    48704e88:	b9007fa0 	str	w0, [x29, #124]
    48704e8c:	2a1b03e1 	mov	w1, w27
    48704e90:	aa1303e0 	mov	x0, x19
    48704e94:	d37e7e94 	ubfiz	x20, x20, #2, #32
    48704e98:	97fffd96 	bl	487044f0 <ddr_phy_set_dq_bdl>
    48704e9c:	294f0ba1 	ldp	w1, w2, [x29, #120]
    48704ea0:	8b1402c3 	add	x3, x22, x20
    48704ea4:	4b010040 	sub	w0, w2, w1
    48704ea8:	2a014042 	orr	w2, w2, w1, lsl #16
    48704eac:	11000400 	add	w0, w0, #0x1
    48704eb0:	b8346ac2 	str	w2, [x22, x20]
    48704eb4:	0b400421 	add	w1, w1, w0, lsr #1
    48704eb8:	2a004020 	orr	w0, w1, w0, lsl #16
    48704ebc:	b9008060 	str	w0, [x3, #128]
    48704ec0:	17ffffab 	b	48704d6c <ddr_dataeye_deskew+0xc4>
    48704ec4:	35000148 	cbnz	w8, 48704eec <ddr_dataeye_deskew+0x244>
    48704ec8:	2a1c03e1 	mov	w1, w28
    48704ecc:	aa1303e0 	mov	x0, x19
    48704ed0:	97fffd88 	bl	487044f0 <ddr_phy_set_dq_bdl>
    48704ed4:	b9407e61 	ldr	w1, [x19, #124]
    48704ed8:	2a1503e3 	mov	w3, w21
    48704edc:	2a1803e2 	mov	w2, w24
    48704ee0:	52800800 	mov	w0, #0x40                  	// #64
    48704ee4:	97fffc65 	bl	48704078 <ddr_training_stat>
    48704ee8:	17ffffa9 	b	48704d8c <ddr_dataeye_deskew+0xe4>
    48704eec:	12003ce7 	and	w7, w7, #0xffff
    48704ef0:	aa1303e0 	mov	x0, x19
    48704ef4:	2a0703e1 	mov	w1, w7
    48704ef8:	52800019 	mov	w25, #0x0                   	// #0
    48704efc:	97fffd7d 	bl	487044f0 <ddr_phy_set_dq_bdl>
    48704f00:	0b0702f7 	add	w23, w23, w7
    48704f04:	b94102c0 	ldr	w0, [x22, #256]
    48704f08:	0b080000 	add	w0, w0, w8
    48704f0c:	b90102c0 	str	w0, [x22, #256]
    48704f10:	17ffff9f 	b	48704d8c <ddr_dataeye_deskew+0xe4>

0000000048704f14 <ddr_vref_get_win>:
    48704f14:	a9be7bfd 	stp	x29, x30, [sp, #-32]!
    48704f18:	aa0003e5 	mov	x5, x0
    48704f1c:	910003fd 	mov	x29, sp
    48704f20:	f9000bf3 	str	x19, [sp, #16]
    48704f24:	aa0103f3 	mov	x19, x1
    48704f28:	b901003f 	str	wzr, [x1, #256]
    48704f2c:	52800641 	mov	w1, #0x32                  	// #50
    48704f30:	b9408c00 	ldr	w0, [x0, #140]
    48704f34:	7100041f 	cmp	w0, #0x1
    48704f38:	540000c1 	b.ne	48704f50 <ddr_vref_get_win+0x3c>  // b.any
    48704f3c:	b9409ca0 	ldr	w0, [x5, #156]
    48704f40:	528001e1 	mov	w1, #0xf                   	// #15
    48704f44:	7100001f 	cmp	w0, #0x0
    48704f48:	52800fe0 	mov	w0, #0x7f                  	// #127
    48704f4c:	1a810001 	csel	w1, w0, w1, eq  // eq = none
    48704f50:	6b02003f 	cmp	w1, w2
    48704f54:	aa0503e0 	mov	x0, x5
    48704f58:	1a812041 	csel	w1, w2, w1, cs  // cs = hs, nlast
    48704f5c:	97fffbc8 	bl	48703e7c <ddr_vref_set>
    48704f60:	aa0503e0 	mov	x0, x5
    48704f64:	aa1303e1 	mov	x1, x19
    48704f68:	97ffff50 	bl	48704ca8 <ddr_dataeye_deskew>
    48704f6c:	b9410260 	ldr	w0, [x19, #256]
    48704f70:	f9400bf3 	ldr	x19, [sp, #16]
    48704f74:	a8c27bfd 	ldp	x29, x30, [sp], #32
    48704f78:	d65f03c0 	ret

0000000048704f7c <ddr_vref_find_best>:
    48704f7c:	a9bb7bfd 	stp	x29, x30, [sp, #-80]!
    48704f80:	910003fd 	mov	x29, sp
    48704f84:	a9046bf9 	stp	x25, x26, [sp, #64]
    48704f88:	aa0103fa 	mov	x26, x1
    48704f8c:	a90153f3 	stp	x19, x20, [sp, #16]
    48704f90:	2a0303f9 	mov	w25, w3
    48704f94:	a90363f7 	stp	x23, x24, [sp, #48]
    48704f98:	aa0003f7 	mov	x23, x0
    48704f9c:	a9025bf5 	stp	x21, x22, [sp, #32]
    48704fa0:	52800654 	mov	w20, #0x32                  	// #50
    48704fa4:	b9408c00 	ldr	w0, [x0, #140]
    48704fa8:	7100041f 	cmp	w0, #0x1
    48704fac:	540000c1 	b.ne	48704fc4 <ddr_vref_find_best+0x48>  // b.any
    48704fb0:	b9409ee0 	ldr	w0, [x23, #156]
    48704fb4:	52800ff4 	mov	w20, #0x7f                  	// #127
    48704fb8:	7100001f 	cmp	w0, #0x0
    48704fbc:	528001e0 	mov	w0, #0xf                   	// #15
    48704fc0:	1a800294 	csel	w20, w20, w0, eq  // eq = none
    48704fc4:	6b02029f 	cmp	w20, w2
    48704fc8:	0b020335 	add	w21, w25, w2
    48704fcc:	1a829293 	csel	w19, w20, w2, ls  // ls = plast
    48704fd0:	52800016 	mov	w22, #0x0                   	// #0
    48704fd4:	52800018 	mov	w24, #0x0                   	// #0
    48704fd8:	6b15029f 	cmp	w20, w21
    48704fdc:	54000143 	b.cc	48705004 <ddr_vref_find_best+0x88>  // b.lo, b.ul, b.last
    48704fe0:	2a1503e2 	mov	w2, w21
    48704fe4:	aa1a03e1 	mov	x1, x26
    48704fe8:	aa1703e0 	mov	x0, x23
    48704fec:	97ffffca 	bl	48704f14 <ddr_vref_get_win>
    48704ff0:	6b00031f 	cmp	w24, w0
    48704ff4:	54000169 	b.ls	48705020 <ddr_vref_find_best+0xa4>  // b.plast
    48704ff8:	110006d6 	add	w22, w22, #0x1
    48704ffc:	71000edf 	cmp	w22, #0x3
    48705000:	54000161 	b.ne	4870502c <ddr_vref_find_best+0xb0>  // b.any
    48705004:	2a1303e0 	mov	w0, w19
    48705008:	a94153f3 	ldp	x19, x20, [sp, #16]
    4870500c:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48705010:	a94363f7 	ldp	x23, x24, [sp, #48]
    48705014:	a9446bf9 	ldp	x25, x26, [sp, #64]
    48705018:	a8c57bfd 	ldp	x29, x30, [sp], #80
    4870501c:	d65f03c0 	ret
    48705020:	2a0003f8 	mov	w24, w0
    48705024:	2a1503f3 	mov	w19, w21
    48705028:	52800016 	mov	w22, #0x0                   	// #0
    4870502c:	0b1902b5 	add	w21, w21, w25
    48705030:	17ffffea 	b	48704fd8 <ddr_vref_find_best+0x5c>

0000000048705034 <ddr_vref_cal>:
    48705034:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
    48705038:	910003fd 	mov	x29, sp
    4870503c:	a90153f3 	stp	x19, x20, [sp, #16]
    48705040:	aa0003f4 	mov	x20, x0
    48705044:	a9025bf5 	stp	x21, x22, [sp, #32]
    48705048:	aa0103f5 	mov	x21, x1
    4870504c:	b9408c00 	ldr	w0, [x0, #140]
    48705050:	b9407e82 	ldr	w2, [x20, #124]
    48705054:	7100041f 	cmp	w0, #0x1
    48705058:	b9409280 	ldr	w0, [x20, #144]
    4870505c:	54000461 	b.ne	487050e8 <ddr_vref_cal+0xb4>  // b.any
    48705060:	b9409e81 	ldr	w1, [x20, #156]
    48705064:	35000361 	cbnz	w1, 487050d0 <ddr_vref_cal+0x9c>
    48705068:	53196000 	lsl	w0, w0, #7
    4870506c:	1109d000 	add	w0, w0, #0x274
    48705070:	b8626813 	ldr	w19, [x0, x2]
    48705074:	12001a73 	and	w19, w19, #0x7f
    48705078:	51000e62 	sub	w2, w19, #0x3
    4870507c:	aa1503e1 	mov	x1, x21
    48705080:	aa1403e0 	mov	x0, x20
    48705084:	97ffffa4 	bl	48704f14 <ddr_vref_get_win>
    48705088:	2a0003f6 	mov	w22, w0
    4870508c:	11000e62 	add	w2, w19, #0x3
    48705090:	aa1503e1 	mov	x1, x21
    48705094:	aa1403e0 	mov	x0, x20
    48705098:	97ffff9f 	bl	48704f14 <ddr_vref_get_win>
    4870509c:	6b0002df 	cmp	w22, w0
    487050a0:	540002e2 	b.cs	487050fc <ddr_vref_cal+0xc8>  // b.hs, b.nlast
    487050a4:	52800023 	mov	w3, #0x1                   	// #1
    487050a8:	2a1303e2 	mov	w2, w19
    487050ac:	aa1503e1 	mov	x1, x21
    487050b0:	aa1403e0 	mov	x0, x20
    487050b4:	97ffffb2 	bl	48704f7c <ddr_vref_find_best>
    487050b8:	2a0003e1 	mov	w1, w0
    487050bc:	a9425bf5 	ldp	x21, x22, [sp, #32]
    487050c0:	aa1403e0 	mov	x0, x20
    487050c4:	a94153f3 	ldp	x19, x20, [sp, #16]
    487050c8:	a8c37bfd 	ldp	x29, x30, [sp], #48
    487050cc:	17fffb6c 	b	48703e7c <ddr_vref_set>
    487050d0:	53165421 	lsl	w1, w1, #10
    487050d4:	0b001c20 	add	w0, w1, w0, lsl #7
    487050d8:	1109d000 	add	w0, w0, #0x274
    487050dc:	b8626813 	ldr	w19, [x0, x2]
    487050e0:	d3556273 	ubfx	x19, x19, #21, #4
    487050e4:	17ffffe5 	b	48705078 <ddr_vref_cal+0x44>
    487050e8:	53196000 	lsl	w0, w0, #7
    487050ec:	1109c000 	add	w0, w0, #0x270
    487050f0:	b8626813 	ldr	w19, [x0, x2]
    487050f4:	12001673 	and	w19, w19, #0x3f
    487050f8:	17ffffe0 	b	48705078 <ddr_vref_cal+0x44>
    487050fc:	54000069 	b.ls	48705108 <ddr_vref_cal+0xd4>  // b.plast
    48705100:	12800003 	mov	w3, #0xffffffff            	// #-1
    48705104:	17ffffe9 	b	487050a8 <ddr_vref_cal+0x74>
    48705108:	b9408e81 	ldr	w1, [x20, #140]
    4870510c:	52800640 	mov	w0, #0x32                  	// #50
    48705110:	7100043f 	cmp	w1, #0x1
    48705114:	540000c1 	b.ne	4870512c <ddr_vref_cal+0xf8>  // b.any
    48705118:	b9409e80 	ldr	w0, [x20, #156]
    4870511c:	528001e1 	mov	w1, #0xf                   	// #15
    48705120:	7100001f 	cmp	w0, #0x0
    48705124:	52800fe0 	mov	w0, #0x7f                  	// #127
    48705128:	1a810000 	csel	w0, w0, w1, eq  // eq = none
    4870512c:	6b40067f 	cmp	w19, w0, lsr #1
    48705130:	54fffe82 	b.cs	48705100 <ddr_vref_cal+0xcc>  // b.hs, b.nlast
    48705134:	17ffffdc 	b	487050a4 <ddr_vref_cal+0x70>

0000000048705138 <ddr_adjust_move_win.constprop.11>:
    48705138:	a9ba7bfd 	stp	x29, x30, [sp, #-96]!
    4870513c:	910003fd 	mov	x29, sp
    48705140:	a9025bf5 	stp	x21, x22, [sp, #32]
    48705144:	2a0203f6 	mov	w22, w2
    48705148:	a90363f7 	stp	x23, x24, [sp, #48]
    4870514c:	aa0103f8 	mov	x24, x1
    48705150:	a90573fb 	stp	x27, x28, [sp, #80]
    48705154:	aa0003fc 	mov	x28, x0
    48705158:	a90153f3 	stp	x19, x20, [sp, #16]
    4870515c:	a9046bf9 	stp	x25, x26, [sp, #64]
    48705160:	29518001 	ldp	w1, w0, [x0, #140]
    48705164:	b9407f82 	ldr	w2, [x28, #124]
    48705168:	7100083f 	cmp	w1, #0x2
    4870516c:	54000580 	b.eq	4870521c <ddr_adjust_move_win.constprop.11+0xe4>  // b.none
    48705170:	7100043f 	cmp	w1, #0x1
    48705174:	52803ff9 	mov	w25, #0x1ff                 	// #511
    48705178:	54000541 	b.ne	48705220 <ddr_adjust_move_win.constprop.11+0xe8>  // b.any
    4870517c:	53196000 	lsl	w0, w0, #7
    48705180:	1108b000 	add	w0, w0, #0x22c
    48705184:	b8626814 	ldr	w20, [x0, x2]
    48705188:	d3412694 	ubfx	x20, x20, #1, #9
    4870518c:	2a1403f3 	mov	w19, w20
    48705190:	52800015 	mov	w21, #0x0                   	// #0
    48705194:	52800037 	mov	w23, #0x1                   	// #1
    48705198:	5280081b 	mov	w27, #0x40                  	// #64
    4870519c:	5280009a 	mov	w26, #0x4                   	// #4
    487051a0:	6b15033f 	cmp	w25, w21
    487051a4:	540002e3 	b.cc	48705200 <ddr_adjust_move_win.constprop.11+0xc8>  // b.lo, b.ul, b.last
    487051a8:	b9408f84 	ldr	w4, [x28, #140]
    487051ac:	aa1c03e0 	mov	x0, x28
    487051b0:	7100089f 	cmp	w4, #0x2
    487051b4:	54000441 	b.ne	4870523c <ddr_adjust_move_win.constprop.11+0x104>  // b.any
    487051b8:	97fffa14 	bl	48703a08 <ddr_adjust_get_average>
    487051bc:	7100dc1f 	cmp	w0, #0x37
    487051c0:	54000069 	b.ls	487051cc <ddr_adjust_move_win.constprop.11+0x94>  // b.plast
    487051c4:	7101201f 	cmp	w0, #0x48
    487051c8:	1a978344 	csel	w4, w26, w23, hi  // hi = pmore
    487051cc:	52800021 	mov	w1, #0x1                   	// #1
    487051d0:	7100049f 	cmp	w4, #0x1
    487051d4:	7a561084 	ccmp	w4, w22, #0x4, ne  // ne = any
    487051d8:	54000140 	b.eq	48705200 <ddr_adjust_move_win.constprop.11+0xc8>  // b.none
    487051dc:	b9408f80 	ldr	w0, [x28, #140]
    487051e0:	7100041f 	cmp	w0, #0x1
    487051e4:	54000461 	b.ne	48705270 <ddr_adjust_move_win.constprop.11+0x138>  // b.any
    487051e8:	0b010260 	add	w0, w19, w1
    487051ec:	710012df 	cmp	w22, #0x4
    487051f0:	4b010273 	sub	w19, w19, w1
    487051f4:	1a801273 	csel	w19, w19, w0, ne  // ne = any
    487051f8:	7107fe7f 	cmp	w19, #0x1ff
    487051fc:	540004a9 	b.ls	48705290 <ddr_adjust_move_win.constprop.11+0x158>  // b.plast
    48705200:	a94153f3 	ldp	x19, x20, [sp, #16]
    48705204:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48705208:	a94363f7 	ldp	x23, x24, [sp, #48]
    4870520c:	a9446bf9 	ldp	x25, x26, [sp, #64]
    48705210:	a94573fb 	ldp	x27, x28, [sp, #80]
    48705214:	a8c67bfd 	ldp	x29, x30, [sp], #96
    48705218:	d65f03c0 	ret
    4870521c:	52800ff9 	mov	w25, #0x7f                  	// #127
    48705220:	b9409f81 	ldr	w1, [x28, #156]
    48705224:	53196000 	lsl	w0, w0, #7
    48705228:	0b012800 	add	w0, w0, w1, lsl #10
    4870522c:	1108d000 	add	w0, w0, #0x234
    48705230:	b8626814 	ldr	w20, [x0, x2]
    48705234:	d3493e94 	ubfx	x20, x20, #9, #7
    48705238:	17ffffd5 	b	4870518c <ddr_adjust_move_win.constprop.11+0x54>
    4870523c:	97fff9f3 	bl	48703a08 <ddr_adjust_get_average>
    48705240:	7101001f 	cmp	w0, #0x40
    48705244:	540000a9 	b.ls	48705258 <ddr_adjust_move_win.constprop.11+0x120>  // b.plast
    48705248:	7101201f 	cmp	w0, #0x48
    4870524c:	51010001 	sub	w1, w0, #0x40
    48705250:	1a978344 	csel	w4, w26, w23, hi  // hi = pmore
    48705254:	17ffffdf 	b	487051d0 <ddr_adjust_move_win.constprop.11+0x98>
    48705258:	4b000361 	sub	w1, w27, w0
    4870525c:	1a971021 	csel	w1, w1, w23, ne  // ne = any
    48705260:	7100dc1f 	cmp	w0, #0x37
    48705264:	1a9f87e4 	cset	w4, ls  // ls = plast
    48705268:	11000484 	add	w4, w4, #0x1
    4870526c:	17ffffd9 	b	487051d0 <ddr_adjust_move_win.constprop.11+0x98>
    48705270:	4b010260 	sub	w0, w19, w1
    48705274:	710012df 	cmp	w22, #0x4
    48705278:	0b010273 	add	w19, w19, w1
    4870527c:	1a801273 	csel	w19, w19, w0, ne  // ne = any
    48705280:	7101fe7f 	cmp	w19, #0x7f
    48705284:	17ffffde 	b	487051fc <ddr_adjust_move_win.constprop.11+0xc4>
    48705288:	110006b5 	add	w21, w21, #0x1
    4870528c:	17ffffc5 	b	487051a0 <ddr_adjust_move_win.constprop.11+0x68>
    48705290:	2a1303e1 	mov	w1, w19
    48705294:	aa1c03e0 	mov	x0, x28
    48705298:	97fffcef 	bl	48704654 <ddr_adjust_set_val>
    4870529c:	aa1803e1 	mov	x1, x24
    487052a0:	aa1c03e0 	mov	x0, x28
    487052a4:	97fffe81 	bl	48704ca8 <ddr_dataeye_deskew>
    487052a8:	34ffff00 	cbz	w0, 48705288 <ddr_adjust_move_win.constprop.11+0x150>
    487052ac:	2a1403e1 	mov	w1, w20
    487052b0:	aa1c03e0 	mov	x0, x28
    487052b4:	97fffce8 	bl	48704654 <ddr_adjust_set_val>
    487052b8:	aa1803e1 	mov	x1, x24
    487052bc:	aa1c03e0 	mov	x0, x28
    487052c0:	a94153f3 	ldp	x19, x20, [sp, #16]
    487052c4:	a9425bf5 	ldp	x21, x22, [sp, #32]
    487052c8:	a94363f7 	ldp	x23, x24, [sp, #48]
    487052cc:	a9446bf9 	ldp	x25, x26, [sp, #64]
    487052d0:	a94573fb 	ldp	x27, x28, [sp, #80]
    487052d4:	a8c67bfd 	ldp	x29, x30, [sp], #96
    487052d8:	17fffe74 	b	48704ca8 <ddr_dataeye_deskew>

00000000487052dc <ddr_adjust_dataeye>:
    487052dc:	b9408402 	ldr	w2, [x0, #132]
    487052e0:	36e00362 	tbz	w2, #28, 4870534c <ddr_adjust_dataeye+0x70>
    487052e4:	d65f03c0 	ret
    487052e8:	0b0106a1 	add	w1, w21, w1, lsl #1
    487052ec:	aa1303e0 	mov	x0, x19
    487052f0:	b9009261 	str	w1, [x19, #144]
    487052f4:	97fff9c5 	bl	48703a08 <ddr_adjust_get_average>
    487052f8:	7100dc1f 	cmp	w0, #0x37
    487052fc:	52800082 	mov	w2, #0x4                   	// #4
    48705300:	54000409 	b.ls	48705380 <ddr_adjust_dataeye+0xa4>  // b.plast
    48705304:	7101201f 	cmp	w0, #0x48
    48705308:	52800042 	mov	w2, #0x2                   	// #2
    4870530c:	540003a8 	b.hi	48705380 <ddr_adjust_dataeye+0xa4>  // b.pmore
    48705310:	110006b5 	add	w21, w21, #0x1
    48705314:	b940a261 	ldr	w1, [x19, #160]
    48705318:	b9409a62 	ldr	w2, [x19, #152]
    4870531c:	2a0103e0 	mov	w0, w1
    48705320:	9b177c42 	mul	x2, x2, x23
    48705324:	9b160800 	madd	x0, x0, x22, x2
    48705328:	8b000260 	add	x0, x19, x0
    4870532c:	b9401800 	ldr	w0, [x0, #24]
    48705330:	6b0002bf 	cmp	w21, w0
    48705334:	54fffda3 	b.cc	487052e8 <ddr_adjust_dataeye+0xc>  // b.lo, b.ul, b.last
    48705338:	a94153f3 	ldp	x19, x20, [sp, #16]
    4870533c:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48705340:	f9401bf7 	ldr	x23, [sp, #48]
    48705344:	a8c47bfd 	ldp	x29, x30, [sp], #64
    48705348:	d65f03c0 	ret
    4870534c:	b940a402 	ldr	w2, [x0, #164]
    48705350:	34000202 	cbz	w2, 48705390 <ddr_adjust_dataeye+0xb4>
    48705354:	a9bc7bfd 	stp	x29, x30, [sp, #-64]!
    48705358:	910003fd 	mov	x29, sp
    4870535c:	a90153f3 	stp	x19, x20, [sp, #16]
    48705360:	aa0103f4 	mov	x20, x1
    48705364:	a9025bf5 	stp	x21, x22, [sp, #32]
    48705368:	aa0003f3 	mov	x19, x0
    4870536c:	f9001bf7 	str	x23, [sp, #48]
    48705370:	52800015 	mov	w21, #0x0                   	// #0
    48705374:	d2800196 	mov	x22, #0xc                   	// #12
    48705378:	d2800797 	mov	x23, #0x3c                  	// #60
    4870537c:	17ffffe6 	b	48705314 <ddr_adjust_dataeye+0x38>
    48705380:	aa1403e1 	mov	x1, x20
    48705384:	aa1303e0 	mov	x0, x19
    48705388:	97ffff6c 	bl	48705138 <ddr_adjust_move_win.constprop.11>
    4870538c:	17ffffe1 	b	48705310 <ddr_adjust_dataeye+0x34>
    48705390:	d65f03c0 	ret

0000000048705394 <ddr_dataeye_process>:
    48705394:	a9bc7bfd 	stp	x29, x30, [sp, #-64]!
    48705398:	910003fd 	mov	x29, sp
    4870539c:	a90153f3 	stp	x19, x20, [sp, #16]
    487053a0:	aa0003f3 	mov	x19, x0
    487053a4:	a9025bf5 	stp	x21, x22, [sp, #32]
    487053a8:	aa0103f6 	mov	x22, x1
    487053ac:	a90363f7 	stp	x23, x24, [sp, #48]
    487053b0:	52800015 	mov	w21, #0x0                   	// #0
    487053b4:	52800014 	mov	w20, #0x0                   	// #0
    487053b8:	d2800197 	mov	x23, #0xc                   	// #12
    487053bc:	d2800798 	mov	x24, #0x3c                  	// #60
    487053c0:	b940a261 	ldr	w1, [x19, #160]
    487053c4:	b9409a62 	ldr	w2, [x19, #152]
    487053c8:	2a0103e0 	mov	w0, w1
    487053cc:	9b187c42 	mul	x2, x2, x24
    487053d0:	9b170800 	madd	x0, x0, x23, x2
    487053d4:	8b000260 	add	x0, x19, x0
    487053d8:	b9401800 	ldr	w0, [x0, #24]
    487053dc:	6b15001f 	cmp	w0, w21
    487053e0:	540001c8 	b.hi	48705418 <ddr_dataeye_process+0x84>  // b.pmore
    487053e4:	350002b4 	cbnz	w20, 48705438 <ddr_dataeye_process+0xa4>
    487053e8:	aa1603e1 	mov	x1, x22
    487053ec:	aa1303e0 	mov	x0, x19
    487053f0:	97ffffbb 	bl	487052dc <ddr_adjust_dataeye>
    487053f4:	aa1603e1 	mov	x1, x22
    487053f8:	aa1303e0 	mov	x0, x19
    487053fc:	9400055e 	bl	48706974 <ddr_result_data_save>
    48705400:	2a1403e0 	mov	w0, w20
    48705404:	a94153f3 	ldp	x19, x20, [sp, #16]
    48705408:	a9425bf5 	ldp	x21, x22, [sp, #32]
    4870540c:	a94363f7 	ldp	x23, x24, [sp, #48]
    48705410:	a8c47bfd 	ldp	x29, x30, [sp], #64
    48705414:	d65f03c0 	ret
    48705418:	0b0106a1 	add	w1, w21, w1, lsl #1
    4870541c:	aa1303e0 	mov	x0, x19
    48705420:	b9009261 	str	w1, [x19, #144]
    48705424:	aa1603e1 	mov	x1, x22
    48705428:	110006b5 	add	w21, w21, #0x1
    4870542c:	97fffe1f 	bl	48704ca8 <ddr_dataeye_deskew>
    48705430:	0b000294 	add	w20, w20, w0
    48705434:	17ffffe3 	b	487053c0 <ddr_dataeye_process+0x2c>
    48705438:	12800014 	mov	w20, #0xffffffff            	// #-1
    4870543c:	17ffffee 	b	487053f4 <ddr_dataeye_process+0x60>

0000000048705440 <ddr_dataeye_training>:
    48705440:	a9ad7bfd 	stp	x29, x30, [sp, #-304]!
    48705444:	52802082 	mov	w2, #0x104                 	// #260
    48705448:	52800001 	mov	w1, #0x0                   	// #0
    4870544c:	910003fd 	mov	x29, sp
    48705450:	a90153f3 	stp	x19, x20, [sp, #16]
    48705454:	aa0003f3 	mov	x19, x0
    48705458:	52800040 	mov	w0, #0x2                   	// #2
    4870545c:	b9008e60 	str	w0, [x19, #140]
    48705460:	9100a3a0 	add	x0, x29, #0x28
    48705464:	97fff9a2 	bl	48703aec <ddrtr_memset>
    48705468:	9100a3a1 	add	x1, x29, #0x28
    4870546c:	aa1303e0 	mov	x0, x19
    48705470:	97ffffc9 	bl	48705394 <ddr_dataeye_process>
    48705474:	2a0003f4 	mov	w20, w0
    48705478:	52800020 	mov	w0, #0x1                   	// #1
    4870547c:	52802082 	mov	w2, #0x104                 	// #260
    48705480:	52800001 	mov	w1, #0x0                   	// #0
    48705484:	b9008e60 	str	w0, [x19, #140]
    48705488:	9100a3a0 	add	x0, x29, #0x28
    4870548c:	97fff998 	bl	48703aec <ddrtr_memset>
    48705490:	9100a3a1 	add	x1, x29, #0x28
    48705494:	aa1303e0 	mov	x0, x19
    48705498:	97ffffbf 	bl	48705394 <ddr_dataeye_process>
    4870549c:	2a000280 	orr	w0, w20, w0
    487054a0:	7100001f 	cmp	w0, #0x0
    487054a4:	a94153f3 	ldp	x19, x20, [sp, #16]
    487054a8:	5a9f03e0 	csetm	w0, ne  // ne = any
    487054ac:	a8d37bfd 	ldp	x29, x30, [sp], #304
    487054b0:	d65f03c0 	ret

00000000487054b4 <ddr_dataeye_training_func>:
    487054b4:	b9408401 	ldr	w1, [x0, #132]
    487054b8:	36800061 	tbz	w1, #16, 487054c4 <ddr_dataeye_training_func+0x10>
    487054bc:	52800000 	mov	w0, #0x0                   	// #0
    487054c0:	d65f03c0 	ret
    487054c4:	a9bb7bfd 	stp	x29, x30, [sp, #-80]!
    487054c8:	52a00022 	mov	w2, #0x10000               	// #65536
    487054cc:	910003fd 	mov	x29, sp
    487054d0:	9100a3a1 	add	x1, x29, #0x28
    487054d4:	f9000bf3 	str	x19, [sp, #16]
    487054d8:	aa0003f3 	mov	x19, x0
    487054dc:	97fffba9 	bl	48704380 <ddr_training_save_reg>
    487054e0:	aa1303e0 	mov	x0, x19
    487054e4:	97fffb7b 	bl	487042d0 <ddr_training_switch_axi>
    487054e8:	aa1303e0 	mov	x0, x19
    487054ec:	52800041 	mov	w1, #0x2                   	// #2
    487054f0:	97fffce7 	bl	4870488c <ddr_ddrt_init>
    487054f4:	52800020 	mov	w0, #0x1                   	// #1
    487054f8:	29148260 	stp	w0, w0, [x19, #164]
    487054fc:	aa1303e0 	mov	x0, x19
    48705500:	97ffffd0 	bl	48705440 <ddr_dataeye_training>
    48705504:	2a0003e6 	mov	w6, w0
    48705508:	9100a3a1 	add	x1, x29, #0x28
    4870550c:	aa1303e0 	mov	x0, x19
    48705510:	97fffbd8 	bl	48704470 <ddr_training_restore_reg>
    48705514:	f9400bf3 	ldr	x19, [sp, #16]
    48705518:	2a0603e0 	mov	w0, w6
    4870551c:	a8c57bfd 	ldp	x29, x30, [sp], #80
    48705520:	d65f03c0 	ret

0000000048705524 <ddr_hw_dataeye_read>:
    48705524:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48705528:	d2800782 	mov	x2, #0x3c                  	// #60
    4870552c:	aa0003e5 	mov	x5, x0
    48705530:	910003fd 	mov	x29, sp
    48705534:	b9409801 	ldr	w1, [x0, #152]
    48705538:	b9407c06 	ldr	w6, [x0, #124]
    4870553c:	d503201f 	nop
    48705540:	9b020021 	madd	x1, x1, x2, x0
    48705544:	b9401027 	ldr	w7, [x1, #16]
    48705548:	97fffcc4 	bl	48704858 <ddr_training_cfg_init>
    4870554c:	52800003 	mov	w3, #0x0                   	// #0
    48705550:	6b07007f 	cmp	w3, w7
    48705554:	540000e1 	b.ne	48705570 <ddr_hw_dataeye_read+0x4c>  // b.any
    48705558:	2a0603e0 	mov	w0, w6
    4870555c:	97fffb7c 	bl	4870434c <ddr_phy_cfg_update>
    48705560:	aa0503e0 	mov	x0, x5
    48705564:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48705568:	52802001 	mov	w1, #0x100                 	// #256
    4870556c:	17fffb23 	b	487041f8 <ddr_hw_training_process>
    48705570:	b9409ca1 	ldr	w1, [x5, #156]
    48705574:	53196060 	lsl	w0, w3, #7
    48705578:	2a0603e2 	mov	w2, w6
    4870557c:	11000463 	add	w3, w3, #0x1
    48705580:	0b012801 	add	w1, w0, w1, lsl #10
    48705584:	11087021 	add	w1, w1, #0x21c
    48705588:	b822683f 	str	wzr, [x1, x2]
    4870558c:	b9409ca1 	ldr	w1, [x5, #156]
    48705590:	0b012801 	add	w1, w0, w1, lsl #10
    48705594:	1108b000 	add	w0, w0, #0x22c
    48705598:	11088021 	add	w1, w1, #0x220
    4870559c:	93407c00 	sxtw	x0, w0
    487055a0:	b822683f 	str	wzr, [x1, x2]
    487055a4:	b822681f 	str	wzr, [x0, x2]
    487055a8:	17ffffea 	b	48705550 <ddr_hw_dataeye_read+0x2c>

00000000487055ac <ddr_hw_training_ctl>:
    487055ac:	a9b97bfd 	stp	x29, x30, [sp, #-112]!
    487055b0:	910003fd 	mov	x29, sp
    487055b4:	a9025bf5 	stp	x21, x22, [sp, #32]
    487055b8:	a90153f3 	stp	x19, x20, [sp, #16]
    487055bc:	a90363f7 	stp	x23, x24, [sp, #48]
    487055c0:	a9046bf9 	stp	x25, x26, [sp, #64]
    487055c4:	a90573fb 	stp	x27, x28, [sp, #80]
    487055c8:	b9407c17 	ldr	w23, [x0, #124]
    487055cc:	b9408416 	ldr	w22, [x0, #132]
    487055d0:	f9405c19 	ldr	x25, [x0, #184]
    487055d4:	2a1703f5 	mov	w21, w23
    487055d8:	710002df 	cmp	w22, #0x0
    487055dc:	fa401b24 	ccmp	x25, #0x0, #0x4, ne  // ne = any
    487055e0:	b940c6b8 	ldr	w24, [x21, #196]
    487055e4:	54001be0 	b.eq	48705960 <ddr_hw_training_ctl+0x3b4>  // b.none
    487055e8:	aa0003f3 	mov	x19, x0
    487055ec:	b9409800 	ldr	w0, [x0, #152]
    487055f0:	d280079a 	mov	x26, #0x3c                  	// #60
    487055f4:	9b1a4c00 	madd	x0, x0, x26, x19
    487055f8:	b9401000 	ldr	w0, [x0, #16]
    487055fc:	b9006fa0 	str	w0, [x29, #108]
    48705600:	2a1703e0 	mov	w0, w23
    48705604:	97fffb52 	bl	4870434c <ddr_phy_cfg_update>
    48705608:	121102c1 	and	w1, w22, #0x8000
    4870560c:	aa1303e0 	mov	x0, x19
    48705610:	97fffafa 	bl	487041f8 <ddr_hw_training_process>
    48705614:	2a0003f4 	mov	w20, w0
    48705618:	121f0ac1 	and	w1, w22, #0xe
    4870561c:	aa1303e0 	mov	x0, x19
    48705620:	97fffaf6 	bl	487041f8 <ddr_hw_training_process>
    48705624:	0b000294 	add	w20, w20, w0
    48705628:	b9409e60 	ldr	w0, [x19, #156]
    4870562c:	34000d40 	cbz	w0, 487057d4 <ddr_hw_training_ctl+0x228>
    48705630:	5280000f 	mov	w15, #0x0                   	// #0
    48705634:	b9406fa0 	ldr	w0, [x29, #108]
    48705638:	6b0001ff 	cmp	w15, w0
    4870563c:	54000ec1 	b.ne	48705814 <ddr_hw_training_ctl+0x268>  // b.any
    48705640:	b9409a60 	ldr	w0, [x19, #152]
    48705644:	d2800781 	mov	x1, #0x3c                  	// #60
    48705648:	9b014c00 	madd	x0, x0, x1, x19
    4870564c:	b9400400 	ldr	w0, [x0, #4]
    48705650:	7100181f 	cmp	w0, #0x6
    48705654:	54001721 	b.ne	48705938 <ddr_hw_training_ctl+0x38c>  // b.any
    48705658:	b94066b9 	ldr	w25, [x21, #100]
    4870565c:	52828001 	mov	w1, #0x1400                	// #5120
    48705660:	0a0102c1 	and	w1, w22, w1
    48705664:	120b02da 	and	w26, w22, #0x200000
    48705668:	12006f20 	and	w0, w25, #0xfffffff
    4870566c:	b90066a0 	str	w0, [x21, #100]
    48705670:	aa1303e0 	mov	x0, x19
    48705674:	97fffae1 	bl	487041f8 <ddr_hw_training_process>
    48705678:	b90066b9 	str	w25, [x21, #100]
    4870567c:	0b000294 	add	w20, w20, w0
    48705680:	2a1a03e1 	mov	w1, w26
    48705684:	b9404ab9 	ldr	w25, [x21, #72]
    48705688:	121f7b20 	and	w0, w25, #0xfffffffe
    4870568c:	b9004aa0 	str	w0, [x21, #72]
    48705690:	aa1303e0 	mov	x0, x19
    48705694:	97fffad9 	bl	487041f8 <ddr_hw_training_process>
    48705698:	0b000294 	add	w20, w20, w0
    4870569c:	32000320 	orr	w0, w25, #0x1
    487056a0:	2a1a03e1 	mov	w1, w26
    487056a4:	b9004aa0 	str	w0, [x21, #72]
    487056a8:	aa1303e0 	mov	x0, x19
    487056ac:	97fffad3 	bl	487041f8 <ddr_hw_training_process>
    487056b0:	0b000294 	add	w20, w20, w0
    487056b4:	121602c1 	and	w1, w22, #0x400
    487056b8:	b9004ab9 	str	w25, [x21, #72]
    487056bc:	aa1303e0 	mov	x0, x19
    487056c0:	120e02da 	and	w26, w22, #0x40000
    487056c4:	97fffacd 	bl	487041f8 <ddr_hw_training_process>
    487056c8:	0b000294 	add	w20, w20, w0
    487056cc:	121502c1 	and	w1, w22, #0x800
    487056d0:	aa1303e0 	mov	x0, x19
    487056d4:	97fffac9 	bl	487041f8 <ddr_hw_training_process>
    487056d8:	0b000294 	add	w20, w20, w0
    487056dc:	120a02c1 	and	w1, w22, #0x400000
    487056e0:	aa1303e0 	mov	x0, x19
    487056e4:	97fffac5 	bl	487041f8 <ddr_hw_training_process>
    487056e8:	0b000294 	add	w20, w20, w0
    487056ec:	12007b00 	and	w0, w24, #0x7fffffff
    487056f0:	2a1a03e1 	mov	w1, w26
    487056f4:	b900c6a0 	str	w0, [x21, #196]
    487056f8:	b940c2a0 	ldr	w0, [x21, #192]
    487056fc:	32010000 	orr	w0, w0, #0x80000000
    48705700:	b900c2a0 	str	w0, [x21, #192]
    48705704:	aa1303e0 	mov	x0, x19
    48705708:	97fffabc 	bl	487041f8 <ddr_hw_training_process>
    4870570c:	0b000294 	add	w20, w20, w0
    48705710:	2a1a03e1 	mov	w1, w26
    48705714:	aa1303e0 	mov	x0, x19
    48705718:	97fffab8 	bl	487041f8 <ddr_hw_training_process>
    4870571c:	0b000294 	add	w20, w20, w0
    48705720:	b940c2a0 	ldr	w0, [x21, #192]
    48705724:	2a1a03e1 	mov	w1, w26
    48705728:	12007800 	and	w0, w0, #0x7fffffff
    4870572c:	b900c2a0 	str	w0, [x21, #192]
    48705730:	aa1303e0 	mov	x0, x19
    48705734:	97fffab1 	bl	487041f8 <ddr_hw_training_process>
    48705738:	0b000294 	add	w20, w20, w0
    4870573c:	52807e01 	mov	w1, #0x3f0                 	// #1008
    48705740:	aa1303e0 	mov	x0, x19
    48705744:	72a40061 	movk	w1, #0x2003, lsl #16
    48705748:	b900c6b8 	str	w24, [x21, #196]
    4870574c:	0a0102c1 	and	w1, w22, w1
    48705750:	97fffaaa 	bl	487041f8 <ddr_hw_training_process>
    48705754:	0b000294 	add	w20, w20, w0
    48705758:	b9409a60 	ldr	w0, [x19, #152]
    4870575c:	d2800781 	mov	x1, #0x3c                  	// #60
    48705760:	9b014c00 	madd	x0, x0, x1, x19
    48705764:	b9400400 	ldr	w0, [x0, #4]
    48705768:	7100281f 	cmp	w0, #0xa
    4870576c:	540001e1 	b.ne	487057a8 <ddr_hw_training_ctl+0x1fc>  // b.any
    48705770:	b900e2b9 	str	w25, [x21, #224]
    48705774:	528aaaa0 	mov	w0, #0x5555                	// #21845
    48705778:	72a0aaa0 	movk	w0, #0x555, lsl #16
    4870577c:	121602c1 	and	w1, w22, #0x400
    48705780:	b941e2b9 	ldr	w25, [x21, #480]
    48705784:	b9402eb8 	ldr	w24, [x21, #44]
    48705788:	b901e2a0 	str	w0, [x21, #480]
    4870578c:	32120300 	orr	w0, w24, #0x4000
    48705790:	b9002ea0 	str	w0, [x21, #44]
    48705794:	aa1303e0 	mov	x0, x19
    48705798:	97fffa98 	bl	487041f8 <ddr_hw_training_process>
    4870579c:	b901e2b9 	str	w25, [x21, #480]
    487057a0:	0b000294 	add	w20, w20, w0
    487057a4:	b9002eb8 	str	w24, [x21, #44]
    487057a8:	2a1703e0 	mov	w0, w23
    487057ac:	97fffae8 	bl	4870434c <ddr_phy_cfg_update>
    487057b0:	2a1403e0 	mov	w0, w20
    487057b4:	910003bf 	mov	sp, x29
    487057b8:	a94153f3 	ldp	x19, x20, [sp, #16]
    487057bc:	a9425bf5 	ldp	x21, x22, [sp, #32]
    487057c0:	a94363f7 	ldp	x23, x24, [sp, #48]
    487057c4:	a9446bf9 	ldp	x25, x26, [sp, #64]
    487057c8:	a94573fb 	ldp	x27, x28, [sp, #80]
    487057cc:	a8c77bfd 	ldp	x29, x30, [sp], #112
    487057d0:	d65f03c0 	ret
    487057d4:	b9409a60 	ldr	w0, [x19, #152]
    487057d8:	b9407e63 	ldr	w3, [x19, #124]
    487057dc:	d503201f 	nop
    487057e0:	9b1a4c00 	madd	x0, x0, x26, x19
    487057e4:	b9401002 	ldr	w2, [x0, #16]
    487057e8:	d2800000 	mov	x0, #0x0                   	// #0
    487057ec:	eb00005f 	cmp	x2, x0
    487057f0:	54fff200 	b.eq	48705630 <ddr_hw_training_ctl+0x84>  // b.none
    487057f4:	53196001 	lsl	w1, w0, #7
    487057f8:	2a0303e4 	mov	w4, w3
    487057fc:	1108b021 	add	w1, w1, #0x22c
    48705800:	93407c21 	sxtw	x1, w1
    48705804:	b8646821 	ldr	w1, [x1, x4]
    48705808:	b8207b21 	str	w1, [x25, x0, lsl #2]
    4870580c:	91000400 	add	x0, x0, #0x1
    48705810:	17fffff7 	b	487057ec <ddr_hw_training_ctl+0x240>
    48705814:	b9409e63 	ldr	w3, [x19, #156]
    48705818:	531961e5 	lsl	w5, w15, #7
    4870581c:	b9407e64 	ldr	w4, [x19, #124]
    48705820:	910003e0 	mov	x0, sp
    48705824:	b900926f 	str	w15, [x19, #144]
    48705828:	d100c3ff 	sub	sp, sp, #0x30
    4870582c:	0b0328a3 	add	w3, w5, w3, lsl #10
    48705830:	1108b0a5 	add	w5, w5, #0x22c
    48705834:	11087071 	add	w17, w3, #0x21c
    48705838:	11088070 	add	w16, w3, #0x220
    4870583c:	11089063 	add	w3, w3, #0x224
    48705840:	f90033a0 	str	x0, [x29, #96]
    48705844:	910003e2 	mov	x2, sp
    48705848:	12800001 	mov	w1, #0xffffffff            	// #-1
    4870584c:	b8646a3a 	ldr	w26, [x17, x4]
    48705850:	b8646a1c 	ldr	w28, [x16, x4]
    48705854:	b8646867 	ldr	w7, [x3, x4]
    48705858:	d3411f4c 	ubfx	x12, x26, #1, #7
    4870585c:	b86468a6 	ldr	w6, [x5, x4]
    48705860:	d3493f4d 	ubfx	x13, x26, #9, #7
    48705864:	53197f4e 	lsr	w14, w26, #25
    48705868:	d3515f5b 	ubfx	x27, x26, #17, #7
    4870586c:	53197f80 	lsr	w0, w28, #25
    48705870:	d3411f88 	ubfx	x8, x28, #1, #7
    48705874:	d3493f8b 	ubfx	x11, x28, #9, #7
    48705878:	d3515f8a 	ubfx	x10, x28, #17, #7
    4870587c:	d3411cf9 	ubfx	x25, x7, #1, #7
    48705880:	d34124de 	ubfx	x30, x6, #1, #9
    48705884:	d280001a 	mov	x26, #0x0                   	// #0
    48705888:	290037ec 	stp	w12, w13, [sp]
    4870588c:	29013bfb 	stp	w27, w14, [sp, #8]
    48705890:	29022fe8 	stp	w8, w11, [sp, #16]
    48705894:	b9001bea 	str	w10, [sp, #24]
    48705898:	29047bf9 	stp	w25, w30, [sp, #32]
    4870589c:	b9001fe0 	str	w0, [sp, #28]
    487058a0:	b87a685c 	ldr	w28, [x2, x26]
    487058a4:	9100135a 	add	x26, x26, #0x4
    487058a8:	6b1c003f 	cmp	w1, w28
    487058ac:	1a9c9021 	csel	w1, w1, w28, ls  // ls = plast
    487058b0:	f100a35f 	cmp	x26, #0x28
    487058b4:	54ffff61 	b.ne	487058a0 <ddr_hw_training_ctl+0x2f4>  // b.any
    487058b8:	4b010000 	sub	w0, w0, w1
    487058bc:	4b01014a 	sub	w10, w10, w1
    487058c0:	4b0101ce 	sub	w14, w14, w1
    487058c4:	4b0101ad 	sub	w13, w13, w1
    487058c8:	4b01037b 	sub	w27, w27, w1
    487058cc:	4b01018c 	sub	w12, w12, w1
    487058d0:	53071800 	lsl	w0, w0, #25
    487058d4:	4b010108 	sub	w8, w8, w1
    487058d8:	530719ce 	lsl	w14, w14, #25
    487058dc:	531759ad 	lsl	w13, w13, #9
    487058e0:	2a0a440a 	orr	w10, w0, w10, lsl #17
    487058e4:	4b010160 	sub	w0, w11, w1
    487058e8:	2a1b45ce 	orr	w14, w14, w27, lsl #17
    487058ec:	2a0c05ac 	orr	w12, w13, w12, lsl #1
    487058f0:	2a0c01da 	orr	w26, w14, w12
    487058f4:	121860e7 	and	w7, w7, #0xffffff01
    487058f8:	53175800 	lsl	w0, w0, #9
    487058fc:	4b010339 	sub	w25, w25, w1
    48705900:	b8246a3a 	str	w26, [x17, x4]
    48705904:	121658c6 	and	w6, w6, #0xfffffc01
    48705908:	2a080400 	orr	w0, w0, w8, lsl #1
    4870590c:	4b0103de 	sub	w30, w30, w1
    48705910:	2a00015c 	orr	w28, w10, w0
    48705914:	2a1904e7 	orr	w7, w7, w25, lsl #1
    48705918:	b8246a1c 	str	w28, [x16, x4]
    4870591c:	2a1e04c6 	orr	w6, w6, w30, lsl #1
    48705920:	b8246867 	str	w7, [x3, x4]
    48705924:	110005ef 	add	w15, w15, #0x1
    48705928:	b82468a6 	str	w6, [x5, x4]
    4870592c:	f94033a0 	ldr	x0, [x29, #96]
    48705930:	9100001f 	mov	sp, x0
    48705934:	17ffff40 	b	48705634 <ddr_hw_training_ctl+0x88>
    48705938:	7100281f 	cmp	w0, #0xa
    4870593c:	52800019 	mov	w25, #0x0                   	// #0
    48705940:	540000a1 	b.ne	48705954 <ddr_hw_training_ctl+0x3a8>  // b.any
    48705944:	b940e2b9 	ldr	w25, [x21, #224]
    48705948:	12047720 	and	w0, w25, #0xf3ffffff
    4870594c:	32050000 	orr	w0, w0, #0x8000000
    48705950:	b900e2a0 	str	w0, [x21, #224]
    48705954:	52828001 	mov	w1, #0x1400                	// #5120
    48705958:	0a0102c1 	and	w1, w22, w1
    4870595c:	17ffff58 	b	487056bc <ddr_hw_training_ctl+0x110>
    48705960:	52800000 	mov	w0, #0x0                   	// #0
    48705964:	17ffff94 	b	487057b4 <ddr_hw_training_ctl+0x208>

0000000048705968 <ddr_hw_training_by_phy>:
    48705968:	a9b67bfd 	stp	x29, x30, [sp, #-160]!
    4870596c:	910003fd 	mov	x29, sp
    48705970:	a90153f3 	stp	x19, x20, [sp, #16]
    48705974:	9101c3b4 	add	x20, x29, #0x70
    48705978:	a9025bf5 	stp	x21, x22, [sp, #32]
    4870597c:	aa0003f3 	mov	x19, x0
    48705980:	a90363f7 	stp	x23, x24, [sp, #48]
    48705984:	d2800798 	mov	x24, #0x3c                  	// #60
    48705988:	a9046bf9 	stp	x25, x26, [sp, #64]
    4870598c:	9101a3b7 	add	x23, x29, #0x68
    48705990:	f9002bfb 	str	x27, [sp, #80]
    48705994:	d2800015 	mov	x21, #0x0                   	// #0
    48705998:	52800016 	mov	w22, #0x0                   	// #0
    4870599c:	b9409801 	ldr	w1, [x0, #152]
    487059a0:	9b180021 	madd	x1, x1, x24, x0
    487059a4:	b9400c39 	ldr	w25, [x1, #12]
    487059a8:	aa1703e1 	mov	x1, x23
    487059ac:	f9005c14 	str	x20, [x0, #184]
    487059b0:	2a1903fa 	mov	w26, w25
    487059b4:	97fff916 	bl	48703e0c <ddr_training_save_timing>
    487059b8:	eb1a02bf 	cmp	x21, x26
    487059bc:	2a1503fb 	mov	w27, w21
    487059c0:	54000481 	b.ne	48705a50 <ddr_hw_training_by_phy+0xe8>  // b.any
    487059c4:	71000b3f 	cmp	w25, #0x2
    487059c8:	540001c1 	b.ne	48705a00 <ddr_hw_training_by_phy+0x98>  // b.any
    487059cc:	29533e60 	ldp	w0, w15, [x19, #152]
    487059d0:	d2800781 	mov	x1, #0x3c                  	// #60
    487059d4:	9100428c 	add	x12, x20, #0x10
    487059d8:	d280458e 	mov	x14, #0x22c                 	// #556
    487059dc:	5280000d 	mov	w13, #0x0                   	// #0
    487059e0:	52800035 	mov	w21, #0x1                   	// #1
    487059e4:	9b014c00 	madd	x0, x0, x1, x19
    487059e8:	b9401014 	ldr	w20, [x0, #16]
    487059ec:	6b0d029f 	cmp	w20, w13
    487059f0:	54000a61 	b.ne	48705b3c <ddr_hw_training_by_phy+0x1d4>  // b.any
    487059f4:	b9407e60 	ldr	w0, [x19, #124]
    487059f8:	b9009e6f 	str	w15, [x19, #156]
    487059fc:	97fffa54 	bl	4870434c <ddr_phy_cfg_update>
    48705a00:	b9409a60 	ldr	w0, [x19, #152]
    48705a04:	d2800281 	mov	x1, #0x14                  	// #20
    48705a08:	52800783 	mov	w3, #0x3c                  	// #60
    48705a0c:	d2800004 	mov	x4, #0x0                   	// #0
    48705a10:	9ba30403 	umaddl	x3, w0, w3, x1
    48705a14:	2a0003e0 	mov	w0, w0
    48705a18:	d2800781 	mov	x1, #0x3c                  	// #60
    48705a1c:	8b030263 	add	x3, x19, x3
    48705a20:	9b014c13 	madd	x19, x0, x1, x19
    48705a24:	b9400a60 	ldr	w0, [x19, #8]
    48705a28:	6b04001f 	cmp	w0, w4
    48705a2c:	54000b28 	b.hi	48705b90 <ddr_hw_training_by_phy+0x228>  // b.pmore
    48705a30:	2a1603e0 	mov	w0, w22
    48705a34:	f9402bfb 	ldr	x27, [sp, #80]
    48705a38:	a94153f3 	ldp	x19, x20, [sp, #16]
    48705a3c:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48705a40:	a94363f7 	ldp	x23, x24, [sp, #48]
    48705a44:	a9446bf9 	ldp	x25, x26, [sp, #64]
    48705a48:	a8ca7bfd 	ldp	x29, x30, [sp], #160
    48705a4c:	d65f03c0 	ret
    48705a50:	b9409a61 	ldr	w1, [x19, #152]
    48705a54:	93407ea0 	sxtw	x0, w21
    48705a58:	91001000 	add	x0, x0, #0x4
    48705a5c:	b9009e75 	str	w21, [x19, #156]
    48705a60:	9b187c21 	mul	x1, x1, x24
    48705a64:	8b000c20 	add	x0, x1, x0, lsl #3
    48705a68:	b9407e61 	ldr	w1, [x19, #124]
    48705a6c:	8b000260 	add	x0, x19, x0
    48705a70:	b9401000 	ldr	w0, [x0, #16]
    48705a74:	b9008660 	str	w0, [x19, #132]
    48705a78:	b9404820 	ldr	w0, [x1, #72]
    48705a7c:	121c6c00 	and	w0, w0, #0xfffffff0
    48705a80:	2a150000 	orr	w0, w0, w21
    48705a84:	b9004820 	str	w0, [x1, #72]
    48705a88:	b9407e62 	ldr	w2, [x19, #124]
    48705a8c:	b9409e60 	ldr	w0, [x19, #156]
    48705a90:	b940c841 	ldr	w1, [x2, #200]
    48705a94:	12007821 	and	w1, w1, #0x7fffffff
    48705a98:	2a007c20 	orr	w0, w1, w0, lsl #31
    48705a9c:	b900c840 	str	w0, [x2, #200]
    48705aa0:	aa1303e0 	mov	x0, x19
    48705aa4:	97fffec2 	bl	487055ac <ddr_hw_training_ctl>
    48705aa8:	0b0002d6 	add	w22, w22, w0
    48705aac:	71000b3f 	cmp	w25, #0x2
    48705ab0:	54fffa81 	b.ne	48705a00 <ddr_hw_training_by_phy+0x98>  // b.any
    48705ab4:	b9409a60 	ldr	w0, [x19, #152]
    48705ab8:	d37ceea3 	lsl	x3, x21, #4
    48705abc:	b9407e64 	ldr	w4, [x19, #124]
    48705ac0:	d503201f 	nop
    48705ac4:	9b184c00 	madd	x0, x0, x24, x19
    48705ac8:	b9401002 	ldr	w2, [x0, #16]
    48705acc:	d2800000 	mov	x0, #0x0                   	// #0
    48705ad0:	eb02001f 	cmp	x0, x2
    48705ad4:	540000c1 	b.ne	48705aec <ddr_hw_training_by_phy+0x184>  // b.any
    48705ad8:	7100077f 	cmp	w27, #0x1
    48705adc:	d2800001 	mov	x1, #0x0                   	// #0
    48705ae0:	54000281 	b.ne	48705b30 <ddr_hw_training_by_phy+0x1c8>  // b.any
    48705ae4:	910006b5 	add	x21, x21, #0x1
    48705ae8:	17ffffb4 	b	487059b8 <ddr_hw_training_by_phy+0x50>
    48705aec:	53196001 	lsl	w1, w0, #7
    48705af0:	2a0403e5 	mov	w5, w4
    48705af4:	1108b021 	add	w1, w1, #0x22c
    48705af8:	93407c21 	sxtw	x1, w1
    48705afc:	b8656825 	ldr	w5, [x1, x5]
    48705b00:	8b000861 	add	x1, x3, x0, lsl #2
    48705b04:	8b010281 	add	x1, x20, x1
    48705b08:	91000400 	add	x0, x0, #0x1
    48705b0c:	b9001025 	str	w5, [x1, #16]
    48705b10:	17fffff0 	b	48705ad0 <ddr_hw_training_by_phy+0x168>
    48705b14:	53196022 	lsl	w2, w1, #7
    48705b18:	2a0403e3 	mov	w3, w4
    48705b1c:	1108b042 	add	w2, w2, #0x22c
    48705b20:	b8617a85 	ldr	w5, [x20, x1, lsl #2]
    48705b24:	91000421 	add	x1, x1, #0x1
    48705b28:	93407c42 	sxtw	x2, w2
    48705b2c:	b8236845 	str	w5, [x2, x3]
    48705b30:	eb00003f 	cmp	x1, x0
    48705b34:	54ffff01 	b.ne	48705b14 <ddr_hw_training_by_phy+0x1ac>  // b.any
    48705b38:	17ffffeb 	b	48705ae4 <ddr_hw_training_by_phy+0x17c>
    48705b3c:	b9400184 	ldr	w4, [x12]
    48705b40:	b9401183 	ldr	w3, [x12, #16]
    48705b44:	d3412480 	ubfx	x0, x4, #1, #9
    48705b48:	b900926d 	str	w13, [x19, #144]
    48705b4c:	d3412461 	ubfx	x1, x3, #1, #9
    48705b50:	b9407e62 	ldr	w2, [x19, #124]
    48705b54:	6b01001f 	cmp	w0, w1
    48705b58:	54000149 	b.ls	48705b80 <ddr_hw_training_by_phy+0x218>  // b.plast
    48705b5c:	b82e6844 	str	w4, [x2, x14]
    48705b60:	4b010001 	sub	w1, w0, w1
    48705b64:	b9009e75 	str	w21, [x19, #156]
    48705b68:	aa1303e0 	mov	x0, x19
    48705b6c:	97fffa96 	bl	487045c4 <ddr_rdqs_sync_rank_rdq>
    48705b70:	110005ad 	add	w13, w13, #0x1
    48705b74:	910201ce 	add	x14, x14, #0x80
    48705b78:	9100118c 	add	x12, x12, #0x4
    48705b7c:	17ffff9c 	b	487059ec <ddr_hw_training_by_phy+0x84>
    48705b80:	b82e6843 	str	w3, [x2, x14]
    48705b84:	4b000021 	sub	w1, w1, w0
    48705b88:	b9009e7f 	str	wzr, [x19, #156]
    48705b8c:	17fffff7 	b	48705b68 <ddr_hw_training_by_phy+0x200>
    48705b90:	b8647ae1 	ldr	w1, [x23, x4, lsl #2]
    48705b94:	b840c460 	ldr	w0, [x3], #12
    48705b98:	97fff890 	bl	48703dd8 <ddr_training_set_timing>
    48705b9c:	91000484 	add	x4, x4, #0x1
    48705ba0:	17ffffa1 	b	48705a24 <ddr_hw_training_by_phy+0xbc>

0000000048705ba4 <ddr_hw_training>:
    48705ba4:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
    48705ba8:	910003fd 	mov	x29, sp
    48705bac:	a90153f3 	stp	x19, x20, [sp, #16]
    48705bb0:	aa0003f4 	mov	x20, x0
    48705bb4:	a9025bf5 	stp	x21, x22, [sp, #32]
    48705bb8:	d2800013 	mov	x19, #0x0                   	// #0
    48705bbc:	52800015 	mov	w21, #0x0                   	// #0
    48705bc0:	d2800796 	mov	x22, #0x3c                  	// #60
    48705bc4:	b9407a80 	ldr	w0, [x20, #120]
    48705bc8:	6b13001f 	cmp	w0, w19
    48705bcc:	540000c8 	b.hi	48705be4 <ddr_hw_training+0x40>  // b.pmore
    48705bd0:	2a1503e0 	mov	w0, w21
    48705bd4:	a94153f3 	ldp	x19, x20, [sp, #16]
    48705bd8:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48705bdc:	a8c37bfd 	ldp	x29, x30, [sp], #48
    48705be0:	d65f03c0 	ret
    48705be4:	b9009a93 	str	w19, [x20, #152]
    48705be8:	9b167e60 	mul	x0, x19, x22
    48705bec:	91000673 	add	x19, x19, #0x1
    48705bf0:	b8606a80 	ldr	w0, [x20, x0]
    48705bf4:	b9007e80 	str	w0, [x20, #124]
    48705bf8:	aa1403e0 	mov	x0, x20
    48705bfc:	97ffff5b 	bl	48705968 <ddr_hw_training_by_phy>
    48705c00:	0b0002b5 	add	w21, w21, w0
    48705c04:	17fffff0 	b	48705bc4 <ddr_hw_training+0x20>

0000000048705c08 <ddr_vref_training>:
    48705c08:	a9a67bfd 	stp	x29, x30, [sp, #-416]!
    48705c0c:	d2800787 	mov	x7, #0x3c                  	// #60
    48705c10:	910003fd 	mov	x29, sp
    48705c14:	a90153f3 	stp	x19, x20, [sp, #16]
    48705c18:	aa0003f3 	mov	x19, x0
    48705c1c:	a9025bf5 	stp	x21, x22, [sp, #32]
    48705c20:	910123b4 	add	x20, x29, #0x48
    48705c24:	a90363f7 	stp	x23, x24, [sp, #48]
    48705c28:	b940a001 	ldr	w1, [x0, #160]
    48705c2c:	b9407c05 	ldr	w5, [x0, #124]
    48705c30:	b9409c00 	ldr	w0, [x0, #156]
    48705c34:	b9409a64 	ldr	w4, [x19, #152]
    48705c38:	53185c22 	lsl	w2, w1, #8
    48705c3c:	53165400 	lsl	w0, w0, #10
    48705c40:	11084006 	add	w6, w0, #0x210
    48705c44:	11087000 	add	w0, w0, #0x21c
    48705c48:	0b0200c3 	add	w3, w6, w2
    48705c4c:	0b020000 	add	w0, w0, w2
    48705c50:	2a0103e2 	mov	w2, w1
    48705c54:	9b077c84 	mul	x4, x4, x7
    48705c58:	d2800187 	mov	x7, #0xc                   	// #12
    48705c5c:	9b071042 	madd	x2, x2, x7, x4
    48705c60:	8b020262 	add	x2, x19, x2
    48705c64:	b9401842 	ldr	w2, [x2, #24]
    48705c68:	0b010441 	add	w1, w2, w1, lsl #1
    48705c6c:	aa1403e2 	mov	x2, x20
    48705c70:	0b011cc1 	add	w1, w6, w1, lsl #7
    48705c74:	6b03003f 	cmp	w1, w3
    48705c78:	54000581 	b.ne	48705d28 <ddr_vref_training+0x120>  // b.any
    48705c7c:	910263a0 	add	x0, x29, #0x98
    48705c80:	52802082 	mov	w2, #0x104                 	// #260
    48705c84:	52800001 	mov	w1, #0x0                   	// #0
    48705c88:	97fff799 	bl	48703aec <ddrtr_memset>
    48705c8c:	b9408e60 	ldr	w0, [x19, #140]
    48705c90:	7100041f 	cmp	w0, #0x1
    48705c94:	540008e1 	b.ne	48705db0 <ddr_vref_training+0x1a8>  // b.any
    48705c98:	52800015 	mov	w21, #0x0                   	// #0
    48705c9c:	d2800196 	mov	x22, #0xc                   	// #12
    48705ca0:	d2800797 	mov	x23, #0x3c                  	// #60
    48705ca4:	b940a260 	ldr	w0, [x19, #160]
    48705ca8:	b9409a62 	ldr	w2, [x19, #152]
    48705cac:	2a0003e1 	mov	w1, w0
    48705cb0:	9b177c42 	mul	x2, x2, x23
    48705cb4:	9b160821 	madd	x1, x1, x22, x2
    48705cb8:	8b010261 	add	x1, x19, x1
    48705cbc:	b9401821 	ldr	w1, [x1, #24]
    48705cc0:	6b15003f 	cmp	w1, w21
    48705cc4:	54000628 	b.hi	48705d88 <ddr_vref_training+0x180>  // b.pmore
    48705cc8:	b9409e63 	ldr	w3, [x19, #156]
    48705ccc:	52800002 	mov	w2, #0x0                   	// #0
    48705cd0:	b9407e65 	ldr	w5, [x19, #124]
    48705cd4:	d2800186 	mov	x6, #0xc                   	// #12
    48705cd8:	d2800787 	mov	x7, #0x3c                  	// #60
    48705cdc:	53165463 	lsl	w3, w3, #10
    48705ce0:	b940a260 	ldr	w0, [x19, #160]
    48705ce4:	b9409a64 	ldr	w4, [x19, #152]
    48705ce8:	2a0003e1 	mov	w1, w0
    48705cec:	9b077c84 	mul	x4, x4, x7
    48705cf0:	9b061021 	madd	x1, x1, x6, x4
    48705cf4:	8b010261 	add	x1, x19, x1
    48705cf8:	b9401821 	ldr	w1, [x1, #24]
    48705cfc:	6b01005f 	cmp	w2, w1
    48705d00:	54000a83 	b.cc	48705e50 <ddr_vref_training+0x248>  // b.lo, b.ul, b.last
    48705d04:	910263a1 	add	x1, x29, #0x98
    48705d08:	aa1303e0 	mov	x0, x19
    48705d0c:	9400031a 	bl	48706974 <ddr_result_data_save>
    48705d10:	a94153f3 	ldp	x19, x20, [sp, #16]
    48705d14:	52800000 	mov	w0, #0x0                   	// #0
    48705d18:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48705d1c:	a94363f7 	ldp	x23, x24, [sp, #48]
    48705d20:	a8da7bfd 	ldp	x29, x30, [sp], #416
    48705d24:	d65f03c0 	ret
    48705d28:	b9408e64 	ldr	w4, [x19, #140]
    48705d2c:	7100089f 	cmp	w4, #0x2
    48705d30:	2a0503e4 	mov	w4, w5
    48705d34:	540001c1 	b.ne	48705d6c <ddr_vref_training+0x164>  // b.any
    48705d38:	2a0303e6 	mov	w6, w3
    48705d3c:	b86468c6 	ldr	w6, [x6, x4]
    48705d40:	b9000046 	str	w6, [x2]
    48705d44:	11001066 	add	w6, w3, #0x4
    48705d48:	b86468c6 	ldr	w6, [x6, x4]
    48705d4c:	b9001046 	str	w6, [x2, #16]
    48705d50:	11002066 	add	w6, w3, #0x8
    48705d54:	b86468c4 	ldr	w4, [x6, x4]
    48705d58:	b9004044 	str	w4, [x2, #64]
    48705d5c:	11020063 	add	w3, w3, #0x80
    48705d60:	91001042 	add	x2, x2, #0x4
    48705d64:	11020000 	add	w0, w0, #0x80
    48705d68:	17ffffc3 	b	48705c74 <ddr_vref_training+0x6c>
    48705d6c:	2a0003e6 	mov	w6, w0
    48705d70:	b86468c6 	ldr	w6, [x6, x4]
    48705d74:	b9000046 	str	w6, [x2]
    48705d78:	11001006 	add	w6, w0, #0x4
    48705d7c:	b86468c4 	ldr	w4, [x6, x4]
    48705d80:	b9001044 	str	w4, [x2, #16]
    48705d84:	17fffff6 	b	48705d5c <ddr_vref_training+0x154>
    48705d88:	0b0006a0 	add	w0, w21, w0, lsl #1
    48705d8c:	b9009260 	str	w0, [x19, #144]
    48705d90:	121e7800 	and	w0, w0, #0xfffffffd
    48705d94:	7100041f 	cmp	w0, #0x1
    48705d98:	54000080 	b.eq	48705da8 <ddr_vref_training+0x1a0>  // b.none
    48705d9c:	910263a1 	add	x1, x29, #0x98
    48705da0:	aa1303e0 	mov	x0, x19
    48705da4:	97fffca4 	bl	48705034 <ddr_vref_cal>
    48705da8:	110006b5 	add	w21, w21, #0x1
    48705dac:	17ffffbe 	b	48705ca4 <ddr_vref_training+0x9c>
    48705db0:	b9409a60 	ldr	w0, [x19, #152]
    48705db4:	d2800781 	mov	x1, #0x3c                  	// #60
    48705db8:	b9408263 	ldr	w3, [x19, #128]
    48705dbc:	d503201f 	nop
    48705dc0:	9b014c00 	madd	x0, x0, x1, x19
    48705dc4:	b9400402 	ldr	w2, [x0, #4]
    48705dc8:	b9409e60 	ldr	w0, [x19, #156]
    48705dcc:	531e7400 	lsl	w0, w0, #2
    48705dd0:	11018000 	add	w0, w0, #0x60
    48705dd4:	b8636816 	ldr	w22, [x0, x3]
    48705dd8:	51001840 	sub	w0, w2, #0x6
    48705ddc:	721d781f 	tst	w0, #0xfffffffb
    48705de0:	54fff981 	b.ne	48705d10 <ddr_vref_training+0x108>  // b.any
    48705de4:	d34a2ed6 	ubfx	x22, x22, #10, #2
    48705de8:	7100185f 	cmp	w2, #0x6
    48705dec:	1a9f16d6 	csinc	w22, w22, wzr, ne  // ne = any
    48705df0:	aa0103f7 	mov	x23, x1
    48705df4:	52800015 	mov	w21, #0x0                   	// #0
    48705df8:	d2800198 	mov	x24, #0xc                   	// #12
    48705dfc:	b940a261 	ldr	w1, [x19, #160]
    48705e00:	b9409a62 	ldr	w2, [x19, #152]
    48705e04:	2a0103e0 	mov	w0, w1
    48705e08:	9b177c42 	mul	x2, x2, x23
    48705e0c:	9b180800 	madd	x0, x0, x24, x2
    48705e10:	8b000260 	add	x0, x19, x0
    48705e14:	b9401800 	ldr	w0, [x0, #24]
    48705e18:	6b15001f 	cmp	w0, w21
    48705e1c:	54fff569 	b.ls	48705cc8 <ddr_vref_training+0xc0>  // b.plast
    48705e20:	0b0106a1 	add	w1, w21, w1, lsl #1
    48705e24:	710006df 	cmp	w22, #0x1
    48705e28:	b9009261 	str	w1, [x19, #144]
    48705e2c:	54000081 	b.ne	48705e3c <ddr_vref_training+0x234>  // b.any
    48705e30:	121e7aa0 	and	w0, w21, #0xfffffffd
    48705e34:	7100041f 	cmp	w0, #0x1
    48705e38:	54000080 	b.eq	48705e48 <ddr_vref_training+0x240>  // b.none
    48705e3c:	910263a1 	add	x1, x29, #0x98
    48705e40:	aa1303e0 	mov	x0, x19
    48705e44:	97fffc7c 	bl	48705034 <ddr_vref_cal>
    48705e48:	110006b5 	add	w21, w21, #0x1
    48705e4c:	17ffffec 	b	48705dfc <ddr_vref_training+0x1f4>
    48705e50:	b9408e61 	ldr	w1, [x19, #140]
    48705e54:	0b000440 	add	w0, w2, w0, lsl #1
    48705e58:	7100083f 	cmp	w1, #0x2
    48705e5c:	2a0503e1 	mov	w1, w5
    48705e60:	53196000 	lsl	w0, w0, #7
    48705e64:	0b030000 	add	w0, w0, w3
    48705e68:	540001a1 	b.ne	48705e9c <ddr_vref_training+0x294>  // b.any
    48705e6c:	11084004 	add	w4, w0, #0x210
    48705e70:	b9400288 	ldr	w8, [x20]
    48705e74:	b8216888 	str	w8, [x4, x1]
    48705e78:	11085004 	add	w4, w0, #0x214
    48705e7c:	b9401288 	ldr	w8, [x20, #16]
    48705e80:	11086000 	add	w0, w0, #0x218
    48705e84:	b8216888 	str	w8, [x4, x1]
    48705e88:	b9404284 	ldr	w4, [x20, #64]
    48705e8c:	b8216804 	str	w4, [x0, x1]
    48705e90:	11000442 	add	w2, w2, #0x1
    48705e94:	91001294 	add	x20, x20, #0x4
    48705e98:	17ffff92 	b	48705ce0 <ddr_vref_training+0xd8>
    48705e9c:	11087004 	add	w4, w0, #0x21c
    48705ea0:	b9400288 	ldr	w8, [x20]
    48705ea4:	11088000 	add	w0, w0, #0x220
    48705ea8:	b8216888 	str	w8, [x4, x1]
    48705eac:	b9401284 	ldr	w4, [x20, #16]
    48705eb0:	17fffff7 	b	48705e8c <ddr_vref_training+0x284>

0000000048705eb4 <ddr_vref_training_func>:
    48705eb4:	a9bb7bfd 	stp	x29, x30, [sp, #-80]!
    48705eb8:	52a02002 	mov	w2, #0x1000000             	// #16777216
    48705ebc:	910003fd 	mov	x29, sp
    48705ec0:	9100a3a1 	add	x1, x29, #0x28
    48705ec4:	a90153f3 	stp	x19, x20, [sp, #16]
    48705ec8:	aa0003f3 	mov	x19, x0
    48705ecc:	97fff92d 	bl	48704380 <ddr_training_save_reg>
    48705ed0:	aa1303e0 	mov	x0, x19
    48705ed4:	97fff8ff 	bl	487042d0 <ddr_training_switch_axi>
    48705ed8:	aa1303e0 	mov	x0, x19
    48705edc:	52800041 	mov	w1, #0x2                   	// #2
    48705ee0:	97fffa6b 	bl	4870488c <ddr_ddrt_init>
    48705ee4:	52800020 	mov	w0, #0x1                   	// #1
    48705ee8:	b9408661 	ldr	w1, [x19, #132]
    48705eec:	b900aa60 	str	w0, [x19, #168]
    48705ef0:	36c00161 	tbz	w1, #24, 48705f1c <ddr_vref_training_func+0x68>
    48705ef4:	52800014 	mov	w20, #0x0                   	// #0
    48705ef8:	b9408660 	ldr	w0, [x19, #132]
    48705efc:	36c801a0 	tbz	w0, #25, 48705f30 <ddr_vref_training_func+0x7c>
    48705f00:	9100a3a1 	add	x1, x29, #0x28
    48705f04:	aa1303e0 	mov	x0, x19
    48705f08:	97fff95a 	bl	48704470 <ddr_training_restore_reg>
    48705f0c:	2a1403e0 	mov	w0, w20
    48705f10:	a94153f3 	ldp	x19, x20, [sp, #16]
    48705f14:	a8c57bfd 	ldp	x29, x30, [sp], #80
    48705f18:	d65f03c0 	ret
    48705f1c:	b9008e60 	str	w0, [x19, #140]
    48705f20:	aa1303e0 	mov	x0, x19
    48705f24:	97ffff39 	bl	48705c08 <ddr_vref_training>
    48705f28:	2a0003f4 	mov	w20, w0
    48705f2c:	17fffff3 	b	48705ef8 <ddr_vref_training_func+0x44>
    48705f30:	52800040 	mov	w0, #0x2                   	// #2
    48705f34:	b9008e60 	str	w0, [x19, #140]
    48705f38:	aa1303e0 	mov	x0, x19
    48705f3c:	97ffff33 	bl	48705c08 <ddr_vref_training>
    48705f40:	0b000294 	add	w20, w20, w0
    48705f44:	17ffffef 	b	48705f00 <ddr_vref_training_func+0x4c>

0000000048705f48 <ddr_wl_func>:
    48705f48:	52800000 	mov	w0, #0x0                   	// #0
    48705f4c:	d65f03c0 	ret

0000000048705f50 <ddr_gating_func>:
    48705f50:	52800000 	mov	w0, #0x0                   	// #0
    48705f54:	d65f03c0 	ret

0000000048705f58 <ddr_ac_training_func>:
    48705f58:	52800000 	mov	w0, #0x0                   	// #0
    48705f5c:	d65f03c0 	ret

0000000048705f60 <ddr_lpca_training_func>:
    48705f60:	52800000 	mov	w0, #0x0                   	// #0
    48705f64:	d65f03c0 	ret

0000000048705f68 <ddr_training_break_point>:
    48705f68:	d2800000 	mov	x0, #0x0                   	// #0
    48705f6c:	14000217 	b	487067c8 <ddr_training_console_if>

0000000048705f70 <ddr_dcc_training>:
    48705f70:	a9a27bfd 	stp	x29, x30, [sp, #-480]!
    48705f74:	d2800a0b 	mov	x11, #0x50                  	// #80
    48705f78:	910003fd 	mov	x29, sp
    48705f7c:	a9046bf9 	stp	x25, x26, [sp, #64]
    48705f80:	aa0003f9 	mov	x25, x0
    48705f84:	a90153f3 	stp	x19, x20, [sp, #16]
    48705f88:	a9025bf5 	stp	x21, x22, [sp, #32]
    48705f8c:	a90363f7 	stp	x23, x24, [sp, #48]
    48705f90:	a90573fb 	stp	x27, x28, [sp, #80]
    48705f94:	b9409801 	ldr	w1, [x0, #152]
    48705f98:	d2800780 	mov	x0, #0x3c                  	// #60
    48705f9c:	9b006421 	madd	x1, x1, x0, x25
    48705fa0:	b9400c20 	ldr	w0, [x1, #12]
    48705fa4:	b90093a0 	str	w0, [x29, #144]
    48705fa8:	2a0003ea 	mov	w10, w0
    48705fac:	d2800000 	mov	x0, #0x0                   	// #0
    48705fb0:	eb0a001f 	cmp	x0, x10
    48705fb4:	54000c61 	b.ne	48706140 <ddr_dcc_training+0x1d0>  // b.any
    48705fb8:	9102a3a1 	add	x1, x29, #0xa8
    48705fbc:	aa1903e0 	mov	x0, x25
    48705fc0:	97fff793 	bl	48703e0c <ddr_training_save_timing>
    48705fc4:	b9407f36 	ldr	w22, [x25, #124]
    48705fc8:	12800003 	mov	w3, #0xffffffff            	// #-1
    48705fcc:	b9409b20 	ldr	w0, [x25, #152]
    48705fd0:	52800781 	mov	w1, #0x3c                  	// #60
    48705fd4:	b90163bf 	str	wzr, [x29, #352]
    48705fd8:	b9015fa3 	str	w3, [x29, #348]
    48705fdc:	d2821e85 	mov	x5, #0x10f4                	// #4340
    48705fe0:	b9019fa3 	str	w3, [x29, #412]
    48705fe4:	52800043 	mov	w3, #0x2                   	// #2
    48705fe8:	9ba17c00 	umull	x0, w0, w1
    48705fec:	b9017bbf 	str	wzr, [x29, #376]
    48705ff0:	b90187a3 	str	w3, [x29, #388]
    48705ff4:	528001e3 	mov	w3, #0xf                   	// #15
    48705ff8:	b9017fbf 	str	wzr, [x29, #380]
    48705ffc:	8b000324 	add	x4, x25, x0
    48706000:	b9018ba3 	str	w3, [x29, #392]
    48706004:	52800183 	mov	w3, #0xc                   	// #12
    48706008:	b90183bf 	str	wzr, [x29, #384]
    4870600c:	d2821f81 	mov	x1, #0x10fc                	// #4348
    48706010:	b9018fa3 	str	w3, [x29, #396]
    48706014:	52800063 	mov	w3, #0x3                   	// #3
    48706018:	b901a3bf 	str	wzr, [x29, #416]
    4870601c:	8b0102c2 	add	x2, x22, x1
    48706020:	b901c7a3 	str	w3, [x29, #452]
    48706024:	528003e3 	mov	w3, #0x1f                  	// #31
    48706028:	b901bbbf 	str	wzr, [x29, #440]
    4870602c:	8b0502db 	add	x27, x22, x5
    48706030:	b901cba3 	str	w3, [x29, #456]
    48706034:	52800383 	mov	w3, #0x1c                  	// #28
    48706038:	b901bfbf 	str	wzr, [x29, #444]
    4870603c:	b901c3bf 	str	wzr, [x29, #448]
    48706040:	b901cfa3 	str	w3, [x29, #460]
    48706044:	b9400481 	ldr	w1, [x4, #4]
    48706048:	b950fed4 	ldr	w20, [x22, #4348]
    4870604c:	b950fed4 	ldr	w20, [x22, #4348]
    48706050:	7100183f 	cmp	w1, #0x6
    48706054:	b950f6d3 	ldr	w19, [x22, #4340]
    48706058:	1a9f17f5 	cset	w21, eq  // eq = none
    4870605c:	b950f6d3 	ldr	w19, [x22, #4340]
    48706060:	110006b5 	add	w21, w21, #0x1
    48706064:	f90043a2 	str	x2, [x29, #128]
    48706068:	54000a40 	b.eq	487061b0 <ddr_dcc_training+0x240>  // b.none
    4870606c:	aa1903e0 	mov	x0, x25
    48706070:	52800021 	mov	w1, #0x1                   	// #1
    48706074:	97fff824 	bl	48704104 <ddr_training_ctrl_easr>
    48706078:	34000b80 	cbz	w0, 487061e8 <ddr_dcc_training+0x278>
    4870607c:	12800015 	mov	w21, #0xffffffff            	// #-1
    48706080:	910743b3 	add	x19, x29, #0x1d0
    48706084:	52800014 	mov	w20, #0x0                   	// #0
    48706088:	aa1303e3 	mov	x3, x19
    4870608c:	d2800784 	mov	x4, #0x3c                  	// #60
    48706090:	52a00045 	mov	w5, #0x20000               	// #131072
    48706094:	b94093a0 	ldr	w0, [x29, #144]
    48706098:	6b00029f 	cmp	w20, w0
    4870609c:	540030c1 	b.ne	487066b4 <ddr_dcc_training+0x744>  // b.any
    487060a0:	aa1903e0 	mov	x0, x25
    487060a4:	97fffe31 	bl	48705968 <ddr_hw_training_by_phy>
    487060a8:	0b150015 	add	w21, w0, w21
    487060ac:	52800001 	mov	w1, #0x0                   	// #0
    487060b0:	d2800784 	mov	x4, #0x3c                  	// #60
    487060b4:	6b01029f 	cmp	w20, w1
    487060b8:	54003161 	b.ne	487066e4 <ddr_dcc_training+0x774>  // b.any
    487060bc:	b9409b20 	ldr	w0, [x25, #152]
    487060c0:	d2800281 	mov	x1, #0x14                  	// #20
    487060c4:	52800784 	mov	w4, #0x3c                  	// #60
    487060c8:	d2800794 	mov	x20, #0x3c                  	// #60
    487060cc:	d2800013 	mov	x19, #0x0                   	// #0
    487060d0:	9ba40404 	umaddl	x4, w0, w4, x1
    487060d4:	2a0003e0 	mov	w0, w0
    487060d8:	8b040336 	add	x22, x25, x4
    487060dc:	9b146417 	madd	x23, x0, x20, x25
    487060e0:	b9400ae0 	ldr	w0, [x23, #8]
    487060e4:	6b13001f 	cmp	w0, w19
    487060e8:	54003148 	b.hi	48706710 <ddr_dcc_training+0x7a0>  // b.pmore
    487060ec:	b9409b20 	ldr	w0, [x25, #152]
    487060f0:	9bb47c00 	umull	x0, w0, w20
    487060f4:	8b000321 	add	x1, x25, x0
    487060f8:	b9400423 	ldr	w3, [x1, #4]
    487060fc:	7100187f 	cmp	w3, #0x6
    48706100:	54000101 	b.ne	48706120 <ddr_dcc_training+0x1b0>  // b.any
    48706104:	91005000 	add	x0, x0, #0x14
    48706108:	910283a4 	add	x4, x29, #0xa0
    4870610c:	8b000322 	add	x2, x25, x0
    48706110:	d2800000 	mov	x0, #0x0                   	// #0
    48706114:	b9400823 	ldr	w3, [x1, #8]
    48706118:	6b00007f 	cmp	w3, w0
    4870611c:	54003068 	b.hi	48706728 <ddr_dcc_training+0x7b8>  // b.pmore
    48706120:	2a1503e0 	mov	w0, w21
    48706124:	a94153f3 	ldp	x19, x20, [sp, #16]
    48706128:	a9425bf5 	ldp	x21, x22, [sp, #32]
    4870612c:	a94363f7 	ldp	x23, x24, [sp, #48]
    48706130:	a9446bf9 	ldp	x25, x26, [sp, #64]
    48706134:	a94573fb 	ldp	x27, x28, [sp, #80]
    48706138:	a8de7bfd 	ldp	x29, x30, [sp], #480
    4870613c:	d65f03c0 	ret
    48706140:	b9009f20 	str	w0, [x25, #156]
    48706144:	53165408 	lsl	w8, w0, #10
    48706148:	b9407f26 	ldr	w6, [x25, #124]
    4870614c:	9102c3a2 	add	x2, x29, #0xb0
    48706150:	b9401025 	ldr	w5, [x1, #16]
    48706154:	11087103 	add	w3, w8, #0x21c
    48706158:	9b0b0804 	madd	x4, x0, x11, x2
    4870615c:	9108b0c7 	add	x7, x6, #0x22c
    48706160:	0b051d05 	add	w5, w8, w5, lsl #7
    48706164:	110870a5 	add	w5, w5, #0x21c
    48706168:	6b05007f 	cmp	w3, w5
    4870616c:	54000061 	b.ne	48706178 <ddr_dcc_training+0x208>  // b.any
    48706170:	91000400 	add	x0, x0, #0x1
    48706174:	17ffff8f 	b	48705fb0 <ddr_dcc_training+0x40>
    48706178:	2a0303e8 	mov	w8, w3
    4870617c:	91001084 	add	x4, x4, #0x4
    48706180:	b8666908 	ldr	w8, [x8, x6]
    48706184:	b81fc088 	stur	w8, [x4, #-4]
    48706188:	11001068 	add	w8, w3, #0x4
    4870618c:	b8666908 	ldr	w8, [x8, x6]
    48706190:	b9000c88 	str	w8, [x4, #12]
    48706194:	11002068 	add	w8, w3, #0x8
    48706198:	11020063 	add	w3, w3, #0x80
    4870619c:	b8666908 	ldr	w8, [x8, x6]
    487061a0:	b9002c88 	str	w8, [x4, #44]
    487061a4:	b84804e8 	ldr	w8, [x7], #128
    487061a8:	b9001c88 	str	w8, [x4, #28]
    487061ac:	17ffffef 	b	48706168 <ddr_dcc_training+0x1f8>
    487061b0:	91005000 	add	x0, x0, #0x14
    487061b4:	910283a6 	add	x6, x29, #0xa0
    487061b8:	8b000320 	add	x0, x25, x0
    487061bc:	d2800001 	mov	x1, #0x0                   	// #0
    487061c0:	b9400883 	ldr	w3, [x4, #8]
    487061c4:	6b01007f 	cmp	w3, w1
    487061c8:	54fff529 	b.ls	4870606c <ddr_dcc_training+0xfc>  // b.plast
    487061cc:	b840c405 	ldr	w5, [x0], #12
    487061d0:	b94020a3 	ldr	w3, [x5, #32]
    487061d4:	b82178c3 	str	w3, [x6, x1, lsl #2]
    487061d8:	321e0463 	orr	w3, w3, #0xc
    487061dc:	91000421 	add	x1, x1, #0x1
    487061e0:	b90020a3 	str	w3, [x5, #32]
    487061e4:	17fffff7 	b	487061c0 <ddr_dcc_training+0x250>
    487061e8:	b9409b20 	ldr	w0, [x25, #152]
    487061ec:	d2800781 	mov	x1, #0x3c                  	// #60
    487061f0:	9b016400 	madd	x0, x0, x1, x25
    487061f4:	b9400400 	ldr	w0, [x0, #4]
    487061f8:	7100181f 	cmp	w0, #0x6
    487061fc:	540015a1 	b.ne	487064b0 <ddr_dcc_training+0x540>  // b.any
    48706200:	321e0680 	orr	w0, w20, #0xc
    48706204:	b910fec0 	str	w0, [x22, #4348]
    48706208:	aa1903e0 	mov	x0, x25
    4870620c:	52800041 	mov	w1, #0x2                   	// #2
    48706210:	97fff7bd 	bl	48704104 <ddr_training_ctrl_easr>
    48706214:	35fff340 	cbnz	w0, 4870607c <ddr_dcc_training+0x10c>
    48706218:	510006b5 	sub	w21, w21, #0x1
    4870621c:	9102c3a1 	add	x1, x29, #0xb0
    48706220:	52800016 	mov	w22, #0x0                   	// #0
    48706224:	d2800798 	mov	x24, #0x3c                  	// #60
    48706228:	d37ae6b5 	lsl	x21, x21, #6
    4870622c:	9103a2a0 	add	x0, x21, #0xe8
    48706230:	9103b2b5 	add	x21, x21, #0xec
    48706234:	8b000020 	add	x0, x1, x0
    48706238:	f9003ba0 	str	x0, [x29, #112]
    4870623c:	8b150020 	add	x0, x1, x21
    48706240:	f9003fa0 	str	x0, [x29, #120]
    48706244:	12800000 	mov	w0, #0xffffffff            	// #-1
    48706248:	52800021 	mov	w1, #0x1                   	// #1
    4870624c:	b9016bbf 	str	wzr, [x29, #360]
    48706250:	b90167a0 	str	w0, [x29, #356]
    48706254:	b901a7a0 	str	w0, [x29, #420]
    48706258:	aa1903e0 	mov	x0, x25
    4870625c:	b901abbf 	str	wzr, [x29, #424]
    48706260:	97fff7a9 	bl	48704104 <ddr_training_ctrl_easr>
    48706264:	35fff0c0 	cbnz	w0, 4870607c <ddr_dcc_training+0x10c>
    48706268:	b9409b20 	ldr	w0, [x25, #152]
    4870626c:	531142c1 	lsl	w1, w22, #15
    48706270:	b9407f23 	ldr	w3, [x25, #124]
    48706274:	d503201f 	nop
    48706278:	9b186400 	madd	x0, x0, x24, x25
    4870627c:	b9400400 	ldr	w0, [x0, #4]
    48706280:	7100181f 	cmp	w0, #0x6
    48706284:	540011a1 	b.ne	487064b8 <ddr_dcc_training+0x548>  // b.any
    48706288:	2a167c21 	orr	w1, w1, w22, lsl #31
    4870628c:	1200ba60 	and	w0, w19, #0x7fff7fff
    48706290:	2a010000 	orr	w0, w0, w1
    48706294:	52800041 	mov	w1, #0x2                   	// #2
    48706298:	b910f460 	str	w0, [x3, #4340]
    4870629c:	b901dba0 	str	w0, [x29, #472]
    487062a0:	aa1903e0 	mov	x0, x25
    487062a4:	97fff798 	bl	48704104 <ddr_training_ctrl_easr>
    487062a8:	35ffeea0 	cbnz	w0, 4870607c <ddr_dcc_training+0x10c>
    487062ac:	52800017 	mov	w23, #0x0                   	// #0
    487062b0:	1280001c 	mov	w28, #0xffffffff            	// #-1
    487062b4:	aa1903e0 	mov	x0, x25
    487062b8:	52800021 	mov	w1, #0x1                   	// #1
    487062bc:	b9015bbc 	str	w28, [x29, #344]
    487062c0:	b9019bbc 	str	w28, [x29, #408]
    487062c4:	97fff790 	bl	48704104 <ddr_training_ctrl_easr>
    487062c8:	35ffeda0 	cbnz	w0, 4870607c <ddr_dcc_training+0x10c>
    487062cc:	b9409b20 	ldr	w0, [x25, #152]
    487062d0:	53144ee3 	lsl	w3, w23, #12
    487062d4:	b9407f24 	ldr	w4, [x25, #124]
    487062d8:	b941dba1 	ldr	w1, [x29, #472]
    487062dc:	d503201f 	nop
    487062e0:	9b186400 	madd	x0, x0, x24, x25
    487062e4:	b9400400 	ldr	w0, [x0, #4]
    487062e8:	7100181f 	cmp	w0, #0x6
    487062ec:	54000ea1 	b.ne	487064c0 <ddr_dcc_training+0x550>  // b.any
    487062f0:	2a177063 	orr	w3, w3, w23, lsl #28
    487062f4:	1201b020 	and	w0, w1, #0x8fff8fff
    487062f8:	2a030000 	orr	w0, w0, w3
    487062fc:	52800041 	mov	w1, #0x2                   	// #2
    48706300:	b910f480 	str	w0, [x4, #4340]
    48706304:	b901dba0 	str	w0, [x29, #472]
    48706308:	aa1903e0 	mov	x0, x25
    4870630c:	97fff77e 	bl	48704104 <ddr_training_ctrl_easr>
    48706310:	35ffeb60 	cbnz	w0, 4870607c <ddr_dcc_training+0x10c>
    48706314:	b9409b20 	ldr	w0, [x25, #152]
    48706318:	52800015 	mov	w21, #0x0                   	// #0
    4870631c:	5280001a 	mov	w26, #0x0                   	// #0
    48706320:	9b186400 	madd	x0, x0, x24, x25
    48706324:	b9400c00 	ldr	w0, [x0, #12]
    48706328:	b9006fa0 	str	w0, [x29, #108]
    4870632c:	b9406fa0 	ldr	w0, [x29, #108]
    48706330:	6b00035f 	cmp	w26, w0
    48706334:	54000ca1 	b.ne	487064c8 <ddr_dcc_training+0x558>  // b.any
    48706338:	910563a0 	add	x0, x29, #0x158
    4870633c:	b9400001 	ldr	w1, [x0]
    48706340:	b9400c03 	ldr	w3, [x0, #12]
    48706344:	6b03003f 	cmp	w1, w3
    48706348:	54000042 	b.cs	48706350 <ddr_dcc_training+0x3e0>  // b.hs, b.nlast
    4870634c:	b9000c01 	str	w1, [x0, #12]
    48706350:	b9401003 	ldr	w3, [x0, #16]
    48706354:	6b03003f 	cmp	w1, w3
    48706358:	54000069 	b.ls	48706364 <ddr_dcc_training+0x3f4>  // b.plast
    4870635c:	b9001001 	str	w1, [x0, #16]
    48706360:	b9002017 	str	w23, [x0, #32]
    48706364:	f9403ba1 	ldr	x1, [x29, #112]
    48706368:	91010000 	add	x0, x0, #0x40
    4870636c:	eb01001f 	cmp	x0, x1
    48706370:	54fffe61 	b.ne	4870633c <ddr_dcc_training+0x3cc>  // b.any
    48706374:	110006f7 	add	w23, w23, #0x1
    48706378:	710022ff 	cmp	w23, #0x8
    4870637c:	54fff9c1 	b.ne	487062b4 <ddr_dcc_training+0x344>  // b.any
    48706380:	910573b7 	add	x23, x29, #0x15c
    48706384:	aa1703fc 	mov	x28, x23
    48706388:	b9400381 	ldr	w1, [x28]
    4870638c:	b9400b80 	ldr	w0, [x28, #8]
    48706390:	6b01001f 	cmp	w0, w1
    48706394:	54000042 	b.cs	4870639c <ddr_dcc_training+0x42c>  // b.hs, b.nlast
    48706398:	b9000380 	str	w0, [x28]
    4870639c:	b9400781 	ldr	w1, [x28, #4]
    487063a0:	b9400f80 	ldr	w0, [x28, #12]
    487063a4:	6b01001f 	cmp	w0, w1
    487063a8:	540000a9 	b.ls	487063bc <ddr_dcc_training+0x44c>  // b.plast
    487063ac:	b9000780 	str	w0, [x28, #4]
    487063b0:	b9401f80 	ldr	w0, [x28, #28]
    487063b4:	b9002796 	str	w22, [x28, #36]
    487063b8:	b9002380 	str	w0, [x28, #32]
    487063bc:	f9403fa0 	ldr	x0, [x29, #120]
    487063c0:	9101039c 	add	x28, x28, #0x40
    487063c4:	eb1c001f 	cmp	x0, x28
    487063c8:	54fffe01 	b.ne	48706388 <ddr_dcc_training+0x418>  // b.any
    487063cc:	110006d6 	add	w22, w22, #0x1
    487063d0:	71000adf 	cmp	w22, #0x2
    487063d4:	54fff381 	b.ne	48706244 <ddr_dcc_training+0x2d4>  // b.any
    487063d8:	b9409b20 	ldr	w0, [x25, #152]
    487063dc:	52800781 	mov	w1, #0x3c                  	// #60
    487063e0:	9ba17c00 	umull	x0, w0, w1
    487063e4:	8b000324 	add	x4, x25, x0
    487063e8:	b9400481 	ldr	w1, [x4, #4]
    487063ec:	7100183f 	cmp	w1, #0x6
    487063f0:	54001360 	b.eq	4870665c <ddr_dcc_training+0x6ec>  // b.none
    487063f4:	aa1903e0 	mov	x0, x25
    487063f8:	52800021 	mov	w1, #0x1                   	// #1
    487063fc:	97fff742 	bl	48704104 <ddr_training_ctrl_easr>
    48706400:	35ffe3e0 	cbnz	w0, 4870607c <ddr_dcc_training+0x10c>
    48706404:	52800025 	mov	w5, #0x1                   	// #1
    48706408:	528000e6 	mov	w6, #0x7                   	// #7
    4870640c:	294002e1 	ldp	w1, w0, [x23]
    48706410:	b94032e2 	ldr	w2, [x23, #48]
    48706414:	4b010000 	sub	w0, w0, w1
    48706418:	7100081f 	cmp	w0, #0x2
    4870641c:	b9402ee0 	ldr	w0, [x23, #44]
    48706420:	540013a8 	b.hi	48706694 <ddr_dcc_training+0x724>  // b.pmore
    48706424:	b9402ae7 	ldr	w7, [x23, #40]
    48706428:	1ac02661 	lsr	w1, w19, w0
    4870642c:	1ac22660 	lsr	w0, w19, w2
    48706430:	12000021 	and	w1, w1, #0x1
    48706434:	12000800 	and	w0, w0, #0x7
    48706438:	b9001ae0 	str	w0, [x23, #24]
    4870643c:	1ac72684 	lsr	w4, w20, w7
    48706440:	1ac720a0 	lsl	w0, w5, w7
    48706444:	12000083 	and	w3, w4, #0x1
    48706448:	0a200294 	bic	w20, w20, w0
    4870644c:	f94043a0 	ldr	x0, [x29, #128]
    48706450:	290206e3 	stp	w3, w1, [x23, #16]
    48706454:	1ac72063 	lsl	w3, w3, w7
    48706458:	2a140074 	orr	w20, w3, w20
    4870645c:	b9000014 	str	w20, [x0]
    48706460:	29458ee1 	ldp	w1, w3, [x23, #44]
    48706464:	1ac120a0 	lsl	w0, w5, w1
    48706468:	1ac320c4 	lsl	w4, w6, w3
    4870646c:	2a040000 	orr	w0, w0, w4
    48706470:	0a200273 	bic	w19, w19, w0
    48706474:	b94016e0 	ldr	w0, [x23, #20]
    48706478:	1ac12000 	lsl	w0, w0, w1
    4870647c:	b9401ae1 	ldr	w1, [x23, #24]
    48706480:	1ac32021 	lsl	w1, w1, w3
    48706484:	2a000020 	orr	w0, w1, w0
    48706488:	910102f7 	add	x23, x23, #0x40
    4870648c:	2a130013 	orr	w19, w0, w19
    48706490:	eb1c02ff 	cmp	x23, x28
    48706494:	b9000373 	str	w19, [x27]
    48706498:	54fffba1 	b.ne	4870640c <ddr_dcc_training+0x49c>  // b.any
    4870649c:	aa1903e0 	mov	x0, x25
    487064a0:	52800041 	mov	w1, #0x2                   	// #2
    487064a4:	97fff718 	bl	48704104 <ddr_training_ctrl_easr>
    487064a8:	34ffdec0 	cbz	w0, 48706080 <ddr_dcc_training+0x110>
    487064ac:	17fffef4 	b	4870607c <ddr_dcc_training+0x10c>
    487064b0:	321e0280 	orr	w0, w20, #0x4
    487064b4:	17ffff54 	b	48706204 <ddr_dcc_training+0x294>
    487064b8:	12107a60 	and	w0, w19, #0xffff7fff
    487064bc:	17ffff75 	b	48706290 <ddr_dcc_training+0x320>
    487064c0:	12117020 	and	w0, w1, #0xffff8fff
    487064c4:	17ffff8d 	b	487062f8 <ddr_dcc_training+0x388>
    487064c8:	b9407f21 	ldr	w1, [x25, #124]
    487064cc:	b9009f3a 	str	w26, [x25, #156]
    487064d0:	b9404820 	ldr	w0, [x1, #72]
    487064d4:	121c6c00 	and	w0, w0, #0xfffffff0
    487064d8:	2a1a0000 	orr	w0, w0, w26
    487064dc:	b9004820 	str	w0, [x1, #72]
    487064e0:	b9407f24 	ldr	w4, [x25, #124]
    487064e4:	b9409f20 	ldr	w0, [x25, #156]
    487064e8:	b940c881 	ldr	w1, [x4, #200]
    487064ec:	12007821 	and	w1, w1, #0x7fffffff
    487064f0:	2a007c20 	orr	w0, w1, w0, lsl #31
    487064f4:	52802001 	mov	w1, #0x100                 	// #256
    487064f8:	b900c880 	str	w0, [x4, #200]
    487064fc:	aa1903e0 	mov	x0, x25
    48706500:	97fff73e 	bl	487041f8 <ddr_hw_training_process>
    48706504:	0b0002b5 	add	w21, w21, w0
    48706508:	b9415ba0 	ldr	w0, [x29, #344]
    4870650c:	d2804a02 	mov	x2, #0x250                 	// #592
    48706510:	910543a8 	add	x8, x29, #0x150
    48706514:	b9008fa0 	str	w0, [x29, #140]
    48706518:	b9409b20 	ldr	w0, [x25, #152]
    4870651c:	b9008ba0 	str	w0, [x29, #136]
    48706520:	2a0003e0 	mov	w0, w0
    48706524:	9b186400 	madd	x0, x0, x24, x25
    48706528:	b9401000 	ldr	w0, [x0, #16]
    4870652c:	b90097a0 	str	w0, [x29, #148]
    48706530:	b94097a0 	ldr	w0, [x29, #148]
    48706534:	53017c01 	lsr	w1, w0, #1
    48706538:	52801000 	mov	w0, #0x80                  	// #128
    4870653c:	9ba00821 	umaddl	x1, w1, w0, x2
    48706540:	aa0203e0 	mov	x0, x2
    48706544:	b9407f22 	ldr	w2, [x25, #124]
    48706548:	eb01001f 	cmp	x0, x1
    4870654c:	f9004fa2 	str	x2, [x29, #152]
    48706550:	54000501 	b.ne	487065f0 <ddr_dcc_training+0x680>  // b.any
    48706554:	9102c3a0 	add	x0, x29, #0xb0
    48706558:	52800001 	mov	w1, #0x0                   	// #0
    4870655c:	97fff54f 	bl	48703a98 <ddr_dcc_get_min_win>
    48706560:	295107a4 	ldp	w4, w1, [x29, #136]
    48706564:	6b01001f 	cmp	w0, w1
    48706568:	9b186484 	madd	x4, x4, x24, x25
    4870656c:	1a819000 	csel	w0, w0, w1, ls  // ls = plast
    48706570:	b9015ba0 	str	w0, [x29, #344]
    48706574:	b9400480 	ldr	w0, [x4, #4]
    48706578:	7100181f 	cmp	w0, #0x6
    4870657c:	540001e1 	b.ne	487065b8 <ddr_dcc_training+0x648>  // b.any
    48706580:	b9419ba0 	ldr	w0, [x29, #408]
    48706584:	910643a7 	add	x7, x29, #0x190
    48706588:	b9008fa0 	str	w0, [x29, #140]
    4870658c:	d2800040 	mov	x0, #0x2                   	// #2
    48706590:	b94097a1 	ldr	w1, [x29, #148]
    48706594:	6b00003f 	cmp	w1, w0
    48706598:	54000368 	b.hi	48706604 <ddr_dcc_training+0x694>  // b.pmore
    4870659c:	9102c3a0 	add	x0, x29, #0xb0
    487065a0:	52800021 	mov	w1, #0x1                   	// #1
    487065a4:	97fff53d 	bl	48703a98 <ddr_dcc_get_min_win>
    487065a8:	b9408fa1 	ldr	w1, [x29, #140]
    487065ac:	6b01001f 	cmp	w0, w1
    487065b0:	1a819000 	csel	w0, w0, w1, ls  // ls = plast
    487065b4:	b9019ba0 	str	w0, [x29, #408]
    487065b8:	b9409f21 	ldr	w1, [x25, #156]
    487065bc:	52800a04 	mov	w4, #0x50                  	// #80
    487065c0:	9102c3a2 	add	x2, x29, #0xb0
    487065c4:	53165420 	lsl	w0, w1, #10
    487065c8:	9ba40821 	umaddl	x1, w1, w4, x2
    487065cc:	f9404fa2 	ldr	x2, [x29, #152]
    487065d0:	11087000 	add	w0, w0, #0x21c
    487065d4:	9108b044 	add	x4, x2, #0x22c
    487065d8:	b94097a2 	ldr	w2, [x29, #148]
    487065dc:	0b021c06 	add	w6, w0, w2, lsl #7
    487065e0:	6b06001f 	cmp	w0, w6
    487065e4:	540001e1 	b.ne	48706620 <ddr_dcc_training+0x6b0>  // b.any
    487065e8:	1100075a 	add	w26, w26, #0x1
    487065ec:	17ffff50 	b	4870632c <ddr_dcc_training+0x3bc>
    487065f0:	f9404fa2 	ldr	x2, [x29, #152]
    487065f4:	b8606845 	ldr	w5, [x2, x0]
    487065f8:	91020000 	add	x0, x0, #0x80
    487065fc:	b8004505 	str	w5, [x8], #4
    48706600:	17ffffd1 	b	48706544 <ddr_dcc_training+0x5d4>
    48706604:	d379e001 	lsl	x1, x0, #7
    48706608:	f9404fa2 	ldr	x2, [x29, #152]
    4870660c:	91094021 	add	x1, x1, #0x250
    48706610:	91000400 	add	x0, x0, #0x1
    48706614:	b8626821 	ldr	w1, [x1, x2]
    48706618:	b80044e1 	str	w1, [x7], #4
    4870661c:	17ffffdd 	b	48706590 <ddr_dcc_training+0x620>
    48706620:	f9404fa2 	ldr	x2, [x29, #152]
    48706624:	2a0003e7 	mov	w7, w0
    48706628:	b9400028 	ldr	w8, [x1]
    4870662c:	91001021 	add	x1, x1, #0x4
    48706630:	b82268e8 	str	w8, [x7, x2]
    48706634:	11001007 	add	w7, w0, #0x4
    48706638:	b9400c28 	ldr	w8, [x1, #12]
    4870663c:	b82268e8 	str	w8, [x7, x2]
    48706640:	11002007 	add	w7, w0, #0x8
    48706644:	11020000 	add	w0, w0, #0x80
    48706648:	b9402c28 	ldr	w8, [x1, #44]
    4870664c:	b82268e8 	str	w8, [x7, x2]
    48706650:	b9401c27 	ldr	w7, [x1, #28]
    48706654:	b8080487 	str	w7, [x4], #128
    48706658:	17ffffe2 	b	487065e0 <ddr_dcc_training+0x670>
    4870665c:	91005000 	add	x0, x0, #0x14
    48706660:	910283a6 	add	x6, x29, #0xa0
    48706664:	8b000320 	add	x0, x25, x0
    48706668:	d2800001 	mov	x1, #0x0                   	// #0
    4870666c:	b9400883 	ldr	w3, [x4, #8]
    48706670:	6b01007f 	cmp	w3, w1
    48706674:	54ffec09 	b.ls	487063f4 <ddr_dcc_training+0x484>  // b.plast
    48706678:	b840c405 	ldr	w5, [x0], #12
    4870667c:	b94020a3 	ldr	w3, [x5, #32]
    48706680:	b82178c3 	str	w3, [x6, x1, lsl #2]
    48706684:	321e0463 	orr	w3, w3, #0xc
    48706688:	91000421 	add	x1, x1, #0x1
    4870668c:	b90020a3 	str	w3, [x5, #32]
    48706690:	17fffff7 	b	4870666c <ddr_dcc_training+0x6fc>
    48706694:	1ac020a4 	lsl	w4, w5, w0
    48706698:	1ac220c3 	lsl	w3, w6, w2
    4870669c:	2a030084 	orr	w4, w4, w3
    487066a0:	0a240273 	bic	w19, w19, w4
    487066a4:	294412e3 	ldp	w3, w4, [x23, #32]
    487066a8:	1ac02081 	lsl	w1, w4, w0
    487066ac:	1ac22060 	lsl	w0, w3, w2
    487066b0:	17ffff75 	b	48706484 <ddr_dcc_training+0x514>
    487066b4:	b9409b20 	ldr	w0, [x25, #152]
    487066b8:	93407e81 	sxtw	x1, w20
    487066bc:	91001021 	add	x1, x1, #0x4
    487066c0:	b9009f34 	str	w20, [x25, #156]
    487066c4:	11000694 	add	w20, w20, #0x1
    487066c8:	9b047c00 	mul	x0, x0, x4
    487066cc:	8b010c01 	add	x1, x0, x1, lsl #3
    487066d0:	8b010321 	add	x1, x25, x1
    487066d4:	b9401020 	ldr	w0, [x1, #16]
    487066d8:	b9001025 	str	w5, [x1, #16]
    487066dc:	b8004460 	str	w0, [x3], #4
    487066e0:	17fffe6d 	b	48706094 <ddr_dcc_training+0x124>
    487066e4:	b9409b23 	ldr	w3, [x25, #152]
    487066e8:	93407c20 	sxtw	x0, w1
    487066ec:	91001000 	add	x0, x0, #0x4
    487066f0:	b9009f21 	str	w1, [x25, #156]
    487066f4:	11000421 	add	w1, w1, #0x1
    487066f8:	9b047c63 	mul	x3, x3, x4
    487066fc:	8b000c60 	add	x0, x3, x0, lsl #3
    48706700:	b8404663 	ldr	w3, [x19], #4
    48706704:	8b000320 	add	x0, x25, x0
    48706708:	b9001003 	str	w3, [x0, #16]
    4870670c:	17fffe6a 	b	487060b4 <ddr_dcc_training+0x144>
    48706710:	9102a3a0 	add	x0, x29, #0xa8
    48706714:	b8737801 	ldr	w1, [x0, x19, lsl #2]
    48706718:	91000673 	add	x19, x19, #0x1
    4870671c:	b840c6c0 	ldr	w0, [x22], #12
    48706720:	97fff5ae 	bl	48703dd8 <ddr_training_set_timing>
    48706724:	17fffe6f 	b	487060e0 <ddr_dcc_training+0x170>
    48706728:	b840c443 	ldr	w3, [x2], #12
    4870672c:	b8607885 	ldr	w5, [x4, x0, lsl #2]
    48706730:	91000400 	add	x0, x0, #0x1
    48706734:	b9002065 	str	w5, [x3, #32]
    48706738:	17fffe77 	b	48706114 <ddr_dcc_training+0x1a4>

000000004870673c <ddr_dcc_training_func>:
    4870673c:	a9bc7bfd 	stp	x29, x30, [sp, #-64]!
    48706740:	910003fd 	mov	x29, sp
    48706744:	a90153f3 	stp	x19, x20, [sp, #16]
    48706748:	aa0003f3 	mov	x19, x0
    4870674c:	a9025bf5 	stp	x21, x22, [sp, #32]
    48706750:	aa0003f5 	mov	x21, x0
    48706754:	f9001bf7 	str	x23, [sp, #48]
    48706758:	52800016 	mov	w22, #0x0                   	// #0
    4870675c:	52800014 	mov	w20, #0x0                   	// #0
    48706760:	52800037 	mov	w23, #0x1                   	// #1
    48706764:	b9407a60 	ldr	w0, [x19, #120]
    48706768:	6b14001f 	cmp	w0, w20
    4870676c:	540000e8 	b.hi	48706788 <ddr_dcc_training_func+0x4c>  // b.pmore
    48706770:	2a1603e0 	mov	w0, w22
    48706774:	f9401bf7 	ldr	x23, [sp, #48]
    48706778:	a94153f3 	ldp	x19, x20, [sp, #16]
    4870677c:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48706780:	a8c47bfd 	ldp	x29, x30, [sp], #64
    48706784:	d65f03c0 	ret
    48706788:	b9009a74 	str	w20, [x19, #152]
    4870678c:	b94002a0 	ldr	w0, [x21]
    48706790:	b9007e60 	str	w0, [x19, #124]
    48706794:	1ad422e0 	lsl	w0, w23, w20
    48706798:	32050000 	orr	w0, w0, #0x8000000
    4870679c:	b9402ea1 	ldr	w1, [x21, #44]
    487067a0:	6a01001f 	tst	w0, w1
    487067a4:	b9008661 	str	w1, [x19, #132]
    487067a8:	54000080 	b.eq	487067b8 <ddr_dcc_training_func+0x7c>  // b.none
    487067ac:	11000694 	add	w20, w20, #0x1
    487067b0:	9100f2b5 	add	x21, x21, #0x3c
    487067b4:	17ffffec 	b	48706764 <ddr_dcc_training_func+0x28>
    487067b8:	aa1303e0 	mov	x0, x19
    487067bc:	97fffded 	bl	48705f70 <ddr_dcc_training>
    487067c0:	0b0002d6 	add	w22, w22, w0
    487067c4:	17fffffa 	b	487067ac <ddr_dcc_training_func+0x70>

00000000487067c8 <ddr_training_console_if>:
    487067c8:	52800000 	mov	w0, #0x0                   	// #0
    487067cc:	d65f03c0 	ret

00000000487067d0 <ddr_training_boot_func>:
    487067d0:	a9bb7bfd 	stp	x29, x30, [sp, #-80]!
    487067d4:	910003fd 	mov	x29, sp
    487067d8:	a90153f3 	stp	x19, x20, [sp, #16]
    487067dc:	aa0003f3 	mov	x19, x0
    487067e0:	b9407c01 	ldr	w1, [x0, #124]
    487067e4:	2a0103e0 	mov	w0, w1
    487067e8:	b9400800 	ldr	w0, [x0, #8]
    487067ec:	362800a0 	tbz	w0, #5, 48706800 <ddr_training_boot_func+0x30>
    487067f0:	12800003 	mov	w3, #0xffffffff            	// #-1
    487067f4:	52800040 	mov	w0, #0x2                   	// #2
    487067f8:	2a0303e2 	mov	w2, w3
    487067fc:	97fff61f 	bl	48704078 <ddr_training_stat>
    48706800:	aa1303e0 	mov	x0, x19
    48706804:	97fffdd7 	bl	48705f60 <ddr_lpca_training_func>
    48706808:	2a0003f4 	mov	w20, w0
    4870680c:	aa1303e0 	mov	x0, x19
    48706810:	97fffdce 	bl	48705f48 <ddr_wl_func>
    48706814:	0b000294 	add	w20, w20, w0
    48706818:	aa1303e0 	mov	x0, x19
    4870681c:	97fffb26 	bl	487054b4 <ddr_dataeye_training_func>
    48706820:	2b000294 	adds	w20, w20, w0
    48706824:	54000280 	b.eq	48706874 <ddr_training_boot_func+0xa4>  // b.none
    48706828:	aa1303e0 	mov	x0, x19
    4870682c:	52a00201 	mov	w1, #0x100000              	// #1048576
    48706830:	97fff6a2 	bl	487042b8 <ddr_training_check_bypass>
    48706834:	35000200 	cbnz	w0, 48706874 <ddr_training_boot_func+0xa4>
    48706838:	9100a3a1 	add	x1, x29, #0x28
    4870683c:	aa1303e0 	mov	x0, x19
    48706840:	52a00202 	mov	w2, #0x100000              	// #1048576
    48706844:	97fff6cf 	bl	48704380 <ddr_training_save_reg>
    48706848:	aa1303e0 	mov	x0, x19
    4870684c:	97fffb36 	bl	48705524 <ddr_hw_dataeye_read>
    48706850:	2a0003f4 	mov	w20, w0
    48706854:	9100a3a1 	add	x1, x29, #0x28
    48706858:	aa1303e0 	mov	x0, x19
    4870685c:	97fff705 	bl	48704470 <ddr_training_restore_reg>
    48706860:	52800020 	mov	w0, #0x1                   	// #1
    48706864:	b900a660 	str	w0, [x19, #164]
    48706868:	aa1303e0 	mov	x0, x19
    4870686c:	97fffaf5 	bl	48705440 <ddr_dataeye_training>
    48706870:	0b000294 	add	w20, w20, w0
    48706874:	aa1303e0 	mov	x0, x19
    48706878:	97fffdb6 	bl	48705f50 <ddr_gating_func>
    4870687c:	0b000294 	add	w20, w20, w0
    48706880:	aa1303e0 	mov	x0, x19
    48706884:	97fffd8c 	bl	48705eb4 <ddr_vref_training_func>
    48706888:	0b000280 	add	w0, w20, w0
    4870688c:	a94153f3 	ldp	x19, x20, [sp, #16]
    48706890:	a8c57bfd 	ldp	x29, x30, [sp], #80
    48706894:	d65f03c0 	ret

0000000048706898 <ddr_sw_training_func>:
    48706898:	d2802b01 	mov	x1, #0x158                 	// #344
    4870689c:	52804262 	mov	w2, #0x213                 	// #531
    487068a0:	f2a24041 	movk	x1, #0x1202, lsl #16
    487068a4:	b9400020 	ldr	w0, [x1]
    487068a8:	12103c00 	and	w0, w0, #0xffff0000
    487068ac:	2a020000 	orr	w0, w0, w2
    487068b0:	b9000020 	str	w0, [x1]
    487068b4:	d2801400 	mov	x0, #0xa0                  	// #160
    487068b8:	f2a24040 	movk	x0, #0x1202, lsl #16
    487068bc:	b9400000 	ldr	w0, [x0]
    487068c0:	3100041f 	cmn	w0, #0x1
    487068c4:	540000e1 	b.ne	487068e0 <ddr_sw_training_func+0x48>  // b.any
    487068c8:	d2801480 	mov	x0, #0xa4                  	// #164
    487068cc:	f2a24040 	movk	x0, #0x1202, lsl #16
    487068d0:	b9400001 	ldr	w1, [x0]
    487068d4:	52800000 	mov	w0, #0x0                   	// #0
    487068d8:	3100043f 	cmn	w1, #0x1
    487068dc:	54000360 	b.eq	48706948 <ddr_sw_training_func+0xb0>  // b.none
    487068e0:	a9b27bfd 	stp	x29, x30, [sp, #-224]!
    487068e4:	910003fd 	mov	x29, sp
    487068e8:	f9000bf3 	str	x19, [sp, #16]
    487068ec:	94000028 	bl	4870698c <ddr_training_start>
    487068f0:	d2806000 	mov	x0, #0x300                 	// #768
    487068f4:	f2a24040 	movk	x0, #0x1202, lsl #16
    487068f8:	b900001f 	str	wzr, [x0]
    487068fc:	910083a0 	add	x0, x29, #0x20
    48706900:	97fff7d6 	bl	48704858 <ddr_training_cfg_init>
    48706904:	f9006bbf 	str	xzr, [x29, #208]
    48706908:	910083a0 	add	x0, x29, #0x20
    4870690c:	97fff4dd 	bl	48703c80 <ddr_training_all>
    48706910:	2a0003f3 	mov	w19, w0
    48706914:	910083a0 	add	x0, x29, #0x20
    48706918:	97ffff89 	bl	4870673c <ddr_dcc_training_func>
    4870691c:	2b000273 	adds	w19, w19, w0
    48706920:	540000c1 	b.ne	48706938 <ddr_sw_training_func+0xa0>  // b.any
    48706924:	94000019 	bl	48706988 <ddr_training_suc>
    48706928:	52800000 	mov	w0, #0x0                   	// #0
    4870692c:	f9400bf3 	ldr	x19, [sp, #16]
    48706930:	a8ce7bfd 	ldp	x29, x30, [sp], #224
    48706934:	d65f03c0 	ret
    48706938:	d2800000 	mov	x0, #0x0                   	// #0
    4870693c:	97ffffa3 	bl	487067c8 <ddr_training_console_if>
    48706940:	2a1303e0 	mov	w0, w19
    48706944:	17fffffa 	b	4870692c <ddr_sw_training_func+0x94>
    48706948:	d65f03c0 	ret

000000004870694c <ddr_hw_training_func>:
    4870694c:	a9b37bfd 	stp	x29, x30, [sp, #-208]!
    48706950:	910003fd 	mov	x29, sp
    48706954:	910043a0 	add	x0, x29, #0x10
    48706958:	97fff7c0 	bl	48704858 <ddr_training_cfg_init>
    4870695c:	910043a0 	add	x0, x29, #0x10
    48706960:	97fffc91 	bl	48705ba4 <ddr_hw_training>
    48706964:	a8cd7bfd 	ldp	x29, x30, [sp], #208
    48706968:	d65f03c0 	ret

000000004870696c <ddr_sw_training_if>:
    4870696c:	17ffffcb 	b	48706898 <ddr_sw_training_func>

0000000048706970 <ddr_hw_training_if>:
    48706970:	17fffff7 	b	4870694c <ddr_hw_training_func>

0000000048706974 <ddr_result_data_save>:
    48706974:	d65f03c0 	ret

0000000048706978 <ddr_lpca_data_save>:
    48706978:	d65f03c0 	ret

000000004870697c <ddr_ddrt_get_test_addr>:
    4870697c:	52a80000 	mov	w0, #0x40000000            	// #1073741824
    48706980:	d65f03c0 	ret

0000000048706984 <ddr_training_error>:
    48706984:	d65f03c0 	ret

0000000048706988 <ddr_training_suc>:
    48706988:	d65f03c0 	ret

000000004870698c <ddr_training_start>:
    4870698c:	d65f03c0 	ret

0000000048706990 <ddr_cmd_prepare_copy>:
    48706990:	d65f03c0 	ret

0000000048706994 <ddr_cmd_site_save>:
    48706994:	d2802981 	mov	x1, #0x14c                 	// #332
    48706998:	f2a24021 	movk	x1, #0x1201, lsl #16
    4870699c:	b9400020 	ldr	w0, [x1]
    487069a0:	321f0000 	orr	w0, w0, #0x2
    487069a4:	b9000020 	str	w0, [x1]
    487069a8:	d503201f 	nop
    487069ac:	b9400020 	ldr	w0, [x1]
    487069b0:	121f7800 	and	w0, w0, #0xfffffffe
    487069b4:	b9000020 	str	w0, [x1]
    487069b8:	d65f03c0 	ret

00000000487069bc <ddr_cmd_site_restore>:
    487069bc:	d2802981 	mov	x1, #0x14c                 	// #332
    487069c0:	f2a24021 	movk	x1, #0x1201, lsl #16
    487069c4:	b9400020 	ldr	w0, [x1]
    487069c8:	32000000 	orr	w0, w0, #0x1
    487069cc:	b9000020 	str	w0, [x1]
    487069d0:	d503201f 	nop
    487069d4:	b9400020 	ldr	w0, [x1]
    487069d8:	121e7800 	and	w0, w0, #0xfffffffd
    487069dc:	b9000020 	str	w0, [x1]
    487069e0:	d65f03c0 	ret
	...

00000000487069f0 <uart_early_init>:
    487069f0:	580002c4 	ldr	x4, 48706a48 <io_base_addr_L0>
    487069f4:	18000623 	ldr	w3, 48706ab8 <uart_base_addr_L3+0x8>
    487069f8:	b900d483 	str	w3, [x4, #212]
    487069fc:	18000603 	ldr	w3, 48706abc <uart_base_addr_L3+0xc>
    48706a00:	b900d883 	str	w3, [x4, #216]
    48706a04:	580001e4 	ldr	x4, 48706a40 <uart_base_addr_L0>
    48706a08:	52800003 	mov	w3, #0x0                   	// #0
    48706a0c:	b9003083 	str	w3, [x4, #48]
    48706a10:	11003463 	add	w3, w3, #0xd
    48706a14:	b9002483 	str	w3, [x4, #36]
    48706a18:	52800023 	mov	w3, #0x1                   	// #1
    48706a1c:	b9002883 	str	w3, [x4, #40]
    48706a20:	18000503 	ldr	w3, 48706ac0 <uart_base_addr_L3+0x10>
    48706a24:	b9002c83 	str	w3, [x4, #44]
    48706a28:	180004e3 	ldr	w3, 48706ac4 <uart_base_addr_L3+0x14>
    48706a2c:	b9003083 	str	w3, [x4, #48]
    48706a30:	d65f03c0 	ret
    48706a34:	d503201f 	nop
    48706a38:	d503201f 	nop
    48706a3c:	d503201f 	nop

0000000048706a40 <uart_base_addr_L0>:
    48706a40:	12100000 	.word	0x12100000
    48706a44:	00000000 	.word	0x00000000

0000000048706a48 <io_base_addr_L0>:
    48706a48:	1f000000 	.word	0x1f000000
    48706a4c:	00000000 	.word	0x00000000

0000000048706a50 <uart_early_puts>:
    48706a50:	58000182 	ldr	x2, 48706a80 <uart_base_addr_L1>
    48706a54:	14000006 	b	48706a6c <next_char>

0000000048706a58 <output>:
    48706a58:	b9401844 	ldr	w4, [x2, #24]
    48706a5c:	721b009f 	tst	w4, #0x20
    48706a60:	54ffffc1 	b.ne	48706a58 <output>  // b.any
    48706a64:	b9000043 	str	w3, [x2]
    48706a68:	91000400 	add	x0, x0, #0x1

0000000048706a6c <next_char>:
    48706a6c:	39400003 	ldrb	w3, [x0]
    48706a70:	7100007f 	cmp	w3, #0x0
    48706a74:	54ffff21 	b.ne	48706a58 <output>  // b.any
    48706a78:	d65f03c0 	ret
    48706a7c:	d503201f 	nop

0000000048706a80 <uart_base_addr_L1>:
    48706a80:	12100000 	.word	0x12100000
    48706a84:	00000000 	.word	0x00000000
    48706a88:	d503201f 	nop
    48706a8c:	d503201f 	nop

0000000048706a90 <uart_early_putc>:
    48706a90:	58000102 	ldr	x2, 48706ab0 <uart_base_addr_L3>

0000000048706a94 <wait3>:
    48706a94:	b9401844 	ldr	w4, [x2, #24]
    48706a98:	721b009f 	tst	w4, #0x20
    48706a9c:	54ffffc1 	b.ne	48706a94 <wait3>  // b.any
    48706aa0:	b9000040 	str	w0, [x2]
    48706aa4:	d65f03c0 	ret
    48706aa8:	d503201f 	nop
    48706aac:	d503201f 	nop

0000000048706ab0 <uart_base_addr_L3>:
    48706ab0:	12100000 	.word	0x12100000
    48706ab4:	00000000 	.word	0x00000000
    48706ab8:	00001d01 	.word	0x00001d01
    48706abc:	00000401 	.word	0x00000401
    48706ac0:	00000070 	.word	0x00000070
    48706ac4:	00000301 	.word	0x00000301

0000000048706ac8 <wait_for_cmd_completion>:
    48706ac8:	51000403 	sub	w3, w0, #0x1
    48706acc:	52800022 	mov	w2, #0x1                   	// #1
    48706ad0:	7100807f 	cmp	w3, #0x20
    48706ad4:	d2800b03 	mov	x3, #0x58                  	// #88
    48706ad8:	1a823000 	csel	w0, w0, w2, cc  // cc = lo, ul, last
    48706adc:	f2a20023 	movk	x3, #0x1001, lsl #16
    48706ae0:	51000400 	sub	w0, w0, #0x1
    48706ae4:	aa0303e4 	mov	x4, x3
    48706ae8:	1ac02040 	lsl	w0, w2, w0
    48706aec:	b00002a2 	adrp	x2, 4875b000 <input_data+0x4b000>
    48706af0:	b9000060 	str	w0, [x3]
    48706af4:	b90da85f 	str	wzr, [x2, #3496]
    48706af8:	9136a042 	add	x2, x2, #0xda8
    48706afc:	b9400083 	ldr	w3, [x4]
    48706b00:	6a03001f 	tst	w0, w3
    48706b04:	54000220 	b.eq	48706b48 <wait_for_cmd_completion+0x80>  // b.none
    48706b08:	52884823 	mov	w3, #0x4241                	// #16961
    48706b0c:	72a001e3 	movk	w3, #0xf, lsl #16
    48706b10:	71000463 	subs	w3, w3, #0x1
    48706b14:	54000161 	b.ne	48706b40 <wait_for_cmd_completion+0x78>  // b.any
    48706b18:	b9400043 	ldr	w3, [x2]
    48706b1c:	11000463 	add	w3, w3, #0x1
    48706b20:	6b03003f 	cmp	w1, w3
    48706b24:	b9000043 	str	w3, [x2]
    48706b28:	54fffea2 	b.cs	48706afc <wait_for_cmd_completion+0x34>  // b.hs, b.nlast
    48706b2c:	d2800b80 	mov	x0, #0x5c                  	// #92
    48706b30:	f2a20020 	movk	x0, #0x1001, lsl #16
    48706b34:	b900001f 	str	wzr, [x0]
    48706b38:	12800040 	mov	w0, #0xfffffffd            	// #-3
    48706b3c:	d65f03c0 	ret
    48706b40:	d503201f 	nop
    48706b44:	17fffff3 	b	48706b10 <wait_for_cmd_completion+0x48>
    48706b48:	52800000 	mov	w0, #0x0                   	// #0
    48706b4c:	d65f03c0 	ret

0000000048706b50 <handle_scsi_completion>:
    48706b50:	b00002a1 	adrp	x1, 4875b000 <input_data+0x4b000>
    48706b54:	51000400 	sub	w0, w0, #0x1
    48706b58:	f946d821 	ldr	x1, [x1, #3504]
    48706b5c:	8b001420 	add	x0, x1, x0, lsl #5
    48706b60:	39602000 	ldrb	w0, [x0, #2056]
    48706b64:	35000400 	cbnz	w0, 48706be4 <handle_scsi_completion+0x94>
    48706b68:	91080022 	add	x2, x1, #0x200
    48706b6c:	39481c21 	ldrb	w1, [x1, #519]
    48706b70:	340000e1 	cbz	w1, 48706b8c <handle_scsi_completion+0x3c>
    48706b74:	7100083f 	cmp	w1, #0x2
    48706b78:	540000c1 	b.ne	48706b90 <handle_scsi_completion+0x40>  // b.any
    48706b7c:	39409040 	ldrb	w0, [x2, #36]
    48706b80:	12000c01 	and	w1, w0, #0xf
    48706b84:	128003e0 	mov	w0, #0xffffffe0            	// #-32
    48706b88:	4b010000 	sub	w0, w0, w1
    48706b8c:	d65f03c0 	ret
    48706b90:	7100603f 	cmp	w1, #0x18
    48706b94:	128002a0 	mov	w0, #0xffffffea            	// #-22
    48706b98:	54ffffa0 	b.eq	48706b8c <handle_scsi_completion+0x3c>  // b.none
    48706b9c:	54000128 	b.hi	48706bc0 <handle_scsi_completion+0x70>  // b.pmore
    48706ba0:	7100103f 	cmp	w1, #0x4
    48706ba4:	12800260 	mov	w0, #0xffffffec            	// #-20
    48706ba8:	54ffff20 	b.eq	48706b8c <handle_scsi_completion+0x3c>  // b.none
    48706bac:	7100203f 	cmp	w1, #0x8
    48706bb0:	12800280 	mov	w0, #0xffffffeb            	// #-21
    48706bb4:	54fffec0 	b.eq	48706b8c <handle_scsi_completion+0x3c>  // b.none
    48706bb8:	128003c0 	mov	w0, #0xffffffe1            	// #-31
    48706bbc:	d65f03c0 	ret
    48706bc0:	7100c03f 	cmp	w1, #0x30
    48706bc4:	128002e0 	mov	w0, #0xffffffe8            	// #-24
    48706bc8:	54fffe20 	b.eq	48706b8c <handle_scsi_completion+0x3c>  // b.none
    48706bcc:	7101003f 	cmp	w1, #0x40
    48706bd0:	12800300 	mov	w0, #0xffffffe7            	// #-25
    48706bd4:	54fffdc0 	b.eq	48706b8c <handle_scsi_completion+0x3c>  // b.none
    48706bd8:	128002c0 	mov	w0, #0xffffffe9            	// #-23
    48706bdc:	7100a03f 	cmp	w1, #0x28
    48706be0:	17fffff5 	b	48706bb4 <handle_scsi_completion+0x64>
    48706be4:	12000c01 	and	w1, w0, #0xf
    48706be8:	128015e0 	mov	w0, #0xffffff50            	// #-176
    48706bec:	17ffffe7 	b	48706b88 <handle_scsi_completion+0x38>

0000000048706bf0 <send_uic_command.constprop.4>:
    48706bf0:	b00002a2 	adrp	x2, 4875b000 <input_data+0x4b000>
    48706bf4:	d2800605 	mov	x5, #0x30                  	// #48
    48706bf8:	9136a044 	add	x4, x2, #0xda8
    48706bfc:	f2a20025 	movk	x5, #0x1001, lsl #16
    48706c00:	b90da85f 	str	wzr, [x2, #3496]
    48706c04:	b94000a3 	ldr	w3, [x5]
    48706c08:	37180143 	tbnz	w3, #3, 48706c30 <send_uic_command.constprop.4+0x40>
    48706c0c:	52884823 	mov	w3, #0x4241                	// #16961
    48706c10:	72a001e3 	movk	w3, #0xf, lsl #16
    48706c14:	71000463 	subs	w3, w3, #0x1
    48706c18:	54000601 	b.ne	48706cd8 <send_uic_command.constprop.4+0xe8>  // b.any
    48706c1c:	b9400083 	ldr	w3, [x4]
    48706c20:	11000463 	add	w3, w3, #0x1
    48706c24:	7100047f 	cmp	w3, #0x1
    48706c28:	b9000083 	str	w3, [x4]
    48706c2c:	54fffec9 	b.ls	48706c04 <send_uic_command.constprop.4+0x14>  // b.plast
    48706c30:	d2800406 	mov	x6, #0x20                  	// #32
    48706c34:	d2801288 	mov	x8, #0x94                  	// #148
    48706c38:	d2801307 	mov	x7, #0x98                  	// #152
    48706c3c:	d280138a 	mov	x10, #0x9c                  	// #156
    48706c40:	d280120b 	mov	x11, #0x90                  	// #144
    48706c44:	9136a042 	add	x2, x2, #0xda8
    48706c48:	52800145 	mov	w5, #0xa                   	// #10
    48706c4c:	52800003 	mov	w3, #0x0                   	// #0
    48706c50:	f2a20026 	movk	x6, #0x1001, lsl #16
    48706c54:	1280000c 	mov	w12, #0xffffffff            	// #-1
    48706c58:	f2a20028 	movk	x8, #0x1001, lsl #16
    48706c5c:	f2a20027 	movk	x7, #0x1001, lsl #16
    48706c60:	f2a2002a 	movk	x10, #0x1001, lsl #16
    48706c64:	f2a2002b 	movk	x11, #0x1001, lsl #16
    48706c68:	5280004d 	mov	w13, #0x2                   	// #2
    48706c6c:	b90000cc 	str	w12, [x6]
    48706c70:	b9000100 	str	w0, [x8]
    48706c74:	b90000ff 	str	wzr, [x7]
    48706c78:	b9000141 	str	w1, [x10]
    48706c7c:	b900016d 	str	w13, [x11]
    48706c80:	b900005f 	str	wzr, [x2]
    48706c84:	b94000c4 	ldr	w4, [x6]
    48706c88:	375002c4 	tbnz	w4, #10, 48706ce0 <send_uic_command.constprop.4+0xf0>
    48706c8c:	52884824 	mov	w4, #0x4241                	// #16961
    48706c90:	72a001e4 	movk	w4, #0xf, lsl #16
    48706c94:	71000484 	subs	w4, w4, #0x1
    48706c98:	540003e1 	b.ne	48706d14 <send_uic_command.constprop.4+0x124>  // b.any
    48706c9c:	b9400044 	ldr	w4, [x2]
    48706ca0:	11000484 	add	w4, w4, #0x1
    48706ca4:	7100289f 	cmp	w4, #0xa
    48706ca8:	b9000044 	str	w4, [x2]
    48706cac:	54fffec9 	b.ls	48706c84 <send_uic_command.constprop.4+0x94>  // b.plast
    48706cb0:	710004a5 	subs	w5, w5, #0x1
    48706cb4:	54fffdc1 	b.ne	48706c6c <send_uic_command.constprop.4+0x7c>  // b.any
    48706cb8:	d2800400 	mov	x0, #0x20                  	// #32
    48706cbc:	12800001 	mov	w1, #0xffffffff            	// #-1
    48706cc0:	f2a20020 	movk	x0, #0x1001, lsl #16
    48706cc4:	12000c63 	and	w3, w3, #0xf
    48706cc8:	b9000001 	str	w1, [x0]
    48706ccc:	128013e0 	mov	w0, #0xffffff60            	// #-160
    48706cd0:	4b030000 	sub	w0, w0, w3
    48706cd4:	d65f03c0 	ret
    48706cd8:	d503201f 	nop
    48706cdc:	17ffffce 	b	48706c14 <send_uic_command.constprop.4+0x24>
    48706ce0:	b94000e3 	ldr	w3, [x7]
    48706ce4:	72001c63 	ands	w3, w3, #0xff
    48706ce8:	540000e0 	b.eq	48706d04 <send_uic_command.constprop.4+0x114>  // b.none
    48706cec:	52884824 	mov	w4, #0x4241                	// #16961
    48706cf0:	72a001e4 	movk	w4, #0xf, lsl #16
    48706cf4:	71000484 	subs	w4, w4, #0x1
    48706cf8:	54fffdc0 	b.eq	48706cb0 <send_uic_command.constprop.4+0xc0>  // b.none
    48706cfc:	d503201f 	nop
    48706d00:	17fffffd 	b	48706cf4 <send_uic_command.constprop.4+0x104>
    48706d04:	12800000 	mov	w0, #0xffffffff            	// #-1
    48706d08:	b90000c0 	str	w0, [x6]
    48706d0c:	52800000 	mov	w0, #0x0                   	// #0
    48706d10:	d65f03c0 	ret
    48706d14:	d503201f 	nop
    48706d18:	17ffffdf 	b	48706c94 <send_uic_command.constprop.4+0xa4>

0000000048706d1c <create_cmd_upiu.constprop.3>:
    48706d1c:	7100a01f 	cmp	w0, #0x28
    48706d20:	2a0003e8 	mov	w8, w0
    48706d24:	54000081 	b.ne	48706d34 <create_cmd_upiu.constprop.3+0x18>  // b.any
    48706d28:	f2402c7f 	tst	x3, #0xfff
    48706d2c:	12801fc0 	mov	w0, #0xffffff01            	// #-255
    48706d30:	54001041 	b.ne	48706f38 <create_cmd_upiu.constprop.3+0x21c>  // b.any
    48706d34:	b00002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48706d38:	7100083f 	cmp	w1, #0x2
    48706d3c:	f946d805 	ldr	x5, [x0, #3504]
    48706d40:	51000480 	sub	w0, w4, #0x1
    48706d44:	912000a6 	add	x6, x5, #0x800
    48706d48:	8b0014c6 	add	x6, x6, x0, lsl #5
    48706d4c:	540009c0 	b.eq	48706e84 <create_cmd_upiu.constprop.3+0x168>  // b.none
    48706d50:	71000c3f 	cmp	w1, #0x3
    48706d54:	12801fc0 	mov	w0, #0xffffff01            	// #-255
    48706d58:	54000f01 	b.ne	48706f38 <create_cmd_upiu.constprop.3+0x21c>  // b.any
    48706d5c:	5280000a 	mov	w10, #0x0                   	// #0
    48706d60:	52800000 	mov	w0, #0x0                   	// #0
    48706d64:	9109a0ab 	add	x11, x5, #0x268
    48706d68:	910800a7 	add	x7, x5, #0x200
    48706d6c:	380024ff 	strb	wzr, [x7], #2
    48706d70:	eb0b00ff 	cmp	x7, x11
    48706d74:	54ffffc1 	b.ne	48706d6c <create_cmd_upiu.constprop.3+0x50>  // b.any
    48706d78:	321c0000 	orr	w0, w0, #0x10
    48706d7c:	390064df 	strb	wzr, [x6, #25]
    48706d80:	39000cc0 	strb	w0, [x6, #3]
    48706d84:	528001a0 	mov	w0, #0xd                   	// #13
    48706d88:	390060c0 	strb	w0, [x6, #24]
    48706d8c:	528001e0 	mov	w0, #0xf                   	// #15
    48706d90:	390020c0 	strb	w0, [x6, #8]
    48706d94:	12bfff80 	mov	w0, #0x3ffff               	// #262143
    48706d98:	0b000067 	add	w7, w3, w0
    48706d9c:	52800020 	mov	w0, #0x1                   	// #1
    48706da0:	53127ce7 	lsr	w7, w7, #18
    48706da4:	390070c7 	strb	w7, [x6, #28]
    48706da8:	d34834e7 	ubfx	x7, x7, #8, #6
    48706dac:	390074c7 	strb	w7, [x6, #29]
    48706db0:	390000a0 	strb	w0, [x5]
    48706db4:	128009e0 	mov	w0, #0xffffffb0            	// #-80
    48706db8:	39000ca4 	strb	w4, [x5, #3]
    48706dbc:	5ac00864 	rev	w4, w3
    48706dc0:	390008a0 	strb	w0, [x5, #2]
    48706dc4:	d3483c80 	ubfx	x0, x4, #8, #8
    48706dc8:	390030a4 	strb	w4, [x5, #12]
    48706dcc:	390034a0 	strb	w0, [x5, #13]
    48706dd0:	d3505c80 	ubfx	x0, x4, #16, #8
    48706dd4:	d3587c84 	ubfx	x4, x4, #24, #8
    48706dd8:	390038a0 	strb	w0, [x5, #14]
    48706ddc:	39003ca4 	strb	w4, [x5, #15]
    48706de0:	910040a4 	add	x4, x5, #0x10
    48706de4:	390004aa 	strb	w10, [x5, #1]
    48706de8:	d2800000 	mov	x0, #0x0                   	// #0
    48706dec:	390010bf 	strb	wzr, [x5, #4]
    48706df0:	390014bf 	strb	wzr, [x5, #5]
    48706df4:	390018bf 	strb	wzr, [x5, #6]
    48706df8:	39001cbf 	strb	wzr, [x5, #7]
    48706dfc:	390020bf 	strb	wzr, [x5, #8]
    48706e00:	390024bf 	strb	wzr, [x5, #9]
    48706e04:	390028bf 	strb	wzr, [x5, #10]
    48706e08:	39002cbf 	strb	wzr, [x5, #11]
    48706e0c:	3820689f 	strb	wzr, [x4, x0]
    48706e10:	91000800 	add	x0, x0, #0x2
    48706e14:	f100801f 	cmp	x0, #0x20
    48706e18:	54ffffa1 	b.ne	48706e0c <create_cmd_upiu.constprop.3+0xf0>  // b.any
    48706e1c:	340003a8 	cbz	w8, 48706e90 <create_cmd_upiu.constprop.3+0x174>
    48706e20:	7100a11f 	cmp	w8, #0x28
    48706e24:	54000861 	b.ne	48706f30 <create_cmd_upiu.constprop.3+0x214>  // b.any
    48706e28:	53147c60 	lsr	w0, w3, #20
    48706e2c:	390040a8 	strb	w8, [x5, #16]
    48706e30:	390044bf 	strb	wzr, [x5, #17]
    48706e34:	39005ca0 	strb	w0, [x5, #23]
    48706e38:	530c7c60 	lsr	w0, w3, #12
    48706e3c:	390048bf 	strb	wzr, [x5, #18]
    48706e40:	39004cbf 	strb	wzr, [x5, #19]
    48706e44:	390050bf 	strb	wzr, [x5, #20]
    48706e48:	390054bf 	strb	wzr, [x5, #21]
    48706e4c:	390058bf 	strb	wzr, [x5, #22]
    48706e50:	390060a0 	strb	w0, [x5, #24]
    48706e54:	390064bf 	strb	wzr, [x5, #25]
    48706e58:	71000c3f 	cmp	w1, #0x3
    48706e5c:	54000660 	b.eq	48706f28 <create_cmd_upiu.constprop.3+0x20c>  // b.none
    48706e60:	d360fc40 	lsr	x0, x2, #32
    48706e64:	52800006 	mov	w6, #0x0                   	// #0
    48706e68:	911000a5 	add	x5, x5, #0x400
    48706e6c:	d3483c0b 	ubfx	x11, x0, #8, #8
    48706e70:	53001c0c 	uxtb	w12, w0
    48706e74:	d3505c0a 	ubfx	x10, x0, #16, #8
    48706e78:	53187c01 	lsr	w1, w0, #24
    48706e7c:	52a00088 	mov	w8, #0x40000               	// #262144
    48706e80:	14000029 	b	48706f24 <create_cmd_upiu.constprop.3+0x208>
    48706e84:	5280080a 	mov	w10, #0x40                  	// #64
    48706e88:	52800080 	mov	w0, #0x4                   	// #4
    48706e8c:	17ffffb6 	b	48706d64 <create_cmd_upiu.constprop.3+0x48>
    48706e90:	390040bf 	strb	wzr, [x5, #16]
    48706e94:	390044bf 	strb	wzr, [x5, #17]
    48706e98:	390048bf 	strb	wzr, [x5, #18]
    48706e9c:	39004cbf 	strb	wzr, [x5, #19]
    48706ea0:	390050bf 	strb	wzr, [x5, #20]
    48706ea4:	390054bf 	strb	wzr, [x5, #21]
    48706ea8:	17ffffec 	b	48706e58 <create_cmd_upiu.constprop.3+0x13c>
    48706eac:	d37c7cc7 	ubfiz	x7, x6, #4, #32
    48706eb0:	0b064844 	add	w4, w2, w6, lsl #18
    48706eb4:	8b0700a0 	add	x0, x5, x7
    48706eb8:	7141007f 	cmp	w3, #0x40, lsl #12
    48706ebc:	110004c6 	add	w6, w6, #0x1
    48706ec0:	382768a4 	strb	w4, [x5, x7]
    48706ec4:	d3483c87 	ubfx	x7, x4, #8, #8
    48706ec8:	39000407 	strb	w7, [x0, #1]
    48706ecc:	d3505c87 	ubfx	x7, x4, #16, #8
    48706ed0:	39000807 	strb	w7, [x0, #2]
    48706ed4:	d3587c84 	ubfx	x4, x4, #24, #8
    48706ed8:	1a889067 	csel	w7, w3, w8, ls  // ls = plast
    48706edc:	39000c04 	strb	w4, [x0, #3]
    48706ee0:	510004e4 	sub	w4, w7, #0x1
    48706ee4:	4b070063 	sub	w3, w3, w7
    48706ee8:	d3483c8d 	ubfx	x13, x4, #8, #8
    48706eec:	39003004 	strb	w4, [x0, #12]
    48706ef0:	3900340d 	strb	w13, [x0, #13]
    48706ef4:	d3505c8d 	ubfx	x13, x4, #16, #8
    48706ef8:	d3587c84 	ubfx	x4, x4, #24, #8
    48706efc:	3900100c 	strb	w12, [x0, #4]
    48706f00:	3900140b 	strb	w11, [x0, #5]
    48706f04:	3900180a 	strb	w10, [x0, #6]
    48706f08:	39001c01 	strb	w1, [x0, #7]
    48706f0c:	3900201f 	strb	wzr, [x0, #8]
    48706f10:	3900241f 	strb	wzr, [x0, #9]
    48706f14:	3900281f 	strb	wzr, [x0, #10]
    48706f18:	39002c1f 	strb	wzr, [x0, #11]
    48706f1c:	3900380d 	strb	w13, [x0, #14]
    48706f20:	39003c04 	strb	w4, [x0, #15]
    48706f24:	35fffc43 	cbnz	w3, 48706eac <create_cmd_upiu.constprop.3+0x190>
    48706f28:	52800000 	mov	w0, #0x0                   	// #0
    48706f2c:	d65f03c0 	ret
    48706f30:	12801fc0 	mov	w0, #0xffffff01            	// #-255
    48706f34:	d65f03c0 	ret
    48706f38:	d65f03c0 	ret

0000000048706f3c <ufs_boot_read>:
    48706f3c:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
    48706f40:	910003fd 	mov	x29, sp
    48706f44:	a90153f3 	stp	x19, x20, [sp, #16]
    48706f48:	2a0103f4 	mov	w20, w1
    48706f4c:	aa0003f3 	mov	x19, x0
    48706f50:	a9025bf5 	stp	x21, x22, [sp, #32]
    48706f54:	8b344401 	add	x1, x0, w20, uxtw #1
    48706f58:	eb01001f 	cmp	x0, x1
    48706f5c:	54001081 	b.ne	4870716c <ufs_boot_read+0x230>  // b.any
    48706f60:	52800001 	mov	w1, #0x0                   	// #0
    48706f64:	52a40880 	mov	w0, #0x20440000            	// #541327360
    48706f68:	97ffff22 	bl	48706bf0 <send_uic_command.constprop.4>
    48706f6c:	52800001 	mov	w1, #0x0                   	// #0
    48706f70:	52a408a0 	mov	w0, #0x20450000            	// #541392896
    48706f74:	97ffff1f 	bl	48706bf0 <send_uic_command.constprop.4>
    48706f78:	52800121 	mov	w1, #0x9                   	// #9
    48706f7c:	52a40800 	mov	w0, #0x20400000            	// #541065216
    48706f80:	97ffff1c 	bl	48706bf0 <send_uic_command.constprop.4>
    48706f84:	b00002ae 	adrp	x14, 4875b000 <input_data+0x4b000>
    48706f88:	d2aa0000 	mov	x0, #0x50000000            	// #1342177280
    48706f8c:	d2880001 	mov	x1, #0x4000                	// #16384
    48706f90:	f2aa0001 	movk	x1, #0x5000, lsl #16
    48706f94:	f906d9c0 	str	x0, [x14, #3504]
    48706f98:	3800241f 	strb	wzr, [x0], #2
    48706f9c:	eb01001f 	cmp	x0, x1
    48706fa0:	54ffffc1 	b.ne	48706f98 <ufs_boot_read+0x5c>  // b.any
    48706fa4:	f946d9c0 	ldr	x0, [x14, #3504]
    48706fa8:	910ffc00 	add	x0, x0, #0x3ff
    48706fac:	9276d400 	and	x0, x0, #0xfffffffffffffc00
    48706fb0:	f906d9c0 	str	x0, [x14, #3504]
    48706fb4:	b4000600 	cbz	x0, 48707074 <ufs_boot_read+0x138>
    48706fb8:	91200001 	add	x1, x0, #0x800
    48706fbc:	91400403 	add	x3, x0, #0x1, lsl #12
    48706fc0:	aa0103e2 	mov	x2, x1
    48706fc4:	3800245f 	strb	wzr, [x2], #2
    48706fc8:	eb02007f 	cmp	x3, x2
    48706fcc:	54ffffc1 	b.ne	48706fc4 <ufs_boot_read+0x88>  // b.any
    48706fd0:	d5033fbf 	dmb	sy
    48706fd4:	f946d9c2 	ldr	x2, [x14, #3504]
    48706fd8:	52800023 	mov	w3, #0x1                   	// #1
    48706fdc:	91300000 	add	x0, x0, #0xc00
    48706fe0:	52801004 	mov	w4, #0x80                  	// #128
    48706fe4:	b9100043 	str	w3, [x2, #4096]
    48706fe8:	d2800a03 	mov	x3, #0x50                  	// #80
    48706fec:	f2a20023 	movk	x3, #0x1001, lsl #16
    48706ff0:	f946d9c2 	ldr	x2, [x14, #3504]
    48706ff4:	11200042 	add	w2, w2, #0x800
    48706ff8:	b9000062 	str	w2, [x3]
    48706ffc:	d2800a82 	mov	x2, #0x54                  	// #84
    48707000:	f2a20022 	movk	x2, #0x1001, lsl #16
    48707004:	52802003 	mov	w3, #0x100                 	// #256
    48707008:	b900005f 	str	wzr, [x2]
    4870700c:	f946d9c2 	ldr	x2, [x14, #3504]
    48707010:	2a0203e5 	mov	w5, w2
    48707014:	79003424 	strh	w4, [x1, #26]
    48707018:	79003c23 	strh	w3, [x1, #30]
    4870701c:	29027c25 	stp	w5, wzr, [x1, #16]
    48707020:	91008021 	add	x1, x1, #0x20
    48707024:	eb01001f 	cmp	x0, x1
    48707028:	54ffff61 	b.ne	48707014 <ufs_boot_read+0xd8>  // b.any
    4870702c:	91328041 	add	x1, x2, #0xca0
    48707030:	91300040 	add	x0, x2, #0xc00
    48707034:	3800241f 	strb	wzr, [x0], #2
    48707038:	eb00003f 	cmp	x1, x0
    4870703c:	54ffffc1 	b.ne	48707034 <ufs_boot_read+0xf8>  // b.any
    48707040:	d2800e01 	mov	x1, #0x70                  	// #112
    48707044:	f946d9c0 	ldr	x0, [x14, #3504]
    48707048:	f2a20021 	movk	x1, #0x1001, lsl #16
    4870704c:	11300000 	add	w0, w0, #0xc00
    48707050:	b9000020 	str	w0, [x1]
    48707054:	d2800e80 	mov	x0, #0x74                  	// #116
    48707058:	f2a20020 	movk	x0, #0x1001, lsl #16
    4870705c:	d2800c01 	mov	x1, #0x60                  	// #96
    48707060:	f2a20021 	movk	x1, #0x1001, lsl #16
    48707064:	b900001f 	str	wzr, [x0]
    48707068:	52800020 	mov	w0, #0x1                   	// #1
    4870706c:	b9000020 	str	w0, [x1]
    48707070:	b9002020 	str	w0, [x1, #32]
    48707074:	f946d9c0 	ldr	x0, [x14, #3504]
    48707078:	b9500005 	ldr	w5, [x0, #4096]
    4870707c:	d5033fbf 	dmb	sy
    48707080:	52800067 	mov	w7, #0x3                   	// #3
    48707084:	5280002c 	mov	w12, #0x1                   	// #1
    48707088:	52800208 	mov	w8, #0x10                  	// #16
    4870708c:	5280010a 	mov	w10, #0x8                   	// #8
    48707090:	528001eb 	mov	w11, #0xf                   	// #15
    48707094:	510004a0 	sub	w0, w5, #0x1
    48707098:	7100801f 	cmp	w0, #0x20
    4870709c:	f946d9c0 	ldr	x0, [x14, #3504]
    487070a0:	1a8c30a5 	csel	w5, w5, w12, cc  // cc = lo, ul, last
    487070a4:	510004a6 	sub	w6, w5, #0x1
    487070a8:	91200001 	add	x1, x0, #0x800
    487070ac:	91090003 	add	x3, x0, #0x240
    487070b0:	91080002 	add	x2, x0, #0x200
    487070b4:	d37be8c6 	lsl	x6, x6, #5
    487070b8:	8b060021 	add	x1, x1, x6
    487070bc:	3800245f 	strb	wzr, [x2], #2
    487070c0:	eb02007f 	cmp	x3, x2
    487070c4:	54ffffc1 	b.ne	487070bc <ufs_boot_read+0x180>  // b.any
    487070c8:	39000c28 	strb	w8, [x1, #3]
    487070cc:	3900602a 	strb	w10, [x1, #24]
    487070d0:	3900643f 	strb	wzr, [x1, #25]
    487070d4:	3900703f 	strb	wzr, [x1, #28]
    487070d8:	3900743f 	strb	wzr, [x1, #29]
    487070dc:	3900202b 	strb	w11, [x1, #8]
    487070e0:	d2800001 	mov	x1, #0x0                   	// #0
    487070e4:	3900001f 	strb	wzr, [x0]
    487070e8:	3900041f 	strb	wzr, [x0, #1]
    487070ec:	3900081f 	strb	wzr, [x0, #2]
    487070f0:	39000c05 	strb	w5, [x0, #3]
    487070f4:	3900101f 	strb	wzr, [x0, #4]
    487070f8:	3900141f 	strb	wzr, [x0, #5]
    487070fc:	3900181f 	strb	wzr, [x0, #6]
    48707100:	39001c1f 	strb	wzr, [x0, #7]
    48707104:	3900201f 	strb	wzr, [x0, #8]
    48707108:	3900241f 	strb	wzr, [x0, #9]
    4870710c:	3900281f 	strb	wzr, [x0, #10]
    48707110:	39002c1f 	strb	wzr, [x0, #11]
    48707114:	8b010002 	add	x2, x0, x1
    48707118:	91000421 	add	x1, x1, #0x1
    4870711c:	f100503f 	cmp	x1, #0x14
    48707120:	3900305f 	strb	wzr, [x2, #12]
    48707124:	54ffff81 	b.ne	48707114 <ufs_boot_read+0x1d8>  // b.any
    48707128:	2a0503e0 	mov	w0, w5
    4870712c:	52800141 	mov	w1, #0xa                   	// #10
    48707130:	97fffe66 	bl	48706ac8 <wait_for_cmd_completion>
    48707134:	35000100 	cbnz	w0, 48707154 <ufs_boot_read+0x218>
    48707138:	f946d9c1 	ldr	x1, [x14, #3504]
    4870713c:	8b0100c6 	add	x6, x6, x1
    48707140:	396020c0 	ldrb	w0, [x6, #2056]
    48707144:	34000180 	cbz	w0, 48707174 <ufs_boot_read+0x238>
    48707148:	12000c01 	and	w1, w0, #0xf
    4870714c:	128015e0 	mov	w0, #0xffffff50            	// #-176
    48707150:	4b010000 	sub	w0, w0, w1
    48707154:	52884821 	mov	w1, #0x4241                	// #16961
    48707158:	72a001e1 	movk	w1, #0xf, lsl #16
    4870715c:	71000421 	subs	w1, w1, #0x1
    48707160:	540001a0 	b.eq	48707194 <ufs_boot_read+0x258>  // b.none
    48707164:	d503201f 	nop
    48707168:	17fffffd 	b	4870715c <ufs_boot_read+0x220>
    4870716c:	3800241f 	strb	wzr, [x0], #2
    48707170:	17ffff7a 	b	48706f58 <ufs_boot_read+0x1c>
    48707174:	39480020 	ldrb	w0, [x1, #512]
    48707178:	12001400 	and	w0, w0, #0x3f
    4870717c:	7100801f 	cmp	w0, #0x20
    48707180:	54000b01 	b.ne	487072e0 <ufs_boot_read+0x3a4>  // b.any
    48707184:	39481820 	ldrb	w0, [x1, #518]
    48707188:	34000b00 	cbz	w0, 487072e8 <ufs_boot_read+0x3ac>
    4870718c:	12800060 	mov	w0, #0xfffffffc            	// #-4
    48707190:	17fffff1 	b	48707154 <ufs_boot_read+0x218>
    48707194:	31000c1f 	cmn	w0, #0x3
    48707198:	540000a1 	b.ne	487071ac <ufs_boot_read+0x270>  // b.any
    4870719c:	110004a5 	add	w5, w5, #0x1
    487071a0:	d5033fbf 	dmb	sy
    487071a4:	f946d9c0 	ldr	x0, [x14, #3504]
    487071a8:	b9100005 	str	w5, [x0, #4096]
    487071ac:	710004e7 	subs	w7, w7, #0x1
    487071b0:	54fff721 	b.ne	48707094 <ufs_boot_read+0x158>  // b.any
    487071b4:	f946d9c0 	ldr	x0, [x14, #3504]
    487071b8:	113ffe94 	add	w20, w20, #0xfff
    487071bc:	12144e94 	and	w20, w20, #0xfffff000
    487071c0:	b950000e 	ldr	w14, [x0, #4096]
    487071c4:	d5033fbf 	dmb	sy
    487071c8:	510005c0 	sub	w0, w14, #0x1
    487071cc:	5280064f 	mov	w15, #0x32                  	// #50
    487071d0:	71007c1f 	cmp	w0, #0x1f
    487071d4:	12bfe015 	mov	w21, #0xffffff              	// #16777215
    487071d8:	1a9f95ce 	csinc	w14, w14, wzr, ls  // ls = plast
    487071dc:	6b15029f 	cmp	w20, w21
    487071e0:	54000648 	b.hi	487072a8 <ufs_boot_read+0x36c>  // b.pmore
    487071e4:	2a0e03e4 	mov	w4, w14
    487071e8:	2a1403e3 	mov	w3, w20
    487071ec:	aa1303e2 	mov	x2, x19
    487071f0:	52800041 	mov	w1, #0x2                   	// #2
    487071f4:	52800500 	mov	w0, #0x28                  	// #40
    487071f8:	97fffec9 	bl	48706d1c <create_cmd_upiu.constprop.3>
    487071fc:	35000580 	cbnz	w0, 487072ac <ufs_boot_read+0x370>
    48707200:	2a0e03e0 	mov	w0, w14
    48707204:	52986a01 	mov	w1, #0xc350                	// #50000
    48707208:	97fffe30 	bl	48706ac8 <wait_for_cmd_completion>
    4870720c:	35000500 	cbnz	w0, 487072ac <ufs_boot_read+0x370>
    48707210:	2a0e03e0 	mov	w0, w14
    48707214:	97fffe4f 	bl	48706b50 <handle_scsi_completion>
    48707218:	350004a0 	cbnz	w0, 487072ac <ufs_boot_read+0x370>
    4870721c:	a94153f3 	ldp	x19, x20, [sp, #16]
    48707220:	a9425bf5 	ldp	x21, x22, [sp, #32]
    48707224:	a8c37bfd 	ldp	x29, x30, [sp], #48
    48707228:	d65f03c0 	ret
    4870722c:	31000c1f 	cmn	w0, #0x3
    48707230:	540000a1 	b.ne	48707244 <ufs_boot_read+0x308>  // b.any
    48707234:	110005ef 	add	w15, w15, #0x1
    48707238:	d5033fbf 	dmb	sy
    4870723c:	f946d9c0 	ldr	x0, [x14, #3504]
    48707240:	b910000f 	str	w15, [x0, #4096]
    48707244:	710006b5 	subs	w21, w21, #0x1
    48707248:	54fffb60 	b.eq	487071b4 <ufs_boot_read+0x278>  // b.none
    4870724c:	510005e0 	sub	w0, w15, #0x1
    48707250:	52800003 	mov	w3, #0x0                   	// #0
    48707254:	7100801f 	cmp	w0, #0x20
    48707258:	d2800002 	mov	x2, #0x0                   	// #0
    4870725c:	1a9631ef 	csel	w15, w15, w22, cc  // cc = lo, ul, last
    48707260:	52800061 	mov	w1, #0x3                   	// #3
    48707264:	2a0f03e4 	mov	w4, w15
    48707268:	52800000 	mov	w0, #0x0                   	// #0
    4870726c:	97fffeac 	bl	48706d1c <create_cmd_upiu.constprop.3>
    48707270:	35000100 	cbnz	w0, 48707290 <ufs_boot_read+0x354>
    48707274:	2a0f03e0 	mov	w0, w15
    48707278:	52800141 	mov	w1, #0xa                   	// #10
    4870727c:	97fffe13 	bl	48706ac8 <wait_for_cmd_completion>
    48707280:	35000080 	cbnz	w0, 48707290 <ufs_boot_read+0x354>
    48707284:	2a0f03e0 	mov	w0, w15
    48707288:	97fffe32 	bl	48706b50 <handle_scsi_completion>
    4870728c:	34fff940 	cbz	w0, 487071b4 <ufs_boot_read+0x278>
    48707290:	52884821 	mov	w1, #0x4241                	// #16961
    48707294:	72a001e1 	movk	w1, #0xf, lsl #16
    48707298:	71000421 	subs	w1, w1, #0x1
    4870729c:	54fffc80 	b.eq	4870722c <ufs_boot_read+0x2f0>  // b.none
    487072a0:	d503201f 	nop
    487072a4:	17fffffd 	b	48707298 <ufs_boot_read+0x35c>
    487072a8:	12801fc0 	mov	w0, #0xffffff01            	// #-255
    487072ac:	121e7801 	and	w1, w0, #0xfffffffd
    487072b0:	3100881f 	cmn	w0, #0x22
    487072b4:	3a571824 	ccmn	w1, #0x17, #0x4, ne  // ne = any
    487072b8:	54fffb21 	b.ne	4870721c <ufs_boot_read+0x2e0>  // b.any
    487072bc:	52884820 	mov	w0, #0x4241                	// #16961
    487072c0:	72a001e0 	movk	w0, #0xf, lsl #16
    487072c4:	71000400 	subs	w0, w0, #0x1
    487072c8:	54000060 	b.eq	487072d4 <ufs_boot_read+0x398>  // b.none
    487072cc:	d503201f 	nop
    487072d0:	17fffffd 	b	487072c4 <ufs_boot_read+0x388>
    487072d4:	710005ef 	subs	w15, w15, #0x1
    487072d8:	54fff821 	b.ne	487071dc <ufs_boot_read+0x2a0>  // b.any
    487072dc:	17ffffd0 	b	4870721c <ufs_boot_read+0x2e0>
    487072e0:	12800080 	mov	w0, #0xfffffffb            	// #-5
    487072e4:	17ffff9c 	b	48707154 <ufs_boot_read+0x218>
    487072e8:	b950002f 	ldr	w15, [x1, #4096]
    487072ec:	d5033fbf 	dmb	sy
    487072f0:	52800095 	mov	w21, #0x4                   	// #4
    487072f4:	52800036 	mov	w22, #0x1                   	// #1
    487072f8:	17ffffd5 	b	4870724c <ufs_boot_read+0x310>

00000000487072fc <__div0>:
    487072fc:	1400017a 	b	487078e4 <hang>

0000000048707300 <emmc_boot_read>:
    48707300:	7140203f 	cmp	w1, #0x8, lsl #12
    48707304:	d2a10082 	mov	x2, #0x8040000             	// #134479872
    48707308:	540001c8 	b.hi	48707340 <emmc_boot_read+0x40>  // b.pmore
    4870730c:	d2a04023 	mov	x3, #0x2010000             	// #33619968
    48707310:	d3427c21 	ubfx	x1, x1, #2, #30
    48707314:	8b030021 	add	x1, x1, x3
    48707318:	92800063 	mov	x3, #0xfffffffffffffffc    	// #-4
    4870731c:	f2beff63 	movk	x3, #0xf7fb, lsl #16
    48707320:	d37ef421 	lsl	x1, x1, #2
    48707324:	eb01005f 	cmp	x2, x1
    48707328:	54000041 	b.ne	48707330 <emmc_boot_read+0x30>  // b.any
    4870732c:	d65f03c0 	ret
    48707330:	b8404445 	ldr	w5, [x2], #4
    48707334:	8b030044 	add	x4, x2, x3
    48707338:	b8246805 	str	w5, [x0, x4]
    4870733c:	17fffffa 	b	48707324 <emmc_boot_read+0x24>
    48707340:	92800066 	mov	x6, #0xfffffffffffffffc    	// #-4
    48707344:	d2900003 	mov	x3, #0x8000                	// #32768
    48707348:	f2beff66 	movk	x6, #0xf7fb, lsl #16
    4870734c:	f2a10083 	movk	x3, #0x804, lsl #16
    48707350:	b8404445 	ldr	w5, [x2], #4
    48707354:	8b060044 	add	x4, x2, x6
    48707358:	eb03005f 	cmp	x2, x3
    4870735c:	b8246805 	str	w5, [x0, x4]
    48707360:	54ffff81 	b.ne	48707350 <emmc_boot_read+0x50>  // b.any
    48707364:	51402021 	sub	w1, w1, #0x8, lsl #12
    48707368:	d2800605 	mov	x5, #0x30                  	// #48
    4870736c:	d2800407 	mov	x7, #0x20                  	// #32
    48707370:	52800004 	mov	w4, #0x0                   	// #0
    48707374:	f2a201e5 	movk	x5, #0x100f, lsl #16
    48707378:	52800406 	mov	w6, #0x20                  	// #32
    4870737c:	f240203f 	tst	x1, #0x1ff
    48707380:	f2a201e7 	movk	x7, #0x100f, lsl #16
    48707384:	1a9f07e2 	cset	w2, ne  // ne = any
    48707388:	0b412441 	add	w1, w2, w1, lsr #9
    4870738c:	52909002 	mov	w2, #0x8480                	// #33920
    48707390:	72a003c2 	movk	w2, #0x1e, lsl #16
    48707394:	b94000a3 	ldr	w3, [x5]
    48707398:	37280143 	tbnz	w3, #5, 487073c0 <emmc_boot_read+0xc0>
    4870739c:	71000442 	subs	w2, w2, #0x1
    487073a0:	540002c0 	b.eq	487073f8 <emmc_boot_read+0xf8>  // b.none
    487073a4:	7211407f 	tst	w3, #0xffff8000
    487073a8:	54000281 	b.ne	487073f8 <emmc_boot_read+0xf8>  // b.any
    487073ac:	52807d23 	mov	w3, #0x3e9                 	// #1001
    487073b0:	71000463 	subs	w3, w3, #0x1
    487073b4:	54ffff00 	b.eq	48707394 <emmc_boot_read+0x94>  // b.none
    487073b8:	d503201f 	nop
    487073bc:	17fffffd 	b	487073b0 <emmc_boot_read+0xb0>
    487073c0:	d3775882 	ubfiz	x2, x4, #9, #23
    487073c4:	d2800003 	mov	x3, #0x0                   	// #0
    487073c8:	91402042 	add	x2, x2, #0x8, lsl #12
    487073cc:	b90000a6 	str	w6, [x5]
    487073d0:	8b020002 	add	x2, x0, x2
    487073d4:	b94000e8 	ldr	w8, [x7]
    487073d8:	b8236848 	str	w8, [x2, x3]
    487073dc:	91001063 	add	x3, x3, #0x4
    487073e0:	f108007f 	cmp	x3, #0x200
    487073e4:	54ffff81 	b.ne	487073d4 <emmc_boot_read+0xd4>  // b.any
    487073e8:	11000484 	add	w4, w4, #0x1
    487073ec:	6b01009f 	cmp	w4, w1
    487073f0:	54fffce1 	b.ne	4870738c <emmc_boot_read+0x8c>  // b.any
    487073f4:	d65f03c0 	ret
    487073f8:	d65f03c0 	ret

00000000487073fc <hw_dec_sop_eop_first_set>:
    487073fc:	7100041f 	cmp	w0, #0x1
    48707400:	900002a2 	adrp	x2, 4875b000 <input_data+0x4b000>
    48707404:	900002a1 	adrp	x1, 4875b000 <input_data+0x4b000>
    48707408:	54000081 	b.ne	48707418 <hw_dec_sop_eop_first_set+0x1c>  // b.any
    4870740c:	b90dbc40 	str	w0, [x2, #3516]
    48707410:	b90db820 	str	w0, [x1, #3512]
    48707414:	d65f03c0 	ret
    48707418:	52800020 	mov	w0, #0x1                   	// #1
    4870741c:	b90db83f 	str	wzr, [x1, #3512]
    48707420:	b90dbc40 	str	w0, [x2, #3516]
    48707424:	d65f03c0 	ret

0000000048707428 <hw_dec_intr_proc>:
    48707428:	d2802480 	mov	x0, #0x124                 	// #292
    4870742c:	f2a22660 	movk	x0, #0x1133, lsl #16
    48707430:	b9400001 	ldr	w1, [x0]
    48707434:	d5033fbf 	dmb	sy
    48707438:	36080241 	tbz	w1, #1, 48707480 <hw_dec_intr_proc+0x58>
    4870743c:	d2841000 	mov	x0, #0x2080                	// #8320
    48707440:	f2a22660 	movk	x0, #0x1133, lsl #16
    48707444:	b9400000 	ldr	w0, [x0]
    48707448:	d5033fbf 	dmb	sy
    4870744c:	36f800c0 	tbz	w0, #31, 48707464 <hw_dec_intr_proc+0x3c>
    48707450:	d5033fbf 	dmb	sy
    48707454:	d2841200 	mov	x0, #0x2090                	// #8336
    48707458:	52800022 	mov	w2, #0x1                   	// #1
    4870745c:	f2a22660 	movk	x0, #0x1133, lsl #16
    48707460:	b9000002 	str	w2, [x0]
    48707464:	d2802602 	mov	x2, #0x130                 	// #304
    48707468:	f2a22662 	movk	x2, #0x1133, lsl #16
    4870746c:	b9400040 	ldr	w0, [x2]
    48707470:	d5033fbf 	dmb	sy
    48707474:	321f0000 	orr	w0, w0, #0x2
    48707478:	d5033fbf 	dmb	sy
    4870747c:	b9000040 	str	w0, [x2]
    48707480:	12800000 	mov	w0, #0xffffffff            	// #-1
    48707484:	36000901 	tbz	w1, #0, 487075a4 <hw_dec_intr_proc+0x17c>
    48707488:	d2841080 	mov	x0, #0x2084                	// #8324
    4870748c:	f2a22660 	movk	x0, #0x1133, lsl #16
    48707490:	b9400001 	ldr	w1, [x0]
    48707494:	d5033fbf 	dmb	sy
    48707498:	52800000 	mov	w0, #0x0                   	// #0
    4870749c:	36f80741 	tbz	w1, #31, 48707584 <hw_dec_intr_proc+0x15c>
    487074a0:	53001c21 	uxtb	w1, w1
    487074a4:	34000661 	cbz	w1, 48707570 <hw_dec_intr_proc+0x148>
    487074a8:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    487074ac:	7102043f 	cmp	w1, #0x81
    487074b0:	910003fd 	mov	x29, sp
    487074b4:	54000261 	b.ne	48707500 <hw_dec_intr_proc+0xd8>  // b.any
    487074b8:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    487074bc:	9131f400 	add	x0, x0, #0xc7d
    487074c0:	97fffd64 	bl	48706a50 <uart_early_puts>
    487074c4:	12800020 	mov	w0, #0xfffffffe            	// #-2
    487074c8:	d5033fbf 	dmb	sy
    487074cc:	d2841281 	mov	x1, #0x2094                	// #8340
    487074d0:	52800022 	mov	w2, #0x1                   	// #1
    487074d4:	f2a22661 	movk	x1, #0x1133, lsl #16
    487074d8:	b9000022 	str	w2, [x1]
    487074dc:	d2802602 	mov	x2, #0x130                 	// #304
    487074e0:	f2a22662 	movk	x2, #0x1133, lsl #16
    487074e4:	b9400041 	ldr	w1, [x2]
    487074e8:	d5033fbf 	dmb	sy
    487074ec:	32000021 	orr	w1, w1, #0x1
    487074f0:	d5033fbf 	dmb	sy
    487074f4:	b9000041 	str	w1, [x2]
    487074f8:	a8c17bfd 	ldp	x29, x30, [sp], #16
    487074fc:	d65f03c0 	ret
    48707500:	7102143f 	cmp	w1, #0x85
    48707504:	54000081 	b.ne	48707514 <hw_dec_intr_proc+0xec>  // b.any
    48707508:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    4870750c:	91327c00 	add	x0, x0, #0xc9f
    48707510:	17ffffec 	b	487074c0 <hw_dec_intr_proc+0x98>
    48707514:	71021c3f 	cmp	w1, #0x87
    48707518:	54000081 	b.ne	48707528 <hw_dec_intr_proc+0x100>  // b.any
    4870751c:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707520:	91330400 	add	x0, x0, #0xcc1
    48707524:	17ffffe7 	b	487074c0 <hw_dec_intr_proc+0x98>
    48707528:	7102243f 	cmp	w1, #0x89
    4870752c:	54000081 	b.ne	4870753c <hw_dec_intr_proc+0x114>  // b.any
    48707530:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707534:	91338c00 	add	x0, x0, #0xce3
    48707538:	17ffffe2 	b	487074c0 <hw_dec_intr_proc+0x98>
    4870753c:	7102283f 	cmp	w1, #0x8a
    48707540:	54000081 	b.ne	48707550 <hw_dec_intr_proc+0x128>  // b.any
    48707544:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707548:	91341400 	add	x0, x0, #0xd05
    4870754c:	17ffffdd 	b	487074c0 <hw_dec_intr_proc+0x98>
    48707550:	71022c3f 	cmp	w1, #0x8b
    48707554:	54000081 	b.ne	48707564 <hw_dec_intr_proc+0x13c>  // b.any
    48707558:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    4870755c:	91349c00 	add	x0, x0, #0xd27
    48707560:	17ffffd8 	b	487074c0 <hw_dec_intr_proc+0x98>
    48707564:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707568:	91352400 	add	x0, x0, #0xd49
    4870756c:	17ffffd5 	b	487074c0 <hw_dec_intr_proc+0x98>
    48707570:	d5033fbf 	dmb	sy
    48707574:	d2841281 	mov	x1, #0x2094                	// #8340
    48707578:	52800022 	mov	w2, #0x1                   	// #1
    4870757c:	f2a22661 	movk	x1, #0x1133, lsl #16
    48707580:	b9000022 	str	w2, [x1]
    48707584:	d2802602 	mov	x2, #0x130                 	// #304
    48707588:	f2a22662 	movk	x2, #0x1133, lsl #16
    4870758c:	b9400041 	ldr	w1, [x2]
    48707590:	d5033fbf 	dmb	sy
    48707594:	32000021 	orr	w1, w1, #0x1
    48707598:	d5033fbf 	dmb	sy
    4870759c:	b9000041 	str	w1, [x2]
    487075a0:	d65f03c0 	ret
    487075a4:	d65f03c0 	ret

00000000487075a8 <hw_dec_start>:
    487075a8:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    487075ac:	b94003e7 	ldr	w7, [sp]
    487075b0:	b94dbc08 	ldr	w8, [x0, #3516]
    487075b4:	34000188 	cbz	w8, 487075e4 <hw_dec_start+0x3c>
    487075b8:	35000447 	cbnz	w7, 48707640 <hw_dec_start+0x98>
    487075bc:	d5033fbf 	dmb	sy
    487075c0:	d2840408 	mov	x8, #0x2020                	// #8224
    487075c4:	f2a22668 	movk	x8, #0x1133, lsl #16
    487075c8:	b9000102 	str	w2, [x8]
    487075cc:	d5033fbf 	dmb	sy
    487075d0:	d2840482 	mov	x2, #0x2024                	// #8228
    487075d4:	f2a22662 	movk	x2, #0x1133, lsl #16
    487075d8:	b9000043 	str	w3, [x2]
    487075dc:	d5033fbf 	dmb	sy
    487075e0:	b9000445 	str	w5, [x2, #4]
    487075e4:	d5033fbf 	dmb	sy
    487075e8:	d2840882 	mov	x2, #0x2044                	// #8260
    487075ec:	710000ff 	cmp	w7, #0x0
    487075f0:	f2a22662 	movk	x2, #0x1133, lsl #16
    487075f4:	b9000041 	str	w1, [x2]
    487075f8:	b94dbc01 	ldr	w1, [x0, #3516]
    487075fc:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707600:	b94db800 	ldr	w0, [x0, #3512]
    48707604:	53030800 	lsl	w0, w0, #29
    48707608:	2a017001 	orr	w1, w0, w1, lsl #28
    4870760c:	52b00000 	mov	w0, #0x80000000            	// #-2147483648
    48707610:	1a9f0000 	csel	w0, w0, wzr, eq  // eq = none
    48707614:	2a040004 	orr	w4, w0, w4
    48707618:	2a010084 	orr	w4, w4, w1
    4870761c:	d5033fbf 	dmb	sy
    48707620:	d2840900 	mov	x0, #0x2048                	// #8264
    48707624:	f2a22660 	movk	x0, #0x1133, lsl #16
    48707628:	b9000004 	str	w4, [x0]
    4870762c:	d5033fbf 	dmb	sy
    48707630:	d2880000 	mov	x0, #0x4000                	// #16384
    48707634:	f2a22660 	movk	x0, #0x1133, lsl #16
    48707638:	b9000006 	str	w6, [x0]
    4870763c:	d65f03c0 	ret
    48707640:	d5033fbf 	dmb	sy
    48707644:	d2840588 	mov	x8, #0x202c                	// #8236
    48707648:	f2a22668 	movk	x8, #0x1133, lsl #16
    4870764c:	b9000102 	str	w2, [x8]
    48707650:	d5033fbf 	dmb	sy
    48707654:	d2840602 	mov	x2, #0x2030                	// #8240
    48707658:	113ffca5 	add	w5, w5, #0xfff
    4870765c:	f2a22662 	movk	x2, #0x1133, lsl #16
    48707660:	530c7ca5 	lsr	w5, w5, #12
    48707664:	531e74a5 	lsl	w5, w5, #2
    48707668:	b9000043 	str	w3, [x2]
    4870766c:	17ffffdc 	b	487075dc <hw_dec_start+0x34>

0000000048707670 <hw_dec_wait_finish>:
    48707670:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
    48707674:	910003fd 	mov	x29, sp
    48707678:	a90153f3 	stp	x19, x20, [sp, #16]
    4870767c:	5290d433 	mov	w19, #0x86a1                	// #34465
    48707680:	72a00033 	movk	w19, #0x1, lsl #16
    48707684:	d2800001 	mov	x1, #0x0                   	// #0
    48707688:	52800ac0 	mov	w0, #0x56                  	// #86
    4870768c:	97ffff67 	bl	48707428 <hw_dec_intr_proc>
    48707690:	2a0003f4 	mov	w20, w0
    48707694:	71000673 	subs	w19, w19, #0x1
    48707698:	54000101 	b.ne	487076b8 <hw_dec_wait_finish+0x48>  // b.any
    4870769c:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    487076a0:	9135b400 	add	x0, x0, #0xd6d
    487076a4:	97fffceb 	bl	48706a50 <uart_early_puts>
    487076a8:	2a1403e0 	mov	w0, w20
    487076ac:	a94153f3 	ldp	x19, x20, [sp, #16]
    487076b0:	a8c37bfd 	ldp	x29, x30, [sp], #48
    487076b4:	d65f03c0 	ret
    487076b8:	b9002fbf 	str	wzr, [x29, #44]
    487076bc:	b9402fa0 	ldr	w0, [x29, #44]
    487076c0:	7104ac1f 	cmp	w0, #0x12b
    487076c4:	54000089 	b.ls	487076d4 <hw_dec_wait_finish+0x64>  // b.plast
    487076c8:	3100069f 	cmn	w20, #0x1
    487076cc:	54fffdc0 	b.eq	48707684 <hw_dec_wait_finish+0x14>  // b.none
    487076d0:	17fffff6 	b	487076a8 <hw_dec_wait_finish+0x38>
    487076d4:	d503201f 	nop
    487076d8:	b9402fa0 	ldr	w0, [x29, #44]
    487076dc:	11000400 	add	w0, w0, #0x1
    487076e0:	b9002fa0 	str	w0, [x29, #44]
    487076e4:	17fffff6 	b	487076bc <hw_dec_wait_finish+0x4c>

00000000487076e8 <hw_dec_decompress>:
    487076e8:	d100c3ff 	sub	sp, sp, #0x30
    487076ec:	aa0003eb 	mov	x11, x0
    487076f0:	12bfe000 	mov	w0, #0xffffff              	// #16777215
    487076f4:	6b0000bf 	cmp	w5, w0
    487076f8:	a9017bfd 	stp	x29, x30, [sp, #16]
    487076fc:	910043fd 	add	x29, sp, #0x10
    48707700:	f90013f3 	str	x19, [sp, #32]
    48707704:	5400012d 	b.le	48707728 <hw_dec_decompress+0x40>
    48707708:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    4870770c:	91310000 	add	x0, x0, #0xc40
    48707710:	97fffcd0 	bl	48706a50 <uart_early_puts>
    48707714:	12800000 	mov	w0, #0xffffffff            	// #-1
    48707718:	910003bf 	mov	sp, x29
    4870771c:	f9400bf3 	ldr	x19, [sp, #16]
    48707720:	a8c27bfd 	ldp	x29, x30, [sp], #32
    48707724:	d65f03c0 	ret
    48707728:	aa0103ec 	mov	x12, x1
    4870772c:	aa0303e8 	mov	x8, x3
    48707730:	aa0403ea 	mov	x10, x4
    48707734:	2a0503e4 	mov	w4, w5
    48707738:	52800020 	mov	w0, #0x1                   	// #1
    4870773c:	aa0203f3 	mov	x19, x2
    48707740:	97ffff2f 	bl	487073fc <hw_dec_sop_eop_first_set>
    48707744:	2a0a03e1 	mov	w1, w10
    48707748:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    4870774c:	2a0c03e3 	mov	w3, w12
    48707750:	2a0b03e2 	mov	w2, w11
    48707754:	52800007 	mov	w7, #0x0                   	// #0
    48707758:	52800006 	mov	w6, #0x0                   	// #0
    4870775c:	b94dc000 	ldr	w0, [x0, #3520]
    48707760:	b90003e0 	str	w0, [sp]
    48707764:	2a0803e0 	mov	w0, w8
    48707768:	b9400265 	ldr	w5, [x19]
    4870776c:	97ffff8f 	bl	487075a8 <hw_dec_start>
    48707770:	97ffffc0 	bl	48707670 <hw_dec_wait_finish>
    48707774:	d2841101 	mov	x1, #0x2088                	// #8328
    48707778:	f2a22661 	movk	x1, #0x1133, lsl #16
    4870777c:	b9400021 	ldr	w1, [x1]
    48707780:	d5033fbf 	dmb	sy
    48707784:	7100001f 	cmp	w0, #0x0
    48707788:	b9000261 	str	w1, [x19]
    4870778c:	5a9f03e0 	csetm	w0, ne  // ne = any
    48707790:	17ffffe2 	b	48707718 <hw_dec_decompress+0x30>

0000000048707794 <hw_dec_init>:
    48707794:	d2802901 	mov	x1, #0x148                 	// #328
    48707798:	f2a24021 	movk	x1, #0x1201, lsl #16
    4870779c:	b9400020 	ldr	w0, [x1]
    487077a0:	d5033fbf 	dmb	sy
    487077a4:	321f0000 	orr	w0, w0, #0x2
    487077a8:	d5033fbf 	dmb	sy
    487077ac:	b9000020 	str	w0, [x1]
    487077b0:	d5033fbf 	dmb	sy
    487077b4:	d2804180 	mov	x0, #0x20c                 	// #524
    487077b8:	f2a22660 	movk	x0, #0x1133, lsl #16
    487077bc:	b900001f 	str	wzr, [x0]
    487077c0:	d5033fbf 	dmb	sy
    487077c4:	52a06001 	mov	w1, #0x3000000             	// #50331648
    487077c8:	b9000001 	str	w1, [x0]
    487077cc:	d5033fbf 	dmb	sy
    487077d0:	52a60001 	mov	w1, #0x30000000            	// #805306368
    487077d4:	b9000001 	str	w1, [x0]
    487077d8:	d5033fbf 	dmb	sy
    487077dc:	d2802500 	mov	x0, #0x128                 	// #296
    487077e0:	52800061 	mov	w1, #0x3                   	// #3
    487077e4:	f2a22660 	movk	x0, #0x1133, lsl #16
    487077e8:	b9000001 	str	w1, [x0]
    487077ec:	d5033fbf 	dmb	sy
    487077f0:	d2802000 	mov	x0, #0x100                 	// #256
    487077f4:	52800021 	mov	w1, #0x1                   	// #1
    487077f8:	f2a22660 	movk	x0, #0x1133, lsl #16
    487077fc:	b9000001 	str	w1, [x0]
    48707800:	d65f03c0 	ret

0000000048707804 <hw_dec_uinit>:
    48707804:	d5033fbf 	dmb	sy
    48707808:	d2802000 	mov	x0, #0x100                 	// #256
    4870780c:	f2a22660 	movk	x0, #0x1133, lsl #16
    48707810:	b900001f 	str	wzr, [x0]
    48707814:	d5033fbf 	dmb	sy
    48707818:	d2802901 	mov	x1, #0x148                 	// #328
    4870781c:	b900281f 	str	wzr, [x0, #40]
    48707820:	f2a24021 	movk	x1, #0x1201, lsl #16
    48707824:	b9400020 	ldr	w0, [x1]
    48707828:	d5033fbf 	dmb	sy
    4870782c:	121e7800 	and	w0, w0, #0xfffffffd
    48707830:	d5033fbf 	dmb	sy
    48707834:	b9000020 	str	w0, [x1]
    48707838:	d65f03c0 	ret

000000004870783c <start_armboot>:
    4870783c:	a9be7bfd 	stp	x29, x30, [sp, #-32]!
    48707840:	910003fd 	mov	x29, sp
    48707844:	97fffc6b 	bl	487069f0 <uart_early_init>
    48707848:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    4870784c:	91363400 	add	x0, x0, #0xd8d
    48707850:	97fffc80 	bl	48706a50 <uart_early_puts>
    48707854:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707858:	b90dc01f 	str	wzr, [x0, #3520]
    4870785c:	97ffffce 	bl	48707794 <hw_dec_init>
    48707860:	900002a5 	adrp	x5, 4875b000 <input_data+0x4b000>
    48707864:	b0000044 	adrp	x4, 48710000 <input_data>
    48707868:	912e98a5 	add	x5, x5, #0xba6
    4870786c:	91000084 	add	x4, x4, #0x0
    48707870:	910073a2 	add	x2, x29, #0x1c
    48707874:	d2800006 	mov	x6, #0x0                   	// #0
    48707878:	d2800003 	mov	x3, #0x0                   	// #0
    4870787c:	d2a91001 	mov	x1, #0x48800000            	// #1216348160
    48707880:	385fc0a0 	ldurb	w0, [x5, #-4]
    48707884:	390073a0 	strb	w0, [x29, #28]
    48707888:	385fd0a0 	ldurb	w0, [x5, #-3]
    4870788c:	390077a0 	strb	w0, [x29, #29]
    48707890:	385fe0a0 	ldurb	w0, [x5, #-2]
    48707894:	39007ba0 	strb	w0, [x29, #30]
    48707898:	385ff0a0 	ldurb	w0, [x5, #-1]
    4870789c:	4b0400a5 	sub	w5, w5, w4
    487078a0:	39007fa0 	strb	w0, [x29, #31]
    487078a4:	d2800000 	mov	x0, #0x0                   	// #0
    487078a8:	97ffff90 	bl	487076e8 <hw_dec_decompress>
    487078ac:	35000140 	cbnz	w0, 487078d4 <start_armboot+0x98>
    487078b0:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    487078b4:	91366c00 	add	x0, x0, #0xd9b
    487078b8:	97fffc66 	bl	48706a50 <uart_early_puts>
    487078bc:	97ffffd2 	bl	48707804 <hw_dec_uinit>
    487078c0:	97ffeda2 	bl	48702f48 <__asm_invalidate_icache_all>
    487078c4:	d2a91000 	mov	x0, #0x48800000            	// #1216348160
    487078c8:	d63f0000 	blr	x0
    487078cc:	a8c27bfd 	ldp	x29, x30, [sp], #32
    487078d0:	d65f03c0 	ret
    487078d4:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    487078d8:	91367c00 	add	x0, x0, #0xd9f
    487078dc:	97fffc5d 	bl	48706a50 <uart_early_puts>
    487078e0:	14000000 	b	487078e0 <start_armboot+0xa4>

00000000487078e4 <hang>:
    487078e4:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    487078e8:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    487078ec:	91305800 	add	x0, x0, #0xc16
    487078f0:	910003fd 	mov	x29, sp
    487078f4:	97fffc57 	bl	48706a50 <uart_early_puts>
    487078f8:	14000000 	b	487078f8 <hang+0x14>

00000000487078fc <invalidate_icache_all>:
    487078fc:	17ffed93 	b	48702f48 <__asm_invalidate_icache_all>

0000000048707900 <icache_enable>:
    48707900:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48707904:	910003fd 	mov	x29, sp
    48707908:	97ffed90 	bl	48702f48 <__asm_invalidate_icache_all>
    4870790c:	d5384240 	mrs	x0, currentel
    48707910:	53027c00 	lsr	w0, w0, #2
    48707914:	7100041f 	cmp	w0, #0x1
    48707918:	54000161 	b.ne	48707944 <icache_enable+0x44>  // b.any
    4870791c:	d5381000 	mrs	x0, sctlr_el1
    48707920:	32140000 	orr	w0, w0, #0x1000
    48707924:	d5384241 	mrs	x1, currentel
    48707928:	53027c21 	lsr	w1, w1, #2
    4870792c:	7100043f 	cmp	w1, #0x1
    48707930:	54000161 	b.ne	4870795c <icache_enable+0x5c>  // b.any
    48707934:	d5181000 	msr	sctlr_el1, x0
    48707938:	d5033fdf 	isb
    4870793c:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48707940:	d65f03c0 	ret
    48707944:	7100081f 	cmp	w0, #0x2
    48707948:	54000061 	b.ne	48707954 <icache_enable+0x54>  // b.any
    4870794c:	d53c1000 	mrs	x0, sctlr_el2
    48707950:	17fffff4 	b	48707920 <icache_enable+0x20>
    48707954:	d53e1000 	mrs	x0, sctlr_el3
    48707958:	17fffff2 	b	48707920 <icache_enable+0x20>
    4870795c:	7100083f 	cmp	w1, #0x2
    48707960:	54000061 	b.ne	4870796c <icache_enable+0x6c>  // b.any
    48707964:	d51c1000 	msr	sctlr_el2, x0
    48707968:	17fffff4 	b	48707938 <icache_enable+0x38>
    4870796c:	d51e1000 	msr	sctlr_el3, x0
    48707970:	17fffff2 	b	48707938 <icache_enable+0x38>

0000000048707974 <do_bad_sync>:
    48707974:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48707978:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    4870797c:	912fc400 	add	x0, x0, #0xbf1
    48707980:	910003fd 	mov	x29, sp
    48707984:	97fffc33 	bl	48706a50 <uart_early_puts>
    48707988:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    4870798c:	91300800 	add	x0, x0, #0xc02
    48707990:	97fffc30 	bl	48706a50 <uart_early_puts>
    48707994:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48707998:	d2800000 	mov	x0, #0x0                   	// #0
    4870799c:	1400004f 	b	48707ad8 <reset_cpu>

00000000487079a0 <do_sync>:
    487079a0:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    487079a4:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    487079a8:	912fd400 	add	x0, x0, #0xbf5
    487079ac:	910003fd 	mov	x29, sp
    487079b0:	97fffc28 	bl	48706a50 <uart_early_puts>
    487079b4:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    487079b8:	91300800 	add	x0, x0, #0xc02
    487079bc:	97fffc25 	bl	48706a50 <uart_early_puts>
    487079c0:	a8c17bfd 	ldp	x29, x30, [sp], #16
    487079c4:	d2800000 	mov	x0, #0x0                   	// #0
    487079c8:	14000044 	b	48707ad8 <reset_cpu>

00000000487079cc <do_bad_error>:
    487079cc:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    487079d0:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    487079d4:	912ec000 	add	x0, x0, #0xbb0
    487079d8:	910003fd 	mov	x29, sp
    487079dc:	97fffc1d 	bl	48706a50 <uart_early_puts>
    487079e0:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    487079e4:	91300800 	add	x0, x0, #0xc02
    487079e8:	97fffc1a 	bl	48706a50 <uart_early_puts>
    487079ec:	a8c17bfd 	ldp	x29, x30, [sp], #16
    487079f0:	d2800000 	mov	x0, #0x0                   	// #0
    487079f4:	14000039 	b	48707ad8 <reset_cpu>

00000000487079f8 <do_error>:
    487079f8:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    487079fc:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707a00:	912ed000 	add	x0, x0, #0xbb4
    48707a04:	910003fd 	mov	x29, sp
    48707a08:	97fffc12 	bl	48706a50 <uart_early_puts>
    48707a0c:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707a10:	91300800 	add	x0, x0, #0xc02
    48707a14:	97fffc0f 	bl	48706a50 <uart_early_puts>
    48707a18:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48707a1c:	d2800000 	mov	x0, #0x0                   	// #0
    48707a20:	1400002e 	b	48707ad8 <reset_cpu>

0000000048707a24 <do_bad_fiq>:
    48707a24:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48707a28:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707a2c:	912ef000 	add	x0, x0, #0xbbc
    48707a30:	910003fd 	mov	x29, sp
    48707a34:	97fffc07 	bl	48706a50 <uart_early_puts>
    48707a38:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707a3c:	91300800 	add	x0, x0, #0xc02
    48707a40:	97fffc04 	bl	48706a50 <uart_early_puts>
    48707a44:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48707a48:	d2800000 	mov	x0, #0x0                   	// #0
    48707a4c:	14000023 	b	48707ad8 <reset_cpu>

0000000048707a50 <do_bad_irq>:
    48707a50:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48707a54:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707a58:	912f6400 	add	x0, x0, #0xbd9
    48707a5c:	910003fd 	mov	x29, sp
    48707a60:	97fffbfc 	bl	48706a50 <uart_early_puts>
    48707a64:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707a68:	91300800 	add	x0, x0, #0xc02
    48707a6c:	97fffbf9 	bl	48706a50 <uart_early_puts>
    48707a70:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48707a74:	d2800000 	mov	x0, #0x0                   	// #0
    48707a78:	14000018 	b	48707ad8 <reset_cpu>

0000000048707a7c <do_fiq>:
    48707a7c:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48707a80:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707a84:	912f0000 	add	x0, x0, #0xbc0
    48707a88:	910003fd 	mov	x29, sp
    48707a8c:	97fffbf1 	bl	48706a50 <uart_early_puts>
    48707a90:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707a94:	91300800 	add	x0, x0, #0xc02
    48707a98:	97fffbee 	bl	48706a50 <uart_early_puts>
    48707a9c:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48707aa0:	d2800000 	mov	x0, #0x0                   	// #0
    48707aa4:	1400000d 	b	48707ad8 <reset_cpu>

0000000048707aa8 <do_irq>:
    48707aa8:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
    48707aac:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707ab0:	912f7400 	add	x0, x0, #0xbdd
    48707ab4:	910003fd 	mov	x29, sp
    48707ab8:	97fffbe6 	bl	48706a50 <uart_early_puts>
    48707abc:	900002a0 	adrp	x0, 4875b000 <input_data+0x4b000>
    48707ac0:	91300800 	add	x0, x0, #0xc02
    48707ac4:	97fffbe3 	bl	48706a50 <uart_early_puts>
    48707ac8:	a8c17bfd 	ldp	x29, x30, [sp], #16
    48707acc:	d2800000 	mov	x0, #0x0                   	// #0
    48707ad0:	14000002 	b	48707ad8 <reset_cpu>
    48707ad4:	00000000 	.inst	0x00000000 ; undefined

0000000048707ad8 <reset_cpu>:
    48707ad8:	580000c1 	ldr	x1, 48707af0 <rstctl>
    48707adc:	52800043 	mov	w3, #0x2                   	// #2
    48707ae0:	b9000023 	str	w3, [x1]
    48707ae4:	aa0003e0 	mov	x0, x0

0000000048707ae8 <_loop_forever>:
    48707ae8:	14000000 	b	48707ae8 <_loop_forever>
    48707aec:	d503201f 	nop

0000000048707af0 <rstctl>:
    48707af0:	12020004 	.word	0x12020004
    48707af4:	00000000 	.word	0x00000000
