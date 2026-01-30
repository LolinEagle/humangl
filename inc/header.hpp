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
#define MAX_TEX_ID 128

// usage: TEXID(TEX_FACE, Front)
#define TEXID(raw, face) (static_cast<uint>(((raw) << 3) + (face)))

enum Body{
	TORSO,
	HEAD,
	HAT_BASE,
	HAT,
	LEFT_UPPER_ARM,
	LEFT_LOWER_ARM,
	RIGHT_UPPER_ARM,
	RIGHT_LOWER_ARM,
	LEFT_UPPER_LEG,
	LEFT_LOWER_LEG,
	RIGHT_UPPER_LEG,
	RIGHT_LOWER_LEG,
	BODY_LENGTH
};

enum Scene{
	HUMANGL,
	SCOP
};


enum FaceId {
	None = 0,
	Front,
	Back,
	Left,
	Right,
	Top,
	Bottom,
	_FaceIdLast,
};

// the forumla to get a texId for an actual face is as such:
// texId = (texIdRaw << 3) | (faceId)
// 
enum TexIdRaw {
	TEX_NONE = 0,
	
	TEX_SKYBOX,
	TEX_GROUND,
	TEX_HEAD,
	TEX_TORSO,
	TEX_LEFT_UPPER_ARM,
	TEX_LEFT_LOWER_ARM,
	TEX_RIGHT_UPPER_ARM,
	TEX_RIGHT_LOWER_ARM,
	TEX_LEFT_UPPER_LEG,
	TEX_LEFT_LOWER_LEG,
	TEX_RIGHT_UPPER_LEG,
	TEX_RIGHT_LOWER_LEG,

	// used to assert that we never got out of bounds :D
	_TEX_LAST,
};
