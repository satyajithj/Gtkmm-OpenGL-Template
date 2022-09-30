#pragma once

#include <memory>

#include "common/appstate.h"

class BaseType {

public:
    BaseType() { };
    ~BaseType() { };

    std::shared_ptr<AppState> _AppState {nullptr};

    void setAppState(std::shared_ptr<AppState> p_AppState) {
        _AppState = p_AppState;
    }
};