# Introduction

* virtual machine VS docker
---------------------------
    
      Virtual Machine
    
    ++++++++++++++++++++
    |   app  |   app   |                      
    ++++++++++++++++++++
    |  bins  |  bins   |                      Docker
    |  libs  |  libs   |                ++++++++++++++++++++
    ++++++++++++++++++++                |   app  |   app   |
    | kernel |  kernel |                ++++++++++++++++++++
    ++++++++++++++++++++                |  bins  |  bins   |
    |    Hypervisor    |                |  libs  |  libs   |
    ++++++++++++++++++++                ++++++++++++++++++++
    |      Host OS     |                |      Host OS     |
    ++++++++++++++++++++                ++++++++++++++++++++
    |       server     |                |       server     |
    ++++++++++++++++++++                ++++++++++++++++++++


* Container & Image
---------------------------

      ++++++++++              +++++++++++++
      |  image |  --> run --> | container |  
      ++++++++++              +++++++++++++
          |
          v
       contian
          |
          v
    +++++++++++++++
    |     os      |
    |   software  |
    | application |
    +++++++++++++++ 

* Dockerfile & Image
---------------------------

    ++++++++++++++++++++
    |    dokerfile     | 
    ++++++++++++++++++++
    |                  |                 +++++++++++              +++++++++++++
    |      steps       |  --> build -->  |  image  |  --> run --> | container |
    |        to        |                 +++++++++++              +++++++++++++
    |      create      |
    |      image       |
    |                  |
    ++++++++++++++++++++

1. 创建一个 python 应用
--------------------

* 1). 目录结构如下

        helloworld/
        ├── Dockerfile
        └── app
            └── app.py

* 2). 编写我们的flask应用
  
  main.py

* 3). hub.docker.com 上找到一个可以运行python的Image<br>
  (因为是要运行web，所以可以选择一个同时有uwsgi, nginx的Image)
  
  登录dockerhub，（注意这里的登录名不是邮箱地址而是用户名，可以登录到hub.docker.com查看）:

  <code>
    docker login
  </code>

* 4). build Image
  
  <code>
  cd helloworld/<br>
  docker build -t helloworld_img .
  </code>

  > 这个会执行我们在Dockerfileh中定义的步骤

* 5). Run Image

  <code>
  docker run -p 8080:80 helloworld_img
  </code>

  > -p 参数中: 第一个是host的端口，第二个是container中的端口

* 6). 访问 http://127.0.0.1:8080

  浏览器中得到如下响应, 
  
  > Hello World from Flask in a uWSGI Nginx Docker container with Python 3.7 (default)

  因为是把源代码拷贝到docker image中的。所以在host中修改了app，并不能在container中生效<br>
  我们可以配置host中的directory作为container的共享

* 7). 配置共享目录，把host的directory 挂载到contianer中

  <code>
  docker run -p8080:80 -v /Users/liuyanan/my_work/Eclipse_Space/linux_playground.git/docker/intro/helloworld/app:/app helloworld_img
  </code>

  

  