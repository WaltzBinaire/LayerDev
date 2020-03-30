#pragma once
#include "ofMain.h"
#include <functional>

struct UniformFunction {
    UniformFunction() {
    }

    UniformFunction(std::function<void(ofShader*)> _assign) {
        assign = _assign;
    }

    std::function<void(ofShader*)>  assign;
    bool                            present = true;
};

typedef map<string, UniformFunction> UniformFunctionsList;

