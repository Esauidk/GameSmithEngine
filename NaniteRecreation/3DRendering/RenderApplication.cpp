#include "RenderApplication.h"
#include <iostream>

RenderApplication::RenderApplication(RendererOptions renderType) : wnd(1080, 600, "Render Application") {
    switch (renderType) {
        case RendererOptions::DirectX:
            std::cout << "Making DirectX Renderer" << std::endl;
            renderer = std::make_unique<DirectXRenderer>();
            break;
    }

    renderer->Initialize(wnd.GetWindowHandle());

}


RenderApplication::~RenderApplication() {

}

int RenderApplication::Execute() {
    while (true) {
        if (const auto ecode = Window::ProcessMessages()) {
            return *ecode;
        }
    }
}
