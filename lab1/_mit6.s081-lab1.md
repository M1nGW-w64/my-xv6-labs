# 1. sleep

- 热身小项目，比较简单。

- 主要学习了如何将命令行的输入作为参数：

    1. `int argc`：参数的个数

    2. `char *argv[]`/`char **argv`：参数列表。其中`argv[0]`是所执行命令的完整路径。

# 2. pingpong

- 同1。
- 使用管道进行进程间通信。由于是进程双向通信，因此得设置两个管道。

- 把用不上的文件描述符读写端口关闭是一个好习惯。

# 3. find

- 学习了如何访问目录

    1. 首先打开当前路径下的文件，获取其**文件描述符**。

    2. 读取当前路径下的**文件状态**，**文件状态**指明了当前访问的文件的类型（文件、目录或者设备）。

        - 如果文件描述符fd指向的对象是文件，那么我们可以直接对它操作（访问等）。

        - 如果fd指向的对象是目录，那么我们要读取目录文件中的每一个目录项，根据它来对文件操作。

        ```cpp
        struct stat {
            int dev;     // File system's disk device
            uint ino;    // Inode number
            short type;  // Type of file
            short nlink; // Number of links to file
            uint64 size; // Size of file in bytes
        };
        ```
    3. 进入下一级目录（如果有）。

- 加深对文件和目录的理解

    1. 目录：是一个文件，由一个一个目录项组成。在xv6中，目录项仅包含两个属性，分别是**文件名**和**inode编号**，如下：
        ```cpp
        struct dirent {
            ushort inum;
            char name[DIRSIZ];
        };
        ```
    
    2. `find <dir> <filename>`命令的实现：与`ls`类似。区别在于find命令是**递归地**进行查找。注意不能在当前目录`.`和父级目录`..`下递归。

# 4. primes

- 这个实验用管道实现了一个质数筛。

- 实现思路如下 ：

    1. 父进程负责往管道写，子进程负责读并筛选。

    2. 子进程从管道里读出来的第一个数一定是质数。把这个数作为质基数(记为`prime`)，筛去所有`prime`的倍数，并把剩下数写到另一个管道内。
    3. 递归的进行上面的步骤，直到筛完为止。
- 示例：

    ![sieve](_sieve.gif)

# 5. xargs

- 学习xargs命令：

    1. 简单来说，xargs的作用是把标准输入转化为命令行参数。

    2. 一般来说和管道一起使用。管道的作用是把标准输出转化为标准输入。比如
        ```sh
        echo "world" | xargs echo "hello"
        ```
        它的输出就是：
        ```sh
        hello world
        ```
        `echo "world"`的标准输出被管道转换为标准输入，然后，`xargs`命令将其转换为命令行参数。上面的命令等价于：
        ```sh
        echo "hello world"
        ```
    3. 参考资料：[xargs](https://www.ruanyifeng.com/blog/2019/08/xargs-tutorial.html)

- xargs命令实现细节：

    1. 首先，我们得读取xargs的命令行参数。
        ```cpp
        for (int i = 1; i < argc; i++)
            xargs_args[i - 1] = argv[i];
        ```
    
    2. 然后，我们还得读取管道产生的标准输入，并将标准输入作为参数加入xargs命令的参数列表。
        ```cpp
        while (read(0, &buf, sizeof(buf))) {
            // code
        }
        ```
    3. fork产生一个子进程用于执行xargs后的命令;父进程等待子进程执行完毕，然后重置指针，继续读取下一批标准输入。