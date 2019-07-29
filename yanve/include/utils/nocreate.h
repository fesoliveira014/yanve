#pragma once

/**
 * NoCreateT serves as a placeholder for constructors of OpenGL objects that
 * should have no initialization. This means that their initial state is 
 * the same as after std::move.
 * Concept adapted from: https://github.com/mosra/magnum/blob/master/src/Magnum/Tags.h
 */

namespace yanve
{

struct NoCreateT
{
  struct Init{};
  constexpr explicit NoCreateT(Init) {}
};

constexpr NoCreateT NoCreate{ NoCreateT::Init{} };

}