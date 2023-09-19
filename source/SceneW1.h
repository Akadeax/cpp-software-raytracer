#pragma once
#include "Scene.h"

class SceneW1 : public dae::Scene
{
public:
	SceneW1() = default;
	~SceneW1() override = default;

	SceneW1(const SceneW1&) = delete;
	SceneW1(SceneW1&&) noexcept = delete;
	SceneW1& operator=(const SceneW1&) = delete;
	SceneW1& operator=(SceneW1&&) noexcept = delete;

	void Initialize() override;
};

