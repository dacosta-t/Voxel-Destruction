#pragma once

#include "DrawableGameComponent.h"
#include "Voxel.h"

using namespace Library;

namespace Rendering {
	class Chunk : public DrawableGameComponent {
		RTTI_DECLARATIONS(Chunk, DrawableGameComponent)
	public:
		Chunk(Game& game, Camera& camera);
		~Chunk();

		void AddVoxel(Voxel* voxel);
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;
	private:
		std::vector<Voxel*> mVoxels;
	};
}