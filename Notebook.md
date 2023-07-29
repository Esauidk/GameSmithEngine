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

# 7/2/2022
Been working on making the engine Render agnostics, so I'm adding in interfaces to represent basic compoenents of a modern Renderer.
I'm trying to be careful though, it seems like OpenGL and DX12 have very unique implementations, but I think I can find the similarities between components.

Completed: Vertex Buffer, Index Buffer, Shaders, and PipelineStateObject. Currently working on root signature. Honestly trying to piece together the similairites is 
helping me better understand DX12 components more than I thought