#include <string>
#include <memory>
#include <map>

class StatusPlayer;
class StatusEnemy;
class StatusWeapon;


class StatusData
{
public:

	// ステータスの種類
	enum class STATUS_TYPE
	{
		PLAYER,
		ENEMY,
		WEAPON,

		MAX,
	};

	// プレイヤー保存データ
	enum class DATA_PLAYER
	{
		POWER, // 攻撃力
		LUCK,  // 幸運
		SPEED, // 移動速度
		WEAPON_NUM,      // 武器番号
		TIME_INVINCIBLE, // 無敵時間
		TIME_PARRY,		 // パリィ時間
		TIME_EVASION,    // 回避時間
		ATTACK_RAMGE,    // 攻撃範囲

		MAX,
	};


	// CSVファイルパス
	const std::string PATH_CSV_FILE = "Data/CSV/";

	// セーブデータのハンドル
	const char* PATH_PLAYER = "PlayerData.csv";
	const char* PATH_ENEMY  = "EnemyData.csv";
	const char* PATH_WEAPON = "WeaponData.csv";


	/// <summary>
	/// インスタンス生成処理
	/// </summary>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	static StatusData* GetInstance(void);

	/// <summary>
	/// インスタンス削除処理
	/// </summary>
	static void Destroy(void);

	/// <summary>
	/// 初回読み込み処理
	/// </summary>
	void Load(void);

	/// <summary>
	/// 描画処理(デバッグ)
	/// </summary>
	void DrawDebug(void);

	/// <summary>
	/// .csvファイル書き出し処理
	/// </summary>
	/// <returns>正常動作できたか否か</returns>
	void SaveCSV(void);


private:

	// インスタンス
	static StatusData* instance_;

	// プレイヤーパラメータ
	std::unique_ptr<StatusPlayer> player_;

	// 敵パラメータ
	std::map<int, std::unique_ptr<StatusEnemy>> enemy_;

	// 武器パラメータ
	std::map<int, std::unique_ptr<StatusWeapon>> weapon_;

	
	// デフォルトコンストラクタ
	StatusData(void);

	// デストラクタ
	~StatusData(void) = default;

	// コピーコンストラクタ
	StatusData(const StatusData& other) = default;

	void LoadPlayer(void);
	void LoadEnemy(void);
	void LoadWeapon(void);
};