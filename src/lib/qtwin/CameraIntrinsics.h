//
// Simple pinhole intrinsics model.
//
// Will expand later with more practical models.
//
 
#pragma once

#include <glm/glm.hpp>

namespace qtwin // Probably belongs in another namespace/library entirely.
{
    class CameraIntrinsics
    {
    public:
        CameraIntrinsics(
            float f, 
            glm::vec2 p,
            glm::vec2 s
        );
        
        const float& GetFocalLength() const;
        const glm::vec2 GetPrincipalPoint() const;
        const glm::vec2 GetSpatialResolution() const;

    private:
        float&    m_f;
        glm::vec2 m_p;
        glm::vec2 m_s;
    };
}

