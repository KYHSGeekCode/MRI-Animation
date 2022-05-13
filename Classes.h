//몇 초부터 몇 초까지 이 행동을해라
//Object.ChangeUpdater(AnimationUpdater *au);
//void update(DWORD dt){animUpdater->Update(dt);HDC hdc;//CS_OWNDC
typedef float degree;
#define NUMBIT B_END-101
#include <windows.h>
class AnimUpdate
{
	DWORD createTime;
	DWORD life;
	float dx,dy;
	degree deg,ddeg;
	XFORM trans;
	public:
	AnimUpdate();
	AnimUpdate(float dx,float dy,degree _ddeg);
	AnimUpdate(int x, int y, degree deg, float dx,float dy,degree _ddeg);
	//이번 소스에서는충돌위험은없다
	void Update(DWORD dt);
	void Apply();
};

class AnimObject
{
	HBITMAP img;
	public:
		AnimUpdate *updater;
		AnimObject(HBITMAP img, AnimUpdate *a);
		void Update(DWORD dt);
		virtual void Draw();
};

void MakeObjects();
void UpdateObjects(DWORD timeelapsed);

extern vector<AnimObject> objects;
extern HBITMAP bitmaps[NUMBIT];

//extern DWORD starttick;
//extern DWORD nowTick;
