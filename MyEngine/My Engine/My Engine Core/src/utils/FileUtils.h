#pragma once
#include<string>
namespace myarcane {
	class FileUtils {
	public:
		static std::string readFile(const char* filepath) {
			FILE* file = fopen(filepath, "rt");//打开文件
			fseek(file, 0, SEEK_END);//移动文件指针到文件末尾
			unsigned long length = ftell(file);//获取当前指针位置（即文件总字节数）
			char* data = new char[length + 1];//为文件内容分配内存
			memset(data, 0, length + 1);//初始化内存
			fseek(file, 0, SEEK_SET);		//移动文件指针到文件开头
			fread(data, 1, length, file);	//读取文件内容到内存
			fclose(file);			

			std::string result(data);//将内容存储到string中
			delete[] data;
			return result;//返回文件内容
		}
	};
}