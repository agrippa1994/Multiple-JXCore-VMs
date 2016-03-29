#pragma once

template<typename ...T>
std::shared_ptr<VM> VM::createVM(T&&... args) {
	auto vm = std::shared_ptr<VM>(new VM(args...));
	virtualMachines_[vm->id_] = vm;
	return vm;
}

template<class RetType, class T, class, class ...P>
RetType VM::executeWithReturn(T fnc, P&&... args) {
	RetType val;
	queue_.dispatch_sync([&] {
		val = fnc(args...);
	});
	return val;
}

template<class RetType, class T, class, class ...P>
void VM::executeWithReturn(T fnc, P&&... args) {
	queue_.dispatch_sync([&] {
		fnc(args...);
	});
}

template<class T, class ...P>
typename std::result_of<T(P...)>::type VM::operator()(T func, P&&... p) {
	return executeWithReturn<std::result_of<decltype(func)&(P...)>::type>(func, p...);
}

template<class T, class ...P>
typename std::result_of<T(P...)>::type VM::exec(T func, P&&... p) {
	return (*this)(func, p...);
}
