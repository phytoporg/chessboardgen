#include "CameraIntrinsics.h"

namespace qtwin
{
    CameraIntrinsics::CameraIntrinsics(
        float f, 
        glm::vec2 p,
        glm::vec2 s
    ) : m_f(f), m_p(p), m_s(s)
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
}

