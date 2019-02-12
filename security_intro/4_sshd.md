# sshd

配置文件在/etc/ssh/sshd_config<br>
每次修改完配置文件都要重启 sshd 或是 ssh

* 推荐的一些安全配置
 ---------------------------------------
  <pre>
  <code>
    PermitRootLogin no # 禁止人们使用root通过ssh登录 <br>
    PubkeyAuthentication yes # 使用证书登录 <br>
    AuthorizedKeysFile	%h/.ssh/authorized_keys # 授权证书的存放位置 <br>
    PasswordAuthentication no   # 禁止使用密码登录 <br>
    PermitEmptyPasswords no  # 不允许使用空密码 <br>
    UsePAM  yes      # 使用PAM认证
  </code>
  </pre>