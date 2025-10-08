#pragma once
#include <memory>
#include <vector>
#include "Window/BaseWindow.h"

class StackManager {
private:
    std::vector<std::unique_ptr<BaseWindow>> stack;

public:
    StackManager() = default;
    ~StackManager() = default;

    void pushWindow(std::unique_ptr<BaseWindow> window) {
        stack.push_back(std::move(window));
    }

    void popWindow() {
        if (!stack.empty())
            stack.pop_back();
    }

    BaseWindow* top() {
        return stack.empty() ? nullptr : stack.back().get();
    }
};
