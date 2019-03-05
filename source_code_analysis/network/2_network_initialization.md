### Network Initialization
---------------------

1. what happens when the Linux operating system boots
2. how the kernel and supporting programs ifconfig and route establish network links
3. the differences between several example configurations
4. summarizes the implementation code within the kernel and network programs

#### 1. 核心结构 (include/linux/skbuff.h)
---------------------

```c
struct sk_buff
```

1. 节省时间，因为linux的协议栈是严格的layer结构，layer之间的数据传递 基本上 只需通过指针实现
2. 数据的拷贝只需两次， 一是，user-space 到 kernel-space之间； 二是，kernel-space到输出媒介之间
3. 每层只需填充自己处理packet时需要的数据

<br>

#### 2. 路由 （Internet Routing）
---------------------

DataStructure:

  1. **FIB (Forwarding Infomation Base)**
   
     这个结构保存了所有已知的路由信息，是完整的路由信息
     按bit分成了32个zone
     1. zone的id是由，ip和掩码产生的有效位来确定的，比如子网掩码为255.0.0.0的网络的有效位是8，其entry就会放在zone-8中
     2. 每个zone中存放的是具体的e路由entry
     3. 在文件 /proc/net/route 中

  2. **Routing Cache**

     1. 当前正在使用的destination的缓存
     2. 这个是一个hash table
     3. 在文件 /proc/net/rt_cache 中


  3. **Neighbor table**

     直接物理相连的的机器的信息
