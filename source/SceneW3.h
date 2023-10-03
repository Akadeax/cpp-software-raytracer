#pragma once
#include "Scene.h"

class SceneW3 : public dae::Scene
{
public:
	SceneW3() = default;
	~SceneW3() override = default;

	SceneW3(const SceneW3&) = delete;
	SceneW3(SceneW3&&) noexcept = delete;
	SceneW3& operator=(const SceneW3&) = delete;
	SceneW3& operator=(SceneW3&&) noexcept = delete;

	void Initialize() override;
};

