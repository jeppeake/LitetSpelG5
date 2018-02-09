#include "SAIB.h"

glm::vec3 SAIB::flyTo(glm::vec3 position, glm::quat orientation, glm::vec3 target) {
	glm::vec3 front = glm::normalize(glm::toMat3(orientation) * glm::vec3(0.0, 0.0, 1.0));
	glm::vec3 up = glm::normalize(glm::toMat3(orientation) * glm::vec3(0.0, 1.0, 0.0));
	glm::vec3 left = glm::normalize(glm::toMat3(orientation) * glm::vec3(1.0, 0.0, 0.0));

	glm::vec3 pt = glm::normalize(target - position);

	float pow = length(pt - front) / 2;
	float roll = 0.f;
	if (pow > 0.2) {
		roll = -test2Axis(position, orientation, target, front, up);
	} else {
		roll = -test2Axis(position, orientation, position + glm::vec3(0.0, 1.0, 0.0), front, up);
	}
	float pitch = test2Axis(position, orientation, target, left, front);
	float yaw = -test2Axis(position, orientation, target, up, front);

	//std::cout << roll << " : " << pitch << " : " << yaw << " : " << glm::length(position - target) << " : " << pow <<"\n";
	return glm::vec3(roll, pitch, yaw);
}

glm::vec3 SAIB::flyToRes(glm::vec3 position, glm::quat orientation, glm::vec3 target, glm::vec3 restrictions) {
	glm::vec3 front = glm::normalize(glm::toMat3(orientation) * glm::vec3(0.0, 0.0, 1.0));
	glm::vec3 up = glm::normalize(glm::toMat3(orientation) * glm::vec3(0.0, 1.0, 0.0));
	glm::vec3 left = glm::normalize(glm::toMat3(orientation) * glm::vec3(1.0, 0.0, 0.0));

	glm::vec3 pt = glm::normalize(target - position);

	float pow = length(pt - front) / 2;
	float roll = 0.f;
	if (pow > 0.2) {
		roll = -test2Axis(position, orientation, target, front, up);
	}
	else {
		roll = -test2Axis(position, orientation, position + glm::vec3(0.0, 1.0, 0.0), front, up);
	}
	float pitch = test2Axis(position, orientation, target, left, front);
	float yaw = -test2Axis(position, orientation, target, up, front);

	//std::cout << roll << " : " << pitch << " : " << yaw << " : " << glm::length(position - target) << " : " << pow <<"\n";
	return glm::vec3(roll * restrictions.x, pitch * restrictions.y, yaw * restrictions.z);
}

glm::vec3 SAIB::fly_to(glm::vec3 position, glm::quat orientation, glm::vec3 target) {//fly to target, ignore orientation
	//return flyTo(position, orientation, target);
	//std::cout << glm::length(position - target) << "\n";

	glm::vec3 front = glm::toMat3(orientation) * glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 up = glm::toMat3(orientation) * glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 left = glm::toMat3(orientation) * glm::vec3(1.0, 0.0, 0.0);

	glm::vec3 t = target;

	glm::vec3 pt = t - position;
	pt = glm::normalize(pt);

	glm::vec3 pt_proj_on_front = glm::proj(pt, front);

	glm::vec3 pt_adjusted_for_roll_towards_target = glm::normalize(pt - pt_proj_on_front);

	glm::vec3 input = glm::vec3(0.0, 0.0, 0.0);
	float tolerance = 0.2;
	if (glm::length(front - pt) > tolerance) {//fast mode
		if (glm::length(up - pt_adjusted_for_roll_towards_target) > tolerance) {//roll mode
			input += glm::vec3(-testAxis(position, orientation, target, front), 0.0, 0.0);
		} else {//pitch mode
			input += glm::vec3(0.0, testAxis(position, orientation, target, left), 0.0);
		}
	} else {//precision mode
		float adjust = 2.0;
		float roll = -testAxis(position, orientation, position + glm::vec3(0.0, 1.0, 0.0), front);
		float pitch = -testAxis(position, orientation, target, left) / adjust;
		float yaw = testAxis(position, orientation, target, up) / adjust;
		input += glm::vec3(roll, pitch, yaw);
	}
	return input;
}

float SAIB::rollTowards(glm::vec3 position, glm::quat orientation, glm::vec3 target) {
	glm::vec3 front = glm::toMat3(orientation) * glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 up = glm::toMat3(orientation) * glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 left = glm::toMat3(orientation) * glm::vec3(1.0, 0.0, 0.0);

	glm::vec3 t = target;

	glm::vec3 pt = t - position;
	pt = glm::normalize(pt);

	glm::vec3 pt_proj_on_front = glm::proj(pt, front);

	glm::vec3 pt_adjusted_for_roll = glm::normalize(pt - pt_proj_on_front);
	glm::vec3 roll_right_t = glm::rotate(up, glm::radians(5.f), front);
	glm::vec3 roll_left_t = glm::rotate(up, glm::radians(-5.f), front);

	return testTowards(roll_left_t, roll_right_t, pt_adjusted_for_roll);
}

float SAIB::pitchTowards(glm::vec3 position, glm::quat orientation, glm::vec3 target) {
	glm::vec3 front = glm::toMat3(orientation) * glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 up = glm::toMat3(orientation) * glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 left = glm::toMat3(orientation) * glm::vec3(1.0, 0.0, 0.0);

	glm::vec3 t = target;

	glm::vec3 pt = t - position;
	pt = glm::normalize(pt);

	glm::vec3 pt_proj_on_left = glm::proj(pt, left);

	glm::vec3 pt_adjusted_for_pitch = glm::normalize(pt - pt_proj_on_left);
	glm::vec3 pitch_up_t = glm::rotate(up, glm::radians(5.f), front);
	glm::vec3 pitch_down_t = glm::rotate(up, glm::radians(-5.f), front);

	return testTowards(pitch_up_t, pitch_down_t, pt_adjusted_for_pitch);
}

float SAIB::yawTowards(glm::vec3 position, glm::quat orientation, glm::vec3 target) {
	glm::vec3 front = glm::toMat3(orientation) * glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 up = glm::toMat3(orientation) * glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 left = glm::toMat3(orientation) * glm::vec3(1.0, 0.0, 0.0);

	glm::vec3 t = target;

	glm::vec3 pt = t - position;
	pt = glm::normalize(pt);

	glm::vec3 pt_proj_on_up = glm::proj(pt, up);

	glm::vec3 pt_adjusted_for_yaw = glm::normalize(pt - pt_proj_on_up);
	glm::vec3 yaw_right_t = glm::rotate(up, glm::radians(5.f), front);
	glm::vec3 yaw_left_t = glm::rotate(up, glm::radians(-5.f), front);
	return testTowards(yaw_right_t, yaw_left_t, pt_adjusted_for_yaw);
}

float SAIB::testAxis(glm::vec3 position, glm::quat orientation, glm::vec3 target, glm::vec3 axis) {
	glm::vec3 front = glm::normalize(glm::toMat3(orientation) * glm::vec3(0.0, 0.0, 1.0));
	glm::vec3 up = glm::normalize(glm::toMat3(orientation) * glm::vec3(0.0, 1.0, 0.0));
	glm::vec3 left = glm::normalize(glm::toMat3(orientation) * glm::vec3(1.0, 0.0, 0.0));

	glm::vec3 t = target;

	glm::vec3 pt = t - position;
	pt = glm::normalize(pt);

	glm::vec3 pt_proj_on_axis = glm::proj(pt, axis);

	glm::vec3 pt_adjusted_for_axis = glm::normalize(pt - pt_proj_on_axis);
	glm::vec3 axis_plus_t = glm::rotate(up, glm::radians(5.f), front);
	glm::vec3 axis_minus_t = glm::rotate(up, glm::radians(-5.f), front);

	return testTowards(axis_plus_t, axis_minus_t, pt_adjusted_for_axis);
}

float SAIB::test2Axis(glm::vec3 position, glm::quat orientation, glm::vec3 target, glm::vec3 rot_axis, glm::vec3 test_axis) {
	glm::vec3 pt = glm::normalize(target - position);
	float pow = glm::length(pt - test_axis) / 2;

	glm::vec3 axis_plus_t = glm::rotate(test_axis, glm::radians(5.f), rot_axis);
	glm::vec3 axis_minus_t = glm::rotate(test_axis, glm::radians(-5.f), rot_axis);

	return testTowards(axis_plus_t, axis_minus_t, pt) * glm::pow(pow, 0.5);
}

float SAIB::testTowards(glm::vec3 v1, glm::vec3 v2, glm::vec3 t) {
	float ret = 1.f;
	if (length(v1 - t) < length(v2 - t)) {
		ret = -1.f;
	}
	return ret;
}

glm::vec3 SAIB::calculateInterdiction(entityx::Entity target, entityx::Entity interdictor) {
	glm::vec3 target_vector;
	if (glm::length(target.component<Physics>()->velocity) > 0.0001) {
		target_vector = glm::normalize(target.component<Physics>()->velocity);
	}

	glm::vec3 target_position = target.component<Transform>()->pos;

	float target_speed = target.component<FlightComponent>()->current_speed;

	glm::vec3 AI_position = interdictor.component<Transform>()->pos;

	glm::vec3 projectile_origin = AI_position;
	float projectile_speed = interdictor.component<FlightComponent>()->current_speed + interdictor.component<Equipment>()->primary.at(0).stats.speed;

	float t = glm::length(target_position - AI_position) / (projectile_speed - target_speed);

	glm::vec3 aimPos = target_position + (target_vector * target_speed * t);

	return aimPos;
}