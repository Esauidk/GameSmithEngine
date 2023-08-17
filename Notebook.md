# 7/13/2023
Looking into a math library. Still contemplating whether I create my own math library or use DirectXMath.
It seems very powerful and well made. Looking into SIMD-friendly math library creation, it's pretty new for me. Compiler intrisics huh?
I'll keep looking into what is generally used today by game studios.

Looking at how performance and cross-platform is a desire, GLM is looking more intising than DirectXMath

# 7/22/2023
Finally got a nice triangle! Wooot! Some things to note though, viewports are a thing to look into, a shader 
loader pipelines needs to be addressed. Maybe this is a Asset Manager Issue than a render pipeline issue. Basically the only problem
needed to be solved for the shaders are providing the correct path for the shader .cso file. 

Now that I'm thinking, yes, this is an Asset Management Problem. Anyways, I'm happy the current progress of the engine

# 7/2/2023
Been working on making the engine Render agnostics, so I'm adding in interfaces to represent basic compoenents of a modern Renderer.
I'm trying to be careful though, it seems like OpenGL and DX12 have very unique implementations, but I think I can find the similarities between components.

Completed: Vertex Buffer, Index Buffer, Shaders, and PipelineStateObject. Currently working on root signature. Honestly trying to piece together the similairites is 
helping me better understand DX12 components more than I thought.

# 7/31/2023
With the RenderAgnostic architecture implemented, I can now move on to Job Submission creation! Currently DirectXRenderer has a Job Submission function, but I would like a RenderAgnostic version.
On top of that, I need to figure out how to handle binding in a RenderAgnostic fashion. DirectX12 needs to pass a commandList that has the Viewport, Rect, and RenderTarget already set, which is 
dependent on an instance of Renderer. I'm not sure if I want to support ulimited instances of Renderers, so I might create a Renderer Manager that will keep references to all Renderers and be able to do static functions
for DrawSubmissions too

# 8/1/2023
After hours of examining how Unreal Engine 5 interacts with the DirectX 12 API, It appears rendering is done on a single thread while other threads are used for other components of the engine. Ex: Game thread. It 
keeps track of a single command list at a time. When the command list is done it is submitted into the queue and a new command list is acquired. I'm going to change the current architecture to work like that instead. I want sleep btw

# 8/4/2023
Okay, this architecture is really messy and I want to clean it up as fast as I can, but first things first is to get it to compile and run. Currently there are new context classes for command lists, they hold a single ongoing command list
at a time and you can access it and do what you need to do it at anytime. This was learned from looking at Unreal Engine 5's  source code. The unfortunate thing is this engine is not multi-threaded yet, meaning that I can't have completed
be submitted in a seperate thread in teh background. This means I have to start executing it as soon as its complete. I will look into added a submission thread later but my main goal right now is to decouple the command lists from the bind
function of my bindableresources.

# 8/5/2023
API Agnostic code has been qdded now I just need to try using it. An example will porbably be recreating the triangle demo using the commands

# 8/13/2023
So.... I managed to find a way for shaders to have a modular input, but there are some flaws to this solution. 1) The shaders need to know the exact structure of the data and define it in code (this is because the data will come as a data array). 
This is not that bad of an issue since the intent is for shaders to define their inputs. 2) The inputs need to be known before the storage for it is generated. I need to find a way to extract the input from the compiled shader. Next I'm going to look into 
material systems.

# 8/16/2023
So I've done a majority of reach today, particularly with chatGPT, which surprisingly is a very good teacher when you want advice or to ruberduck with someone. I'm going to go with a full descriptor table approach for all shader specific arguements. 
Inputs that an artist or programer making a shader would want. Anything else like Camera Projection, or static parameters will be baked into the root signature as a direct descriptor or a direct constant. I've been looking at the Unreal Engine 5 source code to get a snese of this.
But basically I'm thinking to have resource a heap manager that will pass out heaps to shaders. When shaders are no longer using said shaders, it can give it back to the heap manager to reuse for some other shader. Each shader will have its own seperate heap. I still need to figure out
how different shader instances will deal with heaps (shared or unique?). I think it comes down to whether shader instances will attempt to be rendered at the same time. If they are being rendered at the same time, I can't let them use the same heap. If they are done sequentially
then I can let the shader instances touch the same heap resource without worring about undefined behaviors. Since this engine is currently single-threaded in terms of the engine logic, I think I will be going with the latter approach
