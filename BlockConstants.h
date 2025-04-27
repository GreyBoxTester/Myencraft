#pragma once
#include "Vector2.h"

#define IS_DRAWABLE_BLOCK(blocktype) ((unsigned char)(blocktype)>0u)
#define IS_FULL_BLOCK(blocktype) ((unsigned char)(blocktype)>5u)
#define IS_TRANSPARENT_BLOCK(blocktype) ((unsigned char)(blocktype)<8u)
//#define IS_TRANSPARENT_AND_NOT_EQUAL(blocktype, comp) (IS_TRANSPARENT_BLOCK(blocktype) && blocktype != comp)
#define IS_FLUID(blocktype) (((unsigned char)(blocktype)>0u)&&((unsigned char)(blocktype)<2u))
#define NOT_IGNORED_BY_RAY(blocktype) ((unsigned char)(blocktype)>1u)
#define IS_PLANT(blocktype) (((unsigned char)(blocktype)>1u)&&((unsigned char)(blocktype)<7u))
#define IS_X_SHAPED(blocktype) (((unsigned char)(blocktype)>1u)&&((unsigned char)(blocktype)<6u))

#define BLOCK_PIXEL_SIZE 0.25f //is 16 / 64

namespace mc
{
	enum class BlockType : unsigned char
	{
		Air = 0u,
		//drawable blocks
		//transparent blocks
		Water,
		GrassTuft,
		OxyeyeDaisy,
		BlueOrchid,
		SugarCane,
		//full blocks
		OakLeaves,
		Glass,
		
		Grass,
		Dirt,
		Stone,
		Sand,
		OakLog,
		Bedrock
	};

	struct TextureData
	{
		cl::Vector2f top;
		cl::Vector2f side;
		cl::Vector2f bottom;
	};

	const TextureData textureData[14] = {
		{},//air
		{{0.25f, 0.25f}, {0.25f, 0.25f}, {0.25f, 0.25f}},//water
		{{0.0f, 0.75f}, {0.0f, 0.75f}, {0.0f, 0.75f}},//grass tuft
		{{0.25f, 0.75f}, {0.25f, 0.75f}, {0.25f, 0.75f}},//oxyeye daisy
		{{0.5f, 0.75f}, {0.5f, 0.75f}, {0.5f, 0.75f}},//blue orchideria
		{{0.75f, 0.25f}, {0.75f, 0.25f}, {0.75f, 0.25f}},//sugar cane
		{{0.5f, 0.5f}, {0.5f, 0.5f}, {0.5f, 0.5f}},//oak leaves
		{{0.75f, 0.5f}, {0.75f, 0.5f}, {0.75f, 0.5f}},//glass
		{{0.0f, 0.0f}, {0.0f, 0.25f}, {0.25f, 0.0f}},//grass block
		{{0.25f, 0.0f}, {0.25f, 0.0f}, {0.25f, 0.0f}},//dirt
		{{0.5f, 0.0f}, {0.5f, 0.0f}, {0.5f, 0.0f}},//stone
		{{0.75f, 0.0f}, {0.75f, 0.0f}, {0.75f, 0.0f}},//sand
		{{0.25f, 0.5f}, {0.0f, 0.5f}, {0.25f, 0.5f}},//oak log
		{{0.5f, 0.25f}, {0.5f, 0.25f}, {0.5f, 0.25f}} //bedrock
	};
}