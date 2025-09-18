#include "Enemy.h"
#include "../Object.h"
#include "../Status/StatusData.h"
#include "../Status/StatusEnemy.h"

Enemy::Enemy(Enemy::TYPE type)
	:status_(StatusData::GetInstance().GetEnemyStatus(static_cast<int>(type)))
{
	
}

void Enemy::Load(void)
{
}
void Enemy::LoadStatus(void)
{
	StatusEnemy& status = StatusData::GetInstance().GetEnemyStatus(0);


}

void Enemy::SetParam(void)
{

}

void Enemy::Init(const VECTOR& pos, float angleY)
{
}

void Enemy::Update(void)
{
}

void Enemy::Draw(void)
{
}


