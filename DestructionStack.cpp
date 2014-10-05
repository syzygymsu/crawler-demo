#include "DestructionStack.h"

void DestructionStack::push(std::function<void()> destructor) {
	destructors.push(destructor);
}

DestructionStack::~DestructionStack() {
	while(!destructors.empty()) {
		std::function<void()> destructor = destructors.top();
		destructors.pop();
		destructor();
	}
}
