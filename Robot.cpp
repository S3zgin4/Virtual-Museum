#include "Robot.h"

void Robot::moveTo(const glm::vec3& target) {
    targetPosition = target;
    movingToTarget = true;
}