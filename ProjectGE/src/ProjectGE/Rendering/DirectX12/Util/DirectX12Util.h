#pragma once

#include <d3d12.h>
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Topology.h"

namespace ProjectGE {
	inline static D3D12_PRIMITIVE_TOPOLOGY_TYPE TranslateTopType(TopologyType top) {
		switch (top) {
		case TopologyType::Triangle:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		}
	};

	inline static D3D12_PRIMITIVE_TOPOLOGY TranslateTopListType(TopologyType top) {
		switch (top) {
		case TopologyType::Triangle:
			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}
	}
}
