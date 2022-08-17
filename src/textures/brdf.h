
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
      : wi(wi) {
        FourierBSDFTable::Read(bsdffile, bsdfTable);
        bsdf = new FourierBSDF(*bsdfTable, TransportMode::Radiance);
      }

    Spectrum Evaluate(const SurfaceInteraction &si) const {
      Vector3f norm(si.n.x, si.n.y, si.n.z);
      return bsdf->f(norm, wi);
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
