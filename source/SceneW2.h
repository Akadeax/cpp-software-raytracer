#pragma once
#include "Scene.h"

class SceneW2 : public dae::Scene
{
public:
	SceneW2() = default;
	~SceneW2() override = default;

	SceneW2(const SceneW2&) = delete;
	SceneW2(SceneW2&&) noexcept = delete;
	SceneW2& operator=(const SceneW2&) = delete;
	SceneW2& operator=(SceneW2&&) noexcept = delete;

	void Initialize() override;
};

