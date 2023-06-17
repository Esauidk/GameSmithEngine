#define DXWND_EXCEPT(hr) Window::HrException(__LINE__, __FILE__, hr)
#define DXWND_LAST_EXCEPT Window::HrException(__LINE__, __FILE__, GetLastError())
#define DXWND_NOGFX_EXCEPT Window::NoGfxException(__LINE__,__FILE__)
