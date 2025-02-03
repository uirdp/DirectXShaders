#include "VertexBuffer.h"
#include "Engine.h"
#include <d3dx12.h>

VertexBuffer::VertexBuffer(size_t size, size_t stride, const void* pInitData)
{
	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);

	auto hr = g_Engine->Device()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_pBuffer.GetAddressOf())
	);

	if(FAILED(hr))
	{
		printf("���_�o�b�t�@���\�[�X�̐����Ɏ��s\n");
		return;
	}

	m_View.BufferLocation = m_pBuffer->GetGPUVirtualAddress();
	m_View.SizeInBytes = static_cast<UINT>(size);
	m_View.StrideInBytes = static_cast<UINT>(stride);

	if(pInitData)
	{
		void* p;
		hr = m_pBuffer->Map(0, nullptr, &p);

		if(FAILED(hr))
		{
			printf("���_�o�b�t�@���\�[�X�̃}�b�v�Ɏ��s\n");
			return;
		}

		memcpy(p, pInitData, size);
		m_pBuffer->Unmap(0, nullptr);
	}

	m_IsValid = true;
}

D3D12_VERTEX_BUFFER_VIEW VertexBuffer::View() const
{
	return m_View;
}

bool VertexBuffer::IsValid()
{
	return m_IsValid;
}
