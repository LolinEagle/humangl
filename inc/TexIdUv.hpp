#pragma once

#include <VeFrameInfo.hpp>
#include "header.hpp"

#define MCM(name, face, x, y, w, h, flag) \
	const TexIdMapping mc##name##face = p(1024 + (x), 1024 + (y), (w), (h), (flag));

struct TexUvMapping {
	static constexpr uint ATLAS_WIDTH = 2048;
	static constexpr uint ATLAS_HEIGHT = 2048;

	static constexpr uint TILE = 512;

	static vem::vec2 uv(uint x, uint y){
		return vem::vec2(
			static_cast<float>(x) / static_cast<float>(ATLAS_WIDTH),
			static_cast<float>(y) / static_cast<float>(ATLAS_WIDTH)
		);
	}
	
	static TexIdMapping p(uint x, uint y,
						     uint w, uint h,
						     uint flags){
		return {uv(x, y), uv(x + w, y + h), flags};
	}

	static TexIdMapping t(uint x, uint y, uint flags){
		return p(x * TILE, y * TILE, TILE, TILE, flags);
	}

	const TexIdMapping skyboxTop    = t(1, 0, TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	const TexIdMapping skyboxBottom = t(1, 2, TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	const TexIdMapping skyboxFront  = t(1, 1, TMAP_FLIP_HORIZONTAL);
	const TexIdMapping skyboxBack   = t(3, 1, TMAP_FLIP_HORIZONTAL);
	const TexIdMapping skyboxLeft   = t(0, 1, TMAP_FLIP_HORIZONTAL);
	const TexIdMapping skyboxRight  = t(2, 1, TMAP_FLIP_HORIZONTAL);

	const TexIdMapping lolin        = t(0, 0, TMAP_NONE);
	const TexIdMapping maix         = t(0, 2, TMAP_NONE);

	const TexIdMapping wood         = t(2, 0, TMAP_NONE);
	const TexIdMapping stone        = t(3, 0, TMAP_NONE);

	MCM(Head, Front,  8,  8, 8, 8, TMAP_NONE);
	MCM(Head, Back,   24, 8, 8, 8, TMAP_NONE);
	MCM(Head, Left,   16, 8, 8, 8, TMAP_NONE);
	MCM(Head, Right,  0,  8, 8, 8, TMAP_NONE);
	MCM(Head, Top,    8,  0, 8, 8, TMAP_NONE);
	MCM(Head, Bottom, 16, 0, 8, 8, TMAP_NONE);

	MCM(Torso, Front,  20, 20, 8, 12, TMAP_NONE);
	MCM(Torso, Back,   32, 20, 8, 12, TMAP_NONE);
	MCM(Torso, Left,   16, 20, 4, 12, TMAP_NONE);
	MCM(Torso, Right,  28, 20, 4, 12, TMAP_NONE);
	MCM(Torso, Top,    20, 16, 8, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	MCM(Torso, Bottom, 28, 16, 8, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);


	MCM(LeftArmUpper, Front,  36, 52, 4, 6, TMAP_NONE);
	MCM(LeftArmUpper, Back,   44, 52, 4, 6, TMAP_NONE);
	MCM(LeftArmUpper, Left,   32, 52, 4, 6, TMAP_NONE);
	MCM(LeftArmUpper, Right,  40, 52, 4, 6, TMAP_NONE);
	MCM(LeftArmUpper, Top,    36, 48, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	MCM(LeftArmUpper, Bottom, 40, 48, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	
	MCM(LeftArmLower, Front,  36, 56, 4, 6, TMAP_NONE);
	MCM(LeftArmLower, Back,   44, 56, 4, 6, TMAP_NONE);
	MCM(LeftArmLower, Left,   32, 56, 4, 6, TMAP_NONE);
	MCM(LeftArmLower, Right,  40, 56, 4, 6, TMAP_NONE);
	MCM(LeftArmLower, Top,    36, 48, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	MCM(LeftArmLower, Bottom, 40, 48, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);

	MCM(LeftLegUpper, Front,  20, 52, 4, 6, TMAP_NONE);
	MCM(LeftLegUpper, Back,   28, 52, 4, 6, TMAP_NONE);
	MCM(LeftLegUpper, Left,   16, 52, 4, 6, TMAP_NONE);
	MCM(LeftLegUpper, Right,  24, 52, 4, 6, TMAP_NONE);
	MCM(LeftLegUpper, Top,    20, 48, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	MCM(LeftLegUpper, Bottom, 24, 48, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	
	MCM(LeftLegLower, Front,  20, 56, 4, 6, TMAP_NONE);
	MCM(LeftLegLower, Back,   28, 56, 4, 6, TMAP_NONE);
	MCM(LeftLegLower, Left,   16, 56, 4, 6, TMAP_NONE);
	MCM(LeftLegLower, Right,  24, 56, 4, 6, TMAP_NONE);
	MCM(LeftLegLower, Top,    20, 48, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	MCM(LeftLegLower, Bottom, 24, 48, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);

	MCM(RightLegUpper, Front,   4, 20, 4, 6, TMAP_NONE);
	MCM(RightLegUpper, Back,   12, 20, 4, 6, TMAP_NONE);
	MCM(RightLegUpper, Left,    0, 20, 4, 6, TMAP_NONE);
	MCM(RightLegUpper, Right,   8, 20, 4, 6, TMAP_NONE);
	MCM(RightLegUpper, Top,     4, 16, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	MCM(RightLegUpper, Bottom,  8, 16, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	
	MCM(RightLegLower, Front,   4, 24, 4, 6, TMAP_NONE);
	MCM(RightLegLower, Back,   12, 24, 4, 6, TMAP_NONE);
	MCM(RightLegLower, Left,    0, 24, 4, 6, TMAP_NONE);
	MCM(RightLegLower, Right,   8, 24, 4, 6, TMAP_NONE);
	MCM(RightLegLower, Top,     4, 16, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	MCM(RightLegLower, Bottom,  8, 16, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);

	MCM(RightArmUpper, Front,  44, 20, 4, 6, TMAP_NONE);
	MCM(RightArmUpper, Back,   52, 20, 4, 6, TMAP_NONE);
	MCM(RightArmUpper, Left,   40, 20, 4, 6, TMAP_NONE);
	MCM(RightArmUpper, Right,  48, 20, 4, 6, TMAP_NONE);
	MCM(RightArmUpper, Top,    44, 16, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	MCM(RightArmUpper, Bottom, 48, 16, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	
	MCM(RightArmLower, Front,  44, 24, 4, 6, TMAP_NONE);
	MCM(RightArmLower, Back,   52, 24, 4, 6, TMAP_NONE);
	MCM(RightArmLower, Left,   40, 24, 4, 6, TMAP_NONE);
	MCM(RightArmLower, Right,  48, 24, 4, 6, TMAP_NONE);
	MCM(RightArmLower, Top,    44, 16, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	MCM(RightArmLower, Bottom, 48, 16, 4, 4 , TMAP_FLIP_HORIZONTAL | TMAP_ROT_180);
	
	//MCM(head, Bottom, 0, 0, 0, 0, TMAP_NONE);
};
