#include "RenderApplication.h"
#include <iostream>

RenderApplication::RenderApplication(RendererOptions renderType) : wnd(1080, 600, "Render Application"), timer() {
    switch (renderType) {
        case RendererOptions::DirectX:
            std::cout << "Making DirectX Renderer" << std::endl;
            renderer = std::make_unique<DirectXRenderer>(wnd.GetWindowHandle());
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

        Tick();
    }
}

void RenderApplication::Tick() {
    auto dt = timer.Mark();
    renderer->StartFrame();
    renderer->EndFrame();
    throw std::exception("Ran one loop, synchronization issues");
}