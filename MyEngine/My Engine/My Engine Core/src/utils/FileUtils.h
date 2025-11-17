#pragma once
#include<string>
namespace myarcane {
	class FileUtils {
	public:
		static std::string readFile(const char* filepath);
	};
}