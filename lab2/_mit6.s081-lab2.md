# 1. system calling trace

- 理解了xv6中系统调用的执行过程，加深了对`exec`的认识。
- 先捋一下系统调用的过程，假设使用`read()`函数：

    1. 用户程序调用`read()`函数。
        ```cpp
        int main(){
            // code
            read(fd, &buf, sizeof(buf));
            // code
        }
        ```

    2. 调用`read()`函数时， 切换到`usys.asm`，将系统调用号压入a7寄存器。
        ```S
        .global read
        read:
        li a7, SYS_read
        ecall
        ret
        ```
    3. 执行`ecall`指令后，陷入软中断，进入内核态。执行`syscall.c`中的`syscall`函数。根据系统调用号索引到对应的系统调用。然后再返回。
        ```cpp
        // 读取系统调用号
        num = p->trapframe->a7;
        // 根据系统调用号索引对应的系统调用
        p->trapframe->a0 = syscalls[num]();
        ```

- `trace()`的整个执行过程，和一般的系统调用类似：

    1. `trace()`执行时，和上面的过程类似。但是在`sys_trace`系统调用中，需要将系统调用号保存在当前进程的结构`proc`中。
        ```cpp
        myproc()->syscall_num = n;
        ```
    
    2. 执行完`trace()`后，还会执行`exec()`。结合示例这一点不难理解。
    3. `exec()`执行时，会切换内存镜像，**但是，进程的pid不变。这就意味着结构体**`strcut proc`**中的内容也是不变的**。因此可以从该结构体中读出trace系统调用保存的待追踪的系统调用号，也就可以完成该功能的开发。
- 给出的`trace`用法有些歧义。trace后面跟着的数字不能直接当成要追踪的系统调用号，而是要转为二进制来看，比如：

    1. 比如`trace 32 grep hello README`

    2. `32 -> 0001 0000`，第五位为1，所以追踪第五号系统调用`read`。 

# 2. sysinfo 

- 按照Hint完成即可，该任务比较简单，和任务一大同小异。    