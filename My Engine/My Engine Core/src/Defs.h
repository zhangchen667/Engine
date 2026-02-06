#pragma once
//该头文件用于定义引擎核心模块的宏定义


#define V_SYNC 1	//1启动垂直同步(即渲染刷新率和屏幕刷新率相同)
#define FULLSCREEN_MODE 0//1全屏模式
#define SHOW_MOUSE 0//1显示鼠标光标
#define MSAA_SAMPLE_AMOUNT 4//多重采样抗锯齿采样数量,即每个像素采样次数,一般为2的幂次方(2,4,8等)
#define ANISOTROPIC_FILTERING 4.0f//各向异性过滤等级,一般为1,2,4,8,16,数值越大质量越高但性能消耗也越大
//各向异性就是