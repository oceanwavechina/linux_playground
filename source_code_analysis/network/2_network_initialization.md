# Network Initialization

1. what happens when the Linux operating system boots
2. how the kernel and supporting programs ifconfig and route establish network links
3. the differences between several example configurations
4. summarizes the implementation code within the kernel and network programs

## 1. 启动 (startup)
---------------------

1. 从磁盘加载 image 到内存，解包镜像
2. 安装文件系统，内存管理系统，和其他关键的系统
3. 执行 init 程序
   1. 读取配置文件 /etc/inittab
   2. 执行启动脚本 /etc/rc.d 其中包含了网络初始化脚本 /etc/rc.d/init.d/network

### 1.1 网络初始化脚本 (The Network Initialization Script)
   
唤起各种程序

<br>

### 1.2 接口设备配置 (ifconfig)

这个程序用来配置 interface device, 但它并不是内核的一部分。

1. 首先，它给这些设备提供ip地址，网络掩码，和广播地址
2. 接下来，这些设备执行它自己的初始化函数，比如设置一些静态变量，在kernel中注册中断和服务程序

执行ficonfig会返回当前interface的配置信息和状态

```
eth0  Link encap:Ethernet  HWaddr 00:C1:4E:7D:9E:25
      inet addr:172.16.1.1  Bcast:172.16.1.255  Mask:255.255.255.0
      UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
      RX packets:389016 errors:16534 dropped:0 overruns:0 frame:24522
      TX packets:400845 errors:0 dropped:0 overruns:0 carrier:0
      collisions:0 txqueuelen:100
      Interrupt:11 Base address:0xcc00
```

<br>

### 1.3 路由 （route）

这个程序对interface device 添加预先定义的路由信息到FIB(Forwarding Information Base), 和也不是内核的一部分。