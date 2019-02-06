# Auditd

为了让管理员可以快速的查看都发生了那些活动<br>
(auditd 在debian下边默认没有安装需要手动安装 sudo apt-get install auditd)


## Auditd - Auditing 事件
---------------------------------------

* 文件访问
* 系统调用
* 命令执行
* 登录失败
* 防火墙更改


## 例子
--------------------------------------------

#### 监视密码修改

1. 查看当前规则列表

    <code>sudo auditctl -l</code>
    
    默认是没有规则

2. 设置规则，监视密码修改<br>
   (通过命令创建的是 **临时** 规则, 如果要创建永久规则需要修改audit.rules文件)
    
    <code>$ sudo auditctl -w /etc/passwd -p wa -k passwd_changes</code>

    > 参数说明<br>
    > -w :审计监视的路径<br>
    > -p：目录或者文件的权限 ,rxwa分别是读、执行、写、属性

3. 通过search查看审计日志
   
   <code>sudo ausearch -f/etc/passwd | grep useradd</code>
    > type=SYSCALL msg=audit(1549270599.815:36): arch=c000003e syscall=2 success=yes exit=5 a0=7f649d8ecc00 a1=20902 a2=0 a3=7f649ce27460 items=1 ppid=2316 pid=2317 auid=1000 uid=0 gid=0 euid=0 suid=0 fsuid=0 egid=0 sgid=0 fsgid=0 tty=pts0 ses=1 comm="useradd" exe="/usr/sbin/useradd" key="passwd_changes"<br>
    <br>
    > type=SYSCALL msg=audit(1549270599.827:40): arch=c000003e syscall=82 success=yes exit=0 a0=7ffcbefe0ca0 a1=7f649d8ecc00 a2=7ffcbefe0c10 a3=7f649ce276a0 items=5 ppid=2316 pid=2317 auid=1000 uid=0 gid=0 euid=0 suid=0 fsuid=0 egid=0 sgid=0 fsgid=0 tty=pts0 ses=1 comm="useradd" exe="/usr/sbin/useradd" key="passwd_changes"
   
   参数说明：
    > time :审计时间<br>
    > name :审计对象<br>
    > cwd :当前路径<br>
    > syscall :相关的系统调用<br>
    > auid :审计用户ID<br>
    > uid和 gid :访问文件的用户ID和用户组ID<br>
    > comm :用户访问文件的命令<br>
    > exe :上面命令的可执行文件路径

4. 通过report 查看生成的审计报告
   可以看到failed authentications 是2，因为我两次尝试用iceserver账户ssh登录失败了
   
   <code>$ sudo aureport</code>
    > Summary Report <br>
    > \====================== <br>
    > Range of time in logs: 02/04/2019 16:34:14.470 - 02/04/2019 17:06:40.343 <br>
    > Selected time for report: 02/04/2019 16:34:14 - 02/04/2019 17:06:40.343 <br>
    > Number of changes in configuration: 2 <br>
    > Number of changes to accounts, groups, or roles: 3 <br>
    > Number of logins: 0 <br>
    > Number of failed logins: 0 <br>
    > Number of authentications: 0 <br>
    > Number of failed authentications: 2 <br>
    > Number of users: 3 <br>
    > Number of terminals: 5 <br>
    > Number of host names: 2 <br>
    > Number of executables: 5 <br>
    > Number of files: 3 <br>
    > Number of AVC's: 0 <br>
    > Number of MAC events: 0 <br>
    > Number of failed syscalls: 0 <br>
    > Number of anomaly events: 0 <br>
    > Number of responses to anomaly events: 0 <br>
    > Number of crypto events: 0 <br>
    > Number of keys: 1 <br>
    > Number of process IDs: 19 <br>
    > Number of events: 76 <br>