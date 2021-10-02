#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/glm.hpp>

void FreeMovement(glm::mat4& a_m4Transform, float a_fDeltaTime, float a_fSpeed, const glm::vec3& a_v3Up = { 0,1,0 });
#endif//!__CAMERA_H_