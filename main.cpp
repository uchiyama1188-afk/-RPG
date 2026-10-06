//#include <iostream>
//
//int main() {
//	std::cout << "RPG" << std::endl;
//}
//
////　プレイヤークラス
//class Player
//{
//public:
//    std::string Name;
//    int HP;
//
//    Player() : Player("Default", 100) {}
//
//    Player(std::string name, int hp)
//        : Name(name), HP(hp)
//    {
//    }
//};
//
///// <summary>
///// ゲームマネージャー
///// </summary>
//class GameManager {
//private:
//    GameManager() {}
//
//public:
//
//    GameManager(const GameManager&) = delete;
//    GameManager& operator=(const GameManager&) = delete;
//
//    static GameManager& Instance() {
//        static GameManager instance; // C++11以降のスレッドセーフなローカルstatic
//        return instance;
//    }
//
//    void StartGame() {
//        // ゲーム開始処理
//    }
//};



#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

/////////////////////////////////////////////////
// FSM
/////////////////////////////////////////////////

enum class GameState
{
    Start,
    Battle,
    Victory,
    GameOver
};

/////////////////////////////////////////////////
// 敵データテーブル
/////////////////////////////////////////////////

struct EnemyData
{
    int id;
    string name;
    int hp;
    int atk;
};

EnemyData enemyTable[] =
{
    {1, "スライム", 30, 5},
    {2, "ゴブリン", 50, 10},
    {3, "オーク", 80, 15}
};

/////////////////////////////////////////////////
// オブジェクトプール
/////////////////////////////////////////////////

class Effect
{
public:

    bool active;

    Effect()
    {
        active = false;
    }

    void Play()
    {
        active = true;
        cout << "★攻撃エフェクト発生★" << endl;
        active = false;
    }
};

class EffectPool
{
private:

    vector<Effect> effects;

public:

    EffectPool(int size)
    {
        effects.resize(size);
    }

    Effect* GetEffect()
    {
        for (auto& effect : effects)
        {
            if (!effect.active)
            {
                return &effect;
            }
        }

        return nullptr;
    }
};

/////////////////////////////////////////////////
// プレイヤー
/////////////////////////////////////////////////

class Player
{
public:

    string name;
    int hp;
    int atk;

    Player(string n, int h, int a)
        : name(n), hp(h), atk(a)
    {
    }
};

/////////////////////////////////////////////////
// 敵
/////////////////////////////////////////////////

class Enemy
{
public:

    string name;
    int hp;
    int atk;

    Enemy(const EnemyData& data)
        : name(data.name),
        hp(data.hp),
        atk(data.atk)
    {
    }
};

/////////////////////////////////////////////////
// Factory
/////////////////////////////////////////////////

class EnemyFactory
{
public:

    static Enemy CreateEnemy(int id)
    {
        for (auto data : enemyTable)
        {
            if (data.id == id)
            {
                return Enemy(data);
            }
        }

        return Enemy(enemyTable[0]);
    }
};

/////////////////////////////////////////////////
// Singleton
/////////////////////////////////////////////////

class GameManager
{
private:

    GameManager() {}

public:

    static GameManager& Instance()
    {
        static GameManager instance;
        return instance;
    }

    GameState state = GameState::Start;

    void Run()
    {
        srand((unsigned)time(nullptr));

        Player player("勇者", 100, 20);

        int enemyId = rand() % 3 + 1;

        Enemy enemy =
            EnemyFactory::CreateEnemy(enemyId);

        EffectPool pool(3);

        while (true)
        {
            switch (state)
            {
            case GameState::Start:

                cout << "===================" << endl;
                cout << enemy.name
                    << " が現れた！" << endl;
                cout << "===================" << endl;

                state = GameState::Battle;
                break;

            case GameState::Battle:
            {
                cout << endl;

                cout << "勇者HP : "
                    << player.hp << endl;

                cout << enemy.name
                    << " HP : "
                    << enemy.hp << endl;

                cout << endl;

                cout << "1.攻撃" << endl;

                int cmd;
                cin >> cmd;

                if (cmd == 1)
                {
                    Effect* effect =
                        pool.GetEffect();

                    if (effect)
                    {
                        effect->Play();
                    }

                    enemy.hp -= player.atk;

                    cout
                        << "勇者の攻撃！"
                        << endl;

                    if (enemy.hp <= 0)
                    {
                        state = GameState::Victory;
                        break;
                    }

                    player.hp -= enemy.atk;

                    cout
                        << enemy.name
                        << " の反撃！"
                        << endl;

                    if (player.hp <= 0)
                    {
                        state = GameState::GameOver;
                    }
                }

                break;
            }

            case GameState::Victory:

                cout << endl;
                cout << "勝利！！" << endl;
                return;

            case GameState::GameOver:

                cout << endl;
                cout << "ゲームオーバー..."
                    << endl;
                return;
            }
        }
    }
};

/////////////////////////////////////////////////
// Main
/////////////////////////////////////////////////

int main()
{
    GameManager::Instance().Run();

    return 0;
}
