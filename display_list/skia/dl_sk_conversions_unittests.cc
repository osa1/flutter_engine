// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/display_list/skia/dl_sk_conversions.h"

#include "flutter/display_list/display_list_blend_mode.h"
#include "flutter/display_list/display_list_paint.h"
#include "flutter/display_list/display_list_sampling_options.h"
#include "flutter/display_list/display_list_tile_mode.h"
#include "flutter/display_list/display_list_vertices.h"
#include "flutter/display_list/types.h"
#include "gtest/gtest.h"
#include "include/core/SkSamplingOptions.h"

namespace flutter {
namespace testing {

TEST(DisplayListImageFilter, LocalImageSkiaNull) {
  auto blur_filter =
      std::make_shared<DlBlurImageFilter>(0, 0, DlTileMode::kClamp);
  DlLocalMatrixImageFilter dl_local_matrix_filter(SkMatrix::RotateDeg(45),
                                                  blur_filter);
  // With sigmas set to zero on the blur filter, Skia will return a null filter.
  // The local matrix filter should return nullptr instead of crashing.
  ASSERT_EQ(ToSk(dl_local_matrix_filter), nullptr);
}

TEST(DisplayListSkConversions, ToSkTileMode) {
  ASSERT_EQ(ToSk(DlTileMode::kClamp), SkTileMode::kClamp);
  ASSERT_EQ(ToSk(DlTileMode::kRepeat), SkTileMode::kRepeat);
  ASSERT_EQ(ToSk(DlTileMode::kMirror), SkTileMode::kMirror);
  ASSERT_EQ(ToSk(DlTileMode::kDecal), SkTileMode::kDecal);
}

TEST(DisplayListSkConversions, ToSkDrawStyle) {
  ASSERT_EQ(ToSk(DlDrawStyle::kFill), SkPaint::Style::kFill_Style);
  ASSERT_EQ(ToSk(DlDrawStyle::kStroke), SkPaint::Style::kStroke_Style);
  ASSERT_EQ(ToSk(DlDrawStyle::kStrokeAndFill),
            SkPaint::Style::kStrokeAndFill_Style);
}

TEST(DisplayListSkConversions, ToSkStrokeCap) {
  ASSERT_EQ(ToSk(DlStrokeCap::kButt), SkPaint::Cap::kButt_Cap);
  ASSERT_EQ(ToSk(DlStrokeCap::kRound), SkPaint::Cap::kRound_Cap);
  ASSERT_EQ(ToSk(DlStrokeCap::kSquare), SkPaint::Cap::kSquare_Cap);
}

TEST(DisplayListSkConversions, ToSkStrokeJoin) {
  ASSERT_EQ(ToSk(DlStrokeJoin::kMiter), SkPaint::Join::kMiter_Join);
  ASSERT_EQ(ToSk(DlStrokeJoin::kRound), SkPaint::Join::kRound_Join);
  ASSERT_EQ(ToSk(DlStrokeJoin::kBevel), SkPaint::Join::kBevel_Join);
}

TEST(DisplayListSkConversions, ToSkVertexMode) {
  ASSERT_EQ(ToSk(DlVertexMode::kTriangles),
            SkVertices::VertexMode::kTriangles_VertexMode);
  ASSERT_EQ(ToSk(DlVertexMode::kTriangleStrip),
            SkVertices::VertexMode::kTriangleStrip_VertexMode);
  ASSERT_EQ(ToSk(DlVertexMode::kTriangleFan),
            SkVertices::VertexMode::kTriangleFan_VertexMode);
}

TEST(DisplayListSkConversions, ToSkFilterMode) {
  ASSERT_EQ(ToSk(DlFilterMode::kLinear), SkFilterMode::kLinear);
  ASSERT_EQ(ToSk(DlFilterMode::kNearest), SkFilterMode::kNearest);
  ASSERT_EQ(ToSk(DlFilterMode::kLast), SkFilterMode::kLast);
}

TEST(DisplayListSkConversions, ToSkSrcRectConstraint) {
  ASSERT_EQ(ToSk(DlCanvas::SrcRectConstraint::kFast),
            SkCanvas::SrcRectConstraint::kFast_SrcRectConstraint);
  ASSERT_EQ(ToSk(DlCanvas::SrcRectConstraint::kStrict),
            SkCanvas::SrcRectConstraint::kStrict_SrcRectConstraint);
}

TEST(DisplayListSkConversions, ToSkSamplingOptions) {
  ASSERT_EQ(ToSk(DlImageSampling::kLinear),
            SkSamplingOptions(SkFilterMode::kLinear, SkMipmapMode::kNone));
  ASSERT_EQ(ToSk(DlImageSampling::kMipmapLinear),
            SkSamplingOptions(SkFilterMode::kLinear, SkMipmapMode::kLinear));
  ASSERT_EQ(ToSk(DlImageSampling::kNearestNeighbor),
            SkSamplingOptions(SkFilterMode::kNearest, SkMipmapMode::kNone));
  ASSERT_EQ(ToSk(DlImageSampling::kCubic),
            SkSamplingOptions(SkCubicResampler{1 / 3.0f, 1 / 3.0f}));
}

#define FOR_EACH_BLEND_MODE_ENUM(FUNC) \
  FUNC(kSrc)                           \
  FUNC(kClear)                         \
  FUNC(kSrc)                           \
  FUNC(kDst)                           \
  FUNC(kSrcOver)                       \
  FUNC(kDstOver)                       \
  FUNC(kSrcIn)                         \
  FUNC(kDstIn)                         \
  FUNC(kSrcOut)                        \
  FUNC(kDstOut)                        \
  FUNC(kSrcATop)                       \
  FUNC(kDstATop)                       \
  FUNC(kXor)                           \
  FUNC(kPlus)                          \
  FUNC(kModulate)                      \
  FUNC(kScreen)                        \
  FUNC(kOverlay)                       \
  FUNC(kDarken)                        \
  FUNC(kLighten)                       \
  FUNC(kColorDodge)                    \
  FUNC(kColorBurn)                     \
  FUNC(kHardLight)                     \
  FUNC(kSoftLight)                     \
  FUNC(kDifference)                    \
  FUNC(kExclusion)                     \
  FUNC(kMultiply)                      \
  FUNC(kHue)                           \
  FUNC(kSaturation)                    \
  FUNC(kColor)                         \
  FUNC(kLuminosity)                    \
  FUNC(kLastCoeffMode)                 \
  FUNC(kLastSeparableMode)             \
  FUNC(kLastMode)

TEST(DisplayListSkConversions, ToSkBlendMode) {
#define CHECK_TO_SKENUM(V) ASSERT_EQ(ToSk(DlBlendMode::V), SkBlendMode::V);
  FOR_EACH_BLEND_MODE_ENUM(CHECK_TO_SKENUM)
#undef CHECK_TO_SKENUM
}

TEST(DisplayListSkConversions, BlendColorFilterModifiesTransparency) {
  auto test_mode_color = [](DlBlendMode mode, DlColor color) {
    std::stringstream desc_str;
    desc_str << "blend[" << static_cast<int>(mode) << ", " << color << "]";
    std::string desc = desc_str.str();
    DlBlendColorFilter filter(color, mode);
    if (filter.modifies_transparent_black()) {
      auto dl_filter = DlBlendColorFilter::Make(color, mode);
      auto sk_filter = ToSk(filter);
      ASSERT_NE(dl_filter, nullptr) << desc;
      ASSERT_NE(sk_filter, nullptr) << desc;
      ASSERT_TRUE(sk_filter->filterColor(0) != 0) << desc;
    } else {
      auto dl_filter = DlBlendColorFilter::Make(color, mode);
      auto sk_filter = ToSk(filter);
      EXPECT_EQ(dl_filter == nullptr, sk_filter == nullptr) << desc;
      ASSERT_TRUE(sk_filter == nullptr || sk_filter->filterColor(0) == 0)
          << desc;
    }
  };

  auto test_mode = [&test_mode_color](DlBlendMode mode) {
    test_mode_color(mode, DlColor::kTransparent());
    test_mode_color(mode, DlColor::kWhite());
    test_mode_color(mode, DlColor::kWhite().modulateOpacity(0.5));
    test_mode_color(mode, DlColor::kBlack());
    test_mode_color(mode, DlColor::kBlack().modulateOpacity(0.5));
  };

#define TEST_MODE(V) test_mode(DlBlendMode::V);
  FOR_EACH_BLEND_MODE_ENUM(TEST_MODE)
#undef TEST_MODE
}

#undef FOR_EACH_BLEND_MODE_ENUM

TEST(DisplayListSkConversions, ConvertWithZeroAndNegativeVerticesAndIndices) {
  std::shared_ptr<const DlVertices> vertices1 = DlVertices::Make(
      DlVertexMode::kTriangles, 0, nullptr, nullptr, nullptr, 0, nullptr);
  EXPECT_NE(vertices1, nullptr);
  EXPECT_NE(ToSk(vertices1), nullptr);

  std::shared_ptr<const DlVertices> vertices2 = DlVertices::Make(
      DlVertexMode::kTriangles, -1, nullptr, nullptr, nullptr, -1, nullptr);
  EXPECT_NE(vertices2, nullptr);
  EXPECT_NE(ToSk(vertices2), nullptr);
}

TEST(DisplayListVertices, ConvertWithZeroAndNegativeVerticesAndIndices) {
  DlVertices::Builder builder1(DlVertexMode::kTriangles, 0,
                               DlVertices::Builder::kNone, 0);
  EXPECT_TRUE(builder1.is_valid());
  std::shared_ptr<DlVertices> vertices1 = builder1.build();
  EXPECT_NE(vertices1, nullptr);
  EXPECT_NE(ToSk(vertices1), nullptr);

  DlVertices::Builder builder2(DlVertexMode::kTriangles, -1,
                               DlVertices::Builder::kNone, -1);
  EXPECT_TRUE(builder2.is_valid());
  std::shared_ptr<DlVertices> vertices2 = builder2.build();
  EXPECT_NE(vertices2, nullptr);
  EXPECT_NE(ToSk(vertices2), nullptr);
}

TEST(DisplayListColorSource, ConvertRuntimeEffect) {
  const sk_sp<DlRuntimeEffect> kTestRuntimeEffect1 = DlRuntimeEffect::MakeSkia(
      SkRuntimeEffect::MakeForShader(
          SkString("vec4 main(vec2 p) { return vec4(0); }"))
          .effect);
  const sk_sp<DlRuntimeEffect> kTestRuntimeEffect2 = DlRuntimeEffect::MakeSkia(
      SkRuntimeEffect::MakeForShader(
          SkString("vec4 main(vec2 p) { return vec4(1); }"))
          .effect);
  std::shared_ptr<DlRuntimeEffectColorSource> source1 =
      DlColorSource::MakeRuntimeEffect(
          kTestRuntimeEffect1, {}, std::make_shared<std::vector<uint8_t>>());
  std::shared_ptr<DlRuntimeEffectColorSource> source2 =
      DlColorSource::MakeRuntimeEffect(
          kTestRuntimeEffect2, {}, std::make_shared<std::vector<uint8_t>>());
  std::shared_ptr<DlRuntimeEffectColorSource> source3 =
      DlColorSource::MakeRuntimeEffect(
          nullptr, {}, std::make_shared<std::vector<uint8_t>>());

  ASSERT_NE(ToSk(source1), nullptr);
  ASSERT_NE(ToSk(source2), nullptr);
  ASSERT_EQ(ToSk(source3), nullptr);
}

TEST(DisplayListColorSource, ConvertRuntimeEffectWithNullSampler) {
  const sk_sp<DlRuntimeEffect> kTestRuntimeEffect1 = DlRuntimeEffect::MakeSkia(
      SkRuntimeEffect::MakeForShader(
          SkString("vec4 main(vec2 p) { return vec4(0); }"))
          .effect);
  std::shared_ptr<DlRuntimeEffectColorSource> source1 =
      DlColorSource::MakeRuntimeEffect(
          kTestRuntimeEffect1, {nullptr},
          std::make_shared<std::vector<uint8_t>>());

  ASSERT_EQ(ToSk(source1), nullptr);
}

TEST(DisplayListSkConversions, MatrixColorFilterModifiesTransparency) {
  auto test_matrix = [](int element, SkScalar value) {
    // clang-format off
    float matrix[] = {
        1, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 1, 0,
    };
    // clang-format on
    std::string desc =
        "matrix[" + std::to_string(element) + "] = " + std::to_string(value);
    matrix[element] = value;
    DlMatrixColorFilter filter(matrix);
    auto dl_filter = DlMatrixColorFilter::Make(matrix);
    auto sk_filter = ToSk(filter);
    EXPECT_EQ(dl_filter == nullptr, sk_filter == nullptr);
    EXPECT_EQ(filter.modifies_transparent_black(),
              sk_filter && sk_filter->filterColor(0) != 0);
  };

  // Tests identity (matrix[0] already == 1 in an identity filter)
  test_matrix(0, 1);
  // test_matrix(19, 1);
  for (int i = 0; i < 20; i++) {
    test_matrix(i, -0.25);
    test_matrix(i, 0);
    test_matrix(i, 0.25);
    test_matrix(i, 1);
    test_matrix(i, 1.25);
    test_matrix(i, SK_ScalarNaN);
    test_matrix(i, SK_ScalarInfinity);
    test_matrix(i, -SK_ScalarInfinity);
  }
}

}  // namespace testing
}  // namespace flutter
