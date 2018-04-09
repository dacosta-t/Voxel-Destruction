#pragma once

#include "DrawableGameComponent.h"
#include "Voxel.h"

using namespace Library;

namespace Rendering {
	class Chunk : public DrawableGameComponent {
		RTTI_DECLARATIONS(Chunk, DrawableGameComponent)
	public:
		Chunk(Game& game, Camera& camera, ID3DX11EffectPass& pass);
		~Chunk();

		void AddVoxel(Voxel* voxel);
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;
	private:
		ID3DX11EffectPass* mPass;
		std::vector<Voxel*> mVoxels;
	};
}