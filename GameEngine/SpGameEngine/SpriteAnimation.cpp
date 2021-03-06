#include"SpriteAnimation.h"
#include"SpCommon\EngineDefine.h"
#include"Application.h"
#include"Time.h"
#include"GameObject.h"

IMPL_CLASS(CSpriteAnimation)

CSpriteAnimation::CSpriteAnimation() : speed(1), timer(0)
{
}


CSpriteAnimation::~CSpriteAnimation()
{
}


void CSpriteAnimation::Initialize(vector<PTexture> frames, int frameCount, int* indexList, int length, float rate)
{
	this->isPlay = true;
	this->frames = frames;
	this->frameCount = frameCount;
	this->indexList = indexList;
	this->length = length;
	this->rate = rate;
	this->fps = 1 / rate;
	this->renderer->SetTexture(frames[indexList[0]]);
}

//IAnimation---------------------------------

void CSpriteAnimation::SetSpeed(float speed)
{
	this->speed = speed;
}

float CSpriteAnimation::GetSpeed()
{
	return this->speed;
}

void CSpriteAnimation::Play()
{
	isPlay = true;
}

void CSpriteAnimation::Pause()
{
	isPlay = false;
}

void CSpriteAnimation::Stop()
{
	index = 0;
	isPlay = false;
	timer = 0;
}

void CSpriteAnimation::ResetToBegin(bool play)
{
	index = 0;
	timer = 0;
	isPlay = play;
}

//------------------------------------------

//CComponent--------------------------------

void CSpriteAnimation::OnStart()
{
	renderer = gameObject->GetComponent<CSpriteRenderer>();
	if (!renderer)
		renderer = gameObject->AddComponent<CSpriteRenderer>();
}

void CSpriteAnimation::OnUpdate()
{
	if (!isPlay || !renderer) return;

	timer += CTime::deltaTime * speed;
	if (timer >= fps)
	{
		timer -= fps;
		renderer->SetTexture(frames[indexList[index]]);
		index++;
		if (index >= length)
			index = 0;
	}
}

void CSpriteAnimation::OnDestroy()
{

}

//------------------------------------------
