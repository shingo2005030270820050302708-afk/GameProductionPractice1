#pragma once


enum class SceneType {
    Title,
    StageSelect,
    Play,
    Setting,
    GameClear,
    GameOver,
};

class SceneBase {
public:
    virtual ~SceneBase() = default;

    virtual void Init() = 0;
    virtual void Load() = 0;
    virtual void Start() = 0;
    virtual void Step() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Fin() = 0;

    virtual bool IsEnd() const = 0;
    virtual SceneType NextScene() const = 0;
};
//