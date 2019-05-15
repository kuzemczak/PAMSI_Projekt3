#pragma once

#include <vector>
#include <iostream>
#include <cstdint>
#include "glm/glm.hpp"

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
	if (!v.empty()) {
		out << '[';

		for (T element : v)
			out << element << ", ";

		out << "\b\b]";
	}
	return out;
}

std::ostream& operator<< (std::ostream& out, const glm::mat3 & v);
std::ostream& operator<< (std::ostream& out, const glm::mat4 & v);
std::ostream& operator<< (std::ostream& out, const glm::vec2 & v);
std::ostream& operator<< (std::ostream& out, const glm::vec3 & v);
std::ostream& operator<< (std::ostream& out, const glm::vec4 & v);

bool has_bits_set(int integer, int bits);
