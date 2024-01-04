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
So I've done a majority of reasearch today, particularly with chatGPT, which surprisingly is a very good teacher when you want advice or to ruberduck with someone. I'm going to go with a full descriptor table approach for all shader specific arguements. 
Inputs that an artist or programer making a shader would want. Anything else like Camera Projection, or static parameters will be baked into the root signature as a direct descriptor or a direct constant. I've been looking at the Unreal Engine 5 source code to get a snese of this.
But basically I'm thinking to have resource a heap manager that will pass out heaps to shaders. When shaders are no longer using said shaders, it can give it back to the heap manager to reuse for some other shader. Each shader will have its own seperate heap. I still need to figure out
how different shader instances will deal with heaps (shared or unique?). I think it comes down to whether shader instances will attempt to be rendered at the same time. If they are being rendered at the same time, I can't let them use the same heap. If they are done sequentially
then I can let the shader instances touch the same heap resource without worring about undefined behaviors. Since this engine is currently single-threaded in terms of the engine logic, I think I will be going with the latter approach.

# 8/18/2023
Alreight here's a dump so I don't forogt all of this. I realize I'm trying to make hookups to the DirectX12 and expect the high level engine to be able to handle it. APIS like DX12 are too complicate to be able to handle the same way as APIs like openGL.
I need to handle this as a whole DirectX12 layer. First things first, I need to seperate the static objects from the DirectX12Context class. These need to be part of a DirectX12Core class. Afer that I need create a pipeline state class, this will hold the pipeline state object 
along with other state related information. Last but not least, I need to create a mapping of the root signature to be able to tell where to place resources. This will require looking through the root signature description generated by the root signature builder. Use AnalysisSignature 
l from Unreal Engine as inspiration.

# 8/25/2023
Doing a quick notebook entry, I have now have a root signstuare and PSO that can be created with an arguement builder type of object. Along with that I have a state class to keep track of the current state of the DirectX12 Layer. Here's my thinking right now, graphic APIs like DirectX12, 
OpenGL, etc have similarities but have difference that are very opposite of each ofher, their systems rely on these differences to the point that it's hard to abstract them without catering to specific implementation details of certain APIs. So here's what I'm going to do. I will abstract 
modules of the layer (ex: VertexShader) in terms of high engine level needs while letting the specific classes have special implmentation details that the API layers can cast to their respective classes to use. This way I not stepping on toes/making it harder to implement certain APIs

# 8/27/2023
Realizing I'm having a hard time organizing what kind of task I want to work on, decided to go back and add to my old trello board and follow along wiht it https://trello.com/c/8R3B7KdP/8-directx12-layer. Next steps are going to be keeping track of resources (SRV, CBV, and UAV)

# 9/4/2023
Now tha I've simplified and enacapsulated the rendering system with their respect Render API enviornments, I can move on focusing on the shader part of the pipeline. I need to be able to tell what parameters are in the shader and be able to keep values for them. This is going to involve shader 
reflection. I think when addressing DX12, I'm going to keep a giant root signature for now instead of making specialized ones per shader. I will be using reflection to figure out what resources need to be made and where to bind them. ONWARDS TO MORE!

# 9/14/2023
Now that views (besides UAV) can be added to the layer and saved for state, it's time to do some clean up. Need to add more state functions for rendering specifications (viewport, rect). Also want to refactor and add a linter to make this good. Will also go back and comment on fuctions to give them purpose.
This wil be in tommorows commit.

# 9/28/2023
With the renderer subsytem being at a point that I feel comfortable with, I think it's time to look into other required subsystems for this engine. Going to start looking into making a resource manager subsystem (loading textures, mesh data, shaders, etc). Been reading into the Game Engine Arichtecture book by Jason Gregory so I have an
idea of where to start going. We have the layer stack that we can use to inject subsystems into and make sure their update logic is ran. But what about inter subsystem communication? I'm thinking to extend the event system by adding more events and use that to support communication between the subsystems. No need to expose public methods. The only concern now is making sure I allow subsystems to get the list of all events available in the engine. I'm thinking to have all subsystems have a public method that exposes the list of events their emit. 

# 12/26/2023
Wow, I haven't wrote in here for a while. Don't thik I can make much progress tonight so I'm writing things that need to be looked at the next time:

- Constant Buffer needs to be verified that fence/signal setup is written correctly
- Texture Resource needs to be verified that fence/signal setup is wirtten correctly
- Logic for notifying that frame is completed and ready for swapping
- Logic for swapping back buffers in presentation

# 1/3/2024
Finally I think I know what is causing this GB level memory usage. It appears allocators are constantly unavailable for usage. What's weird is unavailabiltiy is determined by complete fence value. Before swapping, the CPU waits for all GPU tasks to be completed, so allocators should be available in the next frame recording. Why is the memory constantly growing??????????? Will need to look into this.