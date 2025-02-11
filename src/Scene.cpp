#include "Scene.h"
#include "Engine.h"
#include "App.h"
#include <d3dx12.h>
#include "SharedStruct.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "RootSignature.h"
#include "PipelineState.h"

Scene* g_Scene;

using namespace DirectX;

VertexBuffer* vertexBuffer;
IndexBuffer* indexBuffer;
ConstantBuffer* constantBuffer[Engine::FRAME_BUFFER_COUNT];
RootSignature* rootSignature;
PipelineState* pipelineState;

bool Scene::Init()
{
	Vertex vertices[4] = {};
	vertices[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertices[0].Color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[1].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].Color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	vertices[3].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[3].Color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);

	auto vertexSize = sizeof(Vertex) * std::size(vertices);
	auto vertexStride = sizeof(Vertex);

	vertexBuffer = new VertexBuffer(vertexSize, vertexStride, vertices);

	if (!vertexBuffer->IsValid())
	{
		printf("頂点バッファの生成に失敗\n");
		return false;
	}

	uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };

	auto size = sizeof(uint32_t) * std::size(indices);
	indexBuffer = new IndexBuffer(size, indices);

	if (!indexBuffer->IsValid())
	{
		printf("インデックスバッファの生成に失敗\n");
		return false;
	}

	auto eyePos = XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);
	auto targetPos = XMVectorZero();
	auto upward = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	auto fov = XMConvertToRadians(37.5f);
	auto aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);

	for (size_t i = 0; i < Engine::FRAME_BUFFER_COUNT; i++)
	{
		constantBuffer[i] = new ConstantBuffer(sizeof(Transform));
		if (!constantBuffer[i]->IsValid())
		{
			printf("定数バッファの生成に失敗\n");
			return false;
		}

		// mapされている
		auto ptr = constantBuffer[i]->GetPtr<Transform>();
		ptr->World = XMMatrixIdentity();
		ptr->View = XMMatrixLookAtRH(eyePos, targetPos, upward);
		ptr->Projection = XMMatrixPerspectiveFovRH(fov, aspect, 0.3f, 1000.0f);
	}

	rootSignature = new RootSignature();
	if (!rootSignature->IsValid())
	{
		printf("ルートシグネチャの生成に失敗\n");
		return false;
	}

	pipelineState = new PipelineState();
	pipelineState->SetInputLayout(Vertex::InputLayout);
	pipelineState->SetRootSignature(rootSignature->Get());
	pipelineState->SetVertexShader(L"../x64/Debug/SampleVS.cso");
	pipelineState->SetPixelShader(L"../x64/Debug/SamplePS.cso");
	pipelineState->Create();
	if (!pipelineState->IsValid())
	{
		printf("パイプラインステートの生成に失敗\n");
		return false;
	}

	printf("シーンの初期化に成功\n");
	return true;
}


float rotateY = 0.0f;
void Scene::Update()
{
	rotateY += 0.02f;
	auto currentIndex = g_Engine->CurrentBackBufferIndex();
	auto currentTransform = constantBuffer[currentIndex]->GetPtr<Transform>();
	currentTransform->World = XMMatrixRotationY(rotateY);
}

void Scene::Draw()
{
	auto currentIndex = g_Engine->CurrentBackBufferIndex();
	auto commandList = g_Engine->CommandList();
	auto vbView = vertexBuffer->View();
	auto ibView = indexBuffer->View();

	commandList->SetGraphicsRootSignature(rootSignature->Get());
	commandList->SetPipelineState(pipelineState->Get());
	// slot0にバインドされる
	commandList->SetGraphicsRootConstantBufferView(0, constantBuffer[currentIndex]->GetAddress());

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vbView);
	commandList->IASetIndexBuffer(&ibView);

	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
