# myftp
简单的ftp服务器

看了unix网络编程之后练手的项目，结构可能比较乱

* 目前实现了：

 1.  help         展示所有可用的命令

 2.  ls              展示当前目录下所有的文件，默认初始化在ftp根目录

 3.  quit          退出ftp系统

 4.  get            传统ftp方法，从服务器上下文件

 5.  put            传统ftp方法，向服务器上传文件

 6.  cd path     类似bash的功能，进入到下一个目录中

     

* 已解决bug

    1. cd功能会出去ftp的地址范围，可以浏览到任意服务器文件

       

       

* 已知bug

    1. cd path 不对path进行检错，导致下次调用其他命令式当前目录错误



* 准备实现的功能
    1. 多人同时登录
    2. 身份验证
    3. 统计在线人数



* get用法

  ```bash
  get filename
  ```

  这里的filname是指当前用户所在ftp目录下的filename，接受的文件放在./recvftp文件夹下

* put用法

  ```bash
  put filepath
  ```

  注意，这里面是指要上传文件的**完整路径**，**不是**单一的文件名，默认上传路径在用户当前所在的ftp目录下

* cd用法

  ```bash
  cd ..
  cd .
  cd xxx
  ```

  和linux下用法一样，没什么好说的





* 目前调用没发现什么其他的bug，以后发现再加
* 然后现在也只能实现一个人登录，等功能写完再向上加多人同时登录的版本，我得想想用什么方法比较好
* 说下目录结构

myftpserver.c    顾名思义，服务器端主程序，主要调用network.c进行网络的初始化，然后对客户端过来的命令进行分析，分别调用不同的处理函数

myclient.c     同理，客户端主程序，同服务器端主程序，先调用network.c进行初始化，然后对终端输入的命令进行分析，调用不同的函数进行处理

error.c     封装了几个报错函数，只有错误输出，没啥大用

cliftp.c     客户端的ftp文件，在用户分析了命令之后就调用这里封装的函数，主要是跟服务器端进行沟通，并将发送回来的数据显示到终端上。

servftp.c   服务器端的ftp文件，服务器接收到了客户端的请求之后，调用这里封装的函数，对其进行响应。

network.c   网络模块的初始化，服务器端 socket, bind, listen，客户端只socket。并且把命令分析函数也放在这里面了，因为实在不知道放在哪。。。



* makefile

  因为嫌每次都链接太多文件上去，就写了个简单的makefile，每次make就可以了，用完之后make clean

* 用法

  因为图测试方便，然后把端口和服务器地址都写死了

  ```C
  //打开服务器端程序
  ./server &
  //打开客户端程序
  ./client
  ```

  

* 演示截图

  ![这里写图片描述](https://img-blog.csdn.net/20180810164756855?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI0ODg5NTc1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
