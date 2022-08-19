
/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_TEXTURES_BRDF_H
#define PBRT_TEXTURES_BRDF_H

// textures/brdf.h*
#include "pbrt.h"
#include "reflection.h"
#include "interaction.h"
#include "texture.h"
#include "paramset.h"
#include "material.h"

namespace pbrt {

// ConstantTexture Declarations
class BRDFTexture: public Texture<Spectrum> {
  public:
    // ConstantTexture Public Methods
    BRDFTexture(const Vector3f &wi, const std::string bsdffile)
      : wi(Normalize(wi)) {
        bsdfTable = new FourierBSDFTable();
        FourierBSDFTable::Read(bsdffile, bsdfTable);
        bsdf = new FourierBSDF(*bsdfTable, TransportMode::Radiance);
      }

    Spectrum Evaluate(const SurfaceInteraction &si) const {
      Vector3f norm(si.n.x, si.n.y, si.n.z);
      auto woLocal = si.bsdf->WorldToLocal(si.wo);
      auto wiLocal = si.bsdf->WorldToLocal(wi);
      Spectrum res = bsdf->f(woLocal, wiLocal);
      float rgb[3];
      res.ToRGB(rgb);
      float len = Vector3f(rgb[0], rgb[1], rgb[2]).Length();
      float zeroRGB[3] = {0., 0., 1.};
      float oneRGB[3] = {1., 0., 0.};
      Spectrum zeroColor = Spectrum::FromRGB(zeroRGB);
      Spectrum oneColor = Spectrum::FromRGB(oneRGB);
      return Lerp(len / std::sqrt(3), zeroColor, oneColor);
    }

  private:
    Vector3f wi;
    FourierBSDFTable *bsdfTable;
    FourierBSDF *bsdf;
};

BRDFTexture *CreateBRDFTexture(
    const Transform &tex2world, const TextureParams &tp);

}  // namespace pbrt

#endif  // PBRT_TEXTURES_CONSTANT_H
