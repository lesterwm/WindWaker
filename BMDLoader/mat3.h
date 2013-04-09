#ifndef BMD_MAT3_H
#define BMD_MAT3_H BMD_MAT3_H

#include "common.h"

#include <iosfwd>

struct MColor
{
  u8 r, g, b, a;
};

struct Color16
{
  s16 r, g, b, a;
};

struct ColorChanInfo
{
  //not sure if this is right
  u8 ambColorSource;
  u8 matColorSource;
  u8 litMask;
  u8 attenuationFracFunc;
  u8 diffuseAttenuationFunc;
};

struct TexGenInfo
{
  u8 texGenType;
  u8 texGenSrc;
  u8 matrix;
};

struct TexMtxInfo
{
  float scaleCenterX, scaleCenterY;
  float scaleU, scaleV;
};

struct TevOrderInfo
{
  u8 texCoordId;
  u8 texMap;
  u8 chanId;
};

struct TevSwapModeInfo
{
  u8 rasSel;
  u8 texSel;
};

struct TevSwapModeTable
{
  u8 r;
  u8 g;
  u8 b;
  u8 a;
};

struct AlphaCompare
{
  u8 comp0, ref0;
  u8 alphaOp;
  u8 comp1, ref1;
};

struct BlendInfo
{
  u8 blendMode;
  u8 srcFactor, dstFactor;
  u8 logicOp;
};

struct ZMode
{
  bool enable;
  u8 zFunc;
  bool enableUpdate;
};


struct TevStageInfo
{
  //GX_SetTevColorIn() arguments
  u8 colorIn[4]; //GX_CC_*

  //GX_SetTevColorOp() arguments
  u8 colorOp;
  u8 colorBias;
  u8 colorScale;
  u8 colorClamp;
  u8 colorRegId;

  //GX_SetTevAlphaIn() arguments
  u8 alphaIn[4]; //GC_CA_*

  //GX_SetTevAlphaOp() arguments
  u8 alphaOp;
  u8 alphaBias;
  u8 alphaScale;
  u8 alphaClamp;
  u8 alphaRegId;
};

struct Material
{
  u8 flag;

  // Unknown/Unused
  u8 numChansIndex;
  
  u16 color1[2];
  u16 chanControls[4];
  u16 color2[2];
  
  // State settings
  u16 alphaCompIndex;
  u16 blendIndex;
  u8 zModeIndex;
  u8 cullIndex;

  // Texture Units (8)
  u8 texGenCountIndex;
  u16 texGenInfos[8];
  u16 texMtxInfos[8];
  u16 texStages[8];

  // TEV Units (16)
  u8 tevCountIndex;
  u16 tevStageInfo[16];
  u16 tevOrderInfo[16];
  u16 tevSwapModeInfo[16]; // TODO: Add WARN messages that this is unsupported
  u16 tevSwapModeTable[4]; // TODO: Add WARN messages that this is unsupported
  //this is to be loaded into
  //GX_CC_CPREV - GX_CC_A2??
  u16 colorS10[4];

  u16 color3[4]; // The Konst Color Registers. This is an index into the actual colors stored in Mat3::color3
  u8 constColorSel[16]; //Inputs to GX_SetTevKColorSel. For each TEV Stage, binds a constant or a Konst Color Register value to the KONST register.
  u8 constAlphaSel[16]; //Inputs to GX_SetTevKColorSel. For each TEV Stage, binds a constant or a Konst Color Register value to the KONST register.
};

struct Mat3
{
  std::vector<MColor> color1;
  std::vector<u8> numChans;
  std::vector<ColorChanInfo> colorChanInfos;
  std::vector<MColor> color2;

  std::vector<Material> materials;
  std::vector<int> indexToMatIndex;
  std::vector<std::string> stringtable;

  std::vector<u32> cullModes;

  std::vector<u8> texGenCounts;
  std::vector<TexGenInfo> texGenInfos;
  std::vector<TexMtxInfo> texMtxInfos;

  std::vector<int> texStageIndexToTextureIndex;
  std::vector<TevOrderInfo> tevOrderInfos;
  std::vector<Color16> colorS10;
  std::vector<MColor> color3;
  std::vector<u8> tevCounts;
  std::vector<TevStageInfo> tevStageInfos;
  std::vector<TevSwapModeInfo> tevSwapModeInfos;
  std::vector<TevSwapModeTable> tevSwapModeTables;
  std::vector<AlphaCompare> alphaCompares;
  std::vector<BlendInfo> blendInfos;
  std::vector<ZMode> zModes;
};

void dumpMat3(Chunk* f, Mat3& dst);
void writeMat3Info(Chunk* f, std::ostream& out);;

#endif //BMD_MAT3_H
