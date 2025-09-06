#pragma once

/// <summary>
/// フェーダ
/// </summary>
class Fader
{
public:

    // 処理状態
    enum class FADE_STATE
    {
        NONE = 0,
        FADE_OUT, // フェードアウト
        LOADING,   // フェード時のロード
        FADE_IN,  // フェードイン
    };


    // 透明度の変動量
    static constexpr float SPEED_ALPHA = 5.0f;

    // 読み込み時間
    static constexpr float LOAD_TIME = 0.5f;

    // フェードアウト時の透明度
    static constexpr int FADE_OUT_MAX = 255;

    /// <summary>
    /// デフォルトコンストラクタ
    /// </summary>
    Fader(void);

    /// <summary>
    /// デフォルトデストラクタ
    /// </summary>
    ~Fader(void) = default;

    ///<sammary>
    /// 初期化処理
    ///</sammary>
    void Init(void);

    ///<sammary>
    ///更新処理
    ///</sammary>
    void Update(void);

    ///<sammary>
    ///描画処理
    ///</sammary>
    void Draw(void);

    /// <sammary>
    /// 指定のフェードを割り当て
    ///.<param>フェードの状態</param>
    /// </sammary>
    void SetFade(Fader::FADE_STATE state);

    /// <summary>
    /// フェードが完了したか否か
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    bool GetFadeEnd(void) const;

    /// <summary>
    /// フェード状態を取得
    /// </summary>
    /// <returns>現在のフェード状態</returns>
    Fader::FADE_STATE GetState(void) const;


private:

    FADE_STATE state_; // フェード状態
    float alpha_; // 透明度
    float loadTime_; // ロード時間

    bool isPreEnd_; // 状態遷移完了したか否か
    bool isEnd_; // フェード終了したか否か
};