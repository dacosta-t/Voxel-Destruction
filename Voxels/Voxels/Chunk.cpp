#include "Chunk.h"
#include "Game.h"
#include "Camera.h"

namespace Rendering {
	RTTI_DEFINITIONS(Chunk)

	Chunk::Chunk(Game& game, Camera& camera, ID3DX11EffectPass& pass)
		: DrawableGameComponent(game, camera), mPass(&pass)
	{
		mVoxels = std::vector<Voxel*>();
	}

	Chunk::~Chunk()
	{
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