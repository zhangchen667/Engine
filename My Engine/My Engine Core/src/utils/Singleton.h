#pragma once
//定义一个「单例模式的抽象基类」，封装单例模式的「核心约束逻辑」，让
//其他类继承它后，能快速实现符合规范的单例（无需重复编写禁止拷贝 / 移动的代码）

namespace myarcane {

	class Singleton {
	public:
		Singleton(const Singleton& singleton) = delete;  // Get rid of copy ctor
		Singleton(const Singleton&& singleton) = delete; // Get rid of move ctor

		Singleton& operator=(const Singleton& singleton) = delete;
		Singleton& operator=(const Singleton&& singleton) = delete;
	protected:
		Singleton() = default;
		virtual ~Singleton() = default;
	};

}