#include "MarioManager.h"
#include "GameManager.h"

MarioManager::MarioManager() {
	mGraphics = Graphics::Instance();
	mCamera = Camera::Instance();
	mKoopaRespawnTimer = 0;
	mKoopaRespawnTimeMillis = 10000;
	mScreenshaking = false;
	mScreenshakeTimer = 0;
	mScreenshakeTimeMillis = 1000;
	mWobble = 0.0f;

	mBackground = new Texture("Secret/BackgroundMB.png");
	mCamera->SetCentre(Vector2((mGraphics->mScreenWidth - mBackground->GetWidth()) * 0.5f, (mGraphics->mScreenHeight - 100.0f) * 0.5f));

	mLevel = new Level("Textures/Tileset.png");
	mLevel->LoadLevel("Secret");

	mPlayers[0] = new Player(new AnimatedTexture("Secret/Mario.png", 0, 0, 32, 48, 1, 1, AnimatedTexture::AnimDir::HORIZONTAL), mLevel, 1);
	mPlayers[1] = new Player(new AnimatedTexture("Secret/Luigi.png", 0, 0, 32, 48, 1, 1, AnimatedTexture::AnimDir::HORIZONTAL), mLevel, 2);
	for (int i = 0; i < 2; i++) {
		mPlayers[i]->ToggleAnimation();
		mPlayers[i]->SetPos(mLevel->GetStartPos());
		mPlayers[i]->SetParent(mLevel);
	}

	for (int i = 0; i < 2; i++) {
		mKoopas[i] = new Koopa(mLevel);
		mKoopas[i]->SetPos(Vector2(i * ((mLevel->GetWidth() * TILESIZE) - 32), TILESIZE));
		mKoopas[i]->SetParent(mLevel);
	}

	mPowBlock = new POWBlock();
	mPowBlock->SetPos(Vector2(8 * TILESIZE, 8 * TILESIZE));
	mPowBlock->SetParent(mLevel);
}

MarioManager::~MarioManager() {
	mGraphics = NULL;
	mCamera = NULL;

	delete mBackground;
	mBackground = NULL;

	delete mLevel;
	mLevel = NULL;

	for (int i = 0; i < 2; i++) {
		delete mPlayers[i];
		mPlayers[i] = NULL;

		delete mKoopas[i];
		mKoopas[i] = NULL;
	}

	delete mPowBlock;
	mPowBlock = NULL;
}

void MarioManager::Tick(unsigned int deltaTime) {
	// Exit condition
	if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_ESCAPE)) {
		mCamera->SetCentre(GameManager::Instance()->GetMenuState()->GetMainMenu()->GetPos(GameObject::WORLD));
		GameManager::Instance()->GetMenuState()->SetMenu(MenuManager::MAIN);
		GameManager::Instance()->SetGameState(GameManager::MENU);
		delete this;
		return;
	}

	mKoopaRespawnTimer += deltaTime;
	if (mKoopas[0]->IsDead() && mKoopas[1]->IsDead() && mKoopaRespawnTimer >= mKoopaRespawnTimeMillis) {
		mKoopaRespawnTimer = 0;
		for (int i = 0; i < 2; i++) {
			mKoopas[i]->SetFlipped(false);
			mKoopas[i]->SetDead(false, NONE);
			mKoopas[i]->SetPos(Vector2(i * ((mLevel->GetWidth() * TILESIZE) - 32), TILESIZE));
		}
	}

	// Game Objects
	mBackground->Tick(deltaTime);
	mLevel->Tick(deltaTime);
	mPowBlock->Tick(deltaTime);
	for (int i = 0; i < 2; i++) {
		mPlayers[i]->Tick(deltaTime);
		mKoopas[i]->Tick(deltaTime);
		CheckLoss(mPlayers[i]);
		CheckPowBlock(mPlayers[i]);
	}

	DoScreenshake(deltaTime);
}

void MarioManager::Render(SDL_Renderer* renderer) {
	mBackground->Render(renderer);
	mPowBlock->Render(renderer);
	for (int i = 0; i < 2; i++) {
		mKoopas[i]->Render(renderer);
		if (!mPlayers[i]->IsDead())
			mPlayers[i]->Render(renderer);
	}
}

bool MarioManager::CheckLoss(Player* player) {
	if (player->GetPos(GameObject::WORLD).y > mLevel->GetHeight() * TILESIZE) {
		player->SetDead(true, FALL);
		return true;
	}
	for (int i = 0; i < 2; i++) {
		if (CollisionManager::Instance()->Check(player, mKoopas[i], CollisionManager::BOX)) {
			if (mKoopas[i]->IsFlipped()) {
				mKoopas[i]->SetDead(true, ATTACK);
			} else {
				player->SetDead(true, ATTACK);
			}
		}
	}
	return false;
}

void MarioManager::CheckPowBlock(Player* player) {
	if (mScreenshaking) return;

	if (!mPowBlock->IsDepleted()) {
		if (CollisionManager::Instance()->Check(player, mPowBlock, CollisionManager::BOX)) {
			player->SetJumping(false);
			player->AddVelocity(player->GetVelocity() * -1.0f);
			player->AddAcceleration(player->GetAcceleration() * -1.0f);

			mPowBlock->RegisterHit();
			for (int i = 0; i < 2; i++) {
				mKoopas[i]->SetFlipped(true);
			}
			
			Graphics* graphics = Graphics::Instance();
			mCameraStartPos = mCamera->GetOffset() + (Vector2(graphics->mScreenWidth, graphics->mScreenHeight) * 0.5f);
			mScreenshaking = true;
		}
	}
}

void MarioManager::DoScreenshake(unsigned int deltaTime) {
	if (!mScreenshaking) return;

	mScreenshakeTimer += deltaTime;
	if (mScreenshakeTimer > mScreenshakeTimeMillis) {
		mScreenshaking = false;
		mScreenshakeTimer = 0;
		mWobble = 0.0f;
		mCamera->SetCentre(mCameraStartPos);
		return;
	}

	mWobble++;
	Vector2 shake(0.0f, 3 * sin(mWobble));
	mCamera->SetCentre(mCameraStartPos + shake);
}