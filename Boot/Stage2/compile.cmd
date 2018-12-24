@echo off
cd assembler
nasm -f bin KLOADER.asm -o A:\KLOADER.SYS
cd ..
@echo "Compile Competed!"