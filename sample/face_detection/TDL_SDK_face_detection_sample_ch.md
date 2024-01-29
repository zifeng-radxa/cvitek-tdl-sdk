# TDL_SDK 人脸识别编译

> TDL_SDK 仓库： https://github.com/milkv-duo/cvitek-tdl-sdk
>
> 使用交叉编译工具链 https://sophon-file.sophon.cn/sophon-prod-s3/drive/23/03/07/16/host-tools.tar.gz
>



## PC端

- 创建工程目录

  ```bash
  mkdir tdl_sdk
  cd tdl_sdk
  ```

  

- 获取交叉编译工具链并配置环境

  ```bash
  wget https://sophon-file.sophon.cn/sophon-prod-s3/drive/23/03/07/16/host-tools.tar.gz
  tar -xvf host-tools.tar.gz
  cd host-tools
  export PATH=$PATH:$(pwd)/gcc/riscv64-linux-musl-x86_64/bin
  ```

- 获取TDL_SDK仓库

  ```bash
  git clone https://github.com/milkv-duo/cvitek-tdl-sdk
  ```

- 人脸识别代码编译

  ```bash
  cd cvitek-tdl-sdk/face_detection
  chmod +x compile_fd.sh
  ./compile_fd.sh
  ```

- 将编成成的 *sample_vi_fd* 和 scrfd_768_432_int8_1x.cvimodel 模型传输到 Duo 256中

  ```bash
  scp sample_vi_fd scrfd_768_432_int8_1x.cvimodel root@192.168.42.1:/root/
  ```

  

## 板端

- 配置推理环境

  ```bash
  export LD_LIBRARY_PATH='/mnt/system/lib'
  ```

- 运行人脸识别二进制文件

  ```bash
  cd /root/
  sample_vi_fd ./scrfd_768_432_int8_1x.cvimodel
  ```

- 终端打印

  ```bash
  Initialize VENC
  venc codec: h264
  venc frame size: 1920x1080
  Initialize RTSP
  rtsp://127.0.1.1/h264
  prio:0
  version: 1.4.0
  scrfd768432 Build at 2023-12-25 01:21:44 For platform cv181x
  Max SharedMem size:1658880
  ```

  

## PC 端利用 VLC 拉流

打开 VLC media player, 点击左上角Media, 选择 Open Network Stream

输入URL, 点击Play

```bash
rtsp://192.168.42.1/h264
```



![image-20240129182607583](https://github.com/zifeng-radxa/cvitek-tdl-sdk/blob/master/sample/face_detection/1.png?raw=true)

![image-20240129183019338](https://github.com/zifeng-radxa/cvitek-tdl-sdk/blob/master/sample/face_detection/2.png?raw=true)