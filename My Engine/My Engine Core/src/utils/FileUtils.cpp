#include "FileUtils.h"
// 新增必要头文件：获取错误信息、类型匹配
#include <errno.h>
#include <string.h>
#include <cstdio>

namespace myarcane {
	std::string FileUtils::readFile(const char* filepath) {
		// 1. 打开文件 + 强制检查返回值
		FILE* file = fopen(filepath, "rb"); // 替换 rt 为标准 r
		if (!file) {
			// 输出错误原因（关键：定位文件打开失败的根源）
			fprintf(stderr, "文件打开失败！路径：%s，原因：%s\n", filepath, strerror(errno));
			return "";
		}

		// 2. 定位文件末尾 + 检查 fseek 失败
		if (fseek(file, 0, SEEK_END) != 0) {
			fprintf(stderr, "fseek失败！路径：%s，原因：%s\n", filepath, strerror(errno));
			fclose(file); // 必须关闭文件，避免句柄泄漏
			return "";
		}

		// 3. 获取文件长度 + 检查 ftell 失败
		long length = ftell(file); // 改用 long 匹配 ftell 返回值
		if (length < 0) {
			fprintf(stderr, "ftell失败！路径：%s，原因：%s\n", filepath, strerror(errno));
			fclose(file);
			return "";
		}
		// 处理空文件
		if (length == 0) {
			fclose(file);
			return "";
		}

		// 4. 分配内存 + 复位文件指针
		char* data = new char[length + 1];
		memset(data, 0, length + 1); // 确保字符串以 \0 结尾
		fseek(file, 0, SEEK_SET);

		// 5. 读取文件 + 检查读取完整性
		size_t readBytes = fread(data, 1, length, file);
		if (readBytes != static_cast<size_t>(length)) {
			fprintf(stderr, "文件读取不完整！路径：%s，预期读取%ld字节，实际读取%zu字节\n",
				filepath, length, readBytes);
			delete[] data; // 释放内存，避免泄漏
			fclose(file);
			return "";
		}

		// 6. 收尾：关闭文件、转换字符串、释放内存
		fclose(file);
		std::string result(data);
		delete[] data;

		return result;
	}
}