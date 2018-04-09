#include "Chunk.h"
#include "Game.h"
#include "Camera.h"

namespace Rendering {
	RTTI_DEFINITIONS(Chunk)

	Chunk::Chunk(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera)
	{
		mVoxels = std::vector<Voxel*>();
	}

	Chunk::~Chunk()
	{
		for (int i = 0; i < mVoxels.size(); i++) {
			delete mVoxels[i];
		}
		mVoxels.clear();
	}

	void Chunk::AddVoxel(Voxel* voxel)
	{
		mVoxels.push_back(voxel);
	}

	void Chunk::Update(const GameTime& gameTime)
	{
	}

	void Chunk::Draw(const GameTime& gameTime)
	{
		for (auto it = mVoxels.begin(); it != mVoxels.end(); it++) {
			(*it)->Draw(gameTime);
		}
	}
}