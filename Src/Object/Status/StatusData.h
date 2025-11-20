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


	/// @brief インスタンス生成処理
	static void CreateInstance(void);

	/// @brief インスタンス取得処理
	static StatusData& GetInstance(void);

	/// @brief インスタンス削除処理
	static void Destroy(void);

	/// @brief 初回読み込み処理
	void Load(void);

	/// @brief .csvファイル書き出し処理
	/// @returns 正常動作できたか否か
	void SaveCSV(void);

	
	/// @brief プレイヤーステータス参照
	StatusPlayer& GetPlayerStatus(void)const { return *player_; };

	/// @brief 敵ステータス参照
	StatusEnemy& GetEnemyStatus(int target)const { return *enemy_.at(target - 1).get(); };

	/// @brief 敵ステータスマップ参照
	const EnemyMap& GetEnemyStatusMap(void)const { return enemy_; };

	/// @brief 武器ステータス参照
	const StatusWeapon& GetWeaponStatus(int target)const { return *weapon_.at(target).get(); };

	/// @brief 武器ステータスマップ参照
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

	
	/// @brief デフォルトコンストラクタ
	StatusData(void);

	/// @brief デフォルトデストラクタ
	~StatusData(void) = default;

	// コピーコンストラクタ対策
	StatusData(const StatusData&) = delete;
	StatusData& operator=(const StatusData&) = delete;
	StatusData(StatusData&&) = delete;
	StatusData& operator=(StatusData&&) = delete;

	// 各csvファイル読み込み
	void LoadPlayerStatus(void);
	void LoadPlayerMotion(void);
	void LoadEnemy(void);
	void LoadWeapon(void);
};