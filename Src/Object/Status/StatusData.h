#include <string>
#include <memory>
#include <array>
#include <map>

class StatusPlayer;
class StatusEnemy;
class StatusWeapon;


class StatusData
{
public:

	// 敵パラメータマップ
	using EnemyMap = std::map<int, std::unique_ptr<StatusEnemy>>;

	// 武器パラメータマップ
	using WeaponMap = std::map<int, std::unique_ptr<StatusWeapon>>;

	// ステータスの種類
	enum class STATUS_TYPE
	{
		PLAYER,
		ENEMY,
		WEAPON,

		MAX,
	};

	// CSVファイルパス
	const std::string PATH_CSV_FILE = "Data/CSV/";

	// セーブデータのハンドル
	const char* PATH_PLAYER = "PlayerData.csv";
	const char* PATH_PLAYER_MOTION = "PlayerMotionData.csv";
	const char* PATH_ENEMY = "EnemyData.csv";
	const char* PATH_WEAPON = "WeaponData.csv";


	/// <summary>
	/// インスタンス生成処理
	/// </summary>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	static StatusData& GetInstance(void);

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

	
	/// <summary>
	/// プレイヤーステータス参照
	/// </summary>
	StatusPlayer& GetPlayerStatus(void)const { return *player_; };

	/// <summary>
	/// 敵ステータス参照
	/// </summary>
	StatusEnemy& GetEnemyStatus(int target)const { return *enemy_.at(target - 1).get(); };

	/// <summary>
	/// 敵ステータスマップ参照
	/// </summary>
	const EnemyMap& GetEnemyStatusMap(void)const { return enemy_; };

	/// <summary>
	/// 武器ステータス参照
	/// </summary>
	const StatusWeapon& GetWeaponStatus(int target)const { return *weapon_.at(target).get(); };

	/// <summary>
	/// ブキステータスマップ参照
	/// </summary>
	const WeaponMap& GetWeaponStatus(void)const { return weapon_; };

	
	std::string& GetHandlePathPlayer(void);
	std::string& GetHandlePathEnemy(int type);
	std::string& GetHandlePathWeapon(int type);

private:

	// インスタンス
	static StatusData* instance_;

	// プレイヤーパラメータ
	std::unique_ptr<StatusPlayer> player_;
	
	// 敵パラメータ
	
	EnemyMap enemy_;

	// 武器パラメータ
	std::map<int, std::unique_ptr<StatusWeapon>> weapon_;

	
	// デフォルトコンストラクタ
	StatusData(void);

	// デストラクタ
	~StatusData(void) = default;

	// コピーコンストラクタ
	StatusData(const StatusData& other) = default;

	void LoadPlayerStatus(void);
	void LoadPlayerMotion(void);
	void LoadEnemy(void);
	void LoadWeapon(void);
};