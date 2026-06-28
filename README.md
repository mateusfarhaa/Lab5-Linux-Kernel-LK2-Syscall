# MC504 – Implementação das syscalls `set_secret` e `get_secret`

Este repositório contém os arquivos modificados para a implementação das chamadas de sistema **`set_secret`** (472) e **`get_secret`** (473), além do programa de teste utilizado para validação.

## Funcionalidade

Foi implementado um **cofre de segredo** no kernel Linux. A syscall `set_secret` armazena uma string em um buffer global protegido por mutex, enquanto `get_secret` recupera esse conteúdo para o espaço de usuário.

## Arquivos modificados

| Arquivo |
| -------- |
| `arch/x86/entry/syscalls/syscall_64.tbl` |
| `include/linux/syscalls.h` |
| `include/uapi/asm-generic/unistd.h` |
| `kernel/sys_ni.c` |
| `kernel/Makefile` |
| `kernel/secret.c` |
| `test_secret.c` |

## Compilação e execução

Compile o kernel:

```bash
make -j$(nproc)
```

Inicie o QEMU:

```bash
qemu-system-x86_64 \
    -drive file=../my_disk.raw,format=raw,index=0,media=disk \
    -m 4G -nographic \
    -kernel ./arch/x86_64/boot/bzImage \
    -append "root=/dev/sda rw console=ttyS0 loglevel=6" \
    -fsdev local,id=fs1,path=../shared_folder,security_model=none \
    -device virtio-9p-pci,fsdev=fs1,mount_tag=shared_folder
```

Dentro da máquina virtual, execute:

```bash
cd /root && ./test_secret
```

Para encerrar o QEMU, pressione **Ctrl+A** seguido de **X**.
