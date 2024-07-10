#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"ImGuiManager.h"
#include"PrimitiveDrawer.h"
#include"AxisIndicator.h"
GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete sprite_; 
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("./Resources/mario.png");
	soundDataHandle_=audio_->LoadWave("fanfare.wav");
	//スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {100.50});
	//3Dモデルの生成
	model_=Model::Create();
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//音声再生
	audio_->PlayWave(soundDataHandle_);
	voiceHandle_=audio_->PlayWave(soundDataHandle_,true);
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	debugCamera_=new DebugCamera(1280,720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {
	Vector2 position =sprite_->GetPosition();
	///座標を{2,1}移動
	position.x+=2.0f;
	position.y+=1.0f;
	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);
	//スペース押した瞬間
	if(input_->TriggerKey(DIK_SPACE)){
	//音声停止
		audio_->StopWave(voiceHandle_);
	}
	//デバックテキストの表示
#ifdef _DEBUG


	InGui::Begin("Debug1");
	ImGui::Text("Kamata Tarou %d,%d",2050,12,31);
	ImGui:End();
	}
	ImGui::InputFloat3("InputFloat3",inputFloat3);
	ImGui::InputFloat3("InputFloat3",inputFloat3,0.0f,1.0f);
	ImGui::ShowDemoWindow();
#endif // DEBUG

	debugCamera_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	///3Dモデル
	model_->Draw(worldTransform_,viewProjection_,textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
	PrimitiveDrawer::GetInstance()->DrawLine3d({0,0,0},{0,10,0},{1.0f,0.0f,0.0f,1.0f});
model_->Draw(worldTransform_,debugCamera_->GetViewProjection(),textureHandle_);
}
	
