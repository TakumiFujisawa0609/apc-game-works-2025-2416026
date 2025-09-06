
#include <Dxlib.h>
#include "../Manager/SceneManager.h" 
#include "../Application.h"
#include "Fader.h"

Fader::Fader(void)
{

}


void Fader::Init(void)
{
    state_ = FADE_STATE::NONE;
    alpha_ = 0.0f;
    loadTime_ = 0.0f;
    isPreEnd_ = false;
    isEnd_ = false;
}

void Fader::Update(void)
{
    // フェード終了時 処理終了
    if (!isEnd_) return;

    switch (state_)
    {
    case FADE_STATE::FADE_OUT:
    {
        alpha_ += SPEED_ALPHA;
        if (alpha_ > FADE_OUT_MAX)
        {
            alpha_ = FADE_OUT_MAX;

            // 描画後の更新処理で終了
            if (!isPreEnd_) isEnd_ = true;

            isPreEnd_ = true;
        }
    }
    break;

    case FADE_STATE::FADE_IN:
    {
        alpha_ -= SPEED_ALPHA;
        if (alpha_ < 0)
        {
            alpha_ = 0;

            // 描画後の更新処理で終了
            if (!isPreEnd_) isEnd_ = true;

            isPreEnd_ = true;
        }
    }
    break;

    case FADE_STATE::LOADING:
    {
        // 読み込み時間増加
        loadTime_ += SceneManager::GetInstance().GetDeltaTime();

        // 読み込み時間を終了
        if (loadTime_ >= LOAD_TIME)
        {
            loadTime_ = 0.0f;
        }
    }
    break;


    default:
        return;
    }
}

void Fader::Draw(void)
{
    // 状態なし時 処理終了
    if (state_ == FADE_STATE::NONE) return;

    // フェード画像追加
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));
    DrawBox(0, 0,
        Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
        0x0, true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void Fader::SetFade(Fader::FADE_STATE state)
{
    state_ = state;

    if (state_ == FADE_STATE::NONE) return;

    isPreEnd_ = false;
    isEnd_ = false;

    if (state_ == FADE_STATE::FADE_OUT)
    {
        alpha_ = 0;
    }
    else if (state == FADE_STATE::FADE_IN)
    {
        alpha_ = FADE_OUT_MAX;
    }
}

bool Fader::GetFadeEnd(void) const
{
    return isEnd_;
}

Fader::FADE_STATE Fader::GetState(void) const
{
    return state_;
}