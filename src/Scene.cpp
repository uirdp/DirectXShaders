#include "Scene.h"
#include "Engine.h"
#include "App.h"
#include <d3dx12.h>
#include "SharedStruct.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"

Scene* g_Scene;

using namespace DirectX;

VertexBuffer* vertexBuffer;
ConstantBuffer* constantBuffer[Engine::FRAME_BUFFER_COUNT];

bool Scene::Init()
{
	Vertex vertices[3] = {};
	vertices[0].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].Color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	vertices[1].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[1].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].Position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[2].Color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	auto vertexSize = sizeof(Vertex) * std::size(vertices);
	auto vertexStride = sizeof(Vertex);

	vertexBuffer = new VertexBuffer(vertexSize, vertexStride, vertices);

	if (!vertexBuffer->IsValid())
	{
		printf("���_�o�b�t�@�̐����Ɏ��s\n");
		return false;
	}

	auto eyePos = XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);
	auto targetPos = XMVectorZero();
	auto upward = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	auto fov = XMConvertToRadians(37.5f);
	auto aspect = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;

	for (size_t i = 0; i < Engine::FRAME_BUFFER_COUNT; i++)
	{
		constantBuffer[i] = new ConstantBuffer(sizeof(Transform));
		if (!constantBuffer[i]->IsValid())
		{
			printf("�萔�o�b�t�@�̐����Ɏ��s\n");
			return false;
		}

		// map����Ă���
		auto ptr = constantBuffer[i]->GetPtr<Transform>();
		ptr->World = XMMatrixIdentity();
		ptr->View = XMMatrixLookAtLH(eyePos, targetPos, upward);
		ptr->Projection = XMMatrixPerspectiveFovLH(fov, aspect, 0.3f, 1000.0f);
	}
	printf("�V�[���̏������ɐ���\n");
	return true;
}

void Scene::Update()
{
}

void Scene::Draw()
{
}
