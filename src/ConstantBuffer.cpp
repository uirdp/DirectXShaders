#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer(size_t size)
{
	size_t align = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
	UINT64 sizeAligned = (size + (align - 1)) & ~(align - 1);

	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(sizeAligned);

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
		printf("�萔�o�b�t�@���\�[�X�̐����Ɏ��s\n");
		return;
	}

	// �X�V����̂�Map�����܂�
	hr = m_pBuffer->Map(0, nullptr, &m_pMappedPtr);
	if(FAILED(hr))
	{
		printf("�萔�o�b�t�@���\�[�X�̃}�b�v�Ɏ��s\n");
		return;
	}

	m_Desc = {};
	m_Desc.BufferLocation = m_pBuffer->GetGPUVirtualAddress();
	m_Desc.SizeInBytes = static_cast<UINT>(sizeAligned);

	m_IsValid = true;
}

bool ConstantBuffer::IsValid()
{
	return m_IsValid;
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetAddress() const
{
	return m_Desc.BufferLocation;
}

D3D12_CONSTANT_BUFFER_VIEW_DESC ConstantBuffer::ViewDesc() const
{
	return m_Desc;
}

// �����ɒl������΃}�b�v�����A�A�h���X�̐擪����A�T�C�Y<T>�����������Ă���
void* ConstantBuffer::GetPtr() const
{
	return m_pMappedPtr;
}