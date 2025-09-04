#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <map>

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

	// 敵保存データ
	enum class DATA_ENEMY
	{
		NAME,            // 敵名

		HP,              // HP
		POWER,           // 攻撃力
		SPEED,			 // 移動速度
		ATTACK_INTERVAL, // 攻撃間隔
		ATTACK_RANEGE,   // 攻撃範囲
		SEARCH_RANGE,    // 索敵範囲
		TIME_INVINCIBLE, // 無敵時間
		ANIM_SPEED_IDLE, // 待機アニメーション速度
		ANIM_SPEED_ATTACK, // 攻撃アニメーション速度

		MAX,
	};

	// 武器データ
	enum class DATA_WEAPON
	{
		NAME,

		POWER,      // 攻撃力
		DIFENCE,    // 防御力
		SPEED,      // 移動速度
		DURABILITY,  // 耐久
		PARRY_TIME, // パリィ有効時間

		ANIM_SPEED_ATTACK, // 攻撃アニメーション速度

		MAX,
	};


	// セーブ容量 + ラベル
	static constexpr int SAVE_LENGTH = (2 + 1); 

	//セーブデータのハンドル
	const char* DATA_HANDLE = "../../../Source/Data/SaveData.csv";


	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	static StatusData* GetInstance(void);

	/// <summary>
	/// インスタンス削除処理
	/// </summary>
	static void Destroy(void);

	/// <summary>
	/// 描画処理(デバッグ)
	/// </summary>
	void DrawDebug(void);


	/// <summary>
	/// .csvファイル読み込み処理
	/// </summary>
	/// <param name = "_filePath">ファイルパス</param name>
	/// <returns>正常動作できたか否か</returns>
	bool LoadCSV(const char* _filePath);

	/// <summary>
	/// .csvファイル書き出し処理
	/// </summary>
	/// <returns>正常動作できたか否か</returns>
	bool SaveCSV(void);

	/// <summary>
	/// 数値取得処理
	/// </summary>
	/// <param name="load">読み込む対象</param>
	/// <param name="type">読み込む種類</param>
	float GetScore(int load, DATA_PLAYER type);

	/// <summary>
	/// 数値保存処理
	/// </summary>
	/// <param name="save">保存する対象</param>
	/// <param name="type">保存する種類</param>
	/// <param name="saveValue">保存する値</param>
	void SetScore(int save, STATUS_TYPE type, float saveValue);


private:

	// インスタンス
	static StatusData* instance_;

	// プレイヤーパラメータ
	struct PLAYER
	{
	private:

		std::string startName = "";
		int power = 0;
		int luck = 0;
		int weaponId = 0;
		float speed = 0.0f;
		float timeParry = 0.0f;
		float timeEvasion = 0.0f;
		float timeInv = 0.0f;

	public:

		void LoadParam(std::string& _startName, int _power, int _luck,
			           int _weaponId, float _speed, float _timeParry,
			           float _timeEvasion, float _timeInv)
		{
			startName = _startName;
			power = _power;
			luck  = _luck;
			weaponId = _weaponId;
			speed     = _speed;
			timeParry = _timeParry;
			timeEvasion = _timeEvasion;
			timeInv     = _timeInv;
		}

		std::string& GetStartName(void) { return startName; };
		int GetPower(void) { return power; };
		int GetLuck(void)  { return luck; };
		int GetWeaponId(void) { return weaponId; };
		float GetSpeed(void)  { return speed; };
		float GetTimeParry(void) { return timeParry; };
		float GetTimeEvasion(void) { return timeEvasion; };
		float GetTimeInvicible(void) { return timeInv; };
	};
	std::unique_ptr<PLAYER> player_;

	// 敵パラメータ
	struct ENEMY
	{
	private:

		std::string name = "";
		int hp = 0;
		int power = 0;
		float speed = 0.0f;
		float atkInterval = 0.0f;
		float atkRange = 0.0f;
		float animSpeedIdle = 0.0f;
		float animSpeedAtk = 0.0f;

	public:

		/// <summary>
		/// 数値ロード処理
		/// </summary>
		/// <param name="_name"></param>
		/// <param name="_hp"></param>
		/// <param name="_power"></param>
		/// <param name="_speed"></param>
		/// <param name="_atkInterval"></param>
		/// <param name="_atkRange"></param>
		/// <param name="_animSpeedIdle"></param>
		/// <param name="_animSpeedAtk"></param>
		void LoadParam(std::string& _name, int& _hp, int& _power,
			           float& _speed, float& _atkInterval, float& _atkRange,
			           float& _animSpeedIdle, float& _animSpeedAtk)
		{
			name = _name;
			hp   = _hp;
			power = _power;
			speed = _speed;
			atkInterval = _atkInterval;
			atkRange    = _atkRange;
			animSpeedIdle = _animSpeedIdle;
			animSpeedAtk  = _animSpeedAtk;
		};

		std::string GetName(void) { return name; }
		int GetMaxHp(void) { return hp; }
		int GetPower(void) { return power; }
		float GetSpeed(void) { return speed; }
		float GetAtkInterval(void) { return atkInterval; }
		float GetAtkRange(void) { return atkRange; }
		float GetAnimSpeedIdle(void) { return animSpeedIdle; }
		float GetAnimSpeedAtk(void) { return animSpeedAtk; }
	};
	std::map<int, std::unique_ptr<ENEMY>> enemy_;

	// 武器パラメータ
	struct WEAPON
	{
	private:
		std::string name = "";
		int power = 0;
		int difence = 0;
		float speed = 1.0f;
		int durability = 0;
		float attackRange = 0.0f;
		float parryTime = 0.0f;
		float animSpeedAtk = 0.0f;

	public:

		void LoadParam(std::string& _name, int _power, int _difence,
			           float _speed, int _durability, float _attackRange,
			           float _parryTime, float _animSpeedAtk)
		{
			name  = _name;
			power = _power;
			difence = _difence;
			speed   = _speed;
			durability  = _durability;
			attackRange = _attackRange;
			parryTime    = _parryTime;
			animSpeedAtk = _animSpeedAtk;
		};
			           
		std::string GetName(void) { return name; };
		int GetPower(void) { return power; };
		int GetDifence(void) { return difence; };
		float GetSpeed(void) { return speed; };
		int GetDurability(void) { return durability; };
		float GetAttackRange(void) { return attackRange; };
		float GetParryTime(void) { return parryTime; };
		float GetAnimSpeedAtk(void) { return animSpeedAtk; };
	};
	std::map<int, std::unique_ptr<WEAPON>> weapon_;


	//ロードする値
	std::string loadString_[SAVE_LENGTH][static_cast<int>(DATA_PLAYER::MAX)];

	
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