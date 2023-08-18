#pragma once
#include "gepch.h"
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"

#include "DirectX12Macos.h"

namespace ProjectGE {
	enum class RootParameterVisibility {
		Vertex,
		Pixel
		// MORE TO COME
	};

	enum class DescriptorRangeType {
		CBV,
		SRV,
		UAV,
		Sampler
		// MORE TO COME
	};

	inline D3D12_SHADER_VISIBILITY TranslateVisibilityFlags(RootParameterVisibility vis) {
		switch (vis) {
		case RootParameterVisibility::Vertex:
			return D3D12_SHADER_VISIBILITY_VERTEX;
		case RootParameterVisibility::Pixel:
			return D3D12_SHADER_VISIBILITY_PIXEL;
		default:
			return D3D12_SHADER_VISIBILITY_ALL;
		}
	}

	inline D3D12_DESCRIPTOR_RANGE_TYPE TranslateRangeType(DescriptorRangeType range) {
		switch (range) {
		case DescriptorRangeType::CBV:
			return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		case DescriptorRangeType::SRV:
			return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		case DescriptorRangeType::UAV:
			return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
		case DescriptorRangeType::Sampler:
			return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
		default:
			return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		}
	}

	inline D3D12_DESCRIPTOR_RANGE_FLAGS TranslateRangeFlag(DescriptorRangeType range) {
		switch (range) {
		case DescriptorRangeType::CBV:
			return D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
		case DescriptorRangeType::SRV:
			return D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
		case DescriptorRangeType::UAV:
			return D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
		case DescriptorRangeType::Sampler:
			return D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
		}
	}


	struct DirectX12RootSignatureBuilder
	{
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;
		UINT registerSpace = 0;
		std::vector<CD3DX12_DESCRIPTOR_RANGE1> ranges;
		std::vector<CD3DX12_ROOT_PARAMETER1> parameters;
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC desc;

		struct TableMetadata {
			RootParameterVisibility visibility;
			DescriptorRangeType rangeType;
			UINT numDescriptor;
			D3D12_DESCRIPTOR_RANGE_FLAGS flags;
		};

		std::vector<TableMetadata> tables;

		void SetRegisterSpace(UINT newRegisterSpace) {
			registerSpace = newRegisterSpace;
		}

		void SetRootFlag(D3D12_ROOT_SIGNATURE_FLAGS newFlag) {
			flags = newFlag;
		}

		void AddRootFlag(D3D12_ROOT_SIGNATURE_FLAGS newFlag) {
			flags |= newFlag;
		}

		void AddConstant(UINT num32Constant, UINT reg, UINT space) {
			CD3DX12_ROOT_PARAMETER1& parm = parameters.emplace_back();
			parm.InitAsConstants(num32Constant, reg, space);
		}

		void AddShaderResource(UINT reg, UINT space) {
			CD3DX12_ROOT_PARAMETER1& parm = parameters.emplace_back();
			parm.InitAsShaderResourceView(reg, space);
		}

		void AddTable(RootParameterVisibility visibility, DescriptorRangeType rangeType, UINT numDescriptor, D3D12_DESCRIPTOR_RANGE_FLAGS flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE) {
			TableMetadata data{ visibility, rangeType, numDescriptor, flags };
			tables.emplace_back(data);
		}

		const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& Build() {
			ranges.resize(tables.size());

			for (int i = 0; i < tables.size(); i++) {
				const TableMetadata data = tables[i];

				D3D12_DESCRIPTOR_RANGE_FLAGS flags = data.flags ? data.flags : TranslateRangeFlag(data.rangeType);
				CD3DX12_DESCRIPTOR_RANGE1& range = ranges[i];
				range.Init(TranslateRangeType(data.rangeType), data.numDescriptor, 0, registerSpace, flags);

				CD3DX12_ROOT_PARAMETER1& parm = parameters.emplace_back();
				parm.InitAsDescriptorTable(1, &range, TranslateVisibilityFlags(data.visibility));

				bool res = false;
			}

			desc.Init_1_1((UINT)parameters.size(), parameters.data(), 0, nullptr, flags);

			return desc;
		}
	};

	inline void CreateMaxTables(DirectX12RootSignatureBuilder& builder, RootParameterVisibility visibility) {
		builder.AddTable(visibility, DescriptorRangeType::CBV, MAX_CBV);
		builder.AddTable(visibility, DescriptorRangeType::SRV, MAX_SRV);
		builder.AddTable(visibility, DescriptorRangeType::Sampler, MAX_SAMPLER);
	}

	inline void CreateLargeGraphicsRootSignature(DirectX12RootSignatureBuilder& builder, D3D12_ROOT_SIGNATURE_FLAGS flags) {
		builder.SetRootFlag(flags | D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		CreateMaxTables(builder, RootParameterVisibility::Vertex);
		CreateMaxTables(builder, RootParameterVisibility::Pixel);
	}
};


