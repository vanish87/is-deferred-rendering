#include "Cannon.h"
using namespace MocapGE;

Cannon::Cannon(D3DModel* model, D3DModel* bullet)
	:model_(model), bullet_(bullet), fired(false), hit_(false), bullet_speed_(20), hp_(100), dir_(float3(0,0,1)), pos_(float3(0,0,0))
{
}


Cannon::~Cannon(void)
{
}

void Cannon::Update(float4x4 parent )
{
	float4x4 cannon_mat;
	Math::Identity(cannon_mat);
	float theta = Math::ArcTan(Math::Sqrt(dir_.x()*dir_.x()+ dir_.y()*dir_.y())/ dir_.z());
	float phi = Math::ArcTan(dir_.y() / dir_.x());
	//Math::YRotation(cannon_mat, theta);
	//Math::XRotation(cannon_mat, phi);
	float4x4 inv_parent = Math::Inverse(parent);
	//pos_ = Math::Transform(pos_, inv_parent);
	Math::Translate(cannon_mat, pos_.x(), pos_.y(), pos_.z());

	cannon_mat = cannon_mat * parent;
	model_->SetModelMatrix(cannon_mat);
	bullet_->SetModelMatrix(cannon_mat);
}

float3 Cannon::GetPos()
{
	return pos_;
}

void Cannon::SetPos(float3 pos )
{
	pos_ = pos;
}
