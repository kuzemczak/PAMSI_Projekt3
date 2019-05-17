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

template < typename T>
int find_i(const std::vector<T>  & vecOfElements, const T  & element)
{
	int ret;

	// Find given element in vector
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

	if (it != vecOfElements.end())
	{
		ret = distance(vecOfElements.begin(), it);
	}
	else
	{
		ret = -1;
	}

	return ret;
}

std::ostream& operator<< (std::ostream& out, const glm::mat3 & v);
std::ostream& operator<< (std::ostream& out, const glm::mat4 & v);
std::ostream& operator<< (std::ostream& out, const glm::vec2 & v);
std::ostream& operator<< (std::ostream& out, const glm::vec3 & v);
std::ostream& operator<< (std::ostream& out, const glm::vec4 & v);

bool has_bits_set(int integer, int bits);
