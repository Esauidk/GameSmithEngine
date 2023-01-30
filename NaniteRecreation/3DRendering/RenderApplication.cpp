#include "RenderApplication.h"

RenderApplication::RenderApplication() :wnd(800, 600, "Test") {}

RenderApplication::~RenderApplication() {

}

int RenderApplication::Execute() {
    while (true) {
        if (const auto ecode = Window::ProcessMessages()) {
            return *ecode;
        }
    }
}
