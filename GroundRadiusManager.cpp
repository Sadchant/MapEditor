#include "GroundRadiusManager.h"

CGroundRadiusManager::CGroundRadiusManager() :
border_1(g_pLoader->getTexture("T_1")),
border_3(g_pLoader->getTexture("T_3")),
border_5(g_pLoader->getTexture("T_5")),
border_7(g_pLoader->getTexture("T_7")),
border_9(g_pLoader->getTexture("T_9")),
border_11(g_pLoader->getTexture("T_11")),
border_13(g_pLoader->getTexture("T_13")),
border_15(g_pLoader->getTexture("T_15"))
{	
	fillVector();	
}

void CGroundRadiusManager::Init(SDL_Rect* camera, int tile_width, int tile_height, int numTilesX)
{
	this->camera = camera;
	this->tile_width = tile_width;
	this->tile_height = tile_height;
	this->numTilesX = numTilesX;
	aktRadius = 0;
	aktBorder = &border_1;
	GenerateRadiusData();
}

void CGroundRadiusManager::fillVector()
{
	int temp;
	string fileName = "Editor_Data/Radius.radius";
	ifstream radiusData(fileName);
	if (!radiusData.is_open())
	{
		printf("%s konnte nicht geoeffnet werden!", fileName);
		cout << " ";
		return;
	}
	while (!radiusData.eof())
	{
		// liest Zahlen aus datei in einen Vector ein
		radiusData >> temp;
		vecRadiusData.push_back(temp);
	}
	radiusData.close();
}
	

void CGroundRadiusManager::GenerateRadiusData()
{
	ClearAllVectors();
	int radius, row, column, number;
	vector<int>::iterator it_vecRadiusData = vecRadiusData.begin();
	while (it_vecRadiusData != vecRadiusData.end())
	{
		// liest Zahlen aus dem Vector in temp-Variablen ein
		radius = *it_vecRadiusData++;
		row = *it_vecRadiusData++;
		column = *it_vecRadiusData++;
		number = *it_vecRadiusData++;

		// sorgt dafür, dass in den richtigen Vector geschrieben wird
		SelectAktBorder(radius);
		// pusht werte in Vector
		AddRowtoVector(row, column, number);
	}
}


// schaut, ob der Iterator schon am Ende ist für den entsprechenden Radius
int CGroundRadiusManager::HasNextElement()
{
	return !(it_aktRadius == aktVector->end());
}


// wird aufgerufen, um Operationen im Map-Array auszuführen und gibt den nächsten Array-Index 
// zurück, wo etwas passieren soll
int CGroundRadiusManager::GetNextElement()
{
	if (it_aktRadius == aktVector->end())
	{
		cout << "SACH MA BENUTZT DU DIE KLASSE FALSCH?!" << endl;
		return -1;
	}
	tempZahl = *it_aktRadius;
	it_aktRadius++;
	return tempZahl;
}


// setzt den Zeiger auf den aktuellen Vector und resettet den Iterator
void CGroundRadiusManager::SelectAktBorder(int radius)
{
	aktRadius = radius;
	switch (radius)
	{
	case 1:
	{
			  aktVector = &radius_1;
			  aktBorder = &border_1;
	}break;
	case 3:
	{
			  aktVector = &radius_3;
			  aktBorder = &border_3;
	}break;
	case 5:
	{
			  aktVector = &radius_5;
			  aktBorder = &border_5;
	}break;
	case 7:
	{
			  aktVector = &radius_7;
			  aktBorder = &border_7;
	}break;
	case 9:
	{
			  aktVector = &radius_9;
			  aktBorder = &border_9;
	}break;
	case 11:
	{
			  aktVector = &radius_11;
			  aktBorder = &border_11;
	}break;
	case 13:
	{
			  aktVector = &radius_13;
			  aktBorder = &border_13;
	}break;
	case 15:
	{
			  aktVector = &radius_15;
			  aktBorder = &border_15;
	}break;
	}
	it_aktRadius = aktVector->begin();
}

void CGroundRadiusManager::AddRowtoVector(int row, int column, int number)
{
	for (int i = 0; i < number; i++)
	{
		tempZahl = row * numTilesX + column + i;
		aktVector->push_back(tempZahl);
	}
}

void CGroundRadiusManager::ClearAllVectors()
{
	radius_1.clear();
	radius_3.clear();
	radius_5.clear();
	radius_7.clear();
	radius_9.clear();
	radius_11.clear();
	radius_13.clear();
	radius_15.clear();
}

void CGroundRadiusManager::RenderBrushBorder(int mouseX, int mouseY)
{
	borderXPos = ((camera->x + mouseX) / tile_width)*tile_width - tile_width*(aktRadius / 2) - 1 - camera->x;
	borderYPos = ((camera->y + mouseY) / tile_height)*tile_height - tile_height*(aktRadius / 2) - 1 - camera->y;
	aktBorder->SetPos(borderXPos, borderYPos);
	aktBorder->Render();
}

	