#pragma once
#include <Engine\CComponent.h>

void CreateStageLevel();
void CreateTitleLevel();
CGameObject* LoadTilemap(wstring _strRelativePath);
CGameObject* LoadGameObject(FILE* _File);
CComponent* LoadAnimator2D(wstring _strRelativePath);
