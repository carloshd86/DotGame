#include "globals.h"
#include "game.h"
#include "applicationmanager.h"
#include "entity.h"
#include "gameinputmanager.h"
#include "messagereceiver.h"
#include "messages.h"
//#include "rapidjson/document.h"
//#include "rapidjson/error/en.h"
#include "asserts.h"
#include "memorycontrol.h"
#include <fstream>
#include "component.h"
//#include "componentenemy.h"
//#include "componentgoal.h"
//#include "componentmovable.h"
//#include "componentplayer.h"
//#include "componentrenderable.h"
//#include "componentrigidbody.h"
//#include "componentroutepath.h"


//using namespace rapidjson;


Game::Game() : 
	mInitialized     (false), 
	mEnded           (false), 
	m_pPlayer        (nullptr), 
	m_pGoal          (nullptr) {}

// *************************************************
//
// *************************************************

Game::~Game()
{
	if (mInitialized && !mEnded) End();
}

// *************************************************
//
// *************************************************

void Game::Init()
{
	if (!mInitialized)
	{
		// Init game state
		std::string jsonSrc;
		switch (mGameLevel)
		{
			case Level1: { jsonSrc = DATA_FOLDER + "level1.json"; break; }
			case Level2: { jsonSrc = DATA_FOLDER + "level2.json"; break; }
			case Level3: { jsonSrc = DATA_FOLDER + "level3.json"; break; }
		}

		GAME_ASSERT(jsonSrc.c_str());

		std::ifstream file(jsonSrc, std::ios::binary | std::ios::ate);
		GAME_ASSERT(file.is_open());

		std::vector<char> json(static_cast<uint16_t>(file.tellg()));
		file.seekg(0);
		file.read(json.data(), json.size());
		json.push_back('\0');

		//Document document;
		//ParseResult ok = document.Parse(json.data());
		//if (!ok) {
		//	OutputDebugStringA(GetParseError_En(ok.Code()));
		//	OutputDebugStringA(std::to_string(ok.Offset()).c_str());
		//	exit(EXIT_FAILURE);
		//}
		//GAME_ASSERT(document.IsObject());
	
		//// Level
		//GAME_ASSERT(document.HasMember(LEVEL_JSON_KEY));
		//GAME_ASSERT(document[LEVEL_JSON_KEY].IsObject());
		//GAME_ASSERT(document[LEVEL_JSON_KEY].HasMember(BACKGROUND_IMG_JSON_KEY));

		//const char * backgroundImg    = document[LEVEL_JSON_KEY][BACKGROUND_IMG_JSON_KEY].GetString();
		//std::string backgroundImgSrc = DATA_FOLDER;
		//backgroundImgSrc.append(backgroundImg);
		//float backgroundR = 1.f;
		//float backgroundG = 1.f;
		//float backgroundB = 1.f;
		//if (document[LEVEL_JSON_KEY].HasMember(BACKGROUND_R_JSON_KEY))
		//	backgroundR = document[LEVEL_JSON_KEY][BACKGROUND_R_JSON_KEY].GetFloat();
		//if (document[LEVEL_JSON_KEY].HasMember(BACKGROUND_G_JSON_KEY))
		//	backgroundG = document[LEVEL_JSON_KEY][BACKGROUND_G_JSON_KEY].GetFloat();
		//if (document[LEVEL_JSON_KEY].HasMember(BACKGROUND_B_JSON_KEY))
		//	backgroundB = document[LEVEL_JSON_KEY][BACKGROUND_B_JSON_KEY].GetFloat();
		//g_pWindowManager->SetBackgroundImage(backgroundImgSrc.c_str());
		//g_pWindowManager->SetBackgroundColor(backgroundR, backgroundG, backgroundB);

		//// Player
		//GAME_ASSERT(document.HasMember(PLAYER_JSON_KEY));
		//GAME_ASSERT(document[PLAYER_JSON_KEY].IsObject());
		//GAME_ASSERT(document[PLAYER_JSON_KEY].HasMember(POS_X_JSON_KEY));
		//GAME_ASSERT(document[PLAYER_JSON_KEY].HasMember(POS_Y_JSON_KEY));
		//GAME_ASSERT(document[PLAYER_JSON_KEY].HasMember(WIDTH_JSON_KEY));
		//GAME_ASSERT(document[PLAYER_JSON_KEY].HasMember(HEIGHT_JSON_KEY));
		//GAME_ASSERT(document[PLAYER_JSON_KEY].HasMember(SPEED_JSON_KEY));
		//GAME_ASSERT(document[PLAYER_JSON_KEY].HasMember(IMG_JSON_KEY));

		//float        playerPosX   = document[PLAYER_JSON_KEY][POS_X_JSON_KEY].GetFloat();
		//float        playerPosY   = document[PLAYER_JSON_KEY][POS_Y_JSON_KEY].GetFloat();
		//float        playerWidth  = document[PLAYER_JSON_KEY][WIDTH_JSON_KEY].GetFloat();
		//float        playerHeight = document[PLAYER_JSON_KEY][HEIGHT_JSON_KEY].GetFloat();
		//float        playerSpeed  = document[PLAYER_JSON_KEY][SPEED_JSON_KEY].GetFloat();
		//const char * playerImg    = document[PLAYER_JSON_KEY][IMG_JSON_KEY].GetString();
		//std::string  playerImgSrc = DATA_FOLDER;
		//playerImgSrc.append(playerImg);

		//m_pPlayer = GAME_NEW(Entity, ());
		//C_Player * cPlayer                    = GAME_NEW(C_Player, (m_pPlayer));
		//cPlayer->Init();
		//C_Rigidbody * cPlayerRigidbody        = GAME_NEW(C_Rigidbody, (m_pPlayer, vmake(playerPosX, playerPosY), vmake(playerWidth, playerHeight)));
		//cPlayerRigidbody->Init();
		//C_Renderable * cPlayerRenderable      = GAME_NEW(C_Renderable, (m_pPlayer, vmake(playerPosX, playerPosY), vmake(playerWidth, playerHeight), playerImgSrc.c_str()));
		//cPlayerRenderable->Init();
		//C_Movable      * cPlayerMovable       = GAME_NEW(C_Movable, (m_pPlayer, playerSpeed));
		//cPlayerMovable->Init();
		//m_pPlayer->AddComponent(cPlayer);
		//m_pPlayer->AddComponent(cPlayerRigidbody);
		//m_pPlayer->AddComponent(cPlayerRenderable);
		//m_pPlayer->AddComponent(cPlayerMovable);
		//mEntities.push_back(m_pPlayer);

		//// Goal
		//GAME_ASSERT(document.HasMember(GOAL_JSON_KEY));
		//GAME_ASSERT(document[GOAL_JSON_KEY].IsObject());
		//GAME_ASSERT(document[GOAL_JSON_KEY].HasMember(POS_X_JSON_KEY));
		//GAME_ASSERT(document[GOAL_JSON_KEY].HasMember(POS_Y_JSON_KEY));
		//GAME_ASSERT(document[GOAL_JSON_KEY].HasMember(WIDTH_JSON_KEY));
		//GAME_ASSERT(document[GOAL_JSON_KEY].HasMember(HEIGHT_JSON_KEY));
		//GAME_ASSERT(document[GOAL_JSON_KEY].HasMember(IMG_JSON_KEY));

		//float        goalPosX   = document[GOAL_JSON_KEY][POS_X_JSON_KEY].GetFloat();
		//float        goalPosY   = document[GOAL_JSON_KEY][POS_Y_JSON_KEY].GetFloat();
		//float        goalWidth  = document[GOAL_JSON_KEY][WIDTH_JSON_KEY].GetFloat();
		//float        goalHeight = document[GOAL_JSON_KEY][HEIGHT_JSON_KEY].GetFloat();
		//const char * goalImg    = document[GOAL_JSON_KEY][IMG_JSON_KEY].GetString();
		//std::string  goalImgSrc = DATA_FOLDER;
		//goalImgSrc.append(goalImg);

		//m_pGoal = GAME_NEW(Entity, ());
		//C_Goal * cGoal                 = GAME_NEW(C_Goal, (m_pGoal));
		//cGoal->Init();
		//C_Rigidbody * cGoalRigidbody   = GAME_NEW(C_Rigidbody, (m_pGoal, vmake(goalPosX, goalPosY),vmake(goalWidth, goalHeight)));
		//cGoalRigidbody->Init();
		//C_Renderable * cGoalRenderable = GAME_NEW(C_Renderable, (m_pGoal, vmake(goalPosX, goalPosY), vmake(goalWidth, goalHeight), goalImgSrc.c_str()));
		//cGoalRenderable->Init();
		//m_pGoal->AddComponent(cGoal);
		//m_pGoal->AddComponent(cGoalRigidbody);
		//m_pGoal->AddComponent(cGoalRenderable);
		//mEntities.push_back(m_pGoal);

		//// Enemies
		//if (document.HasMember(ENEMIES_JSON_KEY))
		//	for (auto& item : document[ENEMIES_JSON_KEY].GetArray())
		//	{
		//		auto& jsonEnemy = item.GetObjectA();

		//		GAME_ASSERT(jsonEnemy.HasMember(POS_X_JSON_KEY));
		//		GAME_ASSERT(jsonEnemy.HasMember(POS_Y_JSON_KEY));
		//		GAME_ASSERT(jsonEnemy.HasMember(WIDTH_JSON_KEY));
		//		GAME_ASSERT(jsonEnemy.HasMember(HEIGHT_JSON_KEY));
		//		GAME_ASSERT(jsonEnemy.HasMember(SPEED_JSON_KEY));
		//		GAME_ASSERT(jsonEnemy.HasMember(PURSUING_SPEED_JSON_KEY));
		//		GAME_ASSERT(jsonEnemy.HasMember(IMG_LEFT_JSON_KEY));
		//		GAME_ASSERT(jsonEnemy.HasMember(IMG_RIGHT_JSON_KEY));
		//		GAME_ASSERT(jsonEnemy.HasMember(IMG_ALERT_LEFT_JSON_KEY));
		//		GAME_ASSERT(jsonEnemy.HasMember(IMG_ALERT_RIGHT_JSON_KEY));

		//		float        enemyPosX          = jsonEnemy[POS_X_JSON_KEY].GetFloat();
		//		float        enemyPosY          = jsonEnemy[POS_Y_JSON_KEY].GetFloat();
		//		float        enemyWidth         = jsonEnemy[WIDTH_JSON_KEY].GetFloat();
		//		float        enemyHeight        = jsonEnemy[HEIGHT_JSON_KEY].GetFloat();
		//		float        enemySpeed         = jsonEnemy[SPEED_JSON_KEY].GetFloat();
		//		float        enemyPursuingSpeed = jsonEnemy[PURSUING_SPEED_JSON_KEY].GetFloat();
		//		const char * enemyImgLeft       = jsonEnemy[IMG_LEFT_JSON_KEY].GetString();
		//		const char * enemyImgRight      = jsonEnemy[IMG_RIGHT_JSON_KEY].GetString();
		//		const char * enemyImgAlertLeft  = jsonEnemy[IMG_ALERT_LEFT_JSON_KEY].GetString();
		//		const char * enemyImgAlertRight = jsonEnemy[IMG_ALERT_RIGHT_JSON_KEY].GetString();
		//		std::string  enemyImgSrc        = DATA_FOLDER;

		//		Entity * enemy = GAME_NEW(Entity, ());
		//		C_Enemy * cEnemy                = GAME_NEW(C_Enemy, (enemy, enemyPursuingSpeed, (enemyImgSrc + enemyImgLeft).c_str(), (enemyImgSrc + enemyImgRight).c_str(), (enemyImgSrc + enemyImgAlertLeft).c_str(), (enemyImgSrc + enemyImgAlertRight).c_str()));
		//		cEnemy->Init();
		//		C_Rigidbody * cEnemyRigidbody   = GAME_NEW(C_Rigidbody, (enemy, vmake(enemyPosX, enemyPosY),vmake(enemyWidth, enemyHeight)));
		//		cEnemyRigidbody->Init();
		//		C_Renderable * cEnemyRenderable = GAME_NEW(C_Renderable, (enemy, vmake(enemyPosX, enemyPosY), vmake(enemyWidth, enemyHeight), (enemyImgSrc + enemyImgLeft).c_str()));
		//		cEnemyRenderable->Init();
		//		C_Movable * cEnemyMovable       = GAME_NEW(C_Movable, (enemy, enemySpeed));
		//		cEnemyMovable->Init();
		//		enemy->AddComponent(cEnemy);
		//		enemy->AddComponent(cEnemyRigidbody);
		//		enemy->AddComponent(cEnemyRenderable);
		//		enemy->AddComponent(cEnemyMovable);
	
		//		if (jsonEnemy.HasMember(ROUTE_POINTS_JSON_KEY))
		//		{
		//			C_RoutePath * cRoutePath = GAME_NEW(C_RoutePath, (enemy));
		//			cRoutePath->Init();
		//			for (auto& point : jsonEnemy[ROUTE_POINTS_JSON_KEY].GetArray())
		//			{
		//				auto& routePoint = point.GetObjectA();

		//				GAME_ASSERT(routePoint.HasMember(POS_X_JSON_KEY));
		//				GAME_ASSERT(routePoint.HasMember(POS_Y_JSON_KEY));

		//				float routePointX = routePoint[POS_X_JSON_KEY].GetFloat();
		//				float routePointY = routePoint[POS_Y_JSON_KEY].GetFloat();

		//				cRoutePath->AddRoutePoint(routePointX, routePointY);
		//			}
		//			enemy->AddComponent(cRoutePath);
		//		}

		//		mEntities.push_back(enemy);
		//		mEnemies.push_back(enemy);
		//	}

		//// Walls
		//if (document.HasMember(WALLS_JSON_KEY))
		//{
		//	for (auto& item : document[WALLS_JSON_KEY].GetArray())
		//	{
		//		auto& jsonWall = item.GetObjectA();

		//		GAME_ASSERT(jsonWall.HasMember(POS_X_JSON_KEY));
		//		GAME_ASSERT(jsonWall.HasMember(POS_Y_JSON_KEY));
		//		GAME_ASSERT(jsonWall.HasMember(WIDTH_JSON_KEY));
		//		GAME_ASSERT(jsonWall.HasMember(HEIGHT_JSON_KEY));
		//		GAME_ASSERT(jsonWall.HasMember(IMG_JSON_KEY));

		//		float        wallPosX = jsonWall[POS_X_JSON_KEY].GetFloat();
		//		float        wallPosY = jsonWall[POS_Y_JSON_KEY].GetFloat();
		//		float        wallWidth = jsonWall[WIDTH_JSON_KEY].GetFloat();
		//		float        wallHeight = jsonWall[HEIGHT_JSON_KEY].GetFloat();
		//		const char * wallImg = jsonWall[IMG_JSON_KEY].GetString();
		//		std::string  wallImgSrc = DATA_FOLDER;
		//		wallImgSrc.append(wallImg);

		//		Entity * wall = GAME_NEW(Entity, ());
		//		C_Rigidbody * cWall = GAME_NEW(C_Rigidbody, (wall, vmake(wallPosX, wallPosY),vmake(wallWidth, wallHeight)));
		//		cWall->Init();
		//		C_Renderable * cWallRenderable = GAME_NEW(C_Renderable, (wall, vmake(wallPosX, wallPosY), vmake(wallWidth, wallHeight), wallImgSrc.c_str()));
		//		cWallRenderable->Init();
		//		wall->AddComponent(cWall);
		//		wall->AddComponent(cWallRenderable);

		//		mEntities.push_back(wall);
		//		mWalls.push_back(wall);
		//	}
		//}

		//// Scenery limits (invisible walls)
		//Vec2 worldLimits = g_pWindowManager->GetWorldSize();
		//Entity * limitDown = GAME_NEW(Entity, ());
		//C_Rigidbody * cRigidbodyDown = GAME_NEW(C_Rigidbody, (limitDown, vmake(-1.f, -2.f),vmake(worldLimits.x, 1.f)));
		//cRigidbodyDown->Init();
		//limitDown->AddComponent(cRigidbodyDown);
		//mWalls.push_back(limitDown);

		//Entity * limitUp = GAME_NEW(Entity, ());
		//C_Rigidbody * cRigidbodyUp = GAME_NEW(C_Rigidbody, (limitUp, vmake(-1.f, worldLimits.y + 1),vmake(worldLimits.x, 1.f)));
		//cRigidbodyUp->Init();
		//limitUp->AddComponent(cRigidbodyUp);
		//mWalls.push_back(limitUp);

		//Entity * limitLeft = GAME_NEW(Entity, ());
		//C_Rigidbody * cRigidbodyLeft = GAME_NEW(C_Rigidbody, (limitLeft, vmake(-2.f, -1.f),vmake(1.f, worldLimits.y)));
		//cRigidbodyLeft->Init();
		//limitLeft->AddComponent(cRigidbodyLeft);
		//mWalls.push_back(limitLeft);

		//Entity * limitRight = GAME_NEW(Entity, ());
		//C_Rigidbody * cRigidbodyRight = GAME_NEW(C_Rigidbody, (limitRight, vmake(worldLimits.x + 1, -1.f),vmake(1.f, worldLimits.y)));
		//cRigidbodyRight->Init();
		//limitRight->AddComponent(cRigidbodyRight);
		//mWalls.push_back(limitRight);

		mInitialized = true;
	}
}

// *************************************************
//
// *************************************************

void Game::End()
{
	if (mInitialized)
	{
		for(auto entity : mEntities) { GAME_DELETE(entity); }
		mEntities.clear();
		
		m_pPlayer = nullptr;
		m_pGoal   = nullptr;

		mWalls.clear();
		mEnemies.clear();

		mInitialized = false;
	}
	mEnded = true;
}

// *************************************************
//
// *************************************************

void Game::Run(float deltaTime)
{
	for (auto entity : mEntities) entity->Run(deltaTime);
}

// *************************************************
//
// *************************************************

Entity * Game::GetPlayer() const
{
	return m_pPlayer;
}

// *************************************************
//
// *************************************************

Game::GameLevel Game::GetGameLevel() const 
{
	return mGameLevel;
}

// *************************************************
//
// *************************************************

void Game::SetGameLevel(GameLevel level)
{
	mGameLevel = level;
}

// *************************************************
//
// *************************************************

Game::GameLevel Game::GetNextLevel(GameLevel level)
{
	switch (level)
	{
		case Game::Level1: return Level2;
		case Game::Level2: return Level3;
		case Game::Level3: return Level1;
		default:           return Level1;
	}
}