#include "destruction_stack.h"


void DestructionStack::push(std::function<void()> destructor) {
	destructors_.push(destructor);
}


DestructionStack::~DestructionStack() {
	while(!destructors_.empty()) {
		std::function<void()> destructor = destructors_.top();
		destructors_.pop();
		destructor();
	}
}
