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