#ifndef MATERIAL_H
#define MATERIAL_H

class HitResult;

class ReflectResult
{
public:
    Ray m_ray;
    Vector3D m_color;
};

class Material
{
public:
    Vector3D m_color;
    virtual ReflectResult reflect(Ray& ray, HitResult& hit) = 0;
};


class Diffuse : public Material
{
public:
    Diffuse(const Vector3D& color)
    {
        m_color = color;
    };
    
    virtual ReflectResult reflect(Ray& ray, HitResult& hit) override
    {
        ReflectResult res;
        
        //TODO: 4. generate one scattered ray and assign it to res.m_ray
        
        // pick random x, y, z, check if valid points in a hemisphere aligned with normal
        float x, y, z, dist;
        Vector3D diffuse_dir;
        do {
            x = 1 - random_float() * 2;
            y = 1 - random_float() * 2;
            z = 1 - random_float() * 2;
            dist = sqrt(x * x + y * y + z * z);
            diffuse_dir = Vector3D(x, y, z);
        }
        // 
        while(dist > 1 || dot(diffuse_dir, hit.m_hitNormal) < 0);

        diffuse_dir = normalize(diffuse_dir);

        res.m_ray = Ray(hit.m_hitPos, diffuse_dir);
        res.m_color = m_color;
        return res;
    }
};


class Specular : public Material
{
public:
    Specular(const Vector3D& color)
    {
        m_color = color;
    }
    
    virtual ReflectResult reflect(Ray& ray, HitResult& hit) override
    {
        ReflectResult res;
        //TODO: 5. generate one mirrored ray and assign it to res.m_ray
        
        Vector3D specular_dir = ray.direction() - 2 * dot(ray.direction(), hit.m_hitNormal) * hit.m_hitNormal;

        res.m_ray = Ray(hit.m_hitPos, specular_dir);
        res.m_color = m_color;
        return res;
    }
};
#endif
