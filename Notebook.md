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

# 7/71/2023
With the RenderAgnostic architecture implemented, I can now move on to Job Submission creation! Currently DirectXRenderer has a Job Submission function, but I would like a RenderAgnostic version.
On top of that, I need to figure out how to handle binding in a RenderAgnostic fashion. DirectX12 needs to pass a commandList that has the Viewport, Rect, and RenderTarget already set, which is 
dependent on an instance of Renderer. I'm not sure if I want to support ulimited instances of Renderers, so I might create a Renderer Manager that will keep references to all Renderers and be able to do static functions
for DrawSubmissions too