#ifndef _SPRITEANIMTION_H_
#define _SPRITEANIMTION_H_

#include"SpRendering\Texture.h"
#include"Animation.h"
#include"DynamicFactory.h"
#include"Component.h"
#include"SpriteRenderer.h"

using namespace spgameengine;

class ENGINE_API CSpriteAnimation : public CComponent, public IAnimation
{
	REFLECT_CLASS(CSpriteAnimation)
private:
	CSpriteRenderer* renderer;

	//ÿһ֡����ͼ
	vector<PTexture> frames;
	int frameCount;

	//�����б���±�
	int* indexList;

	//�ܳ������±�����ĳ��ȣ�������֡����
	int length;
	float rate;
	float fps;
	float timer;
	int index;
	float speed;

	bool isPlay;

public:
	CSpriteAnimation();
	~CSpriteAnimation();

	void Initialize(vector<PTexture> frames, int frameCount, int* indexList, int length, float rate);

	//IAnimation---------------------------------
	virtual void SetSpeed(float speed) override;

	virtual float GetSpeed() override;

	virtual void Play() override;

	virtual void Pause() override;

	virtual void Stop() override;

	virtual void ResetToBegin(bool play) override;
	//------------------------------------------

	//CComponent--------------------------------
	virtual void OnStart() override;

	virtual void OnUpdate() override;

	virtual void OnDestroy() override;
	//------------------------------------------
};

#endif