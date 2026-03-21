# MyArcane Engine

一个基于 OpenGL 的轻量级 3D 渲染引擎，支持模型加载、光照系统、地形渲染、天空盒等核心图形学功能。

## 🎯 项目概述

MyArcane Engine 是一个用于学习和演示图形学原理的 3D 渲染引擎。项目采用模块化设计，实现了完整的渲染管线，包括模型加载、光照计算、后处理效果等核心功能。

### 核心特性

- **模型加载**：支持多种 3D 模型格式（OBJ、FBX、GLTF 等）的加载和渲染
- **光照系统**：实现 Blinn-Phong 光照模型，支持方向光、点光源、聚光灯
- **地形渲染**：基于高度图的地形生成，支持多纹理混合
- **天空盒**：立方体贴图实现的环境天空盒
- **后处理效果**：MSAA 抗锯齿、FXAA、帧缓冲后处理
- **相机系统**：第一人称相机控制，支持平滑移动
- **输入处理**：完整的键盘鼠标输入系统

## 🏗️ 项目结构

```
My Engine Core/
├── src/                          # 源代码
│   ├── graphics/                 # 图形渲染模块
│   │   ├── camera/               # 相机系统
│   │   │   ├── FPSCamera.cpp/h   # 第一人称相机
│   │   ├── renderer/             # 渲染器
│   │   │   ├── Renderer.cpp/h    # 主渲染器
│   │   │   ├── Renderable3D.cpp/h# 3D可渲染对象
│   │   ├── Mesh.cpp/h            # 网格数据管理
│   │   ├── MeshFactory.cpp/h     # 网格工厂
│   │   ├── Shader.cpp/h          # 着色器管理
│   │   ├── Skybox.cpp/h          # 天空盒
│   │   ├── Window.cpp/h          # 窗口管理
│   │   ├── model.cpp/h           # 模型加载（Assimp）
│   ├── platform/OpenGL/          # OpenGL 平台封装
│   │   ├── Buffer.cpp/h          # 缓冲区基类
│   │   ├── Framebuffer.cpp/h     # 帧缓冲对象
│   │   ├── IndexBuffer.cpp/h     # 索引缓冲
│   │   ├── VertexArray.cpp/h     # 顶点数组对象
│   │   ├── Utility.cpp/h         # OpenGL 工具函数
│   ├── shaders/                  # GLSL 着色器
│   │   ├── basic.vert/frag       # 基础着色器
│   │   ├── model.frag            # 模型渲染着色器
│   │   ├── terrain.frag          # 地形渲染着色器
│   │   ├── skybox.vert/frag      # 天空盒着色器
│   │   ├── directionalLight.frag # 方向光计算
│   │   ├── pointlight.frag       # 点光源计算
│   │   ├── spotlight.frag        # 聚光灯计算
│   │   ├── framebuffer.vert/frag # 后处理着色器
│   │   ├── fxaa.vert/frag        # FXAA抗锯齿
│   ├── terrain/                  # 地形模块
│   │   ├── Terrain.cpp/h         # 地形渲染
│   ├── utils/                    # 工具模块
│   │   ├── Logger.cpp/h          # 日志系统
│   │   ├── Timer.cpp/h           # 计时器
│   │   ├── Time.cpp/h            # 时间管理
│   │   ├── FileUtils.cpp/h       # 文件工具
│   │   ├── Singleton.h           # 单例模板
│   ├── Scene3D.cpp/h             # 3D场景管理
│   ├── main.cpp                  # 程序入口
│   ├── Defs.h                    # 全局定义
├── res/                          # 资源文件
│   ├── 3D_Models/                # 3D模型资源
│   │   ├── Crysis/               # Nanosuit模型
│   │   ├── Duck/                 # Duck模型
│   │   ├── Sponza/               # Sponza场景
│   ├── skybox/                   # 天空盒纹理
│   ├── terrain/                  # 地形资源
│   │   ├── heightMap.png         # 高度图
│   │   ├── blendMap.png          # 混合贴图
│   │   ├── grass.png             # 草地纹理
│   │   ├── dirt.png              # 泥土纹理
│   │   ├── sand.png              # 沙地纹理
│   │   ├── stone.png             # 石头纹理
│   ├── textures/                 # 其他纹理
├── logged_files/                 # 日志文件
```

## 🚀 快速开始

### 环境要求

- Windows 10/11
- Visual Studio 2019 或更高版本
- OpenGL 3.3+ 支持的显卡
- CMake 3.10+（可选）

### 依赖库

- **GLFW**：窗口和输入管理
- **GLEW**：OpenGL 扩展加载
- **Assimp**：3D 模型加载
- **GLM**：数学库（向量、矩阵运算）
- **stb_image**：图像加载

### 编译运行

1. 克隆仓库
```bash
git clone <repository-url>
cd "My Engine/My Engine Core"
```

2. 使用 Visual Studio 打开 `My Engine Core.vcxproj`

3. 配置项目属性（确保包含目录和库目录正确）

4. 编译并运行（F5）

### 控制说明

| 按键 | 功能 |
|------|------|
| `W/A/S/D` | 相机移动 |
| `鼠标移动` | 相机视角 |
| `鼠标滚轮` | 缩放 |
| `ESC` | 退出程序 |

## 🎨 渲染管线

```
┌─────────────────────────────────────────────────────────────┐
│                        渲染管线流程                          │
├─────────────────────────────────────────────────────────────┤
│  1. 输入处理 → 2. 场景更新 → 3. 渲染提交 → 4. 后处理          │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐     │
│  │  不透明物体  │───→│   深度测试   │───→│  颜色缓冲   │     │
│  │  (Terrain)  │    │             │    │             │     │
│  └─────────────┘    └─────────────┘    └─────────────┘     │
│                                                             │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐     │
│  │  透明物体   │───→│  排序(远→近) │───→│  混合渲染   │     │
│  │  (Model)    │    │             │    │             │     │
│  └─────────────┘    └─────────────┘    └─────────────┘     │
│                                                             │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐     │
│  │   天空盒    │───→│  深度测试   │───→│  最终输出   │     │
│  │  (Skybox)   │    │  (LEqual)   │    │             │     │
│  └─────────────┘    └─────────────┘    └─────────────┘     │
│                                                             │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐     │
│  │  MSAA渲染   │───→│  后处理效果  │───→│  屏幕显示   │     │
│  │  目标缓冲   │    │  (FXAA等)   │    │             │     │
│  └─────────────┘    └─────────────┘    └─────────────┘     │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

## 🔧 核心模块详解

### 1. 窗口系统 (Window)

封装 GLFW 窗口管理，提供：
- 窗口创建和销毁
- OpenGL 上下文管理
- 输入事件处理（键盘、鼠标）
- 垂直同步控制

### 2. 渲染系统 (Renderer)

核心渲染管理器，实现：
- 渲染队列管理
- 透明/不透明物体分离渲染
- 深度测试和混合
- MSAA 抗锯齿

### 3. 模型加载 (Model)

基于 Assimp 的模型加载器：
- 支持多种格式（OBJ、FBX、GLTF 等）
- 自动纹理加载
- 网格数据管理

### 4. 光照系统

Blinn-Phong 光照模型：
- **方向光**：模拟太阳光，有方向无位置
- **点光源**：模拟灯泡，有位置无方向，随距离衰减
- **聚光灯**：模拟手电筒，有位置和方向，有锥形范围

### 5. 地形系统 (Terrain)

高度图地形渲染：
- 基于灰度图生成地形高度
- 多纹理混合（草地、泥土、沙地、石头）
- 法线计算实现光照

### 6. 后处理系统

帧缓冲后处理：
- MSAA 多重采样抗锯齿
- FXAA 快速近似抗锯齿
- 可扩展的后处理效果框架

## 📊 性能优化

### 已实现优化

- **渲染排序**：透明物体按距离排序，确保正确混合
- **视锥剔除**：只渲染可见物体
- **MSAA**：多重采样抗锯齿，提升画质
- **实例化渲染**：支持批量渲染相同模型

### 可扩展优化

- [ ] 延迟渲染 (Deferred Shading)
- [ ] 遮挡剔除 (Occlusion Culling)
- [ ] 纹理图集 (Texture Atlas)
- [ ] LOD 系统 (Level of Detail)

## 🎓 学习资源

### 图形学基础

- [LearnOpenGL](https://learnopengl.com/) - 优秀的 OpenGL 教程
- [OpenGL Wiki](https://www.khronos.org/opengl/wiki/) - OpenGL 官方文档

### 项目相关

- [GLFW Documentation](https://www.glfw.org/documentation.html)
- [Assimp Documentation](https://assimp-docs.readthedocs.io/)
- [GLM Documentation](https://glm.g-truc.net/0.9.9/api/index.html)

## 📝 代码统计

| 类型 | 文件数 | 代码行数 |
|------|--------|----------|
| C++ 源文件 (.cpp) | 22 | ~1,665 行 |
| C++ 头文件 (.h) | 22 | ~664 行 |
| GLSL 着色器 | 15 | ~624 行 |
| **总计** | **59** | **~2,953 行** |

## 🤝 贡献指南

欢迎提交 Issue 和 Pull Request！

1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开 Pull Request

## 📄 许可证

本项目仅供学习交流使用。

## 🙏 致谢

- [LearnOpenGL](https://learnopengl.com/) - 项目主要参考教程
- [GLFW](https://www.glfw.org/) - 窗口管理库
- [Assimp](https://assimp.org/) - 模型加载库
- [GLM](https://glm.g-truc.net/) - 数学库

## 📧 联系方式

如有问题或建议，欢迎通过以下方式联系：

- 邮箱：[2271875988@qq.com]

---

**注意**：本项目为学习和面试演示用途，代码结构和实现方式可能不适合生产环境直接使用。