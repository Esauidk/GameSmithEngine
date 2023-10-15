#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Topology.h"
#include <d3d12.h>

namespace ProjectGE {
	inline static D3D12_PRIMITIVE_TOPOLOGY_TYPE TranslateTopType(TopologyType top, bool tessel) {
		if (tessel) {
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
		}

		switch (top) {
		case TopologyType::Triangle:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		}

		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	};

	inline static D3D12_PRIMITIVE_TOPOLOGY TranslateTopListType(TopologyType top, bool tesselation) {
		if (tesselation) {
			return D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;
		}
		switch (top) {
		case TopologyType::Triangle:
			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}

		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
}
