
SECTIONS {
	.rodata : {
		*(SORT(.mirm.*.rodata))
	}
}
