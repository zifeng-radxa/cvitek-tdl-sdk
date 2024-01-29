
# TDL_SDK Face Recognition Compilation

> TDL_SDK Repository: https://github.com/milkv-duo/cvitek-tdl-sdk
>
> Use cross-compilation toolchain https://sophon-file.sophon.cn/sophon-prod-s3/drive/23/03/07/16/host-tools.tar.gz



## PC Side

- Create project directory

  ```bash
  mkdir tdl_sdk
  cd tdl_sdk
  ```

- Obtain cross-compilation toolchain and configure the environment

  ```bash
  wget https://sophon-file.sophon.cn/sophon-prod-s3/drive/23/03/07/16/host-tools.tar.gz
  tar -xvf host-tools.tar.gz
  cd host-tools
  export PATH=$PATH:$(pwd)/gcc/riscv64-linux-musl-x86_64/bin
  ```

- Get the TDL_SDK repository

  ```bash
  git clone https://github.com/milkv-duo/cvitek-tdl-sdk
  ```

- Compile the face recognition code

  ```bash
  cd cvitek-tdl-sdk/face_detection
  chmod +x compile_fd.sh
  ./compile_fd.sh
  ```

- Transfer the compiled *sample_vi_fd* and *scrfd_768_432_int8_1x.cvimodel* model to Duo 256

  ```bash
  scp sample_vi_fd scrfd_768_432_int8_1x.cvimodel root@192.168.42.1:/root/
  ```

## Board Side

- Configure the inference environment

  ```bash
  export LD_LIBRARY_PATH='/mnt/system/lib'
  ```

- Run the face recognition binary file

  ```bash
  cd /root/
  ./sample_vi_fd ./scrfd_768_432_int8_1x.cvimodel
  ```

- Terminal output

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



## PC Side: Stream using VLC

Open VLC media player, click on Media in the top left corner, select Open Network Stream.

Enter the URL and click Play

```bash
rtsp://192.168.42.1/h264
```

![](/home/zifeng/git_clone/cvitek-tdl-sdk/sample/face_detection/1.png)

![](/home/zifeng/git_clone/cvitek-tdl-sdk/sample/face_detection/2.png)
