# 摄像头基本配置

## 1.CUBEMX主要配置

- 时钟基本配置
- GPIO
  	- 软件I2C
  	- led

- SPI
- USART
- DCMI

###  1.软件I2C

找两个合适的gpio引脚配置软件I2C，这里用的PB8为SCL，PB9为SDA

![image-20260501210122316](readme.assets/image-20260501210122316.png)

​	I2C具体的配置代码沿用商家的sccb.h和sccb.c

剩下几个gpio分别是led的配置以及我记得是与spi背光有关的配置

### 2.SPI

沿用template里的SPI4配置即可

### 3.USART

配置的USART1，PA9为TX，PA10为RX，其余配置同模版工程

### 4.DCMI

- 先放上引脚的配置

![](readme.assets/联想截图_20260501210146(1).png)

- 然后是参数的配置

![image-20260501210326661](readme.assets/image-20260501210326661.png)

主要注意那几个polarity不要配错了

- DMCI里DMA这样配

![](readme.assets/联想截图_20260501210336(1).png)

主要注意一下要选DMA2 Stream 7，里面的配置按着图来就可以了

- 以及最后启用一下中断

  ![image-20260501210425764](readme.assets/image-20260501210425764.png)

  

### 5.时钟树

时钟树和模版工程一样

## 2.工程内配置

- 内存分配按这样配，我还不知道有的具体是为什么，反正按商家这样是可以的

![](readme.assets/联想截图_20260501210521(1).png)



- 工程内部修改步骤

### 1.导入文件

remove原模版工程里的OLED相关文件，导入sccb.c和sccb.h

### 2.修改dcmi

把My_template里的dcmi.c和dcmi.h里面USER CODE的部分复制粘贴进去

### 3.导入头文件

在AllHeader.h里导入各头文件，主要是dcmi和sccb

### 4.修改Mymain

把My_template中初始化和while里的USER CODE部分都加入Mymain里面

### 5.其他

主要就上述4点，应该问题不大了，如果编译报错再根据具体报错内容修改一下应该就可以了