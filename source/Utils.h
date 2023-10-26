#pragma once
#include <cassert>
#include <fstream>
#include "Math.h"
#include "DataTypes.h"

namespace dae
{
	namespace GeometryUtils
	{
#pragma region Sphere HitTest
		//SPHERE HIT-TESTS
		inline bool HitTest_Sphere(const Sphere& sphere, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			// Coefficients of the Quadratic Equation
			float a{ Vector3::Dot(ray.direction, ray.direction) };
			Vector3 sphereToRay{ ray.origin - sphere.origin };
			float b{ Vector3::Dot(2.f * ray.direction, sphereToRay) };
			float c{ Vector3::Dot(sphereToRay, sphereToRay) - sphere.radius * sphere.radius };

			float discriminant{ b * b - 4 * a * c };

			if (discriminant <= 0) return false;

			float t{ (-1 * b - sqrtf(discriminant)) / 2 * a };

			if (t <= ray.min || t > ray.max) return false;
			if (ignoreHitRecord) return true;

			hitRecord.t = t;
			hitRecord.didHit = true;
			hitRecord.origin = ray.origin + t * ray.direction;
			hitRecord.normal = (hitRecord.origin - sphere.origin).Normalized();
			hitRecord.materialIndex = sphere.materialIndex;
			return true;
		}

		inline bool HitTest_Sphere(const Sphere& sphere, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_Sphere(sphere, ray, temp, true);
		}
#pragma endregion
#pragma region Plane HitTest
		//PLANE HIT-TESTS
		inline bool HitTest_Plane(const Plane& plane, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			const float denom{ Vector3::Dot(plane.normal, ray.direction) };
			const float t{ Vector3::Dot(plane.origin - ray.origin, plane.normal) / denom };

			if (t <= ray.min || t > ray.max) return false;
			if (ignoreHitRecord) return true;

			hitRecord.t = t;
			hitRecord.didHit = true;
			hitRecord.origin = ray.origin + t * ray.direction;
			hitRecord.normal = plane.normal;
			hitRecord.materialIndex = plane.materialIndex;
			return true;
		}

		inline bool HitTest_Plane(const Plane& plane, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_Plane(plane, ray, temp, true);
		}
#pragma endregion
#pragma region Triangle HitTest
		inline bool HitTest_RightSideOfLine(Vector3 point, Vector3 linePoint0, Vector3 linePoint1, Vector3 normal)
		{
			const Vector3 e{ linePoint1 - linePoint0 };
			const Vector3 cross{ Vector3::Cross(e, point - linePoint0) };

			return Vector3::Dot(cross, normal) <= 0.f;
		}


		//TRIANGLE HIT-TESTS
		inline bool HitTest_Triangle(const Triangle& triangle, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			// Face Culling
			const float dot{ Vector3::Dot(triangle.normal, ray.direction) };
			if (AreEqual(dot, 0)) return false;

			if (triangle.cullMode == TriangleCullMode::FrontFaceCulling)
			{
				if (!ignoreHitRecord && dot < 0.f || ignoreHitRecord && dot > 0.f) return false;
			}
			if (triangle.cullMode == TriangleCullMode::BackFaceCulling)
			{
				if (!ignoreHitRecord && dot > 0.f || ignoreHitRecord && dot < 0.f) return false;
			}

			const Vector3 l{ triangle.v0 - ray.origin };
			const float t{ Vector3::Dot(l, triangle.normal) / Vector3::Dot(ray.direction, triangle.normal) };

			if (t < ray.min || t > ray.max) return false;

			const Vector3 p{ ray.origin + ray.direction * t };

			if (HitTest_RightSideOfLine(p, triangle.v0, triangle.v1, triangle.normal) ||
				HitTest_RightSideOfLine(p, triangle.v1, triangle.v2, triangle.normal) ||
				HitTest_RightSideOfLine(p, triangle.v2, triangle.v0, triangle.normal))
			{
				return false;
			}

			hitRecord.t = t;
			hitRecord.didHit = true;
			hitRecord.origin = p;
			hitRecord.normal = triangle.normal;
			hitRecord.materialIndex = triangle.materialIndex;
			return true;
		}

		// Moeller-Trumbore intersection https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
		inline bool HitTest_TriangleMoellerTrumbore(const Triangle& triangle, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			// Face Culling
			const float dot{ Vector3::Dot(triangle.normal, ray.direction) };
			if (AreEqual(dot, 0)) return false;

			if (triangle.cullMode == TriangleCullMode::FrontFaceCulling)
			{
				if (!ignoreHitRecord && dot < 0.f || ignoreHitRecord && dot > 0.f) return false;
			}
			if (triangle.cullMode == TriangleCullMode::BackFaceCulling)
			{
				if (!ignoreHitRecord && dot > 0.f || ignoreHitRecord && dot < 0.f) return false;
			}

			const Vector3 edge1{ triangle.v1 - triangle.v0 };
			const Vector3 edge2{ triangle.v2 - triangle.v0 };

			const Vector3 pVec{ Vector3::Cross(ray.direction, edge2) };

			const float factor{ Vector3::Dot(pVec, edge1) };
			if (AreEqual(factor, 0)) return false;

			const float inverseFactor{ 1.f / factor };

			// Get u
			const Vector3 tVec{ ray.origin - triangle.v0 };

			const float u{ inverseFactor * Vector3::Dot(pVec, tVec) };
			if (u < 0.f || u > 1.f) return false;

			// Get v
			const Vector3 qVec{ Vector3::Cross(tVec, edge1) };

			const float v{ inverseFactor * Vector3::Dot(qVec, ray.direction) };
			if (v < 0.f || u + v > 1.f) return false;



			const float t{ inverseFactor * Vector3::Dot(qVec, edge2) };

			if (t < ray.min || t > ray.max) return false;

			if (ignoreHitRecord) return true;

			hitRecord.t = t;
			hitRecord.didHit = true;
			hitRecord.origin = ray.origin + ray.direction * t;
			hitRecord.normal = triangle.normal;
			hitRecord.materialIndex = triangle.materialIndex;
			return true;
		}


		inline bool HitTest_Triangle(const Triangle& triangle, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_Triangle(triangle, ray, temp, true);
		}

		inline bool SlabTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray)
		{
			const float tx1 = (mesh.transformedMinAABB.x - ray.origin.x) / ray.direction.x;
			const float tx2 = (mesh.transformedMaxAABB.x - ray.origin.x) / ray.direction.x;

			float tmin = std::min(tx1, tx2);
			float tmax = std::max(tx1, tx2);

			const float ty1 = (mesh.transformedMinAABB.y - ray.origin.y) / ray.direction.y;
			const float ty2 = (mesh.transformedMaxAABB.y - ray.origin.y) / ray.direction.y;

			tmin = std::max(tmin, std::min(ty1, ty2));
			tmax = std::min(tmax, std::max(ty1, ty2));

			const float tz1 = (mesh.transformedMinAABB.z - ray.origin.z) / ray.direction.z;
			const float tz2 = (mesh.transformedMaxAABB.z - ray.origin.z) / ray.direction.z;

			tmin = std::max(tmin, std::min(tz1, tz2));
			tmax = std::min(tmax, std::max(tz1, tz2));

			return tmax > 0 && tmax >= tmin;
		}
#pragma endregion


#pragma region TriangeMesh HitTest
		inline bool HitTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			if (!SlabTest_TriangleMesh(mesh, ray))
			{
				return false;
			}

			const size_t triangleCount{ mesh.normals.size() };
			float currentLowestDistance{ ray.max };

			for (size_t i{ 0 }; i < triangleCount; ++i)
			{
				// Construct triangle from vertex list
				const Vector3 v0{ mesh.transformedPositions[mesh.indices[3 * i + 0]] };
				const Vector3 v1{ mesh.transformedPositions[mesh.indices[3 * i + 1]] };
				const Vector3 v2{ mesh.transformedPositions[mesh.indices[3 * i + 2]] };

				Triangle triangle{ v0, v1, v2, mesh.transformedNormals[i] };
				triangle.cullMode = mesh.cullMode;
				triangle.materialIndex = mesh.materialIndex;

				HitRecord temp{};
				if (HitTest_TriangleMoellerTrumbore(triangle, ray, temp, ignoreHitRecord))
				{
					if (ignoreHitRecord) return true;

					// Is this the triangle in the mesh that should be renderer on top?
					if (temp.didHit && temp.t < currentLowestDistance)
					{
						currentLowestDistance = temp.t;
						hitRecord = temp;
					}
				}
			}

			return hitRecord.didHit;
		}

		inline bool HitTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_TriangleMesh(mesh, ray, temp, true);
		}
#pragma endregion
	}

	namespace LightUtils
	{
		//Direction from target to light
		inline Vector3 GetDirectionToLight(const Light& light, const Vector3 origin)
		{
			return light.origin - origin;
		}

		inline ColorRGB GetRadiance(const Light& light, const Vector3& target)
		{
			if (light.type == LightType::Directional)
			{
				return light.color * light.intensity;
			}

			return light.color * (light.intensity / (light.origin - target).SqrMagnitude());
		}
	}

	namespace Utils
	{
		//Just parses vertices and indices
#pragma warning(push)
#pragma warning(disable : 4505) //Warning unreferenced local function
		static bool ParseOBJ(const std::string& filename, std::vector<Vector3>& positions, std::vector<Vector3>& normals, std::vector<int>& indices)
		{
			std::ifstream file(filename);
			if (!file)
				return false;

			std::string sCommand;
			// start a while iteration ending when the end of file is reached (ios::eof)
			while (!file.eof())
			{
				//read the first word of the string, use the >> operator (istream::operator>>) 
				file >> sCommand;
				//use conditional statements to process the different commands	
				if (sCommand == "#")
				{
					// Ignore Comment
				}
				else if (sCommand == "v")
				{
					//Vertex
					float x, y, z;
					file >> x >> y >> z;
					positions.push_back({ x, y, z });
				}
				else if (sCommand == "f")
				{
					float i0, i1, i2;
					file >> i0 >> i1 >> i2;

					indices.push_back(static_cast<int>(i0) - 1);
					indices.push_back(static_cast<int>(i1) - 1);
					indices.push_back(static_cast<int>(i2) - 1);
				}
				//read till end of line and ignore all remaining chars
				file.ignore(1000, '\n');

				if (file.eof())
					break;
			}

			//Precompute normals
			for (uint64_t index = 0; index < indices.size(); index += 3)
			{
				uint32_t i0 = indices[index];
				uint32_t i1 = indices[index + 1];
				uint32_t i2 = indices[index + 2];

				Vector3 edgeV0V1 = positions[i1] - positions[i0];
				Vector3 edgeV0V2 = positions[i2] - positions[i0];
				Vector3 normal = Vector3::Cross(edgeV0V1, edgeV0V2);

				if (isnan(normal.x))
				{
					int k = 0;
				}

				normal.Normalize();
				if (isnan(normal.x))
				{
					int k = 0;
				}

				normals.push_back(normal);
			}

			return true;
		}
#pragma warning(pop)
	}
}