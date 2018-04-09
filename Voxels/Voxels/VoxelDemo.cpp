#include "VoxelDemo.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "ColorHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "D3DCompiler.h"

namespace Rendering
{
	RTTI_DEFINITIONS(VoxelDemo)

	VoxelDemo::VoxelDemo(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera), mWorldMatrix(MatrixHelper::Identity)
	{
	}

	VoxelDemo::~VoxelDemo()
	{
		ReleaseObject(mPass);
		ReleaseObject(mTechnique);
		ReleaseObject(mWvpVariable);
		ReleaseObject(mInputLayout);
		ReleaseObject(mVertexBuffer);
		ReleaseObject(mEffect);
		DeleteObject(mChunk);
	}

	void VoxelDemo::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Compile the shader
		UINT shaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		// Load in the effect (shader) file and compile it
		ID3D10Blob* compiledShader = nullptr;
		ID3D10Blob* errorMessages = nullptr;
		HRESULT hr = D3DCompileFromFile(L"Content\\Effects\\Outline.fx", nullptr, nullptr, nullptr, "fx_5_0", shaderFlags, 0, &compiledShader, &errorMessages);
		if (FAILED(hr))
		{
			const char* errorMessage = (errorMessages != nullptr ? (char*)errorMessages->GetBufferPointer() : "D3DX11CompileFromFile() failed");
			GameException ex(errorMessage, hr);
			ReleaseObject(errorMessages);

			throw ex;
		}

		// Create an effect object from the compiled shader
		hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, mGame->Direct3DDevice(), &mEffect);
		if (FAILED(hr))
		{
			throw GameException("D3DX11CreateEffectFromMemory() failed.", hr);
		}

		ReleaseObject(compiledShader);

		// Look up the technique, pass, and WVP variable from the effect
		//each technique contains one or more passes
		//each pass consists of setting one or more texture stage registers of the 3D hardware
		//Often a fallback pass is provided for older hardware
		//Initial pass might render water, specular highlights, caustic textures, lighting
		//Second pass might do the above but no lighting (assuming older hardware not supporting this)
		//Thus first pass is tried, if it fails, second pass is tried
		//Variables can be passed to GPU for effect functions e.g. the WorldViewProjection (WVP) in this case
		//which is needed for the vertex shader to move vertices into the world view
		mTechnique = mEffect->GetTechniqueByName("main11");
		if (mTechnique == nullptr)
		{
			throw GameException("ID3DX11Effect::GetTechniqueByName() could not find the specified technique.", hr);
		}

		mPass = mTechnique->GetPassByName("p0");
		if (mPass == nullptr)
		{
			throw GameException("ID3DX11EffectTechnique::GetPassByName() could not find the specified pass.", hr);
		}

		ID3DX11EffectVariable* variable = mEffect->GetVariableByName("WorldViewProjection");
		if (variable == nullptr)
		{
			throw GameException("ID3DX11Effect::GetVariableByName() could not find the specified variable.", hr);
		}

		ID3DX11EffectMatrixVariable* positionVariable = mEffect->GetVariableByName("PositionMatrix")->AsMatrix();

		//As the variable is the WVP which is a matrix, need to have it returned (cast) as a matrix type
		mWvpVariable = variable->AsMatrix();
		if (mWvpVariable->IsValid() == false)
		{
			throw GameException("Invalid effect variable cast.");
		}

		// Create the input layout to map the vertex data from the CPU to the GPU
		//From the pass we obtain the pipeline, signature of the pass (params), stencil etc.
		D3DX11_PASS_DESC passDesc;
		mPass->GetDesc(&passDesc);

		//Define our input elements (consists of position format and colour format for this example)
		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		//Now can create the InputLayout to provide the mapping of vertex data from CPU to GPU
		if (FAILED(hr = mGame->Direct3DDevice()->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout)))
		{
			throw GameException("ID3D11Device::CreateInputLayout() failed.", hr);
		}

		mChunk = new Chunk(*mGame, *mCamera, *positionVariable);

		float xOffset = 0.0f;
		for (int x = 0; x < 16; x += 2) {
			float yOffset = 0.0f;
			for (int y = 0; y < 16; y += 2) {
				float zOffset = 0.0f;
				for (int z = 0; z < 16; z += 2) {
					Voxel* voxel = new Voxel(*mGame, *mCamera, XMFLOAT3(x+xOffset, y+yOffset, z+zOffset), 1, *mTechnique);
					mChunk->AddVoxel(voxel);
					zOffset += 0.025f;
				}
				yOffset += 0.025f;
			}
			xOffset += 0.025f;
		}
	}

	void VoxelDemo::Update(const GameTime& gameTime)
	{
	}

	void VoxelDemo::Draw(const GameTime& gameTime)
	{
		//As in OpenCL we need a context
		//The context here is a simple primitive
		//With a context we can set our vertex buffers
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		direct3DDeviceContext->IASetInputLayout(mInputLayout);

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX wvp = worldMatrix * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();
		mWvpVariable->SetMatrix(reinterpret_cast<const float*>(&wvp));

		mChunk->Draw(gameTime);
	}

	void VoxelDemo::SetMotionVectors(long x, long y) {
		XMFLOAT4X4 proj;
		XMStoreFloat4x4(&proj, mCamera->ProjectionMatrix());
		float dx = (((2.0f * x) / mGame->ScreenWidth()) - 1.0f) / proj(0, 0);
		float dy = (((-2.0f * y) / mGame->ScreenHeight()) + 1.0f) / proj(1, 1);

		XMVECTOR orig = XMLoadFloat3(new XMFLOAT3(0.0f, 0.0f, 0.0f));
		XMVECTOR dir = XMLoadFloat3(new XMFLOAT3(dx, dy, -1.0f));
		XMMATRIX invView = XMMatrixInverse(nullptr, mCamera->ViewMatrix());
		orig = XMVector3TransformCoord(orig, invView);
		dir = XMVector3TransformNormal(dir, invView);
		XMVector3Normalize(dir);

		float dist = mChunk->FindClosestVoxel(orig, dir);
		if (dist >= 0) {
			dir = dir * dist;
			XMVECTOR impact = orig + dir;
			mChunk->SetMotionVectors(impact);
		}

		//long dx = (x / (mGame->ScreenWidth() * 0.5f - 1.0f)) / mGame->AspectRatio();
		//long dy = 1.0f - y / (mGame->ScreenHeight() * 0.5f);
		//XMVECTOR v = XMLoadFloat3(new XMFLOAT3(dx, dy, 0.0f));
		
		//v = XMVector3Unproject(v, 0, 0, mGame->ScreenWidth(), mGame->ScreenHeight(), 0, 1.0f, mCamera->ProjectionMatrix(), mCamera->ViewMatrix(), XMLoadFloat4x4(&mWorldMatrix));

		//mChunk->SetMotionVectors(v);
	}
}