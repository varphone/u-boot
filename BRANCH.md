# 当前分支说明 #

当前分支为 `u-boot 2009.08` 版本，合并了明远智睿的 `u-boot-2009.08--svn56` 发行版代码。

在此基础上增加了 `CVR-MIL-V1`, `CVR-MIL-V1-VGA` 产品的支持。

> 备注：明远智睿的代码可以在百度网盘地址：`http://pan.baidu.com/s/1bpc95Or` 取得，网盘提取码为：`u22y`，其内里的原理图解压密码为：`MYZRIMX6EK200d34ac38a02345b1dbf56ea4c5502525a`。

## 1. 板子说明 ##

代号                | 说明
------------------- | --------------------------------------------------------
CVR-MIL-V1          | 军用车载行车记录仪，第一版，设备集成 LCD 屏显示。
CVR-MIL-V1-VGA      | 军用车载行车记录仪，第一版，外接 VGA 显示器显示。

代号格式说明：

- `CVR` 代表车载视频记录仪。
- `MIL` 代表军用。
- `V1` 表示第一版。
- `VGA` 表示在基础版本上定制的 `VGA` 输出版本。

## 2. 配置说明 ##

**U-Boot** 的板级配置文件存放在 `include/configs` 目录中，其格式为 `C` 头文件格式。

这里之说明我们产品之中所用到的各个配置，如下表：

配置                                      | 说明
----------------------------------------- | ----------------------------------
myimx6ek200_6q_cvr_mil_v1_config          | 用于 CVR-MIL-V1 四核版本。
myimx6ek200_6q_cvr_mil_v1_vga_config      | 用于 CVR-MIL-V1-VGA 四核版本。
myimx6ek200_6s_cvr_mil_v1_config          | 用于 CVR-MIL-V1 单核版本。
myimx6ek200_6s_cvr_mil_v1_vga_config      | 用于 CVR-MIL-V1-VGA 单核版本。

## 3. 编译说明 ##

通用命令格式：

```sh
make myimx6ek200_*_config
```

例如：编译用于 CVR-MIL-V1 的单核版本

```sh
make myimx6ek200_6s_cvr_mil_v1_config
make -j4
```
