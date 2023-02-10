#include "RenderApplication.h"
#include <iostream>

using namespace Render;

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
    renderer->CreateObject();
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
    renderer->DrawObject();
    renderer->EndFrame();
}