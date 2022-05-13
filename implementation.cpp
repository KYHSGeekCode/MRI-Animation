#include "resource.h"
#include <vector>
using namespace std;
#include <windows.h>
#include "Classes.h"

//AnimUpdate(x,y,deg, dx,dy,delta deg);
AnimUpdate alight(100,100,0,20,0,0);					//초기 위치는 100,100 각도 0 초당 20픽셀 오른쪽으로 
AnimUpdate aMagnetA(200,100,0.0f,0,0,0.1f);				//초기 위치는 200,100 각도 0 초당 0픽셀 초당 0.1씩 시계방향 회전 


//IDB_BITLIGHT,IDB_BITMAGNET은 resource.h와 Animation.rc 참조 
void MakeObjects()
{
	objects.push_back(AnimObject(bitmaps[IDB_BITLIGHT],&alight));	//alight행동하는  빛 생성 
	objects.push_back(AnimObject(bitmaps[IDB_BITMAGNET],&aMagnetA));//aMagnetA행동하는 자석 그림 생성 
}

void UpdateObjects(DWORD timeelapsed)
{
	float asfloat=(float)timeelapsed/1000.0f;
	if(asfloat>10.0f)								//실행 후 10초가 지나면  
	{							
		objects[1].updater=&alight;					//위에서 두 번재 추가한 오브젝트가 빛처럼 움직인다. 
	}
	/*
	else if(asfloat>15.0f)							//실행 후 15초가 지나면 
	{
													//어쩌구 저쩌구ㄴ 
	}
	*/
}

