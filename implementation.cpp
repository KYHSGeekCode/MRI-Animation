#include "resource.h"
#include <vector>
using namespace std;
#include <windows.h>
#include "Classes.h"

//AnimUpdate(x,y,deg, dx,dy,delta deg);
AnimUpdate alight(100,100,0,20,0,0);					//�ʱ� ��ġ�� 100,100 ���� 0 �ʴ� 20�ȼ� ���������� 
AnimUpdate aMagnetA(200,100,0.0f,0,0,0.1f);				//�ʱ� ��ġ�� 200,100 ���� 0 �ʴ� 0�ȼ� �ʴ� 0.1�� �ð���� ȸ�� 


//IDB_BITLIGHT,IDB_BITMAGNET�� resource.h�� Animation.rc ���� 
void MakeObjects()
{
	objects.push_back(AnimObject(bitmaps[IDB_BITLIGHT],&alight));	//alight�ൿ�ϴ�  �� ���� 
	objects.push_back(AnimObject(bitmaps[IDB_BITMAGNET],&aMagnetA));//aMagnetA�ൿ�ϴ� �ڼ� �׸� ���� 
}

void UpdateObjects(DWORD timeelapsed)
{
	float asfloat=(float)timeelapsed/1000.0f;
	if(asfloat>10.0f)								//���� �� 10�ʰ� ������  
	{							
		objects[1].updater=&alight;					//������ �� ���� �߰��� ������Ʈ�� ��ó�� �����δ�. 
	}
	/*
	else if(asfloat>15.0f)							//���� �� 15�ʰ� ������ 
	{
													//��¼�� ��¼���� 
	}
	*/
}

