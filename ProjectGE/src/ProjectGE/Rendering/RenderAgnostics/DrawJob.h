#pragma once
namespace ProjectGE {
	// Adapter Pattern Interface for performing commands to Render APIs
	class DrawJob {
	public:
		virtual void SetVertex() = 0;
		virtual void SetIndex() = 0;
		virtual void SetShaderConstant() = 0;
		virtual void SetTopology() = 0;
		virtual void SetPipeline() = 0;
		virtual void SetShaderArgList() = 0;
		virtual void DrawIndex() = 0;
	};
};
