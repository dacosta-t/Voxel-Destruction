#include "Chunk.h"
#include "Game.h"
#include "Camera.h"

namespace Rendering {
	RTTI_DEFINITIONS(Chunk)

	Chunk::Chunk(Game& game, Camera& camera, ID3DX11EffectMatrixVariable& positionVariable)
		: DrawableGameComponent(game, camera)
		, mPositionVariable(&positionVariable)
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
			mPositionVariable->SetMatrix(reinterpret_cast<const float*>(&(*it)->GetPositionMatrix()));
			(*it)->Draw(gameTime);
		}
	}

	void Chunk::SetMotionVectors(XMVECTOR point) {
		for (auto it = mVoxels.begin(); it != mVoxels.end(); it++) {
			(*it)->SetMotionVector(point);
		}
	}

	float Chunk::FindClosestVoxel(XMVECTOR orig, XMVECTOR dir) {
		float sMin = -1;
		for (auto it = mVoxels.begin(); it != mVoxels.end(); it++) {
			Voxel* vox = *it;
			XMVECTOR v = orig - vox->GetOriginVector();

			XMFLOAT3 bf;
			XMStoreFloat3(&bf, XMVector3Dot(dir, v));
			float b = 2.0f * bf.x;
			XMStoreFloat3(&bf, XMVector3Dot(v, v));
			float c = bf.x - vox->GetSize() * vox->GetSize();
			float discriminant = (b * b) - (4.0f * c);

			if (discriminant < 0.0f)
				continue;

			discriminant = sqrtf(discriminant);

			float s0 = (-b + discriminant) / 2.0f;
			float s1 = (-b - discriminant) / 2.0f;

			if (s0 >= 0.0f || s1 >= 0.0f) {
				if (sMin == -1 || s0 < sMin || s1 < sMin) {
					sMin = s0 < s1 ? s0 : s1;
				}
			}
		}
		return sMin;
	}
}