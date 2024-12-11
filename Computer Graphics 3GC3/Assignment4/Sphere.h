#ifndef SPHERE_H
#define SPHERE_H

#include <memory>

using namespace std;
class Material;

class HitResult {
public:
    HitResult() { m_isHit = false; };
    bool m_isHit;
    Vector3D m_hitPos;
    Vector3D m_hitNormal;
    shared_ptr<Material> m_hitMaterial;
    float m_t;
};


class Sphere {
    
public:
    Sphere() {}
    Sphere(Vector3D center, float r, shared_ptr<Material> m)
    {
        m_center = center;
        m_radius = r;
        m_pMaterial = m;
    }
    HitResult hit(Ray& r, float min_t, float max_t);

    public:
    Vector3D m_center;
    float m_radius;
    shared_ptr<Material> m_pMaterial;
};


//test if ray hits this sphere within range min_t and max_t
HitResult Sphere::hit(Ray& ray, float min_t, float max_t)
{
    HitResult hit_result;
    /* TODO: 2. compute ray hit information on the sphere */
    Vector3D oc = ray.origin() - m_center;
    auto half_b = dot(oc, ray.direction());
    auto a = ray.direction().length_squared();
    auto c = oc.length_squared() - m_radius * m_radius;

    auto discr = half_b * half_b - a * c;

    // and fill in hit result
    if (discr < 0) return hit_result; // no hit

    hit_result.m_t = ((-1) * half_b - sqrt(discr)) / ray.direction().length_squared();
    if (hit_result.m_t < min_t || hit_result.m_t > max_t) return hit_result; // too close or far

    hit_result.m_isHit = true;
    hit_result.m_hitPos = ray.origin() + hit_result.m_t * ray.direction();
    hit_result.m_hitNormal = (hit_result.m_hitPos - m_center) / m_radius;
    hit_result.m_hitMaterial = m_pMaterial;
    return hit_result;
}

#endif
