



arm-none-eabi-objcopy build/project.elf -O binary build/project.bin 
st-flash write build/project.bin 0x08000000 
