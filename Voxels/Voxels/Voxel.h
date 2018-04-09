#pragma once

#include "DrawableGameComponent.h"

using namespace Library;

namespace Rendering {
	class Voxel : public DrawableGameComponent {
		RTTI_DECLARATIONS(Voxel, DrawableGameComponent)

	public:
		Voxel(Game& game, Camera& camera, XMFLOAT3 origin, float size, ID3DX11EffectPass& pass);
		~Voxel();
		ID3D11Buffer* GetVertexBuffer() const;

		void CreateVoxel();
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

		XMFLOAT3 mOrigin;
		float mSize;
		ID3D11Buffer* mVertexBuffer;
		ID3DX11EffectPass* mPass;
	};
}