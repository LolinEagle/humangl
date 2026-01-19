#pragma once

// glfw3
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Include
#include <chrono>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <png.h>
#include <sstream>
#include <stdexcept>
#include <vem.hpp>

// Containers
#include <array>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Define
#define TITLE "humangl"
#define WIDTH 1600
#define HEIGHT 900
#define WIDTH_HALF 800
#define HEIGHT_HALF 450
#define TWO_PI 6.283185307179586476925286766559f
#define MAX_FRAMES 2
#define MAX_LIGHTS 16

enum Body{
	NONE,
	TORSO,
	HEAD,
	LEFT_UPPER_ARM,
	LEFT_LOWER_ARM,
	RIGHT_UPPER_ARM,
	RIGHT_LOWER_ARM,
	LEFT_UPPER_LEG,
	LEFT_LOWER_LEG,
	RIGHT_UPPER_LEG,
	RIGHT_LOWER_LEG
};
