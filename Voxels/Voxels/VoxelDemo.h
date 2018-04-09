#pragma once

#include "DrawableGameComponent.h"
#include "Chunk.h"

using namespace Library;

namespace Rendering
{
	class VoxelDemo : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(VoxelDemo, DrawableGameComponent)

	public:
		VoxelDemo(Game& game, Camera& camera);
		~VoxelDemo();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		typedef struct _BasicVertex
		{
			XMFLOAT3 Position;

			_BasicVertex() { }

			_BasicVertex(XMFLOAT3 position)
				: Position(position) { }
		} BasicVertex;

		VoxelDemo();
		VoxelDemo(const VoxelDemo& rhs);
		VoxelDemo& operator=(const VoxelDemo& rhs);

		ID3DX11Effect* mEffect;
		ID3DX11EffectTechnique* mTechnique;
		ID3DX11EffectPass* mPass;
		ID3D11InputLayout* mInputLayout;
		ID3DX11EffectMatrixVariable* mWvpVariable;

		XMFLOAT4X4 mWorldMatrix;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;

		Chunk* mChunk;
	};
}
