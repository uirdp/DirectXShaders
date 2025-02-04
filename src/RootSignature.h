#pragma once
#include "ComPtr.h"

struct ID3D12RootSignature;
class RootSignature
{
public:
	RootSignature();
	bool IsValid() const;
	ID3D12RootSignature* Get() const;

private:
	bool m_IsValid = false;
	ComPtr<ID3D12RootSignature> m_pRootSignature = nullptr;
};