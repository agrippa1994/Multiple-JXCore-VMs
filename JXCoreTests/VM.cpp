#include "VM.hpp"

std::map<std::thread::id, std::shared_ptr<class VM>> VM::virtualMachines_;

bool VM::hasVM() {
	return virtualMachines_.find(std::this_thread::get_id()) != virtualMachines_.end();
}

std::shared_ptr<VM> VM::currentVM() {
	return virtualMachines_.at(std::this_thread::get_id());
}

std::shared_ptr<VM> VM::deleteVM() {
	auto vm = currentVM();
	virtualMachines_.erase(std::this_thread::get_id());
	return vm;
}

const decltype(VM::virtualMachines_)& VM::virtualMachines() {
	return virtualMachines_;
}

VM::VM(const std::string& content, bool isFile) {
	exec([&] {
		id_ = std::this_thread::get_id();

		JX_InitializeNewEngine();

		std::ifstream t(content);
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

		JX_DefineMainFile(str.c_str());
	});
}
