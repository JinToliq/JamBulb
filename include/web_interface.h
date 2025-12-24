#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "input.h"
#include "state.h"

class WebInterface {
public:
    WebInterface(Input& input, State& state);
    void begin();

private:
    Input& _input;
    State& _state;
    AsyncWebServer _server;
    
    void setupRoutes();
};
