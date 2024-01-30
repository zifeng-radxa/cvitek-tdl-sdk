# TDL_SDK Demo Manual

> TDL_SDK Repository: https://github.com/milkv-duo/cvitek-tdl-sdk
>
> Cross-compilation toolchain https://sophon-file.sophon.cn/sophon-prod-s3/drive/23/03/07/16/host-tools.tar.gz



## TDL_SDK and Cross-Compilation Environment Configuration

- Obtain the cross-compilation toolchain and configure the environment

  ```bash
  wget https://sophon-file.sophon.cn/sophon-prod-s3/drive/23/03/07/16/host-tools.tar.gz
  tar -xvf host-tools.tar.gz
  cd host-tools
  export PATH=$PATH:$(pwd)/gcc/riscv64-linux-musl-x86_64/bin
  ```

- Get the TDL_SDK repository

  ```bash
  git clone https://github.com/zifeng-radxa/cvitek-tdl-sdk.git
  ```



## TDL_SDK Face Recognition Compilation

### PC Side

- Compile the face recognition code

  ```bash
  cd cvitek-tdl-sdk/sample/face_detection
  chmod +x compile_fd.sh
  ./compile_fd.sh
  ```

- Transfer the compiled *sample_vi_fd* and *scrfd_768_432_int8_1x.cvimodel* model to Duo 256

  ```bash
  scp sample_vi_fd scrfd_768_432_int8_1x.cvimodel root@192.168.42.1:/root/
  ```

### Board Side

- Configure the inference environment

  ```bash
  export LD_LIBRARY_PATH='/mnt/system/lib'
  ```

- Run the face recognition binary file

  ```bash
  cd /root/
  export PATH=$(pwd):$PATH
  sample_vi_fd ./scrfd_768_432_int8_1x.cvimodel
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

### PC Side: Stream using VLC

Open VLC media player, click on Media in the top left corner, select Open Network Stream.

Enter the URL and click Play

```bash
rtsp://192.168.42.1/h264
```

![image-20240129182607583](https://github.com/zifeng-radxa/cvitek-tdl-sdk/blob/master/sample/face_detection/1.png?raw=true)

![image-20240129183019338](https://github.com/zifeng-radxa/cvitek-tdl-sdk/blob/master/sample/face_detection/2.png?raw=true)



## TDL_SDK YOLOv5 Compilation

### PC Side Cross-Compile YOLO Program

- Open the sample/cvi_yolo directory

  ```bash
  cd cvitek-tdl-sdk/sample/cvi_yolo
  ```

- Compile the YOLO series programs to get the sample_yolov5 binary file

  ```bash
  make KERNEL_ROOT=../../../cvitek-tdl-sdk/sample MW_PATH=../../../cvitek-tdl-sdk/sample/3rd/middleware/v2 TPU_PATH=../../../cvitek-tdl-sdk/sample/3rd/tpu IVE_PATH=../../../cvitek-tdl-sdk/sample/3rd/ive USE_TPU_IVE=ON CHIP=CV180X SDK_VER=musl_riscv64 -j10
  ```

- *(Optional) Remove the generated target binary files*

  ```bash
  make clean
  ```

### Export the yolov5s.onnx Model

First, clone the yolov5 official repository code, available at: [ultralytics/yolov5: YOLOv5 🚀 in PyTorch > ONNX > CoreML > TFLite](https://github.com/ultralytics/yolov5)

```bash
git clone https://github.com/ultralytics/yolov5.git
```

Get the .pt format model from yolov5, for example, download the yolov5s model from: [yolov5s](https://github.com/ultralytics/yolov5/releases/download/v7.0/yolov5s.pt)

```bash
wget https://github.com/ultralytics/yolov5/releases/download/v7.0/yolov5s.pt
```

Copy the `cvitek-tdl-sdk/sample/yolo_export/yolov5_export.py` to the yolov5 repository directory

Use `yolov5_export.py` to replace the forward function, allowing RISC-V to handle post-processing and export the model in onnx format

```bash
python3 yolov5_export.py --weights ./yolov5s.pt --img-size 640 640
```

Parameter explanation:

--weights: PyTorch model path

--img-size: Image input size

### TPU-MLIR Model Conversion

Please refer to the [TPU-MLIR documentation](https://github.com/sophgo/tpu-mlir) to set up the TPU-MLIR working environment. For parameter analysis, refer to the [TPU-MLIR documentation](https://github.com/sophgo/tpu-mlir)

The specific implementation steps are divided into three steps:

- `model_transform.py`: Convert the onnx model to the mlir intermediate format

  onnx -> model_transform.py -> mlir

- `run_calibration.py`: Generate the int8 quantization calibration table

  calibration_set -> run_calibration.py -> calibration_table

- `model_deploy.py`: Generate the cvimodel for TPU inference using mlir and the int8 quantization table

  mlir + calibration_table -> model_deploy.py -> cvimodel

#### onnx to MLIR

```bash
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

After converting to the mlir file, a `yolov5s_in_f32.npz` file will be generated, which is the input file for the model.

#### MLIR to INT8 Model (Supports only INT8 quantization models)

Before converting to the INT8 model, run `calibration.py` to obtain the calibration table. Prepare about 100 to 1000 input data.

Generate int8 symmetric cv

imodel:

```bash
run_calibration.py yolov5s.mlir \
	--dataset ../COCO2017 \
    --input_num 100 \
    -o yolov5s_cali_table
```

Generate int8 asymmetric cvimodel:

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

After compilation, the file `yolov5_cv181x_int8_asym.cvimodel` will be generated.

### Inference on the Board

Copy the compiled `sample_yolov5`, `cvimodel`, and the image to be inferred to the board, then execute the binary program:

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