#include "ResembleState.h"

#include "Ship.h"
#include "Picking.h"
using namespace MocapGE;

ResembleState::ResembleState(Ship* ship, PartList parts)
	:ship_(ship), parts_(parts)
{
	ship_dir_ = ship_->GetDir();
	ship_pos_ = ship_->GetPos();
	ship_up_  = ship_->GetUp();

	first_flag_ = true;
	left_ctr_down_ = false;

	cam_speed_deg_ = Math::PI /180;
	mouse_down_ = false;
	picked_ = false;

	picking_ = new Picking();
}


ResembleState::~ResembleState(void)
{
}

void ResembleState::Update()
{

	//std::cout<<"dir= "<<ship_dir.x()<<" "<<ship_dir.y()<<" "<<ship_dir.z()<<" "<<std::endl;
	float3 cam_tag_pos = ship_pos_ - ship_dir_ + ship_up_ * 3;
	Camera* camera = Context::Instance().AppInstance().GetCamera();
	float3 cam_pos = camera->GetPos();
	float3 cam_up = camera->GetUp();

	float3 dis = cam_tag_pos - cam_pos;
	if(first_flag_ && Math::Dot(dis, dis) > 0.001 )
	{
		dis = Math::Normalize(dis);
		camera->SetView(cam_pos + dis* 0.01, ship_pos_, cam_up);
	}
	else
	{
		first_flag_ = false;
	}
	//PRINT("loading");
}

void ResembleState::OnKeyDown( WPARAM key_para )
{
	switch (key_para) 
	{ 
	case VK_CONTROL:
		{
			if(GetKeyState(VK_LCONTROL) & 0x8000)
			{
				//std::cout<<"L Control"<<std::endl;
				left_ctr_down_ = true;
				Context::Instance().AppInstance().LockCursor(true);
			}
			break;
		}
	}
}

void ResembleState::OnKeyUp( WPARAM key_para )
{
	switch (key_para) 
	{ 
	case VK_CONTROL:
		{
			if(!(GetKeyState(VK_LCONTROL) & 0x8000))
			{
				left_ctr_down_ = false;
				Context::Instance().AppInstance().LockCursor(false);
			}
			break;
		}
	}
}

void ResembleState::OnMouseDown( WPARAM mouse_para, int x, int y )
{
	float2 screen_pos(x, y);
	float3 picked_pos;
	Viewport* viewport = Context::Instance().GetRenderFactory().GetRenderEngine().CurrentFrameBuffer()->GetViewport();
	mouse_down_= true;
	for(size_t i = 0; i < parts_.size(); i++)
	{
		picked_ = picking_->GetIntersection(parts_[i], viewport, screen_pos, picked_pos);
		if(picked_)
		{
			picked_model_ = parts_[i];
			break;
		}
	}
}

void ResembleState::OnMouseUp( WPARAM mouse_para, int x, int y )
{
	mouse_down_= false;
	picked_ = false;
}

void ResembleState::OnMouseMove( WPARAM mouse_para, int x, int y )
{
	float2 screen_pos(x, y);
	if(left_ctr_down_)
	{
		//std::cout<<screen_pos.x()<<"  ss "<<screen_pos.y()<<std::endl;
		//std::cout<<pre_pos.x()<<" "<<pre_pos.y()<<std::endl;
		float2 delta = screen_pos - pre_pos;
		std::cout<<delta.x()<<" "<<delta.y()<<std::endl;
		Camera* camera = Context::Instance().AppInstance().GetCamera();
		float3 cam_pos = camera->GetPos();
		float3 at = camera->GetLookAt();
		float3 up = camera->GetUp();

		float3 dir =  at - cam_pos;
		float3 inv_dir =  cam_pos - at;
		float3 new_dir;

		float4x4 rot_mat;
		Math::Identity(rot_mat);
		if(delta.x() > 1)
			Math::YRotation(rot_mat, -cam_speed_deg_);
		else
			if(delta.x() < 1)
				Math::YRotation(rot_mat, cam_speed_deg_);

		new_dir = Math::Transform(inv_dir, rot_mat);

		float3 left = Math::Cross(dir,up);
		left = Math::Normalize(left);
		if(delta.y() > 1 )
			Math::RotationAxis(rot_mat, left, -cam_speed_deg_);
		else
			if(delta.y() < 1 )
				Math::RotationAxis(rot_mat, left, cam_speed_deg_);
		new_dir = Math::Transform(new_dir, rot_mat);

		float3 trans_vec = new_dir - inv_dir;

		cam_pos = cam_pos + trans_vec;

		if(delta.x()!=0 && delta.y()!=0)
			camera->SetView(cam_pos, at, up);
		//camera->Yaw(Math::PI * delta.x() /180);
		//camera->Pitch(Math::PI * delta.y() /180);
	}
	else
	{
		if(mouse_down_)
		{
			//std::cout<<screen_pos.x()<<" "<<screen_pos.y()<<std::endl;
			//std::cout<<pre_pos.x()<<" "<<pre_pos.y()<<std::endl;
			float3 picked_pos;
			if(picked_)
			{
				float4x4 model_matrix = picked_model_->GetModelMatrix();
				float2 delta = screen_pos - pre_pos;
				std::cout<<delta.x()<<"d "<<delta.y()<<std::endl;
				Camera* camera = Context::Instance().AppInstance().GetCamera();
				float3 pos = camera->GetPos();
				float3 at = camera->GetLookAt();
				float3 dir =  at - pos;
				float3 up = camera->GetUp();
				float3 left = Math::Cross(dir,up);
				left = Math::Normalize(left);
				float3 right = float3(-left.x(), -left.y(), -left.z());
				float3 real_up = Math::Cross(dir, right);
				real_up = Math::Normalize(real_up);
				float3 down = float3(-real_up.x(), -real_up.y(), -real_up.z());
				if(delta.x() > 0)
					ship_pos = ship_pos + right / 50;
				else
					if(delta.x() < 0)
						ship_pos = ship_pos + left / 50;
				if(delta.y() > 0 )
					ship_pos = ship_pos + down/ 50;
				else
					if(delta.y() < 0 )
						ship_pos = ship_pos + real_up/ 50;

				ship_pos = ship_pos + float3(delta.x()/320, delta.y()/200, 0) ;
				std::cout<<ship_pos.x()<<" "<<ship_pos.y()<<std::endl;
				Math::Translate(model_matrix, ship_pos.x(), ship_pos.y(), ship_pos.z());
				picked_model_->SetModelMatrix(model_matrix);
			}

		}
	}
	
	//pre_pos = float2(x,y);
	int2 center = Context::Instance().AppInstance().GetWindow().GetCenter();
	pre_pos = float2(632, 369);
}
