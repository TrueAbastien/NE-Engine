#pragma once
#include "Vector.h"
#include "Physical.h"
#include "Playable.h"

class Player : public Physical, public Playable {
public:
  Player();
  virtual ~Player() override {}

  virtual void Reset() override;
  virtual void Update() override;
  virtual void OnCollide(Object& other, const Vector3& push) override;

  virtual void Move(float moveF, float moveL) override;

  virtual Matrix4 WorldToCam() const override;
  virtual Matrix4 CamToWorld() const override;
  virtual Vector3 CamOffset() const override;

  virtual int Cast() const override { return 1; }

private:

  float bob_mag;
  float bob_phi;

  bool onGround;
};
