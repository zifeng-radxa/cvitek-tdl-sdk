# TDL_SDK demo 手册

> TDL_SDK 仓库： https://github.com/milkv-duo/cvitek-tdl-sdk
>
> 使用交叉编译工具链 https://sophon-file.sophon.cn/sophon-prod-s3/drive/23/03/07/16/host-tools.tar.gz



## TDL_SDK 与交叉编译环境配置

- 获取交叉编译工具链并配置环境

  ```bash
  wget https://sophon-file.sophon.cn/sophon-prod-s3/drive/23/03/07/16/host-tools.tar.gz
  tar -xvf host-tools.tar.gz
  cd host-tools
  export PATH=$PATH:$(pwd)/gcc/riscv64-linux-musl-x86_64/bin
  ```

- 获取TDL_SDK仓库

  ```bash
  git clone https://github.com/zifeng-radxa/cvitek-tdl-sdk.git
  ```

  

## TDL_SDK 人脸识别编译

### PC端

- 人脸识别代码编译

  ```bash
  cd cvitek-tdl-sdk/sample/face_detection
  chmod +x compile_fd.sh
  ./compile_fd.sh
  ```

- 将编成成的 *sample_vi_fd* 和 scrfd_768_432_int8_1x.cvimodel 模型传输到 Duo 256中

  ```bash
  scp sample_vi_fd scrfd_768_432_int8_1x.cvimodel root@192.168.42.1:/root/
  ```

  

### 板端

- 配置推理环境

  ```bash
  export LD_LIBRARY_PATH='/mnt/system/lib'
  ```

- 运行人脸识别二进制文件

  ```bash
  cd /root/
  export PATH=$(pwd):$PATH
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

  

### PC 端利用 VLC 拉流

打开 VLC media player, 点击左上角Media, 选择 Open Network Stream

输入URL, 点击Play

```bash
rtsp://192.168.42.1/h264
```



![image-20240129182607583](https://github.com/zifeng-radxa/cvitek-tdl-sdk/blob/master/sample/face_detection/1.png?raw=true)

![image-20240129183019338](https://github.com/zifeng-radxa/cvitek-tdl-sdk/blob/master/sample/face_detection/2.png?raw=true)



## TDL_SDK YOLOv5 编译

### PC端交叉编译YOLO程序

- 打开sample/cvi_yolo目录

   ```bash
cd cvitek-tdl-sdk/sample/cvi_yolo
   ```

- 编译yolo系列程序得到sample_yolov5二进制文件

   ```bash
   make KERNEL_ROOT=../../../cvitek-tdl-sdk/sample MW_PATH=../../../cvitek-tdl-sdk/sample/3rd/middleware/v2 TPU_PATH=../../../cvitek-tdl-sdk/sample/3rd/tpu IVE_PATH=../../../cvitek-tdl-sdk/sample/3rd/ive USE_TPU_IVE=ON CHIP=CV180X SDK_VER=musl_riscv64 -j10
   ```
   
- *(Optional)删除生成的目标二进制文件*

   ```bash
   make clean
   ```



### 导出yolov5s.onnx模型

首先载yolov5官方仓库代码，地址如下: [ultralytics/yolov5: YOLOv5 🚀 in PyTorch > ONNX > CoreML > TFLite] (https://github.com/ultralytics/yolov5)

```bash 
git clone https://github.com/ultralytics/yolov5.git
```

获取yolov5的.pt格式的模型，例如下载yolov5s模型的地址： [yolov5s](https://github.com/ultralytics/yolov5/releases/download/v7.0/yolov5s.pt)

```bash
wget https://github.com/ultralytics/yolov5/releases/download/v7.0/yolov5s.pt
```

将 cvitek-tdl-sdk/sample/yolo_export/yolov5_export.py 复制到yolov5仓库目录下

利用 yolov5_export.py 替换forward函数让yolov5的后处理由RISC-V来做并导出onnx格式模型

```bash
python3 yolov5_export.py --weights ./yolov5s.pt --img-size 640 640
```

参数解释 

--weights pytorch 模型路径

--img-size 图片输入大小



### TPU-MLIR 转换模型

请参考[TPU-MLIR文档](https://github.com/sophgo/tpu-mlir)配置好TPU-MLIR工作环境，参数解析请参考[TPU-MLIR文档](https://github.com/sophgo/tpu-mlir)

具体实现步骤分三步

- `model_transform.py` 将onnx模型转化成mlir中间格式模型

  onnx -> model_transform.py -> mlir

- `run_calibration.py` 生成int8量化校准表

​	calibration_set -> run_calibration.py -> calibration_table

- `model_deploy.py` 将 mlir 配合 int8量化表生成 用于TPU 推理的 cvimodel

​	mlir + calibration_table  ->model_deploy.py -> cvimodel

#### onnx转MLIR

```bash]
model_transform.py \
--model_name yolov5s \
--model_def yolov5s.onnx \
--input_shapes [[1,3,640,640]] \
--mean 0.0,0.0,0.0 \
--scale 0.0039216,0.0039216,0.0039216 \
--keep_aspect_ratio \
--pixel_format rgb \
--test_input ../image/dog.jpg \
--test_result yolov5s_top_outputs.npz \
--mlir yolov5s.mlir
```

转换成mlir文件之后，会生成一个yolov5s_in_f32.npz文件，该文件是模型的输入文件

#### MLIR转INT8模型 (仅支持INT8量化模型)

转 INT8 模型前需要跑 calibration.py，得到校准表；输入数据的数量根据情况准备 100~1000 张左右。

然后用校准表生成 cvimodel 生成校对表的图片尽可能和训练数据分布相似

```bash
run_calibration.py yolov5s.mlir \
	--dataset ../COCO2017 \
    --input_num 100 \
    -o yolov5s_cali_table
```

生成 int8 对称cvimodel

```bash
model_deploy.py \
--mlir yolov5s.mlir \
--quant_input --quant_output \
--quantize INT8 \
--calibration_table yolov5s_cali_table \
--processor cv181x \
--test_input yolov5s_in_f32.npz \
--test_reference yolov5s_top_outputs.npz \
--tolerance 0.85,0.45 \
--model yolov5_cv181x_int8_sym.cvimodel
```

编译完成后，会生成名为yolov5_cv181x_int8_sym.cvimodel的文件

*(Optional)生成int8 非对称 cvimodel*

```bash
model_deploy.py \
--mlir yolov5s.mlir \
--quant_input --quant_output \
--quantize INT8 --asymmetric \
--calibration_table yolov5s_cali_table \
--processor cv181x \
--test_input yolov5s_in_f32.npz \
--test_reference yolov5s_top_outputs.npz \
--tolerance 0.85,0.45 \
--model yolov5_cv181x_int8_asym.cvimodel
```

编译完成后，会生成名为yolov5_cv181x_int8_asym.cvimodel的文件



### 板端推理

将编译好的 sample_yolov5, cvimodel, 要推理的jpg图片拷贝到板端然后执行二进制程序

```bash
export LD_LIBRARY_PATH='/mnt/system/lib'
./sample_yolov5 ./yolov5_cv181x_int8_asym.cvimodel  000000000113.jpg 
```

```bash
[root@milkv-duo]~/data/test_make# ./sample_yolov5 ./yolov5_cv181x_int8_asym.cvim
odel  000000000113.jpg 
[ 5665.088539] vb has already inited, set_config cmd has no effect
version: 1.4.0
yolov5s Build at 2024-01-30 16:32:00 For platform cv181x
Max SharedMem size:5734400
model opened:./yolov5_cv181x_int8_asym.cvimodel
detect res: 340.399902 96.056824 415.000000 423.052612 0.866147 0
detect res: 149.599243 52.756699 344.154053 434.715759 0.864127 0
detect res: 165.295807 434.305786 389.069794 546.548950 0.739044 55
detect res: 5.554703 34.055344 161.554688 521.089905 0.718132 0
detect res: 79.427734 458.260071 124.559998 515.725830 0.677201 41
detect res: 256.286438 367.324158 303.713562 450.689941 0.598015 43
detect res: 282.405457 93.188477 309.046570 121.621582 0.555912 41
detect res: 281.694244 60.846092 309.968231 91.153908 0.520292 41
```









