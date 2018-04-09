#pragma once

#include "DrawableGameComponent.h"

using namespace Library;

namespace Rendering {
	class Voxel : public DrawableGameComponent {
		RTTI_DECLARATIONS(Voxel, DrawableGameComponent)

	public:
		Voxel(Game& game, Camera& camera, XMFLOAT3 origin, float size, ID3DX11EffectTechnique& technique);
		~Voxel();
		ID3D11Buffer* GetVertexBuffer() const;

		void CreateVoxel();
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;
	private:
		typedef struct _BasicVertex
		{
			XMFLOAT4 Position;
			XMFLOAT4 Normal;
			XMFLOAT2 Texture;

			_BasicVertex() { }

			_BasicVertex(XMFLOAT4 position, XMFLOAT4 normal, XMFLOAT2 texture)
				: Position(position), Normal(normal), Texture(texture) { }
		} BasicVertex;

		XMFLOAT3 mOrigin;
		float mSize;
		ID3D11Buffer* mVertexBuffer;
		ID3DX11EffectTechnique* mTechnique;
	};
}