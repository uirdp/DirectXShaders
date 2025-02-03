#pragma once
#include <d3d12.h>
#include "ComPtr.h"

class VertexBuffer
{
public:
	VertexBuffer(size_t size, size_t stride, const void* pInitData);
	D3D12_VERTEX_BUFFER_VIEW View() const;
	bool IsValid();

	// �R�s�[�֎~
	// ���g�������ɂƂ�R���X�g���N�^�i�R�s�[�R���X�g���N�^�j���֎~
	VertexBuffer(const VertexBuffer&) = delete;
	// ������Z�q���֎~
	void operator = (const VertexBuffer&) = delete;

private:
	bool m_IsValid = false;
	ComPtr<ID3D12Resource> m_pBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_View = {};
};