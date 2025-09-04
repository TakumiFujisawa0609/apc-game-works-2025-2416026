#pragma once
class Vector2
{
public:

	int x; // XÀ•W
	int y; // YÀ•W

	// ƒfƒXƒgƒ‰ƒNƒ^
	~Vector2(void) = default;

#pragma region l‘¥‰‰Z
	/// <summary>
	/// ‰ÁZˆ—
	/// </summary>
	/// <param name="vec">‚Q‚Â–Ú‚ÌVector2</param>
	/// <returns>‘«‚µ‚½’l</returns>
	Vector2 operator+(const Vector2& vec)const;

	/// <summary>
	/// ‰ÁZˆ—
	/// </summary>
	/// <param name="vec">‚Q‚Â–Ú‚ÌVector2</param>
	void operator+=(const Vector2& vec);

	/// <summary>
	/// Œ¸Zˆ—
	/// </summary>
	/// <param name="vec">‚Q‚Â–Ú‚ÌVector2</param>
	/// <returns>ˆø‚¢‚½’l</returns>
	Vector2 operator-(const Vector2& vec)const;

	/// <summary>
	/// Œ¸Zˆ—
	/// </summary>
	/// <param name="vec">‚Q‚Â–Ú‚ÌVector2</param>
	void operator-=(const Vector2& vec);

	/// <summary>
	/// æZˆ—
	/// </summary>
	/// <param name="vec">‚Q‚Â–Ú‚ÌVector2</param>
	/// <returns>Š|‚¯‚½’l</returns>
	Vector2 operator*(const Vector2& vec)const;

	/// <summary>
	/// æZˆ—
	/// </summary>
	/// <param name="vec">‚Q‚Â–Ú‚ÌVector2</param>
	void operator*=(const Vector2& vec);

	/// <summary>
	/// œZˆ—
	/// </summary>
	/// <param name="vec">‚Q‚Â–Ú‚ÌVector2</param>
	/// <returns>‘«‚µ‚½’l</returns>
	Vector2 operator/(const Vector2& vec)const;

	/// <summary>
	/// œZˆ—
	/// </summary>
	/// <param name="vec">‚Q‚Â–Ú‚ÌVector2</param>
	void operator/=(const Vector2& vec);

#pragma endregion
};