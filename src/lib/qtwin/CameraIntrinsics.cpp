#include "CameraIntrinsics.h"

namespace qtwin
{
    CameraIntrinsics::CameraIntrinsics(
        float f, 
        glm::vec2 p,
        glm::vec2 s
    ) : m_f(f), m_p(p), m_s(s), m_k(0.0f, 0.0f, 0.0f), m_t(0.0f, 0.0f)
    {}

    CameraIntrinsics::CameraIntrinsics(
        float f, 
        glm::vec2 p,
        glm::vec2 s,
        glm::vec3 k,
        glm::vec2 t
    ) : m_f(f), m_p(p), m_s(s), m_k(k), m_t(t)
    {}
        
    const float& CameraIntrinsics::GetFocalLength() const
    {
        return m_f;
    }

    const glm::vec2 CameraIntrinsics::GetPrincipalPoint() const
    {
        return m_p;
    }

    const glm::vec2 CameraIntrinsics::GetSpatialResolution() const
    {
        return m_s;
    }

    const glm::vec3 CameraIntrinsics::GetRadialDistortion() const
    {
        return m_k;
    }

    const glm::vec2 CameraIntrinsics::GetTangentialDistortion() const
    {
        return m_t;
    }
}

