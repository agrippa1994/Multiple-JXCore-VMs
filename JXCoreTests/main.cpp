#include <iostream>
#include "VM.hpp"


#pragma comment(lib, "jx.lib")

#include <thread>
#include <memory>
#include <functional>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <map>
#include <sstream>

void getThreadId(JXValue *args, int argc) {
	std::stringstream ss;
	ss << std::this_thread::get_id();
	JX_SetInt32(args + argc, std::stoi(ss.str().c_str()));
}

void uncaughtException(JXValue *args, int argc) {
	std::cout << "Uncaught exception: " << JX_GetString(&args[0]) << std::endl;
}

void Initialize() {
	JX_Initialize("", NULL);

	for (int i = 0; i < 20; i++) {
		auto p = VM::createVM("test.js", true);
		p->exec(JX_DefineExtension, "getThreadId", getThreadId);
		p->exec(JX_DefineExtension, "uncaughtException", uncaughtException);
		p->exec(JX_DefineFile, "vm", R"(
			process.on("uncaughtException", process.natives.uncaughtException);
			module.exports = process.natives;
		)");

		p->exec(JX_StartEngine);
	}
}

void OnTick() {
	for (auto& pair : VM::virtualMachines())
		pair.second->exec(JX_LoopOnce);
}

int main() {
	Initialize();

	for (;; std::this_thread::sleep_for(std::chrono::milliseconds(500)))
		OnTick();

	std::cin.get();
}
