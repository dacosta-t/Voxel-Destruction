#pragma once

#include "DrawableGameComponent.h"
#include "Voxel.h"

using namespace Library;

namespace Rendering {
	class Chunk : public DrawableGameComponent {
		RTTI_DECLARATIONS(Chunk, DrawableGameComponent)
	public:
		Chunk(Game& game, Camera& camera, ID3DX11EffectMatrixVariable& positionVariable);
		~Chunk();

		void AddVoxel(Voxel* voxel);
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;
		virtual void SetMotionVectors(XMVECTOR point);
		virtual float FindClosestVoxel(XMVECTOR orig, XMVECTOR dir);
	private:
		std::vector<Voxel*> mVoxels;
		ID3DX11EffectMatrixVariable* mPositionVariable;
	};
}