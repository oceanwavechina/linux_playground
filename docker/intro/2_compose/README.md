# docker compose

#### 简介
----------------------
  
  website是client提供接口的web层，而product是后端的 micro-service 中的一个服务<br>
  这个例子是如何利用compose自动化部署多服务的应用

#### Note
----------------------

  doker之间相互调用不能用127.0.0.1，因为127.0.0.1在docker container看来是他自己而不是host的ip

  docker compose 会给所有的docker containers 创建一个virtual network， <br>
  所以每个contianer可以通过在composer配置文件中的配置，来访问其他contianer <br>
  其中yml配置文件中的每个service节点的名字就是每个container的hostname（比如 ‘product-service’）


#### 步骤 
----------------------

* 部署运行:  

  <code>
  docker-compose up
  
  docker ps
  </code>
          
      CONTAINER ID        IMAGE                       COMMAND                  CREATED             STATUS              PORTS                  NAMES
  
      d76c7e150c72        2_compose_website           "docker-php-entrypoi…"   6 minutes ago       Up 3 minutes        0.0.0.0:5000->80/tcp   2_compose_website_1
  
      96b3503c9dba        2_compose_product-service   "python api.py"          32 minutes ago      Up 3 minutes        0.0.0.0:5001->80/tcp   2_compose_product-service_1

* 停止

  <code>
  docker-compose stop
  </code>