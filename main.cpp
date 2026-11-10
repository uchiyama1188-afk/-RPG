
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
                cout << enemy.name<< " が現れた！" << endl;
                cout << "===================" << endl;

                state = GameState::Battle;
                break;

            case GameState::Battle:
            {
                cout << endl;

                cout << "勇者HP : "<< player.hp << endl;

                cout << enemy.name<< " HP : "<< enemy.hp << endl;

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

                    cout<< "勇者の攻撃！"<< endl;

                    if (enemy.hp <= 0)
                    {
                        state = GameState::Victory;
                        break;
                    }

                    player.hp -= enemy.atk;

                    cout<< enemy.name<< " の反撃！"<< endl;

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
                cout << "ゲームオーバー..."<< endl;
                return;
            }
        }
    }
};


class GameView
{
public:

    void ShowEnemyAppear(const Enemy& enemy)
    {
        cout << "===================" << endl;
        cout << enemy.name << " が現れた！" << endl;
        cout << "===================" << endl;
    }

    void ShowStatus(
        const Player& player,
        const Enemy& enemy)
    {
        cout << endl;

        cout << "勇者HP : "
            << player.hp
            << endl;

        cout << enemy.name
            << " HP : "
            << enemy.hp
            << endl;
    }

    void ShowMenu()
    {
        cout << endl;
        cout << "1.攻撃" << endl;
    }

    void ShowPlayerAttack()
    {
        cout << "勇者の攻撃！" << endl;
    }

    void ShowEnemyAttack(
        const string& name)
    {
        cout << name
            << " の反撃！"
            << endl;
    }

    void ShowVictory()
    {
        cout << "勝利！！" << endl;
    }

    void ShowGameOver()
    {
        cout << "ゲームオーバー..." << endl;
    }
};

class GameController
{
private:

    Player player;
    Enemy enemy;

    GameView view;
    EffectPool pool;

    GameState state;

public:

    GameController()
        : player("勇者", 100, 20),
        enemy(
            EnemyFactory::CreateEnemy(
                rand() % 3 + 1)),
        pool(3)
    {
        state = GameState::Start;
    }

    void Run()
    {
        while (true)
        {
            switch (state)
            {
            case GameState::Start:

                view.ShowEnemyAppear(enemy);

                state = GameState::Battle;

                break;

            case GameState::Battle:

                Battle();

                break;

            case GameState::Victory:

                view.ShowVictory();

                return;

            case GameState::GameOver:

                view.ShowGameOver();

                return;
            }
        }
    }

private:

    void Battle()
    {
        view.ShowStatus(
            player,
            enemy);

        view.ShowMenu();

        int cmd;
        cin >> cmd;

        if (cmd != 1)
        {
            return;
        }

        Effect* effect =
            pool.GetEffect();

        if (effect)
        {
            effect->Play();
        }

        enemy.hp -= player.atk;

        view.ShowPlayerAttack();

        if (enemy.hp <= 0)
        {
            state = GameState::Victory;
            return;
        }

        player.hp -= enemy.atk;

        view.ShowEnemyAttack(
            enemy.name);

        if (player.hp <= 0)
        {
            state = GameState::GameOver;
        }
    }
};

/////////////////////////////////////////////////
// Main
/////////////////////////////////////////////////
int main()
{
    srand((unsigned)time(nullptr));

    GameController game;

    game.Run();

    return 0;
}