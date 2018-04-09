#include "Voxel.h"
#include "Game.h"
#include "GameException.h"
#include "Camera.h"

namespace Rendering {
	RTTI_DEFINITIONS(Voxel)

	Voxel::Voxel(Game& game, Camera& camera, XMFLOAT3 origin, float size, ID3DX11EffectPass& pass)
		: DrawableGameComponent(game, camera), mOrigin(origin), mSize(size), mPass(&pass)
	{
		CreateVoxel();
	}

	Voxel::~Voxel()
	{
		ReleaseObject(mVertexBuffer);
	}

	ID3D11Buffer* Voxel::GetVertexBuffer() const
	{
		return mVertexBuffer;
	}

	void Voxel::CreateVoxel()
	{
		XMFLOAT2 x = XMFLOAT2(mOrigin.x - mSize, mOrigin.x + mSize);
		XMFLOAT2 y = XMFLOAT2(mOrigin.y - mSize, mOrigin.y + mSize);
		XMFLOAT2 z = XMFLOAT2(mOrigin.z - mSize, mOrigin.z + mSize);

		BasicVertex vertices[] = {
			// Front Face
			BasicVertex(XMFLOAT3(x.y, y.y, z.y)),
			BasicVertex(XMFLOAT3(x.y, y.x, z.y)),
			BasicVertex(XMFLOAT3(x.x, y.x, z.y)),
			BasicVertex(XMFLOAT3(x.x, y.x, z.y)),
			BasicVertex(XMFLOAT3(x.x, y.y, z.y)),
			BasicVertex(XMFLOAT3(x.y, y.y, z.y)),
			// Back Face
			BasicVertex(XMFLOAT3(x.x, y.y, z.x)),
			BasicVertex(XMFLOAT3(x.x, y.x, z.x)),
			BasicVertex(XMFLOAT3(x.y, y.x, z.x)),
			BasicVertex(XMFLOAT3(x.y, y.x, z.x)),
			BasicVertex(XMFLOAT3(x.y, y.y, z.x)),
			BasicVertex(XMFLOAT3(x.x, y.y, z.x)),
			// Left Face
			BasicVertex(XMFLOAT3(x.x, y.y, z.y)),
			BasicVertex(XMFLOAT3(x.x, y.x, z.y)),
			BasicVertex(XMFLOAT3(x.x, y.x, z.x)),
			BasicVertex(XMFLOAT3(x.x, y.x, z.x)),
			BasicVertex(XMFLOAT3(x.x, y.y, z.x)),
			BasicVertex(XMFLOAT3(x.x, y.y, z.y)),
			// Right Face
			BasicVertex(XMFLOAT3(x.y, y.y, z.x)),
			BasicVertex(XMFLOAT3(x.y, y.x, z.x)),
			BasicVertex(XMFLOAT3(x.y, y.x, z.y)),
			BasicVertex(XMFLOAT3(x.y, y.x, z.y)),
			BasicVertex(XMFLOAT3(x.y, y.y, z.y)),
			BasicVertex(XMFLOAT3(x.y, y.y, z.x)),
			// Top Face
			BasicVertex(XMFLOAT3(x.x, y.y, z.y)),
			BasicVertex(XMFLOAT3(x.x, y.y, z.x)),
			BasicVertex(XMFLOAT3(x.y, y.y, z.x)),
			BasicVertex(XMFLOAT3(x.y, y.y, z.x)),
			BasicVertex(XMFLOAT3(x.y, y.y, z.y)),
			BasicVertex(XMFLOAT3(x.x, y.y, z.y)),
			// Bottom Face
			BasicVertex(XMFLOAT3(x.y, y.x, z.y)),
			BasicVertex(XMFLOAT3(x.y, y.x, z.x)),
			BasicVertex(XMFLOAT3(x.x, y.x, z.x)),
			BasicVertex(XMFLOAT3(x.x, y.x, z.x)),
			BasicVertex(XMFLOAT3(x.x, y.x, z.y)),
			BasicVertex(XMFLOAT3(x.y, y.x, z.y)),
		};

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.ByteWidth = sizeof(BasicVertex) * ARRAYSIZE(vertices);
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		//A subsource is a mip-map level
		//The vertex buffer above is Immutable thus cannot be modified
		//A subresource initialized with the vertex buffer data is instead used 
		D3D11_SUBRESOURCE_DATA vertexSubResourceData;
		ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
		vertexSubResourceData.pSysMem = vertices;

		//Create buffer will hold the mutable vertices used by the GPU
		if (FAILED(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &mVertexBuffer)))
		{
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}
	}

	void Voxel::Update(const GameTime& gameTime)
	{
	}

	void Voxel::Draw(const GameTime& gameTime)
	{
		//As in OpenCL we need a context
		//The context here is a simple primitive
		//With a context we can set our vertex buffers
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();

		UINT stride = sizeof(BasicVertex);
		UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

		//Apply the pass to all the vertices and pixels
		mPass->Apply(0, direct3DDeviceContext);

		direct3DDeviceContext->Draw(36, 0);
	}
}