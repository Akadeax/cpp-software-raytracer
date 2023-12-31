#include "Scene.h"
#include "Utils.h"
#include "Material.h"

namespace dae {

#pragma region Base Scene
	//Initialize Scene with Default Solid Color Material (RED)
	Scene::Scene():
		m_Materials({ new Material_SolidColor({1,0,0})})
	{
		m_SphereGeometries.reserve(32);
		m_PlaneGeometries.reserve(32);
		m_TriangleMeshGeometries.reserve(32);
		m_Lights.reserve(32);
	}

	Scene::~Scene()
	{
		for(auto& pMaterial : m_Materials)
		{
			delete pMaterial;
			pMaterial = nullptr;
		}

		m_Materials.clear();
	}

	void dae::Scene::GetClosestHit(const Ray& ray, HitRecord& closestHit) const
	{
		for (size_t i{}; i < m_SphereGeometries.size(); ++i)
		{
			HitRecord currentHitRecord{};
			GeometryUtils::HitTest_Sphere(m_SphereGeometries[i], ray, currentHitRecord);

			if (currentHitRecord.t < closestHit.t)
			{
				closestHit = currentHitRecord;
			}
		}

		for (size_t i{}; i < m_PlaneGeometries.size(); ++i)
		{
			HitRecord currentHitRecord{};
			GeometryUtils::HitTest_Plane(m_PlaneGeometries[i], ray, currentHitRecord);

			if (currentHitRecord.t < closestHit.t)
			{
				closestHit = currentHitRecord;
			}
		}

		//for (size_t i{}; i < m_Triangles.size(); ++i)
		//{
		//	HitRecord currentHitRecord{};
		//	GeometryUtils::HitTest_Triangle(m_Triangles[i], ray, currentHitRecord);

		//	if (currentHitRecord.t < closestHit.t)
		//	{
		//		closestHit = currentHitRecord;
		//	}
		//}

		for (size_t i{}; i < m_TriangleMeshGeometries.size(); ++i)
		{
			HitRecord currentHitRecord{};
			GeometryUtils::HitTest_TriangleMesh(m_TriangleMeshGeometries[i], ray, currentHitRecord);

			if (currentHitRecord.t < closestHit.t)
			{
				closestHit = currentHitRecord;
			}
		}
	}

	bool Scene::DoesHit(const Ray& ray) const
	{
		for (size_t i{}; i < m_SphereGeometries.size(); ++i)
		{
			if (GeometryUtils::HitTest_Sphere(m_SphereGeometries[i], ray)) return true;
		}

		for (size_t i{}; i < m_PlaneGeometries.size(); ++i)
		{
			if (GeometryUtils::HitTest_Plane(m_PlaneGeometries[i], ray)) return true;
		}

		//for (size_t i{}; i < m_Triangles.size(); ++i)
		//{
		//	if (GeometryUtils::HitTest_Triangle(m_Triangles[i], ray)) return true;
		//}

		for (size_t i{}; i < m_TriangleMeshGeometries.size(); ++i)
		{
			if (GeometryUtils::HitTest_TriangleMesh(m_TriangleMeshGeometries[i], ray)) return true;
		}

		return false;
	}

#pragma region Scene Helpers
	Sphere* Scene::AddSphere(const Vector3& origin, float radius, unsigned char materialIndex)
	{
		Sphere s;
		s.origin = origin;
		s.radius = radius;
		s.materialIndex = materialIndex;

		m_SphereGeometries.emplace_back(s);
		return &m_SphereGeometries.back();
	}

	Plane* Scene::AddPlane(const Vector3& origin, const Vector3& normal, unsigned char materialIndex)
	{
		Plane p;
		p.origin = origin;
		p.normal = normal;
		p.materialIndex = materialIndex;

		m_PlaneGeometries.emplace_back(p);
		return &m_PlaneGeometries.back();
	}

	TriangleMesh* Scene::AddTriangleMesh(TriangleCullMode cullMode, unsigned char materialIndex)
	{
		TriangleMesh m{};
		m.cullMode = cullMode;
		m.materialIndex = materialIndex;

		m_TriangleMeshGeometries.emplace_back(m);
		return &m_TriangleMeshGeometries.back();
	}

	TriangleMesh* Scene::AddTriangleMesh(const std::string& objFile, Vector3 position, Vector3 scale, float yaw, unsigned char materialIndex)
	{
		TriangleMesh* pMesh = AddTriangleMesh(TriangleCullMode::NoCulling, materialIndex);
		Utils::ParseOBJ(objFile,
			pMesh->positions,
			pMesh->normals,
			pMesh->indices
		);

		pMesh->Scale(scale);
		pMesh->Translate(position);
		pMesh->RotateY(yaw);

		pMesh->UpdateAABB();
		pMesh->UpdateTransforms();

		return pMesh;
	}

	Light* Scene::AddPointLight(const Vector3& origin, float intensity, const ColorRGB& color)
	{
		Light l;
		l.origin = origin;
		l.intensity = intensity;
		l.color = color;
		l.type = LightType::Point;

		m_Lights.emplace_back(l);
		return &m_Lights.back();
	}

	Light* Scene::AddDirectionalLight(const Vector3& direction, float intensity, const ColorRGB& color)
	{
		Light l;
		l.direction = direction;
		l.intensity = intensity;
		l.color = color;
		l.type = LightType::Directional;

		m_Lights.emplace_back(l);
		return &m_Lights.back();
	}

	unsigned char Scene::AddMaterial(Material* pMaterial)
	{
		m_Materials.push_back(pMaterial);
		return static_cast<unsigned char>(m_Materials.size() - 1);
	}
#pragma endregion
}
