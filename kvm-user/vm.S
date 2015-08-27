# sample vm code

.global _start
.code16

_start:
	xorw %ax, %ax

loop1:
	out %ax, $0x10 # this will trap to QEMU
	inc %ax
	jmp loop1
